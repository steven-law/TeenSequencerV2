#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "projectVariables.h"

#include <Plugins/Plugin_12.h>
////#include "hardware/tftClass.h"
// class tftClass;
extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
extern const char *filterName[4];

void Plugin_12::setup()
{
  IhaveADSR = true;
  dc.amplitude(1);

  // waveform.begin(WAVEFORM_SINE);
  // waveform.amplitude(1);
  waveform.enableInterpolation(true);
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
  potentiometer[presetNr][0] = 0;
  potentiometer[presetNr][1] = 1;
  potentiometer[presetNr][1] = 48;
  potentiometer[presetNr][8] = 60;
  potentiometer[presetNr][9] = 0;
  potentiometer[presetNr][10] = 30;
  potentiometer[presetNr][11] = 0;
  potentiometer[presetNr][12] = 5;
  potentiometer[presetNr][13] = 0;
  potentiometer[presetNr][14] = 127;
  potentiometer[presetNr][15] = 20;
  setParameterNames("Bank", "File", "Volume", "0", "0", "0", "0", "0", "Filter-Freq", "Resonance", "Sweep", "Type", "1", "1", "1", "1");

  // SongVol.gain(1);
}
void Plugin_12::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
  // float frequency = note_frequency[notePlayed] * tuning;
  //  waveform.frequency(frequency);
  float velo = (velocity * (MixerGain / MIDI_CC_RANGE_FLOAT));
  MixGain.gain(velo);
  waveform.setPlaybackRate(note_frequency[notePlayed]);
  waveform.playRaw(_filename, 1);
  Fenv.noteOn();
  Aenv.noteOn();
  Serial.println("PL12 NOteOn");
  Serial.println(_filename);
}
void Plugin_12::noteOff(uint8_t notePlayed, uint8_t voice)
{
  Fenv.noteOff();
  Aenv.noteOff();
}

void Plugin_12::assign_parameter(uint8_t pot)
{
  switch (pot)
  {
  case 0:
  {
    sprintf(_filename, "%s%d.raw", bankNames[get_Potentiometer(pot)], potentiometer[presetNr][1]);
  }
  break;
  case 1:
  {
    sprintf(_filename, "%s%d.raw", bankNames[potentiometer[presetNr][0]], get_Potentiometer(pot));
  }
  break;
  case 2:
  {
    float ampl = get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT;
    fMixer.gain(0, ampl);
    fMixer.gain(1, ampl);
    fMixer.gain(2, ampl);
    fMixer.gain(3, ampl);
  }
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
    float swp = get_Potentiometer(pot) / 18.14;
    filter.octaveControl(swp);
  }
  break;
  case 11:
  {
    fMixer.gain(0, 0);
    fMixer.gain(1, 0);
    fMixer.gain(2, 0);
    fMixer.gain(3, 0);
    fMixer.gain(get_Potentiometer(pot), 1);
  }
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

Plugin_12 plugin_12("REC", 12);