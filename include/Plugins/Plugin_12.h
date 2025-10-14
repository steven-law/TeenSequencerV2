#ifndef PLUGIN_12_H
#define PLUGIN_12_H

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include <Plugins/pluginClass.h>
#include "ownLibs/filter_ladderlite.h"
#include "TeensyVariablePlayback.h"
////#include "hardware/tftClass.h"
// class tftClass;
//  TeensyDAW: begin automatically generated code
//  Name: 1Osc
//  Description: Subtractive Synthesizer
//  Voices: 12

// VCO
// Pot 1: Waveform
// Pot 2: Vol
// Pot 3:
// Pot 4:

// Pot 5:
// Pot 6:
// Pot 7:
// Pot 8:

// Filter:
// Pot 9: Frequency
// Pot 10: Resonance
// Pot 11: Sweep
// Pot 12: Type

// Envelope:
// Pot 13: Attack
// Pot 14: Decay
// Pot 15: Sustain
// Pot 16: Release


extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;

class Plugin_12 : public PluginControll
{
public:
    AudioSynthWaveformDc dc;
    AudioPlaySdResmp waveform;
    AudioEffectEnvelope Fenv;
    AudioFilterStateVariable filter;
    AudioMixer4 fMixer;
    AudioEffectEnvelope Aenv;


    AudioConnection *patchCord[9]; // total patchCordCount:98 including array typed ones.
    char *_filename = "C0.RAW";
    // constructor (this is called when class-object is created)
    Plugin_12(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords

        patchCord[pci++] = new AudioConnection(dc, 0, Fenv, 0);
        patchCord[pci++] = new AudioConnection(waveform, 0, filter, 0);
        patchCord[pci++] = new AudioConnection(Fenv, 0, filter, 1);
        patchCord[pci++] = new AudioConnection(filter, 0, fMixer, 0);
        patchCord[pci++] = new AudioConnection(filter, 1, fMixer, 1);

        patchCord[pci++] = new AudioConnection(filter, 2, fMixer, 2);
        patchCord[pci++] = new AudioConnection(fMixer, 0, Aenv, 0);
        patchCord[pci++] = new AudioConnection(Aenv, 0, MixGain, 0);
        patchCord[pci++] = new AudioConnection(MixGain, 0, performFilter, 0);
    }
    virtual ~Plugin_12() = default;

    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;
    virtual void assign_parameter(uint8_t pot) override;

};

#endif // PLUGIN_2_H
extern Plugin_12 plugin_12;
