
#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "projectVariables.h"

#include <Plugins/Plugin_3.h>

extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
// void clearWorkSpace();

void Plugin_3::setup()
{

    modulator.begin(WAVEFORM_SINE);
    modulator.frequency(440);
    modulator.amplitude(1);

    modEnv.delay(0);
    modEnv.attack(0);
    modEnv.hold(0);
    modEnv.decay(0);
    modEnv.sustain(1);
    modEnv.release(200);

    carrier.begin(WAVEFORM_SINE);
    carrier.amplitude(1);

    outEnv.delay(0);
    outEnv.attack(0);
    outEnv.hold(0);
    outEnv.decay(0);
    outEnv.sustain(1);
    outEnv.release(200);

    // mixer.gain(0, 1);

    MixGain.gain(1);

    potentiometer[presetNr][0] = 17;
    potentiometer[presetNr][1] = 53;
    potentiometer[presetNr][2] = 5;
    potentiometer[presetNr][3] = 10;

    potentiometer[presetNr][4] = 0;
    potentiometer[presetNr][5] = 0;
    potentiometer[presetNr][6] = 127;
    potentiometer[presetNr][7] = 127;

    potentiometer[presetNr][8] = 0;
    potentiometer[presetNr][9] = 0;
    potentiometer[presetNr][10] = 127;
    potentiometer[presetNr][11] = 20;

    // SongVol.gain(1);
    Serial.println("Setup Pl3 Done");
}
void Plugin_3::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    Serial.println("2FM-NoteOn");
    float carrier_frequency = note_frequency[notePlayed] * tuning;
    float modulator_frequency = carrier_frequency * modulator_ratio;
    carrier.frequency(carrier_frequency);
    modulator.frequency(modulator_frequency);
    modEnv.noteOn();
    outEnv.noteOn();
}
void Plugin_3::noteOff(uint8_t notePlayed, uint8_t voice)
{
    modEnv.noteOff();
    outEnv.noteOff();
}
void Plugin_3::set_parameters(uint8_t row)
{
    draw_plugin();
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        if (row == 0)
        {
            set_mod_waveform(0, 0, "mW~Form");
            set_mod_ratio(1, 0, "mRatio");
            set_mod_amplitude(2, 0, "mVolume");
            set_car_waveform(3, 0, "cW~Form");
        }

        if (row == 1)
        {
            set_envelope_mattack(0, 1, "mAttack", 0, 1000);
            set_envelope_mdecay(1, 1, "mDecay", 0, 500);
            set_envelope_msustain(2, 1, "mSustain");
            set_envelope_mrelease(3, 1, "mRelease", 0, 2000);
        }

        if (row == 2)
        {
            set_envelope_ADSR(2, 1000, 500, 2000);

            // set_envelope_attack(0, 2, "Attack", 0, 1000);
            // set_envelope_decay(1, 2, "Decay", 0, 500);
            // set_envelope_sustain(2, 2, "Sustain");
            // set_envelope_release(3, 2, "Release", 0, 2000);
        }

        if (row == 3)
        {
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        set_presetNr();
    }
}
void Plugin_3::draw_plugin()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
       // clearWorkSpace();

        drawPot(0, 0, potentiometer[presetNr][0], "mW~Form");
        drawPot(1, 0, potentiometer[presetNr][1], "mRatio");
        drawPot(2, 0, potentiometer[presetNr][2], "mVolume");
        drawPot(3, 0, potentiometer[presetNr][3], "cW~Form");

        // drawPot(0, 2, potentiometer[8], "Filt-Frq");
        // drawPot(1, 2, potentiometer[9], "Resonance");
        // drawPot(2, 2, potentiometer[10], "Sweep");
        // drawPot(3, 2, potentiometer[10], "Type");
        drawPot(0, 1, potentiometer[presetNr][4], "mAttack");
        drawPot(1, 1, potentiometer[presetNr][5], "mDecay");
        drawPot(2, 1, potentiometer[presetNr][6], "mSustain");
        drawPot(3, 1, potentiometer[presetNr][7], "mRelease");

        // drawPot(0, 2, potentiometer[presetNr][8], "Attack");
        //  drawPot(1, 2, potentiometer[presetNr][9], "Decay");
        //  drawPot(2, 2, potentiometer[presetNr][10], "Sustain");
        //  drawPot(3, 2, potentiometer[presetNr][11], "Release");
        drawEnvelope(2, potentiometer[presetNr][12], potentiometer[presetNr][13],
                     potentiometer[presetNr][14], potentiometer[presetNr][15]);
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 11, 4, 4, NO_VALUE, "Prset", ILI9341_BLUE, 2, false, false);
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 12, 4, 4, presetNr, NO_NAME, ILI9341_BLUE, 2, true, false);
    }
}
void Plugin_3::change_preset()
{
    assign_mod_waveform(potentiometer[presetNr][0]);
    assign_mod_ratio(potentiometer[presetNr][1]);
    assign_mod_amplitude(potentiometer[presetNr][2]);
    assign_car_waveform(potentiometer[presetNr][3]);

    assign_envelope_mattack(potentiometer[presetNr][4], 1000);
    assign_envelope_mdecay(potentiometer[presetNr][5], 500);
    assign_envelope_msustain(potentiometer[presetNr][6]);
    assign_envelope_mrelease(potentiometer[presetNr][7], 2000);

    assign_envelope_attack(potentiometer[presetNr][8], 1000);
    assign_envelope_decay(potentiometer[presetNr][9], 500);
    assign_envelope_sustain(potentiometer[presetNr][10]);
    assign_envelope_release(potentiometer[presetNr][11], 2000);
}

