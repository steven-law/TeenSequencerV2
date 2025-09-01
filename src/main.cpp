#include <Arduino.h>

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "ownLibs/Adafruit_ST7796S_kbv.h"
#include <MIDI.h>
#include <USBHost_t36.h>
#include <Adafruit_MCP4728.h>
// #include "smfwriter.h"
#include <SerialFlash.h>
#include "projectVariables.h"

#include "input/encoder.h"
#include "input/trellis.h"
#include "input/mouse.h"
#include "input/touch.h"

#include "output/tft.h"
#include "output/dac.h"
#include "output/TrellisLED.h"

#include <ownLibs/myClock.h>
#include "Track.h"

#include <Plugins/plugin_List.h>
#include <FX/fx_List.h>
#include "FX/Output.h"

extern MyTrellis trellisOut;

PluginControll *allPlugins[NUM_PLUGINS] = {&plugin_1, &plugin_2, &plugin_3, &plugin_4, &plugin_5, &plugin_6, &plugin_7, &plugin_8, &plugin_9, &plugin_10, &plugin_11, &plugin_12, &plugin_13, &plugin_14};
FX_1 fx_1("Rev", 21);
FX_2 fx_2("Bit", 22);
FX_3 fx_3("Nix", 23);
Output MasterOut(3);
MyClock myClock(1);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI1);
USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
MIDIDevice usbMidi1(myusb);
USBHIDParser hid1(myusb);
MouseController mouse1(myusb);
IntervalTimer usbDeviceTimer;
IntervalTimer SerialMidiTimer;
String inputString = "";
void input_behaviour();
// midi
void clock_to_notes(int _tick);
void midi_setup(uint8_t dly);
void midi_read();
void sendClock();
void sendStart();
void sendStop();
void onExternalStart();
void onExternalStop();

void sendNoteOn(uint8_t _track, uint8_t Note, uint8_t Velo, uint8_t Channel);
void sendNoteOff(uint8_t _track, uint8_t Note, uint8_t Velo, uint8_t Channel);
void sendControlChange(uint8_t control, uint8_t value, uint8_t Channel, uint8_t fromTrack = 255);

void myNoteOn(uint8_t channel, uint8_t note, uint8_t velocity);
void myNoteOff(uint8_t channel, uint8_t note, uint8_t velocity);
void myUSBNoteOn(uint8_t channel, uint8_t note, uint8_t velocity);
void myUSBNoteOff(uint8_t channel, uint8_t note, uint8_t velocity);
void myControlChange(uint8_t channel, uint8_t control, uint8_t value);
void myMIDIClock();
void usbDevice_handleInput();
void SerialMidi_handleInput();
void detect_USB_device();

void play_plugin_on_DAC(uint8_t _track, uint8_t _pluginNr);

// some trellis
void trellis_show_tft_mixer();
void trellis_perform();
void neotrellis_show_plugin();
void trellis_play_plugins();
uint8_t getPluginValue(uint8_t pot);
uint8_t getPluginValue(uint8_t pot)
{
  int trackChannel = allTracks[active_track]->clip[allTracks[active_track]->parameter[SET_CLIP2_EDIT]].midiChOut;
  int pluginChannel = trackChannel - (NUM_MIDI_OUTPUTS + 1);
  int pluginValue = allPlugins[pluginChannel]->get_Potentiometer(pot);
  Serial.printf("pluginValue =  %d, pluginPot: %d, pluginchannel= %d\n", pluginValue, pot, pluginChannel);
  return pluginValue;
}

void sendCCToActiveTracks(uint8_t cc, uint8_t val);
void printInfo(const char *label, int val, uint8_t cc);
template <typename T>
void updateFxVolume(uint8_t val, float gainVal, uint8_t cc, T *fxArray);
;

// mixer
void set_mixer();
void set_mixer_gain(uint8_t XPos, uint8_t YPos, const char *name, uint8_t trackn);
void set_mixer_FX_page1();
void set_mixer_FX_page2();
void set_mixer_dry(uint8_t XPos, uint8_t YPos, const char *name, uint8_t trackn);
void set_mixer_FX1(uint8_t XPos, uint8_t YPos, const char *name, uint8_t trackn);
void set_mixer_FX2(uint8_t XPos, uint8_t YPos, const char *name, uint8_t trackn);
void set_mixer_FX3(uint8_t XPos, uint8_t YPos, const char *name, uint8_t trackn);
template <typename FXClass>
void set_mixer_FX(uint8_t XPos, uint8_t YPos, const char *name, uint8_t trackn, FXClass &fx);
void save_plugin(uint8_t _songNr, uint8_t _pluginNr);
void load_plugin(uint8_t _songNr, uint8_t _pluginNr);
void show_trellisFX_mixerPage();
void set_sgtlControls(uint8_t row);
void draw_sgtlControls();
void set_sgtlcontrol(uint8_t _xPos, uint8_t _yPos, const char *name);
uint8_t get_sgtl_potentiometer(uint8_t index, uint8_t min, uint8_t max, const char *name);

void set_input_level(uint8_t _value);
void assign_PSRAM_variables();
bool compareFiles(File &file, SerialFlashFile &ffile);
void error(const char *message);
void export_midi_track(Track *track, int songNr, uint16_t ppqn = 24);
void export_serial_Midi(Track *track, uint16_t ppqn = 24);
bool loadMidiFile(const char *filename, MidiTrack &track);
void receiveClipData();
int getPPQN(const MidiTrack &track);
int fillNoteInfoAtTick(MidiTrack &track, int miditick);

void setup()
{
  // while (!Serial)
  {
    /* code */
  }
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(180000);
  Wire1.begin();
  Wire1.setClock(100000);
  SD.begin(BUILTIN_SDCARD);
  tft_setup(100);
  encoder_setup(100);
  trellisOut.setup(100);
  neotrellis_setup(100);
  touch_setup();
  dac_setup(100);
  Serial.println("neotrellis Setup done");
  delay(1500);
  midi_setup(100);

  myClock.setup();
  AudioMemory(330);
  delay(100);
  MasterOut.setup();
  // for (int i=0; i<NUM_PLUGINS-1;i++)
  {
    // allPlugins[i]->change_preset();
  }
  assign_PSRAM_variables();
  allPlugins[0]->change_preset();
  allPlugins[1]->change_preset();
  allPlugins[2]->change_preset();
  allPlugins[3]->change_preset();
  allPlugins[4]->change_preset();
  allPlugins[5]->change_preset();
  allPlugins[8]->change_preset();
  allPlugins[9]->change_preset();
  allPlugins[10]->change_preset();
  allPlugins[11]->change_preset();
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    allTracks[i]->mixFX1 = 0;
    allTracks[i]->mixFX2 = 0;
    allTracks[i]->mixFX3 = 0;
    allTracks[i]->mixFX1Pot = 0;
    allTracks[i]->mixFX2Pot = 0;
    allTracks[i]->mixFX3Pot = 0;
  }
  for (int i = 0; i < NUM_PLUGINS; i++)
  {
    fx_1.pl[i].gain(0);
    fx_2.pl[i].gain(0);
    fx_3.pl[i].gain(0);
  }
  //MasterOut.finalFilter.frequency(5500);
  //MasterOut.finalFilter.resonance(0);
  MasterOut.sgtl5000.dacVolume(.5);
  Serial.println("Audio & MIDI Setup done");

  assign_PSRAM_variables();
  // if (!SerialFlash.begin(FlashChipSelect))
  {
    //  error("Unable to access SPI Flash chip");
  }
  // else
  //   Serial.println("Access SPI Chip");
  startUpScreen();
  // tft.update();
  // tft.updateScreenAsync();
  for (int x = 0; x < X_DIM / 4; x++)
  {
    for (int y = 0; y < Y_DIM / 4; y++)
    {
      t_array[y][x].pixels.setBrightness(5);
    }
  }
  neotrellis_show();
  trellis.setBrightness(0);
  // light up all the LEDs in order
  for (uint8_t i = 0; i < numKeys; i++)
  {
    trellis.setLED(i);
    trellis.writeDisplay();
    delay(1);
  }
  // then turn them off
  for (uint8_t i = 0; i < numKeys; i++)
  {
    trellis.clrLED(i);
    trellis.writeDisplay();
    delay(1);
  }
  pixelTouchX = SEQ_GRID_LEFT;
  gridTouchY = 1;
  if (loadMidiFile("0.mid", myMidi[0]))
  {
    Serial.println("MIDI geladen!");
  }
  else
  {
    Serial.println("Fehler beim Laden");
  }
  if (fillNoteInfoAtTick(myMidi[0], 0) > 0)
  {
    Serial.print("Note: ");
    Serial.println(noteInfo[0][0]);
    Serial.print("Vel : ");
    Serial.println(noteInfo[0][1]);
    Serial.print("Start: ");
    Serial.println(noteInfo[0][2]);
    Serial.print("Len  : ");
    Serial.println(noteInfo[0][3]);
  }
  else
  {
    Serial.println("Keine Note gefunden.");
  }
}

