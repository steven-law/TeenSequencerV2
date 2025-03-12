#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include "ownLibs/Adafruit_ST7796S_kbv.h"
#include <MIDI.h>
#include <USBHost_t36.h>
#include <Adafruit_MCP4728.h>
#include <SerialFlash.h>
#include "projectVariables.h"

#include "input/encoder.h"
#include "input/trellis.h"
#include "input/mouse.h"
#include "input/touch.h"

#include "output/tft.h"
#include "output/dac.h"

#include <ownLibs/myClock.h>
#include "Track.h"

#include <Plugins/plugin_List.h>
#include <FX/fx_List.h>
#include "FX/Output.h"
/*
Plugin_1 plugin_1("Strg", 1);
Plugin_2 plugin_2("1OSC", 2);
Plugin_3 plugin_3("2FM", 3);
Plugin_4 plugin_4("mDrm", 4);
Plugin_5 plugin_5("Drum", 5);
Plugin_6 plugin_6("Adtv", 6);
Plugin_7 plugin_7("Boom", 7);
Plugin_8 plugin_8("dTune", 8);
Plugin_9 plugin_9("rDrm", 9);
Plugin_10 plugin_10("SF2", 9);
Plugin_11 plugin_11("Ext", 9);*/

PluginControll *allPlugins[NUM_PLUGINS] = {&plugin_1, &plugin_2, &plugin_3, &plugin_4, &plugin_5, &plugin_6, &plugin_7, &plugin_8, &plugin_9, &plugin_10, &plugin_11, &plugin_12, &plugin_13, &plugin_14};
// PluginControll *allPlugins[NUM_PLUGINS] = {&plugin_1, &plugin_2, &plugin_3, &plugin_4, &plugin_5, &plugin_6, &plugin_7, &plugin_8, &plugin_9, &plugin_10, &plugin_11, &plugin_12, &plugin_13};
FX_1 fx_1("Rev", 1);
FX_2 fx_2("Bit", 2);
FX_3 fx_3("Nix", 3);
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
void input_behaviour();
// midi
void clock_to_notes(int _tick);
void midi_setup(uint8_t dly);
void midi_read();
void sendClock();
void onExternalStart();
void onExternalStop();

void sendNoteOn(uint8_t _track, uint8_t Note, uint8_t Velo, uint8_t Channel);
void sendNoteOff(uint8_t _track, uint8_t Note, uint8_t Velo, uint8_t Channel);
void sendControlChange(uint8_t control, uint8_t value, uint8_t Channel);

void myNoteOn(uint8_t channel, uint8_t note, uint8_t velocity);
void myNoteOff(uint8_t channel, uint8_t note, uint8_t velocity);
void myControlChange(uint8_t channel, uint8_t control, uint8_t value);
void myMIDIClock();
void usbDevice_handleInput();
void SerialMidi_handleInput();
void detect_USB_device();

void play_plugin_on_DAC(uint8_t _track, uint8_t _pluginNr);

// some trellis
void trellis_show_tft_mixer();
void trellis_play_mixer();
void trellis_perform();

// mixer
void set_mixer(uint8_t row);
void set_mixer_gain(uint8_t XPos, uint8_t YPos, const char *name, uint8_t trackn);
void set_mixer_FX_page1(uint8_t row);
void set_mixer_FX_page2(uint8_t row);
void set_mixer_dry(uint8_t XPos, uint8_t YPos, const char *name, uint8_t trackn);
void set_mixer_FX1(uint8_t XPos, uint8_t YPos, const char *name, uint8_t trackn);
void set_mixer_FX2(uint8_t XPos, uint8_t YPos, const char *name, uint8_t trackn);
void set_mixer_FX3(uint8_t XPos, uint8_t YPos, const char *name, uint8_t trackn);
void show_trellisFX_mixerPage();

void set_input_level(uint8_t _value);
void assign_PSRAM_variables();
bool compareFiles(File &file, SerialFlashFile &ffile);

void error(const char *message);
void save_plugin(uint8_t _songNr, uint8_t _pluginNr);
void load_plugin(uint8_t _songNr, uint8_t _pluginNr);
void setup()
{
  // while (!Serial)
  {
    /* code */
  }
  Serial.begin(115200);
  Wire1.begin();
  Wire1.setClock(100000);
  SD.begin(BUILTIN_SDCARD);
  tft_setup(100);
  encoder_setup(100);
  trellis_setup(100);
  neotrellis_setup(100);
  touch_setup();
  dac_setup(100);
  Serial.println("neotrellis Setup done");
  delay(1500);
  midi_setup(100);

  myClock.setup();
  AudioMemory(330);
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
  neotrellis.show();
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
}

