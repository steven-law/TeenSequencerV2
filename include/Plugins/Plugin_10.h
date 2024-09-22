#ifndef PLUGIN_10_H
#define PLUGIN_10_H

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include <Plugins/pluginClass.h>
#include <Plugins/AudioSamples/AudioSamples.h>
#include "ownLibs/filter_ladderlite.h"
////#include "hardware/tftClass.h"
//class tftClass;
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
extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
class Plugin_10 : public PluginControll
{
public:
    AudioSynthWaveformDc dc;
    AudioSynthWavetable waveform;
    AudioEffectEnvelope Fenv;
    AudioFilterStateVariable filter;
    AudioMixer4 fMixer;
    AudioEffectEnvelope Aenv;
    // AudioMixer12 mixer;
    AudioAmplifier MixGain;
    AudioAmplifier SongVol;
    AudioConnection *patchCord[9]; // total patchCordCount:98 including array typed ones.

    // constructor (this is called when class-object is created)
    Plugin_10(const char *Name, byte ID) : PluginControll(Name, ID)
    {

        int pci = 0; // used only for adding new patchcords

        // patchCord[pci++] = new AudioConnection(mixer, 0, MixGain, 0);
        patchCord[pci++] = new AudioConnection(dc, 0, Fenv, 0);
        patchCord[pci++] = new AudioConnection(waveform, 0, filter, 0);
        patchCord[pci++] = new AudioConnection(Fenv, 0, filter, 1);
        patchCord[pci++] = new AudioConnection(filter, 0, fMixer, 0);
        patchCord[pci++] = new AudioConnection(filter, 1, fMixer, 1);
        patchCord[pci++] = new AudioConnection(filter, 2, fMixer, 2);
        patchCord[pci++] = new AudioConnection(fMixer, 0, Aenv, 0);
        patchCord[pci++] = new AudioConnection(Aenv, 0, MixGain, 0);
        patchCord[pci++] = new AudioConnection(MixGain, 0, SongVol, 0);
    }
    virtual ~Plugin_10() = default;

    virtual void setup() override;
    virtual void noteOn(byte notePlayed, float velocity, byte voice) override;
    virtual void noteOff(byte notePlayed, byte voice) override;

    virtual void set_parameters(byte row) override;
    virtual void draw_plugin() override;
    virtual void change_preset() override;

    void set_voice_waveform(byte XPos, byte YPos, const char *name); // make virtual in baseclass
    void set_voice_amplitude(byte XPos, byte YPos, const char *name);

    void set_filter_frequency(byte XPos, byte YPos, const char *name);
    void set_filter_resonance(byte XPos, byte YPos, const char *name, float min, float max);
    void set_filter_sweep(byte XPos, byte YPos, const char *name);
    void set_filter_type(byte XPos, byte YPos, const char *name);
    void selectFilterType(byte mixerchannel);

    void set_envelope_ADSR(byte YPos, int maxA, int maxD, int maxR);
    void set_envelope_attack(byte XPos, byte YPos, const char *name, int max);
    void set_envelope_decay(byte XPos, byte YPos, const char *name, int max);
    void set_envelope_sustain(byte XPos, byte YPos, const char *name);
    void set_envelope_release(byte XPos, byte YPos, const char *name, int max);

    void assign_voice_waveform(byte value); // make virtual in baseclass but override
    void assign_voice_amplitude(byte value);

    void assign_filter_frequency(byte value);
    void assign_filter_resonance(byte value);
    void assign_filter_sweep(byte value);

    void assign_envelope_attack(byte value, int max);
    void assign_envelope_decay(byte value, int max);
    void assign_envelope_sustain(byte value);
    void assign_envelope_release(byte value, int max);
};
#endif // PLUGIN_10_H

extern Plugin_10 plugin_10;
