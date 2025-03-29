#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "projectVariables.h"

#include <Plugins/Plugin_8.h>

extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;

void Plugin_8::setup()
{

    dc.amplitude(1);

    waveform.begin(WAVEFORM_SINE);
    waveform.amplitude(1);
    waveform1.begin(WAVEFORM_SINE);
    waveform1.amplitude(1);

    vcoMixer.gain(0, 1);
    vcoMixer.gain(1, 1);

    Fenv.delay(0);
    Fenv.attack(0);
    Fenv.hold(0);
    Fenv.decay(0);
    Fenv.sustain(1);
    Fenv.release(200);

    filter.frequency(2000);
    filter.resonance(1);
    filter.octaveControl(4);

    fMixer.gain(0, 1);
    fMixer.gain(1, 0);
    fMixer.gain(2, 0);
    fMixer.gain(3, 0);

    Aenv.delay(0);
    Aenv.attack(0);
    Aenv.hold(0);
    Aenv.decay(0);
    Aenv.sustain(1);
    Aenv.release(200);

    MixGain.gain(1);
    // SongVol.gain(1);
    Serial.println("pl8 setup done");
}
void Plugin_8::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    waveform.frequency((note_frequency[notePlayed] + detune1) * tuning);
    waveform1.frequency((note_frequency[notePlayed] + detune2) * tuning);
    // Serial.printf("pl8 detune1: %d\n", detune1);
    Fenv.noteOn();
    Aenv.noteOn();
}
void Plugin_8::noteOff(uint8_t notePlayed, uint8_t voice)
{
    Fenv.noteOff();
    Aenv.noteOff();
}

void Plugin_8::set_parameters(uint8_t row)
{
    draw_plugin();
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        if (row == 0)
        {
            set_voice_waveform(0, 0, "W~Form");
            set_voice_detune(1, 0, "Detune");
            set_voice_amplitude(2, 0, "Volume 1");
        }

        if (row == 1)
        {
            set_voice1_waveform(0, 1, "W~Form");
            set_voice1_detune(1, 1, "Detune");
            set_voice1_amplitude(2, 1, "Volume 2");
        }

        if (row == 2)
        {
            set_filter_frequency(0, 2, "Filt-Frq");
            set_filter_resonance(1, 2, "Resonance", 0.00, MAX_RESONANCE);
            set_filter_sweep(2, 2, "Sweep", 0.00, MAX_FILTERSWEEP);
            set_filter_type(3, 2, "Type");
        }

        if (row == 3)
        {
            set_envelope1_ADSR(3, 1000, 500, 2000);
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        set_presetNr();
    }
}
void Plugin_8::draw_plugin()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        // Serial.println("drawing plugin 2");
        drawPot(0, 0, potentiometer[presetNr][0], "W~Form");
        drawPot(1, 0, potentiometer[presetNr][1], "Detune");
        drawPot(2, 0, potentiometer[presetNr][2], "Volume 1");

        drawPot(0, 1, potentiometer[presetNr][4], "W~Form");
        drawPot(1, 1, potentiometer[presetNr][5], "Detune");
        drawPot(2, 1, potentiometer[presetNr][6], "Volume 2");

        drawPot(0, 2, potentiometer[presetNr][8], "Filt-Frq");
        drawPot(1, 2, potentiometer[presetNr][9], "Resonance");
        drawPot(2, 2, potentiometer[presetNr][10], "Sweep");
        drawPot(3, 2, potentiometer[presetNr][11], "Type");

        drawEnvelope(3, potentiometer[presetNr][12], potentiometer[presetNr][13],
                     potentiometer[presetNr][14], potentiometer[presetNr][15]);

        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 11, 4, 4, NO_VALUE, "Prset", ILI9341_BLUE, 2, false, false);
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 12, 4, 4, presetNr, NO_NAME, ILI9341_BLUE, 2, true, false);
    }
}
void Plugin_8::change_preset()
{
    assign_voice1_waveform(potentiometer[presetNr][0]);
    detune1 = (float)((note_frequency[potentiometer[presetNr][1]] * 0.01));
    assign_voice1_amplitude(potentiometer[presetNr][2]);

    assign_voice2_waveform(potentiometer[presetNr][4]);
    detune2 = (float)((note_frequency[potentiometer[presetNr][5]] * 0.01));
    assign_voice2_amplitude(potentiometer[presetNr][6]);

    assign_filter_frequency(potentiometer[presetNr][8]);
    assign_filter_resonance(potentiometer[presetNr][9]);
    assign_filter_sweep(potentiometer[presetNr][10]);
    selectFilterType(potentiometer[presetNr][11]);

    assign_envelope1_attack(potentiometer[presetNr][12], 1000);
    assign_envelope1_decay(potentiometer[presetNr][13], 500);
    assign_envelope1_sustain(potentiometer[presetNr][14]);
    assign_envelope1_release(potentiometer[presetNr][15], 2000);
}

