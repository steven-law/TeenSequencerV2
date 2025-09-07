#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"

#include <Plugins/Plugin_5.h>



extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;

void Plugin_5::setup()
{

  for (int i = 0; i < MAX_VOICES; i++)
  {
    mixer.gain(i, 1);
  }
  pink.amplitude(1);
  hhEnv.delay(0);
  hhEnv.attack(0);
  hhEnv.hold(0);
  hhEnv.decay(0);
  hhEnv.sustain(0.4);
  hhFilterEnv.sustain(0.4);
  hhEnv.release(200);

  MixGain.gain(1);
  // SongVol.gain(1);
  tomL.secondMix(0);
  tomL.pitchMod(0.7);
  tomM.secondMix(0);
  tomM.pitchMod(0.7);
  tomH.secondMix(0);
  tomH.pitchMod(0.7);
  setParameterNames("Freq", "Sweep", "O-Drive", "Decay", "Freq", "Sweep", "Noise", "Decay", "Freq", "Reso", "Attack", "Decay", "TomL", "TomM", "TomH", "Decay");
}
void Plugin_5::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
  float velo = (velocity * (MixerGain / MIDI_CC_RANGE_FLOAT));
  MixGain.gain(velo);
  if (voice == 0)
    fm_drum.noteOn();

  if (voice == 1)
    fm_snare.noteOn();
  if (voice == 2)
  {
    hhEnv.noteOn();
    hhFilterEnv.noteOn();
  }
  if (voice == 3)
    tomL.noteOn();
  if (voice == 4)
    tomM.noteOn();
  if (voice == 5)
    tomH.noteOn();
}
void Plugin_5::noteOff(uint8_t notePlayed, uint8_t voice)
{
  if (notePlayed == 50)
  {
    hhEnv.noteOff();
    hhFilterEnv.noteOff();
  }
}
void Plugin_5::assign_parameter(uint8_t pot)
{
  switch (pot)
  {
  case 0:
  {
    int freq = map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 10, 300);
    fm_drum.frequency(freq);
  }
  break;
  case 1:
  {
    float sustain = get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT;
    fm_drum.fm(sustain);
  }
  break;
  case 2:
  {
    float sustain = (float)(get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT);
    fm_drum.overdrive(sustain);
  }
  break;
  case 3:
  {
    float sustain = (float)(get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT);
    fm_drum.decay(sustain);
  }
  break;
  case 4:
  {
    int freq = map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 10, 300);
    fm_snare.frequency(freq);
  }
  break;
  case 5:
  {
    float sustain = get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT;
    fm_snare.fm(sustain);
  }
  break;
  case 6:
  {
    float sustain = get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT;
    fm_snare.noise(sustain);
    fm_snare.overdrive(sustain);
  }
  break;
  case 7:
  {
    float sustain = get_Potentiometer(pot) / MIDI_CC_RANGE_FLOAT;
    fm_snare.decay(sustain);
  }
  break;
  case 8:
  {
    int frequency = map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 1000, 8000);
    filter.frequency(frequency);
  }
  break;
  case 9:
  {
    float reso = (float)(get_Potentiometer(pot) / (MIDI_CC_RANGE_FLOAT / MAX_RESONANCE));
    filter.resonance(reso);
  }
  break;
  case 10:
  {
    int attack = map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 0, 50);
    hhEnv.attack(attack);
  }
  break;
  case 11:
  {
    int release = map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 0, 2000);
    hhEnv.release(release);
    hhFilterEnv.release(release);
    hhEnv.decay(release / 4);
    hhFilterEnv.decay(release / 4);
  }
  break;
  case 12:
  {
    int freq = note_frequency[get_Potentiometer(pot)] * tuning;
    tomL.frequency(freq);
  }
  break;
  case 13:
  {
    int freq = note_frequency[get_Potentiometer(pot)] * tuning;
    tomM.frequency(freq);
  }
  break;
  case 14:
  {
    int freq = note_frequency[get_Potentiometer(pot)] * tuning;
    tomH.frequency(freq);
  }
  break;
  case 15:
  {
    int decay = map(get_Potentiometer(pot), 0, MIDI_CC_RANGE, 0, 2000);
    tomL.length(decay);
    tomM.length(decay);
    tomH.length(decay);
  }
  break;
  default:
    break;
  }
}

Plugin_5 plugin_5("Drum", 5);
