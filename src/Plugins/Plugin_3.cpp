
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
//void clearWorkSpace();

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
    SongVol.gain(1);
    Serial.println("Setup Pl3 Done");
}
void Plugin_3::noteOn(byte notePlayed, float velocity, byte voice)
{
    Serial.println("2FM-NoteOn");
    float carrier_frequency = note_frequency[notePlayed] * tuning;
    float modulator_frequency = carrier_frequency * modulator_ratio;
    carrier.frequency(carrier_frequency);
    modulator.frequency(modulator_frequency);
    modEnv.noteOn();
    outEnv.noteOn();
}
void Plugin_3::noteOff(byte notePlayed, byte voice)
{
    modEnv.noteOff();
    outEnv.noteOff();
}
void Plugin_3::set_parameters(byte row)
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
            set_envelope_attack(0, 2, "Attack", 0, 1000);
            set_envelope_decay(1, 2, "Decay", 0, 500);
            set_envelope_sustain(2, 2, "Sustain");
            set_envelope_release(3, 2, "Release", 0, 2000);
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
        clearWorkSpace();

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

        drawPot(0, 2, potentiometer[presetNr][8], "Attack");
        drawPot(1, 2, potentiometer[presetNr][9], "Decay");
        drawPot(2, 2, potentiometer[presetNr][10], "Sustain");
        drawPot(3, 2, potentiometer[presetNr][11], "Release");

        //draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "Prset", presetNr, 3, 0);
    }
}
void Plugin_3::change_preset()
{

}

void Plugin_3::get_peak()
{
    // Serial.printf("Pl3: %f  ", peak.read());
}

void Plugin_3::set_mod_waveform(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        int walveform = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, 0, 12);

        modulator.begin(walveform);
    }
}
void Plugin_3::set_mod_amplitude(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float ampl = (float)(get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT);

        modulator.amplitude(ampl);
    }
}
void Plugin_3::set_mod_ratio(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        int rationem = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, 0, NUM_RATIOS);

        modulator_ratio = ratios[rationem];
    }
}

void Plugin_3::set_car_waveform(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        int walveform = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, 0, 12);

        carrier.begin(walveform);
    }
}

void Plugin_3::set_envelope_attack(byte XPos, byte YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int attack = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, min, max);

        outEnv.attack(attack);
    }
}
void Plugin_3::set_envelope_decay(byte XPos, byte YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int decay = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, min, max);

        outEnv.decay(decay);
    }
}
void Plugin_3::set_envelope_sustain(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float sustain = (float)(get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT);

        outEnv.sustain(sustain);
    }
}
void Plugin_3::set_envelope_release(byte XPos, byte YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int release = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, min, max);

        outEnv.release(release);
    }
}

void Plugin_3::set_envelope_mattack(byte XPos, byte YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int attack = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, min, max);

        modEnv.attack(attack);
    }
}
void Plugin_3::set_envelope_mdecay(byte XPos, byte YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int decay = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, min, max);

        modEnv.decay(decay);
    }
}
void Plugin_3::set_envelope_msustain(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float sustain = (float)(get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT);

        modEnv.sustain(sustain);
    }
}
void Plugin_3::set_envelope_mrelease(byte XPos, byte YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int release = map(get_Potentiometer(XPos, YPos, name), 0, MIDI_CC_RANGE, min, max);

        modEnv.release(release);
    }
}
Plugin_3 plugin_3("2FM", 3);

// TeensyDAW: end automatically generated code