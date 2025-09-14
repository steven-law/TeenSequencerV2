#include <Plugins/Plugin_10.h>

void Plugin_10::setup()
{
    dc.amplitude(1);

    waveform.setInstrument(Flute_100kuint8_t);
    waveform.amplitude(1);

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

    // mixer.gain(0, 1);

    MixGain.gain(1);
    potentiometer[presetNr][0] = 1;
    potentiometer[presetNr][1] = 48;
    potentiometer[presetNr][8] = 60;
    potentiometer[presetNr][9] = 0;
    potentiometer[presetNr][10] = 30;
    potentiometer[presetNr][11] = 0;
    potentiometer[presetNr][12] = 5;
    potentiometer[presetNr][13] = 0;
    potentiometer[presetNr][14] = 127;
    potentiometer[presetNr][15] = 20;
    setParameterNames("W~Form", 27, "Volume", MIDI_CC_RANGE, "0", 0, "0", 0,
                      "0", 0, "0", 0, "0", 0, "0", 0,
                      "Filt-Freq", MIDI_CC_RANGE, "Resonance", MIDI_CC_RANGE, "Sweep", MIDI_CC_RANGE, "Type", 3,
                      "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE);

    // SongVol.gain(1);
}
void Plugin_10::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    float velo = (velocity * (MixerGain / MIDI_CC_RANGE_FLOAT));
    MixGain.gain(velo);
    float frequency = note_frequency[notePlayed] * tuning;
    Serial.printf("pl10: notePlayed= %d\n", notePlayed);
    waveform.playFrequency(frequency);
    Fenv.noteOn();
    Aenv.noteOn();
}
void Plugin_10::noteOff(uint8_t notePlayed, uint8_t voice)
{
    Fenv.noteOff();
    Aenv.noteOff();
}

void Plugin_10::assign_parameter(uint8_t pot)
{
    uint8_t value = get_Potentiometer(pot);
    switch (pot)
    {
    case 0:
        assign_voice_waveform(value);
        break;
    case 1:
    {
        float ampl = value / MIDI_CC_RANGE_FLOAT;
        fMixer.gain(0, 0);
        fMixer.gain(1, 0);
        fMixer.gain(2, 0);
        fMixer.gain(3, 0);
        fMixer.gain(get_Potentiometer(11), ampl);
    }
    break;
    case 2:

        break;
    case 3:

        break;
    case 4:

        break;
    case 5:

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

void Plugin_10::assign_voice_waveform(uint8_t value)
{
    uint8_t walveform = map(value, 0, MIDI_CC_RANGE, 0, 27);

    switch (walveform)
    {
    case 0:
        waveform.setInstrument(Flute_100kuint8_t);
        break;
    case 1:
        waveform.setInstrument(BasicFlute1);
        break;
    case 2:
        waveform.setInstrument(FrenchHorns);
        break;
    case 3:
        waveform.setInstrument(Ocarina);
        break;
    case 4:
        waveform.setInstrument(Ocarina);
        break;
    case 5:
        waveform.setInstrument(Pizzicato);
        break;
    case 6:
        waveform.setInstrument(bassoon);
        break;
    case 7:
        waveform.setInstrument(clarinet);
        break;

    case 8:
        waveform.setInstrument(distortiongt);
        break;
    case 9:
        waveform.setInstrument(epiano);
        break;
    case 10:
        waveform.setInstrument(flute);
        break;
    case 11:
        waveform.setInstrument(glockenspiel);
        break;
    case 12:
        waveform.setInstrument(gtfretnoise);
        break;
    case 13:
        waveform.setInstrument(harmonica);
        break;
    case 14:
        waveform.setInstrument(harp);
        break;
    case 15:
        waveform.setInstrument(mutedgtr);
        break;
    case 16:
        waveform.setInstrument(nylonstrgtr);
        break;
    case 17:
        waveform.setInstrument(oboe);
        break;
    case 18:
        waveform.setInstrument(overdrivegt);
        break;
    case 19:
        waveform.setInstrument(piano);
        break;
    case 20:
        waveform.setInstrument(recorder);
        break;
    case 21:
        waveform.setInstrument(standard_DRUMS);
        break;
    case 22:
        waveform.setInstrument(steelstrgtr);
        break;
    case 23:
        waveform.setInstrument(strings);
        break;
    case 24:
        waveform.setInstrument(trombone);
        break;
    case 25:
        waveform.setInstrument(trumpet);
        break;
    case 26:
        waveform.setInstrument(tuba);
        break;
    case 27:
        waveform.setInstrument(vibraphone);
        break;

    default:
        waveform.setInstrument(Pizzicato);
        break;
    }
}

Plugin_10 plugin_10("SF2", 10);
