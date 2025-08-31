
#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "projectVariables.h"
#include <FX/FX_2.h>

void FX_2::setup()
{
    FX_mixer.gain(0, 1);
    FX_mixer.gain(1, 1);
    FX_mixer.gain(2, 1);
    FX_mixer.gain(3, 1);
    FX_mixer.gain(4, 1);
    setFXParameterNames("BitDepth", "SmpRate");
}
void FX_2::noteOn(uint8_t notePlayed, float velocity, uint8_t voice) {}
void FX_2::noteOff(uint8_t notePlayed, uint8_t voice) {}
void FX_2::assign_parameter(uint8_t pot)
{
       switch (pot)
    {
    case 0:
 {
         bitcrusher.bits(map(get_Potentiometer(pot), 0, 127, 1, 16));
    }
        break;
    case 1:
{
        bitcrusher.sampleRate(map(get_Potentiometer(pot), 0, 127, 1, 44100));
    }
        break;
    default:
        break;
    }
}
void FX_2::set_gain(uint8_t gain)
{
    // MixGain.gain(gain/ MIDI_CC_RANGE_FLOAT);
}
