
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
    setParameterNames("mW~Form", "mRatio", "mVolume", "cW~Form", "mAttack", "mDecay", "mSustain", "mRelease", "0", "0", "0", "0", "0", "0", "0", "0");

    // SongVol.gain(1);
    
}
void Plugin_3::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    Serial.println("2FM-NoteOn");
    float velo = (velocity * (MixerGain / MIDI_CC_RANGE_FLOAT));
    MixGain.gain(velo);
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
            set_mod_waveform(0, 0);
            set_mod_ratio(1, 0);
            set_mod_amplitude(2, 0);
            set_car_waveform(3, 0);
        }

        if (row == 1)
        {
            set_envelope_mattack(0, 1, 0, 1000);
            set_envelope_mdecay(1, 1, 0, 500);
            set_envelope_msustain(2, 1);
            set_envelope_mrelease(3, 1, 0, 2000);
        }

        if (row == 2)
        {
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
void Plugin_3::set_gain(uint8_t gain)
{
    MixerGain = gain;
}
void Plugin_3::get_peak()
{
    // Serial.printf("Pl3: %f  ", peak.read());
}
void Plugin_3::set_mod_waveform(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        assign_mod_waveform(get_Potentiometer(XPos, YPos));
    }
}
void Plugin_3::assign_mod_waveform(uint8_t value)
{
    int walveform = map(value, 0, MIDI_CC_RANGE, 0, 12);
    modulator.begin(walveform);
}
void Plugin_3::set_mod_amplitude(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        assign_mod_amplitude(get_Potentiometer(XPos, YPos));
    }
}
void Plugin_3::assign_mod_amplitude(uint8_t value)
{
    float ampl = (float)(value / MIDI_CC_RANGE_FLOAT);
    modulator.amplitude(ampl);
}
void Plugin_3::set_mod_ratio(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        assign_mod_ratio(get_Potentiometer(XPos, YPos));
    }
}
void Plugin_3::assign_mod_ratio(uint8_t value)
{
    int rationem = map(value, 0, MIDI_CC_RANGE, 0, NUM_RATIOS);
    modulator_ratio = ratios[rationem];
}

void Plugin_3::set_car_waveform(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        assign_car_waveform(get_Potentiometer(XPos, YPos));
    }
}
void Plugin_3::assign_car_waveform(uint8_t value)
{
    int walveform = map(value, 0, MIDI_CC_RANGE, 0, 12);
    carrier.begin(walveform);
}

void Plugin_3::set_envelope_mattack(uint8_t XPos, uint8_t YPos, int min, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_mattack(get_Potentiometer(XPos, YPos), max);
    }
}
void Plugin_3::assign_envelope_mattack(uint8_t value, int max)
{
    int attack = map(value, 0, MIDI_CC_RANGE, 0, max);
    modEnv.attack(attack);
}
void Plugin_3::set_envelope_mdecay(uint8_t XPos, uint8_t YPos, int min, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_mdecay(get_Potentiometer(XPos, YPos), max);
    }
}
void Plugin_3::assign_envelope_mdecay(uint8_t value, int max)
{
    int decay = map(value, 0, MIDI_CC_RANGE, 0, max);
    modEnv.decay(decay);
}
void Plugin_3::set_envelope_msustain(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        assign_envelope_msustain(get_Potentiometer(XPos, YPos));
    }
}
void Plugin_3::assign_envelope_msustain(uint8_t value)
{
    float sustain = (float)(value / MIDI_CC_RANGE_FLOAT);
    modEnv.sustain(sustain);
}
void Plugin_3::set_envelope_mrelease(uint8_t XPos, uint8_t YPos, int min, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_mrelease(get_Potentiometer(XPos, YPos), max);
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