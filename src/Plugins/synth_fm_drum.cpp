/* Audio Library for Teensy, FM Drum
 * Copyright (c) 2023, Matt Kuebrich / mxmxmx
 * 
 * This is a tweaked version of mxmxmx's Teensy port of the FM Drum mode from the Mutable Instruments Peaks eurorack module.
 * https://github.com/mxmxmx/minitrash/blob/master/soft/libraries/Audio/synth_fm_drum.cpp
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

// Copyright 2013 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// Sine FM drum - similar to the BD/SD in Anushri.

#include <Arduino.h> // for precompiled header speedup
#include "ownLibs/synth_fm_drum.h"
#include "utility/dspinst.h"

// drum_data.c
extern "C" {
  extern const uint32_t lut_env_increments[257];
  extern const int16_t wav_overdrive[1025];
  extern const int16_t wav_sine[1025];
  extern const uint16_t lut_env_expo[257];
  extern const uint32_t lut_oscillator_increments[101];
}

static const uint16_t kHighestNote = 128 * 128;
static const uint16_t kPitchTableStart = 116 * 128;
static const uint16_t kOctave = 128 * 12;
static const uint8_t kNumZones = 6;

void AudioSynthFMDrum::init(void)
{
  phase_ = 0;
  fm_envelope_phase_ = 0xffffffff;
  am_envelope_phase_ = 0xffffffff;
  previous_sample_ = 0;
}

void AudioSynthFMDrum::noteOn(void)
{
  __disable_irq();
  noteOnTrig = 1;
  __enable_irq();
}

uint32_t AudioSynthFMDrum::ComputeEnvelopeIncrement(uint16_t decay)
{
  uint32_t a = lut_env_increments[decay >> 8];
  uint32_t b = lut_env_increments[(decay >> 8) + 1];
  return a - ((a - b) * (decay & 0xff) >> 8);
}


uint32_t AudioSynthFMDrum::ComputePhaseIncrement(float hz) {
uint32_t phase_increment = (hz * (0x7fffffffLL/AUDIO_SAMPLE_RATE_EXACT*2)) + 0.5f;
return phase_increment;
}

inline uint16_t AudioSynthFMDrum::Interpolate824(const uint16_t* table, uint32_t phase) {
  uint32_t a = table[phase >> 24];
  uint32_t b = table[(phase >> 24) + 1];
  return a + ((b - a) * static_cast<uint32_t>((phase >> 8) & 0xffff) >> 16);
}


inline int16_t AudioSynthFMDrum::Interpolate1022(const int16_t* table, uint32_t phase) {
  int32_t a = table[phase >> 22];
  int32_t b = table[(phase >> 22) + 1];
  return a + ((b - a) * static_cast<int32_t>((phase >> 6) & 0xffff) >> 16);
}

inline int16_t AudioSynthFMDrum::Mix(int16_t a, int16_t b, uint16_t balance) {
  return (a * (65535 - balance) + b * balance) >> 16;
}


void AudioSynthFMDrum::update(void)
{

  audio_block_t *block, *trigInput;
  trigInput = receiveReadOnly();
  block = allocate();

  if (!block) return;

  uint32_t am_envelope_increment = ComputeEnvelopeIncrement(am_decay_);
  uint32_t fm_envelope_increment = ComputeEnvelopeIncrement(fm_decay_);
  uint32_t phase = phase_;
  uint32_t fm_envelope_phase = fm_envelope_phase_;
  uint32_t am_envelope_phase = am_envelope_phase_;
  uint32_t aux_envelope_phase = aux_envelope_phase_;
  uint32_t phase_increment = phase_increment_;

  for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {

    if (noteOnTrig){
      fm_envelope_phase = 0;
      am_envelope_phase = 0;
      phase = 0x3fff * fm_amount_ >> 16;
}
    if (trigInput) {
      clkin = trigInput->data[i];
      release(trigInput);
    } else {
      clkin = 0;
      // if no trigInput, the trigger equals, the noteOn();
      trig = noteOnTrig;
      noteOnTrig = 0;
    }

    if (clkin > triggerThreshold) {
      trig = 1;
    } else {
      trig = 0;
    }

    // this ensures the note is triggered from the trigInput or noteOn()
    if ((!trig && _prev_clk) || (noteOnTrig != prevnoteOnTrig)) {
      fm_envelope_phase = 0;
      am_envelope_phase = 0;
      phase = 0x3fff * fm_amount_ >> 16;
    }

    _prev_clk = trig;
    prevnoteOnTrig = noteOnTrig;

    fm_envelope_phase += fm_envelope_increment;

    if (fm_envelope_phase < fm_envelope_increment) fm_envelope_phase = 0xffffffff;

    if ((i & 3) == 0) {

      uint32_t fm_envelope = 65535 - Interpolate824(lut_env_expo, fm_envelope_phase);
      phase_increment = ComputePhaseIncrement(frequency_ + (fm_envelope * fm_amount_ >> 16));                          
    }
    
    phase += phase_increment;

    int16_t mix = Interpolate1022(wav_sine, phase);

    if (noise_) mix = Mix(mix, random(65335), noise_);

    am_envelope_phase += am_envelope_increment;

    if (am_envelope_phase < am_envelope_increment) am_envelope_phase = 0xffffffff;

    uint32_t am_envelope = 65535 - Interpolate824(lut_env_expo, am_envelope_phase);

    mix = mix * am_envelope >> 16;

    if (overdrive_) {

      uint32_t phi = (static_cast<int32_t>(mix) << 16) + (1L << 31);
      int16_t overdriven = Interpolate1022(wav_overdrive, phi);
      mix = Mix(mix, overdriven, overdrive_);
    }

    previous_sample_ = mix;
    block->data[i] = mix;
  }

  phase_ = phase;
  fm_envelope_phase_ = fm_envelope_phase;
  am_envelope_phase_ = am_envelope_phase;
  aux_envelope_phase_ = aux_envelope_phase;
  phase_increment_ = phase_increment;
  noteOnTrig = 0; // reset trig for noteOn to work
  transmit(block);
  release(block);
}