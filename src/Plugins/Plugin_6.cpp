#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "ownLibs/Adafruit_ST7796S_kbv.h"
// #include <ILI9341_t3n.h>
// #include <ili9341_t3n_font_Arial.h> // from ILI9341_t3
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
    float velo = (velocity * (MixerGain / MIDI_CC_RANGE_FLOAT));
    MixGain.gain(velo);
    for (int i = 0; i < PL6_VOICES; i++)
    {
        frequency[i] = note_frequency[notePlayed + chords[pl6ChoosenChord][i]] * tuning;
        waveform[i].frequency(frequency[i]);
        Serial.printf("pl6 note %d, v= %d, freq: %f\n", notePlayed + chords[pl6ChoosenChord][i], i, frequency[i]);
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
            set_voice_waveform(0, 0);
            set_voice_waveform(1, 0);
            set_voice_waveform(2, 0);
            set_voice_waveform(3, 0);
        }

        if (row == 1)
        {
            set_voice_offset(0, 1);
            set_voice_amplitude(1, 1, 1);
            //  set_voice_offset(2, 1, "Note 3");
            //  set_voice_offset(3, 1, "Note 4");
        }

        if (row == 2)
        {
            set_filter_frequency(0, 2);
            set_filter_resonance(1, 2);
            set_filter_sweep(2, 2);
            // set_envelope_release(3, 2, "Release", 2000);
        }

        if (row == 3)
        {

            set_envelope_ADSR(3, 1000, 500, 2000);
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        set_presetNr();
    }
}
void Plugin_6::set_gain(uint8_t gain)
{
    MixerGain = gain;
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

    assign_envelope_release(potentiometer[presetNr][15], 2000);
}

void Plugin_6::set_voice_waveform(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        int walveform = map(get_Potentiometer(XPos, YPos), 0, MIDI_CC_RANGE, 0, 12);

        assign_voice_waveform(XPos, walveform);
    }
}
void Plugin_6::assign_voice_waveform(uint8_t voice, uint8_t value)
{
    waveform[voice].begin(value);
}

void Plugin_6::set_voice_offset(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        int chordType = get_Potentiometer(XPos, YPos);

        assign_voice_offset(XPos, chordType);
    }
}
void Plugin_6::assign_voice_offset(uint8_t voice, uint8_t value)
{
    pl6ChoosenChord = value;
}

void Plugin_6::set_voice_amplitude(uint8_t voice, uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        assign_voice_amplitude(voice, get_Potentiometer(XPos, YPos));
    }
}
void Plugin_6::assign_voice_amplitude(uint8_t voice, uint8_t value)
{
    for (int i = 0; i < PL6_VOICES; i++)
    {
        float ampl = chordVolumes[value][voice] / MIDI_CC_RANGE_FLOAT;
        waveform[i].amplitude(ampl);
    }
}

void Plugin_6::set_filter_frequency(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        assign_filter_frequency(get_Potentiometer(XPos, YPos));
    }
}
void Plugin_6::assign_filter_frequency(uint8_t value)
{

    int frequency = note_frequency[value] * tuning;

    filter.frequency(frequency);
    // ladder.frequency(frequency);
}
void Plugin_6::set_filter_resonance(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        assign_filter_resonance(get_Potentiometer(XPos, YPos));
    }
}
void Plugin_6::assign_filter_resonance(uint8_t value)
{

    float reso = value / 25.40;

    filter.resonance(reso);
    // ladder.resonance(reso);
}
void Plugin_6::set_filter_sweep(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        assign_filter_sweep(map(get_Potentiometer(XPos, YPos), 0, MIDI_CC_RANGE, 0, 7.00));
    }
}
void Plugin_6::assign_filter_sweep(uint8_t value)
{
    // float swp = value / 18.14;

    filter.octaveControl(value);
    // ladder.octaveControl(swp);
}

void Plugin_6::set_envelope_release(uint8_t XPos, uint8_t YPos, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_release(get_Potentiometer(XPos, YPos), max);
    }
} // TeensyDAW: end automatically generated code

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
Plugin_6 plugin_6("Adtv", 6);
