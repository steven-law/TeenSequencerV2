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
    AudioInputI2S input;
    AudioAmplifier NoteGain;
    AudioAmplifier MixGain;
    AudioAmplifier SongVol;
    AudioConnection *patchCord[3]; // total patchCordCount:98 including array typed ones.

    // constructor (this is called when class-object is created)
    Plugin_11(const char *Name, byte ID) : PluginControll(Name, ID)
    {

        int pci = 0; // used only for adding new patchcords

 
        patchCord[pci++] = new AudioConnection(input, 0, NoteGain, 0);
        patchCord[pci++] = new AudioConnection(NoteGain, 0, MixGain, 0);

        //patchCord[pci++] = new AudioConnection(input, 0, MixGain, 0);

        patchCord[pci++] = new AudioConnection(MixGain, 0, SongVol, 0);
    }
    virtual ~Plugin_11() = default;

    virtual void setup() override;
    virtual void noteOn(byte notePlayed, float velocity, byte voice) override;
    virtual void noteOff(byte notePlayed, byte voice) override;

    virtual void set_parameters(byte row) override;
    virtual void draw_plugin() override;
    virtual void change_preset() override;

    void set_voice_amplitude(byte XPos, byte YPos, const char *name);

};
#endif // PLUGIN_11_H


extern Plugin_11 plugin_11;