void loop()
{
  if (!getArrangerFromPC)
  {
    unsigned long loopStartTime = millis();
    unsigned long trellisRestartMillis = millis();

    if (!digitalRead(NEOTRELLIS_INT_PIN) && !i2c_busy)
    {
      // i2c_busy = true;
      // Serial.println("reading trellis");
      neotrellis.read();
      neotrellis.setPixelColor(3, 1, TRELLIS_AQUA);
      neotrellis_show();
      //  i2c_busy = false;
    }
    neotrellis_set_control_buffer(3, 1, TRELLIS_BLACK);
    neotrellis.setPixelColor(3, 1, TRELLIS_BLACK);
    neotrellis_show();
    readEncoders();
    trellis_update();
    neotrellis_update();

    midi_read();
    touch_update();
    input_behaviour();
    draw_potRow();

    if (Serial.available())
    {
      inputString = Serial.readStringUntil('\n');
      inputString.trim(); // Entfernt \r, \n und Leerzeichen

      if (inputString == "send")
      {
        delay(5);
        Serial.println("Sende Tracks...");
        export_serial_Midi(allTracks[0]);
        export_serial_Midi(allTracks[1]);
        export_serial_Midi(allTracks[2]);
        export_serial_Midi(allTracks[3]);
        export_serial_Midi(allTracks[4]);
        export_serial_Midi(allTracks[5]);
        export_serial_Midi(allTracks[6]);
        export_serial_Midi(allTracks[7]);
        Serial.println("Fertig.");
      }
      else
      {
        Serial.print("Unbekannter Befehl: ");
        Serial.println(inputString);
      }

      inputString = ""; // Vorbereitung für nächsten Befehl
    }

    static int lastBar;
    if (lastBar != myClock.barTick)
    {
      for (int t = 0; t < NUM_TRACKS; t++)
      {
        for (int p = 0; p < NUM_PLUGINS; p++)
        {
          if (allTracks[t]->clip[allTracks[t]->clip_to_play[myClock.barTick]].midiChOut == p + NUM_MIDI_OUTPUTS)
            allPlugins[p]->presetNr = allTracks[t]->play_presetNr_Plugin_ccValue[myClock.barTick];
        }
      }
      lastBar = myClock.barTick;
    }

    for (int i = 0; i < NUM_TRACKS; i++)
    {
      allTracks[i]->update(pixelTouchX, gridTouchY);
      // trellis_show_clockbar(i, allTracks[i]->internal_clock / TICKS_PER_STEP);
      // if (allTracks[i]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1) >= 0)
      //  play_plugin_on_DAC(i, allTracks[i]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1));
    }
    get_infobox_background();
    unsigned long loopEndTime = millis();
    unsigned long neotrellisCurrentMillis = millis();

    // if we need to restart the trellisboard
    // if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    // Serial.println("shift pressed");
    if (neotrellisCurrentMillis - neotrellisReadPreviousMillis >= neotrellisReadInterval && !i2c_busy)
    {
      // Serial.printf("loop activeScrren:%d, trellisScreen: %D\n", activeScreen, trellisScreen);
      trellisReadPreviousMillis = neotrellisCurrentMillis;
      neotrellis_recall_control_buffer();
      neotrellis_show();

      // Serial.println(loopEndTime - loopStartTime);
      if (activeScreen == INPUT_FUNCTIONS_FOR_ARRANGER)
      {
        mouse(2, 14);
        moveCursor(pixelTouchX, gridTouchY, 1, TRACK_FRAME_H);
      }
      else
      {
        mouse(2, 14);
        moveCursor(pixelTouchX, gridTouchY, 1, STEP_FRAME_H);
      }
    }
    if (updateTFTScreen)
    {
      drawPositionCounter();
      tft_show();

      //  Serial.printf("active encoder page: %d\n", activeScreen);
      updateTFTScreen = false;
      enc_moved[0] = false;
      enc_moved[1] = false;
      enc_moved[2] = false;
      enc_moved[3] = false;
      trellisOut.writeDisplay();
    }

    // if (loopEndTime - loopStartTime > 1000 /*|| trellisCurrentMillis - trellisRestartPreviousMillis >= trellisRestartInterval*/)
    // {
    //   trellisRestartPreviousMillis = trellisRestartMillis;
    //   // trellis.trellisReset();
    //   Serial.printf("restart trellis @ %d\n", loopEndTime - loopStartTime);
    //   neotrellis.begin();
    //   neotrellis_setup(0);
    // }
  }
  {
    receiveClipData();
  }
}

void input_behaviour()
{
  switch (activeScreen)
  {
  case INPUT_FUNCTIONS_FOR_SEQUENCER:
  {
    trellis_setStepsequencer();
    neotrellis_SetCursor(14);
    if (neotrellisPressed[TRELLIS_BUTTON_ENTER] && !neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
      int tempTick = (pixelTouchX - SEQ_GRID_LEFT) / PIXEL_PER_TICK;
      int _note = (gridTouchY - 1) + (allTracks[active_track]->parameter[SET_OCTAVE] * NOTES_PER_OCTAVE);
      allTracks[active_track]->set_note_on_tick(tempTick, _note, allTracks[active_track]->parameter[SET_STEP_LENGTH]);
      neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;
    }
    if (neotrellisPressed[TRELLIS_BUTTON_ENTER] && neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
      allTracks[active_track]->clear_active_clip();

      neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;
      neotrellisPressed[TRELLIS_BUTTON_SHIFT] = false;
    }

    if (tsTouched)
    {
      tsTouched = false;
      // allTracks[active_track]->parameter[SET_STEP_LENGTH] = 1;
      int tempTick = (pixelTouchX - SEQ_GRID_LEFT) / PIXEL_PER_TICK;
      if (tempTick % allTracks[active_track]->parameter[SET_STEP_LENGTH] == 0)
      {
        int _note = (gridTouchY - 1) + (allTracks[active_track]->parameter[SET_OCTAVE] * NOTES_PER_OCTAVE);
        allTracks[active_track]->set_note_on_tick(tempTick, _note, allTracks[active_track]->parameter[SET_STEP_LENGTH]);
        updateTFTScreen = true;
        delay(70);
      }
    }
    if (neotrellisPressed[TRELLIS_POTROW])
    {
      change_plugin_row = true;
      draw_stepSequencer_parameters();
      Serial.printf("active screen: %d, activeTrack: %d\n", activeScreen, active_track);
      neotrellisPressed[TRELLIS_POTROW] = false;
    }
    allTracks[active_track]->set_stepSequencer_parameters();

    break;
  }
  case INPUT_FUNCTIONS_FOR_ARRANGER:
  {
    neotrellis_SetCursor(8);
    trellis_set_arranger();
    allTracks[gridTouchY - 1]->set_arranger_parameters();
    if (neotrellisPressed[TRELLIS_BUTTON_ENTER] && neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
      allTracks[active_track]->clear_arrangment();

      neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;
      neotrellisPressed[TRELLIS_BUTTON_SHIFT] = false;
    }
    draw_arranger_parameters();
    if (neotrellisPressed[TRELLIS_POTROW])
    {
      Serial.printf("active screen: %d, arrangerpage: %d\n", activeScreen, arrangerpage);
      change_plugin_row = true;

      // draw_arrangment_lines(gridTouchY - 1, arrangerpage);
      neotrellisPressed[TRELLIS_POTROW] = false;
    }

    if (lastPotRow == 3)
    {
      myClock.set_syncToExtern(0);
      myClock.set_tempo(1);
      myClock.set_start_of_loop(2); // Encoder: 2
      myClock.set_end_of_loop(3);   // Encoder: 3
    }
    if (pixelTouchX == 0 && enc_moved[3] == true)
    {
      int tempTrack = (gridTouchY)-1;
      Serial.printf("encmoved: %s, track: %d\n", enc_moved[3] ? "true" : "false", tempTrack);
      enc_moved[3] = false;

      trellisTrackColor[tempTrack] = trellisTrackColor[tempTrack] + encoded[3];
      trackColor[tempTrack] = rgb24to565(trellisTrackColor[tempTrack]);
      trellisOut.setActiveScreen(arrangerpage);
      tft_showTrackColors();
      neotrellis_set_control_buffer(3, tempTrack + 4, trellisTrackColor[tempTrack]);
      updateTFTScreen = true;
    }
    break;
  }
  case INPUT_FUNCTIONS_FOR_SEQUENCER_MODES:
  {
    if (neotrellisPressed[TRELLIS_POTROW])
    {
      tft.fillRect(18 * STEP_FRAME_W, 5 * STEP_FRAME_H, 20 * STEP_FRAME_W, 12 * STEP_FRAME_H, ILI9341_DARKGREY);
      neotrellisPressed[TRELLIS_POTROW] = false;
    }
    neotrellis_SetCursor(14);
    trellis_play_playmode();
    // trellis_setStepsequencer();
    allTracks[active_track]->set_seq_mode_parameters(lastPotRow);

    break;
  }
  case INPUT_FUNCTIONS_FOR_PLUGIN:
  {

    int trackChannel = allTracks[active_track]->clip[allTracks[active_track]->parameter[SET_CLIP2_EDIT]].midiChOut;
    if (trackChannel <= NUM_MIDI_OUTPUTS)
      allTracks[active_track]->set_MIDI_CC(lastPotRow);
    else if (trackChannel > NUM_MIDI_OUTPUTS)
      MasterOut.set_parameters(trackChannel - 49, lastPotRow);
    trellis_play_plugins();
    neotrellis_SetCursor(14);
    break;
  }
  case INPUT_FUNCTIONS_FOR_MIXER1:
  {
    trellis_play_mixer();
    set_mixer();
    break;
  }
  case INPUT_FUNCTIONS_FOR_MIXER2:
  {
    set_mixer_FX_page1();
    trellis_play_mixer();
    break;
  }
  case INPUT_FUNCTIONS_FOR_MIXER3:
  {
    set_mixer_FX_page2();
    trellis_play_mixer();
    break;
  }
  case INPUT_FUNCTIONS_FOR_FX1:
  {
    fx_1.PluginParameters(lastPotRow);
    trellis_play_mixer();
    break;
  }
  case INPUT_FUNCTIONS_FOR_FX2:
  {
    fx_2.PluginParameters(lastPotRow);
    trellis_play_mixer();
    break;
  }
  case INPUT_FUNCTIONS_FOR_FX3:
  {
    fx_3.PluginParameters(lastPotRow);
    trellis_play_mixer();
    break;
  }
  case INPUT_FUNCTIONS_FOR_PERFORM:
  {
    neotrellis_perform_set_active();
    trellis_perform();

    break;
  }
  case INPUT_FUNCTIONS_FOR_CLIPLAUNCHER:
  {
    trellis_play_clipLauncher();
    break;
  }
  case INPUT_FUNCTIONS_FOR_SGTL:
  {
    set_sgtlControls(lastPotRow);
    break;
  }
  default:
    break;
  }
}
// midi
void clock_to_notes(int _tick)
{ /*
 if (activeScreen==INPUT_FUNCTIONS_FOR_CLIPLAUNCHER){
   if (_tick==0)
   draw_clip_launcher();
 }*/
  // Serial.println(Masterclock.MIDItick);
  for (int t = 0; t < NUM_TRACKS; t++)
  {
    allTracks[t]->play_sequencer_mode(_tick, myClock.startOfLoop, myClock.endOfLoop);
  }
  // Serial.printf("LoopStart: %d, LoopEnd: %d\n", myClock.startOfLoop, myClock.endOfLoop);
}
void midi_setup(uint8_t dly)
{
  Serial.println("Initializing MIDI");
  tft.println("Initializing MIDI");
  // tft.update();
  // tft.updateScreenAsync();

  myusb.begin();
  usbMIDI.begin(); // Launch MIDI
  MIDI1.begin();

  MIDI1.setHandleNoteOn(myNoteOn);
  MIDI1.setHandleNoteOff(myNoteOff);
  MIDI1.setHandleControlChange(myControlChange);
  MIDI1.setHandleClock(myMIDIClock);
  MIDI1.setHandleStart(onExternalStart);
  MIDI1.setHandleStop(onExternalStop);
  MIDI1.turnThruOff();
  SerialMidiTimer.begin(SerialMidi_handleInput, 250);
  SerialMidiTimer.priority(80);

  usbMidi1.setHandleNoteOn(myNoteOn);
  usbMidi1.setHandleNoteOff(myNoteOff);
  usbMidi1.setHandleControlChange(myControlChange);
  usbMidi1.setHandleClock(myMIDIClock);
  usbDeviceTimer.begin(usbDevice_handleInput, 250);
  usbDeviceTimer.priority(80);

  usbMIDI.setHandleNoteOn(myNoteOn);
  usbMIDI.setHandleNoteOff(myNoteOff);
  usbMIDI.setHandleControlChange(myControlChange);
  usbMIDI.setHandleClock(myMIDIClock);
  usbMIDI.setHandleStart(onExternalStart);
  usbMIDI.setHandleStop(onExternalStop);

  delay(dly);
}
void midi_read()
{
  // usbMIDI.read();
  //  MIDI1.read();
  myusb.Task();
  usbMidi1.read();
  detect_USB_device();
}
void usbDevice_handleInput()
{
  while (usbMIDI.read())
  {
  }
}
void SerialMidi_handleInput()
{

  while (MIDI1.read())
  {
  }
  //  while (usbMidi1.read()) {
  //  }
}

