#ifndef PLUGIN_9_H
#define PLUGIN_9_H

#include <Plugins/pluginClass.h>

extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
class Plugin_9 : public PluginControll
{
public:
    float detune1;
    float detune2;
    AudioSynthWaveform waveform;
    AudioSynthWaveform waveform1;
    AudioSynthWaveformDc dc;
    AudioMixer4 vcoMixer;
    AudioEffectEnvelope Fenv;
    AudioSynthWaveform LFO;
    AudioMixer2 fEnvMixer;
    AudioFilterStateVariable filter;
    AudioFilterLadderLite ladder;
    AudioMixer4 fMixer;
    AudioEffectEnvelope Aenv;

    AudioConnection *patchCord[16]; // total patchCordCount:122 including array typed ones.

    Plugin_9(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords
        patchCord[pci++] = new AudioConnection(waveform, 0, vcoMixer, 0);
        patchCord[pci++] = new AudioConnection(waveform1, 0, vcoMixer, 1);
        patchCord[pci++] = new AudioConnection(dc, 0, Fenv, 0);
        patchCord[pci++] = new AudioConnection(Fenv, 0, fEnvMixer, 0);
        patchCord[pci++] = new AudioConnection(LFO, 0, fEnvMixer, 1);

        patchCord[pci++] = new AudioConnection(fEnvMixer, 0, filter, 1);
        patchCord[pci++] = new AudioConnection(fEnvMixer, 0, ladder, 1);
        patchCord[pci++] = new AudioConnection(vcoMixer, 0, filter, 0);
        patchCord[pci++] = new AudioConnection(vcoMixer, 0, ladder, 0);
        patchCord[pci++] = new AudioConnection(filter, 0, fMixer, 0);

        patchCord[pci++] = new AudioConnection(filter, 1, fMixer, 1);
        patchCord[pci++] = new AudioConnection(filter, 2, fMixer, 2);
        patchCord[pci++] = new AudioConnection(ladder, 0, fMixer, 3);
        patchCord[pci++] = new AudioConnection(fMixer, 0, Aenv, 0);
        patchCord[pci++] = new AudioConnection(Aenv, 0, MixGain, 0);

        patchCord[pci++] = new AudioConnection(MixGain, 0, performFilter, 0);
    }
    virtual ~Plugin_9() = default;
    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;
    virtual void assign_parameter(uint8_t pot) override;
};
#endif // PLUGIN_8_H
extern Plugin_9 plugin_9;
