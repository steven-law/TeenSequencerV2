#ifndef PLUGIN_13_H
#define PLUGIN_13_H

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
// class tftClass;
//  TeensyDAW: begin automatically generated code
//  Name: 1Osc
//  Description: Soundfont Synthesizer
//  Voices: 1

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
class Plugin_13 : public PluginControll
{
public:
    AudioSynthWaveformDc dc;
    AudioSynthWaveform waveform;
    AudioEffectEnvelope Fenv;
    AudioFilterStateVariable filter;
    AudioMixer4 fMixer;
    AudioEffectEnvelope Aenv;
    // AudioMixer12 mixer;
    AudioAmplifier MixGain;
    // AudioAmplifier SongVol;
    AudioConnection *patchCord[8]; // total patchCordCount:98 including array typed ones.
    // draw waveforms
    int16_t pl13_customWaveform[GRID_LENGTH_HOR];
    int8_t pl13_oldCustomWaveformValue = 0;
    int8_t pl13_oldCustomWaveformXPos = 32;
    // constructor (this is called when class-object is created)
    Plugin_13(const char *Name, uint8_t ID) : PluginControll(Name, ID)
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
        setParameterNames("W~Form", "Volume", "0", "0", "0", "0", "0", "0", "Filter-Freq", "Resonance", "Sweep", "Type", "0", "0", "0", "0");
        // patchCord[pci++] = new AudioConnection(MixGain, 0, SongVol, 0);
        //  patchCord[pci++] = new AudioConnection(SongVol, 0, dacOut, 0);
    }
    virtual ~Plugin_13() = default;

    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;

    virtual void set_parameters(uint8_t row) override;

    virtual void change_preset() override;
    virtual void set_gain(uint8_t gain) override;
    void set_voice_waveform(uint8_t XPos, uint8_t YPos); // make virtual in baseclass
    void set_voice_amplitude(uint8_t XPos, uint8_t YPos);
    void draw_voice_waveform(uint8_t XPos, uint8_t YPos);
    // draw waveforms
    void clearcustomWaveform(uint8_t YPos);
    void draw_customWaveform(uint8_t YPos);
    void redraw_customWaveform(int8_t YPos);
    void smooth_customWaveform(uint8_t YPos);
    void set_filter_frequency(uint8_t XPos, uint8_t YPos);
    void set_filter_resonance(uint8_t XPos, uint8_t YPos, float min, float max);
    void set_filter_sweep(uint8_t XPos, uint8_t YPos);
    void set_filter_type(uint8_t XPos, uint8_t YPos);
    void selectFilterType(uint8_t mixerchannel);

    void set_envelope_ADSR(uint8_t YPos, int maxA, int maxD, int maxR);


    void assign_voice_waveform(uint8_t value); // make virtual in baseclass but override
    void assign_voice_amplitude(uint8_t value);

    void assign_filter_frequency(uint8_t value);
    void assign_filter_resonance(uint8_t value);
    void assign_filter_sweep(uint8_t value);

    void assign_envelope_attack(uint8_t value, int max);
    void assign_envelope_decay(uint8_t value, int max);
    void assign_envelope_sustain(uint8_t value);
    void assign_envelope_release(uint8_t value, int max);
};
#endif // PLUGIN_10_H

extern Plugin_13 plugin_13;