void sendStart()
{

  MIDI1.sendStart();
  usbMIDI.sendRealTime(usbMIDI.Start);
  usbMidi1.sendRealTime(usbMIDI.Start);
}
void sendStop()
{
  MIDI1.sendStop();
  usbMIDI.sendRealTime(usbMIDI.Stop);
  usbMidi1.sendRealTime(usbMIDI.Stop);
}
void sendClock()
{
  MIDI1.sendClock();
  usbMIDI.sendRealTime(usbMIDI.Clock);
  usbMidi1.sendRealTime(usbMIDI.Clock);
}
void onExternalStart()
{
  clear_positionPointer();
  myClock.set_start();
}

void onExternalStop()
{

  myClock.set_stop();
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    allTracks[i]->internal_clock = -1;
    allTracks[i]->internal_clock_bar = -1;
    allTracks[i]->external_clock_bar = -1;
  }
  Serial.println("external MIDI Stop");
}
void sendNoteOn(uint8_t _track, uint8_t Note, uint8_t Velo, uint8_t Channel)
{
  if (Note < NO_NOTE)
  {
    if (Channel == 0)
      sendNoteOn_CV_Gate(_track, Note);
    if (Channel > 0 && Channel <= 16)
    {
      MIDI1.sendNoteOn(Note, Velo, Channel);
    }
    if (Channel > 16 && Channel <= 32)
      usbMIDI.sendNoteOn(Note, Velo, Channel - 16);
    if (Channel > 32 && Channel <= 48)
      usbMidi1.sendNoteOn(Note, Velo, Channel - 32);
    if (Channel > 48 && Channel <= 48 + NUM_PLUGINS)
      MasterOut.noteOn(Note, Velo, Channel - (NUM_MIDI_OUTPUTS + 1), Note % 12);
    // Serial.printf("Note ON: channel:%d, Note: %d, Velo: %d @tick: %d\n", Channel, Note, Velo, myClock.MIDITick);
  }
}
void sendNoteOff(uint8_t _track, uint8_t Note, uint8_t Velo, uint8_t Channel)
{
  if (Channel == 0)
    sendNoteOff_CV_Gate(_track, Note);
  if (Channel > 0 && Channel <= 16)
    MIDI1.sendNoteOff(Note, Velo, Channel);
  if (Channel > 16 && Channel <= 32)
    usbMIDI.sendNoteOff(Note, Velo, Channel - 16);
  if (Channel > 32 && Channel <= 48)
    usbMidi1.sendNoteOff(Note, Velo, Channel - 32);
  if (Channel > 48 && Channel <= 48 + NUM_PLUGINS)
    MasterOut.noteOff(Note, Velo, Channel - (48 + 1), Note % 12);
  // Serial.printf("Note Off: channel:%d, Note: %d, Velo: %d\n", Channel, Note, Velo);
}
void sendControlChange(uint8_t control, uint8_t value, uint8_t Channel, uint8_t fromTrack = 255)
{
  if (value < 128 && control < 128)
  {
    Serial.printf("send cc Channel: %d, Control: %d, value: %d\n", Channel, control, value);
    if (Channel > 0 && Channel <= 16)
      MIDI1.sendControlChange(control, value, Channel);
    if (Channel > 16 && Channel <= 32)
      usbMIDI.sendControlChange(control, value, Channel - 16);
    if (Channel > 32 && Channel <= 48)
      usbMidi1.sendControlChange(control, value, Channel - 32);
  }
  if (value < 128 && control == 128)
  {
    Serial.printf("send Progchange: %d, Control: %d, value: %d\n", Channel, control, value);
    if (Channel > 0 && Channel <= 16)
      MIDI1.sendProgramChange(value, Channel);
    if (Channel > 16 && Channel <= 32)
      usbMIDI.sendProgramChange(value, Channel - 16);
    if (Channel > 32 && Channel <= 48)
      usbMidi1.sendProgramChange(value, Channel - 32);
  }
  if (value < 128 && control > 131 && control < 147 && fromTrack < NUM_TRACKS)
  {
    Serial.printf("send Plugin Parameter:  Channel: %d, Control: %d, value: %d from Track: %d\n", Channel, control, value, fromTrack);
    int pluginChannel = allTracks[fromTrack]->clip[allTracks[fromTrack]->external_clock_bar].midiChOut - (NUM_MIDI_OUTPUTS + 1);
    allPlugins[pluginChannel]->set_Potentiometer(control - 131, value);
  }
  if (value < 128 && control >= 147 && fromTrack < NUM_TRACKS)
  {
    Serial.printf("send Seq Parameter:  Channel: %d, Control: %d, value: %d from Track: %d\n", Channel, control, value, fromTrack);
    int pluginChannel = allTracks[fromTrack]->clip[allTracks[fromTrack]->external_clock_bar].midiChOut - (NUM_MIDI_OUTPUTS + 1);
    allTracks[fromTrack]->set_seqModValue(control - 148, value);

    allPlugins[pluginChannel]->set_Potentiometer(control - 131, value);
  }
}

void myNoteOn(uint8_t channel, uint8_t note, uint8_t velocity)
{
  int trackChannel = allTracks[channel - 1]->clip[allTracks[channel - 1]->parameter[SET_CLIP2_EDIT]].midiChOut;
  if (channel < 9 && !allTracks[channel - 1]->muted && channel != trackChannel % 16)
  {
    Serial.printf("recieve note: %d, velo: %d, channel: %d\n ", note, velocity, channel);

    allTracks[channel - 1]->noteOn(note, velocity, trackChannel);
  }
  if (channel >= 9)
    sendNoteOn(channel - 1, note, velocity, channel);
  Serial.printf("note: %d, velo: %d, channel: %d\n ", note, velocity, channel);
}

void myNoteOff(uint8_t channel, uint8_t note, uint8_t velocity)
{
  if (channel < 9 && !allTracks[channel - 1]->muted)
  {
    int trackChannel = allTracks[channel - 1]->clip[allTracks[channel - 1]->parameter[SET_CLIP2_EDIT]].midiChOut;
    allTracks[channel - 1]->noteOff(note, velocity, trackChannel);
  }
  if (channel >= 9)
    sendNoteOff(channel - 1, note, velocity, channel);
}
void myControlChange(uint8_t channel, uint8_t control, uint8_t value)
{
  if (channel < 9)
  {

    for (int i = 0; i < NUM_PARAMETERS; i++)
    {

      if (control == i + 25)
      {
        allTracks[channel - 1]->parameter[i] = value;
        change_plugin_row = true;
        updateTFTScreen = true;
        break;
      }
      if (control == i + 80)
      {
        int trackChannel = allTracks[active_track]->clip[allTracks[active_track]->parameter[SET_CLIP2_EDIT]].midiChOut;
        int _pluginCh = trackChannel - (NUM_MIDI_OUTPUTS + 1);
        if (trackChannel > NUM_MIDI_OUTPUTS)
        {
          allPlugins[_pluginCh]->potentiometer[allPlugins[_pluginCh]->presetNr][i] = value;
          Serial.printf("MIDI pluginCh = %d\n", _pluginCh);
          change_plugin_row = true;
          updateTFTScreen = true;
          break;
        }
        allPlugins[_pluginCh]->change_preset();
      }
      // delay(10);
    }
  }
  if (channel >= 9)
    sendControlChange(control, value, channel, channel - 1);
}
void myUSBNoteOn(uint8_t channel, uint8_t note, uint8_t velocity)
{
  int trackChannel = allTracks[channel - 1]->clip[allTracks[channel - 1]->parameter[SET_CLIP2_EDIT]].midiChOut;
  if (trackChannel < 48 && trackChannel > NUM_MIDI_OUTPUTS)
    myNoteOn(channel, note, velocity);
}
void myUSBNoteOff(uint8_t channel, uint8_t note, uint8_t velocity)
{
  int trackChannel = allTracks[channel - 1]->clip[allTracks[channel - 1]->parameter[SET_CLIP2_EDIT]].midiChOut;
  if (trackChannel < 48 && trackChannel > NUM_MIDI_OUTPUTS)
    myNoteOff(channel, note, velocity);
}

void myMIDIClock()
{
  uClock.clockMe();
}
void detect_USB_device()
{
  static bool deviceConnected = false;
  // if connected
  if (usbMidi1.idProduct() != 0 && !deviceConnected)
  {
    set_infobox_background(750);

    deviceConnected = true;
    tft.print("USB Device connected: ");
    set_infobox_next_line(1);
    tft.printf("%s: %s", usbMidi1.manufacturer(), usbMidi1.product());
  }
  // if disconnected
  else if (usbMidi1.idProduct() == 0 && deviceConnected)
  {
    set_infobox_background(750);
    tft.println("USB Device disconnected");
    deviceConnected = false;

    // remove infobox
  }

  reset_infobox_background();
}

void play_plugin_on_DAC(uint8_t _track, uint8_t _pluginNr)
{
  /*
    if (_track == 0)
    {
      // uint8_t _cvNoteValue=4095 / 64 * allTracks[0]->cvNote;
      mcp.setChannelValue(MCP4728_CHANNEL_A, 4095 * allPlugins[_pluginNr]->dacOut.read());

      if (4095 * allPlugins[_pluginNr]->dacOut.read() == 0)
        Serial.printf("DAC OUT: %d\n", 4095 * allPlugins[_pluginNr]->dacOut.read());
    }

    if (_track == 1)
    {
      mcp.setChannelValue(MCP4728_CHANNEL_B, 4095 * allPlugins[_pluginNr]->dacOut.read());
    }

    if (_track == 2)
    {
      mcp.setChannelValue(MCP4728_CHANNEL_C, 4095 * allPlugins[_pluginNr]->dacOut.read());
    }

    if (_track == 3)
    {
      mcp.setChannelValue(MCP4728_CHANNEL_D, 4095 * allPlugins[_pluginNr]->dacOut.read());
    }
    if (_track == 4)
    {
      mcp2.setChannelValue(MCP4728_CHANNEL_A, 4095 * allPlugins[_pluginNr]->dacOut.read());
    }
    if (_track == 5)
    {
      mcp2.setChannelValue(MCP4728_CHANNEL_B, 4095 * allPlugins[_pluginNr]->dacOut.read());
    }
    if (_track == 6)
    {
      mcp2.setChannelValue(MCP4728_CHANNEL_C, 4095 * allPlugins[_pluginNr]->dacOut.read());
    }
    if (_track == 7)
    {
      mcp2.setChannelValue(MCP4728_CHANNEL_D, 4095 * allPlugins[_pluginNr]->dacOut.read());
    }
    */
}

