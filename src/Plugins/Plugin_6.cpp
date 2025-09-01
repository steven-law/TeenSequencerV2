#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "ownLibs/Adafruit_ST7796S_kbv.h"
// #include <ILI9341_t3n.h>
// #include <ili9341_t3n_font_Arial.h> // from ILI9341_t3
#include "projectVariables.h"

#include <Plugins/Plugin_6.h>

extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;

void drawActiveRect(int xPos, uint8_t yPos, uint8_t xsize, uint8_t ysize, bool state, const char *name, int color);
void myDrawLine(int x0, int y0, int x1, int y1, uint16_t color);
void myDrawRect(int x, int y, int w, int h, uint16_t color);

extern int pixelTouchX;
extern int gridTouchY;
// TeensyDAW: begin automatically generated code
// Name: Draw
// Description: Additive Synthesizer
// Voices: 1

// VCO
// Pot 1: Vol
// Pot 2: Vol
// Pot 3: Vol
// Pot 4: Vol

// Pot 5: Vol
// Pot 6: Vol
// Pot 7: Vol
// Pot 8: Vol

// Filter:
// Pot 9: Frequency
// Pot 10: Resonance
// Pot 11: Sweep
// Pot 12: Type

// Envelope:
// Pot 9: Attack
// Pot 10: Decay
// Pot 11: Sustain
// Pot 12: Release

void Plugin_6::setup()
{
    IhaveADSR = true;
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
    // SongVol.gain(1);
    potentiometer[presetNr][0] = 0;
    potentiometer[presetNr][1] = 0;
    potentiometer[presetNr][2] = 0;
    potentiometer[presetNr][3] = 0;

    potentiometer[presetNr][4] = 0;
    potentiometer[presetNr][5] = 3;
    potentiometer[presetNr][6] = 7;
    potentiometer[presetNr][7] = 10;

    potentiometer[presetNr][8] = 32;
    potentiometer[presetNr][9] = 32;
    potentiometer[presetNr][10] = 32;
    potentiometer[presetNr][11] = 32;

    potentiometer[presetNr][12] = 40;
    potentiometer[presetNr][13] = 0;
    potentiometer[presetNr][14] = 0;
    potentiometer[presetNr][15] = 10;

    setParameterNames("W~F 1", "W~F 2", "W~F 3", "W~F 4", "Chord", "Volumes", "0", "0", "Filt-Freq", "Resonance", "Sweep", "0", "1", "1", "1", "1");
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
    switch (pot)
    {
    case 0:
    {
        int walveform = map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 0, 12);
        waveform[0].begin(walveform);
    }
    break;
    case 1:
    {
        int walveform = map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 0, 12);
        waveform[1].begin(walveform);
    }
    break;
    case 2:
    {
        int walveform = map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 0, 12);
        waveform[2].begin(walveform);
    }
    break;
    case 3:
    {
        int walveform = map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 0, 12);
        waveform[3].begin(walveform);
    }
    break;
    case 4:
    {
        pl6ChoosenChord = get_Potentiometer(pot);
    }
    break;
    case 5:
    {
        for (int i = 0; i < PL6_VOICES; i++)
        {
            float ampl = chordVolumes[get_Potentiometer(pot)][i] * 16;
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
        int frequency = note_frequency[get_Potentiometer(pot)] * tuning;
        filter.frequency(frequency);
    }
    break;
    case 9:
    {
        float reso = get_Potentiometer(pot) / 25.40;
        filter.resonance(reso);
    }
    break;
    case 10:
    {
        float value = map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 0, 7.00);
        filter.octaveControl(value);
    }
    break;
    case 11:

        break;
    case 12:
    {
        int attack = map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 0, 1000);
        Fenv.attack(attack);
        Aenv.attack(attack);
    }
    break;
    case 13:
    {
        int decay = map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 0, 500);
        Fenv.decay(decay);
        Aenv.decay(decay);
    }
    break;
    case 14:
    {
        float ampl = get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT;
        Fenv.sustain(ampl);
        Aenv.sustain(ampl);
    }
    break;
    case 15:
    {
        int release = map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 0, 2000);
        Fenv.release(release);
        Aenv.release(release);
    }
    break;
    default:
        break;
    }
}

Plugin_6 plugin_6("Adtv", 6);
