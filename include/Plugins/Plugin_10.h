#ifndef PLUGIN_10_H
#define PLUGIN_10_H

#include <Plugins/pluginClass.h>
#include <Plugins/AudioSamples/AudioSamples.h>

extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
class Plugin_10 : public PluginControll
{
public:
    AudioSynthWaveformDc dc;
    AudioSynthWavetable waveform;
    AudioEffectEnvelope Fenv;
    AudioFilterStateVariable filter;
    AudioMixer4 fMixer;
    AudioEffectEnvelope Aenv;

    AudioConnection *patchCord[9]; // total patchCordCount:98 including array typed ones.

    Plugin_10(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords

        patchCord[pci++] = new AudioConnection(dc, 0, Fenv, 0);
        patchCord[pci++] = new AudioConnection(waveform, 0, filter, 0);
        patchCord[pci++] = new AudioConnection(Fenv, 0, filter, 1);
        patchCord[pci++] = new AudioConnection(filter, 0, fMixer, 0);
        patchCord[pci++] = new AudioConnection(filter, 1, fMixer, 1);

        patchCord[pci++] = new AudioConnection(filter, 2, fMixer, 2);
        patchCord[pci++] = new AudioConnection(fMixer, 0, Aenv, 0);
        patchCord[pci++] = new AudioConnection(Aenv, 0, MixGain, 0);
        patchCord[pci++] = new AudioConnection(MixGain, 0, performFilter, 0);
    }
    virtual ~Plugin_10() = default;

    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;
    virtual void assign_parameter(uint8_t pot) override;

    void assign_voice_waveform(uint8_t value);
};
#endif // PLUGIN_10_H

extern Plugin_10 plugin_10;
