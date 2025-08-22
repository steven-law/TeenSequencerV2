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

void Plugin_7::setup()
{

    // dynamics.gate(threshold, attack, release, hysterisis);              // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
    // dynamics.compression(threshold, attack, release, ratio, kneeWidth); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 1.0f-60.0f ;; 0.0f-32.0f
    dynamics.limit(threshold, attack, release); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
    dynamics.autoMakeupGain(6.0f);
    dynamics.makeupGain(6.0f);
    MixGain.gain(1);
    // SongVol.gain(1);
    Serial.println("pl7 setup done");
}
void Plugin_7::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    float velo = (velocity * (MixerGain / MIDI_CC_RANGE_FLOAT));
    MixGain.gain(velo);
    float frequence = note_frequency[notePlayed] * tuning;
    fm_drum.frequency(frequence);
    fm_drum.noteOn();
}
void Plugin_7::noteOff(uint8_t notePlayed, uint8_t voice)
{
}
void Plugin_7::set_parameters(uint8_t row)
{
    draw_plugin();
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        if (row == 0)
        {
            set_fmdrum_pitchMod(0, 0);
            set_fmdrum_noise(1, 0);
            set_fmdrum_overdrive(2, 0);
            set_fmdrum_decay(3, 0);
        }

        if (row == 1)
        {
            set_dynamics_threshold(0, 1);
            set_dynamics_attack(1, 1);
            set_dynamics_release(2, 1);
            set_dynamics_hysterisis(3, 1);
        }

        if (row == 2)
        {
            set_dynamics_ratio(0, 2);
            set_dynamics_kneeWidth(1, 2);
            set_dynamics_autoMakeupGain(2, 2);
            set_dynamics_makeupGain(3, 2);
        }

        if (row == 3)
        {
            set_amp_gain(3, 3);
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        set_presetNr();
    }
}

void Plugin_7::change_preset() {}
void Plugin_7::set_gain(uint8_t gain)
{
    MixerGain = gain;
}
// test
void Plugin_7::set_fmdrum_pitchMod(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        float sustain = get_Potentiometer(XPos, YPos) / MIDI_CC_RANGE_FLOAT;
        fm_drum.fm(sustain); // float 0-1
    }
}
void Plugin_7::set_fmdrum_decay(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        float sustain = get_Potentiometer(XPos, YPos) / MIDI_CC_RANGE_FLOAT;
        fm_drum.decay(sustain); // float 0-1
    }
}
void Plugin_7::set_fmdrum_noise(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        float sustain = get_Potentiometer(XPos, YPos) / MIDI_CC_RANGE_FLOAT;
        fm_drum.noise(sustain); // float 0-1
    }
}
void Plugin_7::set_fmdrum_overdrive(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        float sustain = get_Potentiometer(XPos, YPos) / MIDI_CC_RANGE_FLOAT;
        fm_drum.overdrive(sustain); // float 0-1
    }
}

void Plugin_7::set_dynamics_threshold(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        threshold = (float)map(get_Potentiometer(XPos, YPos), 0, 127, MIN_DB, MAX_DB);
        dynamics.gate(threshold, attack, release, hysterisis);              // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
        dynamics.compression(threshold, attack, release, ratio, kneeWidth); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 1.0f-60.0f ;; 0.0f-32.0f
        dynamics.limit(threshold, attack, release);                         // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
    }
}
void Plugin_7::set_dynamics_attack(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        attack = (float)map(get_Potentiometer(XPos, YPos), 0, 127, MIN_T, MAX_T);
        dynamics.gate(threshold, attack, release, hysterisis);              // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
        dynamics.compression(threshold, attack, release, ratio, kneeWidth); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 1.0f-60.0f ;; 0.0f-32.0f
        dynamics.limit(threshold, attack, release);                         // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
    }
}
void Plugin_7::set_dynamics_release(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        release = (float)map(get_Potentiometer(XPos, YPos), 0, 127, MIN_T, MAX_T);
        dynamics.gate(threshold, attack, release, hysterisis);              // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
        dynamics.compression(threshold, attack, release, ratio, kneeWidth); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 1.0f-60.0f ;; 0.0f-32.0f
        dynamics.limit(threshold, attack, release);                         // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
    }
}
void Plugin_7::set_dynamics_hysterisis(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        hysterisis = (float)map(get_Potentiometer(XPos, YPos), 0, 127, 0.0f, 6.0f);
        dynamics.gate(threshold, attack, release, hysterisis); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
    }
}

void Plugin_7::set_dynamics_ratio(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        ratio = (float)map(get_Potentiometer(XPos, YPos), 0, 127, RATIO_OFF, RATIO_INFINITY);
        dynamics.compression(threshold, attack, release, ratio, kneeWidth); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 1.0f-60.0f ;; 0.0f-32.0f
    }
}
void Plugin_7::set_dynamics_kneeWidth(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        kneeWidth = (float)map(get_Potentiometer(XPos, YPos), 0, 127, 0.0f, 32.0f);
        dynamics.compression(threshold, attack, release, ratio, kneeWidth); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 1.0f-60.0f ;; 0.0f-32.0f
    }
}
void Plugin_7::set_dynamics_makeupGain(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        float sustain = (float)map(get_Potentiometer(XPos, YPos), 0, 127, -12.0f, 24.0f);
        dynamics.makeupGain(sustain); // float -12.0f, 24.0f
    }
}
void Plugin_7::set_dynamics_autoMakeupGain(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        float sustain = (float)map(get_Potentiometer(XPos, YPos), 0, 127, 0.0f, 60.0f);
        dynamics.autoMakeupGain(sustain); // float 0.0f, 60.0f
    }
}
void Plugin_7::set_amp_gain(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        float sustain = (float)map(get_Potentiometer(XPos, YPos), 0, 127, 0.0f, 6.0f);
        amp.gain(sustain); // float 0.0f, 60.0f
    }
}

Plugin_7 plugin_7("Boom", 7);
