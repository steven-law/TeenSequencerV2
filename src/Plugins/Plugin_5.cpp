#include <Plugins/Plugin_5.h>

void Plugin_5::setup()
{

  for (int i = 0; i < MAX_VOICES; i++)
  {
    mixer.gain(i, 3);
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


  setParameterNames("Freq", MIDI_CC_RANGE, "Sweep", MIDI_CC_RANGE, "O-Drive", MIDI_CC_RANGE, "Decay", MIDI_CC_RANGE,
                    "Freq", MIDI_CC_RANGE, "Sweep", MIDI_CC_RANGE, "Noise", MIDI_CC_RANGE, "Decay", MIDI_CC_RANGE,
                    "Freq", MIDI_CC_RANGE, "Reso", MIDI_CC_RANGE, "Attack", MIDI_CC_RANGE, "Decay", MIDI_CC_RANGE,
                    "TomL", MIDI_CC_RANGE, "TomM", MIDI_CC_RANGE, "TomH", MIDI_CC_RANGE, "Decay", MIDI_CC_RANGE);

  set_preset(0,
             16, 4, 29, 49,
             55, 0, 90, 86,
             60, 127, 0, 20,
             18, 23, 28, 25);
  set_preset(1,
             28, 15, 116, 87,
             63, 22, 127, 30,
             37, 127, 16, 37,
             39, 46, 50, 18);
  set_preset(2,
             21, 0, 58, 31,
             55, 7, 0, 50,
             40, 60, 0, 50,
             60, 65, 70, 30);
  set_preset(3,
             45, 10, 20, 40,
             60, 2, 60, 30,
             90, 110, 10, 100,
             50, 53, 56, 25);
  change_preset();
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
  if (voice == 2)
  {
    hhEnv.noteOff();
    hhFilterEnv.noteOff();
  }
}
void Plugin_5::assign_parameter(uint8_t pot)
{
  uint8_t value = get_Potentiometer(pot);
  switch (pot)
  {
  case 0:
  {
    int freq = map(value, 0, MIDI_CC_RANGE, 10, 300);
    fm_drum.frequency(freq);
  }
  break;
  case 1:
  {
    float sustain = value / MIDI_CC_RANGE_FLOAT;
    fm_drum.fm(sustain);
  }
  break;
  case 2:
  {
    float sustain = (float)(value / MIDI_CC_RANGE_FLOAT);
    fm_drum.overdrive(sustain);
  }
  break;
  case 3:
  {
    float sustain = (float)(value / MIDI_CC_RANGE_FLOAT);
    fm_drum.decay(sustain);
  }
  break;
  case 4:
  {
    int freq = map(value, 0, MIDI_CC_RANGE, 10, 300);
    fm_snare.frequency(freq);
  }
  break;
  case 5:
  {
    float sustain = value / MIDI_CC_RANGE_FLOAT;
    fm_snare.fm(sustain);
  }
  break;
  case 6:
  {
    float sustain = value / MIDI_CC_RANGE_FLOAT;
    fm_snare.noise(sustain);
    fm_snare.overdrive(sustain);
  }
  break;
  case 7:
  {
    float sustain = value / MIDI_CC_RANGE_FLOAT;
    fm_snare.decay(sustain);
  }
  break;
  case 8:
  {
    int frequency = map(value, 0, MIDI_CC_RANGE, 1000, 8000);
    filter.frequency(frequency);
  }
  break;
  case 9:
  {
    float reso = (float)(value / (MIDI_CC_RANGE_FLOAT / MAX_RESONANCE));
    filter.resonance(reso);
  }
  break;
  case 10:
  {
    int attack = map(value, 0, MIDI_CC_RANGE, 0, 50);
    hhEnv.attack(attack);
  }
  break;
  case 11:
  {
    int release = map(value, 0, MIDI_CC_RANGE, 0, 2000);
    hhEnv.release(release);
    hhFilterEnv.release(release);
    hhEnv.decay(release / 4);
    hhFilterEnv.decay(release / 4);
  }
  break;
  case 12:
  {
    int freq = note_frequency[value] * tuning;
    tomL.frequency(freq);
  }
  break;
  case 13:
  {
    int freq = note_frequency[value] * tuning;
    tomM.frequency(freq);
  }
  break;
  case 14:
  {
    int freq = note_frequency[value] * tuning;
    tomH.frequency(freq);
  }
  break;
  case 15:
  {
    int decay = map(value, 0, MIDI_CC_RANGE, 0, 2000);
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
