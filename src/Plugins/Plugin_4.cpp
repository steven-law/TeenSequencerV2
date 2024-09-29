#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "projectVariables.h"

#include <Plugins/Plugin_4.h>

extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
void clearWorkSpace();

void Plugin_4::setup()
{

    for (int i = 0; i < MAX_VOICES; i++)
    {
        mixer.gain(i, 1);
    }
    MixGain.gain(1);
    //SongVol.gain(1);
}
void Plugin_4::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    if (voice ==0)
        playMem[0].play(AudioSampleKick);
    if (voice ==1)
        playMem[1].play(AudioSampleClap);
    if (voice ==2)
        playMem[2].play(AudioSampleHihat);
    if (voice ==3)
        playMem[3].play(AudioSampleSnare);
    if (voice ==4)
        playMem[4].play(AudioSampleP2);
    if (voice ==5)
        playMem[5].play(AudioSamplePongblip);
    if (voice ==6)
        playMem[6].play(AudioSampleTomtom);
    if (voice ==6)
        playMem[7].play(AudioSampleCashregister);
}
void Plugin_4::noteOff(uint8_t notePlayed, uint8_t voice)
{
}
void Plugin_4::set_parameters(uint8_t row)
{
    draw_plugin();
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        if (row == 0)
        {
            set_mixer_gain(0, 0, "Kick");
            set_mixer_gain(1, 0, "Clap");
            set_mixer_gain(2, 0, "HHat");
            set_mixer_gain(3, 0, "Snare");
        }

        if (row == 1)
        {
            set_mixer_gain(0, 1, "Tick");
            set_mixer_gain(1, 1, "Pong");
            set_mixer_gain(2, 1, "Tom");
            set_mixer_gain(3, 1, "Cash");
        }

        if (row == 2)
        {
            //set_mixer_gain(0, 2, "Vol");
            //set_mixer_gain(1, 2, "Vol");
            //set_mixer_gain(2, 2, "Vol");
            //set_mixer_gain(3, 2, "Vol");
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
void Plugin_4::draw_plugin()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        clearWorkSpace();

        drawPot(0, 0, potentiometer[presetNr][0], "Kick");
        drawPot(1, 0, potentiometer[presetNr][1], "Clap");
        drawPot(2, 0, potentiometer[presetNr][2], "HHat");
        drawPot(3, 0, potentiometer[presetNr][3], "Snare");

        drawPot(0, 1, potentiometer[presetNr][4], "Tick");
        drawPot(1, 1, potentiometer[presetNr][5], "Pong");
        drawPot(2, 1, potentiometer[presetNr][6], "Tom");
        drawPot(3, 1, potentiometer[presetNr][7], "Cash");

        drawPot(0, 2, potentiometer[presetNr][8], "Vol");
        drawPot(1, 2, potentiometer[presetNr][9], "Vol");
        drawPot(2, 2, potentiometer[presetNr][10], "Vol");
        drawPot(3, 2, potentiometer[presetNr][11], "Vol");

        //draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "Prset", presetNr, 3, 0);
    }
}

void Plugin_4::set_mixer_gain(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        int n = XPos + (YPos * NUM_ENCODERS);
        assign_mixer_gain(get_Potentiometer(XPos, YPos, name),n );
    }
}
void Plugin_4::assign_mixer_gain(uint8_t value, uint8_t channel)
{
    float sustain = value / MIDI_CC_RANGE_FLOAT;
    mixer.gain(channel, sustain);
}
void Plugin_4::change_preset(){
    for (int i=0;i<MAX_VOICES;i++){
        assign_mixer_gain(potentiometer[presetNr][i], i);
    }
}
Plugin_4 plugin_4("mDrm", 4);

// TeensyDAW: end automatically generated code