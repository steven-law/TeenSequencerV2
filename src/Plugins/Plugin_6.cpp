#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "ownLibs/Adafruit_ST7796S_kbv.h"
//#include <ILI9341_t3n.h>
//#include <ili9341_t3n_font_Arial.h> // from ILI9341_t3
#include "projectVariables.h"

#include <Plugins/Plugin_6.h>

extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;

void drawActiveRect(int xPos, uint8_t yPos, uint8_t xsize, uint8_t ysize, bool state, const char *name, int color);
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
        mixer.gain(i, 1);
        waveform[i].begin(WAVEFORM_SQUARE);
        waveform[i].amplitude(1);
    }
    Fenv.delay(0);
    Fenv.attack(0);
    Fenv.hold(0);
    Fenv.decay(0);
    Fenv.sustain(.7);
    Fenv.release(200);

    filter.frequency(2000);
    filter.resonance(1);
    filter.octaveControl(0);

    fMixer.gain(0, 1);
    fMixer.gain(1, 0);
    fMixer.gain(2, 0);
    fMixer.gain(3, 0);

    Aenv.delay(0);
    Aenv.attack(0);
    Aenv.hold(0);
    Aenv.decay(0);
    Aenv.sustain(.7);
    Aenv.release(200);

    MixGain.gain(1);
    // SongVol.gain(1);
    potentiometer[presetNr][0] = 0;
    potentiometer[presetNr][1] = 0;
    potentiometer[presetNr][2] = 0;
    potentiometer[presetNr][3] = 0;

    potentiometer[presetNr][4] = 0;
    potentiometer[presetNr][5] = 3;
    potentiometer[presetNr][6] = 7;
    potentiometer[presetNr][7] = 10;

    potentiometer[presetNr][8] = 32;
    potentiometer[presetNr][9] = 32;
    potentiometer[presetNr][10] = 32;
    potentiometer[presetNr][11] = 32;

    potentiometer[presetNr][12] = 40;
    potentiometer[presetNr][13] = 0;
    potentiometer[presetNr][14] = 0;
    potentiometer[presetNr][15] = 10;

    Serial.println("pl6 setup done");
}
void Plugin_6::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    float frequency[PL6_VOICES];
    for (int i = 0; i < PL6_VOICES; i++)
    {
        frequency[i] = note_frequency[notePlayed + pl6Offset[i]] * tuning;
        waveform[i].frequency(frequency[i]);
        Serial.printf("pl6 note %d, v= %d, freq: %f\n", notePlayed + pl6Offset[i], i, frequency[i]);
    }

    Fenv.noteOn();
    Aenv.noteOn();
}
void Plugin_6::noteOff(uint8_t notePlayed, uint8_t voice)
{
    Fenv.noteOff();
    Aenv.noteOff();
}

void Plugin_6::set_parameters(uint8_t row)
{
    draw_plugin();
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        if (row == 0)
        {
            set_voice_waveform(0, 0, "W~F 1");
            set_voice_waveform(1, 0, "W~F 2");
            set_voice_waveform(2, 0, "W~F 3");
            set_voice_waveform(3, 0, "W~F 4");
        }

        if (row == 1)
        {
            set_voice_offset(0, 1, "Note 1");
            set_voice_offset(1, 1, "Note 2");
            set_voice_offset(2, 1, "Note 3");
            set_voice_offset(3, 1, "Note 4");
        }

        if (row == 2)
        {
            set_voice_amplitude(0, 0, 2, "Vol 1");
            set_voice_amplitude(1, 1, 2, "Vol 2");
            set_voice_amplitude(2, 2, 2, "Vol 3");
            set_voice_amplitude(3, 3, 2, "Vol 4");
            //  set_filter_sweep(2, 2, "Sweep");
            //  set_filter_type(3, 2, filterName[potentiometer[presetNr][11]]);
        }

        if (row == 3)
        {
            set_filter_frequency(0, 3, "Filt-Frq");
            set_filter_resonance(1, 3, "Resonance");
            set_filter_sweep(2, 3, "Sweep");
            set_envelope_release(3, 3, "Release", 2000);
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
        //change_plugin_row = false;
        // Serial.println("drawing plugin 2");
        drawPot(0, 0, potentiometer[presetNr][0], "W~F 1");
        drawPot(1, 0, potentiometer[presetNr][1], "W~F 2");
        drawPot(2, 0, potentiometer[presetNr][2], "W~F 3");
        drawPot(3, 0, potentiometer[presetNr][3], "W~F 4");

        drawPot(0, 1, potentiometer[presetNr][4], "Note 1");
        drawPot(1, 1, potentiometer[presetNr][5], "Note 2");
        drawPot(2, 1, potentiometer[presetNr][6], "Note 3");
        drawPot(3, 1, potentiometer[presetNr][7], "Note 4");

        drawPot(0, 2, potentiometer[presetNr][8], "Vol 1");
        drawPot(1, 2, potentiometer[presetNr][9], "Vol 2");
        drawPot(2, 2, potentiometer[presetNr][10], "Vol 3");
        drawPot(3, 2, potentiometer[presetNr][11], "Vol 4");

        drawPot(0, 3, potentiometer[presetNr][12], "Filt-Frq");
        drawPot(1, 3, potentiometer[presetNr][13], "Resonance");
        drawPot(2, 3, potentiometer[presetNr][14], "Sweep");
        drawPot(3, 3, potentiometer[presetNr][15], "Release");

        // drawPot(2, 2, potentiometer[presetNr][10], "Sweep");
        // drawPot(3, 2, potentiometer[presetNr][11], filterName[potentiometer[presetNr][11]]);

        /* drawEnvelope(3, potentiometer[presetNr][12], potentiometer[presetNr][13],
                      potentiometer[presetNr][14], potentiometer[presetNr][15]);
 */
        // drawPot(1, 3, potentiometer[presetNr][13], "Decay");
        // drawPot(2, 3, potentiometer[presetNr][14], "Sustain");

        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 11, 4, 4, NO_VALUE, "Prset", ILI9341_BLUE, 2, false, false);
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 12, 4, 4, presetNr, NO_NAME, ILI9341_BLUE, 2, true, false);
    }
}
void Plugin_6::change_preset()
{

    assign_voice_waveform(0, potentiometer[presetNr][0]);
    assign_voice_waveform(1, potentiometer[presetNr][1]);
    assign_voice_waveform(2, potentiometer[presetNr][2]);
    assign_voice_waveform(3, potentiometer[presetNr][3]);

    assign_voice_offset(0, potentiometer[presetNr][4]);
    assign_voice_offset(1, potentiometer[presetNr][5]);
    assign_voice_offset(2, potentiometer[presetNr][6]);
    assign_voice_offset(3, potentiometer[presetNr][7]);

    assign_voice_amplitude(0, potentiometer[presetNr][8]);
    assign_voice_amplitude(1, potentiometer[presetNr][9]);
    assign_voice_amplitude(2, potentiometer[presetNr][10]);
    assign_voice_amplitude(3, potentiometer[presetNr][11]);

    assign_filter_frequency(potentiometer[presetNr][12]);
    assign_filter_resonance(potentiometer[presetNr][13]);

    assign_envelope_attack(potentiometer[presetNr][14], 1000);
    assign_envelope_release(potentiometer[presetNr][15], 2000);
}
void Plugin_6::set_voice_waveform(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        int walveform = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, 0, 12);

        assign_voice_waveform(XPos, walveform);
    }
}
void Plugin_6::assign_voice_waveform(uint8_t voice, uint8_t value)
{
    waveform[voice].begin(value);
}

