#ifndef PLUGIN_5_H
#define PLUGIN_5_H

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include <Plugins/pluginClass.h>
#include <ownLibs/synth_fm_drum.h>
////#include "hardware/tftClass.h"
// class tftClass;
//  TeensyDAW: begin automatically generated code
//  Name: Drum
//  Description: Synthesize 12 Drum sounds
//  Voices: 12

// Kick
// Pot 1: Freq
// Pot 2: Sweep
// Pot 3: O-Drive
// Pot 4: Decay

// Snare
// Pot 5: Freq
// Pot 6: Sweep
// Pot 7: Noise
// Pot 8: Decay

// Hihat
// Pot 9: Freq
// Pot 10: Reso
// Pot 11: Attack
// Pot 12: Decay

// Toms
// Pot 13: TomL
// Pot 14: TomM
// Pot 15: TomH
// Pot 16: Decay
extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
class Plugin_5 : public PluginControll
{
public:
    AudioSynthWaveformDc dc;
    AudioSynthNoisePink pink;
    AudioEffectEnvelope hhFilterEnv;
    AudioFilterStateVariable filter;
    AudioSynthFMDrum fm_drum;
    AudioSynthFMDrum fm_snare;
    AudioEffectEnvelope hhEnv;
    AudioSynthSimpleDrum tomL;
    AudioSynthSimpleDrum tomM;
    AudioSynthSimpleDrum tomH;
    AudioMixer12 mixer;
    AudioAmplifier MixGain;
    // AudioAmplifier SongVol;
    AudioConnection *patchCord[11]; // total patchCordCount:12 including array typed ones.

    // constructor (this is called when class-object is created)
    Plugin_5(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords

        patchCord[pci++] = new AudioConnection(dc, 0, hhFilterEnv, 0);
        patchCord[pci++] = new AudioConnection(pink, 0, filter, 0);
        patchCord[pci++] = new AudioConnection(hhFilterEnv, 0, filter, 1);
        patchCord[pci++] = new AudioConnection(filter, 2, hhEnv, 0);

        patchCord[pci++] = new AudioConnection(fm_drum, 0, mixer, 0);
        patchCord[pci++] = new AudioConnection(fm_snare, 0, mixer, 1);
        patchCord[pci++] = new AudioConnection(hhEnv, 0, mixer, 2);
        patchCord[pci++] = new AudioConnection(tomL, 0, mixer, 3);

        patchCord[pci++] = new AudioConnection(tomM, 0, mixer, 4);
        patchCord[pci++] = new AudioConnection(tomH, 0, mixer, 5);
        patchCord[pci++] = new AudioConnection(mixer, 0, MixGain, 0);
        // patchCord[pci++] = new AudioConnection(MixGain, 0, SongVol, 0);
        // patchCord[pci++] = new AudioConnection(SongVol, 0, dacOut, 0);
    }
    virtual ~Plugin_5() = default;

    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;
    virtual void set_parameters(uint8_t row) override;
    virtual void draw_plugin() override;
    virtual void change_preset() override;
    virtual void set_gain(uint8_t gain) override;
    void set_fmsnare_frequency(uint8_t XPos, uint8_t YPos, const char *name, int min, int max);
    void set_fmsnare_pitchMod(uint8_t XPos, uint8_t YPos, const char *name);
    void set_fmsnare_decay(uint8_t XPos, uint8_t YPos, const char *name);
    void set_fmsnare_noise(uint8_t XPos, uint8_t YPos, const char *name);
    void set_fmsnare_overdrive(uint8_t XPos, uint8_t YPos, const char *name);

    void set_fmdrum_frequency(uint8_t XPos, uint8_t YPos, const char *name, int min, int max);
    void set_fmdrum_pitchMod(uint8_t XPos, uint8_t YPos, const char *name);
    void set_fmdrum_decay(uint8_t XPos, uint8_t YPos, const char *name);
    void set_fmdrum_noise(uint8_t XPos, uint8_t YPos, const char *name);
    void set_fmdrum_overdrive(uint8_t XPos, uint8_t YPos, const char *name);

    void set_hhfilter_frequency(uint8_t XPos, uint8_t YPos, const char *name, int min, int max);
    void set_hhfilter_resonance(uint8_t XPos, uint8_t YPos, const char *name, float min, float max);
    void set_hhfilter_sweep(uint8_t XPos, uint8_t YPos, const char *name, float min, float max);

    void set_hhEnv_attack(uint8_t XPos, uint8_t YPos, const char *name, int min, int max);
    void set_hhEnv_decay(uint8_t XPos, uint8_t YPos, const char *name, int min, int max);
    void set_hhEnv_sustain(uint8_t XPos, uint8_t YPos, const char *name);
    void set_hhEnv_release(uint8_t XPos, uint8_t YPos, const char *name, int min, int max);

    void set_tomL_frequency(uint8_t XPos, uint8_t YPos, const char *name);
    void set_tomM_frequency(uint8_t XPos, uint8_t YPos, const char *name);
    void set_tomH_frequency(uint8_t XPos, uint8_t YPos, const char *name);
    void set_toms_decay(uint8_t XPos, uint8_t YPos, const char *name, int min, int max);
};
#endif // PLUGIN_5_H

extern Plugin_5 plugin_5;
