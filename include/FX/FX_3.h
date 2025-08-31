#ifndef FX_3_H
#define FX_3_H

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "projectVariables.h"
#include <Plugins/pluginClass.h>
#include <Plugins/plugin_List.h>

#include "ownLibs/mixers.h"

extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;

class FX_3 : public PluginControll
{
public:
    AudioAmplifier pl[NUM_PLUGINS];
    AudioEffectDelay delay;
    AudioMixer4 delayMixer;
    AudioMixer16 FX_mixer;

    AudioConnection *patchCord[NUM_PLUGINS * 2 + 3]; // total patchCordCount:46 including array typed ones.
    FX_3(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords

        patchCord[pci++] = new AudioConnection(plugin_1.MixGain, 0, pl[0], 0);
        patchCord[pci++] = new AudioConnection(plugin_2.MixGain, 0, pl[1], 0);
        patchCord[pci++] = new AudioConnection(plugin_3.MixGain, 0, pl[2], 0);
        patchCord[pci++] = new AudioConnection(plugin_4.MixGain, 0, pl[3], 0);
        patchCord[pci++] = new AudioConnection(plugin_5.MixGain, 0, pl[4], 0);
        patchCord[pci++] = new AudioConnection(plugin_6.MixGain, 0, pl[5], 0);
        patchCord[pci++] = new AudioConnection(plugin_7.MixGain, 0, pl[6], 0);
        patchCord[pci++] = new AudioConnection(plugin_8.MixGain, 0, pl[7], 0);
        patchCord[pci++] = new AudioConnection(plugin_9.MixGain, 0, pl[8], 0);
        patchCord[pci++] = new AudioConnection(plugin_10.MixGain, 0, pl[9], 0);
        patchCord[pci++] = new AudioConnection(plugin_11.MixGain, 0, pl[10], 0);
        patchCord[pci++] = new AudioConnection(plugin_12.MixGain, 0, pl[11], 0);
        patchCord[pci++] = new AudioConnection(plugin_13.MixGain, 0, pl[12], 0);
        patchCord[pci++] = new AudioConnection(plugin_14.MixGain, 0, pl[13], 0);
        for (int i = 0; i < NUM_PLUGINS; i++)
        {
            patchCord[pci++] = new AudioConnection(pl[i], 0, FX_mixer, i);
        }

        patchCord[pci++] = new AudioConnection(FX_mixer, 0, delayMixer, 0);
        patchCord[pci++] = new AudioConnection(delayMixer, 0, delay, 0);
        patchCord[pci++] = new AudioConnection(delay, 0, delayMixer, 1);
    }

    virtual ~FX_3() = default;

    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;
    virtual void assign_parameter(uint8_t pot) override;
    virtual void set_gain(uint8_t gain) override;

};
#endif // FX_3_H