#include <Plugins/Plugin_3.h>

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
    modEnv.release(2000);

    carrier.begin(WAVEFORM_SINE);
    carrier.amplitude(1);

    outEnv.delay(0);
    outEnv.attack(0);
    outEnv.hold(0);
    outEnv.decay(0);
    outEnv.sustain(1);
    outEnv.release(200);

    MixGain.gain(1);
    setParameterNames("mW~Form", 12, "mRatio", NUM_RATIOS, "mVolume", MIDI_CC_RANGE, "cW~Form", 12,
                      "mAttack", MIDI_CC_RANGE, "mDecay", MIDI_CC_RANGE, "mSustain", MIDI_CC_RANGE, "mRelease", MIDI_CC_RANGE,
                      "0", 0, "0", 0, "0", 0, "0", 0,
                      "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE);

    set_preset(0,
               0, 3, 15, 0,
               0, 0, 127, 127,
               0, 0, 0, 0,
               0, 0, 127, 20);
    set_preset(1,
               1, 10, 26, 2,
               0, 0, 127, 127,
               0, 0, 0, 0,
               0, 0, 127, 20);
    set_preset(2,
               0, 10, 73, 0,
               0, 30, 0, 0,
               0, 0, 0, 0,
               0, 43, 0, 20);
    set_preset(3,
               0, 10, 26, 0,
               36, 41, 80, 4,
               0, 0, 0, 0,
               0, 0, 127, 20);


    change_preset();
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
    uint8_t value = get_Potentiometer(pot);
    switch (pot)
    {
    case 0: // modulator waveform
    {
        modulator.begin(value);
    }
    break;
    case 1: // modulator ratio
    {

        modulator_ratio = ratios[value];
    }
    break;
    case 2: // modulator amplitude
    {
        float ampl = (float)(value / MIDI_CC_RANGE_FLOAT);
        modulator.amplitude(ampl);
    }
    break;
    case 3: // carrier waveform
    {
        carrier.begin(value);
    }
    break;
    case 4: // mod attack
    {
        int attack = map(value, 0, MIDI_CC_RANGE, 0, 1000);
        modEnv.attack(attack);
    }
    break;
    case 5: // mod decay
    {
        int decay = map(value, 0, MIDI_CC_RANGE, 0, 500);
        modEnv.decay(decay);
    }
    break;
    case 6: // mod sustain
    {
        float sustain = (float)(value / MIDI_CC_RANGE_FLOAT);
        modEnv.sustain(sustain);
    }
    break;
    case 7: // mod release
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
    case 12: // Attack
    {
        int attack = map(value, 0, MIDI_CC_RANGE, 0, 1000);
        outEnv.attack(attack);
    }
    break;
    case 13: // decay
    {
        int decay = map(value, 0, MIDI_CC_RANGE, 0, 500);
        outEnv.decay(decay);
    }
    break;
    case 14: // sustain
    {
        float ampl = value / MIDI_CC_RANGE_FLOAT;
        outEnv.sustain(ampl);
    }
    break;
    case 15: // release
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