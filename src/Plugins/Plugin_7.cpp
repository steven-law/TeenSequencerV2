#include <Plugins/Plugin_7.h>

// Name: Boom
// Description:

void Plugin_7::setup()
{

    // dynamics.gate(threshold, attack, release, hysterisis);              // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
    // dynamics.compression(threshold, attack, release, ratio, kneeWidth); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 1.0f-60.0f ;; 0.0f-32.0f
    dynamics.limit(threshold, attack, release); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
    dynamics.autoMakeupGain(6.0f);
    dynamics.makeupGain(6.0f);
    MixGain.gain(1);
    // SongVol.gain(1);
    setParameterNames("Sweep", MIDI_CC_RANGE, "Noise", MIDI_CC_RANGE, "Q-Drive", MIDI_CC_RANGE, "Decay", MIDI_CC_RANGE,
                      "Thrshld", MIDI_CC_RANGE, "Attack", MIDI_CC_RANGE, "Release", MIDI_CC_RANGE, "Hysteresis", MIDI_CC_RANGE,
                      "Ratio", MIDI_CC_RANGE, "Knewdth", MIDI_CC_RANGE, "AmakUpG", MIDI_CC_RANGE, "makUpG", MIDI_CC_RANGE,
                      "0", 0, "0", 0, "0", 0, "Gain", MIDI_CC_RANGE);
}
void Plugin_7::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    float velo = (velocity * (MixerGain / MIDI_CC_RANGE_FLOAT));
    MixGain.gain(velo);
    float frequence = note_frequency[notePlayed] * tuning;
    fm_drum.frequency(frequence);
    fm_drum.noteOn();
}
void Plugin_7::noteOff(uint8_t notePlayed, uint8_t voice)
{
}
void Plugin_7::assign_parameter(uint8_t pot)
{
    switch (pot)
    {
    case 0:
    {
        float sustain = get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT;
        fm_drum.fm(sustain); // float 0-1
    }
    break;
    case 1:
    {
        float sustain = get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT;
        fm_drum.noise(sustain); // float 0-1
    }
    break;
    case 2:
    {
        float sustain = get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT;
        fm_drum.overdrive(sustain); // float 0-1
    }
    break;
    case 3:
    {
        float sustain = get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT;
        fm_drum.decay(sustain); // float 0-1
    }
    break;
    case 4:
    {
        threshold = (float)map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, MIN_DB, MAX_DB);
        dynamics.gate(threshold, attack, release, hysterisis);              // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
        dynamics.compression(threshold, attack, release, ratio, kneeWidth); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 1.0f-60.0f ;; 0.0f-32.0f
        dynamics.limit(threshold, attack, release);                         // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
    }
    break;
    case 5:
    {
        attack = (float)map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, MIN_T, MAX_T);
        dynamics.gate(threshold, attack, release, hysterisis);              // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
        dynamics.compression(threshold, attack, release, ratio, kneeWidth); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 1.0f-60.0f ;; 0.0f-32.0f
        dynamics.limit(threshold, attack, release);                         // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
    }
    break;
    case 6:
    {
        release = (float)map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, MIN_T, MAX_T);
        dynamics.gate(threshold, attack, release, hysterisis);              // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
        dynamics.compression(threshold, attack, release, ratio, kneeWidth); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 1.0f-60.0f ;; 0.0f-32.0f
        dynamics.limit(threshold, attack, release);                         // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
    }
    break;
    case 7:
    {
        hysterisis = (float)map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 0.0f, 6.0f);
        dynamics.gate(threshold, attack, release, hysterisis); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 0.0f-6.0f
    }
    break;
    case 8:
    {
        ratio = (float)map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, RATIO_OFF, RATIO_INFINITY);
        dynamics.compression(threshold, attack, release, ratio, kneeWidth); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 1.0f-60.0f ;; 0.0f-32.0f
    }
    break;
    case 9:
    {
        kneeWidth = (float)map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 0.0f, 32.0f);
        dynamics.compression(threshold, attack, release, ratio, kneeWidth); // float -110.0f-0.0f ;; 0.03f-4.00f ;; 0.03f-4.00f ;; 1.0f-60.0f ;; 0.0f-32.0f
    }
    break;
    case 10:
    {
        float sustain = (float)map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, -12.0f, 24.0f);
        dynamics.makeupGain(sustain); // float -12.0f, 24.0f
    }
    break;
    case 11:
    {
        float sustain = (float)map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 0.0f, 60.0f);
        dynamics.autoMakeupGain(sustain); // float 0.0f, 60.0f
    }
    break;
    case 12:
    {
        float sustain = (float)map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 0.0f, 6.0f);
        amp.gain(sustain); // float 0.0f, 60.0f
    }
    break;
    case 13:

        break;
    case 14:

        break;
    case 15:

        break;
    default:
        break;
    }
}

Plugin_7 plugin_7("Boom", 7);