// some trellis
void neo_trellis_select_mixer()
{
  if (neotrellisPressed[TRELLIS_BUTTON_MIXER])
  {
    trellisOut.drawSelectMixerPage();
    if (isPressed())
    {

      trellisOut.clearMainGridNow();
      clearWorkSpace();
      if (getPressedKey() == 0)
      {
        trellisOut.setActiveScreen(TRELLIS_SCREEN_MIXER1);
        trellisOut.recall_main_buffer(TRELLIS_SCREEN_MIXER1);
        trellisOut.writeDisplay();
        activeScreen = INPUT_FUNCTIONS_FOR_MIXER1;
        draw_mixer();
        show_active_page_info("Mixer", 0);
      }
      if (getPressedKey() == 1)
      {
        show_trellisFX_mixerPage();
        trellisOut.setActiveScreen(TRELLIS_SCREEN_MIXER);
        activeScreen = INPUT_FUNCTIONS_FOR_MIXER2;
        draw_mixer_FX_page1();
        show_active_page_info("FX Vol", 1);
      }
      if (getPressedKey() == 2)
      {
        show_trellisFX_mixerPage();
        trellisOut.setActiveScreen(TRELLIS_SCREEN_MIXER);
        activeScreen = INPUT_FUNCTIONS_FOR_MIXER3;
        draw_mixer_FX_page2();
        show_active_page_info("FX Vol", 2);
      }
      if (getPressedKey() == 3)
      {
        show_trellisFX_mixerPage();
        trellisOut.setActiveScreen(TRELLIS_SCREEN_MIXER);
        activeScreen = INPUT_FUNCTIONS_FOR_FX1;
        fx_1.draw_plugin();
        show_active_page_info("FX Ctrl", 1);
      }
      if (getPressedKey() == 4)
      {
        show_trellisFX_mixerPage();
        trellisOut.setActiveScreen(TRELLIS_SCREEN_MIXER);
        activeScreen = INPUT_FUNCTIONS_FOR_FX2;
        fx_2.draw_plugin();
        show_active_page_info("FX Ctrl", 2);
      }
      if (getPressedKey() == 5)
      {
        show_trellisFX_mixerPage();
        trellisOut.setActiveScreen(TRELLIS_SCREEN_MIXER);
        activeScreen = INPUT_FUNCTIONS_FOR_FX3;
        fx_3.draw_plugin();
        show_active_page_info("FX Ctrl", 3);
      }
      if (getPressedKey() == 6)
      {
        trellisOut.setActiveScreen(TRELLIS_SCREEN_PERFORM);
        activeScreen = INPUT_FUNCTIONS_FOR_PERFORM;

        set_perform_page(lastPotRow);
        show_active_page_info("Perform", 0);
      }
      if (getPressedKey() == 7)
      {
        trellisOut.setActiveScreen(TRELLIS_SCREEN_CLIPLAUNCHER);
        activeScreen = INPUT_FUNCTIONS_FOR_CLIPLAUNCHER;
        draw_clip_launcher();
        show_active_page_info("Launch", 0);
      }
      if (getPressedKey() == 15)
      {
        activeScreen = INPUT_FUNCTIONS_FOR_SGTL;
        draw_sgtlControls();
        show_active_page_info("SGTL", 0);
      }
      change_plugin_row = true;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      revertPressedKey();
    }
  }
}
void trellis_play_mixer()
{
  if (trellisOut.getActiveScreen() == TRELLIS_SCREEN_MIXER1)
  {

    uint8_t _gain[NUM_STEPS] = {0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 111, 119, 127};
    if (isPressed())
    {
      int t = getPressedKey() / NUM_STEPS;
      int s = getPressedKey() % NUM_STEPS;
      int trackChannel = allTracks[t]->clip[allTracks[t]->clip_to_play[allTracks[t]->internal_clock_bar]].midiChOut;

      allTracks[t]->mixGainPot = _gain[s];
      if (trackChannel > NUM_MIDI_OUTPUTS)
        allPlugins[trackChannel - (NUM_MIDI_OUTPUTS + 1)]->set_gain(_gain[s]);
      for (int i = 0; i < NUM_STEPS; i++)
        trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER1, i, t, TRELLIS_BLACK);
      trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER1, allTracks[t]->mixGainPot / 8, t, trackColor[t]);
      Serial.printf("trellis play mixer track: %d, gain: %d, trackchannel: %d\n", t, _gain[s], trackChannel);
      draw_infobox("Track: ", t, "Main Vol =  ", _gain[s]);
      revertPressedKey();
      trellisOut.writeDisplay();
    }
  }
  if (trellisOut.getActiveScreen() == TRELLIS_SCREEN_MIXER)
  {
    float _gain[4] = {0, 0.30, 0.60, 1};

    if (isPressed())
    {
      int t = getPressedKey() / NUM_STEPS;
      int s = getPressedKey() % NUM_STEPS;
      int trackChannel = allTracks[t]->clip[allTracks[t]->clip_to_play[allTracks[t]->internal_clock_bar]].midiChOut;
      for (int c = 0; c < 4; c++)
      {
        if (getPressedKey() % TRELLIS_PADS_X_DIM == c)
        {
          Serial.printf("dry channel = %d, track channel : %d\n", trackChannel - (NUM_MIDI_OUTPUTS + 1), trackChannel);
          MasterOut.fx_section.dry[trackChannel - (NUM_MIDI_OUTPUTS + 1)].gain(_gain[c]);
          allTracks[t]->mixDryPot = (c * 42);
          for (int i = 0; i < 4; i++)
            trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER, i, t, TRELLIS_BLACK);
          trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER, c, t, TRELLIS_PINK);
          draw_infobox("Track: ", t, "Dry Vol =  ", (c * 42));
        }
        if (getPressedKey() % TRELLIS_PADS_X_DIM == c + 4)
        {
          Serial.printf("fx1 channel = %d, track channel : %d\n", trackChannel - (NUM_MIDI_OUTPUTS + 1), trackChannel);
          fx_1.pl[trackChannel - (NUM_MIDI_OUTPUTS + 1)].gain(_gain[c]);
          allTracks[t]->mixFX1Pot = (c * 42);
          for (int i = 0; i < 4; i++)
            trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER, i + 4, t, TRELLIS_BLACK);
          trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER, c + 4, t, TRELLIS_OLIVE);
          draw_infobox("Track: ", t, "FX1 Vol =  ", (c * 42));
        }
        if (getPressedKey() % TRELLIS_PADS_X_DIM == c + 8)
        {
          Serial.printf("fx2 channel = %d, track channel : %d\n", trackChannel - (NUM_MIDI_OUTPUTS + 1), trackChannel);
          fx_2.pl[trackChannel - (NUM_MIDI_OUTPUTS + 1)].gain(_gain[c]);
          allTracks[t]->mixFX2Pot = (c * 42);
          for (int i = 0; i < 4; i++)
            trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER, i + 8, t, TRELLIS_BLACK);
          trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER, c + 8, t, TRELLIS_AQUA);
          draw_infobox("Track: ", t, "FX2 Vol =  ", (c * 42));
        }
        if (getPressedKey() % TRELLIS_PADS_X_DIM == c + 12)
        {
          Serial.printf("fx3 channel = %d, track channel : %d\n", trackChannel - (NUM_MIDI_OUTPUTS + 1), trackChannel);
          fx_3.pl[trackChannel - (NUM_MIDI_OUTPUTS + 1)].gain(_gain[c]);
          allTracks[t]->mixFX3Pot = (c * 42);
          for (int i = 0; i < 4; i++)
            trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER, i + 12, t, TRELLIS_BLACK);
          trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER, c + 12, t, TRELLIS_ORANGE);
          draw_infobox("Track: ", t, "FX3 Vol =  ", (c * 42));
        }
        // change_plugin_row=true;
      }
      revertPressedKey();
      trellisOut.writeDisplay();
    }
  }
}
// Hilfsfunktionen
void sendCCToActiveTracks(uint8_t cc, uint8_t val)
{
  for (int s = 0; s < NUM_TRACKS; s++)
  {
    if (!allTracks[s]->performIsActive)
      continue;
    uint8_t ch = allTracks[s]->clip[allTracks[s]->clip_to_play[allTracks[s]->internal_clock_bar]].midiChOut;
    if (ch <= NUM_MIDI_OUTPUTS)
      sendControlChange(cc, val, ch, s);
  }
}

void updateFxVolume(uint8_t val, float gainVal, uint8_t cc, int fxNr)
{
  for (int s = 0; s < NUM_TRACKS; s++)
  {
    if (!allTracks[s]->performIsActive)
      continue;
    uint8_t ch = allTracks[s]->clip[allTracks[s]->clip_to_play[allTracks[s]->external_clock_bar]].midiChOut;
    if (ch > NUM_MIDI_OUTPUTS)
      MasterOut.setFXGain(fxNr, ch - (NUM_MIDI_OUTPUTS + 1), gainVal);
    else
      sendControlChange(cc, val, ch, s);
  }
}
void performPluginFreq(uint8_t val, uint8_t cc)
{
  for (int s = 0; s < NUM_TRACKS; s++)
  {
    if (!allTracks[s]->performIsActive)
      continue;
    uint8_t ch = allTracks[s]->clip[allTracks[s]->clip_to_play[allTracks[s]->external_clock_bar]].midiChOut;
    if (ch > NUM_MIDI_OUTPUTS)
      MasterOut.performFrequency(ch - (NUM_MIDI_OUTPUTS + 1), val);
    else
      sendControlChange(cc, val, ch, s);
  }
}
void performpluginReso(uint8_t val, uint8_t cc)
{
  for (int s = 0; s < NUM_TRACKS; s++)
  {
    if (!allTracks[s]->performIsActive)
      continue;
    uint8_t ch = allTracks[s]->clip[allTracks[s]->clip_to_play[allTracks[s]->external_clock_bar]].midiChOut;
    if (ch > NUM_MIDI_OUTPUTS)
      MasterOut.performResonance(ch - (NUM_MIDI_OUTPUTS + 1), val);
    else
      sendControlChange(cc, val, ch, s);
  }
}
void printInfo(const char *label, int val, uint8_t cc)
{
  set_infobox_background(750);
  tft.printf("%s = %d ", label, val);
  set_infobox_next_line(1);
  tft.printf("send CC%d = %d ", cc, val);
  reset_infobox_background();
}

