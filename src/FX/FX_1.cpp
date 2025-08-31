
#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "projectVariables.h"
#include <FX/FX_1.h>

void FX_1::setup()
{
    FX_mixer.gain(0, 1);
    FX_mixer.gain(1, 1);
    FX_mixer.gain(2, 1);
    FX_mixer.gain(3, 1);
    FX_mixer.gain(4, 1);
    setFXParameterNames("Roomsize", "Damping");
}
void FX_1::noteOn(uint8_t notePlayed, float velocity, uint8_t voice) {}
void FX_1::noteOff(uint8_t notePlayed, uint8_t voice) {}
void FX_1::assign_parameter(uint8_t pot)
{
    switch (pot)
    {
    case 0:
 {
        float size = get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT;
        freeverb.roomsize(size);
    }
        break;
    case 1:
{
        float damp = get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT;
        freeverb.damping(damp);
    }
        break;
    default:
        break;
    }
}
void FX_1::set_gain(uint8_t gain)
{
    // MixGain.gain(gain/ MIDI_CC_RANGE_FLOAT);
}

