#include <Plugins/Plugin_2.h>

void Plugin_2::setup()
{
    dc.amplitude(1);

    waveform.begin(WAVEFORM_SINE);
    waveform.amplitude(1);
    waveform.frequencyModulation(3);

    LFO.begin(WAVEFORM_SINE);
    LFO.amplitude(1);
    LFO.frequency(2);
    fEnvMixer.gain(0, 1);
    fEnvMixer.gain(1, 1);

    Fenv.delay(0);
    Fenv.hold(0);

    fMixer.gain(0, 1);
    fMixer.gain(1, 0);
    fMixer.gain(2, 0);
    fMixer.gain(3, 0);

    Aenv.delay(0);
    Aenv.hold(0);
    MixGain.gain(1);
    this->setParameterNames("W~Form", 12, "Vol", MIDI_CC_RANGE, "LFO-Freq", MIDI_CC_RANGE, "LFO-Lvl", MIDI_CC_RANGE,
                            "Env-Lvl", MIDI_CC_RANGE, "LFO W~F", 12, "LFO-Freq", MIDI_CC_RANGE, "LFO-Lvl", MIDI_CC_RANGE,
                            "Filt-Frq", MIDI_CC_RANGE, "Resonance", MIDI_CC_RANGE, "Sweep", MIDI_CC_RANGE, "LPF", 2,
                            "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE);
    set_preset(0,
               1, 80, 1, 0,
               127, 0, 1, 0,
               60, 0, 30, 0,
               0, 0, 127, 10);
    set_preset(1,
               2, 80, 1, 0,
               127, 0, 0, 0,
               35, 127, 71, 0,
               16, 43, 0, 10);
    set_preset(2,
               1, 80, 45, 1,
               80, 0, 52, 31,
               35, 0, 45, 1,
               30, 45, 75, 25);
    set_preset(3,
               1, 80, 5, 1,
               80, 0, 0, 0,
               60, 127, 77, 2,
               5, 40, 64, 30);

    change_preset();
}
void Plugin_2::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    float velo = (velocity * (MixerGain / MIDI_CC_RANGE_FLOAT));
    MixGain.gain(velo);
    float frequency = note_frequency[notePlayed] * tuning;
    waveform.frequency(frequency);
    Fenv.noteOn();
    Aenv.noteOn();
    Serial.printf("pl2 note %d on voice %d, velocity: %f velo: %02f\n", notePlayed, voice, velocity, velo);
}
void Plugin_2::noteOff(uint8_t notePlayed, uint8_t voice)
{
    Fenv.noteOff();
    Aenv.noteOff();
}
void Plugin_2::assign_parameter(uint8_t pot)
{
    uint8_t value = get_Potentiometer(pot);
    switch (pot)
    {
    case 0: // W~Form
    {
        waveform.begin(value);
    }
    break;
    case 1: // Vol
    {
        float ampl = value / MIDI_CC_RANGE_FLOAT;
        waveform.amplitude(ampl);
    }
    break;
    case 2: // LFO Freq
    {
        Lfo2Vco.frequency((value + 1) / 2);
    }
    break;
    case 3: // LFO Lvl
    {
        float ampl = value / MIDI_CC_RANGE_FLOAT;
        Lfo2Vco.amplitude(ampl / 4.0f);
    }
    break;
    case 4: // ENV Lvl
    {
        float ampl = value / MIDI_CC_RANGE_FLOAT;
        dc.amplitude(ampl);
    }
    break;
    case 5: // LFO W~Form
    {
        LFO.begin(value);
    }
    break;
    case 6: // LFO Freq
    {
        LFO.frequency((value + 1) / 2);
    }
    break;
    case 7: // LFO Lvl
    {
        float ampl = value / MIDI_CC_RANGE_FLOAT;
        fEnvMixer.gain(1, ampl);
    }
    break;
    case 8: // Filter Freq
    {
        int frequency = note_frequency[value] * tuning;
        filter.frequency(frequency);
    }
    break;
    case 9: // Reso
    {
        float reso = value / 25.40;
        filter.resonance(reso);
    }
    break;
    case 10: // Sweep
    {
        float swp = value / 18.14;
        filter.octaveControl(swp);
    }
    break;
    case 11: // Type
    {
        fMixer.gain(0, 0);
        fMixer.gain(1, 0);
        fMixer.gain(2, 0);
        fMixer.gain(3, 0);
        fMixer.gain(value, 1);
    }
    break;
    case 12: // Attack
    {
        int attack = map(value, 0, MIDI_CC_RANGE, 0, 1000);
        Fenv.attack(attack);
        Aenv.attack(attack);
    }
    break;
    case 13: // Decay
    {
        int decay = map(value, 0, MIDI_CC_RANGE, 0, 500);
        Fenv.decay(decay);
        Aenv.decay(decay);
    }
    break;
    case 14: // Sustain
    {
        float ampl = value / MIDI_CC_RANGE_FLOAT;
        Fenv.sustain(ampl);
        Aenv.sustain(ampl);
    }
    break;
    case 15: // Release
    {
        int release = map(value, 0, MIDI_CC_RANGE, 0, 2000);
        Fenv.release(release);
        Aenv.release(release);
    }
    break;
    default:
        break;
    }
}

Plugin_2 plugin_2("1OSC", 2);