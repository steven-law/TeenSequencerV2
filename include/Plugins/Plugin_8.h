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
//class tftClass;
// TeensyDAW: begin automatically generated code
// Name: dTune
// Description: 2VCO Detuned Subtractive Synthesizer
// Voices: 12

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
extern bool enc_moved[4];
extern int encoded[4];
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
    AudioAmplifier MixGain;
    AudioAmplifier SongVol;
    AudioConnection *patchCord[13]; // total patchCordCount:122 including array typed ones.

    // constructor (this is called when class-object is created)
    Plugin_8(const char *Name, byte ID) : PluginControll(Name, ID)
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
        patchCord[pci++] = new AudioConnection(MixGain, 0, SongVol, 0);
    }
    virtual ~Plugin_8() = default;
    virtual void setup() override;
    virtual void noteOn(byte notePlayed, float velocity, byte voice) override;
    virtual void noteOff(byte notePlayed, byte voice) override;
    virtual void set_parameters(byte row) override;
    virtual void draw_plugin() override;
    virtual void change_preset() override;
    
    void set_voice_waveform(byte XPos, byte YPos, const char *name);
    void set_voice_amplitude(byte XPos, byte YPos, const char *name);
    void set_voice_detune(byte XPos, byte YPos, const char *name);
    void set_voice1_waveform(byte XPos, byte YPos, const char *name);
    void set_voice1_amplitude(byte XPos, byte YPos, const char *name);
    void set_voice1_detune(byte XPos, byte YPos, const char *name);

    void set_filter_frequency(byte XPos, byte YPos, const char *name);
    void set_filter_resonance(byte XPos, byte YPos, const char *name, float min, float max);
    void set_filter_sweep(byte XPos, byte YPos, const char *name, float min, float max);
    void set_filter_type(byte XPos, byte YPos, const char *name);
    void selectFilterType(byte mixerchannel);

    void set_envelope_attack(byte XPos, byte YPos, const char *name, int min, int max);
    void set_envelope_decay(byte XPos, byte YPos, const char *name, int min, int max);
    void set_envelope_sustain(byte XPos, byte YPos, const char *name);
    void set_envelope_release(byte XPos, byte YPos, const char *name, int min, int max);
    void assign_voice1_waveform(byte value); // make virtual in baseclass but override
    void assign_voice1_amplitude(byte value);
    // void set_voice1_detune(byte XPos, byte YPos, const char *name);
    void assign_voice2_waveform(byte value); // make virtual in baseclass but override
    void assign_voice2_amplitude(byte value);
    void set_voice2_detune(byte XPos, byte YPos, const char *name);

    void assign_filter_frequency(byte value);
    void assign_filter_resonance(byte value);
    void assign_filter_sweep(byte value);
    void assign_filter_type(byte mixerchannel);

    void assign_envelope1_attack(byte value, int max);
    void assign_envelope1_decay(byte value, int max);
    void assign_envelope1_sustain(byte value);
    void assign_envelope1_release(byte value, int max);
    void set_envelope1_ADSR(byte YPos, int maxA, int maxD, int maxR);
    
};
#endif // PLUGIN_8_H
extern Plugin_8 plugin_8;
