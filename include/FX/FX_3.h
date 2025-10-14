#ifndef FX_3_H
#define FX_3_H

#include <Plugins/pluginClass.h>
#include <Plugins/plugin_List.h>

extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;

class FX_3 : public PluginControll
{
public:
    AudioEffectDelay delay;
    AudioMixer4 delayMixer;
    AudioMixer16 FX_mixer;

    AudioConnection *patchCord[NUM_PLUGINS + 3]; // total patchCordCount:46 including array typed ones.
    FX_3(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords

        for (int i = 0; i < NUM_PLUGINS; i++)
        {
            patchCord[pci++] = new AudioConnection(allPlugins[i]->performFilter, 0, FX_mixer, i);
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
};
#endif // FX_3_H