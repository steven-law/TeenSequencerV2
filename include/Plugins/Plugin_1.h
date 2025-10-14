#ifndef PLUGIN_1_H
#define PLUGIN_1_H

#include <Plugins/pluginClass.h>

extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
class Plugin_1 : public PluginControll
{
public:
    AudioSynthKarplusStrong string[MAX_VOICES];
    AudioMixer12 mixer;
    AudioConnection *patchCord[MAX_VOICES + 2]; // total patchCordCount:14 including array typed ones.
    Plugin_1(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords

        patchCord[pci++] = new AudioConnection(mixer, 0, MixGain, 0);
        patchCord[pci++] = new AudioConnection(MixGain, 0, performFilter, 0);

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
