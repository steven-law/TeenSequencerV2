#ifndef PLUGIN_9_H
#define PLUGIN_9_H


#include "ownLibs/filter_ladderlite.h"
#include <Plugins/pluginClass.h>
////#include "hardware/tftClass.h"
// class tftClass;
//  TeensyDAW: begin automatically generated code
//  Name: dTune
//  Description: 2VCO Detuned Subtractive Synthesizer
//  Voices: 12

// VCO 1
// Pot 1: Waveform
// Pot 2: Detune
// Pot 3: Vol
// Pot 4:

// VCO 2
// Pot 5: Waveform
// Pot 6: Detune
// Pot 7: Vol
// Pot 8:

// Filter:
// Pot 9: Frequency
// Pot 10: Resonance
// Pot 11: Sweep
// Pot 12: Type

// Envelope:
// Pot 13: Attack
// Pot 14: Decay
// Pot 15: Sustain
// Pot 16: Release
extern bool enc_moved[4];
extern int encoded[4];
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
    AudioAmplifier MixGain;
    // AudioAmplifier SongVol;
    AudioConnection *patchCord[16]; // total patchCordCount:122 including array typed ones.

    // constructor (this is called when class-object is created)
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
        patchCord[pci++] = new AudioConnection(Fenv, 0, filter, 1);
        patchCord[pci++] = new AudioConnection(filter, 0, fMixer, 0);
        patchCord[pci++] = new AudioConnection(filter, 1, fMixer, 1);

        patchCord[pci++] = new AudioConnection(filter, 2, fMixer, 2);
        patchCord[pci++] = new AudioConnection(ladder, 0, fMixer, 3);
        patchCord[pci++] = new AudioConnection(fMixer, 0, Aenv, 0);
        patchCord[pci++] = new AudioConnection(Aenv, 0, MixGain, 0);

        // patchCord[pci++] = new AudioConnection(MixGain, 0, SongVol, 0);
        // patchCord[pci++] = new AudioConnection(SongVol, 0, dacOut, 0);
    }
    virtual ~Plugin_9() = default;
    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;
    virtual void set_parameters(uint8_t row) override;
    virtual void draw_plugin() override;
    virtual void change_preset() override;
virtual void set_gain(uint8_t gain) override;
    void set_voice_waveform(uint8_t XPos, uint8_t YPos, const char *name);
    void set_voice_amplitude(uint8_t XPos, uint8_t YPos, const char *name);
    void set_voice_detune(uint8_t XPos, uint8_t YPos, const char *name);
    void set_voice1_waveform(uint8_t XPos, uint8_t YPos, const char *name);
    void set_voice1_amplitude(uint8_t XPos, uint8_t YPos, const char *name);
    void set_voice1_detune(uint8_t XPos, uint8_t YPos, const char *name);
    void set_LFO_waveform(uint8_t XPos, uint8_t YPos, const char *name); // make virtual in baseclass
    void assign_LFO_waveform(uint8_t value); // make virtual in baseclass but override
    void set_LFO_frequency(uint8_t XPos, uint8_t YPos, const char *name);
    void assign_LFO_frequency(uint8_t value); // make virtual in baseclass but override


    void set_LFO_amplitude(uint8_t XPos, uint8_t YPos, const char *name);
    void assign_LFO_amplitude(uint8_t value);

    void set_DC_amplitude(uint8_t XPos, uint8_t YPos, const char *name);
    void assign_DC_amplitude(uint8_t value);

    void set_filter_frequency(uint8_t XPos, uint8_t YPos, const char *name);
    void set_filter_resonance(uint8_t XPos, uint8_t YPos, const char *name, float min, float max);
    void set_filter_sweep(uint8_t XPos, uint8_t YPos, const char *name, float min, float max);
    void set_filter_type(uint8_t XPos, uint8_t YPos, const char *name);
    void selectFilterType(uint8_t mixerchannel);

    void set_envelope_attack(uint8_t XPos, uint8_t YPos, const char *name, int min, int max);
    void set_envelope_decay(uint8_t XPos, uint8_t YPos, const char *name, int min, int max);
    void set_envelope_sustain(uint8_t XPos, uint8_t YPos, const char *name);
    void set_envelope_release(uint8_t XPos, uint8_t YPos, const char *name, int min, int max);
    void assign_voice1_waveform(uint8_t value); // make virtual in baseclass but override
    void assign_voice1_amplitude(uint8_t value);
    // void set_voice1_detune(uint8_t XPos, uint8_t YPos, const char *name);
    void assign_voice2_waveform(uint8_t value); // make virtual in baseclass but override
    void assign_voice2_amplitude(uint8_t value);
    void set_voice2_detune(uint8_t XPos, uint8_t YPos, const char *name);

    void assign_filter_frequency(uint8_t value);
    void assign_filter_resonance(uint8_t value);
    void assign_filter_sweep(uint8_t value);
    void assign_filter_type(uint8_t mixerchannel);

    void assign_envelope1_attack(uint8_t value, int max);
    void assign_envelope1_decay(uint8_t value, int max);
    void assign_envelope1_sustain(uint8_t value);
    void assign_envelope1_release(uint8_t value, int max);
    void set_envelope1_ADSR(uint8_t YPos, int maxA, int maxD, int maxR);
};
#endif // PLUGIN_8_H
extern Plugin_9 plugin_9;