void loop()
{
  unsigned long loopStartTime = millis();
  unsigned long trellisRestartMillis = millis();

  if (!digitalRead(NEOTRELLIS_INT_PIN))
  {
    // Serial.println("reading trellis");
    neotrellis.read();
    neotrellis.setPixelColor(3, 1, TRELLIS_AQUA);
    neotrellis.show();

    delay(0);
  }
  neotrellis_set_control_buffer(3, 1, TRELLIS_BLACK);
  // neotrellis.setPixelColor(3, 1, TRELLIS_BLACK);
  // neotrellis.show();
  readEncoders();
  trellis_update();
  neotrellis_update();
  midi_read();
  input_behaviour();
  touch_update();
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    allTracks[i]->update(pixelTouchX, gridTouchY);
    trellis_show_clockbar(i, allTracks[i]->internal_clock / TICKS_PER_STEP);
    // if (allTracks[i]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1) >= 0)
    //  play_plugin_on_DAC(i, allTracks[i]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1));
  }
  get_infobox_background();
  unsigned long loopEndTime = millis();
  unsigned long neotrellisCurrentMillis = millis();
  unsigned long updateMidiCurrentMillis = micros();
  // if we need to restart the trellisboard

  if (updateMidiCurrentMillis - updateMidiPreviousMillis >= 250)
  {
    // midi_read();
    updateMidiPreviousMillis = updateMidiCurrentMillis;
  }
  if (neotrellisCurrentMillis - neotrellisReadPreviousMillis >= neotrellisReadInterval)
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
    trellis_writeDisplay();
  }

  if (loopEndTime - loopStartTime > 600 /*|| trellisCurrentMillis - trellisRestartPreviousMillis >= trellisRestartInterval*/)
  {
    trellisRestartPreviousMillis = trellisRestartMillis;
    // trellis.trellisReset();
    Serial.printf("restart trellis @ %d\n", loopEndTime - loopStartTime);
    neotrellis.begin();
    neotrellis_setup(0);
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
      int tempTick = (pixelTouchX - SEQ_GRID_LEFT) / 3;
      allTracks[active_track]->set_note_on_tick(tempTick, gridTouchY);
      neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;
    }
    if (neotrellisPressed[TRELLIS_BUTTON_ENTER] && neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
      allTracks[active_track]->clear_active_clip();

      neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;
      neotrellisPressed[TRELLIS_BUTTON_SHIFT] = false;
    }
    /*
    if (ts.touched())
    {
      allTracks[active_track]->parameter[SET_STEP_LENGTH] = 1;
      int tempTick = (pixelTouchX - SEQ_GRID_LEFT) / 2;
      allTracks[active_track]->set_note_on_tick(tempTick, gridTouchY);
      delay(20);
    }*/
    if (neotrellisPressed[TRELLIS_POTROW])
    {
      change_plugin_row = true;
      draw_stepSequencer_parameters(lastPotRow);
      Serial.printf("active screen: %d, activeTrack: %d\n", activeScreen, active_track);
      neotrellisPressed[TRELLIS_POTROW] = false;
    }
    allTracks[active_track]->set_stepSequencer_parameters(lastPotRow);

    break;
  }
  case INPUT_FUNCTIONS_FOR_ARRANGER:
  {
    neotrellis_SetCursor(8);
    trellis_set_arranger();
    allTracks[gridTouchY - 1]->set_arranger_parameters(lastPotRow);
    if (neotrellisPressed[TRELLIS_BUTTON_ENTER] && neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
      allTracks[active_track]->clear_arrangment();

      neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;
      neotrellisPressed[TRELLIS_BUTTON_SHIFT] = false;
    }
    draw_arranger_parameters(lastPotRow);
    if (neotrellisPressed[TRELLIS_POTROW])
    {
      Serial.printf("active screen: %d, arrangerpage: %d\n", activeScreen, arrangerpage);
      change_plugin_row = true;
      

      // draw_arrangment_lines(gridTouchY - 1, arrangerpage);
      neotrellisPressed[TRELLIS_POTROW] = false;
    }

    if (lastPotRow == 3)
    {
      myClock.set_tempo(1);
      myClock.set_start_of_loop(2); // Encoder: 2
      myClock.set_end_of_loop(3);   // Encoder: 3
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
    // if Shift button is NOT pressed
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
      allTracks[active_track]->set_seq_mode_parameters(lastPotRow);
    }
    break;
  }
  case INPUT_FUNCTIONS_FOR_PLUGIN:
  {
    if (allTracks[active_track]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
      allTracks[active_track]->set_MIDI_CC(lastPotRow);
    else if (allTracks[active_track]->parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
      MasterOut.set_parameters(allTracks[active_track]->parameter[SET_MIDICH_OUT] - 49, lastPotRow);
    neotrellisPressed[TRELLIS_POTROW] = false;
    break;
  }
  case INPUT_FUNCTIONS_FOR_MIXER1:
  {
    set_mixer(lastPotRow);
    trellis_play_mixer();
    break;
  }
  case INPUT_FUNCTIONS_FOR_MIXER2:
  {
    trellis_play_mixer();
    set_mixer_FX_page1(lastPotRow);
  
    break;
  }
  case INPUT_FUNCTIONS_FOR_MIXER3:
  {
    trellis_play_mixer();
    set_mixer_FX_page2(lastPotRow);
    break;
  }
  case INPUT_FUNCTIONS_FOR_FX1:
  {
    trellis_play_mixer();
    fx_1.set_parameters(lastPotRow);
    break;
  }
  case INPUT_FUNCTIONS_FOR_FX2:
  {
    trellis_play_mixer();
    fx_2.set_parameters(lastPotRow);
    break;
  }
  case INPUT_FUNCTIONS_FOR_CLIPLAUNCHER:
  {
    trellis_play_clipLauncher();
    break;
  }
  default:
    break;
  }
  /*
  // if we are in one of the sequencer pages
  if (activeScreen == INPUT_FUNCTIONS_FOR_SEQUENCER)
  {

    neotrellis_SetCursor(14);
    if (neotrellisPressed[TRELLIS_BUTTON_ENTER] && !neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
      int tempTick = (pixelTouchX - SEQ_GRID_LEFT) / 2;
      allTracks[active_track]->set_note_on_tick(tempTick, gridTouchY);
      neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;
    }
    if (neotrellisPressed[TRELLIS_BUTTON_ENTER] && neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
      allTracks[active_track]->clear_active_clip();

      neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;
      neotrellisPressed[TRELLIS_BUTTON_SHIFT] = false;
    }
    /*
    if (ts.touched())
    {
      allTracks[active_track]->parameter[SET_STEP_LENGTH] = 1;
      int tempTick = (pixelTouchX - SEQ_GRID_LEFT) / 2;
      allTracks[active_track]->set_note_on_tick(tempTick, gridTouchY);
      delay(20);
    }
  if (neotrellisPressed[TRELLIS_POTROW])
  {
    change_plugin_row = true;
    draw_stepSequencer_parameters(lastPotRow);
    Serial.printf("active screen: %d, activeTrack: %d\n", activeScreen, active_track);
    neotrellisPressed[TRELLIS_POTROW] = false;
  }
  allTracks[active_track]->set_stepSequencer_parameters(lastPotRow);
}
// if we are in one of the Arrangerpages
if (activeScreen == INPUT_FUNCTIONS_FOR_ARRANGER)
{

  neotrellis_SetCursor(8);
  allTracks[gridTouchY - 1]->set_arranger_parameters(lastPotRow);
  if (neotrellisPressed[TRELLIS_BUTTON_ENTER] && neotrellisPressed[TRELLIS_BUTTON_SHIFT])
  {
    allTracks[active_track]->clear_arrangment();

    neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;
    neotrellisPressed[TRELLIS_BUTTON_SHIFT] = false;
  }
  if (neotrellisPressed[TRELLIS_POTROW])
  {
    Serial.printf("active screen: %d, arrangerpage: %d\n", activeScreen, arrangerpage);
    change_plugin_row = true;
    draw_arranger_parameters(lastPotRow);

    // draw_arrangment_lines(gridTouchY - 1, arrangerpage);
    neotrellisPressed[TRELLIS_POTROW] = false;
  }

  if (lastPotRow == 3)
  {
    myClock.set_tempo(1);
    myClock.set_start_of_loop(2); // Encoder: 2
    myClock.set_end_of_loop(3);   // Encoder: 3
  }
}
if (activeScreen == INPUT_FUNCTIONS_FOR_SEQUENCER_MODES)
{
  if (neotrellisPressed[TRELLIS_POTROW])
  {
    tft.fillRect(18 * STEP_FRAME_W, 5 * STEP_FRAME_H, 20 * STEP_FRAME_W, 12 * STEP_FRAME_H, ILI9341_DARKGREY);
    neotrellisPressed[TRELLIS_POTROW] = false;
  }
  // if Shift button is NOT pressed
  if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
  {
    allTracks[active_track]->set_seq_mode_parameters(lastPotRow);
  }
}
if (activeScreen == INPUT_FUNCTIONS_FOR_PLUGIN)
{

  if (allTracks[active_track]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
    allTracks[active_track]->set_MIDI_CC(lastPotRow);
  else if (allTracks[active_track]->parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
    MasterOut.set_parameters(allTracks[active_track]->parameter[SET_MIDICH_OUT] - 49, lastPotRow);
  neotrellisPressed[TRELLIS_POTROW] = false;
}
if (activeScreen == INPUT_FUNCTIONS_FOR_MIXER1)
  set_mixer(lastPotRow);
if (activeScreen == INPUT_FUNCTIONS_FOR_MIXER2)
  set_mixer_FX_page1(lastPotRow);
if (activeScreen == INPUT_FUNCTIONS_FOR_MIXER3)
  set_mixer_FX_page2(lastPotRow);
if (activeScreen == INPUT_FUNCTIONS_FOR_FX1)
  fx_1.set_parameters(lastPotRow);
if (activeScreen == INPUT_FUNCTIONS_FOR_FX2)
  fx_2.set_parameters(lastPotRow);
if (activeScreen == INPUT_FUNCTIONS_FOR_CLIPLAUNCHER)
{
  // Serial.println("hi");
  trellis_play_clipLauncher();
}
*/
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

  usbMIDI.setHandleNoteOff(myNoteOff);
  usbMIDI.setHandleNoteOn(myNoteOn);
  usbMIDI.setHandleControlChange(myControlChange);
  usbMIDI.setHandleClock(myMIDIClock);
  usbMIDI.setHandleStart(onExternalStart);
  usbMIDI.setHandleStop(onExternalStop);
  usbDeviceTimer.begin(usbDevice_handleInput, 250);
  usbDeviceTimer.priority(80);
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
    Serial.printf("Note ON: channel:%d, Note: %d, Velo: %d\n", Channel, Note, Velo);
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
}
void sendControlChange(uint8_t control, uint8_t value, uint8_t Channel)
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
}