// Hauptfunktion für Perform-Modus
void trellis_perform()
{
  if (trellisOut.getActiveScreen() != TRELLIS_SCREEN_PERFORM)
    return;
  draw_perform_page();

  if (isPressed())
  {

    int key = getPressedKey();
    revertPressedKey();
    int t = key / TRELLIS_PADS_X_DIM;
    int col = key % TRELLIS_PADS_X_DIM;
    int row = key / TRELLIS_PADS_X_DIM;
    int value = eigthTo127[t];
    trellisPerformIndex[col] = row;
    trellisOut.drawPerform(lastPotRow);
    switch (col)
    {
    case 0:
      for (int s = 0; s < NUM_TRACKS; s++)
        if (allTracks[s]->performIsActive)
          allTracks[s]->mixGainPot = value;
      printInfo("Master Vol", value, performCC[0]);
      break;

    case 1:
      updateFxVolume(value, row / 8.0f, performCC[1], 0);
      printInfo("Dry Vol", value, performCC[1]);
      break;

    case 2:
      updateFxVolume(value, row / 8.0f, performCC[2], 1);
      printInfo("FX1 Vol", value, performCC[2]);
      break;

    case 3:
      updateFxVolume(value, row / 8.0f, performCC[3], 2);
      printInfo("FX2 Vol", value, performCC[3]);
      break;

    case 4:
      updateFxVolume(value, row / 8.0f, performCC[4], 3);
      printInfo("FX3 Vol", value, performCC[4]);
      break;

    case 5:
      fx_1.potentiometer[fx_1.presetNr][0] = value;
      fx_1.freeverb.roomsize(map(row, 0, 8, 0.0, 1.0));
      sendCCToActiveTracks(performCC[5], value);
      printInfo("FX1 Roomsize", value, performCC[5]);
      break;

    case 6:
      fx_1.potentiometer[fx_1.presetNr][1] = value;
      fx_1.freeverb.damping(map(row, 0, 8, 0.0, 1.0));
      sendCCToActiveTracks(performCC[6], value);
      printInfo("FX1 Damping", value, performCC[6]);
      break;

    case 7:
      fx_2.potentiometer[fx_2.presetNr][0] = value;
      fx_2.bitcrusher.bits(map(row, 0, 8, 16, 0));
      sendCCToActiveTracks(performCC[7], value);
      printInfo("FX2 BitDepth", map(row, 0, 8, 16, 0), performCC[7]);
      break;

    case 8:
    {
      int _rate[] = {44100, 22050, 11025, 5512, 2756, 1378, 1022, 689};
      fx_2.potentiometer[fx_2.presetNr][1] = value;
      fx_2.bitcrusher.sampleRate(_rate[row]);
      sendCCToActiveTracks(performCC[8], value);
      printInfo("FX2 SmplRate", _rate[row], performCC[8]);
      break;
    }

    case 9:
      fx_3.potentiometer[fx_3.presetNr][0] = value;
      fx_3.delay.delay(0, 500 / (row + 1));
      sendCCToActiveTracks(performCC[9], value);
      printInfo("FX3 DlyTime", 500 / (row + 1), performCC[9]);
      break;

    case 10:
      fx_3.potentiometer[fx_3.presetNr][1] = value;
      fx_3.delayMixer.gain(1, row / 8.0f);
      sendCCToActiveTracks(performCC[10], value);
      printInfo("FX3 Feedback", value, performCC[10]);
      break;

    case 11:
    {
      int freq = note_frequency[value] * tuning;
      performPluginFreq(value, performCC[11]);
      sendCCToActiveTracks(performCC[11], value);
      printInfo("Fltr Freq", freq, performCC[11]);
      break;
    }

    case 12:
      performpluginReso(value, performCC[11]);
      sendCCToActiveTracks(performCC[12], value);
      printInfo("Fltr Reso", row * 18, performCC[12]);
      break;

    case 13:
    {
      float _div[] = {1, 1.3333f, 1.6f, 2, 2.6666f, 4, 8, 16};
      for (int s = 0; s < NUM_TRACKS; s++)
      {
        if (!allTracks[s]->performIsActive)
          continue;
        allTracks[s]->performSeqLengthDiv = _div[row];
        if (row == 0)
        {
          allTracks[s]->internal_clock = 0;
          allTracks[s]->internal_clock_bar = myClock.barTick;
          allTracks[s]->external_clock_bar = myClock.barTick;
        }
      }
      sendCCToActiveTracks(performCC[13], value);
      printInfo("Clip Length", (int)(MAX_TICKS / _div[row]), performCC[13]);
      break;
    }

    case 14:
    {
      uint8_t _div[] = {0, 1, 2, 3, 4, 6, 8, 16};
      for (int s = 0; s < NUM_TRACKS; s++)
      {
        if (!allTracks[s]->performIsActive)
          continue;
        allTracks[s]->performClockDivision = _div[row];
        if (row == 0)
        {
          allTracks[s]->internal_clock = 0;
          allTracks[s]->internal_clock_bar = myClock.barTick;
          allTracks[s]->external_clock_bar = myClock.barTick;
        }
      }
      sendCCToActiveTracks(performCC[14], value);
      printInfo("Step Div", _div[row], performCC[14]);
      break;
    }

    case 15:
    {
      int8_t _offset[] = {0, -12, -4, -2, 2, 4, 6, 12};
      for (int s = 0; s < NUM_TRACKS; s++)
        if (allTracks[s]->performIsActive)
          allTracks[s]->performNoteOffset = _offset[row];
      sendCCToActiveTracks(performCC[15], value);
      printInfo("Note Transpose", _offset[row], performCC[15]);
      break;
    }
    }
  }

  set_perform_page(lastPotRow);
}
void trellis_play_plugins()
{
  if (trellisOut.getActiveScreen() != TRELLIS_SCREEN_PLUGIN)
    return;
  int trackChannel = allTracks[active_track]->clip[allTracks[active_track]->parameter[SET_CLIP2_EDIT]].midiChOut;
  if (trackChannel <= NUM_MIDI_OUTPUTS)
    return;
  if (isPressed())
  {
    int pot = getPressedKey() / (NUM_STEPS * 2) + (lastPotRow * NUM_ENCODERS);
    int pluginChannel = trackChannel - (NUM_MIDI_OUTPUTS + 1);
    int value = (getPressedKey() % (NUM_STEPS * 2)) * 4.12f;
    allPlugins[pluginChannel]->set_Potentiometer(pot, value);
    revertPressedKey();
  }
}
void neotrellis_show_plugin()
{
  if (neotrellisPressed[TRELLIS_BUTTON_PLUGIN])
  {
    for (int i = 0; i < NUM_TRACKS; i++)
    {
      if (neotrellisPressed[3 + ((i + 4) * X_DIM)])
      {
        neotrellisPressed[3 + ((i + 4) * X_DIM)] = false;
        active_track = i;
        clearWorkSpace();
        show_active_page_info("Track", i + 1);

        int trackChannel = allTracks[active_track]->clip[allTracks[active_track]->parameter[SET_CLIP2_EDIT]].midiChOut;
        if (trackChannel <= NUM_MIDI_OUTPUTS)
          draw_MIDI_CC_screen();
        else
          allPlugins[trackChannel - (NUM_MIDI_OUTPUTS + 1)]->draw_plugin();
        activeScreen = INPUT_FUNCTIONS_FOR_PLUGIN;
        trellisOut.setActiveScreen(TRELLIS_SCREEN_PLUGIN);
        trellisOut.drawPlugin();
        neotrellis_set_control_buffer(2, 3, trellisTrackColor[active_track]);
        break;
      }
    }
  }
}

// Mixer
void set_mixer()
{
  draw_mixer();
  switch (lastPotRow)
  {
  case 0:
  {
    set_mixer_gain(0, 0, "Tr D", 0);
    set_mixer_gain(1, 0, "Tr 2", 1);
    set_mixer_gain(2, 0, "Tr 3", 2);
    set_mixer_gain(3, 0, "Tr 4", 3);
  }
  break;
  case 1:
  {
    set_mixer_gain(0, 0, "Tr D", 0);
    set_mixer_gain(1, 0, "Tr 2", 1);
    set_mixer_gain(2, 0, "Tr 3", 2);
    set_mixer_gain(3, 0, "Tr 4", 3);
  }
  break;
  case 2:
  {
    set_mixer_gain(0, 2, "Tr 5", 4);
    set_mixer_gain(1, 2, "Tr 6", 5);
    set_mixer_gain(2, 2, "Tr 7", 6);
    set_mixer_gain(3, 2, "Tr 8", 7);
  }
  break;
  case 3:
  {
    set_mixer_gain(0, 2, "Tr 5", 4);
    set_mixer_gain(1, 2, "Tr 6", 5);
    set_mixer_gain(2, 2, "Tr 7", 6);
    set_mixer_gain(3, 2, "Tr 8", 7);
  }
  break;
  default:
    break;
  }
}
void set_mixer_gain(uint8_t XPos, uint8_t YPos, const char *name, uint8_t trackn)
{

  if (enc_moved[XPos])
  {
    int trackChannel = allTracks[trackn]->clip[allTracks[trackn]->clip_to_play[allTracks[trackn]->internal_clock_bar]].midiChOut;
    allTracks[trackn]->mixGainPot = constrain(allTracks[trackn]->mixGainPot + encoded[XPos], 0, MIDI_CC_RANGE);
    allTracks[trackn]->mixGain = (float)(allTracks[trackn]->mixGainPot / MIDI_CC_RANGE_FLOAT);
    Serial.printf("set mixgainpot: %d for track %d, trackCH: %d\n", allTracks[trackn]->mixGainPot, trackn, trackChannel);
    if (trackChannel > NUM_MIDI_OUTPUTS)
      allPlugins[trackChannel - (NUM_MIDI_OUTPUTS + 1)]->set_gain(allTracks[trackn]->mixGainPot);
    drawPot(XPos, YPos, allTracks[trackn]->mixGainPot, name);
  }
}