void Plugin_6::set_voice_offset(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        int walveform = get_Potentiometer(XPos, YPos, name);

        assign_voice_offset(XPos, walveform);
    }
}
void Plugin_6::assign_voice_offset(uint8_t voice, uint8_t value)
{
    pl6Offset[voice] = value;
}
void Plugin_6::set_voice_amplitude(uint8_t voice, uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_voice_amplitude(voice, get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_6::assign_voice_amplitude(uint8_t voice, uint8_t value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;

    waveform[voice].amplitude(ampl);
}

void Plugin_6::set_filter_frequency(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_filter_frequency(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_6::assign_filter_frequency(uint8_t value)
{

    int frequency = note_frequency[value] * tuning;

    filter.frequency(frequency);
    // ladder.frequency(frequency);
}

void Plugin_6::set_filter_resonance(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_filter_resonance(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_6::assign_filter_resonance(uint8_t value)
{

    float reso = value / 25.40;

    filter.resonance(reso);
    // ladder.resonance(reso);
}
void Plugin_6::set_filter_sweep(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_filter_sweep(map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, 0, 7.00));
    }
}
void Plugin_6::assign_filter_sweep(uint8_t value)
{
    // float swp = value / 18.14;

    filter.octaveControl(value);
    // ladder.octaveControl(swp);
}
void Plugin_6::set_filter_type(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        selectFilterType(constrain(get_Potentiometer(XPos, YPos, name), 0, 3));
    }
}
void Plugin_6::selectFilterType(uint8_t mixerchannel)
{

    fMixer.gain(0, 0);
    fMixer.gain(1, 0);
    fMixer.gain(2, 0);
    fMixer.gain(3, 0);
    fMixer.gain(mixerchannel, 1);
}

void Plugin_6::assign_envelope_attack(uint8_t value, int max)
{
    int attack = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.attack(attack);
    Aenv.attack(attack);
}
void Plugin_6::assign_envelope_decay(uint8_t value, int max)
{
    int decay = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.decay(decay);
    Aenv.decay(decay);
}
void Plugin_6::assign_envelope_sustain(uint8_t value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;

    Fenv.sustain(ampl);
    Aenv.sustain(ampl);
}
void Plugin_6::assign_envelope_release(uint8_t value, int max)
{
    int release = map(value, 0, MIDI_CC_RANGE, 0, max);
    Fenv.attack(release / 2);
    Aenv.attack(release / 2);
    Fenv.decay(release / 3);
    Aenv.decay(release / 3);
    Fenv.release(release);
    Aenv.release(release);
}
void Plugin_6::set_envelope_ADSR(uint8_t YPos, int maxA, int maxD, int maxR)
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
void Plugin_6::set_envelope_attack(uint8_t XPos, uint8_t YPos, const char *name, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_attack(get_Potentiometer(XPos, YPos, name), max);
    }
}
void Plugin_6::set_envelope_decay(uint8_t XPos, uint8_t YPos, const char *name, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_decay(get_Potentiometer(XPos, YPos, name), max);
    }
}
void Plugin_6::set_envelope_sustain(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {

        assign_envelope_sustain(get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT);
    }
}
void Plugin_6::set_envelope_release(uint8_t XPos, uint8_t YPos, const char *name, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_release(get_Potentiometer(XPos, YPos, name), max);
    }
} // TeensyDAW: end automatically generated code

Plugin_6 plugin_6("Adtv", 6);
