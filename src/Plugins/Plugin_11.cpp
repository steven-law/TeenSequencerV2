#include <Plugins/Plugin_11.h>

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

    setParameterNames("PW", MIDI_CC_RANGE, "Volume", MIDI_CC_RANGE, "PWM-Freq", MIDI_CC_RANGE, "PWM-Lvl", MIDI_CC_RANGE,
                      "Env-Lvl", MIDI_CC_RANGE, "LFO W~F", 12, "LFO-Freq", MIDI_CC_RANGE, "LFO-Lvl", MIDI_CC_RANGE,
                      "Filter-Freq", MIDI_CC_RANGE, "Resonance", MIDI_CC_RANGE, "Sweep", MIDI_CC_RANGE, "Type", 3,
                      "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE);
    set_preset(0,
               65, 70, 4, 55,
               116, 0, 5, 0,
               127, 0, 0, 0,
               5, 0, 127, 20);
    set_preset(1,
               65, 70, 16, 9,
               127, 0, 8, 7,
               60, 102, 48, 1,
               5, 0, 127, 20);
    set_preset(2,
               31, 70, 16, 51,
               76, 0, 0, 50,
               30, 28, 48, 1,
               5, 0, 127, 20);
    set_preset(3,
               95, 70, 113, 44,
               26, 2, 90, 50,
               0, 0, 0, 2,
               5, 0, 127, 20);
    change_preset();
}
void Plugin_11::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)

{
    float velo = (velocity * (MixerGain / MIDI_CC_RANGE_FLOAT));
    MixGain.gain(velo);
    float frequency = note_frequency[notePlayed] * tuning;
    waveform.frequency(frequency);
    Fenv.noteOn();
    Aenv.noteOn();
    Serial.printf("pl11 Note: %d, velocity: %d\n", notePlayed, velo);
}
void Plugin_11::noteOff(uint8_t notePlayed, uint8_t voice)
{
    Fenv.noteOff();
    Aenv.noteOff();
}

void Plugin_11::assign_parameter(uint8_t pot)
{
    uint8_t value = get_Potentiometer(pot);
    switch (pot)
    {
    case 0:
    {
        float ampl = value / MIDI_CC_RANGE_FLOAT;
        PWMdc.amplitude(ampl);
    }
    break;
    case 1:
    {
        float ampl = value / MIDI_CC_RANGE_FLOAT;
        waveform.amplitude(ampl);
    }
    break;
    case 2:
    {
        Lfo2Vco.frequency((value + 1) / 16.0f);
    }
    break;
    case 3:
    {
        float ampl = value / (MIDI_CC_RANGE_FLOAT * 2);
        Lfo2Vco.amplitude(ampl);
    }
    break;
    case 4:
    {
        float ampl = value / MIDI_CC_RANGE_FLOAT;
        dc.amplitude(ampl);
    }
    break;
    case 5:
    {
        LFO.begin(value);
        Lfo2Vco.begin(value);
    }
    break;
    case 6:
    {
        LFO.frequency((value + 1) / 16.0f);
    }
    break;
    case 7:
    {
        float ampl = value / MIDI_CC_RANGE_FLOAT;
        LFO.amplitude(ampl);
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

Plugin_11 plugin_11("PWM", 11);
