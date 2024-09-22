/* This is a modified variant of the mixer code
 * to make it possible to autogenerate mixers with any size
 * by using the Design Tool++ by Manicksan (Jannik Svensson)
 * 
 * Original copyright note:
 * Audio Library for Teensy 3.X
 * Copyright (c) 2014, Paul Stoffregen, paul@pjrc.com
 *
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
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

#ifndef mixers_h_
#define mixers_h_

#include "Arduino.h"
#include "AudioStream.h"

#if defined(__ARM_ARCH_7EM__)
#define MIXERS_MAX_MULT_I 65536
#define MIXERS_MAX_MULT_F 65536.0f
#define MIXERS_MIN_GAIN -32767.0f
#define MIXERS_MAX_GAIN 32767.0f
#define MIXERS_MULT_TYPE int32_t
#elif defined(KINETISL)
#define MIXERS_MAX_MULT_I 256
#define MIXERS_MAX_MULT_F 256.0f
#define MIXERS_MIN_GAIN -127.0f
#define MIXERS_MAX_GAIN 127.0f
#define MIXERS_MULT_TYPE int16_t
#endif

class AudioMixer5 : public AudioStream
{
public:
	AudioMixer5(void) : AudioStream(5, inputQueueArray) {
		for (int i=0; i<5; i++) multiplier[i] = MIXERS_MAX_MULT_I;
	}
	virtual void update(void);
	void gain(unsigned int channel, float gain) {
		if (channel >= 5) return;
		if (gain > MIXERS_MAX_GAIN) gain = MIXERS_MAX_GAIN;
		else if (gain < MIXERS_MIN_GAIN) gain = MIXERS_MIN_GAIN;
		multiplier[channel] = gain * MIXERS_MAX_MULT_F; // TODO: proper roundoff?
	}
	void gain(float gain) {
	    if (gain > MIXERS_MAX_GAIN) gain = MIXERS_MAX_GAIN;
		else if (gain < MIXERS_MIN_GAIN) gain = MIXERS_MIN_GAIN;
		for (int i=0; i<5; i++) multiplier[i] = gain * MIXERS_MAX_MULT_F;
	}
private:
	MIXERS_MULT_TYPE multiplier[5];
	audio_block_t *inputQueueArray[5];
};
class AudioMixer12 : public AudioStream
{
public:
	AudioMixer12(void) : AudioStream(12, inputQueueArray) {
		for (int i=0; i<12; i++) multiplier[i] = MIXERS_MAX_MULT_I;
	}
	virtual void update(void);
	void gain(unsigned int channel, float gain) {
		if (channel >= 12) return;
		if (gain > MIXERS_MAX_GAIN) gain = MIXERS_MAX_GAIN;
		else if (gain < MIXERS_MIN_GAIN) gain = MIXERS_MIN_GAIN;
		multiplier[channel] = gain * MIXERS_MAX_MULT_F; // TODO: proper roundoff?
	}
	void gain(float gain) {
	    if (gain > MIXERS_MAX_GAIN) gain = MIXERS_MAX_GAIN;
		else if (gain < MIXERS_MIN_GAIN) gain = MIXERS_MIN_GAIN;
		for (int i=0; i<12; i++) multiplier[i] = gain * MIXERS_MAX_MULT_F;
	}
private:
	MIXERS_MULT_TYPE multiplier[12];
	audio_block_t *inputQueueArray[12];
};
class AudioMixer2 : public AudioStream
{
public:
	AudioMixer2(void) : AudioStream(2, inputQueueArray) {
		for (int i=0; i<2; i++) multiplier[i] = MIXERS_MAX_MULT_I;
	}
	virtual void update(void);
	void gain(unsigned int channel, float gain) {
		if (channel >= 2) return;
		if (gain > MIXERS_MAX_GAIN) gain = MIXERS_MAX_GAIN;
		else if (gain < MIXERS_MIN_GAIN) gain = MIXERS_MIN_GAIN;
		multiplier[channel] = gain * MIXERS_MAX_MULT_F; // TODO: proper roundoff?
	}
	void gain(float gain) {
	    if (gain > MIXERS_MAX_GAIN) gain = MIXERS_MAX_GAIN;
		else if (gain < MIXERS_MIN_GAIN) gain = MIXERS_MIN_GAIN;
		for (int i=0; i<2; i++) multiplier[i] = gain * MIXERS_MAX_MULT_F;
	}
private:
	MIXERS_MULT_TYPE multiplier[2];
	audio_block_t *inputQueueArray[2];
};


#endif