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
}
void FX_3::noteOn(byte notePlayed, float velocity, byte voice) {}
void FX_3::noteOff(byte notePlayed, byte voice) {}
void FX_3::set_parameters(byte row)
{
    draw_plugin();
    if (row == 0)
    {
        set_DL_time(0, 0, "Time");
        set_DL_feedback(1, 0, "Feedback");
    }
}
void FX_3::draw_plugin()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, potentiometer[presetNr][0], "Time");
        drawPot(1, 0, potentiometer[presetNr][1], "Feedback");
    }
}

void FX_3::set_DL_time(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        int time = map(get_Potentiometer(XPos, YPos, name), 0, 127, 3, 500);
        delay.delay(0, time);
    }
}
void FX_3::set_DL_feedback(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float gain = get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT;
        delayMixer.gain(1, gain);
    }
}
void FX_3::change_preset()
{
    int time = map(potentiometer[presetNr][0], 0, 127, 3, 500);
    delay.delay(0, time);
    float gain = potentiometer[presetNr][1] / MIDI_CC_RANGE_FLOAT;
    delayMixer.gain(1, gain);
}