/*
This is a more CPU-friendly version of the ladder filter which uses 2x oversampling (instead of 4x).
It uses the changes Richard van Hoesel outlined here:
https://forum.pjrc.com/index.php?threads/porting-moog-ladder-filters-to-audio-objects.60488/post-272591
*/

/* Audio Library for Teensy, Ladder Filter
 * Copyright (c) 2021, Richard van Hoesel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

//-----------------------------------------------------------
// Huovilainen New Moog (HNM) model as per CMJ jun 2006
// Implemented as Teensy Audio Library compatible object
// Richard van Hoesel, Feb. 9 2021
// v1.5 adds polyphase FIR or Linear interpolation
// v1.4 FC extended to 18.7kHz, max res to 1.8, 4x oversampling,
//      and a minor Q-tuning adjustment
// v.1.03 adds oversampling, extended resonance,
// and exposes parameters input_drive and passband_gain
// v.1.02 now includes both cutoff and resonance "CV" modulation inputs
// please retain this header if you use this code.
//-----------------------------------------------------------

// https://forum.pjrc.com/threads/60488?p=271078&viewfull=1#post271078

#include <Arduino.h>
#include "ownLibs/filter_ladderlite.h"
#include <math.h>
#include "arm_math.h"
#include <stdint.h>
#define MOOG_PI ((float)3.14159265358979323846264338327950288)

#define MAX_RESONANCE ((float)1.8)
#define MAX_FREQUENCY ((float)(AUDIO_SAMPLE_RATE_EXACT * 0.425f))


float AudioFilterLadderLite ::interpolation_coeffs[AudioFilterLadderLite::interpolation_taps] = {    
-129.6800658538650740E-6, 0.001562843504973615, 0.004914813078250063, 0.007186231102125209, 0.002728024844356039,-0.007973931496234599,-0.013255882861166815,-841.1646993462468340E-6,
 0.022309967341058432, 0.027240071522569135,-0.007073857882406733,-0.056523975383091875,-0.055929860712812876, 0.042278471323346570, 0.207056768528768836, 0.335633514003638445,
 0.335633514003638445, 0.207056768528768836, 0.042278471323346570,-0.055929860712812876,-0.056523975383091875,-0.007073857882406733, 0.027240071522569135, 0.022309967341058432,
-841.1646993462468340E-6,-0.013255882861166815,-0.007973931496234599, 0.002728024844356039, 0.007186231102125209, 0.004914813078250063, 0.001562843504973615,-129.6800658538650740E-6
};

#define I_NUM_SAMPLES  AUDIO_BLOCK_SAMPLES * INTERPOLATION

void AudioFilterLadderLite::initpoly()
{
	if (arm_fir_interpolate_init_f32(&interpolation, INTERPOLATION, interpolation_taps,
	   interpolation_coeffs, interpolation_state, AUDIO_BLOCK_SAMPLES)) {
		polyCapable = false;
		return;
	}
	if (arm_fir_decimate_init_f32(&decimation, interpolation_taps, INTERPOLATION,
	   interpolation_coeffs, decimation_state, I_NUM_SAMPLES)) {
		polyCapable = false;
		return;
	}
	// TODO: should we fill interpolation_state & decimation_state with zeros?
	polyCapable = true;
	polyOn = true;
}

void AudioFilterLadderLite::interpolationMethod(AudioFilterLadderLiteInterpolation imethod)
{
	if (imethod == LADDERLITE_FILTER_INTERPOLATION_FIR_POLY && polyCapable == true) {
		// TODO: if polyOn == false, clear interpolation_state & decimation_state ??
		polyOn = true;
	} else {
		polyOn = false;
	}
}

float AudioFilterLadderLite::LPF(float s, int i)
{
	float ft = s * (1.0f/1.3f) + (0.3f/1.3f) * z0[i] - z1[i];
	ft = ft * alpha + z1[i];
	z1[i] = ft;
	z0[i] = s;
	return ft;
}

void AudioFilterLadderLite::resonance(float res)
{
	// maps resonance = 0->1 to K = 0 -> 4
	if (res > MAX_RESONANCE) {
		res = MAX_RESONANCE;
	} else if (res < 0.0f) {
		res = 0.0f;
	}
	K = 4.0f * res;
}

void AudioFilterLadderLite::frequency(float c)
{
	Fbase = c;
	compute_coeffs(c);
}

void AudioFilterLadderLite::octaveControl(float octaves)
{
	if (octaves > 7.0f) {
		octaves = 7.0f;
	} else if (octaves < 0.0f) {
		octaves = 0.0f;
	}
	octaveScale = octaves / 32768.0f;
}

void AudioFilterLadderLite:: passbandGain(float passbandgain)
{
	pbg = passbandgain;
	if (pbg > 0.5f) pbg = 0.5f;
	if (pbg < 0.0f) pbg = 0.0f;
	inputDrive(host_overdrive);
}

void AudioFilterLadderLite::inputDrive(float odrv)
{
	host_overdrive = odrv;
	if (host_overdrive > 1.0f) {
		if (host_overdrive > 4.0f) host_overdrive = 4.0f;
		// max is 4 when pbg = 0, and 2.5 when pbg is 0.5
		overdrive = 1.0f + (host_overdrive - 1.0f) * (1.0f - pbg);
	} else {
		overdrive = host_overdrive;
		if (overdrive < 0.0f) overdrive = 0.0f;
	}
}

void AudioFilterLadderLite::compute_coeffs(float c)
{
	if (c > MAX_FREQUENCY) {
		c = MAX_FREQUENCY;
	} else if (c < 5.0f) {
		c = 5.0f;
	}
	float wc = c * (float)(2.0f * MOOG_PI / ((float)INTERPOLATION * AUDIO_SAMPLE_RATE_EXACT));
	float wc2 = wc * wc;
	alpha = 0.9892f * wc - 0.4324f * wc2 + 0.1381f * wc * wc2 - 0.0202f * wc2 * wc2;
	//Qadjust = 1.0029f + 0.0526f * wc - 0.0926 * wc2 + 0.0218* wc * wc2;
	Qadjust = 1.006f + 0.0536f * wc - 0.095f * wc2 - 0.05f * wc2 * wc2;
	// revised hfQ (rvh - feb 14 2021)
}

bool AudioFilterLadderLite::resonating()
{
	for (int i=0; i < 4; i++) {
		if (fabsf(z0[i]) > 0.0001f) return true;
		if (fabsf(z1[i]) > 0.0001f) return true;
	}
	return false;
}

static inline float fast_exp2f(float x)
{
	float i;
	float f = modff(x, &i);
	f *= 0.693147f / 256.0f;
	f += 1.0f;
	f *= f;
	f *= f;
	f *= f;
	f *= f;
	f *= f;
	f *= f;
	f *= f;
	f *= f;
	f = ldexpf(f, i);
	return f;
}

static inline float fast_tanh(float x)
{
	if (x > 3.0f) return 1.0f;
	if (x < -3.0f) return -1.0f;
	float x2 = x * x;
	return x * (27.0f + x2) / (27.0f + 9.0f * x2);
}

void AudioFilterLadderLite::update(void)
{
	audio_block_t *blocka, *blockb, *blockc;
	float Ktot = K;
	bool FCmodActive = true;
	bool QmodActive = true;

	blocka = receiveWritable(0);
	blockb = receiveReadOnly(1);
	blockc = receiveReadOnly(2);
	if (!blocka) {
		if (resonating()) {
			// When no data arrives but the filter is still
			// resonating, we must continue computing the filter
			// with zero input to sustain the resonance
			blocka = allocate();
		}
		if (!blocka) {
			if (blockb) release(blockb);
			if (blockc) release(blockc);
			return;
		}
		for (int i=0; i < AUDIO_BLOCK_SAMPLES; i++) {
			blocka->data[i] = 0;
		}
	}
	if (!blockb) {
		FCmodActive = false;
	}
	if (!blockc) {
		QmodActive = false;
	}
	if (polyOn == true) {
		/*----------------------- upsample -------------------------*/
		float blockOS[I_NUM_SAMPLES], blockIn[AUDIO_BLOCK_SAMPLES];
		float blockOutOS[I_NUM_SAMPLES], blockOut[AUDIO_BLOCK_SAMPLES];
		for (int i=0; i < AUDIO_BLOCK_SAMPLES; i++) {
			blockIn[i] = blocka->data[i] * overdrive * (float)INTERPOLATION / 32768.0f;
		}
		arm_fir_interpolate_f32(&interpolation, blockIn, blockOS, AUDIO_BLOCK_SAMPLES);

		for (int i=0; i < AUDIO_BLOCK_SAMPLES; i++) {
			if (FCmodActive) {
				float FCmod = blockb->data[i] * octaveScale;
				float ftot = Fbase * fast_exp2f(FCmod);
				if (ftot > MAX_FREQUENCY) ftot = MAX_FREQUENCY;
				compute_coeffs(ftot);
			}
			if (QmodActive) {
				float Qmod = blockc->data[i] * (1.0f/32768.0f);
				Ktot = K + 4.0f * Qmod;
			}
			if (Ktot > MAX_RESONANCE * 4.0f) {
				Ktot = MAX_RESONANCE * 4.0f;
			} else if (Ktot < 0.0f) {
				Ktot = 0.0f;
			}

    for (int os = 0; os < INTERPOLATION; os++) {
      float input = blockOS[i * INTERPOLATION + os];
      float u = input - (z1[3] - pbg * input) * Ktot * Qadjust;
      u = fast_tanh(u);
      float stage1 = LPF(u, 0);
      float stage2 = LPF(stage1, 1);
      float stage3 = LPF(stage2, 2);
      float stage4 = LPF(stage3, 3);
      blockOutOS[i * INTERPOLATION + os] = stage4;
    }

		}
		arm_fir_decimate_f32(&decimation, blockOutOS, blockOut, I_NUM_SAMPLES);
		for (int i=0; i < AUDIO_BLOCK_SAMPLES; i++) {
			blocka->data[i] = (float)(blockOut[i]) * 32768.0f;
		}
	} else {
		// linear interpolation
		for (int i=0; i < AUDIO_BLOCK_SAMPLES; i++) {
			float input = blocka->data[i] * overdrive * (1.0f/32768.0f);
			if (FCmodActive) {
				float FCmod = blockb->data[i] * octaveScale;
				float ftot = Fbase * fast_exp2f(FCmod);
				if (ftot > MAX_FREQUENCY) ftot = MAX_FREQUENCY;
				compute_coeffs(ftot);
			}
			if (QmodActive) {
				float Qmod = blockc->data[i] * (1.0f/32768.0f);
				Ktot = K + 4.0f * Qmod;
			}
			if (Ktot > MAX_RESONANCE * 4.0f) {
				Ktot = MAX_RESONANCE * 4.0f;
			} else if (Ktot < 0.0f) {
				Ktot = 0.0f;
			}
			float total = 0.0f;
			float interp = 0.0f;
			for (int os = 0; os < INTERPOLATION; os++) {
				float u = (interp * oldinput + (1.0f - interp) * input)
					- (z1[3] - pbg * input) * Ktot * Qadjust;
				u = fast_tanh(u);
				float stage1 = LPF(u, 0);
				float stage2 = LPF(stage1, 1);
				float stage3 = LPF(stage2, 2);
				float stage4 = LPF(stage3, 3);
				total += stage4 * (1.0f / (float)INTERPOLATION);
				interp += (1.0f / (float)INTERPOLATION);
			}
			oldinput = input;
			blocka->data[i] = total * 32768.0f;
		}
	}
	transmit(blocka);
	release(blocka);
	if (blockb) release(blockb);
	if (blockc) release(blockc);
}