#include <Plugins/Plugin_12.h>

void Plugin_12::setup()
{
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

  MixGain.gain(1);

  setParameterNames("Bank", NUM_SAMPLE_BANKS, "File", MIDI_CC_RANGE, "Volume", MIDI_CC_RANGE, "0", 0,
                    "0", 0, "0", 0, "0", 0, "0", 0,
                    "Filter-Freq", MIDI_CC_RANGE, "Resonance", MIDI_CC_RANGE, "Sweep", MIDI_CC_RANGE, "Type", 3,
                    "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE);
  set_preset(0,
             0, 1, 80, 0,
             0, 0, 0, 0,
             127, 0, 0, 0,
             0, 0, 127, 20);
  set_preset(1,
             2, 39, 80, 0,
             0, 0, 0, 0,
             127, 0, 0, 0,
             0, 0, 127, 20);
  set_preset(2,
             5, 19, 80, 0,
             0, 0, 0, 0,
             127, 0, 0, 0,
             0, 0, 127, 20);
  set_preset(3,
             7, 15, 80, 0,
             0, 0, 0, 0,
             30, 127, 110, 0,
             5, 20, 110, 25);
  change_preset();
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
  uint8_t value = get_Potentiometer(pot);
  switch (pot)
  {
  case 0:
  {
    sprintf(_filename, "%s%d.raw", bankNames[value], potentiometer[presetNr][1]);
  }
  break;
  case 1:
  {
    sprintf(_filename, "%s%d.raw", bankNames[potentiometer[presetNr][0]], value);
  }
  break;
  case 2:
  {
    float ampl = value / MIDI_CC_RANGE_FLOAT;
    fMixer.gain(0, 0);
    fMixer.gain(1, 0);
    fMixer.gain(2, 0);
    fMixer.gain(3, 0);
    fMixer.gain(get_Potentiometer(11), ampl);
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
    fMixer.gain(value, get_Potentiometer(2));
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

Plugin_12 plugin_12("REC", 12);