#ifndef PLUGIN_6_H
#define PLUGIN_6_H

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include <Plugins/pluginClass.h>
////#include "hardware/tftClass.h"
// class tftClass;
#define PL6_VOICES 4
// TeensyDAW: begin automatically generated code
// Name: Draw
// Description: Additive Synthesizer
// Voices: 1

// VCO
// Pot 1: Vol
// Pot 2: Vol
// Pot 3: Vol
// Pot 4: Vol

// Pot 5: Vol
// Pot 6: Vol
// Pot 7: Vol
// Pot 8: Vol

// Filter:
// Pot 9: Frequency
// Pot 10: Resonance
// Pot 11: Sweep
// Pot 12: Type

// Envelope:
// Pot 9: Attack
// Pot 10: Decay
// Pot 11: Sustain
// Pot 12: Release


extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
class Plugin_6 : public PluginControll
{
public:
    AudioSynthWaveformDc dc;
    AudioSynthWaveform waveform[PL6_VOICES];
    AudioMixer4 mixer;
    AudioEffectEnvelope Fenv;
    AudioFilterStateVariable filter;
    AudioMixer4 fMixer;
    AudioEffectEnvelope Aenv;



    // AudioAmplifier SongVol;
    AudioConnection *patchCord[10]; // total patchCordCount:98 including array typed ones.
    int8_t pl6ChoosenChord;
    // constructor (this is called when class-object is created)
    Plugin_6(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {

        int pci = 0; // used only for adding new patchcords
        patchCord[pci++] = new AudioConnection(waveform[0], 0, mixer, 0);
        patchCord[pci++] = new AudioConnection(waveform[1], 0, mixer, 1);
        patchCord[pci++] = new AudioConnection(waveform[2], 0, mixer, 2);
        patchCord[pci++] = new AudioConnection(waveform[3], 0, mixer, 3);

        patchCord[pci++] = new AudioConnection(mixer, 0, filter, 0);
        patchCord[pci++] = new AudioConnection(dc, 0, Fenv, 0);
        patchCord[pci++] = new AudioConnection(Fenv, 0, filter, 1);
        patchCord[pci++] = new AudioConnection(filter, 0, Aenv, 0);

        patchCord[pci++] = new AudioConnection(Aenv, 0, MixGain, 0);
        patchCord[pci++] = new AudioConnection(MixGain, 0, performFilter, 0);
    }
    virtual ~Plugin_6() = default;
    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;
    virtual void assign_parameter(uint8_t pot) override;
};
#endif // PLUGIN_6_H

extern Plugin_6 plugin_6;