void set_mixer_FX_page1()
{
  draw_mixer_FX_page1();

  if (lastPotRow == 0)
  {

    set_mixer_dry(0, 0, "Dry D", 0);
    set_mixer_FX1(1, 0, "FX1 D", 0);
    set_mixer_FX2(2, 0, "FX2 D", 0);
    set_mixer_FX3(3, 0, "FX3 D", 0);
  }

  if (lastPotRow == 1)
  {
    set_mixer_dry(0, 1, "Dry 2", 1);
    set_mixer_FX1(1, 1, "FX1 2", 1);
    set_mixer_FX2(2, 1, "FX2 2", 1);
    set_mixer_FX3(3, 1, "FX3 2", 1);
  }

  if (lastPotRow == 2)
  {
    set_mixer_dry(0, 2, "Dry 3", 2);
    set_mixer_FX1(1, 2, "FX1 3", 2);
    set_mixer_FX2(2, 2, "FX2 3", 2);
    set_mixer_FX3(3, 2, "FX3 3", 2);
  }

  if (lastPotRow == 3)
  {
    set_mixer_dry(0, 3, "Dry 4", 3);
    set_mixer_FX1(1, 3, "FX1 4", 3);
    set_mixer_FX2(2, 3, "FX2 4", 3);
    set_mixer_FX3(3, 3, "FX3 4", 3);
  }
}
void set_mixer_FX_page2()
{
  draw_mixer_FX_page1();
  if (lastPotRow == 0)
  {

    set_mixer_dry(0, 0, "Dry 5", 4);
    set_mixer_FX1(1, 0, "FX1 5", 4);
    set_mixer_FX2(2, 0, "FX2 5", 4);
    set_mixer_FX3(3, 0, "FX3 5", 4);
  }

  if (lastPotRow == 1)
  {
    set_mixer_dry(0, 1, "Dry 6", 5);
    set_mixer_FX1(1, 1, "FX1 6", 5);
    set_mixer_FX2(2, 1, "FX2 6", 5);
    set_mixer_FX3(3, 1, "FX3 6", 5);
  }

  if (lastPotRow == 2)
  {
    set_mixer_dry(0, 2, "Dry 7", 6);
    set_mixer_FX1(1, 2, "FX1 7", 6);
    set_mixer_FX2(2, 2, "FX2 7", 6);
    set_mixer_FX3(3, 2, "FX3 7", 6);
  }

  if (lastPotRow == 3)
  {
    set_mixer_dry(0, 3, "Dry 8", 7);
    set_mixer_FX1(1, 3, "FX1 8", 7);
    set_mixer_FX2(2, 3, "FX2 8", 7);
    set_mixer_FX3(3, 3, "FX3 8", 7);
  }
}
void set_mixer_dry(uint8_t XPos, uint8_t YPos, const char *name, uint8_t trackn)
{
  if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
  {
    if (enc_moved[XPos])
    {
      int trackChannel = allTracks[trackn]->clip[allTracks[trackn]->clip_to_play[allTracks[trackn]->internal_clock_bar]].midiChOut;
      allTracks[trackn]->mixDryPot = constrain(allTracks[trackn]->mixDryPot + encoded[XPos], 0, MIDI_CC_RANGE);
      allTracks[trackn]->mixDry = (float)(allTracks[trackn]->mixDryPot / MIDI_CC_RANGE_FLOAT);
      for (int i = 0; i < NUM_PLUGINS; i++)
      {
        if (trackChannel == CH_PLUGIN_1 + i)
          MasterOut.fx_section.dry[i].gain(allTracks[trackn]->mixDry);
      }
      drawPot(XPos, YPos, allTracks[trackn]->mixDryPot, name);
    }
  }
}
void set_mixer_FX1(uint8_t XPos, uint8_t YPos, const char *name, uint8_t trackn)
{
  if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
  {
    if (enc_moved[XPos])
    {
      int trackChannel = allTracks[trackn]->clip[allTracks[trackn]->clip_to_play[allTracks[trackn]->internal_clock_bar]].midiChOut;
      allTracks[trackn]->mixFX1Pot = constrain(allTracks[trackn]->mixFX1Pot + encoded[XPos], 0, MIDI_CC_RANGE);
      allTracks[trackn]->mixFX1 = (float)(allTracks[trackn]->mixFX1Pot / MIDI_CC_RANGE_FLOAT);
      for (int i = 0; i < NUM_PLUGINS; i++)
      {
        if (trackChannel == CH_PLUGIN_1 + i)
          fx_1.pl[i].gain(allTracks[trackn]->mixFX1);
      }

      drawPot(XPos, YPos, allTracks[trackn]->mixFX1Pot, name);
    }
  }
}
void set_mixer_FX2(uint8_t XPos, uint8_t YPos, const char *name, uint8_t trackn)
{
  if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
  {
    if (enc_moved[XPos])
    {
      int trackChannel = allTracks[trackn]->clip[allTracks[trackn]->clip_to_play[allTracks[trackn]->internal_clock_bar]].midiChOut;
      allTracks[trackn]->mixFX2Pot = constrain(allTracks[trackn]->mixFX2Pot + encoded[XPos], 0, MIDI_CC_RANGE);
      allTracks[trackn]->mixFX2 = (float)(allTracks[trackn]->mixFX2Pot / MIDI_CC_RANGE_FLOAT);
      for (int i = 0; i < NUM_PLUGINS; i++)
      {
        if (trackChannel == CH_PLUGIN_1 + i)
          fx_2.pl[i].gain(allTracks[trackn]->mixFX2);
      }
      drawPot(XPos, YPos, allTracks[trackn]->mixFX2Pot, name);
    }
  }
}
void set_mixer_FX3(uint8_t XPos, uint8_t YPos, const char *name, uint8_t trackn)
{

  if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
  {
    if (enc_moved[XPos])
    {
      int trackChannel = allTracks[trackn]->clip[allTracks[trackn]->clip_to_play[allTracks[trackn]->internal_clock_bar]].midiChOut;
      allTracks[trackn]->mixFX3Pot = constrain(allTracks[trackn]->mixFX3Pot + encoded[XPos], 0, MIDI_CC_RANGE);
      allTracks[trackn]->mixFX3 = (float)(allTracks[trackn]->mixFX3Pot / MIDI_CC_RANGE_FLOAT);
      for (int i = 0; i < NUM_PLUGINS; i++)
      {
        if (trackChannel == CH_PLUGIN_1 + i)
          fx_3.pl[i].gain(allTracks[trackn]->mixFX3);
      }
      drawPot(XPos, YPos, allTracks[trackn]->mixFX3Pot, name);
    }
  }
}

void show_trellisFX_mixerPage()
{
  for (int t = 0; t < NUM_TRACKS; t++)
  {
    // Alle Felder auf schwarz setzen
    for (int i = 0; i < 16; i++)
      trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER, i, t, TRELLIS_BLACK);

    // Aktuelle Werte einfärben
    trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER, allTracks[t]->mixDryPot / 42, t, TRELLIS_PINK);
    trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER, allTracks[t]->mixFX1Pot / 42 + 4, t, TRELLIS_OLIVE);
    trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER, allTracks[t]->mixFX2Pot / 42 + 8, t, TRELLIS_AQUA);
    trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER, allTracks[t]->mixFX3Pot / 42 + 12, t, TRELLIS_ORANGE);
  }
}

void set_sgtlControls(uint8_t row)
{
  draw_sgtlControls();
  if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
  {
    if (row == 0)
    {

      set_sgtlcontrol(0, 0, "mxGain");
      set_sgtlcontrol(1, 0, "Response");
      set_sgtlcontrol(2, 0, "Soft/Hard");
      set_sgtlcontrol(3, 0, "Threshold");
    }

    if (row == 1)
    {

      set_sgtlcontrol(0, 1, "Attack");
      set_sgtlcontrol(1, 1, "Decay");
      set_sgtlcontrol(2, 1, "Comp O/I");
      set_sgtlcontrol(3, 1, "AP O/I");
    }

    if (row == 2)
    {
      set_sgtlcontrol(0, 2, "Width");
      set_sgtlcontrol(1, 2, "Stereo O/I");
      set_sgtlcontrol(2, 2, "Bass O/I");
      set_sgtlcontrol(3, 2, "In Lvl");
    }

    if (row == 3)
    {
      set_sgtlcontrol(0, 3, "Bass Lvl");
      set_sgtlcontrol(1, 3, "Hipass");
      set_sgtlcontrol(2, 3, "Cutoff");
      set_sgtlcontrol(3, 3, "OutVol");
    }
  }
  if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
  {
  }
}
void draw_sgtlControls()
{
  if (change_plugin_row)
  {
    change_plugin_row = false;
    // Serial.println("drawing sgtlcontrol");
    drawPot(0, 0, sgtlparameter[0], "mxGain");
    drawPot(1, 0, sgtlparameter[1], "Response");
    drawPot(2, 0, sgtlparameter[2], "Soft/Hard");
    drawPot(3, 0, sgtlparameter[3], "Threshold");

    drawPot(0, 1, sgtlparameter[4], "Attack");
    drawPot(1, 1, sgtlparameter[5], "Decay");
    drawPot(2, 1, sgtlparameter[6], "Comp O/I");
    drawPot(3, 1, sgtlparameter[7], "AP O/I");

    drawPot(0, 2, sgtlparameter[8], "Width");
    drawPot(1, 2, sgtlparameter[9], "Stereo O/I");
    drawPot(2, 2, sgtlparameter[10], "Bass O/I");
    drawPot(3, 2, sgtlparameter[11], "In Lvl");

    drawPot(0, 3, sgtlparameter[12], "Bass Lvl");
    drawPot(1, 3, sgtlparameter[13], "Hipass I/O");
    drawPot(2, 3, sgtlparameter[14], "Cutoff");
    drawPot(3, 3, sgtlparameter[15], "OutVol");
  }
}
void set_sgtlcontrol(uint8_t _xPos, uint8_t _yPos, const char *name)
{
  int index = _xPos + (NUM_ENCODERS * _yPos);
  if (enc_moved[_xPos])
  {
    enc_moved[_xPos] = false;
    if (index <= 5)
    {
      static uint8_t maxGain;
      static uint8_t lbiResponse;
      static uint8_t hardlimit;
      static float threshold;
      static float attack;
      static float decay;
      switch (index)
      {
      case 0:
        maxGain = get_sgtl_potentiometer(0, 0, 2, "mxGain");
        break;
      case 1:
        lbiResponse = get_sgtl_potentiometer(1, 0, 8, "Response");
        break;
      case 2:
        hardlimit = get_sgtl_potentiometer(2, 0, 1, "Soft/Hard");
        break;
      case 3:
        threshold = get_sgtl_potentiometer(3, 0, 96, "Threshold");
        break;
      case 4:
        attack = get_sgtl_potentiometer(4, 0, 127, "Attack") / 127;
        break;
      case 5:
        decay = get_sgtl_potentiometer(5, 0, 127, "Decay") / 127;
        break;

      default:
        break;
      }
      MasterOut.sgtl5000.autoVolumeControl(maxGain, lbiResponse, hardlimit, threshold, attack, decay);
    }

    else if (index == 6)
    {
      if (get_sgtl_potentiometer(6, 0, 1, "Comp O/I") == 0)
      {
        MasterOut.sgtl5000.autoVolumeDisable();
      }
      else
        MasterOut.sgtl5000.autoVolumeEnable();
    }
    else if (index == 7)
    {
      if (get_sgtl_potentiometer(7, 0, 1, "AP O/I") == 0)
      {
        MasterOut.sgtl5000.audioProcessorDisable();
      }
      else
        MasterOut.sgtl5000.audioPostProcessorEnable();
    }
    else if (index == 8)
    {
      uint8_t width = get_sgtl_potentiometer(8, 0, 7, "Width");
      MasterOut.sgtl5000.surroundSound(width, 2);
    }
    else if (index == 9)
    {
      if (get_sgtl_potentiometer(9, 0, 1, "Stereo O/I") == 0)
      {
        MasterOut.sgtl5000.surroundSoundDisable();
      }
      else
        MasterOut.sgtl5000.surroundSoundEnable();
    }
    else if (index == 10)
    {
      if (get_sgtl_potentiometer(10, 0, 1, "Bass O/I") == 0)
      {
        MasterOut.sgtl5000.enhanceBassDisable();
      }
      else
        MasterOut.sgtl5000.enhanceBassEnable();
    }
    else if (index >= 11 && index <= 14)
    {
      static float bass_levl;
      static float bass_levl_bass;
      static uint8_t bass_hipass;
      static uint8_t bass_cutoff;
      switch (index)
      {
      case 11:
        bass_levl = get_sgtl_potentiometer(11, 0, 127, "In Lvl") / MIDI_CC_RANGE_FLOAT;
        break;
      case 12:
        bass_levl_bass = get_sgtl_potentiometer(12, 0, 127, "Bass Lvl") / MIDI_CC_RANGE_FLOAT;
        break;
      case 13:
        bass_hipass = get_sgtl_potentiometer(13, 0, 1, "Hipass I/O");
        break;
      case 14:
        bass_cutoff = get_sgtl_potentiometer(14, 0, 6, "Cutoff I/O");
      default:
        break;
      }

      MasterOut.sgtl5000.enhanceBass(bass_levl, bass_levl_bass, bass_hipass, bass_cutoff);
    }
    else if (index == 15)
    {
      float vol = get_sgtl_potentiometer(15, 0, 127, "OutVol") / MIDI_CC_RANGE_FLOAT;
      MasterOut.sgtl5000.dacVolume(vol);
    }
  }
}
uint8_t get_sgtl_potentiometer(uint8_t index, uint8_t min, uint8_t max, const char *name)
{
  uint8_t _xPos = index % NUM_ENCODERS;
  uint8_t _yPos = index / NUM_ENCODERS;

  sgtlparameter[index] = constrain(sgtlparameter[index] + encoded[_xPos], min, max);
  // Serial.printf("sgtl parameter: %d, value: %d\n", index, sgtlparameter[index]);
  drawPot(_xPos, _yPos, sgtlparameter[index], name);
  return sgtlparameter[index];
}

