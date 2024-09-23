#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <ILI9341_t3n.h>
#include <ili9341_t3n_font_Arial.h> // from ILI9341_t3
#include <MIDI.h>
#include <USBHost_t36.h>
#include <Adafruit_MCP4728.h>

#include "projectVariables.h"

#include "input/encoder.h"
#include "input/trellis.h"
#include "input/mouse.h"

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

PluginControll *allPlugins[NUM_PLUGINS] = {&plugin_1, &plugin_2, &plugin_3, &plugin_4, &plugin_5, &plugin_6, &plugin_7, &plugin_8, &plugin_9, &plugin_10, &plugin_11};
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
void input_behaviour();
// midi
void clock_to_notes(int _tick);
void midi_setup(byte dly);
void midi_read();
void sendClock();

void sendNoteOn(byte Note, byte Velo, byte Channel);
void sendNoteOff(byte Note, byte Velo, byte Channel);
void sendControlChange(byte control, byte value, byte Channel);

void myNoteOn(byte channel, byte note, byte velocity);
void myNoteOff(byte channel, byte note, byte velocity);
void detect_USB_device();
// some trellis
void trellis_show_tft_mixer();
void trellis_play_mixer();
void trellis_perform();

// mixer
void set_mixer(byte row);
void set_mixer_gain(byte XPos, byte YPos, const char *name, byte trackn);
void set_mixer_FX_page1(byte row);
void set_mixer_FX_page2(byte row);
void set_mixer_dry(byte XPos, byte YPos, const char *name, byte trackn);
void set_mixer_FX1(byte XPos, byte YPos, const char *name, byte trackn);
void set_mixer_FX2(byte XPos, byte YPos, const char *name, byte trackn);
void set_mixer_FX3(byte XPos, byte YPos, const char *name, byte trackn);

