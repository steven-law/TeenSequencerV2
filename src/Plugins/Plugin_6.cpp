#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include <ILI9341_t3n.h>
#include <ili9341_t3n_font_Arial.h> // from ILI9341_t3
#include "projectVariables.h"

#include <Plugins/Plugin_6.h>

extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
void clearWorkSpace();

void drawActiveRect(int xPos, byte yPos, byte xsize, byte ysize, bool state, const char *name, int color);
void myDrawLine(int x0, int y0, int x1, int y1, uint16_t color);
void myDrawRect(int x, int y, int w, int h, uint16_t color);

extern int pixelTouchX;
extern int gridTouchY;
// TeensyDAW: begin automatically generated code
// Name: Draw
// Description: Additive Synthesizer
// Voices: 1

// VCO
// Pot 1: Vol
// Pot 2: Vol
// Pot 3: Vol
// Pot 4: Vol

// Pot 5: Vol
// Pot 6: Vol
// Pot 7: Vol
// Pot 8: Vol

// Filter:
// Pot 9: Frequency
// Pot 10: Resonance
// Pot 11: Sweep
// Pot 12: Type

// Envelope:
// Pot 9: Attack
// Pot 10: Decay
// Pot 11: Sustain
// Pot 12: Release

void Plugin_6::setup()
{

    dc.amplitude(1);
    for (int i = 0; i < PL6_VOICES; i++)
    {
        mixer.gain(i,1);
        waveform[i].begin(WAVEFORM_SQUARE);
        waveform[i].amplitude(1);
    }
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
    SongVol.gain(1);

    Serial.println("pl6 setup done");
}
void Plugin_6::noteOn(byte notePlayed, float velocity, byte voice)
{
    float frequency = note_frequency[notePlayed] * tuning;
    waveform[0].frequency(frequency);
    waveform[1].frequency(frequency * 2);
    waveform[2].frequency(frequency * 3);
    waveform[3].frequency(frequency * 4);
    waveform[4].frequency(frequency * 6);
    waveform[5].frequency(frequency * 8);
    waveform[6].frequency(frequency * 10);
    waveform[7].frequency(frequency * 12);
    Fenv.noteOn();
    Aenv.noteOn();
}
void Plugin_6::noteOff(byte notePlayed, byte voice)
{
    Fenv.noteOff();
    Aenv.noteOff();
}

void Plugin_6::set_parameters(byte row)
{
    draw_plugin();
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        if (row == 0)
        {
            set_voice_amplitude(0, 0, 0, "16*");
            set_voice_amplitude(1, 1, 0, "8*");
            set_voice_amplitude(2, 2, 0, "5 1/3");
            set_voice_amplitude(3, 3, 0, "4*");
        }

        if (row == 1)
        {
            set_voice_amplitude(4, 0, 1, "2 2/3");
            set_voice_amplitude(5, 1, 1, "2*");
            set_voice_amplitude(6, 2, 1, "1 3/5");
            set_voice_amplitude(7, 3, 1, "1 1/3");
        }

        if (row == 2)
        {
            set_filter_frequency(0, 2, "Filt-Frq");
            set_filter_resonance(1, 2, "Resonance");
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

void Plugin_6::draw_plugin()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        clearWorkSpace();
        // Serial.println("drawing plugin 2");
        drawPot(0, 0, potentiometer[presetNr][0], "16*");
        drawPot(1, 0, potentiometer[presetNr][1], "8*");
        drawPot(2, 0, potentiometer[presetNr][2], "5 1/3");
        drawPot(3, 0, potentiometer[presetNr][3], "4*");

        drawPot(0, 1, potentiometer[presetNr][4], "2 2/3");
        drawPot(1, 1, potentiometer[presetNr][5], "2*");
        drawPot(2, 1, potentiometer[presetNr][6], "1 3/5");
        drawPot(3, 1, potentiometer[presetNr][7], "1 1/3");

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

        //draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "Prset", presetNr, 3, 0);
    }
}
void Plugin_6::change_preset(){
    
assign_voice_amplitude(0,potentiometer[presetNr][0]);
assign_voice_amplitude(1,potentiometer[presetNr][1]);
assign_voice_amplitude(2,potentiometer[presetNr][2]);
assign_voice_amplitude(3,potentiometer[presetNr][3]);
assign_voice_amplitude(4,potentiometer[presetNr][4]);
assign_voice_amplitude(5,potentiometer[presetNr][5]);
assign_voice_amplitude(6,potentiometer[presetNr][6]);
assign_voice_amplitude(7,potentiometer[presetNr][7]);


assign_filter_frequency(potentiometer[presetNr][8]);
assign_filter_resonance(potentiometer[presetNr][9]);
assign_filter_sweep(potentiometer[presetNr][10]);
selectFilterType(potentiometer[presetNr][11]);

assign_envelope_attack(potentiometer[presetNr][12], 1000);
assign_envelope_decay(potentiometer[presetNr][13], 500);
assign_envelope_sustain(potentiometer[presetNr][14]);
assign_envelope_release(potentiometer[presetNr][15], 2000);
}
void Plugin_6::set_voice_waveform(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
       // int walveform = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, 0, 12);

        // waveform.begin(walveform);
    }
}

