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
//class tftClass;
extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
//void clearWorkSpace();

void Plugin_1::setup()
{
    for (int i = 0; i < MAX_VOICES; i++)
    {
        mixer.gain(i, 1);
    }
    MixGain.gain(1);
    //SongVol.gain(1);
}
void Plugin_1::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    float frequency = note_frequency[notePlayed] * tuning;
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

            set_mixer_gain(0, 0, "Vol");
            set_mixer_gain(1, 0, "Vol");
            set_mixer_gain(2, 0, "Vol");
            set_mixer_gain(3, 0, "Vol");
        }

        if (row == 1)
        {

            set_mixer_gain(0, 1, "Vol");
            set_mixer_gain(1, 1, "Vol");
        }

        if (row == 2)
        {
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
void Plugin_1::draw_plugin()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        clearWorkSpace();
        // Serial.println("drawing plugin 1");
        drawPot(0, 0, potentiometer[presetNr][0], "Vol");
        drawPot(1, 0, potentiometer[presetNr][1], "Vol");
        drawPot(2, 0, potentiometer[presetNr][2], "Vol");
        drawPot(3, 0, potentiometer[presetNr][3], "Vol");

        drawPot(0, 1, potentiometer[presetNr][4], "Vol");
        drawPot(1, 1, potentiometer[presetNr][5], "Vol");
        drawPot(2, 1, potentiometer[presetNr][6], "Vol");
        drawPot(3, 1, potentiometer[presetNr][7], "Vol");

        drawPot(0, 2, potentiometer[presetNr][8], "Vol");
        drawPot(1, 2, potentiometer[presetNr][9], "Vol");
        drawPot(2, 2, potentiometer[presetNr][10], "Vol");
        drawPot(3, 2, potentiometer[presetNr][11], "Vol");

        //draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "Prset", presetNr, 3, 0);
    }
}
void Plugin_1::change_preset(){
    for (int i=0;i<MAX_VOICES;i++){
        assign_mixer_gain(potentiometer[presetNr][i], i);
    }
}

void Plugin_1::set_mixer_gain(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        int n = XPos + (YPos * NUM_ENCODERS);
        assign_mixer_gain(get_Potentiometer(XPos, YPos, name), n);
    }
}
void Plugin_1::assign_mixer_gain(uint8_t value, uint8_t channel)
{
    float sustain = value / MIDI_CC_RANGE_FLOAT;
    mixer.gain(channel, sustain);
}
Plugin_1 plugin_1("Strg", 1);
// TeensyDAW: end automatically generated code