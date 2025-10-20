#include <Plugins/Plugin_8.h>

void Plugin_8::setup()
{
    dc.amplitude(1);

    waveform.begin(WAVEFORM_SINE);
    waveform.amplitude(1);
    waveform1.begin(WAVEFORM_SINE);
    waveform1.amplitude(1);

    vcoMixer.gain(0, 1);
    vcoMixer.gain(1, 1);

    Fenv.delay(0);
    Fenv.attack(0);
    Fenv.hold(0);
    Fenv.decay(0);
    Fenv.sustain(1);
    Fenv.release(200);

    filter.frequency(2000);
    filter.resonance(1);
    filter.octaveControl(4);

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
    // SongVol.gain(1);
    setParameterNames("W~Form", 12, "Detune", MIDI_CC_RANGE, "Volume 1", MIDI_CC_RANGE, "0", 0,
                      "W~Form", 12, "Detune", MIDI_CC_RANGE, "Volume 2", MIDI_CC_RANGE, "0", 0,
                      "Filt-Freq", MIDI_CC_RANGE, "Resonance", MIDI_CC_RANGE, "Sweep", MIDI_CC_RANGE, "Type", 2,
                      "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE);
    set_preset(0,
               1, 0, 56, 0,
               1, 90, 56, 0,
               60, 0, 30, 0,
               0, 0, 127, 20);
    set_preset(1,
               3, 0, 65, 0,
               0, 65, 65, 0,
               60, 0, 30, 0,
               20, 0, 127, 20);
    set_preset(2,
               2, 0, 65, 0,
               2, 80, 65, 0,
               50, 100, 46, 1,
               0, 0, 127, 20);
    set_preset(3,
               5, 0, 95, 0,
               5, 64, 95, 0,
               50, 55, 46, 2,
               0, 0, 127, 20);
    change_preset();
}
void Plugin_8::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    float velo = (velocity * (MixerGain / MIDI_CC_RANGE_FLOAT));
    MixGain.gain(velo);
    waveform.frequency((note_frequency[notePlayed] + detune1) * tuning);
    waveform1.frequency((note_frequency[notePlayed] + detune2) * tuning);
    // Serial.printf("pl8 detune1: %d\n", detune1);
    Fenv.noteOn();
    Aenv.noteOn();
}
void Plugin_8::noteOff(uint8_t notePlayed, uint8_t voice)
{
    Fenv.noteOff();
    Aenv.noteOff();
}
void Plugin_8::assign_parameter(uint8_t pot)
{
    uint8_t value = get_Potentiometer(pot);
    Serial.println(pot);
    switch (pot)
    {
    case 0:
    {

        waveform.begin(value);
    }
    break;
    case 1:
    {
        detune1 = (float)((note_frequency[value] * 0.01));
    }
    break;
    case 2:
    {
        float ampl = value / MIDI_CC_RANGE_FLOAT;
        waveform.amplitude(ampl);
    }
    break;
    case 3:
    {
    }
    break;
    case 4:
    {
        waveform1.begin(value);
    }
    break;
    case 5:
    {
        detune2 = (float)((note_frequency[value] * 0.01));
    }
    break;
    case 6:
    {
        float ampl = value / MIDI_CC_RANGE_FLOAT;
        waveform1.amplitude(ampl);
    }
    break;
    case 7:
    {
    }
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
        float swp = value / 18.14;
        filter.octaveControl(swp);
    }
    break;
    case 11:
    {
        fMixer.gain(0, 0);
        fMixer.gain(1, 0);
        fMixer.gain(2, 0);
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

Plugin_8 plugin_8("dTune", 8);
