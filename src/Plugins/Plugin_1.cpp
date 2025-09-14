#include <Plugins/Plugin_1.h>


void Plugin_1::setup()
{
    IamPoly = true;
    for (int i = 0; i < MAX_VOICES; i++)
    {
        mixer.gain(i, 1);
        potentiometer[presetNr][i] = 80;
    }
    MixGain.gain(1);
    setParameterNames("Vol1", MIDI_CC_RANGE, "Vol2", MIDI_CC_RANGE, "Vol3", MIDI_CC_RANGE, "Vol4", MIDI_CC_RANGE,
                      "Vol5", MIDI_CC_RANGE, "Vol6", MIDI_CC_RANGE, "Vol7", MIDI_CC_RANGE, "Vol8", MIDI_CC_RANGE,
                      "Vol9", MIDI_CC_RANGE, "Vol10", MIDI_CC_RANGE, "Vol11", MIDI_CC_RANGE, "Vol12", MIDI_CC_RANGE,
                      "0", 0, "0", 0, "0", 0, "0", 0);

    // change_preset();
    // SongVol.gain(1);
}
void Plugin_1::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    float frequency = note_frequency[notePlayed] * tuning;

    mixer.gain(voice, (velocity * (potentiometer[presetNr][voice] / MIDI_CC_RANGE_FLOAT)));
    string[voice].noteOn(frequency, velocity);
    // Serial.printf("ON voice: %d, freq: %f \n", voice, frequency);
}
void Plugin_1::noteOff(uint8_t notePlayed, uint8_t voice)
{
    string[voice].noteOff(0);
    // Serial.printf("OFF voice: %d,  \n", voice);
}
void Plugin_1::assign_parameter(uint8_t pot)
{
    float sustain = get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT;
    mixer.gain(pot, sustain);
}

Plugin_1 plugin_1("Strg", 1);
// TeensyDAW: end automatically generated code