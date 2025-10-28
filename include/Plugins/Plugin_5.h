#ifndef PLUGIN_5_H
#define PLUGIN_5_H

#include <Plugins/pluginClass.h>
#include <ownLibs/synth_fm_drum.h>

extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
class Plugin_5 : public PluginControll
{
public:
    AudioSynthWaveformDc dc;
    AudioSynthNoisePink pink;
    AudioEffectEnvelope hhFilterEnv;
    AudioFilterStateVariable filter;
    AudioSynthFMDrum fm_drum;
    AudioSynthFMDrum fm_snare;
    AudioEffectEnvelope hhEnv;
    AudioSynthSimpleDrum tomL;
    AudioSynthSimpleDrum tomM;
    AudioSynthSimpleDrum tomH;

    AudioSynthWaveformSine modulator;
    AudioSynthWaveformSineModulated sine;
    AudioSynthNoiseWhite noise;
    AudioFilterBiquad noiseFilter;
    AudioEffectEnvelope envelope;
    AudioFilterBiquad HHfilter;
    AudioMixer4 HHmixer;
    AudioMixer12 mixer;

    AudioConnection *patchCord[12]; // total patchCordCount:12 including array typed ones.

    Plugin_5(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {
        int pci = 0; // used only for adding new patchcords

        patchCord[pci++] = new AudioConnection(dc, 0, hhFilterEnv, 0);
        patchCord[pci++] = new AudioConnection(pink, 0, filter, 0);
        patchCord[pci++] = new AudioConnection(hhFilterEnv, 0, filter, 1);
        patchCord[pci++] = new AudioConnection(filter, 2, hhEnv, 0);
        patchCord[pci++] = new AudioConnection(fm_drum, 0, mixer, 0);

        patchCord[pci++] = new AudioConnection(fm_snare, 0, mixer, 1);
        patchCord[pci++] = new AudioConnection(hhEnv, 0, mixer, 2);

        patchCord[pci++] = new AudioConnection(noise, 0, noiseFilter, 0);
        patchCord[pci++] = new AudioConnection(noiseFilter, 0, HHmixer, 0);
        patchCord[pci++] = new AudioConnection(modulator, 0, sine, 0);
        patchCord[pci++] = new AudioConnection(sine, 0, HHmixer, 1);
        patchCord[pci++] = new AudioConnection(HHmixer, 0, envelope, 0);
        patchCord[pci++] = new AudioConnection(envelope, 0, HHfilter, 0);

        patchCord[pci++] = new AudioConnection(HHfilter, 0, mixer, 3);
        patchCord[pci++] = new AudioConnection(tomL, 0, mixer, 4);
        patchCord[pci++] = new AudioConnection(tomM, 0, mixer, 5);
        patchCord[pci++] = new AudioConnection(tomH, 0, mixer, 6);

        patchCord[pci++] = new AudioConnection(mixer, 0, MixGain, 0);
        patchCord[pci++] = new AudioConnection(MixGain, 0, performFilter, 0);
    }
    virtual ~Plugin_5() = default;

    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;
    virtual void assign_parameter(uint8_t pot) override;
};
#endif // PLUGIN_5_H

extern Plugin_5 plugin_5;
