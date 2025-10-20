#include <Plugins/Plugin_1.h>

void Plugin_1::setup()
{
    IamPoly = true;
    for (int i = 0; i < MAX_VOICES; i++)
    {
        mixer.gain(i, 1);
        potentiometer[presetNr][i] = 80;
    }
    lfo.begin(1, 1, WAVEFORM_SINE);
    waveshape.shape(waveshapeArray, sizeof(waveshapeArray) / sizeof(waveshapeArray[0]));
    dc.amplitude(1);

    MixGain.gain(1);
    setParameterNames("Vol", MIDI_CC_RANGE, "Distortion", MIDI_CC_RANGE, "0", 0, "0", 0,
                      "LFO-Lvl", MIDI_CC_RANGE, "LFO-Freq", MIDI_CC_RANGE, "LFO-W~F", 12, "0", 0,
                      "0", 0, "0", 0, "0", 0, "0", 0,
                      "0", 0, "0", 0, "0", 0, "0", 0);
    set_preset(0,
               99, 0, 0, 0,
               0, 0, 0, 0,
               0, 0, 0, 0,
               0, 0, 0, 0);
    set_preset(1,
               99, 45, 0, 0,
               0, 1, 0, 0,
               0, 0, 0, 0,
               0, 0, 0, 0);
    set_preset(2,
               99, 0, 0, 0,
               48, 4, 0, 0,
               0, 0, 0, 0,
               0, 0, 0, 0);
    set_preset(3,
               99, 16, 0, 0,
               48, 90, 0, 0,
               0, 0, 0, 0,
               0, 0, 0, 0);

    change_preset();
}
void Plugin_1::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    float frequency = note_frequency[notePlayed] * tuning;

    mixer.gain(voice, (velocity * (potentiometer[presetNr][0] / MIDI_CC_RANGE_FLOAT)));

    string[voice].noteOn(frequency, velocity);
    Serial.printf("pl1 ON voice: %d, freq: %f \n", voice, frequency);
}
void Plugin_1::noteOff(uint8_t notePlayed, uint8_t voice)
{
    string[voice].noteOff(0);
    // Serial.printf("OFF voice: %d,  \n", voice);
}
void Plugin_1::assign_parameter(uint8_t pot)
{
    int value = get_Potentiometer(pot);
    Serial.println(pot);
    switch (pot)
    {
    case 0:
    {
        for (int i = 0; i < MAX_VOICES; i++)
        {
            float sustain = (3 * value) / MIDI_CC_RANGE_FLOAT;
            mixer.gain(i, sustain);
        }
    }
    break;
    case 1:
    {
        float sustain = (value) / MIDI_CC_RANGE_FLOAT;
        distortionMix.gain(0, 1.00f - sustain);
        distortionMix.gain(1, sustain);
    }
    break;
    case 2:
        /* code */
        break;
    case 3:
        /* code */
        break;
    case 4:
    {
        float sustain = (value) / MIDI_CC_RANGE_FLOAT;
        modMix.gain(1, 1.00f - sustain);
        modMix.gain(0, sustain);
    }
    break;
    case 5:
    {
        lfo.begin(1, (value + 1) / 2.0f, potentiometer[presetNr][6]);
        //  lfo.frequency(note_frequency[value] * 44);
    }
    break;
    case 6:
    {
        lfo.begin(value);
    }
    break;
    case 7:
        /* code */
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
        /* code */
        break;
    case 13:
        /* code */
        break;
    case 14:
        /* code */
        break;
    case 15:
        /* code */
        break;
    default:
        break;
    }
}

Plugin_1 plugin_1("Strg", 1);
// TeensyDAW: end automatically generated code