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
    // AudioAmplifier NoteGain;
    AudioAmplifier MixGain;
    // AudioAmplifier SongVol;
    AudioConnection *patchCord[13]; // total patchCordCount:98 including array typed ones.

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
        // patchCord[pci++] = new AudioConnection(audioInput, 0, NoteGain, 0);
        //    patchCord[pci++] = new AudioConnection(NoteGain, 0, MixGain, 0);

        // patchCord[pci++] = new AudioConnection(input, 0, MixGain, 0);

        // patchCord[pci++] = new AudioConnection(MixGain, 0, SongVol, 0);
        // patchCord[pci++] = new AudioConnection(SongVol, 0, dacOut, 0);
    }
    virtual ~Plugin_11() = default;

    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;

    virtual void set_parameters(uint8_t row) override;
    virtual void draw_plugin() override;
    virtual void change_preset() override;

    void set_voice_amplitude(uint8_t XPos, uint8_t YPos, const char *name);
    void assign_voice_amplitude(uint8_t value);
    void set_LFO2VCO_amplitude(uint8_t XPos, uint8_t YPos, const char *name);
    void assign_LFO2VCO_amplitude(uint8_t value);
    void set_LFO2VCO_frequency(uint8_t XPos, uint8_t YPos, const char *name);
    void assign_LFO2VCO_frequency(uint8_t value); // make virtual in baseclass but override
 void set_PWMdc_amplitude(uint8_t XPos, uint8_t YPos, const char *name);
    void assign_PWMdc_amplitude(uint8_t value);


    void set_DC_amplitude(uint8_t XPos, uint8_t YPos, const char *name);
    void assign_DC_amplitude(uint8_t value);

    void set_filter_frequency(uint8_t XPos, uint8_t YPos, const char *name);
    void set_filter_resonance(uint8_t XPos, uint8_t YPos, const char *name, float min, float max);
    void set_filter_sweep(uint8_t XPos, uint8_t YPos, const char *name);
    void set_filter_type(uint8_t XPos, uint8_t YPos, const char *name);
    void selectFilterType(uint8_t mixerchannel);

    void set_LFO_waveform(uint8_t XPos, uint8_t YPos, const char *name); // make virtual in baseclass
    void assign_LFO_waveform(uint8_t value);                             // make virtual in baseclass but override
    void set_LFO_frequency(uint8_t XPos, uint8_t YPos, const char *name);
    void assign_LFO_frequency(uint8_t value); // make virtual in baseclass but override

    void set_LFO_amplitude(uint8_t XPos, uint8_t YPos, const char *name);
    void assign_LFO_amplitude(uint8_t value);

    void assign_filter_frequency(uint8_t value);
    void assign_filter_resonance(uint8_t value);
    void assign_filter_sweep(uint8_t value);

    void set_envelope_ADSR(uint8_t YPos, int maxA, int maxD, int maxR);
    void assign_envelope_attack(uint8_t value, int max);
    void assign_envelope_decay(uint8_t value, int max);
    void assign_envelope_sustain(uint8_t value);
    void assign_envelope_release(uint8_t value, int max);
};
#endif // PLUGIN_11_H

extern Plugin_11 plugin_11;