void Plugin_3::get_peak()
{
    // Serial.printf("Pl3: %f  ", peak.read());
}

void Plugin_3::set_mod_waveform(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_mod_waveform(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_3::assign_mod_waveform(uint8_t value)
{
    int walveform = map(value, 0, MIDI_CC_RANGE, 0, 12);
    modulator.begin(walveform);
}
void Plugin_3::set_mod_amplitude(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_mod_amplitude(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_3::assign_mod_amplitude(uint8_t value)
{
    float ampl = (float)(value / MIDI_CC_RANGE_FLOAT);
    modulator.amplitude(ampl);
}
void Plugin_3::set_mod_ratio(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_mod_ratio(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_3::assign_mod_ratio(uint8_t value)
{
    int rationem = map(value, 0, MIDI_CC_RANGE, 0, NUM_RATIOS);
    modulator_ratio = ratios[rationem];
}

void Plugin_3::set_car_waveform(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_car_waveform(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_3::assign_car_waveform(uint8_t value)
{
    int walveform = map(value, 0, MIDI_CC_RANGE, 0, 12);
    carrier.begin(walveform);
}
/*
void Plugin_3::set_envelope_attack(uint8_t XPos, uint8_t YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int attack = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, min, max);

        outEnv.attack(attack);
    }
}
void Plugin_3::set_envelope_decay(uint8_t XPos, uint8_t YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int decay = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, min, max);

        outEnv.decay(decay);
    }
}
void Plugin_3::set_envelope_sustain(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float sustain = (float)(get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT);

        outEnv.sustain(sustain);
    }
}
void Plugin_3::set_envelope_release(uint8_t XPos, uint8_t YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int release = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, min, max);

        outEnv.release(release);
    }
}
*/
void Plugin_3::set_envelope_mattack(uint8_t XPos, uint8_t YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_mattack(get_Potentiometer(XPos, YPos, name), max);
    }
}
void Plugin_3::assign_envelope_mattack(uint8_t value, int max)
{
    int attack = map(value, 0, MIDI_CC_RANGE, 0, max);
    modEnv.attack(attack);
}
void Plugin_3::set_envelope_mdecay(uint8_t XPos, uint8_t YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_mdecay(get_Potentiometer(XPos, YPos, name), max);
    }
}
void Plugin_3::assign_envelope_mdecay(uint8_t value, int max)
{
    int decay = map(value, 0, MIDI_CC_RANGE, 0, max);
    modEnv.decay(decay);
}
void Plugin_3::set_envelope_msustain(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_envelope_msustain(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_3::assign_envelope_msustain(uint8_t value)
{
    float sustain = (float)(value / MIDI_CC_RANGE_FLOAT);
    modEnv.sustain(sustain);
}
void Plugin_3::set_envelope_mrelease(uint8_t XPos, uint8_t YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_mrelease(get_Potentiometer(XPos, YPos, name), max);
    }
}
void Plugin_3::assign_envelope_mrelease(uint8_t value, int max)
{
    int release = map(value, 0, MIDI_CC_RANGE, 0, max);
    modEnv.release(release);
}

void Plugin_3::assign_envelope_attack(uint8_t value, int max)
{
    int attack = map(value, 0, MIDI_CC_RANGE, 0, max);

    // Fenv.attack(attack);
    outEnv.attack(attack);
}
void Plugin_3::assign_envelope_decay(uint8_t value, int max)
{
    int decay = map(value, 0, MIDI_CC_RANGE, 0, max);

    // Fenv.decay(decay);
    outEnv.decay(decay);
}
void Plugin_3::assign_envelope_sustain(uint8_t value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;

    // Fenv.sustain(ampl);
    outEnv.sustain(ampl);
}
void Plugin_3::assign_envelope_release(uint8_t value, int max)
{
    int release = map(value, 0, MIDI_CC_RANGE, 0, max);

    // Fenv.release(release);
    outEnv.release(release);
}
void Plugin_3::set_envelope_ADSR(uint8_t YPos, int maxA, int maxD, int maxR)
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
Plugin_3 plugin_3("2FM", 3);

// TeensyDAW: end automatically generated code