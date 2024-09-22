#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "projectVariables.h"

#include <Plugins/Plugin_7.h>
#include <ownLibs/effect_dynamics.h>

// TeensyDAW: begin automatically generated code
// Name: Boom
// Description: Subtractive Synthesizer
// Voices: 1

// Kick
// Pot 1: Sweep
// Pot 2: Noise
// Pot 3: O-Drive
// Pot 4: Decay

//
// Pot 5:
// Pot 6:
// Pot 7:
// Pot 8:

//
// Pot 9:
// Pot 10:
// Pot 11:
// Pot 12:

//
// Pot 13:
// Pot 14:
// Pot 15:
// Pot 16:

extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
void clearWorkSpace();

void Plugin_7::setup()
{

    // dynamics.gate(threshold, attack, release, hysterisis);              // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
    // dynamics.compression(threshold, attack, release, ratio, kneeWidth); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 1.0f-60.0f ;; 0.0f-32.0f
    dynamics.limit(threshold, attack, release); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
    dynamics.autoMakeupGain(6.0f);
    dynamics.makeupGain(6.0f);
    MixGain.gain(1);
    SongVol.gain(1);
    Serial.println("pl7 setup done");
}
void Plugin_7::noteOn(byte notePlayed, float velocity, byte voice)
{
    float frequence = note_frequency[notePlayed] * tuning;
    fm_drum.frequency(frequence);
    fm_drum.noteOn();
}
void Plugin_7::noteOff(byte notePlayed, byte voice)
{
}
void Plugin_7::set_parameters(byte row)
{
    draw_plugin();
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        if (row == 0)
        {
            set_fmdrum_pitchMod(0, 0, "Sweep");
            set_fmdrum_noise(1, 0, "Noise");
            set_fmdrum_overdrive(2, 0, "O-Drive");
            set_fmdrum_decay(3, 0, "Decay");
        }

        if (row == 1)
        {
            set_dynamics_threshold(0, 1, "Trshold");
            set_dynamics_attack(1, 1, "Attack");
            set_dynamics_release(2, 1, "Release");
            set_dynamics_hysterisis(3, 1, "Hysteris");
        }

        if (row == 2)
        {
            set_dynamics_ratio(0, 2, "Ratio");
            set_dynamics_kneeWidth(1, 2, "Knewdth");
            set_dynamics_autoMakeupGain(2, 2, "AmakUpG");
            set_dynamics_makeupGain(3, 2, "makUpG");
        }

        if (row == 3)
        {
            set_amp_gain(3, 3, "Gain");
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        set_presetNr();
    }
}
void Plugin_7::draw_plugin()
{

    if (change_plugin_row)
    {
        change_plugin_row = false;
        clearWorkSpace();
        // Serial.println("drawing plugin 5");

        drawPot(0, 0, potentiometer[presetNr][0], "Sweep");
        drawPot(1, 0, potentiometer[presetNr][1], "Noise");
        drawPot(2, 0, potentiometer[presetNr][2], "O-Drive");
        drawPot(3, 0, potentiometer[presetNr][3], "Decay");

        drawPot(0, 1, potentiometer[presetNr][4], "Trshold");
        drawPot(1, 1, potentiometer[presetNr][5], "Attack");
        drawPot(2, 1, potentiometer[presetNr][6], "Release");
        drawPot(3, 1, potentiometer[presetNr][7], "Hysteris");

        drawPot(0, 2, potentiometer[presetNr][8], "Ratio");
        drawPot(1, 2, potentiometer[presetNr][9], "Knewdth");
        drawPot(2, 2, potentiometer[presetNr][10], "AmakUpG");
        drawPot(3, 2, potentiometer[presetNr][11], "makUpG");

        drawPot(3, 3, potentiometer[presetNr][15], "Gain");

        //draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "Prset", presetNr, 3, 0);
    }
}
void Plugin_7::change_preset(){}

//test
void Plugin_7::set_fmdrum_pitchMod(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float sustain = get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT;
        fm_drum.fm(sustain); // float 0-1
    }
}
void Plugin_7::set_fmdrum_decay(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float sustain = get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT;
        fm_drum.decay(sustain); // float 0-1
    }
}
void Plugin_7::set_fmdrum_noise(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float sustain = get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT;
        fm_drum.noise(sustain); // float 0-1
    }
}
void Plugin_7::set_fmdrum_overdrive(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float sustain = get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT;
        fm_drum.overdrive(sustain); // float 0-1
    }
}

void Plugin_7::set_dynamics_threshold(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        threshold = (float)map(get_Potentiometer(XPos, YPos, name), 0, 127, MIN_DB, MAX_DB);
        dynamics.gate(threshold, attack, release, hysterisis);              // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
        dynamics.compression(threshold, attack, release, ratio, kneeWidth); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 1.0f-60.0f ;; 0.0f-32.0f
        dynamics.limit(threshold, attack, release);                         // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
    }
}
void Plugin_7::set_dynamics_attack(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        attack = (float)map(get_Potentiometer(XPos, YPos, name), 0, 127, MIN_T, MAX_T);
        dynamics.gate(threshold, attack, release, hysterisis);              // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
        dynamics.compression(threshold, attack, release, ratio, kneeWidth); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 1.0f-60.0f ;; 0.0f-32.0f
        dynamics.limit(threshold, attack, release);                         // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
    }
}
void Plugin_7::set_dynamics_release(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        release = (float)map(get_Potentiometer(XPos, YPos, name), 0, 127, MIN_T, MAX_T);
        dynamics.gate(threshold, attack, release, hysterisis);              // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
        dynamics.compression(threshold, attack, release, ratio, kneeWidth); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 1.0f-60.0f ;; 0.0f-32.0f
        dynamics.limit(threshold, attack, release);                         // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
    }
}
void Plugin_7::set_dynamics_hysterisis(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        hysterisis = (float)map(get_Potentiometer(XPos, YPos, name), 0, 127, 0.0f, 6.0f);
        dynamics.gate(threshold, attack, release, hysterisis); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
    }
}

void Plugin_7::set_dynamics_ratio(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        ratio = (float)map(get_Potentiometer(XPos, YPos, name), 0, 127, RATIO_OFF, RATIO_INFINITY);
        dynamics.compression(threshold, attack, release, ratio, kneeWidth); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 1.0f-60.0f ;; 0.0f-32.0f
    }
}
void Plugin_7::set_dynamics_kneeWidth(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        kneeWidth = (float)map(get_Potentiometer(XPos, YPos, name), 0, 127, 0.0f, 32.0f);
        dynamics.compression(threshold, attack, release, ratio, kneeWidth); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 1.0f-60.0f ;; 0.0f-32.0f
    }
}
void Plugin_7::set_dynamics_makeupGain(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float sustain = (float)map(get_Potentiometer(XPos, YPos, name), 0, 127, -12.0f, 24.0f);
        dynamics.makeupGain(sustain); // float -12.0f, 24.0f
    }
}
void Plugin_7::set_dynamics_autoMakeupGain(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float sustain = (float)map(get_Potentiometer(XPos, YPos, name), 0, 127, 0.0f, 60.0f);
        dynamics.autoMakeupGain(sustain); // float 0.0f, 60.0f
    }
}
void Plugin_7::set_amp_gain(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        float sustain = (float)map(get_Potentiometer(XPos, YPos, name), 0, 127, 0.0f, 6.0f);
        amp.gain(sustain); // float 0.0f, 60.0f
    }
}

Plugin_7 plugin_7("Boom", 7);
