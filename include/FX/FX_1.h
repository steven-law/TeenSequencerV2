#ifndef FX_1_H
#define FX_1_H

#include <Plugins/pluginClass.h>
#include <Plugins/plugin_List.h>

extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;

class FX_1 : public PluginControll
{
public:
    AudioMixer16 FX_mixer;
    AudioEffectFreeverb freeverb;

    AudioConnection *patchCord[NUM_PLUGINS + 1]; // total patchCordCount:46 including array typed ones.

    FX_1(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords

        for (int i = 0; i < NUM_PLUGINS; i++)
        {
            patchCord[pci++] = new AudioConnection(allPlugins[i]->performFilter, 0, FX_mixer, i);
        }

        patchCord[pci++] = new AudioConnection(FX_mixer, 0, freeverb, 0);
    }

    virtual ~FX_1() = default;

    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;
    virtual void assign_parameter(uint8_t pot) override;
};
#endif // FX_1_H