
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
    IhaveADSR = true;
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
    setParameterNames("mW~Form", "mRatio", "mVolume", "cW~Form", "mAttack", "mDecay", "mSustain", "mRelease", "0", "0", "0", "0", "1", "1", "1", "1");
    change_preset();
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

void Plugin_3::assign_parameter(uint8_t pot)
{
    uint value = get_Potentiometer(0);
    switch (pot)
    {
    case 0:
    {
        int walveform = map(value, 0, MIDI_CC_RANGE, 0, 12);
        modulator.begin(walveform);
    }
    break;
    case 1:
    {
        int rationem = map(value, 0, MIDI_CC_RANGE, 0, NUM_RATIOS);
        modulator_ratio = ratios[rationem];
    }
    break;
    case 2:
    {
        float ampl = (float)(value / MIDI_CC_RANGE_FLOAT);
        modulator.amplitude(ampl);
    }
    break;
    case 3:
    {
        int walveform = map(value, 0, MIDI_CC_RANGE, 0, 12);
        carrier.begin(walveform);
    }
    break;
    case 4:
    {
        int attack = map(value, 0, MIDI_CC_RANGE, 0, 1000);
        modEnv.attack(attack);
    }
    break;
    case 5:
    {
        int decay = map(value, 0, MIDI_CC_RANGE, 0, 500);
        modEnv.decay(decay);
    }
    break;
    case 6:
    {
        float sustain = (float)(value / MIDI_CC_RANGE_FLOAT);
        modEnv.sustain(sustain);
    }
    break;
    case 7:
    {
        int release = map(value, 0, MIDI_CC_RANGE, 0, 2000);
        modEnv.release(release);
    }
    break;
    case 8:
        /* code */
        break;
    case 9:
        /* code */
        break;
    case 10:
        /* code */
        break;
    case 11:
        /* code */
        break;
    case 12:
    {
        int attack = map(value, 0, MIDI_CC_RANGE, 0, 1000);
        outEnv.attack(attack);
    }
    break;
    case 13:
    {
        int decay = map(value, 0, MIDI_CC_RANGE, 0, 500);
        outEnv.decay(decay);
    }
    break;
    case 14:
    {
        float ampl = value / MIDI_CC_RANGE_FLOAT;
        outEnv.sustain(ampl);
    }
    break;
    case 15:
    {
        int release = map(value, 0, MIDI_CC_RANGE, 0, 2000);
        outEnv.release(release);
    }
    break;

    default:
        break;
    }
}

Plugin_3 plugin_3("2FM", 3);

// TeensyDAW: end automatically generated code