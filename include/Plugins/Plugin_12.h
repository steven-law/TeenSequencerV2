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
//class tftClass;
// TeensyDAW: begin automatically generated code
// Name: 1Osc
// Description: Subtractive Synthesizer
// Voices: 12

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

class Plugin_12 : public PluginControll
{
public:
    AudioSynthWaveformDc dc;
   // AudioSynthWaveform waveform;
    AudioPlaySdResmp waveform;
    AudioEffectEnvelope Fenv;
    AudioFilterStateVariable filter;
    AudioMixer4 fMixer;
    AudioEffectEnvelope Aenv;
    //AudioMixer12 mixer;
    AudioAmplifier MixGain;
    //AudioAmplifier SongVol;
    AudioConnection *patchCord[9]; // total patchCordCount:98 including array typed ones.
 char* _filename = "C0.RAW";
    // constructor (this is called when class-object is created)
    Plugin_12(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {

        int pci = 0; // used only for adding new patchcords

        //patchCord[pci++] = new AudioConnection(mixer, 0, MixGain, 0);
        //patchCord[pci++] = new AudioConnection(MixGain, 0, SongVol, 0);
       // patchCord[pci++] = new AudioConnection(SongVol, 0, dacOut, 0);
       
            patchCord[pci++] = new AudioConnection(dc, 0, Fenv, 0);
            patchCord[pci++] = new AudioConnection(waveform, 0, filter, 0);
            patchCord[pci++] = new AudioConnection(Fenv, 0, filter, 1);
            patchCord[pci++] = new AudioConnection(filter, 0, fMixer, 0);
            patchCord[pci++] = new AudioConnection(filter, 1, fMixer, 1);
            patchCord[pci++] = new AudioConnection(filter, 2, fMixer, 2);
            patchCord[pci++] = new AudioConnection(fMixer, 0, Aenv, 0);
            patchCord[pci++] = new AudioConnection(Aenv, 0, MixGain, 0);
        
    }
    virtual ~Plugin_12() = default;

    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;

    virtual void set_parameters(uint8_t row) override;
    virtual void draw_plugin() override;
virtual void change_preset() override;


    void set_voice_bank(uint8_t XPos, uint8_t YPos, const char *name); // make virtual in baseclass
    void set_voice_waveform(uint8_t XPos, uint8_t YPos, const char *name); // make virtual in baseclass
    void set_voice_amplitude(uint8_t XPos, uint8_t YPos, const char *name);

    void set_filter_frequency(uint8_t XPos, uint8_t YPos, const char *name);
    void set_filter_resonance(uint8_t XPos, uint8_t YPos, const char *name, float min, float max);
    void set_filter_sweep(uint8_t XPos, uint8_t YPos, const char *name);
    void set_filter_type(uint8_t XPos, uint8_t YPos, const char *name);
    void selectFilterType(uint8_t mixerchannel);

    void set_envelope_ADSR(uint8_t YPos, int maxA, int maxD, int maxR);
    void set_envelope_attack(uint8_t XPos, uint8_t YPos, const char *name,  int max);
    void set_envelope_decay(uint8_t XPos, uint8_t YPos, const char *name,  int max);
    void set_envelope_sustain(uint8_t XPos, uint8_t YPos, const char *name);
    void set_envelope_release(uint8_t XPos, uint8_t YPos, const char *name,  int max);

   void  assign_voice_bank(uint8_t value);
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

#endif // PLUGIN_2_H
extern Plugin_12 plugin_12;