void Plugin_8::set_voice_waveform(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        int walveform = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, 0, 12);

        waveform.begin(walveform);
    }
}
void Plugin_8::set_voice_amplitude(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float ampl = get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT;

        waveform.amplitude(ampl);
    }
}
void Plugin_8::set_voice_detune(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        detune1 = (float)((note_frequency[get_Potentiometer(XPos, YPos, name)] * 0.01));
    }
}
void Plugin_8::set_voice1_waveform(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        int walveform = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, 0, 12);
        waveform1.begin(walveform);
    }
}
void Plugin_8::set_voice1_amplitude(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float ampl = get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT;
        waveform1.amplitude(ampl);
    }
}
void Plugin_8::set_voice1_detune(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        detune2 = (float)((note_frequency[get_Potentiometer(XPos, YPos, name)] * 0.01));
    }
}

void Plugin_8::set_filter_frequency(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        int frequency = note_frequency[get_Potentiometer(XPos, YPos, name)] * tuning;
        filter.frequency(frequency);
    }
}
void Plugin_8::set_filter_resonance(uint8_t XPos, uint8_t YPos, const char *name, float min, float max)
{
    if (enc_moved[XPos])
    {
        float reso = get_Potentiometer(XPos, YPos, name) / (MIDI_CC_RANGE_FLOAT / max) + min;

        filter.resonance(reso);
    }
}
void Plugin_8::set_filter_sweep(uint8_t XPos, uint8_t YPos, const char *name, float min, float max)
{
    if (enc_moved[XPos])
    {
        float swp = get_Potentiometer(XPos, YPos, name) / (MIDI_CC_RANGE_FLOAT / max) + min;

        filter.octaveControl(swp);
    }
}
void Plugin_8::set_filter_type(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        selectFilterType(map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, 0, 2));
    }
}
void Plugin_8::selectFilterType(uint8_t mixerchannel)
{

    fMixer.gain(0, 0);
    fMixer.gain(1, 0);
    fMixer.gain(2, 0);
    fMixer.gain(mixerchannel, 1);
}

void Plugin_8::set_envelope_attack(uint8_t XPos, uint8_t YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int attack = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, min, max);

        Fenv.attack(attack);
        Aenv.attack(attack);
    }
}
void Plugin_8::set_envelope_decay(uint8_t XPos, uint8_t YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int decay = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, min, max);

        Fenv.decay(decay);
        Aenv.decay(decay);
    }
}
void Plugin_8::set_envelope_sustain(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float sustain = get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT;

        Fenv.sustain(sustain);
        Aenv.sustain(sustain);
    }
}
void Plugin_8::set_envelope_release(uint8_t XPos, uint8_t YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int release = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, min, max);

        Fenv.release(release);
        Aenv.release(release);
    }
}

void Plugin_8::assign_voice1_waveform(uint8_t value)
{
    int walveform = map(value, 0, MIDI_CC_RANGE, 0, 12);

    waveform.begin(walveform);
}
void Plugin_8::assign_voice1_amplitude(uint8_t value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;

    waveform.amplitude(ampl);
}

void Plugin_8::assign_voice2_waveform(uint8_t value)
{
    int walveform = map(value, 0, MIDI_CC_RANGE, 0, 12);

    waveform1.begin(walveform);
}
void Plugin_8::assign_voice2_amplitude(uint8_t value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;

    waveform1.amplitude(ampl);
}
void Plugin_8::set_voice2_detune(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        detune2 = (float)((note_frequency[get_Potentiometer(XPos, YPos, name)] * 0.01));
    }
}

