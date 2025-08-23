#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "projectVariables.h"

#include <Plugins/Plugin_1.h>
////#include "hardware/tftClass.h"
// class tftClass;
extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;

void Plugin_1::setup()
{
    for (int i = 0; i < MAX_VOICES; i++)
    {
        mixer.gain(i, 1);
        potentiometer[presetNr][i] = 80;
    }
    MixGain.gain(1);
    setParameterNames("Vol1", "Vol2", "Vol3", "Vol4", "Vol5", "Vol6", "Vol7", "Vol8", "Vol9", "Vol10", "Vol11", "Vol12", "0", "0", "0", "0");

    // change_preset();
    // SongVol.gain(1);
}
void Plugin_1::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    float frequency = note_frequency[notePlayed] * tuning;

    mixer.gain(voice, (velocity * (potentiometer[presetNr][voice] / MIDI_CC_RANGE_FLOAT)));
    string[voice].noteOn(frequency, velocity);
    // Serial.printf("ON voice: %d, freq: %f \n", voice, frequency);
}
void Plugin_1::noteOff(uint8_t notePlayed, uint8_t voice)
{
    string[voice].noteOff(0);
    // Serial.printf("OFF voice: %d,  \n", voice);
}
void Plugin_1::set_parameters(uint8_t row)
{
    draw_plugin();
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        if (row == 0)
        {

            set_mixer_gain(0, 0);
            set_mixer_gain(1, 0);
            set_mixer_gain(2, 0);
            set_mixer_gain(3, 0);
        }

        if (row == 1)
        {

            set_mixer_gain(0, 1);
            set_mixer_gain(1, 1);
            set_mixer_gain(2, 1);
            set_mixer_gain(3, 1);
        }

        if (row == 2)
        {
            set_mixer_gain(0, 2);
            set_mixer_gain(1, 2);
            set_mixer_gain(2, 2);
            set_mixer_gain(3, 2);
        }

        if (row == 3)
        {
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        set_presetNr();
    }
}

void Plugin_1::change_preset()
{
    for (int i = 0; i < MAX_VOICES; i++)
    {
        assign_mixer_gain(potentiometer[presetNr][i], i);
    }
}
void Plugin_1::set_gain(uint8_t gain)
{
    MixGain.gain(gain / MIDI_CC_RANGE_FLOAT);
}
void Plugin_1::set_mixer_gain(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        int n = XPos + (YPos * NUM_ENCODERS);
        assign_mixer_gain(get_Potentiometer(XPos, YPos), n);
    }
}
void Plugin_1::assign_mixer_gain(uint8_t value, uint8_t channel)
{
    float sustain = value / MIDI_CC_RANGE_FLOAT;
    mixer.gain(channel, sustain);
}
Plugin_1 plugin_1("Strg", 1);
// TeensyDAW: end automatically generated code