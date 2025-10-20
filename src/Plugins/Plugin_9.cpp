#include <Plugins/Plugin_9.h>

void Plugin_9::setup()
{
  dc.amplitude(1);

  waveform.begin(WAVEFORM_SINE);
  waveform.amplitude(1);
  waveform1.begin(WAVEFORM_SINE);
  waveform1.amplitude(1);

  LFO.begin(WAVEFORM_SINE);
  LFO.amplitude(1);
  LFO.frequency(2);
  fEnvMixer.gain(0, 1);
  fEnvMixer.gain(1, 1);

  vcoMixer.gain(0, 1);
  vcoMixer.gain(1, 1);

  Fenv.delay(0);
  Fenv.attack(0);
  Fenv.hold(0);
  Fenv.decay(0);
  Fenv.sustain(1);
  Fenv.release(200);

  filter.frequency(2000);
  filter.resonance(1);
  filter.octaveControl(4);

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

  setParameterNames("W~Form", 12, "W~Form2", 12, "Detune", MIDI_CC_RANGE, "VCO Mix", MIDI_CC_RANGE,
                    "Env-Lvl", MIDI_CC_RANGE, "LFO W~F", 12, "LFO-Freq", MIDI_CC_RANGE, "LFO-Lvl", MIDI_CC_RANGE,
                    "Filt-Freq", MIDI_CC_RANGE, "Resonance", MIDI_CC_RANGE, "Sweep", MIDI_CC_RANGE, "Type", 3,
                    "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE);
  set_preset(0,
             0, 0, 41, 65,
             41, 0, 9, 20,
             45, 0, 24, 0,
             5, 0, 127, 20);
  set_preset(1,
             2, 2, 40, 65,
             55, 2, 5, 115,
             94, 0, 25, 0,
             5, 0, 127, 20);
  set_preset(2,
             2, 0, 0, 0,
             127, 0, 0, 0,
             40, 70, 80, 3,
             0, 35, 0, 0);
  set_preset(3,
             6, 1, 0, 45,
             61, 0, 0, 75,
             63, 70, 65, 0,
             0, 35, 60, 10);

  change_preset();
}
void Plugin_9::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
  float velo = (velocity * (MixerGain / MIDI_CC_RANGE_FLOAT));
  MixGain.gain(velo);
  waveform.frequency((note_frequency[notePlayed] * tuning));
  waveform1.frequency((note_frequency[notePlayed] * tuning) + detune2);
  // Serial.printf("pl8 detune1: %d\n", detune1);
  Fenv.noteOn();
  Fenv.noteOn();
  Aenv.noteOn();
}
void Plugin_9::noteOff(uint8_t notePlayed, uint8_t voice)
{
  Fenv.noteOff();
  Aenv.noteOff();
}
void Plugin_9::assign_parameter(uint8_t pot)
{
  uint8_t value = get_Potentiometer(pot);
  switch (pot)
  {
  case 0:
  {
    waveform.begin(value);
  }
  break;
  case 1:
  {
    waveform1.begin(value);
  }
  break;
  case 2:
  {
    detune2 = (float)((note_frequency[value] * 4.40));
  }
  break;
  case 3:
  {
    float ampl = value / MIDI_CC_RANGE_FLOAT;

    waveform.amplitude(1.00 - ampl);
    waveform1.amplitude(ampl);
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
  }
  break;
  case 6:
    LFO.frequency(value + 1);
    break;
  case 7:
  {
    float ampl = value / MIDI_CC_RANGE_FLOAT;
    fEnvMixer.gain(1, ampl);
  }
  break;
  case 8:
  {
    float frequency = note_frequency[value] * tuning;
    ladder.frequency(frequency);
    filter.frequency(frequency);
  }
  break;
  case 9:
  {
    float reso = value / 25.40;
    ladder.resonance(reso);
    filter.resonance(reso);
  }
  break;
  case 10:
  {
    float swp = value / 18.14;
    ladder.octaveControl(swp);
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

Plugin_9 plugin_9("Nord", 9);
