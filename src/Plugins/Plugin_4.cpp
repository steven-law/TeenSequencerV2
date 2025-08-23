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

void Plugin_4::setup()
{

    for (int i = 0; i < PL4_VOICES; i++)
    {
        playMem[i].begin();
        playMem[i].enableInterpolation(false);
        AEnv[i].attack(0);
        AEnv[i].decay(0);
        AEnv[i].sustain(1);
        AEnv[i].release(100);
        mixer.gain(i, 1);
    }
    MixGain.gain(1);
    for (int i = 0; i < PL4_VOICES; i++)
    {
        potentiometer[presetNr][i] = 10;
        potentiometer[presetNr][i + 8] = 127;
    }
    setParameterNames("Bk K", "Bk C", "Bk H", "Bk S", "Bk P", "Bk F", "Bk X", "Bk *", "Vol", "Vol", "Vol", "Vol", "Vol", "Vol", "Vol", "Vol");

    // change_preset();
    // SongVol.gain(1);
}
void Plugin_4::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{

    AEnv[voice].sustain(velocity);
    playMem[voice].playRaw(sample[voice]->sampledata, sample[voice]->samplesize, 1);
    AEnv[voice].noteOn();
    // playMem[voice].play(_fileName[voice]);
    //  playMem[voice].playRaw(sample[voice]->sampledata,1);

    // Serial.printf("pl4 play %s on voice %d\n", _fileName[voice], voice);
    Serial.printf("pl4 play %s on voice %d, velocity: %02f\n", _fileName[voice], voice, velocity);
}
void Plugin_4::noteOff(uint8_t notePlayed, uint8_t voice)
{
    AEnv[voice].noteOff();
    // Serial.printf("pl4 Note Off voice: %d\n", voice);
}
void Plugin_4::set_parameters(uint8_t row)
{
    draw_plugin();
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        if (row == 0)
        {
            set_waveform(0, 0);
            set_waveform(1, 0);
            set_waveform(2, 0);
            set_waveform(3, 0);
        }

        if (row == 1)
        {
            set_waveform(0, 1);
            set_waveform(1, 1);
            set_waveform(2, 1);
            set_waveform(3, 1);
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
            set_mixer_gain(0, 3);
            set_mixer_gain(1, 3);
            set_mixer_gain(2, 3);
            set_mixer_gain(3, 3);
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        set_presetNr();
    }
}

void Plugin_4::set_gain(uint8_t gain)
{
    float velo = ((gain / MIDI_CC_RANGE_FLOAT));
    MixGain.gain(velo);
}
void Plugin_4::set_mixer_gain(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        int n = XPos + (YPos * NUM_ENCODERS);
        assign_mixer_gain(get_Potentiometer(XPos, YPos), n);
    }
}
void Plugin_4::assign_mixer_gain(uint8_t value, uint8_t channel)
{
    float sustain = value / MIDI_CC_RANGE_FLOAT;
    mixer.gain(channel - 8, sustain);
}
void Plugin_4::change_preset()
{
    for (int i = 0; i < PL4_VOICES; i++)
    {
        assign_mixer_gain(potentiometer[presetNr][i + 8], i);
        assign_waveform(potentiometer[presetNr][i], i);
    }
}
void Plugin_4::set_waveform(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        int n = XPos + (YPos * NUM_ENCODERS);
        assign_waveform(get_Potentiometer(XPos, YPos), n);
        enc_moved[XPos] = false;
    }
}
void Plugin_4::assign_waveform(uint8_t value, uint8_t channel)
{

    sprintf(_fileName[channel], "%s%d.raw", bankNames[channel], value);
    newdigate::flashloader loader;
    sample[channel] = loader.loadSample(_fileName[channel]);
}
Plugin_4 plugin_4("mDrm", 4);

// TeensyDAW: end automatically generated code