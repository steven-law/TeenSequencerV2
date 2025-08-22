#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "projectVariables.h"

#include <Plugins/Plugin_11.h>

extern bool change_plugin_row;

void Plugin_11::setup()
{
    dc.amplitude(1);

    // waveform.begin(WAVEFORM_SINE);
    waveform.amplitude(1);
    // waveform.frequencyModulation(3);

    LFO.begin(WAVEFORM_SINE);
    LFO.amplitude(1);
    LFO.frequency(2);
    PWMmix.gain(0, 1);
    PWMmix.gain(1, 1);

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
    MixGain.gain(1);
    // mixer.gain(0, 1);
    potentiometer[presetNr][0] = 50;
    potentiometer[presetNr][1] = 60;
    potentiometer[presetNr][4] = 1;
    potentiometer[presetNr][5] = 50;
    potentiometer[presetNr][6] = 50;
    potentiometer[presetNr][7] = 20;
    potentiometer[presetNr][8] = 60;
    potentiometer[presetNr][9] = 0;
    potentiometer[presetNr][10] = 30;
    potentiometer[presetNr][11] = 0;
    potentiometer[presetNr][12] = 5;
    potentiometer[presetNr][13] = 0;
    potentiometer[presetNr][14] = 127;
    potentiometer[presetNr][15] = 20;
}
void Plugin_11::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)

{float velo = (velocity * (MixerGain / MIDI_CC_RANGE_FLOAT));
    MixGain.gain(velo);
    float frequency = note_frequency[notePlayed] * tuning;
    waveform.frequency(frequency);
    Fenv.noteOn();
    Aenv.noteOn();
}
void Plugin_11::noteOff(uint8_t notePlayed, uint8_t voice)
{
    Fenv.noteOff();
    Aenv.noteOff();
}
void Plugin_11::set_parameters(uint8_t row)
{
    draw_plugin();
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        if (row == 0)
        {

            set_PWMdc_amplitude(0, 0);
            set_voice_amplitude(1, 0);
            set_LFO2VCO_frequency(2, 0);
            set_LFO2VCO_amplitude(3, 0);
        }

        if (row == 1)
        {
            set_DC_amplitude(0, 1);
            set_LFO_waveform(1, 1);
            set_LFO_frequency(2, 1);
            set_LFO_amplitude(3, 1);
        }

        if (row == 2)
        {
            set_filter_frequency(0, 2);
            set_filter_resonance(1, 2, 0, 5.00);
            set_filter_sweep(2, 2);
            set_filter_type(3, 2);
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
void Plugin_11::set_gain(uint8_t gain)
{
    MixerGain=gain;
}
void Plugin_11::change_preset()
{
    assign_PWMdc_amplitude(potentiometer[presetNr][0]);
    assign_voice_amplitude(potentiometer[presetNr][1]);
    assign_LFO2VCO_frequency(potentiometer[presetNr][2]);
    assign_LFO2VCO_amplitude(potentiometer[presetNr][3]);

    assign_DC_amplitude(potentiometer[presetNr][4]);
    assign_LFO_waveform(potentiometer[presetNr][5]);
    assign_LFO_frequency(potentiometer[presetNr][6]);
    assign_LFO_amplitude(potentiometer[presetNr][7]);

    assign_filter_frequency(potentiometer[presetNr][8]);
    assign_filter_resonance(potentiometer[presetNr][9]);
    assign_filter_sweep(potentiometer[presetNr][10]);
    selectFilterType(potentiometer[presetNr][11]);

    assign_envelope_attack(potentiometer[presetNr][12], 1000);
    assign_envelope_decay(potentiometer[presetNr][13], 500);
    assign_envelope_sustain(potentiometer[presetNr][14]);
    assign_envelope_release(potentiometer[presetNr][15], 2000);
}

void Plugin_11::set_voice_amplitude(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        assign_voice_amplitude(get_Potentiometer(XPos, YPos));
    }
}
void Plugin_11::assign_voice_amplitude(uint8_t value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;

    waveform.amplitude(ampl);
}

void Plugin_11::set_LFO_waveform(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        assign_LFO_waveform(get_Potentiometer(XPos, YPos));
    }
}
void Plugin_11::assign_LFO_waveform(uint8_t value)
{
    uint8_t walveform = map(value, 0, MIDI_CC_RANGE, 0, 12);

    LFO.begin(walveform);
    Lfo2Vco.begin(walveform);
}
void Plugin_11::set_LFO_frequency(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        assign_LFO_frequency(get_Potentiometer(XPos, YPos));
    }
}
void Plugin_11::assign_LFO_frequency(uint8_t value)
{
    // float ampl = map(value, 0, MIDI_CC_RANGE_FLOAT, 0.10, 100.00);

    LFO.frequency(value + 1);
}
void Plugin_11::set_LFO_amplitude(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        assign_LFO_amplitude(get_Potentiometer(XPos, YPos));
    }
}
void Plugin_11::assign_LFO_amplitude(uint8_t value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;

    LFO.amplitude(ampl);
}
void Plugin_11::set_PWMdc_amplitude(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        assign_DC_amplitude(get_Potentiometer(XPos, YPos));
    }
}
void Plugin_11::assign_PWMdc_amplitude(uint8_t value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;

    PWMdc.amplitude(ampl);
}