void set_input_level(uint8_t _value)
{
  uint8_t ampl = _value / 8;

  MasterOut.sgtl5000.lineInLevel(ampl);
}
void save_plugin(uint8_t _songNr, uint8_t _pluginNr)
{
  allPlugins[_pluginNr]->save_plugin(_songNr);
}
void load_plugin(uint8_t _songNr, uint8_t _pluginNr)
{
  allPlugins[_pluginNr]->load_plugin(_songNr);
}

bool compareFiles(File &file, SerialFlashFile &ffile)
{
  file.seek(0);
  ffile.seek(0);
  unsigned long count = file.size();
  while (count > 0)
  {
    char buf1[128], buf2[128];
    unsigned long n = count;
    if (n > 128)
      n = 128;
    file.read(buf1, n);
    ffile.read(buf2, n);
    if (memcmp(buf1, buf2, n) != 0)
      return false; // differ
    count = count - n;
  }
  return true; // all data identical
}
void assign_PSRAM_variables()
{
  /*
   CCnames = new  char*[129];
   char _ccName[30];
  for (int i = 0; i <129; i++)
  {
 CCnames[i] = new char[30];
  }
  for (int i = 0; i <128; i++)
  {
 printf(_ccName,  "CC%d", i);
    CCnames[i] = _ccName;
    Serial.println(CCnames[i]);
  }
  CCnames[128]="none";
*/
  beatArray = new bool *[BEAT_ARRAY_SIZE];
  for (int i = 0; i < BEAT_ARRAY_SIZE; i++)
  {
    beatArray[i] = new bool[NUM_STEPS];
  }
  for (int i = 0; i < BEAT_ARRAY_SIZE; i++)
  {
    Serial.print(i);
    for (int j = 0; j < NUM_STEPS; j++)
    {

      // Beispiel: Füllen mit zufälligen booleschen Werten (true oder false)
      beatArray[i][j] = random(0, 2); // 0 für false, 1 für true
      Serial.print("  ");
      Serial.print(beatArray[i][j]);
    }
    Serial.println();
  }
  for (int j = 0; j < NUM_STEPS; j++)
  {
    beatArray[0][j] = 0;            // 0 für false, 1 für true
    beatArray[1][j] = 0;            // 0 für false, 1 für true
    beatArray[1][j] = (j % 4 == 0); // 0 für false, 1 für true
    beatArray[2][j] = 0;            // 0 für false, 1 für true
    beatArray[2][j] = (j % 4 == 0); // 0 für false, 1 für true
    beatArray[2][14] = 1;           // 0 für false, 1 für true
    int bucket = 0;
    bucket += 5;
    if (bucket >= NUM_STEPS)
    {
      beatArray[3][j] = 1; // Beat
      bucket -= NUM_STEPS;
    }
    else
    {
      beatArray[3][j] = 0; // Kein Beat
    }
    bucket += 7;
    if (bucket >= NUM_STEPS)
    {
      beatArray[4][j] = 1; // Beat
      bucket -= NUM_STEPS;
    }
    else
    {
      beatArray[4][j] = 0; // Kein Beat
    }
  }

  beatArrayPM6 = new bool *[MAX_VOICES];
  for (int i = 0; i < MAX_VOICES; i++)
  {
    beatArrayPM6[i] = new bool[NUM_STEPS];
  }
  for (int i = 0; i < MAX_VOICES; i++)
  {
    for (int j = 0; j < NUM_STEPS; j++)
    {
      // Beispiel: Füllen mit zufälligen booleschen Werten (true oder false)
      beatArrayPM6[i][j] = 0; // 0 für false, 1 für true
    }
  }
  beatArrayPM7 = new bool[NUM_STEPS];
  for (int j = 0; j < NUM_STEPS; j++)
  {
    // Beispiel: Füllen mit zufälligen booleschen Werten (true oder false)
    beatArrayPM7[j] = 0; // 0 für false, 1 für true
  }

  note_frequency = new float[128];
  for (int r = 0; r < 128; r++)
  {
    note_frequency[r] = pow(2.0, ((double)((r + 12) - SAMPLE_ROOT) / 12.0));
  }
}
void error(const char *message)
{
  while (1)
  {
    Serial.println(message);
    delay(2500);
  }
}

void writeVarLen(File &file, uint32_t value)
{
  uint8_t buffer[4];
  int count = 0;
  buffer[count++] = value & 0x7F;
  while ((value >>= 7) > 0)
  {
    buffer[count++] = 0x80 | (value & 0x7F);
  }
  for (int i = count - 1; i >= 0; i--)
  {
    file.write(buffer[i]);
  }
}

void writeMidiHeader(File &file, int numTracks, uint16_t ppqn)
{
  file.write("MThd", 4); // ✅ Richtig: "MThd"
  file.write((uint8_t)0);
  file.write((uint8_t)0);
  file.write((uint8_t)0);
  file.write((uint8_t)6);
  file.write((uint8_t)0);
  file.write((uint8_t)1); // Format 1
  file.write((uint8_t)(numTracks >> 8));
  file.write((uint8_t)(numTracks & 0xFF));
  file.write((uint8_t)(ppqn >> 8));
  file.write((uint8_t)(ppqn & 0xFF));
}

