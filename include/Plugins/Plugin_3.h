#ifndef PLUGIN_3_H
#define PLUGIN_3_H

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
/*
M WF    M Ratio   M Vol    C WF
M Atk   M Dec     M St     M Rl
C At    C Dc      C St     C Rl
*/
// TeensyDAW: begin automatically generated code
// Name: 2FM
// Description: Simple FM Snthesis Modulator -> Carrier
// Voices: 12

// Operator
// Pot 1: mWaveform
// Pot 2: mRatio
// Pot 3: mVolume
// Pot 4: cWaveform

// Mod Envelope
// Pot 5: mAttack
// Pot 6: mDecay
// Pot 7: mSustain
// Pot 8: mRelease

// Envelope:
// Pot 9: Attack
// Pot 10: Decay
// Pot 11: Sustain
// Pot 12: Release
extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
class Plugin_3 : public PluginControll
{
public:
#define NUM_RATIOS 10
    const float ratios[NUM_RATIOS] = {0.125, 0.25, 0.5, 0.75, 1, 2, 3, 4, 6, 8};
    float modulator_ratio = 1;
    AudioSynthWaveformModulated modulator;
    AudioEffectEnvelope modEnv;
    AudioSynthWaveformModulated carrier;
    AudioEffectEnvelope outEnv;
    // AudioMixer4 mixer;
    AudioAmplifier MixGain;
    // AudioAmplifier SongVol;
    AudioConnection *patchCord[4]; // total patchCordCount:50 including array typed ones.
    // AudioAnalyzePeak peak;
    //  constructor (this is called when class-object is created)
    Plugin_3(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords

        // patchCord[pci++] = new AudioConnection(modulator[0], 0, peak, 0);

        patchCord[pci++] = new AudioConnection(modulator, 0, modEnv, 0);
        patchCord[pci++] = new AudioConnection(modEnv, 0, carrier, 0);
        patchCord[pci++] = new AudioConnection(carrier, 0, outEnv, 0);
        // patchCord[pci++] = new AudioConnection(outEnv, 0, mixer, 0);
        patchCord[pci++] = new AudioConnection(outEnv, 0, MixGain, 0);
        // patchCord[pci++] = new AudioConnection(MixGain, 0, SongVol, 0);
        //  patchCord[pci++] = new AudioConnection(MixGain, 0, dacOut, 0);
    }
    virtual ~Plugin_3() = default;

    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;
    virtual void set_parameters(uint8_t row) override;
    virtual void draw_plugin() override;
    virtual void change_preset() override;
virtual void set_gain(uint8_t gain) override;
    void get_peak();
    void set_mod_waveform(uint8_t XPos, uint8_t YPos, const char *name);
    void assign_mod_waveform(uint8_t value);

    void set_mod_amplitude(uint8_t XPos, uint8_t YPos, const char *name);
    void assign_mod_amplitude(uint8_t value);
    void set_mod_ratio(uint8_t XPos, uint8_t YPos, const char *name);
    void assign_mod_ratio(uint8_t value);
    void set_car_waveform(uint8_t XPos, uint8_t YPos, const char *name);
    void assign_car_waveform(uint8_t value);

    // void set_envelope_attack(uint8_t XPos, uint8_t YPos, const char *name, int min, int max);
    // void set_envelope_decay(uint8_t XPos, uint8_t YPos, const char *name, int min, int max);
    // void set_envelope_sustain(uint8_t XPos, uint8_t YPos, const char *name);
    // void set_envelope_release(uint8_t XPos, uint8_t YPos, const char *name, int min, int max);

    void set_envelope_mattack(uint8_t XPos, uint8_t YPos, const char *name, int min, int max);
void assign_envelope_mattack(uint8_t value,int max);
void assign_envelope_mdecay(uint8_t value,int max);
void assign_envelope_msustain(uint8_t value);
void assign_envelope_mrelease(uint8_t value,int max);
    void set_envelope_mdecay(uint8_t XPos, uint8_t YPos, const char *name, int min, int max);
    void set_envelope_msustain(uint8_t XPos, uint8_t YPos, const char *name);
    void set_envelope_mrelease(uint8_t XPos, uint8_t YPos, const char *name, int min, int max);

    void set_envelope_ADSR(uint8_t YPos, int maxA, int maxD, int maxR);
    void assign_envelope_attack(uint8_t value, int max);
    void assign_envelope_decay(uint8_t value, int max);
    void assign_envelope_sustain(uint8_t value);
    void assign_envelope_release(uint8_t value, int max);
};

#endif                    // PLUGIN_3_H
extern Plugin_3 plugin_3; // Deklaration der Variable