void Plugin_11::set_DC_amplitude(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        assign_DC_amplitude(get_Potentiometer(XPos, YPos));
    }
}
void Plugin_11::assign_DC_amplitude(uint8_t value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;

    dc.amplitude(ampl);
}

void Plugin_11::set_LFO2VCO_amplitude(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        assign_LFO2VCO_amplitude(get_Potentiometer(XPos, YPos));
    }
}
void Plugin_11::assign_LFO2VCO_amplitude(uint8_t value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;
    Lfo2Vco.amplitude(ampl);
    // Serial.printf("pl2 lfo2vco gain: %f\n", ampl);
}
void Plugin_11::set_LFO2VCO_frequency(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        assign_LFO2VCO_frequency(get_Potentiometer(XPos, YPos));
    }
}
void Plugin_11::assign_LFO2VCO_frequency(uint8_t value)
{
    // float ampl = map(value, 0, MIDI_CC_RANGE_FLOAT, 0.10, 100.00);

    Lfo2Vco.frequency(value + 1);
}

void Plugin_11::set_filter_frequency(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        assign_filter_frequency(get_Potentiometer(XPos, YPos));
    }
}
void Plugin_11::assign_filter_frequency(uint8_t value)
{

    int frequency = note_frequency[value] * tuning;

    filter.frequency(frequency);
    // ladder[i].frequency(frequency);
}

void Plugin_11::set_filter_resonance(uint8_t XPos, uint8_t YPos, float min, float max)
{
    if (enc_moved[XPos])
    {
        assign_filter_resonance(get_Potentiometer(XPos, YPos));
    }
}
void Plugin_11::assign_filter_resonance(uint8_t value)
{

    float reso = value / 25.40;

    filter.resonance(reso);
    // ladder[i].resonance(reso);
}
void Plugin_11::set_filter_sweep(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        assign_filter_sweep(get_Potentiometer(XPos, YPos));
    }
}
void Plugin_11::assign_filter_sweep(uint8_t value)
{
    float swp = value / 18.14;

    filter.octaveControl(swp);
    // ladder[i].octaveControl(swp);
}
void Plugin_11::set_filter_type(uint8_t XPos, uint8_t YPos)
{
    if (enc_moved[XPos])
    {
        selectFilterType(constrain(get_Potentiometer(XPos, YPos), 0, 3));
    }
}
void Plugin_11::selectFilterType(uint8_t mixerchannel)
{

    fMixer.gain(0, 0);
    fMixer.gain(1, 0);
    fMixer.gain(2, 0);
    fMixer.gain(3, 0);
    fMixer.gain(mixerchannel, 1);
}

void Plugin_11::assign_envelope_attack(uint8_t value, int max)
{
    int attack = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.attack(attack);
    Aenv.attack(attack);
}
void Plugin_11::assign_envelope_decay(uint8_t value, int max)
{
    int decay = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.decay(decay);
    Aenv.decay(decay);
}
void Plugin_11::assign_envelope_sustain(uint8_t value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;

    Fenv.sustain(ampl);
    Aenv.sustain(ampl);
}
void Plugin_11::assign_envelope_release(uint8_t value, int max)
{
    int release = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.release(release);
    Aenv.release(release);
}
void Plugin_11::set_envelope_ADSR(uint8_t YPos, int maxA, int maxD, int maxR)
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

Plugin_11 plugin_11("PWM", 9);
