#ifndef PLUGIN_1_H
#define PLUGIN_1_H

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "projectVariables.h"

#include <Plugins/pluginClass.h>
////#include "hardware/tftClass.h"
// class tftClass;
//  TeensyDAW: begin automatically generated code
//  Name: Strng
//  Description: Synthesize 12 plucked string sounds, such as a guitar sound
//  Voices: 12
//  Strings
//  Pot 1: Vol1
//  Pot 2: Vol2
//  Pot 3: Vol3
//  Pot 4: Vol4

// Pot 5: Vol5
// Pot 6: Vol6
// Pot 7: Vol7
// Pot 8: Vol8

// Pot 9: Vol9
// Pot 10: Vol10
// Pot 11: Vol11
// Pot 12: Vol12

extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
class Plugin_1 : public PluginControll
{
public:
    AudioSynthKarplusStrong string[MAX_VOICES];
    AudioMixer12 mixer;
    AudioConnection *patchCord[MAX_VOICES + 2]; // total patchCordCount:14 including array typed ones.
    // constructor (this is called when class-object is created)
    Plugin_1(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords

        patchCord[pci++] = new AudioConnection(mixer, 0, MixGain, 0);
        patchCord[pci++] = new AudioConnection(MixGain, 0, performFilter, 0);
        // patchCord[pci++] = new AudioConnection(SongVol, 0, dacOut, 0);

        for (int i = 0; i < MAX_VOICES; i++)
        {
            patchCord[pci++] = new AudioConnection(string[i], 0, mixer, i);
        }
    }
    virtual ~Plugin_1() = default;

    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;
    virtual void assign_parameter(uint8_t pot) override;

};

#endif // PLUGIN_1_H
// TeensyDAW: end automatically generated code
extern Plugin_1 plugin_1;
