#ifndef PLUGIN_7_H
#define PLUGIN_7_H

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include <Plugins/pluginClass.h>
#include <ownLibs/synth_fm_drum.h>
#include <ownLibs/effect_dynamics.h>
////#include "hardware/tftClass.h"
// class tftClass;
//  TeensyDAW: begin automatically generated code
//  Name: Boom
//  Description: Subtractive Synthesizer
//  Voices: 1

// Kick
// Pot 1: Sweep
// Pot 2: Noise
// Pot 3: O-Drive
// Pot 4: Decay

//
// Pot 5:
// Pot 6:
// Pot 7:
// Pot 8:

//
// Pot 9:
// Pot 10:
// Pot 11:
// Pot 12:

//
// Pot 13:
// Pot 14:
// Pot 15:
// Pot 16:



extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
class Plugin_7 : public PluginControll
{
public:
    float threshold;
    float attack = 0.1f;
    float release = 0.5f;
    float hysterisis = 1.0f;
    float ratio = 35.0f;
    float kneeWidth = 1.0f;

    AudioSynthFMDrum fm_drum;
    AudioEffectDynamics dynamics;
    AudioAmplifier amp;


    // AudioAmplifier SongVol;
    AudioConnection *patchCord[4]; // total patchCordCount:2 including array typed ones.

    // constructor (this is called when class-object is created)
    Plugin_7(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords

        patchCord[pci++] = new AudioConnection(fm_drum, 0, dynamics, 0);
        patchCord[pci++] = new AudioConnection(dynamics, 0, amp, 0);
        patchCord[pci++] = new AudioConnection(amp, 0, MixGain, 0);
         patchCord[pci++] = new AudioConnection(MixGain, 0, performFilter, 0);

    }
    virtual ~Plugin_7() = default;

    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;
    virtual void assign_parameter(uint8_t pot) override;

};
#endif // PLUGIN_3_H

extern Plugin_7 plugin_7;