void export_midi_track(Track *track, int songNr, uint16_t ppqn = 24)
{

  char filename[20];
  sprintf(filename, "%02d_%02d.mid", songNr, track->my_Arranger_Y_axis);
  SD.remove(filename); // ← Alte Datei löschen, damit sie neu geschrieben wird
  delay(100);
  File file = SD.open(filename, FILE_WRITE);
  delay(100);
  if (!file)
  {
    Serial.println("Failed to open file");
    return;
  }

  writeMidiHeader(file, 1, ppqn);

  uint32_t trackStart = file.position();
  file.write("MTrk", 4);
  file.write((uint8_t)0);
  file.write((uint8_t)0);
  file.write((uint8_t)0);
  file.write((uint8_t)0); // placeholder

  uint32_t lastTick = 0;
  uint32_t noteOffTick[MAX_VOICES] = {0};
  uint32_t activeNote[MAX_VOICES] = {NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE};
  uint32_t readBar = 0;
  // Für alle Ticks im Clip
  for (int b = 0; b < myClock.endOfLoop; b += track->clip[track->clip_to_play[b]].clockDivision)
  {
    uint8_t activeClip = track->clip_to_play[b];
    uint32_t barTick = (b * track->clip[activeClip].seqLength);
    // if (b % track->clip[activeClip].clockDivision == 0)
    {

      for (int t = 0; t < track->clip[activeClip].seqLength; t++)
      {
        uint32_t songTick = barTick + (t * track->clip[activeClip].clockDivision);
        for (int v = 0; v < MAX_VOICES; v++)
        {

          uint32_t _startTick = (track->clip[activeClip].tick[t].startTick[v] * track->clip[activeClip].clockDivision) + barTick;
          uint32_t _length = track->clip[activeClip].tick[t].noteLength[v] * track->clip[activeClip].clockDivision;
          uint8_t _note = track->clip[activeClip].tick[t].voice[v] + track->noteOffset[b];

          uint8_t _velo = track->clip[activeClip].tick[t].velo[v] * (track->barVelocity[b] / 127.00);
          uint8_t midiChannel = track->my_Arranger_Y_axis;

          // Note On
          if (_startTick == songTick && _note < NO_NOTE && _velo > 0)
          {
            writeVarLen(file, songTick - lastTick);
            lastTick = songTick;

            file.write(0x90 | (midiChannel & 0x0F)); // Note On, Kanal 0
            file.write(_note);
            file.write(_velo);
            Serial.printf("vc%d_c%d_s%d_l%d_n%d_v%d\n", v, midiChannel, _startTick, _length, _note, _velo);
            noteOffTick[v] = songTick + _length;
            activeNote[v] = _note;
          }

          // Note Off
          if (noteOffTick[v] == songTick && activeNote[v] < NO_NOTE)
          {
            writeVarLen(file, songTick - lastTick);
            lastTick = songTick;

            file.write(0x80 | (midiChannel & 0x0F)); // Note Off, Kanal 0
            file.write(activeNote[v]);
            file.write((uint8_t)0);

            activeNote[v] = NO_NOTE;
          }
        }
      }
    }
  }

  // End of Track
  writeVarLen(file, 0);
  file.write(0xFF);
  file.write(0x2F);
  file.write(0x00);

  // Fix track length
  uint32_t trackEnd = file.position();
  uint32_t trackLength = trackEnd - trackStart - 8;
  file.seek(trackStart + 4);
  file.write((uint8_t)(trackLength >> 24));
  file.write((uint8_t)(trackLength >> 16));
  file.write((uint8_t)(trackLength >> 8));
  file.write((uint8_t)(trackLength));

  file.close();
  delay(100);
  Serial.print("Exported to MIDI: ");
  Serial.println(filename);
}
void export_serial_Midi(Track *track, uint16_t ppqn = 24)
{

  // Für alle Ticks im Clip
  for (int b = 0; b < myClock.endOfLoop; b += track->clip[track->clip_to_play[b]].clockDivision)
  {
    uint8_t activeClip = track->clip_to_play[b];
    uint32_t barTick = (b * track->clip[activeClip].seqLength);
    // if (b % track->clip[activeClip].clockDivision == 0)
    {

      for (int t = 0; t < track->clip[activeClip].seqLength; t++)
      {
        uint32_t songTick = barTick + (t * track->clip[activeClip].clockDivision);
        for (int v = 0; v < MAX_VOICES; v++)
        {

          uint32_t _startTick = (track->clip[activeClip].tick[t].startTick[v] * track->clip[activeClip].clockDivision) + barTick;
          uint32_t _length = track->clip[activeClip].tick[t].noteLength[v] * track->clip[activeClip].clockDivision;
          uint8_t _note = track->clip[activeClip].tick[t].voice[v] + track->noteOffset[b];

          uint8_t _velo = track->clip[activeClip].tick[t].velo[v] * (track->barVelocity[b] / 127.00);
          uint8_t midiChannel = track->my_Arranger_Y_axis;

          // Note On
          if (_startTick == songTick && _note < NO_NOTE && _velo > 0)
          {

            Serial.printf("vc%d_c%d_s%d_l%d_n%d_v%d\n", v, midiChannel, _startTick, _length, _note, _velo);
          }
        }
      }
    }
  }
}
bool loadMidiFile(const char *filename, MidiTrack &track)
{
  track.file = SD.open(filename);
  if (!track.file)
    return false;

  // Header Chunk lesen
  if (track.file.read() != 'M' || track.file.read() != 'T' || track.file.read() != 'h' || track.file.read() != 'd')
    return false;
  uint32_t headerLength = (track.file.read() << 24) | (track.file.read() << 16) | (track.file.read() << 8) | track.file.read();
  int format = (track.file.read() << 8) | track.file.read();
  int numTracks = (track.file.read() << 8) | track.file.read();
  track.division = (track.file.read() << 8) | track.file.read();

  if (format != 0 && format != 1)
    return false;

  // Erste Spur mit echten Notenereignissen finden
  for (int i = 0; i < numTracks; i++)
  {
    if (track.file.read() != 'M' || track.file.read() != 'T' || track.file.read() != 'r' || track.file.read() != 'k')
      return false;
    uint32_t trackLength = (track.file.read() << 24) | (track.file.read() << 16) | (track.file.read() << 8) | track.file.read();
    uint32_t pos = track.file.position();

    // Suche Note-On Event in dieser Spur
    bool hasNotes = false;
    uint32_t bytesLeft = trackLength;
    int runningStatus = 0;
    while (bytesLeft > 0 && track.file.available())
    {
      // DeltaTime überspringen
      int b, dummy;
      do
      {
        b = track.file.read();
        bytesLeft--;
      } while ((b & 0x80) && bytesLeft > 0);

      if (!track.file.available())
        break;
      int status = track.file.read();
      bytesLeft--;

      if (status < 0x80)
      {
        status = runningStatus;
        track.file.seek(track.file.position() - 1);
      }
      else
      {
        runningStatus = status;
      }

      int type = status & 0xF0;
      if (type == 0x90)
      {
        int note = track.file.read();
        bytesLeft--;
        int vel = track.file.read();
        bytesLeft--;
        if (vel > 0)
        {
          hasNotes = true;
          break;
        }
      }
      else if (type == 0x80 || type == 0xA0 || type == 0xB0 || type == 0xE0)
      {
        track.file.seek(track.file.position() + 2);
        bytesLeft -= 2;
      }
      else if (type == 0xC0 || type == 0xD0)
      {
        track.file.seek(track.file.position() + 1);
        bytesLeft -= 1;
      }
      else if (status == 0xFF)
      {
        dummy = track.file.read();
        bytesLeft--;
        do
        {
          b = track.file.read();
          bytesLeft--;
        } while (b & 0x80 && bytesLeft > 0);
      }
      else if (status == 0xF0 || status == 0xF7)
      {
        int len = track.file.read();
        bytesLeft--;
        track.file.seek(track.file.position() + len);
        bytesLeft -= len;
      }
    }

    if (hasNotes)
    {
      track.trackStart = pos;
      track.trackLength = trackLength;
      track.file.seek(pos); // Setze auf Track-Beginn zurück
      return true;
    }
    else
    {
      track.file.seek(pos + trackLength); // nächste Spur prüfen
    }
  }

  return false;
}
void receiveClipData()
{
  static String inputBuffer = "";
  static int trackIndex = 0;
  static bool receivedDataThisRound = false;

  while (Serial.available())
  {
    char c = Serial.read();
    if (c == '\n')
    {
      inputBuffer.trim();

      if (inputBuffer == "recieve")
      {
        Serial.println(">> Aufnahme gestartet");
        getArrangerFromPC = true;
        trackIndex = 0;
        receivedDataThisRound = false;
      }
      else if (inputBuffer == "EoL" && getArrangerFromPC)
      {
        if (receivedDataThisRound)
        {
          Serial.printf(">> Zeile %d abgeschlossen\n", trackIndex);

          trackIndex++;
          allTracks[trackIndex]->clear_arrangment();
          receivedDataThisRound = false;
        }

        if (trackIndex >= NUM_TRACKS)
        {
          getArrangerFromPC = false;
          set_infobox_background(500);
          tft.printf("got Arrangement");
          reset_infobox_background();
        }
      }
      else if (getArrangerFromPC)
      {
        receivedDataThisRound = true;
        for (unsigned int i = 0; i < inputBuffer.length() && i < MAX_BARS; i++)
        {
          char c = inputBuffer.charAt(i);
          if (c >= '0' && c <= '8')
          {
            uint8_t value = c - '0';
            allTracks[trackIndex]->clip_to_play[i] = value;
          }
          else
          {
            Serial.printf("Ungültiges Zeichen: %c\n", c);
          }
        }
      }

      inputBuffer = "";
    }
    else
    {
      inputBuffer += c;
    }
  }
}

int fillNoteInfoAtTick(MidiTrack &track, int miditick)
{
  if (!track.file)
    return 0;

  track.file.seek(track.trackStart);
  int tick = 0;
  int runningStatus = 0;
  uint32_t bytesLeft = track.trackLength;

  // Wir speichern alle Note-Ons, die bei miditick passieren
  struct TempNote
  {
    uint8_t note;
    uint8_t vel;
    int tick;
    bool foundOff;
    int len;
  };

  TempNote notes[8]; // Max 8 gleichzeitige Noten
  int numNotes = 0;

  while (bytesLeft > 0 && track.file.available())
  {
    // DeltaTime lesen
    int delta = 0, b;
    do
    {
      b = track.file.read();
      bytesLeft--;
      delta = (delta << 7) | (b & 0x7F);
    } while (b & 0x80 && bytesLeft > 0);
    tick += delta;

    int status = track.file.read();
    bytesLeft--;
    if (status < 0x80)
    {
      track.file.seek(track.file.position() - 1);
      status = runningStatus;
    }
    else
    {
      runningStatus = status;
    }

    int type = status & 0xF0;

    if (type == 0x90 || type == 0x80)
    {
      int note = track.file.read();
      bytesLeft--;
      int vel = track.file.read();
      bytesLeft--;

      // Speichere alle Note-Ons am gewünschten Tick
      if (type == 0x90 && vel > 0 && abs(tick - miditick) <= 10 && numNotes < MAX_VOICES)
      {
        notes[numNotes++] = {static_cast<uint8_t>(note), static_cast<uint8_t>(vel), tick, false, -1};
      }
    }
    else if (status == 0xFF)
    {
      int metaType = track.file.read();
      bytesLeft--;
      int len = 0;
      do
      {
        b = track.file.read();
        bytesLeft--;
        len = (len << 7) | (b & 0x7F);
      } while (b & 0x80 && bytesLeft > 0);
      track.file.seek(track.file.position() + len);
      bytesLeft -= len;
    }
    else if (status == 0xF0 || status == 0xF7)
    {
      int len = track.file.read();
      bytesLeft--;
      track.file.seek(track.file.position() + len);
      bytesLeft -= len;
    }
    else
    {
      int skip = (type == 0xC0 || type == 0xD0) ? 1 : 2;
      track.file.seek(track.file.position() + skip);
      bytesLeft -= skip;
    }
  }

  // Jetzt suche für jede Note das passende Note-Off
  if (numNotes > 0)
  {
    // Nochmal von vorne für Note-Offs
    track.file.seek(track.trackStart);
    tick = 0;
    bytesLeft = track.trackLength;
    runningStatus = 0;

    while (bytesLeft > 0 && track.file.available())
    {
      int delta = 0, b;
      do
      {
        b = track.file.read();
        bytesLeft--;
        delta = (delta << 7) | (b & 0x7F);
      } while (b & 0x80 && bytesLeft > 0);
      tick += delta;

      int status = track.file.read();
      bytesLeft--;
      if (status < 0x80)
      {
        track.file.seek(track.file.position() - 1);
        status = runningStatus;
      }
      else
      {
        runningStatus = status;
      }

      int type = status & 0xF0;
      if (type == 0x90 || type == 0x80)
      {
        int note = track.file.read();
        bytesLeft--;
        int vel = track.file.read();
        bytesLeft--;

        if ((type == 0x80 || (type == 0x90 && vel == 0)))
        {
          for (int i = 0; i < numNotes; i++)
          {
            if (notes[i].note == note && !notes[i].foundOff && tick > notes[i].tick)
            {
              notes[i].len = tick - notes[i].tick;
              notes[i].foundOff = true;
              break;
            }
          }
        }
      }
      else if (status == 0xFF)
      {
        int metaType = track.file.read();
        bytesLeft--;
        int len = 0;
        do
        {
          b = track.file.read();
          bytesLeft--;
          len = (len << 7) | (b & 0x7F);
        } while (b & 0x80 && bytesLeft > 0);
        track.file.seek(track.file.position() + len);
        bytesLeft -= len;
      }
      else if (status == 0xF0 || status == 0xF7)
      {
        int len = track.file.read();
        bytesLeft--;
        track.file.seek(track.file.position() + len);
        bytesLeft -= len;
      }
      else
      {
        int skip = (type == 0xC0 || type == 0xD0) ? 1 : 2;
        track.file.seek(track.file.position() + skip);
        bytesLeft -= skip;
      }
    }

    // Übergib die Daten in noteInfo[voice][4]
    for (int i = 0; i < numNotes; i++)
    {
      noteInfo[i][0] = notes[i].note;
      noteInfo[i][1] = notes[i].vel;
      noteInfo[i][2] = notes[i].tick;
      noteInfo[i][3] = notes[i].len;
    }

    return numNotes; // Anzahl der erkannten Stimmen
  }

  return 0;
}
int getPPQN(const MidiTrack &track)
{
  return track.division;
}