void myNoteOn(uint8_t channel, uint8_t note, uint8_t velocity)
{
  if (channel < 9 && !allTracks[channel - 1]->muted)
  {
    allTracks[channel - 1]->noteOn(note, velocity, allTracks[channel - 1]->parameter[SET_MIDICH_OUT]);
    if (allTracks[channel - 1]->get_recordState())
      allTracks[channel - 1]->record_noteOn(note, velocity, allTracks[channel - 1]->parameter[SET_MIDICH_OUT]);
  }
  if (channel >= 9)
    sendNoteOn(channel - 1, note, velocity, channel);
  Serial.printf("note: %d, velo: %d, channel: %d\n ", note, velocity, channel);
}
void myNoteOff(uint8_t channel, uint8_t note, uint8_t velocity)
{
  if (channel < 9 && !allTracks[channel - 1]->muted)
  {
    allTracks[channel - 1]->noteOff(note, velocity, allTracks[channel - 1]->parameter[SET_MIDICH_OUT]);
    if (allTracks[channel - 1]->get_recordState())
      allTracks[channel - 1]->record_noteOff(note, velocity, allTracks[channel - 1]->parameter[SET_MIDICH_OUT]);
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
        int _pluginCh = allTracks[channel - 1]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1);
        if (allTracks[channel - 1]->parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
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
    sendControlChange(control, value, channel);
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
void trellis_show_tft_mixer()
{
  if (neotrellisPressed[TRELLIS_BUTTON_MIXER])
  {
    if (trellisScreen != TRELLIS_SCREEN_SONGPAGE_SELECTION)
    {
      trellisScreen = TRELLIS_SCREEN_SONGPAGE_SELECTION;
      trellis.clear();
      for (int i = 0; i < 8; i++)
      {
        trellis.setLED(TrellisLED[i]);
        trellis.writeDisplay();
      }
    }

    if (trellisPressed[0])
    {
      trellis.clear();
      trellis.writeDisplay();
      trellisPressed[0] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER1;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_MIXER1;
      clearWorkSpace();
      draw_mixer();
      show_active_page_info("Mixer", 0);
    }
    if (trellisPressed[1])
    {
      trellis.clear();
      show_trellisFX_mixerPage();
      trellis.writeDisplay();
      trellisPressed[1] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_MIXER2;
      clearWorkSpace();
      draw_mixer_FX_page1();
      show_active_page_info("FX Vol", 1);
      // trellis_recall_main_buffer(TRELLIS_SCREEN_MIXER);
    }
    if (trellisPressed[2])
    {
      trellis.clear();
      show_trellisFX_mixerPage();
      trellis.writeDisplay();
      trellisPressed[2] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_MIXER3;
      clearWorkSpace();
      draw_mixer_FX_page2();
      show_active_page_info("FX Vol", 2);
      // trellis_recall_main_buffer(TRELLIS_SCREEN_MIXER);
    }
    if (trellisPressed[3])
    {
      trellis.clear();
      show_trellisFX_mixerPage();
      trellis.writeDisplay();
      trellisPressed[3] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_FX1;
      clearWorkSpace();
      fx_1.draw_plugin();
      show_active_page_info("FX Ctrl", 1);

      // trellis_recall_main_buffer(TRELLIS_SCREEN_MIXER);
    }
    if (trellisPressed[4])
    {
      trellis.clear();
      show_trellisFX_mixerPage();
      trellis.writeDisplay();
      trellisPressed[4] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_FX2;
      clearWorkSpace();
      fx_2.draw_plugin();
      show_active_page_info("FX Ctrl", 2);

      // trellis_recall_main_buffer(TRELLIS_SCREEN_MIXER);
    }
    if (trellisPressed[5])
    {
      trellis.clear();
      show_trellisFX_mixerPage();
      trellis.writeDisplay();
      trellisPressed[5] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_FX3;
      clearWorkSpace();
      fx_3.draw_plugin();
      show_active_page_info("FX Ctrl", 3);

      // trellis_recall_main_buffer(TRELLIS_SCREEN_MIXER);
    }
    if (trellisPressed[6])
    {
      trellis.clear();
      trellis.writeDisplay();
      trellisPressed[6] = false;
      trellisScreen = TRELLIS_SCREEN_PERFORM;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_PERFORM;

      clearWorkSpace();
      // activeScreen = INPUT_FUNCTIONS_FOR_FX1;
      // clearWorkSpace();
      set_perform_page(lastPotRow);
      show_active_page_info("Perform", 0);

      // trellis_recall_main_buffer(TRELLIS_SCREEN_PERFORM);
    }
    if (trellisPressed[7])
    {
      trellis.clear();
      trellis.writeDisplay();
      trellisPressed[7] = false;
      trellisScreen = TRELLIS_SCREEN_CLIPLAUNCHER;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      // change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_CLIPLAUNCHER;

      clearWorkSpace();
      // activeScreen = INPUT_FUNCTIONS_FOR_FX1;
      // clearWorkSpace();
      // set_perform_page(lastPotRow);
      draw_clip_launcher();
      show_active_page_info("Launch", 0);

      // trellis_recall_main_buffer(TRELLIS_SCREEN_PERFORM);
    }
  }
}
void trellis_play_mixer()
{
  if (trellisScreen == TRELLIS_SCREEN_MIXER1)
  {
    for (int t = 0; t < NUM_TRACKS; t++)
    {

      for (int s = 0; s < NUM_STEPS; s++)
      {
        trellis_set_main_buffer(TRELLIS_SCREEN_MIXER1, s, t, TRELLIS_BLACK);
        trellis_set_main_buffer(TRELLIS_SCREEN_MIXER1, allTracks[t]->mixGainPot / 8, t, trackColor[t]);
        uint8_t _nr = s + (t * TRELLIS_PADS_X_DIM);
        if (trellisPressed[_nr])
        {
          uint8_t _gain[NUM_STEPS] = {0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 111, 119, 127};
          trellisPressed[_nr] = false;

          allTracks[t]->mixGainPot = _gain[s];
          set_infobox_background(750);
          tft.printf("Track: %d", t);
          set_infobox_next_line(1);
          tft.printf("Main Vol =  %d ", _gain[s]);
          reset_infobox_background();
          break;
        }
      }
    }
  }
  if (trellisScreen == TRELLIS_SCREEN_MIXER)
  {
    float _gain[4] = {0, 0.30, 0.60, 1};

    for (int t = 0; t < NUM_TRACKS; t++)
    {
      // if (allTracks[t]->parameter[SET_MIDICH_OUT] >= (NUM_MIDI_OUTPUTS + 1))
      {
        for (int s = 0; s < NUM_STEPS; s++)
        {

          uint8_t _nr = s + (t * TRELLIS_PADS_X_DIM);

          if (trellisPressed[_nr])
          {
            trellisPressed[_nr] = false;
            for (int c = 0; c < 4; c++)
            {

              if (_nr % TRELLIS_PADS_X_DIM == c)
              {
                Serial.printf("dry channel = %d, track channel : %d\n", allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[t]->parameter[SET_MIDICH_OUT]);
                MasterOut.fx_section.dry[allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(_gain[c]);
                allTracks[t]->mixDryPot = (c * 42);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 0, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 1, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 2, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 3, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, c, t, TRELLIS_PINK);
                set_infobox_background(750);
                tft.printf("Track: %d", t);
                set_infobox_next_line(1);
                tft.printf("Dry Vol =  %d ", (c * 42));
                reset_infobox_background();
                break;
              }
              if (_nr % TRELLIS_PADS_X_DIM == c + 4)
              {
                Serial.printf("fx1 channel = %d, track channel : %d\n", allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[t]->parameter[SET_MIDICH_OUT]);
                fx_1.pl[allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(_gain[c]);
                allTracks[t]->mixFX1Pot = (c * 42);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 4, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 5, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 6, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 7, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, c + 4, t, TRELLIS_OLIVE);
                set_infobox_background(750);
                tft.printf("Track: %d", t);
                set_infobox_next_line(1);
                tft.printf("FX1 Vol =  %d ", (c * 42));
                reset_infobox_background();
                break;
              }
              if (_nr % TRELLIS_PADS_X_DIM == c + 8)
              {
                Serial.printf("fx2 channel = %d, track channel : %d\n", allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[t]->parameter[SET_MIDICH_OUT]);
                fx_2.pl[allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(_gain[c]);
                allTracks[t]->mixFX2Pot = (c * 42);

                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 8, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 9, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 10, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 11, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, c + 8, t, TRELLIS_AQUA);
                set_infobox_background(750);
                tft.printf("Track: %d", t);
                set_infobox_next_line(1);
                tft.printf("FX2 Vol =  %d ", (c * 42));
                reset_infobox_background();
                break;
              }

              if (_nr % TRELLIS_PADS_X_DIM == c + 12)
              {
                Serial.printf("fx3 channel = %d, track channel : %d\n", allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[t]->parameter[SET_MIDICH_OUT]);
                fx_3.pl[allTracks[t]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(_gain[c]);
                allTracks[t]->mixFX3Pot = (c * 42);

                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 12, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 13, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 14, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 15, t, TRELLIS_BLACK);
                trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, c + 12, t, TRELLIS_ORANGE);
                set_infobox_background(750);
                tft.printf("Track: %d", t);
                set_infobox_next_line(1);
                tft.printf("FX3 Vol =  %d ", (c * 42));
                reset_infobox_background();
                break;
              }
            }

            // change_plugin_row=true;
          }
        }
      }
    }
  }
}
void trellis_perform()
{
  if (trellisScreen == TRELLIS_SCREEN_PERFORM)
  {
    set_perform_page(lastPotRow);
    for (int t = 0; t < NUM_TRACKS; t++)
    {
      for (int i = 0; i < NUM_STEPS; i++)
      {
        int _nr = i + (t * TRELLIS_PADS_X_DIM);
        if (trellisPressed[_nr])

        {
          // change_plugin_row = true;
          trellisPressed[_nr] = false;
          if (_nr % TRELLIS_PADS_X_DIM == 0)
          {

            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                allTracks[s]->mixGainPot = 127 - (t * 16);
                trellisPerformIndex[0] = t;
                // sendControlChange(performCC[0], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
              }
            }
            set_infobox_background(750);
            tft.printf("Master Vol =  %d ", 127 - (t * 16));
            // set_infobox_next_line(1);
            // tft.printf("send CC%d =  %d ", performCC[0],127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 1)
          {
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {

                Serial.printf("dry channel = %d, track channel : %d\n", allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[s]->parameter[SET_MIDICH_OUT]);
                allTracks[s]->mixDryPot = 127 - (t * 16);
                if (allTracks[s]->parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
                  MasterOut.fx_section.dry[allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(t / 8.00);
                if (allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                  sendControlChange(performCC[1], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
                trellisPerformIndex[1] = t;
                // break;
              }
            }
            set_infobox_background(750);
            tft.printf("dry Vol =  %d ", 127 - (t * 16));
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[1], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 2)
          {
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                Serial.printf("fx1 plugin channel = %d, track channel : %d\n", allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[s]->parameter[SET_MIDICH_OUT]);
                allTracks[s]->mixFX1Pot = 127 - (t * 16);
                if (allTracks[s]->parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
                  fx_1.pl[allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(t / 8.00);
                if (allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                  sendControlChange(performCC[2], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
                trellisPerformIndex[2] = t;
                // break;
              }
            }
            set_infobox_background(750);
            tft.printf("FX1 Vol =  %d ", 127 - (t * 16));
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[2], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 3)
          {
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                Serial.printf("fx2 plugin channel = %d, track channel : %d\n", allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[s]->parameter[SET_MIDICH_OUT]);
                allTracks[s]->mixFX2Pot = 127 - (t * 16);
                if (allTracks[s]->parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
                  fx_2.pl[allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(t / 8.00);
                if (allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                  sendControlChange(performCC[3], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
                trellisPerformIndex[3] = t;
                // break;
              }
            }
            set_infobox_background(750);
            tft.printf("FX2 Vol =  %d ", 127 - (t * 16));
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[3], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 4)
          {
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                Serial.printf("fx3 plugin channel = %d, track channel : %d\n", allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1), allTracks[s]->parameter[SET_MIDICH_OUT]);
                allTracks[s]->mixFX3Pot = 127 - (t * 16);
                if (allTracks[s]->parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
                  fx_3.pl[allTracks[s]->parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)].gain(t / 8.00);
                if (allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                  sendControlChange(performCC[4], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
                trellisPerformIndex[4] = t;
                // break;
              }
            }
            set_infobox_background(750);
            tft.printf("FX3 Vol =  %d ", 127 - (t * 16));
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[4], 127 - (t * 16));
            reset_infobox_background();
          }

          if (_nr % TRELLIS_PADS_X_DIM == 5)
          {
            fx_1.potentiometer[fx_1.presetNr][0] = 127 - (t * 16);
            fx_1.freeverb.roomsize((float)map(t, 0, 8, 0, 1.00));
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive && allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                sendControlChange(performCC[5], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
            }
            trellisPerformIndex[5] = t;
            set_infobox_background(750);
            tft.printf("FX1 Roomsize =  %d ", 127 - (t * 16));
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[5], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 6)
          {

            fx_1.potentiometer[fx_1.presetNr][1] = 127 - (t * 16);
            fx_1.freeverb.damping((float)map(t, 0, 8, 0, 1.00));
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive && allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                sendControlChange(performCC[6], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
            }
            trellisPerformIndex[6] = t;
            set_infobox_background(750);
            tft.printf("FX1 Damping =  %d ", 127 - (t * 16));
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[6], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 7)
          {
            fx_2.potentiometer[fx_2.presetNr][0] = 127 - (t * 16);

            fx_2.bitcrusher.bits(map(t, 0, 8, 16, 0));
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive && allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                sendControlChange(performCC[7], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
            }
            trellisPerformIndex[7] = t;
            set_infobox_background(750);
            tft.printf("FX2 BitDepth =  %d ", map(t, 0, 8, 16, 0));
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[7], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 8)
          {
            int _rate[Y_DIM] = {44100, 22050, 11025, 5512, 2756, 1378, 1022, 689};
            fx_2.potentiometer[fx_2.presetNr][1] = 127 - (t * 16);
            fx_2.bitcrusher.sampleRate(_rate[t]);
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive && allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                sendControlChange(performCC[8], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
            }
            trellisPerformIndex[8] = t;
            set_infobox_background(750);
            tft.printf("FX2 SmplRate =  %d ", _rate[t]);
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[8], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 9)
          {
            fx_3.potentiometer[fx_3.presetNr][0] = 127 - (t * 16);
            fx_3.delay.delay(0, 500 / (t + 1));
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive && allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                sendControlChange(performCC[9], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
            }
            trellisPerformIndex[9] = t;
            set_infobox_background(750);
            tft.printf("FX3 DlyTime =  %d ", 500 / (t + 1));
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[9], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 10)
          {
            fx_3.potentiometer[fx_3.presetNr][1] = 127 - (t * 16);
            fx_3.delayMixer.gain(1, t / 8.00);
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive && allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                sendControlChange(performCC[10], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
            }
            trellisPerformIndex[10] = t;
            set_infobox_background(750);
            tft.printf("FX3 Feedback =  %d ", 127 - (t * 16));
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[10], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 11)
          {
            int frequency = note_frequency[t * 16] * tuning;
            MasterOut.finalFilter.frequency(frequency);
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive && allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                sendControlChange(performCC[11], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
            }
            trellisPerformIndex[11] = t;
            set_infobox_background(750);
            tft.printf("Fltr Freq =  %d ", frequency);
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[11], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 12)
          {
            MasterOut.finalFilter.resonance(map(t, 0, 8, 0, 5.00));
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive && allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                sendControlChange(performCC[12], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
            }
            trellisPerformIndex[12] = t;
            set_infobox_background(750);
            tft.printf("Fltr Reso =  %d ", 127 - (t * 16));
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[12], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 13)
          {
            uint8_t _clipLength[Y_DIM]{MAX_TICKS, 72, 60, MAX_TICKS / 2, 36, MAX_TICKS / 4, MAX_TICKS / 8, MAX_TICKS / 16};
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                {
                  allTracks[s]->parameter[SET_SEQUENCE_LENGTH] = _clipLength[t];
                  if (allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                  {
                    sendControlChange(performCC[13], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
                  }
                }
              }

              if (t == 0)
              {
                allTracks[s]->internal_clock = 0;
                allTracks[s]->internal_clock_bar = myClock.barTick;
                allTracks[s]->external_clock_bar = myClock.barTick;
              }
            }
            trellisPerformIndex[13] = t;
            set_infobox_background(750);
            tft.printf("Clip Length =  %d ", _clipLength[t]);
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[13], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 14)
          {
            uint8_t _clockDivision[Y_DIM]{0, 1, 2, 3, 4, 6, 8, 16};
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                allTracks[s]->performClockDivision = _clockDivision[t];
                if (allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                  sendControlChange(performCC[14], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
              }
              if (t == 0)
              {
                allTracks[s]->internal_clock = 0;
                allTracks[s]->internal_clock_bar = myClock.barTick;
                allTracks[s]->external_clock_bar = myClock.barTick;
              }
            }
            trellisPerformIndex[14] = t;
            set_infobox_background(750);
            tft.printf("Step Div =  %d ", _clockDivision[t]);
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[14], 127 - (t * 16));
            reset_infobox_background();
          }
          if (_nr % TRELLIS_PADS_X_DIM == 15)
          {
            int _offset[Y_DIM]{0, -12, -4, -2, 2, 4, 6, 12};
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                allTracks[s]->performNoteOffset = _offset[t];
                if (allTracks[s]->parameter[SET_MIDICH_OUT] <= NUM_MIDI_OUTPUTS)
                  sendControlChange(performCC[15], 127 - (t * 16), allTracks[s]->parameter[SET_MIDICH_OUT]);
              }
            }
            trellisPerformIndex[15] = t;
            set_infobox_background(750);
            tft.printf("Note Transpose =  %d ", _offset[t]);
            set_infobox_next_line(1);
            tft.printf("send CC%d =  %d ", performCC[15], 127 - (t * 16));
            reset_infobox_background();
          }
        }
      }
    }
  }
}

// Mixer
void set_mixer(uint8_t row)
{
  draw_mixer();
  if (row == 0)
  {
    set_mixer_gain(0, 0, "Tr D", 0);
    set_mixer_gain(1, 0, "Tr 2", 1);
    set_mixer_gain(2, 0, "Tr 3", 2);
    set_mixer_gain(3, 0, "Tr 4", 3);
  }

  if (row == 1)
  {
  }

  if (row == 2)
  {
    set_mixer_gain(0, 2, "Tr 5", 4);
    set_mixer_gain(1, 2, "Tr 6", 5);
    set_mixer_gain(2, 2, "Tr 7", 6);
    set_mixer_gain(3, 2, "Tr 8", 7);
  }

  if (row == 3)
  {
  }
}
void set_mixer_gain(uint8_t XPos, uint8_t YPos, const char *name, uint8_t trackn)
{

  if (enc_moved[XPos])
  {

    allTracks[trackn]->mixGainPot = constrain(allTracks[trackn]->mixGainPot + encoded[XPos], 0, MIDI_CC_RANGE);
    allTracks[trackn]->mixGain = (float)(allTracks[trackn]->mixGainPot / MIDI_CC_RANGE_FLOAT);
    Serial.printf("set mixgainpot: %d for track %d\n", allTracks[trackn]->mixGainPot, trackn);
    /*for (int i = 0; i < NUM_PLUGINS; i++)
   {
     if (allTracks[trackn]->MIDI_channel_out == i+17)
       allPlugins[i]->MixGain.gain(allTracks[trackn]->mixGain);
   }*/
    if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_1)
      plugin_1.MixGain.gain(allTracks[trackn]->mixGain);
    if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_2)
      plugin_2.MixGain.gain(allTracks[trackn]->mixGain);
    if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_3)
      plugin_3.MixGain.gain(allTracks[trackn]->mixGain);
    if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_4)
      plugin_4.MixGain.gain(allTracks[trackn]->mixGain);
    if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_5)
      plugin_5.MixGain.gain(allTracks[trackn]->mixGain);
    if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_6)
      plugin_6.MixGain.gain(allTracks[trackn]->mixGain);
    if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_7)
      plugin_7.MixGain.gain(allTracks[trackn]->mixGain);
    if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_8)
      plugin_8.MixGain.gain(allTracks[trackn]->mixGain);
    if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_9)
      plugin_9.MixGain.gain(allTracks[trackn]->mixGain);

    drawPot(XPos, YPos, allTracks[trackn]->mixGainPot, name);
  }
}
void set_mixer_FX_page1(uint8_t row)
{
  draw_mixer_FX_page1();
  if (row == 0)
  {

    set_mixer_dry(0, 0, "Dry D", 0);
    set_mixer_FX1(1, 0, "FX1 D", 0);
    set_mixer_FX2(2, 0, "FX2 D", 0);
    set_mixer_FX3(3, 0, "FX3 D", 0);
  }

  if (row == 1)
  {
    set_mixer_dry(0, 1, "Dry 2", 1);
    set_mixer_FX1(1, 1, "FX1 2", 1);
    set_mixer_FX2(2, 1, "FX2 2", 1);
    set_mixer_FX3(3, 1, "FX3 2", 1);
  }

  if (row == 2)
  {
    set_mixer_dry(0, 2, "Dry 3", 2);
    set_mixer_FX1(1, 2, "FX1 3", 2);
    set_mixer_FX2(2, 2, "FX2 3", 2);
    set_mixer_FX3(3, 2, "FX3 3", 2);
  }

  if (row == 3)
  {
    set_mixer_dry(0, 3, "Dry 4", 3);
    set_mixer_FX1(1, 3, "FX1 4", 3);
    set_mixer_FX2(2, 3, "FX2 4", 3);
    set_mixer_FX3(3, 3, "FX3 4", 3);
  }
}
void set_mixer_FX_page2(uint8_t row)
{
  draw_mixer_FX_page1();
  if (row == 0)
  {

    set_mixer_dry(0, 0, "Dry 5", 4);
    set_mixer_FX1(1, 0, "FX1 5", 4);
    set_mixer_FX2(2, 0, "FX2 5", 4);
    set_mixer_FX3(3, 0, "FX3 5", 4);
  }

  if (row == 1)
  {
    set_mixer_dry(0, 1, "Dry 6", 5);
    set_mixer_FX1(1, 1, "FX1 6", 5);
    set_mixer_FX2(2, 1, "FX2 6", 5);
    set_mixer_FX3(3, 1, "FX3 6", 5);
  }

  if (row == 2)
  {
    set_mixer_dry(0, 2, "Dry 7", 6);
    set_mixer_FX1(1, 2, "FX1 7", 6);
    set_mixer_FX2(2, 2, "FX2 7", 6);
    set_mixer_FX3(3, 2, "FX3 7", 6);
  }

  if (row == 3)
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
      allTracks[trackn]->mixDryPot = constrain(allTracks[trackn]->mixDryPot + encoded[XPos], 0, MIDI_CC_RANGE);
      allTracks[trackn]->mixDry = (float)(allTracks[trackn]->mixDryPot / MIDI_CC_RANGE_FLOAT);
      for (int i = 0; i < NUM_PLUGINS; i++)
      {
        if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_1 + i)
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
      allTracks[trackn]->mixFX1Pot = constrain(allTracks[trackn]->mixFX1Pot + encoded[XPos], 0, MIDI_CC_RANGE);
      allTracks[trackn]->mixFX1 = (float)(allTracks[trackn]->mixFX1Pot / MIDI_CC_RANGE_FLOAT);
      for (int i = 0; i < NUM_PLUGINS; i++)
      {
        if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_1 + i)
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
      allTracks[trackn]->mixFX2Pot = constrain(allTracks[trackn]->mixFX2Pot + encoded[XPos], 0, MIDI_CC_RANGE);
      allTracks[trackn]->mixFX2 = (float)(allTracks[trackn]->mixFX2Pot / MIDI_CC_RANGE_FLOAT);
      for (int i = 0; i < NUM_PLUGINS; i++)
      {
        if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_1 + i)
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

      allTracks[trackn]->mixFX3Pot = constrain(allTracks[trackn]->mixFX3Pot + encoded[XPos], 0, MIDI_CC_RANGE);
      allTracks[trackn]->mixFX3 = (float)(allTracks[trackn]->mixFX3Pot / MIDI_CC_RANGE_FLOAT);
      for (int i = 0; i < NUM_PLUGINS; i++)
      {
        if (allTracks[trackn]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_1 + i)
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

    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 0, t, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 1, t, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 2, t, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 3, t, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, allTracks[t]->mixDryPot / 42, t, TRELLIS_PINK);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 4, t, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 5, t, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 6, t, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 7, t, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, allTracks[t]->mixFX1Pot / 42 + 4, t, TRELLIS_OLIVE);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 8, t, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 9, t, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 10, t, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 11, t, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, allTracks[t]->mixFX2Pot / 42 + 8, t, TRELLIS_AQUA);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 12, t, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 13, t, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 14, t, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 15, t, TRELLIS_BLACK);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, allTracks[t]->mixFX3Pot / 42 + 12, t, TRELLIS_ORANGE);
  }
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
    note_frequency[r] = pow(2.0, ((double)(r - SAMPLE_ROOT) / 12.0));
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