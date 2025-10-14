#ifndef PLUGIN_6_H
#define PLUGIN_6_H

#include <Plugins/pluginClass.h>
#define PL6_VOICES 4

extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
class Plugin_6 : public PluginControll
{
public:
    AudioSynthWaveformDc dc;
    AudioSynthWaveform waveform[PL6_VOICES];
    AudioMixer4 mixer;
    AudioEffectEnvelope Fenv;
    AudioFilterStateVariable filter;
    AudioMixer4 fMixer;
    AudioEffectEnvelope Aenv;

    AudioConnection *patchCord[10]; // total patchCordCount:98 including array typed ones.
    int8_t pl6ChoosenChord;
    Plugin_6(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {

        int pci = 0; // used only for adding new patchcords
        patchCord[pci++] = new AudioConnection(waveform[0], 0, mixer, 0);
        patchCord[pci++] = new AudioConnection(waveform[1], 0, mixer, 1);
        patchCord[pci++] = new AudioConnection(waveform[2], 0, mixer, 2);
        patchCord[pci++] = new AudioConnection(waveform[3], 0, mixer, 3);
        patchCord[pci++] = new AudioConnection(mixer, 0, filter, 0);

        patchCord[pci++] = new AudioConnection(dc, 0, Fenv, 0);
        patchCord[pci++] = new AudioConnection(Fenv, 0, filter, 1);
        patchCord[pci++] = new AudioConnection(filter, 0, Aenv, 0);
        patchCord[pci++] = new AudioConnection(Aenv, 0, MixGain, 0);
        patchCord[pci++] = new AudioConnection(MixGain, 0, performFilter, 0);
    }
    virtual ~Plugin_6() = default;
    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;
    virtual void assign_parameter(uint8_t pot) override;
};
#endif // PLUGIN_6_H

extern Plugin_6 plugin_6;
