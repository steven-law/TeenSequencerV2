#ifndef PLUGIN_3_H
#define PLUGIN_3_H

#include <Plugins/pluginClass.h>

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
    AudioConnection *patchCord[5]; // total patchCordCount:50 including array typed ones.

    Plugin_3(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords

        patchCord[pci++] = new AudioConnection(modulator, 0, modEnv, 0);
        patchCord[pci++] = new AudioConnection(modEnv, 0, carrier, 0);
        patchCord[pci++] = new AudioConnection(carrier, 0, outEnv, 0);
        patchCord[pci++] = new AudioConnection(outEnv, 0, MixGain, 0);
        patchCord[pci++] = new AudioConnection(MixGain, 0, performFilter, 0);
    }
    virtual ~Plugin_3() = default;

    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;
    virtual void assign_parameter(uint8_t pot) override;
};

#endif                    // PLUGIN_3_H
extern Plugin_3 plugin_3; // Deklaration der Variable
