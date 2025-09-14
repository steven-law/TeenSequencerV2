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
    // mixer.gain(0, 1);
    potentiometer[presetNr][0] = 1;
    potentiometer[presetNr][1] = 48;
    potentiometer[presetNr][4] = 1;
    potentiometer[presetNr][5] = 1;
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
    this->setParameterNames("W~Form", 12, "Vol", MIDI_CC_RANGE, "LFO-Freq", MIDI_CC_RANGE, "LFO-Lvl", MIDI_CC_RANGE,
                            "Env-Lvl", MIDI_CC_RANGE, "LFO W~F", 12, "LFO-Freq", MIDI_CC_RANGE, "LFO-Lvl", MIDI_CC_RANGE,
                            "Filt-Frq", MIDI_CC_RANGE, "Resonance", MIDI_CC_RANGE, "Sweep", MIDI_CC_RANGE, "LPF", 3,
                            "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE);

    change_preset();
    // SongVol.gain(1);
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
        Lfo2Vco.frequency(value + 1);
    }
    break;
    case 3: // LFO Lvl
    {
        float ampl = value / MIDI_CC_RANGE_FLOAT;
        Lfo2Vco.amplitude(ampl);
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
        LFO.frequency(value + 1);
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
    case 12:
    {
        int attack = map(value, 0, MIDI_CC_RANGE, 0, 1000);
        Fenv.attack(attack);
        Aenv.attack(attack);
    }
    break;
    case 13:
    {
        int decay = map(value, 0, MIDI_CC_RANGE, 0, 500);
        Fenv.decay(decay);
        Aenv.decay(decay);
    }
    break;
    case 14:
    {
        float ampl = value / MIDI_CC_RANGE_FLOAT;
        Fenv.sustain(ampl);
        Aenv.sustain(ampl);
    }
    break;
    case 15:
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