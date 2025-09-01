#ifndef PLUGIN_11_H
#define PLUGIN_11_H

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include <Plugins/pluginClass.h>

// TeensyDAW: begin automatically generated code
// Name: 1Osc
// Description: Soundfont Synthesizer
// Voices: 1

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

class Plugin_11 : public PluginControll
{
public:
    AudioSynthWaveformDc dc;
    AudioSynthWaveform LFO;
    AudioSynthWaveform Lfo2Vco;
    AudioMixer2 fEnvMixer;
    AudioSynthWaveformDc PWMdc;
    AudioMixer2 PWMmix;
    AudioSynthWaveformPWM waveform;
    AudioEffectEnvelope Fenv;
    AudioFilterStateVariable filter;
    AudioMixer4 fMixer;
    AudioEffectEnvelope Aenv;


    AudioConnection *patchCord[14]; // total patchCordCount:98 including array typed ones.

    // constructor (this is called when class-object is created)
    Plugin_11(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords

        patchCord[pci++] = new AudioConnection(PWMdc, 0, PWMmix, 0);
        patchCord[pci++] = new AudioConnection(Lfo2Vco, 0, PWMmix, 1);
        patchCord[pci++] = new AudioConnection(PWMmix, 0, waveform, 0);
        patchCord[pci++] = new AudioConnection(dc, 0, Fenv, 0);

        patchCord[pci++] = new AudioConnection(Fenv, 0, fEnvMixer, 0);
        patchCord[pci++] = new AudioConnection(LFO, 0, fEnvMixer, 1);
        patchCord[pci++] = new AudioConnection(fEnvMixer, 0, filter, 1);
        patchCord[pci++] = new AudioConnection(waveform, 0, filter, 0);

        patchCord[pci++] = new AudioConnection(filter, 0, fMixer, 0);
        patchCord[pci++] = new AudioConnection(filter, 1, fMixer, 1);
        patchCord[pci++] = new AudioConnection(filter, 2, fMixer, 2);
        patchCord[pci++] = new AudioConnection(fMixer, 0, Aenv, 0);

        patchCord[pci++] = new AudioConnection(Aenv, 0, MixGain, 0);
        patchCord[pci++] = new AudioConnection(MixGain, 0, performFilter, 0);
    }
    virtual ~Plugin_11() = default;

    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;
    virtual void assign_parameter(uint8_t pot) override;

};
#endif // PLUGIN_11_H

extern Plugin_11 plugin_11;
