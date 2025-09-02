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
    IamPoly = true;
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

void Plugin_4::assign_parameter(uint8_t pot)
{

    if (pot < 8)
    {
        sprintf(_fileName[pot], "%s%d.raw", bankNames[pot], get_Potentiometer(pot));
        newdigate::flashloader loader;
        sample[pot] = loader.loadSample(_fileName[pot]);
    }
    else
    {
        float sustain = get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT;
        mixer.gain(pot - 8, sustain);
    }
}

Plugin_4 plugin_4("mDrm", 4);

// TeensyDAW: end automatically generated code