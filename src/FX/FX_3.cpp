#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "projectVariables.h"
#include <FX/FX_3.h>

void FX_3::setup()
{
    FX_mixer.gain(0, 1);
    FX_mixer.gain(1, 1);
    FX_mixer.gain(2, 1);
    FX_mixer.gain(3, 1);
    FX_mixer.gain(4, 1);
    setFXParameterNames("Time", MIDI_CC_RANGE, "Feedback", MIDI_CC_RANGE);
}
void FX_3::noteOn(uint8_t notePlayed, float velocity, uint8_t voice) {}
void FX_3::noteOff(uint8_t notePlayed, uint8_t voice) {}
void FX_3::assign_parameter(uint8_t pot)
{
    switch (pot)
    {
    case 0:
    {
        int time = map(get_Potentiometer(0), 0, 127, 3, 500);
        delay.delay(0, time);
    }
    break;
    case 1:
    {
        float gain = get_Potentiometer(1) / MIDI_CC_RANGE_FLOAT;
        delayMixer.gain(1, gain);
    }
    break;
    default:
        break;
    }
}
