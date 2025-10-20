#include <Plugins/Plugin_6.h>

void Plugin_6::setup()
{
    dc.amplitude(1);
    for (int i = 0; i < PL6_VOICES; i++)
    {
        mixer.gain(i, 1);
        waveform[i].begin(WAVEFORM_SQUARE);
        waveform[i].amplitude(1);
    }
    Fenv.delay(0);
    Fenv.attack(0);
    Fenv.hold(0);
    Fenv.decay(0);
    Fenv.sustain(.7);
    Fenv.release(200);

    filter.frequency(2000);
    filter.resonance(1);
    filter.octaveControl(0);

    fMixer.gain(0, 1);
    fMixer.gain(1, 0);
    fMixer.gain(2, 0);
    fMixer.gain(3, 0);

    Aenv.delay(0);
    Aenv.attack(0);
    Aenv.hold(0);
    Aenv.decay(0);
    Aenv.sustain(.7);
    Aenv.release(200);

    MixGain.gain(1);

    setParameterNames("W~F 1", 12, "W~F 2", 12, "W~F 3", 12, "W~F 4", 12,
                      "Chord", NUM_CHORDS, "Volumes", NUM_CHORD_VOLUMES, "0", 0, "0", 0,
                      "Filt-Freq", MIDI_CC_RANGE, "Resonance", MIDI_CC_RANGE, "Sweep", MIDI_CC_RANGE, "0", 0,
                      "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE);
    set_preset(0,
               0, 0, 0, 0,
               0, 44, 0, 0,
               74, 0, 30, 0,
               0, 0, 127, 10);
    set_preset(1,
               2, 2, 2, 2,
               2, 41, 0, 0,
               0, 30, 95, 0,
               5, 30, 60, 51);
    set_preset(2,
               1, 1, 3, 3,
               11, 41, 0, 0,
               73, 38, 95, 0,
               22, 30, 100, 38);
    set_preset(3,
               2, 2, 5, 5,
               0, 41, 0, 0,
               117, 0, 0, 0,
               5, 25, 55, 40);

    change_preset();
}
void Plugin_6::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    float frequency[PL6_VOICES];
    float velo = (velocity * (MixerGain / MIDI_CC_RANGE_FLOAT));
    MixGain.gain(velo);
    for (int i = 0; i < PL6_VOICES; i++)
    {
        frequency[i] = note_frequency[notePlayed + chords[pl6ChoosenChord][i]] * tuning;
        waveform[i].frequency(frequency[i]);
        Serial.printf("pl6 note %d, v= %d, freq: %f\n", notePlayed + chords[pl6ChoosenChord][i], i, frequency[i]);
    }

    Fenv.noteOn();
    Aenv.noteOn();
}
void Plugin_6::noteOff(uint8_t notePlayed, uint8_t voice)
{
    Fenv.noteOff();
    Aenv.noteOff();
}
void Plugin_6::assign_parameter(uint8_t pot)
{
    uint8_t value = get_Potentiometer(pot);
    switch (pot)
    {
    case 0:
    {
        waveform[0].begin(value);
    }
    break;
    case 1:
    {
        waveform[1].begin(value);
    }
    break;
    case 2:
    {
        waveform[2].begin(value);
    }
    break;
    case 3:
    {
        waveform[3].begin(value);
    }
    break;
    case 4:
    {
        pl6ChoosenChord = value;
    }
    break;
    case 5:
    {
        for (int i = 0; i < PL6_VOICES; i++)
        {
            float ampl = chordVolumes[value][i] * 0.125;
            waveform[i].amplitude(ampl);
        }
    }
    break;
    case 6:

        break;
    case 7:

        break;
    case 8:
    {
        int frequency = note_frequency[value] * tuning;
        filter.frequency(frequency);
    }
    break;
    case 9:
    {
        float reso = value / 25.40;
        filter.resonance(reso);
    }
    break;
    case 10:
    {
        float sweep = map(value, 0, MIDI_CC_RANGE, 0, 7.00);
        filter.octaveControl(sweep);
    }
    break;
    case 11:

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

Plugin_6 plugin_6("Adtv", 6);
