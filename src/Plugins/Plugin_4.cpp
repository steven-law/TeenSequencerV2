#include <Plugins/Plugin_4.h>

void Plugin_4::setup()
{
    IamPoly = true;
    for (int i = 0; i < PL4_VOICES; i++)
    {
        playMem[i].begin();
        playMem[i].enableInterpolation(false);
        AEnv[i].attack(0);
        AEnv[i].decay(0);
        AEnv[i].sustain(1);
        AEnv[i].release(100);
        mixer.gain(i, 1);
    }

    MixGain.gain(1);
    for (int i = 0; i < PL4_VOICES; i++)
    {
        potentiometer[presetNr][i] = 10;
        potentiometer[presetNr][i + 8] = 127;
    }
    setParameterNames("Bk K", MIDI_CC_RANGE, "Bk C", MIDI_CC_RANGE, "Bk H", MIDI_CC_RANGE, "Bk S", MIDI_CC_RANGE,
                      "Bk P", MIDI_CC_RANGE, "Bk F", MIDI_CC_RANGE, "Bk X", MIDI_CC_RANGE, "Bk *", MIDI_CC_RANGE,
                      "Vol", MIDI_CC_RANGE, "Vol", MIDI_CC_RANGE, "Vol", MIDI_CC_RANGE, "Vol", MIDI_CC_RANGE,
                      "Vol", MIDI_CC_RANGE, "Vol", MIDI_CC_RANGE, "Vol", MIDI_CC_RANGE, "Vol", MIDI_CC_RANGE);
}
void Plugin_4::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    if (voice >= PL4_VOICES)
        return;
    AEnv[voice].sustain(velocity);
    playMem[voice].playRaw(sample[voice]->sampledata, sample[voice]->samplesize, 1);
    AEnv[voice].noteOn();
    Serial.printf("pl4 play %s on voice %d, velocity: %02f\n", _fileName[voice], voice, velocity);
}
void Plugin_4::noteOff(uint8_t notePlayed, uint8_t voice)
{
    if (voice >= PL4_VOICES)
        return;
    AEnv[voice].noteOff();
    // Serial.printf("pl4 Note Off voice: %d\n", voice);
}

void Plugin_4::assign_parameter(uint8_t pot)
{
    uint8_t value = get_Potentiometer(pot);
    if (pot < 8)
    {
        sprintf(_fileName[pot], "%s%d.raw", bankNames[pot], value);
        newdigate::flashloader loader;
        sample[pot] = loader.loadSample(_fileName[pot]);
    }
    else
    {
        float sustain = value / MIDI_CC_RANGE_FLOAT;
        mixer.gain(pot - 8, sustain);
    }
}

Plugin_4 plugin_4("mDrm", 4);

// TeensyDAW: end automatically generated code