void setup()
{
  // while (!Serial)
  {
    /* code */
  }
  Serial.begin(115200);
  Wire1.begin();
  tft_setup(100);
  encoder_setup(100);
  trellis_setup(100);
  neotrellis_setup(100);
  dac_setup(100);
  Serial.println("neotrellis Setup done");
  delay(1500);
  midi_setup(100);

  myClock.setup();
  AudioMemory(320);
  MasterOut.setup();
  Serial.println("Audio & MIDI Setup done");
  note_frequency = new float[128];
  for (int r = 0; r < 128; r++)
  {
    note_frequency[r] = pow(2.0, ((double)(r - SAMPLE_ROOT) / 12.0));
  }
  startUpScreen();
  tft.updateScreenAsync();
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

  for (int i = 0; i < NUM_TRACKS; i++)
  {
    allTracks[i]->update(pixelTouchX, gridTouchY);
    trellis_show_clockbar(i, allTracks[i]->internal_clock / 6);
  }
  get_infobox_background();
  unsigned long loopEndTime = millis();
  unsigned long neotrellisCurrentMillis = millis();
  // if we need to restart the trellisboard
  if (neotrellisCurrentMillis - neotrellisReadPreviousMillis >= neotrellisReadInterval)
  {
    trellisReadPreviousMillis = neotrellisCurrentMillis;

    neotrellis_recall_control_buffer();
    neotrellis_show();
    //trellis_recall_main_buffer(trellisScreen);
    trellis_writeDisplay();
    if (activeScreen == INPUT_FUNCTIONS_FOR_ARRANGER)
    {
      mouse(2, 14);
      moveCursor(pixelTouchX, gridTouchY, 1, TRACK_FRAME_H);
    }
    else
    { // Serial.printf("should move cursor x= %d, y=%d\n", pixelTouchX, gridTouchY);
      mouse(2, 14);
      moveCursor(pixelTouchX, gridTouchY, 1, STEP_FRAME_H);
    }
    // tftUpdate(pixelTouchX, gridTouchY);
    // tftUpdateClock(myClock.barTick, myClock.barTick, myClock.startOfLoop, myClock.endOfLoop);
  }
  if (updateTFTScreen)
  {
    tft_show();
    // trellis_recall_main_buffer(trellisScreen);

    updateTFTScreen = false;
    // trellis.writeDisplay();
  }

  if (loopEndTime - loopStartTime > 500 /*|| trellisCurrentMillis - trellisRestartPreviousMillis >= trellisRestartInterval*/)
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
  neotrellis_start_clock();
  neotrellis_stop_clock();
  neotrellis_set_potRow();
  // if we are in one of the sequencer pages
  if (activeScreen == INPUT_FUNCTIONS_FOR_SEQUENCER)
  {

    neotrellis_SetCursor(14);

    if (neotrellisPressed[TRELLIS_POTROW])
    {
      change_plugin_row = true;
      draw_stepSequencer_parameters(lastPotRow);
      neotrellisPressed[TRELLIS_POTROW] = false;
    }
    allTracks[active_track]->set_stepSequencer_parameters(lastPotRow);
  }
  // if we are in one of the Arrangerpages
  if (activeScreen == INPUT_FUNCTIONS_FOR_ARRANGER)
  {

    neotrellis_SetCursor(8);

    allTracks[gridTouchY - 1]->set_arranger_parameters(lastPotRow);
    if (neotrellisPressed[TRELLIS_POTROW])
    {
      draw_arranger_parameters(lastPotRow);
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
      /* for (int i = 0; i < NUM_TRACKS; i++)
       {
           allTracks[i]->set_seq_mode_parameters(lastPotRow);
       }*/
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
  // if (activeScreen == INPUT_FUNCTIONS_FOR_PERFORM)
  //  trellisMain->set_perform_page(lastPotRow);
}
// midi
void clock_to_notes(int _tick)
{

  // Serial.println(Masterclock.MIDItick);
  for (int t = 0; t < NUM_TRACKS; t++)
  {
    allTracks[t]->play_sequencer_mode(_tick, myClock.startOfLoop, myClock.endOfLoop);
  }
  // Serial.printf("LoopStart: %d, LoopEnd: %d\n", myClock.startOfLoop, myClock.endOfLoop);
}
void midi_setup(byte dly)
{
  Serial.println("Initializing MIDI");
  tft.println("Initializing MIDI");
  tft.updateScreenAsync();

  myusb.begin();
  usbMIDI.begin(); // Launch MIDI
  MIDI1.begin();

  MIDI1.setHandleNoteOn(myNoteOn);
  MIDI1.setHandleNoteOff(myNoteOff);
  usbMidi1.setHandleNoteOn(myNoteOn);
  usbMidi1.setHandleNoteOff(myNoteOff);
  usbMIDI.setHandleNoteOff(myNoteOff);
  usbMIDI.setHandleNoteOn(myNoteOn);
  delay(dly);
}
void midi_read()
{
  usbMIDI.read();
  MIDI1.read();
  myusb.Task();
  usbMidi1.read();
  detect_USB_device();
}
void sendClock()
{
  MIDI1.sendClock();
  usbMIDI.sendRealTime(usbMIDI.Clock);
  usbMidi1.sendRealTime(usbMIDI.Clock);
}

void sendNoteOn(byte Note, byte Velo, byte Channel)
{
  if (Note < NO_NOTE)
  {
    if (Channel > 0 && Channel <= 16)
      MIDI1.sendNoteOn(Note, Velo, Channel);
    if (Channel > 16 && Channel <= 32)
      usbMIDI.sendNoteOn(Note, Velo, Channel - 16);
    if (Channel > 32 && Channel <= 48)
      usbMidi1.sendNoteOn(Note, Velo, Channel - 32);
    if (Channel > 48 && Channel <= 48 + NUM_PLUGINS)
      MasterOut.noteOn(Note, Velo, Channel - (48 + 1), Note % 12);
    // Serial.printf("Note ON: channel:%d, Note: %d, Velo: %d\n", Channel, Note, Velo);
  }
}
void sendNoteOff(byte Note, byte Velo, byte Channel)
{

  if (Channel > 0 && Channel <= 16)
    MIDI1.sendNoteOff(Note, Velo, Channel);
  if (Channel > 16 && Channel <= 32)
    usbMIDI.sendNoteOff(Note, Velo, Channel - 16);
  if (Channel > 32 && Channel <= 48)
    usbMidi1.sendNoteOff(Note, Velo, Channel - 32);
  if (Channel > 48 && Channel <= 48 + NUM_PLUGINS)
    MasterOut.noteOff(Note, Velo, Channel - (48 + 1), 0);
}
void sendControlChange(byte control, byte value, byte Channel)
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

void myNoteOn(byte channel, byte note, byte velocity)
{
  if (channel < 9 && !allTracks[channel - 1]->muted)
  {
    allTracks[channel - 1]->noteOn(note, velocity, allTracks[channel - 1]->parameter[SET_MIDICH_OUT]);
    if (allTracks[channel - 1]->get_recordState())
      allTracks[channel - 1]->record_noteOn(note, velocity, allTracks[channel - 1]->parameter[SET_MIDICH_OUT]);
  }
  if (channel >= 9)
    sendNoteOn(note, velocity, channel);
  Serial.printf("note: %d, velo: %d, channel: %d\n ", note, velocity, channel);
}
void myNoteOff(byte channel, byte note, byte velocity)
{
  if (channel < 9 && !allTracks[channel - 1]->muted)
  {
    allTracks[channel - 1]->noteOff(note, velocity, allTracks[channel - 1]->parameter[SET_MIDICH_OUT]);
    if (allTracks[channel - 1]->get_recordState())
      allTracks[channel - 1]->record_noteOff(note, velocity, allTracks[channel - 1]->parameter[SET_MIDICH_OUT]);
  }
  if (channel >= 9)
    sendNoteOff(note, velocity, channel);
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

// some trellis
void trellis_show_tft_mixer()
{
  if (neotrellisPressed[TRELLIS_BUTTON_MIXER])
  {
    if (trellisScreen != TRELLIS_SCREEN_SONGPAGE_SELECTION)
    {
      trellisScreen = TRELLIS_SCREEN_SONGPAGE_SELECTION;
      trellis.clear();
      for (int i = 0; i < 7; i++)
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
      trellis.writeDisplay();
      trellisPressed[1] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_MIXER2;
      clearWorkSpace();
      draw_mixer_FX_page1();
      show_active_page_info("FX Vol", 1);
      //trellis_recall_main_buffer(TRELLIS_SCREEN_MIXER);
    }
    if (trellisPressed[2])
    {
      trellis.clear();
      trellis.writeDisplay();
      trellisPressed[2] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_MIXER3;
      clearWorkSpace();
      draw_mixer_FX_page2();
      show_active_page_info("FX Vol", 2);
      //trellis_recall_main_buffer(TRELLIS_SCREEN_MIXER);
    }
    if (trellisPressed[3])
    {
      trellis.clear();
      trellis.writeDisplay();
      trellisPressed[3] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_FX1;
      clearWorkSpace();
      fx_1.draw_plugin();
      show_active_page_info("FX Ctrl", 1);

      //trellis_recall_main_buffer(TRELLIS_SCREEN_MIXER);
    }
    if (trellisPressed[4])
    {
      trellis.clear();
      trellis.writeDisplay();
      trellisPressed[4] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_FX2;
      clearWorkSpace();
      fx_2.draw_plugin();
      show_active_page_info("FX Ctrl", 2);

      //trellis_recall_main_buffer(TRELLIS_SCREEN_MIXER);
    }
    if (trellisPressed[5])
    {
      trellis.clear();
      trellis.writeDisplay();
      trellisPressed[5] = false;
      trellisScreen = TRELLIS_SCREEN_MIXER;
      neotrellisPressed[TRELLIS_BUTTON_MIXER] = false;
      change_plugin_row = true;
      activeScreen = INPUT_FUNCTIONS_FOR_FX3;
      clearWorkSpace();
      fx_3.draw_plugin();
      show_active_page_info("FX Ctrl", 3);

      //trellis_recall_main_buffer(TRELLIS_SCREEN_MIXER);
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

      //trellis_recall_main_buffer(TRELLIS_SCREEN_PERFORM);
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
        byte _nr = s + (t * TRELLIS_PADS_X_DIM);
        if (trellisPressed[_nr])
        {
          byte _gain[NUM_STEPS] = {0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 111, 119, 127};
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

          byte _nr = s + (t * TRELLIS_PADS_X_DIM);

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
            byte _clipLength[Y_DIM]{96, 72, 60, 48, 36, 24, 12, 6};
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
            byte _stepDivision[Y_DIM]{1, 2, 3, 4, 6, 8, 12, 16};
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              if (allTracks[s]->performIsActive)
              {
                allTracks[s]->parameter[SET_STEP_DIVIVISION] = _stepDivision[t];
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
            tft.printf("Step Div =  %d ", _stepDivision[t]);
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
void set_mixer(byte row)
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
void set_mixer_gain(byte XPos, byte YPos, const char *name, byte trackn)
{

  if (enc_moved[XPos])
  {

    allTracks[trackn]->mixGainPot = constrain(allTracks[trackn]->mixGainPot + encoded[XPos], 0, MIDI_CC_RANGE);
    allTracks[trackn]->mixGain = (float)(allTracks[trackn]->mixGainPot / MIDI_CC_RANGE_FLOAT);
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
void set_mixer_FX_page1(byte row)
{
  draw_mixer_FX_page1();
  if (row == 0)
  {

    set_mixer_dry(0, 0, "Dry D", 0);
    set_mixer_dry(1, 0, "Dry 2", 1);
    set_mixer_dry(2, 0, "Dry 3", 2);
    set_mixer_dry(3, 0, "Dry 4", 3);
  }

  if (row == 1)
  {
    set_mixer_FX1(0, 1, "FX1 D", 0);
    set_mixer_FX1(1, 1, "FX1 2", 1);
    set_mixer_FX1(2, 1, "FX1 3", 2);
    set_mixer_FX1(3, 1, "FX1 4", 3);
  }

  if (row == 2)
  {
    set_mixer_FX2(0, 2, "FX2 D", 0);
    set_mixer_FX2(1, 2, "FX2 2", 1);
    set_mixer_FX2(2, 2, "FX2 3", 2);
    set_mixer_FX2(3, 2, "FX2 4", 3);
  }

  if (row == 3)
  {
    set_mixer_FX3(0, 3, "FX2 D", 0);
    set_mixer_FX3(1, 3, "FX2 2", 1);
    set_mixer_FX3(2, 3, "FX2 3", 2);
    set_mixer_FX3(3, 3, "FX2 4", 3);
  }
}
void set_mixer_FX_page2(byte row)
{
  draw_mixer_FX_page1();
  if (row == 0)
  {

    set_mixer_dry(0, 0, "Dry 5", 4);
    set_mixer_dry(1, 0, "Dry 6", 5);
    set_mixer_dry(2, 0, "Dry 7", 6);
    set_mixer_dry(3, 0, "Dry 8", 7);
  }

  if (row == 1)
  {
    set_mixer_FX1(0, 1, "FX1 5", 4);
    set_mixer_FX1(1, 1, "FX1 6", 5);
    set_mixer_FX1(2, 1, "FX1 7", 6);
    set_mixer_FX1(3, 1, "FX1 8", 7);
  }

  if (row == 2)
  {
    set_mixer_FX2(0, 2, "FX2 5", 4);
    set_mixer_FX2(1, 2, "FX2 6", 5);
    set_mixer_FX2(2, 2, "FX2 7", 6);
    set_mixer_FX2(3, 2, "FX2 8", 7);
  }

  if (row == 3)
  {
    set_mixer_FX3(0, 3, "FX3 5", 4);
    set_mixer_FX3(1, 3, "FX3 6", 5);
    set_mixer_FX3(2, 3, "FX3 7", 6);
    set_mixer_FX3(3, 3, "FX3 8", 7);
  }
}
void set_mixer_dry(byte XPos, byte YPos, const char *name, byte trackn)
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
void set_mixer_FX1(byte XPos, byte YPos, const char *name, byte trackn)
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
void set_mixer_FX2(byte XPos, byte YPos, const char *name, byte trackn)
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
void set_mixer_FX3(byte XPos, byte YPos, const char *name, byte trackn)
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
