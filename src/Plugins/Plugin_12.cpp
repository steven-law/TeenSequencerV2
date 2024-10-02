#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "projectVariables.h"

#include <Plugins/Plugin_12.h>
////#include "hardware/tftClass.h"
// class tftClass;
extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
// void clearWorkSpace();
extern const char *filterName[4];

void Plugin_12::setup()
{

    dc.amplitude(1);

    // waveform.begin(WAVEFORM_SINE);
    // waveform.amplitude(1);
waveform.enableInterpolation(true);
    Fenv.delay(0);
    Fenv.attack(0);
    Fenv.hold(0);
    Fenv.decay(0);
    Fenv.sustain(1);
    Fenv.release(200);

    filter.frequency(2000);
    filter.resonance(1);
    filter.octaveControl(4);
    // ladder[i].frequency(2000);
    // ladder[i].resonance(1);
    // ladder[i].octaveControl(4);

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

    // mixer.gain(0, 1);

    MixGain.gain(1);
    potentiometer[presetNr][0]=0;
potentiometer[presetNr][1]=1;
potentiometer[presetNr][1]=48;
potentiometer[presetNr][8]=60;
potentiometer[presetNr][9]=0;
potentiometer[presetNr][10]=30;
potentiometer[presetNr][11]=0;
potentiometer[presetNr][12]=5;
potentiometer[presetNr][13]=0;
potentiometer[presetNr][14]=127;
potentiometer[presetNr][15]=20;
    // SongVol.gain(1);
}
void Plugin_12::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    //float frequency = note_frequency[notePlayed] * tuning;
    // waveform.frequency(frequency);
    waveform.setPlaybackRate(notePlayed / MIDI_CC_RANGE_FLOAT);
    waveform.playRaw(_filename, 1);
    Fenv.noteOn();
    Aenv.noteOn();
    Serial.println("PL12 NOteOn");
    Serial.println(_filename);
}
void Plugin_12::noteOff(uint8_t notePlayed, uint8_t voice)
{
    Fenv.noteOff();
    Aenv.noteOff();
}
void Plugin_12::set_parameters(uint8_t row)
{
    draw_plugin();
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        if (row == 0)
        {

            set_voice_bank(0, 0, bankNames[potentiometer[presetNr][0]]);
            set_voice_waveform(1, 0, "W~Form");
            set_voice_amplitude(2, 0, "Volume");
        }

        if (row == 1)
        {
        }

        if (row == 2)
        {
            set_filter_frequency(0, 2, "Filt-Frq");
            set_filter_resonance(1, 2, "Resonance", 0, 5.00);
            set_filter_sweep(2, 2, "Sweep");
            set_filter_type(3, 2, filterName[potentiometer[presetNr][11]]);
        }

        if (row == 3)
        {
            set_envelope_ADSR(3, 1000, 500, 2000);
            // set_envelope_attack(0, 3, "Attack", 0, 1000);
            // set_envelope_sustain(2, 3, "Sustain");
            // set_envelope_decay(1, 3, "Decay", 0, 500);
            // set_envelope_release(3, 3, "Release", 0, 2000);
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        set_presetNr();
    }
}
void Plugin_12::draw_plugin()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        clearWorkSpace();
        // Serial.println("drawing plugin 2");
        drawPot(0, 0, potentiometer[presetNr][0], bankNames[potentiometer[presetNr][0]]);
        drawPot(1, 0, potentiometer[presetNr][1], "W~Form");
        drawPot(2, 0, potentiometer[presetNr][2], "Volume");

        drawPot(0, 2, potentiometer[presetNr][8], "Filt-Frq");
        drawPot(1, 2, potentiometer[presetNr][9], "Resonance");
        drawPot(2, 2, potentiometer[presetNr][10], "Sweep");
        drawPot(3, 2, potentiometer[presetNr][11], filterName[potentiometer[presetNr][11]]);

        drawEnvelope(3, potentiometer[presetNr][12], potentiometer[presetNr][13],
                     potentiometer[presetNr][14], potentiometer[presetNr][15]);
        // drawPot(0, 3, potentiometer[presetNr][12], "Attack");
        // drawPot(1, 3, potentiometer[presetNr][13], "Decay");
        // drawPot(2, 3, potentiometer[presetNr][14], "Sustain");
        // drawPot(3, 3, potentiometer[presetNr][15], "Release");

        // draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "Prset", presetNr, 3, 0);
    }
}
void Plugin_12::change_preset()
{
    assign_voice_bank(potentiometer[presetNr][0]);

    assign_voice_waveform(potentiometer[presetNr][1]);
    assign_voice_amplitude(potentiometer[presetNr][2]);

    assign_filter_frequency(potentiometer[presetNr][8]);
    assign_filter_resonance(potentiometer[presetNr][9]);
    assign_filter_sweep(potentiometer[presetNr][10]);
    selectFilterType(potentiometer[presetNr][11]);

    assign_envelope_attack(potentiometer[presetNr][12], 1000);
    assign_envelope_decay(potentiometer[presetNr][13], 500);
    assign_envelope_sustain(potentiometer[presetNr][14]);
    assign_envelope_release(potentiometer[presetNr][15], 2000);
}
void Plugin_12::set_voice_bank(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_voice_bank(constrain(get_Potentiometer(XPos, YPos, name),0,7));
    }
}
void Plugin_12::assign_voice_bank(uint8_t value)
{
   
    //uint8_t walveform = map(value, 0, MIDI_CC_RANGE, 0, 12);
    sprintf(_filename, "%s%d.raw\0", bankNames[value] ,potentiometer[presetNr][1]);

    //waveform.begin(walveform);
}
void Plugin_12::set_voice_waveform(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_voice_waveform(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_12::assign_voice_waveform(uint8_t value)
{
   
    //uint8_t walveform = map(value, 0, MIDI_CC_RANGE, 0, 12);
    sprintf(_filename, "%s%d.raw\0", bankNames[potentiometer[presetNr][0]], value);

    //waveform.begin(walveform);
}

void Plugin_12::set_voice_amplitude(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_voice_amplitude(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_12::assign_voice_amplitude(uint8_t value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;

    fMixer.gain(0, ampl);
    fMixer.gain(1, ampl);
    fMixer.gain(2, ampl);
    fMixer.gain(3, ampl);
}

void Plugin_12::set_filter_frequency(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_filter_frequency(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_12::assign_filter_frequency(uint8_t value)
{

    int frequency = note_frequency[value] * tuning;

    filter.frequency(frequency);
    // ladder[i].frequency(frequency);
}

void Plugin_12::set_filter_resonance(uint8_t XPos, uint8_t YPos, const char *name, float min, float max)
{
    if (enc_moved[XPos])
    {
        assign_filter_resonance(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_12::assign_filter_resonance(uint8_t value)
{

    float reso = value / 25.40;

    filter.resonance(reso);
    // ladder[i].resonance(reso);
}
void Plugin_12::set_filter_sweep(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_filter_sweep(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_12::assign_filter_sweep(uint8_t value)
{
    float swp = value / 18.14;

    filter.octaveControl(swp);
    // ladder[i].octaveControl(swp);
}
void Plugin_12::set_filter_type(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        selectFilterType(constrain(get_Potentiometer(XPos, YPos, name), 0, 3));
    }
}
void Plugin_12::selectFilterType(uint8_t mixerchannel)
{

    fMixer.gain(0, 0);
    fMixer.gain(1, 0);
    fMixer.gain(2, 0);
    fMixer.gain(3, 0);
    fMixer.gain(mixerchannel, 1);
}

void Plugin_12::assign_envelope_attack(uint8_t value, int max)
{
    int attack = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.attack(attack);
    Aenv.attack(attack);
}
void Plugin_12::assign_envelope_decay(uint8_t value, int max)
{
    int decay = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.decay(decay);
    Aenv.decay(decay);
}
void Plugin_12::assign_envelope_sustain(uint8_t value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;

    Fenv.sustain(ampl);
    Aenv.sustain(ampl);
}
void Plugin_12::assign_envelope_release(uint8_t value, int max)
{
    int release = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.release(release);
    Aenv.release(release);
}
void Plugin_12::set_envelope_ADSR(uint8_t YPos, int maxA, int maxD, int maxR)
{

    if (enc_moved[0])
    {
        uint8_t rowIx = YPos * 4;
        potentiometer[presetNr][0 + rowIx] = constrain(potentiometer[presetNr][0 + rowIx] + encoded[0], 0, MIDI_CC_RANGE);
        assign_envelope_attack(potentiometer[presetNr][0 + rowIx], maxA);
        drawEnvelope(YPos, potentiometer[presetNr][0 + rowIx], potentiometer[presetNr][1 + rowIx],
                     potentiometer[presetNr][2 + rowIx], potentiometer[presetNr][3 + rowIx]);
    }
    if (enc_moved[1])
    {
        uint8_t rowIx = YPos * 4;
        potentiometer[presetNr][1 + rowIx] = constrain(potentiometer[presetNr][1 + rowIx] + encoded[1], 0, MIDI_CC_RANGE);
        assign_envelope_decay(potentiometer[presetNr][1 + rowIx], maxD);
        drawEnvelope(YPos, potentiometer[presetNr][0 + rowIx], potentiometer[presetNr][1 + rowIx],
                     potentiometer[presetNr][2 + rowIx], potentiometer[presetNr][3 + rowIx]);
    }
    if (enc_moved[2])
    {
        uint8_t rowIx = YPos * 4;
        potentiometer[presetNr][2 + rowIx] = constrain(potentiometer[presetNr][2 + rowIx] + encoded[2], 0, MIDI_CC_RANGE);
        assign_envelope_sustain(potentiometer[presetNr][2 + rowIx]);
        drawEnvelope(YPos, potentiometer[presetNr][0 + rowIx], potentiometer[presetNr][1 + rowIx],
                     potentiometer[presetNr][2 + rowIx], potentiometer[presetNr][3 + rowIx]);
    }
    if (enc_moved[3])
    {
        uint8_t rowIx = YPos * 4;
        potentiometer[presetNr][3 + rowIx] = constrain(potentiometer[presetNr][3 + rowIx] + encoded[3], 0, MIDI_CC_RANGE);
        assign_envelope_release(potentiometer[presetNr][3 + rowIx], maxR);
        drawEnvelope(YPos, potentiometer[presetNr][0 + rowIx], potentiometer[presetNr][1 + rowIx],
                     potentiometer[presetNr][2 + rowIx], potentiometer[presetNr][3 + rowIx]);
    }
}
void Plugin_12::set_envelope_attack(uint8_t XPos, uint8_t YPos, const char *name, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_attack(get_Potentiometer(XPos, YPos, name), max);
    }
}
void Plugin_12::set_envelope_decay(uint8_t XPos, uint8_t YPos, const char *name, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_decay(get_Potentiometer(XPos, YPos, name), max);
    }
}
void Plugin_12::set_envelope_sustain(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {

        assign_envelope_sustain(get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT);
    }
}
void Plugin_12::set_envelope_release(uint8_t XPos, uint8_t YPos, const char *name, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_release(get_Potentiometer(XPos, YPos, name), max);
    }
}

Plugin_12 plugin_12("REC", 11);