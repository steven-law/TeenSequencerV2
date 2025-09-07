#ifndef PLUGIN_8_H
#define PLUGIN_8_H

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "ownLibs/filter_ladderlite.h"
#include "projectVariables.h"

#include <Plugins/pluginClass.h>
////#include "hardware/tftClass.h"
// class tftClass;
//  TeensyDAW: begin automatically generated code
//  Name: dTune
//  Description: 2VCO Detuned Subtractive Synthesizer
//  Voices: 12

// VCO 1
// Pot 1: Waveform
// Pot 2: Detune
// Pot 3: Vol
// Pot 4:

// VCO 2
// Pot 5: Waveform
// Pot 6: Detune
// Pot 7: Vol
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
class Plugin_8 : public PluginControll
{
public:
    float detune1;
    float detune2;
    AudioSynthWaveform waveform;
    AudioSynthWaveform waveform1;
    AudioSynthWaveformDc dc;
    AudioMixer4 vcoMixer;
    AudioEffectEnvelope Fenv;
    AudioFilterStateVariable filter;
    AudioFilterLadderLite ladder;
    AudioMixer4 fMixer;
    AudioEffectEnvelope Aenv;


    AudioConnection *patchCord[13]; // total patchCordCount:122 including array typed ones.

    // constructor (this is called when class-object is created)
    Plugin_8(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords
        patchCord[pci++] = new AudioConnection(waveform, 0, vcoMixer, 0);
        patchCord[pci++] = new AudioConnection(waveform1, 0, vcoMixer, 1);
        patchCord[pci++] = new AudioConnection(dc, 0, Fenv, 0);
        patchCord[pci++] = new AudioConnection(vcoMixer, 0, filter, 0);

        patchCord[pci++] = new AudioConnection(vcoMixer, 0, ladder, 0);
        patchCord[pci++] = new AudioConnection(Fenv, 0, filter, 1);
        patchCord[pci++] = new AudioConnection(filter, 0, fMixer, 0);
        patchCord[pci++] = new AudioConnection(filter, 1, fMixer, 1);

        patchCord[pci++] = new AudioConnection(filter, 2, fMixer, 2);
        patchCord[pci++] = new AudioConnection(ladder, 0, fMixer, 3);
        patchCord[pci++] = new AudioConnection(fMixer, 0, Aenv, 0);
        patchCord[pci++] = new AudioConnection(Aenv, 0, MixGain, 0);

         patchCord[pci++] = new AudioConnection(MixGain, 0, performFilter, 0);
        // patchCord[pci++] = new AudioConnection(SongVol, 0, dacOut, 0);
    }
    virtual ~Plugin_8() = default;
    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;
    virtual void assign_parameter(uint8_t pot) override;

};
#endif // PLUGIN_8_H
extern Plugin_8 plugin_8;