void Plugin_6::set_voice_amplitude(byte voice, byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_voice_amplitude(voice, get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_6::assign_voice_amplitude(byte voice, byte value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;

    waveform[voice].amplitude(ampl);
}

void Plugin_6::set_filter_frequency(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_filter_frequency(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_6::assign_filter_frequency(byte value)
{

    int frequency = note_frequency[value] * tuning;

    filter.frequency(frequency);
    // ladder.frequency(frequency);
}

void Plugin_6::set_filter_resonance(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_filter_resonance(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_6::assign_filter_resonance(byte value)
{

    float reso = value / 25.40;

    filter.resonance(reso);
    // ladder.resonance(reso);
}
void Plugin_6::set_filter_sweep(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_filter_sweep(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_6::assign_filter_sweep(byte value)
{
    float swp = value / 18.14;

    filter.octaveControl(swp);
    // ladder.octaveControl(swp);
}
void Plugin_6::set_filter_type(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        selectFilterType(constrain(get_Potentiometer(XPos, YPos, name), 0, 3));
    }
}
void Plugin_6::selectFilterType(byte mixerchannel)
{

    fMixer.gain(0, 0);
    fMixer.gain(1, 0);
    fMixer.gain(2, 0);
    fMixer.gain(3, 0);
    fMixer.gain(mixerchannel, 1);
}

void Plugin_6::assign_envelope_attack(byte value, int max)
{
    int attack = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.attack(attack);
    Aenv.attack(attack);
}
void Plugin_6::assign_envelope_decay(byte value, int max)
{
    int decay = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.decay(decay);
    Aenv.decay(decay);
}
void Plugin_6::assign_envelope_sustain(byte value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;

    Fenv.sustain(ampl);
    Aenv.sustain(ampl);
}
void Plugin_6::assign_envelope_release(byte value, int max)
{
    int release = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.release(release);
    Aenv.release(release);
}
void Plugin_6::set_envelope_ADSR(byte YPos, int maxA, int maxD, int maxR)
{

    if (enc_moved[0])
    {
        byte rowIx = YPos * 4;
        potentiometer[presetNr][0 + rowIx] = constrain(potentiometer[presetNr][0 + rowIx] + encoded[0], 0, MIDI_CC_RANGE);
        assign_envelope_attack(potentiometer[presetNr][0 + rowIx], maxA);
        drawEnvelope(YPos, potentiometer[presetNr][0 + rowIx], potentiometer[presetNr][1 + rowIx],
                     potentiometer[presetNr][2 + rowIx], potentiometer[presetNr][3 + rowIx]);
    }
    if (enc_moved[1])
    {
        byte rowIx = YPos * 4;
        potentiometer[presetNr][1 + rowIx] = constrain(potentiometer[presetNr][1 + rowIx] + encoded[1], 0, MIDI_CC_RANGE);
        assign_envelope_decay(potentiometer[presetNr][1 + rowIx], maxD);
        drawEnvelope(YPos, potentiometer[presetNr][0 + rowIx], potentiometer[presetNr][1 + rowIx],
                     potentiometer[presetNr][2 + rowIx], potentiometer[presetNr][3 + rowIx]);
    }
    if (enc_moved[2])
    {
        byte rowIx = YPos * 4;
        potentiometer[presetNr][2 + rowIx] = constrain(potentiometer[presetNr][2 + rowIx] + encoded[2], 0, MIDI_CC_RANGE);
        assign_envelope_sustain(potentiometer[presetNr][2 + rowIx]);
        drawEnvelope(YPos, potentiometer[presetNr][0 + rowIx], potentiometer[presetNr][1 + rowIx],
                     potentiometer[presetNr][2 + rowIx], potentiometer[presetNr][3 + rowIx]);
    }
    if (enc_moved[3])
    {
        byte rowIx = YPos * 4;
        potentiometer[presetNr][3 + rowIx] = constrain(potentiometer[presetNr][3 + rowIx] + encoded[3], 0, MIDI_CC_RANGE);
        assign_envelope_release(potentiometer[presetNr][3 + rowIx], maxR);
        drawEnvelope(YPos, potentiometer[presetNr][0 + rowIx], potentiometer[presetNr][1 + rowIx],
                     potentiometer[presetNr][2 + rowIx], potentiometer[presetNr][3 + rowIx]);
    }
}
void Plugin_6::set_envelope_attack(byte XPos, byte YPos, const char *name, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_attack(get_Potentiometer(XPos, YPos, name), max);
    }
}
void Plugin_6::set_envelope_decay(byte XPos, byte YPos, const char *name, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_decay(get_Potentiometer(XPos, YPos, name), max);
    }
}
void Plugin_6::set_envelope_sustain(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {

        assign_envelope_sustain(get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT);
    }
}
void Plugin_6::set_envelope_release(byte XPos, byte YPos, const char *name, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_release(get_Potentiometer(XPos, YPos, name), max);
    }
} // TeensyDAW: end automatically generated code

Plugin_6 plugin_6("Adtv", 6);
