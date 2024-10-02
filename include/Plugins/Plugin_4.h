#ifndef PLUGIN_4_H
#define PLUGIN_4_H

#define PL4_VOICES 8
#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include <Plugins/pluginClass.h>
#include "TeensyVariablePlayback.h"
#include <Plugins/AudioSamples/AudioSamples.h>
#include "flashloader.h"
////#include "hardware/tftClass.h"
// class tftClass;
//  TeensyDAW: begin automatically generated code
//  Name: mDrm
//  Description: Plays 12 Samplesounds stored on Flash (PROGMEM)
//  Voices: 12

// Pot 1: Vol1
// Pot 2: Vol2
// Pot 3: Vol3
// Pot 4: Vol4

// Pot 5: Vol5
// Pot 6: Vol6
// Pot 7: Vol7
// Pot 8: Vol8

// Pot 9: Vol9
// Pot 10: Vol10
// Pot 11: Vol11
// Pot 12: Vol12
bool compareFiles(File &file, SerialFlashFile &ffile);
void error(const char *message);

extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
class Plugin_4 : public PluginControll
{
public:
    // AudioPlaySerialflashRaw  playMem[MAX_VOICES];

    AudioPlayArrayResmp playMem[PL4_VOICES];
    // AudioPlayMemory playMem[MAX_VOICES];
    newdigate::audiosample *sample[PL4_VOICES];
    newdigate::flashloader loader;
    AudioEffectEnvelope AEnv[PL4_VOICES];
    AudioMixer12 mixer;

    AudioAmplifier MixGain;
    // AudioAmplifier SongVol;
    AudioConnection *patchCord[PL4_VOICES*2 + 1]; // total patchCordCount:14 including array typed ones.
    char _fileName[PL4_VOICES][20];

    // constructor (this is called when class-object is created)
    Plugin_4(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords

        patchCord[pci++] = new AudioConnection(mixer, 0, MixGain, 0);
        // patchCord[pci++] = new AudioConnection(MixGain, 0, SongVol, 0);
        // patchCord[pci++] = new AudioConnection(SongVol, 0, dacOut, 0);
        for (int i = 0; i < PL4_VOICES; i++)
        {
            patchCord[pci++] = new AudioConnection(playMem[i], 0, AEnv[i], 0);
            patchCord[pci++] = new AudioConnection(AEnv[i], 0, mixer, i);
        }
    }
    virtual ~Plugin_4() = default;
    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;
    virtual void set_parameters(uint8_t row) override;
    virtual void draw_plugin() override;
    virtual void change_preset() override;
    void set_mixer_gain(uint8_t XPos, uint8_t YPos, const char *name);
    void assign_mixer_gain(uint8_t value, uint8_t channel);
    void set_waveform(uint8_t XPos, uint8_t YPos, const char *name);
    void assign_waveform(uint8_t value, uint8_t channel);
};

#endif // PLUGIN_3_H

extern Plugin_4 plugin_4;
