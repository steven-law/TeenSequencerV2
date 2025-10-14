#ifndef PLUGIN_8_H
#define PLUGIN_8_H

#include <Plugins/pluginClass.h>

extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
class Plugin_8 : public PluginControll
{
public:
    float detune1;
    float detune2;
    AudioSynthWaveform waveform;
    AudioSynthWaveform waveform1;
    AudioSynthWaveformDc dc;
    AudioMixer4 vcoMixer;
    AudioEffectEnvelope Fenv;
    AudioFilterStateVariable filter;
    AudioFilterLadderLite ladder;
    AudioMixer4 fMixer;
    AudioEffectEnvelope Aenv;

    AudioConnection *patchCord[14]; // total patchCordCount:122 including array typed ones.

    Plugin_8(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords
        patchCord[pci++] = new AudioConnection(waveform, 0, vcoMixer, 0);
        patchCord[pci++] = new AudioConnection(waveform1, 0, vcoMixer, 1);
        patchCord[pci++] = new AudioConnection(dc, 0, Fenv, 0);
        patchCord[pci++] = new AudioConnection(vcoMixer, 0, filter, 0);
        patchCord[pci++] = new AudioConnection(vcoMixer, 0, ladder, 0);

        patchCord[pci++] = new AudioConnection(Fenv, 0, filter, 1);
        patchCord[pci++] = new AudioConnection(Fenv, 0, ladder, 1);
        patchCord[pci++] = new AudioConnection(filter, 0, fMixer, 0);
        patchCord[pci++] = new AudioConnection(filter, 1, fMixer, 1);
        patchCord[pci++] = new AudioConnection(filter, 2, fMixer, 2);

        patchCord[pci++] = new AudioConnection(ladder, 0, fMixer, 3);
        patchCord[pci++] = new AudioConnection(fMixer, 0, Aenv, 0);
        patchCord[pci++] = new AudioConnection(Aenv, 0, MixGain, 0);
        patchCord[pci++] = new AudioConnection(MixGain, 0, performFilter, 0);
    }
    virtual ~Plugin_8() = default;
    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;
    virtual void assign_parameter(uint8_t pot) override;
};
#endif // PLUGIN_8_H
extern Plugin_8 plugin_8;
