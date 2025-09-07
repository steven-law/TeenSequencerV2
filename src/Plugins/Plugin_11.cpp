#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"

#include <Plugins/Plugin_11.h>

extern bool change_plugin_row;

void Plugin_11::setup()
{
    IhaveADSR = true;
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
    setParameterNames("PW", "Volume", "PWM-Freq", "PWM-Lvl", "Env-Lvl", "LFO W~F", "LFO-Freq", "LFO-Lvl", "Filter-Freq", "Resonance", "Sweep", "Type", "1", "1", "1", "1");
}
void Plugin_11::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)

{
    float velo = (velocity * (MixerGain / MIDI_CC_RANGE_FLOAT));
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

void Plugin_11::assign_parameter(uint8_t pot)
{
    switch (pot)
    {
    case 0:
    {
        float ampl = get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT;
        PWMdc.amplitude(ampl);
    }
    break;
    case 1:
    {
        float ampl = get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT;
        waveform.amplitude(ampl);
    }
    break;
    case 2:
    {
        Lfo2Vco.frequency(get_Potentiometer(pot) + 1);
    }
    break;
    case 3:
    {
        float ampl = get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT;
        Lfo2Vco.amplitude(ampl);
    }
    break;
    case 4:
    {
        float ampl = get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT;
        dc.amplitude(ampl);
    }
    break;
    case 5:
    {
        uint8_t walveform = map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 0, 12);
        LFO.begin(walveform);
        Lfo2Vco.begin(walveform);
    }
    break;
    case 6:
    {
        LFO.frequency(get_Potentiometer(pot) + 1);
    }
    break;
    case 7:
    {
        float ampl = get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT;
        LFO.amplitude(ampl);
    }
    break;
    case 8:
    {
        int frequency = note_frequency[get_Potentiometer(pot)] * tuning;
        filter.frequency(frequency);
    }
    break;
    case 9:
    {
        float reso = get_Potentiometer(pot) / 25.40;
        filter.resonance(reso);
    }
    break;
    case 10:
    {
        float swp = get_Potentiometer(pot) / 18.14;
        filter.octaveControl(swp);
    }
    break;
    case 11:
    {
        fMixer.gain(0, 0);
        fMixer.gain(1, 0);
        fMixer.gain(2, 0);
        fMixer.gain(3, 0);
        fMixer.gain(get_Potentiometer(pot), 1);
    }
    break;
    case 12:
    {
        int attack = map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 0, 1000);
        Fenv.attack(attack);
        Aenv.attack(attack);
    }
    break;
    case 13:
    {
        int decay = map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 0, 500);
        Fenv.decay(decay);
        Aenv.decay(decay);
    }
    break;
    case 14:
    {
        float ampl = get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT;
        Fenv.sustain(ampl);
        Aenv.sustain(ampl);
    }
    break;
    case 15:
    {
        int release = map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 0, 2000);

        Fenv.release(release);
        Aenv.release(release);
    }
    break;
    default:
        break;
    }
}

Plugin_11 plugin_11("PWM", 11);
