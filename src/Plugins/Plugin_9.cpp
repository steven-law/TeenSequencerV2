#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "projectVariables.h"

#include <Plugins/Plugin_9.h>

extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
void clearWorkSpace();

void Plugin_9::setup()
{
SD.begin(BUILTIN_SDCARD);
    for (int i = 0; i < MAX_VOICES; i++)
    {
        mixer.gain(i, 1);
    }
    MixGain.gain(1);
    SongVol.gain(1);
}
void Plugin_9::noteOn(byte notePlayed, float velocity, byte voice)
{
    if (voice == 0)
        playSD[0].play("0.raw");
    if (voice == 1)
        playSD[1].play("1.raw");
    if (voice == 2)
        playSD[2].play("2.raw");
    if (voice == 3)
        playSD[3].play("3.raw");
    if (voice == 4)
        playSD[4].play("4.raw");
    if (voice == 5)
        playSD[5].play("5.raw");
    if (voice == 6)
        playSD[6].play("6.raw");
    if (voice == 7)
        playSD[7].play("7.raw");
    if (voice == 8)
        playSD[8].play("8.raw");
    if (voice == 9)
        playSD[9].play("9.raw");
    if (voice == 10)
        playSD[10].play("10.raw");
    // if (voice == 11)
    // playSD[11].play("1.raw");
}
void Plugin_9::noteOff(byte notePlayed, byte voice)
{
}
void Plugin_9::set_parameters(byte row)
{
    draw_plugin();
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        if (row == 0)
        {
            set_mixer_gain(0, 0, "Raw1");
            set_mixer_gain(1, 0, "Raw2");
            set_mixer_gain(2, 0, "Raw3");
            set_mixer_gain(3, 0, "Raw4");
        }

        if (row == 1)
        {
            set_mixer_gain(0, 1, "Raw5");
            set_mixer_gain(1, 1, "Raw6");
            set_mixer_gain(2, 1, "Raw7");
            set_mixer_gain(3, 1, "Raw8");
        }

        if (row == 2)
        {
            set_mixer_gain(0, 2, "Raw9");
            set_mixer_gain(1, 2, "Raw10");
            set_mixer_gain(2, 2, "Raw11");
            //set_mixer_gain(3, 2, "Raw12");
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
void Plugin_9::draw_plugin()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        clearWorkSpace();

        drawPot(0, 0, potentiometer[presetNr][0], "Raw1");
        drawPot(1, 0, potentiometer[presetNr][1], "Raw2");
        drawPot(2, 0, potentiometer[presetNr][2], "Raw3");
        drawPot(3, 0, potentiometer[presetNr][3], "Raw4");

        drawPot(0, 1, potentiometer[presetNr][4], "Raw5");
        drawPot(1, 1, potentiometer[presetNr][5], "Raw6");
        drawPot(2, 1, potentiometer[presetNr][6], "Raw7");
        drawPot(3, 1, potentiometer[presetNr][7], "Raw8");

        drawPot(0, 2, potentiometer[presetNr][8], "Raw9");
        drawPot(1, 2, potentiometer[presetNr][9], "Raw10");
        drawPot(2, 2, potentiometer[presetNr][10], "Raw11");
        //drawPot(3, 2, potentiometer[presetNr][11], "Raw12");

        //draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "Prset", presetNr, 3, 0);
    }
}

void Plugin_9::set_mixer_gain(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        int n = XPos + (YPos * NUM_ENCODERS);
        assign_mixer_gain(get_Potentiometer(XPos, YPos, name), n);
    }
}
void Plugin_9::assign_mixer_gain(byte value, byte channel)
{
    float sustain = value / MIDI_CC_RANGE_FLOAT;
    mixer.gain(channel, sustain);
}
void Plugin_9::change_preset(){
    for (int i=0;i<MAX_VOICES;i++){
        assign_mixer_gain(potentiometer[presetNr][i], i);
    }
}

Plugin_9 plugin_9("rDrm", 9);
// TeensyDAW: end automatically generated code