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

#ifndef synth_fm_drum_h_
#define synth_fm_drum_h_
#include <Arduino.h>
#include "AudioStream.h"

class AudioSynthFMDrum : public AudioStream
{
  public:
    AudioSynthFMDrum() : AudioStream(1, inputQueueArray), magnitude(16384) {
      frequency_ = 50.0f;
      fm_amount_ = 0;
      am_decay_ = 24575; // 0.75
      fm_decay_ = 12287; // 0.75
      noise_ = 0;
      overdrive_ = 0;
    }

    void noteOn();

    void frequency(float hz) {
      frequency_ = hz;
    }

    void fm(float _fm_a) {
      uint16_t fm_a = (uint16_t)(_fm_a * 65535);
      fm_amount_ = fm_a >> 2;
    }

    void decay(float decay) {
      uint16_t d = (uint16_t)(decay * 65535);
      am_decay_ = 16384 + (d >> 1);
      fm_decay_ = 8192 + (d >> 2);
    }

    void noise(float n) {
      noise_ = (uint32_t)(n * 32767);
    }

    void overdrive(float o) {
      overdrive_ = (uint32_t)(o * o * 32767);
    }

    virtual void update(void);
    void init(void);

  private:
    inline uint16_t Interpolate824(const uint16_t* table, uint32_t phase);
    inline int16_t Interpolate1022(const int16_t* table, uint32_t phase);
    inline int16_t Mix(int16_t a, int16_t b, uint16_t balance);
    uint32_t ComputeEnvelopeIncrement(uint16_t decay);
    uint32_t ComputePhaseIncrement(float hz);
    audio_block_t *inputQueueArray[1];
    uint8_t sd_range_;
    int32_t magnitude;
    uint8_t _clk;
    uint8_t _prev_clk;
    float frequency_;
    uint16_t fm_amount_;
    uint16_t am_decay_;
    uint16_t fm_decay_;
    uint32_t noise_;
    uint32_t overdrive_;
    uint16_t aux_envelope_strength_;
    int16_t  previous_sample_;
    uint32_t phase_;
    uint32_t fm_envelope_phase_;
    uint32_t am_envelope_phase_;
    uint32_t aux_envelope_phase_;
    uint32_t phase_increment_;
    int16_t clkin;
    uint8_t trig = 0;
    uint8_t noteOnTrig;
    uint8_t prevnoteOnTrig;
    int16_t triggerThreshold = 0.2 * 32767.0f; ; //a typical 2 "volts" threshold, if we consider the whole range a 10v p2p signal
};

#endif