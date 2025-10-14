#ifndef PLUGIN_2_H
#define PLUGIN_2_H

#include <Plugins/pluginClass.h>

extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
class Plugin_2 : public PluginControll
{
public:
    AudioSynthWaveformDc dc;
    AudioSynthWaveformModulated waveform;
    AudioSynthWaveform LFO;
    AudioSynthWaveform Lfo2Vco;
    AudioMixer2 fEnvMixer;
    AudioEffectEnvelope Fenv;
    AudioFilterStateVariable filter;
    AudioMixer4 fMixer;
    AudioEffectEnvelope Aenv;
    AudioConnection *patchCord[12]; // total patchCordCount:98 including array typed ones.

    Plugin_2(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {

        int pci = 0; // used only for adding new patchcords
        patchCord[pci++] = new AudioConnection(dc, 0, Fenv, 0);
        patchCord[pci++] = new AudioConnection(Fenv, 0, fEnvMixer, 0);
        patchCord[pci++] = new AudioConnection(LFO, 0, fEnvMixer, 1);
        patchCord[pci++] = new AudioConnection(fEnvMixer, 0, filter, 1);
        patchCord[pci++] = new AudioConnection(Lfo2Vco, 0, waveform, 0);

        patchCord[pci++] = new AudioConnection(waveform, 0, filter, 0);
        patchCord[pci++] = new AudioConnection(filter, 0, fMixer, 0);
        patchCord[pci++] = new AudioConnection(filter, 1, fMixer, 1);
        patchCord[pci++] = new AudioConnection(filter, 2, fMixer, 2);
        patchCord[pci++] = new AudioConnection(fMixer, 0, Aenv, 0);

        patchCord[pci++] = new AudioConnection(Aenv, 0, MixGain, 0);
        patchCord[pci++] = new AudioConnection(MixGain, 0, performFilter, 0);
    }
    virtual ~Plugin_2() = default;

    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;
    virtual void assign_parameter(uint8_t pot) override;
};

#endif // PLUGIN_2_H
extern Plugin_2 plugin_2;