void Plugin_8::assign_filter_frequency(uint8_t value)
{

    int frequency = note_frequency[value] * tuning;

    filter.frequency(frequency);
    ladder.frequency(frequency);
}
void Plugin_8::assign_filter_resonance(uint8_t value)
{

    float reso = value / 25.40;

    filter.resonance(reso);
    ladder.resonance(reso);
}
void Plugin_8::assign_filter_sweep(uint8_t value)
{
    float swp = value / 18.14;

    filter.octaveControl(swp);
    ladder.octaveControl(swp);
}
void Plugin_8::assign_filter_type(uint8_t mixerchannel)
{

    fMixer.gain(0, 0);
    fMixer.gain(1, 0);
    fMixer.gain(2, 0);
    fMixer.gain(3, 0);
    fMixer.gain(mixerchannel, 1);
}

void Plugin_8::assign_envelope1_attack(uint8_t value, int max)
{
    int attack = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.attack(attack);
    Aenv.attack(attack);
}
void Plugin_8::assign_envelope1_decay(uint8_t value, int max)
{
    int decay = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.decay(decay);
    Aenv.decay(decay);
}
void Plugin_8::assign_envelope1_sustain(uint8_t value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;

    Fenv.sustain(ampl);
    Aenv.sustain(ampl);
}
void Plugin_8::assign_envelope1_release(uint8_t value, int max)
{
    int release = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.release(release);
    Aenv.release(release);
}
void Plugin_8::set_envelope1_ADSR(uint8_t YPos, int maxA, int maxD, int maxR)
{

    if (enc_moved[0])
    {
        uint8_t rowIx = YPos * 4;
        potentiometer[presetNr][0 + rowIx] = constrain(potentiometer[presetNr][0 + rowIx] + encoded[0], 0, MIDI_CC_RANGE);
        assign_envelope1_attack(potentiometer[presetNr][0 + rowIx], maxA);
        drawEnvelope(YPos, potentiometer[presetNr][0 + rowIx], potentiometer[presetNr][1 + rowIx],
                     potentiometer[presetNr][2 + rowIx], potentiometer[presetNr][3 + rowIx]);
    }
    if (enc_moved[1])
    {
        uint8_t rowIx = YPos * 4;
        potentiometer[presetNr][1 + rowIx] = constrain(potentiometer[presetNr][1 + rowIx] + encoded[1], 0, MIDI_CC_RANGE);
        assign_envelope1_decay(potentiometer[presetNr][1 + rowIx], maxD);
        drawEnvelope(YPos, potentiometer[presetNr][0 + rowIx], potentiometer[presetNr][1 + rowIx],
                     potentiometer[presetNr][2 + rowIx], potentiometer[presetNr][3 + rowIx]);
    }
    if (enc_moved[2])
    {
        uint8_t rowIx = YPos * 4;
        potentiometer[presetNr][2 + rowIx] = constrain(potentiometer[presetNr][2 + rowIx] + encoded[2], 0, MIDI_CC_RANGE);
        assign_envelope1_sustain(potentiometer[presetNr][2 + rowIx]);
        drawEnvelope(YPos, potentiometer[presetNr][0 + rowIx], potentiometer[presetNr][1 + rowIx],
                     potentiometer[presetNr][2 + rowIx], potentiometer[presetNr][3 + rowIx]);
    }
    if (enc_moved[3])
    {
        uint8_t rowIx = YPos * 4;
        potentiometer[presetNr][3 + rowIx] = constrain(potentiometer[presetNr][3 + rowIx] + encoded[3], 0, MIDI_CC_RANGE);
        assign_envelope1_release(potentiometer[presetNr][3 + rowIx], maxR);
        drawEnvelope(YPos, potentiometer[presetNr][0 + rowIx], potentiometer[presetNr][1 + rowIx],
                     potentiometer[presetNr][2 + rowIx], potentiometer[presetNr][3 + rowIx]);
    }
}

Plugin_8 plugin_8("dTune", 8);
