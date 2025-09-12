#ifndef PLUGIN_14_H
#define PLUGIN_14_H

#include <Plugins/pluginClass.h>
#include <SerialFlash.h>
#include "TeensyVariablePlayback.h"
// #include "flashloader.h"
////#include "hardware/tftClass.h"
// class tftClass;
//  TeensyDAW: begin automatically generated code
//  Name: 1Osc
//  Description: Soundfont Synthesizer
//  Voices: 1

// VCO
// Pot 1: Waveform
// Pot 2: Vol
// Pot 3:
// Pot 4:

// Pot 5:
// Pot 6:
// Pot 7:
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


extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
void set_input_level(uint8_t _value);
class Plugin_14 : public PluginControll
{
public:
    AudioSynthWaveformDc dc;
    AudioInputI2S audioInput;
    AudioAmplifier inputGain;
    AudioAnalyzePeak peak1;  // xy=278,108
    AudioRecordQueue queue1; // xy=281,63
    AudioPlaySdResmp playMem;

    AudioEffectEnvelope Fenv;
    AudioFilterStateVariable filter;
    AudioMixer4 fMixer;
    AudioEffectEnvelope Aenv;
    // AudioMixer12 mixer;
    AudioMixer4 MixGain;


    AudioConnection *patchCord[13]; // total patchCordCount:98 including array typed ones.
    // draw waveforms
    int16_t pl14_customWaveform[GRID_LENGTH_HOR];
    int8_t pl14_oldCustomWaveformValue = 0;
    int8_t pl14_oldCustomWaveformXPos = 32;
    char *_playFilename;
    char *_recFilename;
    bool audio_rec_now;
    bool startrecording = false;
    // constructor (this is called when class-object is created)
    Plugin_14(const char *Name, uint8_t ID) : PluginControll(Name, ID)
    {

        int pci = 0; // used only for adding new patchcords

        // patchCord[pci++] = new AudioConnection(mixer, 0, MixGain, 0);
        patchCord[pci++] = new AudioConnection(dc, 0, Fenv, 0);
        patchCord[pci++] = new AudioConnection(playMem, 0, filter, 0);
        patchCord[pci++] = new AudioConnection(Fenv, 0, filter, 1);
        patchCord[pci++] = new AudioConnection(filter, 0, fMixer, 0);

        patchCord[pci++] = new AudioConnection(filter, 1, fMixer, 1);
        patchCord[pci++] = new AudioConnection(filter, 2, fMixer, 2);
        patchCord[pci++] = new AudioConnection(audioInput, 0, inputGain, 0);
        patchCord[pci++] = new AudioConnection(inputGain, 0, MixGain, 1);

        patchCord[pci++] = new AudioConnection(inputGain, 0, peak1, 0);
        patchCord[pci++] = new AudioConnection(inputGain, 0, queue1, 0);
        patchCord[pci++] = new AudioConnection(fMixer, 0, Aenv, 0);
        patchCord[pci++] = new AudioConnection(Aenv, 0, MixGain, 0);

        patchCord[pci++] = new AudioConnection(MixGain, 0, performFilter, 0);
        //  patchCord[pci++] = new AudioConnection(SongVol, 0, dacOut, 0);
    }
    virtual ~Plugin_14() = default;

    virtual void setup() override;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) override;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) override;
    virtual void assign_parameter(uint8_t pot) override;


    void assign_voice_waveform(uint8_t value); // make virtual in baseclass but override
    void draw_actual_waveform(uint8_t YPos);
    // draw waveforms
    void clearcustomWaveform(uint8_t YPos);
    void draw_customWaveform(uint8_t YPos);
    void redraw_customWaveform(int8_t YPos);
    void smooth_customWaveform(uint8_t YPos);
    void show_peak();

    void startRecording();
    void stopRecording();
    void continueRecording();
};
#endif // PLUGIN_10_H

extern Plugin_14 plugin_14;
