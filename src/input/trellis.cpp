#include "input/trellis.h"
// #include "Adafruit_NeoTrellis.h"
// #include <Wire.h>

byte trellisPianoTrack;
int trellisTrackColor[9]{TRELLIS_RED, TRELLIS_PURPLE, TRELLIS_OLIVE, TRELLIS_YELLOW, TRELLIS_BLUE, 9365295, TRELLIS_AQUA, TRELLIS_GREEN, 900909};
int trellisControllBuffer[X_DIM][Y_DIM];
const long trellisReadInterval = 30;            // interval at which to blink (milliseconds)
const long neotrellisReadInterval = 30;         // interval at which to blink (milliseconds)
const long trellisRestartInterval = 60000;      // interval at which to blink (milliseconds)
unsigned long trellisRestartPreviousMillis = 0; // will store last time LED was updated
unsigned long trellisReadPreviousMillis = 0;    // will store last time LED was updated

const byte TrellisLED[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM]{0, 1, 2, 3, 16, 17, 18, 19, 32, 33, 34, 35, 48, 49, 50, 51,
                                                               4, 5, 6, 7, 20, 21, 22, 23, 36, 37, 38, 39, 52, 53, 54, 55,
                                                               8, 9, 10, 11, 24, 25, 26, 27, 40, 41, 42, 43, 56, 57, 58, 59,
                                                               12, 13, 14, 15, 28, 29, 30, 31, 44, 45, 46, 47, 60, 61, 62, 63,
                                                               64, 65, 66, 67, 80, 81, 82, 83, 96, 97, 98, 99, 112, 113, 114, 115,
                                                               68, 69, 70, 71, 84, 85, 86, 87, 100, 101, 102, 103, 116, 117, 118, 119,
                                                               72, 73, 74, 75, 88, 89, 90, 91, 104, 105, 106, 107, 120, 121, 122, 123,
                                                               76, 77, 78, 79, 92, 93, 94, 95, 108, 109, 110, 111, 124, 125, 126, 127};

int trellisMainGridBuffer[TRELLIS_MAX_PAGES][TRELLIS_PADS_X_DIM][TRELLIS_PADS_Y_DIM];

Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_Trellis matrix1 = Adafruit_Trellis();
Adafruit_Trellis matrix2 = Adafruit_Trellis();
Adafruit_Trellis matrix3 = Adafruit_Trellis();
Adafruit_Trellis matrix4 = Adafruit_Trellis();
Adafruit_Trellis matrix5 = Adafruit_Trellis();
Adafruit_Trellis matrix6 = Adafruit_Trellis();
Adafruit_Trellis matrix7 = Adafruit_Trellis();

Adafruit_TrellisSet trellis = Adafruit_TrellisSet(&matrix0, &matrix1, &matrix2, &matrix3, &matrix4, &matrix5, &matrix6, &matrix7);
// create a matrix of trellis panels
Adafruit_NeoTrellis t_array[Y_DIM / 4][X_DIM / 4] = {

    {Adafruit_NeoTrellis(0x46, &Wire1)},
    {Adafruit_NeoTrellis(0x36, &Wire1)},
    {Adafruit_NeoTrellis(0x2E, &Wire1)}

};
Adafruit_MultiTrellis neotrellis((Adafruit_NeoTrellis *)t_array, Y_DIM / 4, X_DIM / 4);

// define a callback for key presses
TrellisCallback blink(keyEvent evt)
{

  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)
  {
    Serial.printf("neotrellispressed :%d\n", evt.bit.NUM);
    neotrellisPressed[evt.bit.NUM] = true;
    updateTFTScreen = true;
    change_plugin_row = true;
  }
  else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING)
  {
    neotrellisPressed[evt.bit.NUM] = false;
  }
  // neotrellis_show();
  return 0;
}
void neotrellis_setup(int dly)
{
  pinMode(NEOTRELLIS_INT_PIN, INPUT);
  if (!neotrellis.begin())
  {
    Serial.println("DAWfailed to begin trellis");
    while (1)
      delay(1);
  }

  for (int y = 0; y < Y_DIM; y++)
  {
    for (int x = 0; x < X_DIM; x++)
    {
      // activate rising and falling edges on all keys
      neotrellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
      neotrellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);

      neotrellis.registerCallback(x, y, blink);
    }
  }
  Serial.println("neotrellis Setup done");
  // INT pin requires a pullup
  // pinMode(NEOTRELLIS_INT_PIN, INPUT);
  // digitalWrite(NEOTRELLIS_INT_PIN, HIGH);

  neotrellis_assign_start_buffer();
}
void neotrellis_assign_start_buffer()
{

  for (int i = 0; i < NUM_TRACKS; i++)
  {
    if (allTracks[i]->muted)
      neotrellis_set_control_buffer(0, i + 4, TRELLIS_BLACK);
    else
      neotrellis_set_control_buffer(0, i + 4, TRELLIS_1);
  }
  for (int i = 0; i < NUM_TRACKS; i++)
  {

    if (allTracks[i]->soloed)
      neotrellis_set_control_buffer(1, i + 4, TRELLIS_ORANGE);
    else
      neotrellis_set_control_buffer(1, i + 4, TRELLIS_1);
  }
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    if (allTracks[i]->recordState)
      neotrellis_set_control_buffer(2, i + 4, TRELLIS_RED);
    else
      // trellis.setPixelColor(nr, TRELLIS_1); // off falling
      neotrellis_set_control_buffer(2, i + 4, TRELLIS_1);
  }
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    if (allTracks[i]->performIsActive)
      neotrellis_set_control_buffer(3, i + 4, TRELLIS_WHITE);
    else
      neotrellis_set_control_buffer(3, i + 4, trellisTrackColor[i]);
  }
  neotrellis_set_control_buffer(0, 0, TRELLIS_ORANGE);
  neotrellis_set_control_buffer(1, 0, TRELLIS_RED);
  neotrellis_set_control_buffer(2, 0, TRELLIS_GREEN);
  neotrellis_set_control_buffer(3, 0, TRELLIS_OLIVE);
  neotrellis_set_control_buffer(0, 3, TRELLIS_WHITE);
  neotrellis_set_control_buffer(3, 3, trellisTrackColor[active_track]);

  trellis_assign_start_buffer(lastPotRow);
  /*
  for (int i = 0; i < NUM_TRACKS; i++)
 {
   for (int s = 0; s < NUM_STEPS; s++)
   {
      byte nr = s + (i*NUM_STEPS);
     if (trellisScreen <= TRELLIS_SCREEN_SEQUENCER_CLIP_8)
       neotrellis.setPixelColor(allTracks[i]->parameter[SET_CLIP2_EDIT], i, trellis_get_main_buffer(trellisScreen, s, i));
     else
       neotrellis.setPixelColor(s, i, trellis_get_main_buffer(trellisScreen, s, i));

     // if (trellisScreen >= TRELLIS_SCREEN_ARRANGER_1)
     // trellis.setPixelColor(s, i, trellis_get_arranger_buffer(arrangerpage, s, i));
   }
 }
 */
  neotrellis_show();
}
void neotrellis_set_control_buffer(int _x, int _y, int _color)
{
  trellisControllBuffer[_x][_y] = _color;
  neotrellis.setPixelColor(_x, _y, _color);
  neotrellis.show();
  // Serial.println("set control buffer");
}
int neotrellis_get_control_buffer(int _x, int _y)
{
  // Serial.println("get control buffer");

  return trellisControllBuffer[_x][_y];
}
void neotrellis_recall_control_buffer()
{

  for (int x = 0; x < X_DIM; x++)
  {
    for (int y = 0; y < Y_DIM; y++)
    {
      neotrellis.setPixelColor(x, y, trellisControllBuffer[x][y]);
    }
  }
  // Serial.println("recall controll buffer");

  // neotrellis_show();
}
void neo_trellis_set_brightness()
{
  // set overall brightness for all pixels
  if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
  {
    if (neotrellisPressed[TRELLIS_BUTTON_LEFT])
    {
      for (int x = 0; x < X_DIM / 4; x++)
      {
        for (int y = 0; y < Y_DIM / 4; y++)
        {
          t_array[y][x].pixels.setBrightness(5);
        }
      }
      trellis.setBrightness(0);
    }
    if (neotrellisPressed[TRELLIS_BUTTON_RIGHT])
    {
      for (int x = 0; x < X_DIM / 4; x++)
      {
        for (int y = 0; y < Y_DIM / 4; y++)
        {
          t_array[y][x].pixels.setBrightness(20);
        }
      }
      trellis.setBrightness(3);
    }
    if (neotrellisPressed[TRELLIS_BUTTON_UP])
    {
      for (int x = 0; x < X_DIM / 4; x++)
      {
        for (int y = 0; y < Y_DIM / 4; y++)
        {
          t_array[y][x].pixels.setBrightness(50);
        }
      }
      trellis.setBrightness(6);
    }
    if (neotrellisPressed[TRELLIS_BUTTON_DOWN])
    {
      for (int x = 0; x < X_DIM / 4; x++)
      {
        for (int y = 0; y < Y_DIM / 4; y++)
        {
          t_array[y][x].pixels.setBrightness(220);
        }
      }
      trellis.setBrightness(15);
    }
  }
}
void neotrellis_show()
{
  neotrellis.show();
}
void neotrellis_update()
{
  trellis_show_tft_mixer();
  neo_trellis_select_trackClips();
  neo_trellis_save_load();
  neotrellis_perform_set_active();
  neotrellis_show_tft_seqMode();
  neotrellis_show_tft_plugin();
  neotrellis_show_piano();
  neotrellis_set_mute();
  neotrellis_set_solo();
  neotrellis_set_fast_record();
  neo_trellis_set_brightness();
}
// 1st row
void neotrellis_set_potRow()
{
  if (neotrellisPressed[TRELLIS_POTROW])
  {
    neotrellisPressed[TRELLIS_POTROW] = false;
    change_plugin_row = true;
    lastPotRow++;
    if (lastPotRow >= 4)
    {
      lastPotRow = 0;
    }
    tft.fillRect(70, 0, 10, 16, ILI9341_DARKGREY);
    // neotrellisPressed[TRELLIS_POTROW] = false;
    // Serial.printf("potrwo=%d\n", lastPotRow);
  }
}
void neo_trellis_save_load()
{
  if (neotrellisPressed[TRELLIS_BUTTON_RECORD])
  {
    for (int i = 0; i < MAX_SONGS; i++)
    {
      // neotrellis.setPixelColor(i, 0, TRELLIS_ORANGE);
      // neotrellis.setPixelColor(i, 1, TRELLIS_GREEN);
      // neotrellis_show();
      trellis.setLED(TrellisLED[i]);
      trellis.setLED(TrellisLED[i + TRELLIS_PADS_X_DIM]);
      trellis.writeDisplay();
      if (trellisPressed[i])
      {
        trellis.clrLED(TrellisLED[i]);
        trellis.clrLED(TrellisLED[i + TRELLIS_PADS_X_DIM]);
        trellis.writeDisplay();

        byte _songNr = i;
        set_infobox_background(750);
        tft.printf("Saved song: %s ", songNames[i]);
        reset_infobox_background();
        allTracks[0]->save_track(_songNr);
        allTracks[1]->save_track(_songNr);
        allTracks[2]->save_track(_songNr);
        allTracks[3]->save_track(_songNr);
        allTracks[4]->save_track(_songNr);
        allTracks[5]->save_track(_songNr);
        allTracks[6]->save_track(_songNr);
        allTracks[7]->save_track(_songNr);
        myClock.save_clock(_songNr);
        trellisPressed[i] = false;
        // change_plugin_row = true;
        neotrellisPressed[TRELLIS_BUTTON_RECORD] = false;
        updateTFTScreen = true;
        break;
      }
      if (trellisPressed[i + TRELLIS_PADS_X_DIM])
      {
        byte _songNr = i;
        Serial.printf("load song: %d\n", _songNr);
        set_infobox_background(750);
        tft.printf("loaded song: %s ", songNames[_songNr]);
        reset_infobox_background();
        allTracks[0]->load_track(_songNr);
        allTracks[1]->load_track(_songNr);
        allTracks[2]->load_track(_songNr);
        allTracks[3]->load_track(_songNr);
        allTracks[4]->load_track(_songNr);
        allTracks[5]->load_track(_songNr);
        allTracks[6]->load_track(_songNr);
        myClock.load_clock(_songNr);
        neotrellis_setup(0);
        trellisPressed[i] = false;
        updateTFTScreen = true;

        neotrellisPressed[TRELLIS_BUTTON_RECORD] = false;

        break;
      }

      // allTracks[7]->load_track();
    }
  }
}
void neotrellis_start_clock()
{
  if (neotrellisPressed[TRELLIS_START_CLOCK] /*|| enc_button[2]*/)
  {
    myClock.set_start();
    // Serial.println("Play");
    neotrellisPressed[TRELLIS_START_CLOCK] = false;
  }
}
void neotrellis_stop_clock()
{
  if (neotrellisPressed[TRELLIS_STOP_CLOCK] /*|| enc_button[3]*/)
  {
    myClock.set_stop();
    // Serial.println("Stop");
    neotrellisPressed[TRELLIS_STOP_CLOCK] = false;
    for (int i = 0; i < NUM_TRACKS; i++)
    {
      allTracks[i]->internal_clock = -1;
      allTracks[i]->internal_clock_bar = -1;
      allTracks[i]->external_clock_bar = -1;
    }
  }
}

// 2nd row
void neotrellis_SetCursor(byte maxY)
{

  if (neotrellisPressed[TRELLIS_BUTTON_LEFT]) //..move the cursor left
  {
    pixelTouchX = constrain(pixelTouchX - 1, 0, 320);
    neotrellisPressed[TRELLIS_BUTTON_LEFT] = false;
  }
  if (neotrellisPressed[TRELLIS_BUTTON_RIGHT]) //..move the cursor right
  {
    pixelTouchX = constrain(pixelTouchX + 1, 0, 320);
    neotrellisPressed[TRELLIS_BUTTON_RIGHT] = false;
  }
  if (neotrellisPressed[TRELLIS_BUTTON_UP]) //..move the cursor up
  {
    gridTouchY = constrain(gridTouchY - 1, 0, maxY);
    neotrellisPressed[TRELLIS_BUTTON_UP] = false;
  }

  if (neotrellisPressed[TRELLIS_BUTTON_DOWN]) //..move the cursor down
  {
    gridTouchY = constrain(gridTouchY + 1, 0, maxY);
    neotrellisPressed[TRELLIS_BUTTON_DOWN] = false;
  }
}

// 3rd row
void neotrellis_show_piano()
{
  if (neotrellisPressed[TRELLIS_BUTTON_PIANO])
  {
    trellisScreen = TRELLIS_SCREEN_PIANO;
    for (int i = 0; i < NUM_TRACKS; i++)
    {
      if (neotrellisPressed[3 + ((i + 4) * X_DIM)])
      {
        neotrellisPressed[3 + ((i + 4) * X_DIM)] = false;
        neotrellisPressed[TRELLIS_BUTTON_PIANO] = false;
        trellisPianoTrack = i;
        int _color = TRELLIS_BLACK;
        for (int x = 0; x < NUM_STEPS; x++)
        {
          for (int y = 0; y < NUM_TRACKS; y++)
          {
            int _nr = x + (y * NUM_STEPS);
            if (_nr % 12 == 0)
            {
              _color = trellisTrackColor[trellisPianoTrack];
              if (allTracks[trellisPianoTrack]->get_recordState())
                _color = TRELLIS_RED;
            }
            else
              _color = TRELLIS_BLACK;
            trellis_set_main_buffer(TRELLIS_SCREEN_PIANO, x, y, _color);
          }
        }
        break;
      }
    }
  }
}
void trellis_play_piano()
{

  if (trellisScreen == TRELLIS_SCREEN_PIANO)
  {
    static bool _holdNote[NUM_STEPS * NUM_TRACKS];
    for (int x = 0; x < NUM_STEPS; x++)
    {
      for (int y = 0; y < NUM_TRACKS; y++)
      {
        // byte _keyNr = x + (y * TRELLIS_PADS_X_DIM);
        byte _keynote = x + (y * NUM_STEPS);
        if (trellisPressed[_keynote] && !_holdNote[_keynote])
        {
          // Serial.printf("pianoPage _keyNr:%d, _keyNote: %d\n", _keyNr, _keynote);
          _holdNote[_keynote] = true;

          allTracks[trellisPianoTrack]->noteOn(_keynote, 99, allTracks[trellisPianoTrack]->parameter[SET_MIDICH_OUT]);
          if (allTracks[trellisPianoTrack]->get_recordState())
            allTracks[trellisPianoTrack]->record_noteOn(_keynote, 99, allTracks[trellisPianoTrack]->parameter[SET_MIDICH_OUT]);
          Serial.printf("trellisPiano NoteON key:%d, track:%d\n", _keynote, trellisPianoTrack);
          Serial.println(_holdNote[_keynote]);
          break;
        }

        else if (!trellisPressed[_keynote] && _holdNote[_keynote])
        {
          _holdNote[_keynote] = false;
          allTracks[trellisPianoTrack]->noteOff(_keynote, 0, allTracks[trellisPianoTrack]->parameter[SET_MIDICH_OUT]);
          if (allTracks[trellisPianoTrack]->get_recordState())
            allTracks[trellisPianoTrack]->record_noteOff(_keynote, 0, allTracks[trellisPianoTrack]->parameter[SET_MIDICH_OUT]);
          Serial.printf("trellisPiano NoteOff key:%d, track:%d\n", _keynote, trellisPianoTrack);
          break;
        }
      }
    }
  }
}
void neotrellis_show_tft_seqMode()
{
  if (neotrellisPressed[TRELLIS_BUTTON_SEQMODE])
  {
    for (int i = 0; i < NUM_TRACKS; i++)
    {
      if (neotrellisPressed[3 + ((i + 4) * X_DIM)])
      {
        neotrellisPressed[3 + ((i + 4) * X_DIM)] = false;
        active_track = i;
        show_active_page_info("Track", active_track);
        allTracks[active_track]->draw_sequencer_modes(allTracks[active_track]->parameter[SET_SEQ_MODE]);
        activeScreen = INPUT_FUNCTIONS_FOR_SEQUENCER_MODES;
        neotrellis_set_control_buffer(3, 2, trellisTrackColor[active_track]);
        // Serial.println("SeqMode selected");
        // trellis_set_main_buffer(arrangerpage + TRELLIS_SCREEN_ARRANGER_1, 3, 2, trellisTrackColor[active_track]);
        break;
      }
    }
  }
}

// 4th row
void trellis_show_arranger()
{
  if (neotrellisPressed[TRELLIS_BUTTON_ARRANGER])
  {
    if (trellisScreen != TRELLIS_SCREEN_SONGPAGE_SELECTION)
    {
      trellisScreen = TRELLIS_SCREEN_SONGPAGE_SELECTION;
      trellis.clear();
      for (int i = 0; i < NUM_STEPS; i++)
      {
        trellis.setLED(TrellisLED[i]);
        trellis.writeDisplay();
      }
    }
    for (int i = 0; i < NUM_STEPS; i++)
    {
      if (trellisPressed[i])
      {
        trellis.clear();
        trellis.writeDisplay();

        trellisPressed[i] = false;
        neotrellisPressed[TRELLIS_BUTTON_ARRANGER] = false;
        trellisScreen = i + TRELLIS_SCREEN_ARRANGER_1;
        arrangerpage = i;

        activeScreen = INPUT_FUNCTIONS_FOR_ARRANGER;
        // trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, i, 0, TRELLIS_BLACK);
        //  neotrellis_show();
        //   Serial.printf("songpage= %d\n", _key);
        gridSongMode(arrangerpage);
        Serial.println("try to recall");
        trellis_recall_main_buffer(trellisScreen);
        break;
      }
    }
  }
}
void trellis_set_arranger()
{
  if (trellisScreen >= TRELLIS_SCREEN_ARRANGER_1 && !neotrellisPressed[TRELLIS_BUTTON_ARRANGER])
  {
    if (!neotrellisPressed[TRELLIS_BUTTON_RECORD])
    {
      for (int t = 0; t < NUM_TRACKS; t++)
      {
        if (neotrellisPressed[3 + (X_DIM * (t + 4))])
        {
          // if (temporaryTrellis)
          {

            trellis.clear();
            Serial.printf("set trellis arranger neotrellis pressed\n");
            for (int x = 0; x < TRELLIS_PADS_X_DIM; x++)
            {
              byte _nr = x + (allTracks[t]->clip_to_play[x + (arrangerpage * 16)] * TRELLIS_PADS_X_DIM);
              trellis.setLED(TrellisLED[_nr]);
            }
            trellis.writeDisplay();
          }
          for (int x = 0; x < TRELLIS_PADS_X_DIM; x++)
          {
            for (int y = 0; y < TRELLIS_PADS_Y_DIM; y++)
            {
              byte _key = x + (y * TRELLIS_PADS_X_DIM);
              // pixelTouchX = (_key % TRELLIS_PADS_X_DIM)*16+32;

              if (trellisPressed[_key])
              {
                trellisScreen = arrangerpage + TRELLIS_SCREEN_ARRANGER_1;
                trellis.clear();
                trellis_recall_main_buffer(trellisScreen);
                trellis.writeDisplay();

                byte _clipPos = x;
                byte _track = t;
                byte _clipNr = y;
                byte _bar = _clipPos + (arrangerpage * 16);
                neotrellisPressed[3 + (X_DIM * (t + 4))] = false;
                trellisPressed[_key] = false;
                change_plugin_row = true;
                Serial.printf("Set trellis arranger track: %d, bar: %d, clipNr: %d\n", _track, _bar, _clipNr);
                for (int i = 0; i < allTracks[_track]->parameter[SET_STEP_DIVIVISION]; i++)
                {
                  allTracks[_track]->set_clip_to_play_trellis(_bar, _clipNr);
                }
                break;
              }
            }
          }
        }
        else
        {
          for (int x = 0; x < TRELLIS_PADS_X_DIM; x++)
          {
            for (int y = 0; y < TRELLIS_PADS_Y_DIM; y++)
            {
              byte _key = x + (y * TRELLIS_PADS_X_DIM);
              // pixelTouchX = (_key % TRELLIS_PADS_X_DIM)*16+32;

              if (trellisPressed[_key])
              {

                byte _clipPos = x;
                byte _track = t;
                byte _clipNr = gridTouchY;
                byte _bar = _clipPos + (arrangerpage * 16);
                neotrellisPressed[3 + (X_DIM * (t + 4))] = false;
                trellisPressed[_key] = false;
                change_plugin_row = true;
                Serial.printf("Set trellis arranger track: %d, bar: %d, clipNr: %d\n", _track, _bar, _clipNr);
                for (int i = 0; i < allTracks[_track]->parameter[SET_STEP_DIVIVISION]; i++)
                {
                  allTracks[_track]->set_clip_to_play_trellis(_bar, _clipNr);
                }
              }
            }
          }
        }
      }
    }
  }
}

void draw_perform_page()
{
  if (change_plugin_row)
  {
    change_plugin_row = false;
    for (int i = 0; i < NUM_STEPS; i++)
    {
      drawPot(i % 4, i / 4, performCC[i], "CC");
      trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, i, 7, TRELLIS_BLACK);
    }
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, lastPotRow * 4, 7, TRELLIS_BLUE);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, lastPotRow * 4 + 1, 7, TRELLIS_RED);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, lastPotRow * 4 + 2, 7, TRELLIS_GREEN);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, lastPotRow * 4 + 3, 7, TRELLIS_WHITE);
  }
}
void neotrellis_perform_set_active()
{
  if (trellisScreen == TRELLIS_SCREEN_PERFORM)
  {

    for (int x = 0; x < NUM_STEPS; x++)
    {
      for (int y = 0; y < NUM_TRACKS; y++)
      {
        trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, x, y, TRELLIS_BLACK);
        // trellis.clrLED(TrellisLED[x + (y * TRELLIS_PADS_X_DIM)]);
        // byte _nr = x + (trellisPerformIndex[x] * TRELLIS_PADS_X_DIM);
        // if (trellisPressed[_nr]){
        trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, x, trellisPerformIndex[x], TRELLIS_WHITE);
        // break;}
        //  trellis.setLED(TrellisLED[_nr]);
      }
    }
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, lastPotRow * 4, 7, TRELLIS_BLUE);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, lastPotRow * 4 + 1, 7, TRELLIS_RED);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, lastPotRow * 4 + 2, 7, TRELLIS_GREEN);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, lastPotRow * 4 + 3, 7, TRELLIS_WHITE);
    for (int t = 0; t < NUM_TRACKS; t++)
    {
      if (neotrellisPressed[3 + ((t + 4) * X_DIM)])
      {

        if (!allTracks[t]->performIsActive)
        {
          neotrellis_set_control_buffer(3, t + 4, TRELLIS_WHITE);
          allTracks[t]->performIsActive = true;
          neotrellisPressed[3 + ((t + 4) * X_DIM)] = false;
          /*
          for (int i = 0; i < NUM_PLUGINS; i++)
          {
            if (allTracks[t]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_1 + i)
            {
              fx_1.pl[i].gain(1);
              fx_2.pl[i].gain(1);
              fx_3.pl[i].gain(1);
            }
          }
          */
          break;
        }
        else if (allTracks[t]->performIsActive)
        {
          neotrellis_set_control_buffer(3, t + 4, trellisTrackColor[t]);
          allTracks[t]->performIsActive = false;
          neotrellisPressed[3 + ((t + 4) * X_DIM)] = false;
          /*
          for (int i = 0; i < NUM_PLUGINS; i++)
          {
            if (allTracks[t]->parameter[SET_MIDICH_OUT] == CH_PLUGIN_1 + i)
            {
              fx_1.pl[i].gain(0);
              fx_2.pl[i].gain(0);
              fx_3.pl[i].gain(0);
            }
          }
          */
          // Serial.println("set unmute");
          break;
        }
      }
    }
  }
}
void set_performCC(byte XPos, byte YPos, const char *name)
{
  if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
  {
    if (enc_moved[XPos])
    {
      byte _nr = XPos + (YPos * 4);
      performCC[_nr] = constrain(performCC[_nr] + encoded[XPos], 0, 128);
      drawPot(XPos, YPos, performCC[_nr], name);
    }
  }
}
void set_perform_page(byte row)
{
  draw_perform_page();
  if (row == 0)
  {

    set_performCC(0, 0, "CC");
    set_performCC(1, 0, "CC");
    set_performCC(2, 0, "CC");
    set_performCC(3, 0, "CC");
  }

  if (row == 1)
  {
    set_performCC(0, 1, "CC");
    set_performCC(1, 1, "CC");
    set_performCC(2, 1, "CC");
    set_performCC(3, 1, "CC");
  }

  if (row == 2)
  {
    set_performCC(0, 2, "CC");
    set_performCC(1, 2, "CC");
    set_performCC(2, 2, "CC");
    set_performCC(3, 2, "CC");
  }

  if (row == 3)
  {
    set_performCC(0, 3, "CC");
    set_performCC(1, 3, "CC");
    set_performCC(2, 3, "CC");
    set_performCC(3, 3, "CC");
  }
}
void neotrellis_show_tft_plugin()
{
  if (neotrellisPressed[TRELLIS_BUTTON_PLUGIN])
  {
    for (int i = 0; i < NUM_TRACKS; i++)
    {
      if (neotrellisPressed[3 + ((i + 4) * X_DIM)])
      {
        neotrellisPressed[3 + ((i + 4) * X_DIM)] = false;
        active_track = i;
        show_active_page_info("Track", i);
        change_plugin_row = true;
        // allTracks[active_track]->draw_MIDI_CC_screen();
        activeScreen = INPUT_FUNCTIONS_FOR_PLUGIN;
        neotrellis_set_control_buffer(2, 3, trellisTrackColor[active_track]);
        break;
      }
    }
  }
}
void neo_trellis_select_trackClips()
{
  if (neotrellisPressed[TRELLIS_BUTTON_SEQUENCER])
  { // byte _activeClip=allTracks[y]->parameter[SET_CLIP2_EDIT];
    trellisScreen = TRELLIS_SCREEN_SONGPAGE_SELECTION;
    trellis.clear();

    for (int y = 0; y < NUM_TRACKS; y++)
    {
      trellis.setLED(TrellisLED[allTracks[y]->parameter[SET_CLIP2_EDIT] + (y * TRELLIS_PADS_X_DIM)]);
    }
    trellis.writeDisplay();
    // neotrellis_show();
    for (int y = 0; y < NUM_TRACKS; y++)
    {
      if (neotrellisPressed[3 + (X_DIM * (y + 4))])
      {
        trellis.clear();
        trellis.writeDisplay();
        neotrellisPressed[TRELLIS_BUTTON_SEQUENCER] = false;
        neotrellisPressed[3 + (X_DIM * (y + 4))] = false;
        // trellis_set_main_buffer(TRELLIS_SCREEN_SONGPAGE_SELECTION, x, y, TRELLIS_BLACK);
        //  neotrellis_show();
        active_track = y;
        show_active_page_info("Track", active_track);

        // updateTFTScreen = true;
        change_plugin_row = true;
        activeScreen = INPUT_FUNCTIONS_FOR_SEQUENCER;
        trellisScreen = allTracks[active_track]->parameter[SET_CLIP2_EDIT];
        //for (int i = 0; i < NUM_PARAMETERS; i++)
          drawStepSequencerStatic();
        draw_stepSequencer_parameters(lastPotRow);
        draw_notes_in_grid();
        neotrellis_set_control_buffer(3, 3, trellisTrackColor[active_track]);
        trellis_recall_main_buffer(trellisScreen);
        break;
      }
      for (int x = 0; x <= NUM_USER_CLIPS; x++)
      {
        if (trellisPressed[x + (TRELLIS_PADS_X_DIM * y)])
        {
          trellis.clear();
          trellis.writeDisplay();
          neotrellisPressed[TRELLIS_BUTTON_SEQUENCER] = false;
          trellisPressed[x + (TRELLIS_PADS_X_DIM * y)] = false;
          // neotrellis_show();
          active_track = y;
          show_active_page_info("Track", active_track);

          activeScreen = INPUT_FUNCTIONS_FOR_SEQUENCER;
          allTracks[y]->parameter[SET_CLIP2_EDIT] = x;
          // updateTFTScreen = true;
          //for (int i = 0; i < NUM_PARAMETERS; i++)
            change_plugin_row = true;
          drawStepSequencerStatic();
          draw_stepSequencer_parameters(lastPotRow);
          draw_notes_in_grid();
          neotrellis_set_control_buffer(3, 3, trellisTrackColor[active_track]);

          trellisScreen = allTracks[active_track]->parameter[SET_CLIP2_EDIT];
          trellis_recall_main_buffer(trellisScreen);
          // trellisScreen = 0;
          break;
        }
      }
    }
  }
}
void trellis_setStepsequencer()
{
  if (trellisScreen < TRELLIS_SCREEN_SEQUENCER_CLIP_8)
  {
    if (!neotrellisPressed[TRELLIS_BUTTON_RECORD])
    {

      for (int x = 0; x < NUM_STEPS; x++)
      {
        for (int y = 0; y < NUM_TRACKS; y++)
        {
          byte _nr = x + (y * TRELLIS_PADS_X_DIM);
          if (trellisPressed[_nr])
          {
            byte trellisNote;
            byte track = _nr / NUM_STEPS;
            byte step = _nr % NUM_STEPS;
            int keyTick = (step * 6);
            // gridTouchY = 1;
            // neotrellisPressed[TRELLIS_BUTTON_ENTER] = true;
            if (gridTouchY > 0 && gridTouchY <= 12)
            {

              trellisNote = gridTouchY;
            }
            else
              trellisNote = 1;
            trellisPressed[_nr] = false;
            // updateTFTScreen = true;
            change_plugin_row = true;
            neotrellisPressed[TRELLIS_BUTTON_SEQUENCER] = false;
            allTracks[track]->set_note_on_tick(keyTick, trellisNote);
            Serial.printf("step: %d, tick: %d, track: %D \n", step, keyTick, track);
            /*
            for (int v = 0; v < MAX_VOICES; v++)
            {
              if (allTracks[_key / TRELLIS_PADS_X_DIM]->get_active_note(allTracks[track]->parameter[SET_CLIP2_EDIT], keyTick, v) < NO_NOTE)
              {
                trellis_set_buffer(_key, trellisTrackColor[track]);
                break;
              }
              else if (allTracks[_key / TRELLIS_PADS_X_DIM]->get_active_note(allTracks[_key / TRELLIS_PADS_X_DIM]->parameter[SET_CLIP2_EDIT], keyTick, v) == NO_NOTE)
              {
                trellis_set_buffer(_key, TRELLIS_BLACK);
                break;
              }
            }
*/
          }
        }
      }
    }
  }
}

// colums
void neotrellis_set_mute()
{
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    if (neotrellisPressed[((i + 4) * X_DIM)])
    {
      neotrellisPressed[((i + 4) * X_DIM)] = false;
      if (!allTracks[i]->muted)
      {
        neotrellis.setPixelColor(0, (i + 4), TRELLIS_BLACK);
        allTracks[i]->muted = true;
        Serial.print("set mute");
        Serial.println(i);
        break;
      }
      else if (allTracks[i]->muted)
      {
        neotrellis.setPixelColor(0, (i + 4), TRELLIS_1);
        allTracks[i]->muted = false;
        Serial.print("set unmute");
        Serial.println(i);
        break;
      }
    }
  }
}
void neotrellis_set_solo()
{
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    if (neotrellisPressed[1 + ((i + 4) * X_DIM)])
    {
      neotrellisPressed[1 + ((i + 4) * X_DIM)] = false;
      if (!allTracks[i]->soloed)
      {
        neotrellis.setPixelColor(1, i + 4, TRELLIS_ORANGE);
        allTracks[i]->soloed = true;
        allTracks[i]->muteThruSolo = false;
        for (int o = 0; o < NUM_TRACKS; o++)
        {

          if (!allTracks[o]->soloed && o != i)
          {
            allTracks[o]->muteThruSolo = true;
          }
        }
        // Serial.println("set solo");
        break;
      }
      else if (allTracks[i]->soloed)
      {
        neotrellis.setPixelColor(1, i + 4, TRELLIS_1);
        allTracks[i]->soloed = false;
        for (int o = 0; o < NUM_TRACKS; o++)
        {
          if (!allTracks[o]->soloed && o != i)
          {
            allTracks[o]->muteThruSolo = false;
          }
        }
        for (int o = 0; o < NUM_TRACKS; o++)
        {
          if (allTracks[o]->soloed)
          {
            for (int s = 0; s < NUM_TRACKS; s++)
            {
              allTracks[s]->muteThruSolo = true;
            }
            allTracks[o]->muteThruSolo = false;
          }
        }
        // Serial.println("set unsolo");
        break;
      }
    }
  }
}
void neotrellis_set_fast_record()
{
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    if (neotrellisPressed[2 + ((i + 4) * X_DIM)])
    {
      neotrellisPressed[2 + ((i + 4) * X_DIM)] = false;
      if (!allTracks[i]->recordState)
      {
        allTracks[i]->recordState = true;
        neotrellis.setPixelColor(2, i + 4, TRELLIS_RED);
        // trellis.setPixelColor(18, i, TRELLIS_RED);
        break;
      }

      else if (allTracks[i]->recordState)
      {
        allTracks[i]->recordState = false;
        neotrellis.setPixelColor(2, (i + 4), TRELLIS_1);
        // trellis.setPixelColor(18, i, TRELLIS_1);
        break;
      }

      // break;
    }
  }
}

void trellis_setup(int dly)
{
  pinMode(TRELLIS_INT_PIN, INPUT);
  digitalWrite(TRELLIS_INT_PIN, HIGH);
  trellis.begin(0x70, 0x72, 0x73, 0x74, 0x71, 0x75, 0x76, 0x77);
  Serial.println("trellis Setup done");
  delay(dly);
}
void trellis_read()
{

  // If a button was just pressed or released...
  if (trellis.readSwitches())
  {
    // go through every button
    for (uint8_t i = 0; i < numKeys; i++)
    {
      // if it was pressed, turn it on
      if (trellis.justPressed(TrellisLED[i]))
      {
        /*
        int _nr = (i % 4) + 4 * (i / 16) + (i / 4 % 4) * 16;
        if (i > 63)
        {
          _nr = _nr + 48;
        }*/
        int _nr = i;

        updateTFTScreen = true;
        // change_plugin_row = true;
        trellisPressed[_nr] = true;
        Serial.print("nr");
        Serial.println(_nr);
        // trellis.setLED(i);
        break;
      }
      // if it was released, turn it off
      if (trellis.justReleased(TrellisLED[i]))
      {
        int _nr = i;
        trellisPressed[_nr] = false;
      }
    }
  }
}

void trellis_update()
{
  trellis_play_piano();
  trellis_show_arranger();
  unsigned long trellisCurrentMillis = millis();
  if (trellisCurrentMillis - trellisReadPreviousMillis >= trellisReadInterval)
  {
    trellisReadPreviousMillis = trellisCurrentMillis;
    trellis_read();
    // trellis_recall_main_buffer(trellisScreen);
    trellis_set_arranger();
    trellis_perform();
    trellis_setStepsequencer();
    trellis_play_mixer();

    trellis_writeDisplay();
  }
}
void trellis_writeDisplay()
{
  // tell the trellis to set the LEDs we requested
  trellis.writeDisplay();
}

void trellis_assign_start_buffer(byte _lastPotRow)
{
  for (int i = 0; i < TRELLIS_PADS_Y_DIM - 1; i++)
  {
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 0, i, TRELLIS_TEAL);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 1, i, TRELLIS_PINK);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 2, i, TRELLIS_OLIVE);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 3, i, TRELLIS_AQUA);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 4, i, TRELLIS_ORANGE);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 5, i, TRELLIS_OLIVE);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 6, i, TRELLIS_OLIVE);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 7, i, TRELLIS_AQUA);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 8, i, TRELLIS_AQUA);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 9, i, TRELLIS_ORANGE);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 10, i, TRELLIS_ORANGE);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 11, i, TRELLIS_PINK);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 12, i, TRELLIS_PINK);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 13, i, TRELLIS_BLUE);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 14, i, TRELLIS_RED);
    trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, 15, i, TRELLIS_GREEN);

    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 3, i, TRELLIS_PINK);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 7, i, TRELLIS_OLIVE);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 11, i, TRELLIS_AQUA);
    trellis_set_main_buffer(TRELLIS_SCREEN_MIXER, 15, i, TRELLIS_ORANGE);
  }

  trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, _lastPotRow * 4, 7, TRELLIS_BLUE);
  trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, _lastPotRow * 4 + 1, 7, TRELLIS_RED);
  trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, _lastPotRow * 4 + 2, 7, TRELLIS_GREEN);
  trellis_set_main_buffer(TRELLIS_SCREEN_PERFORM, _lastPotRow * 4 + 3, 7, TRELLIS_WHITE);

  // neotrellis_recall_control_buffer();
  Serial.println("trellis setup done");
}
void trellis_set_main_buffer(int _page, int _x, int _y, int color)
{
  trellisMainGridBuffer[_page][_x][_y] = color;
  int _nr = _x + (_y * TRELLIS_PADS_X_DIM);
  trellis.setLED(TrellisLED[_nr]);
  if (trellisMainGridBuffer[_page][_x][_y] == 0)
    trellis.clrLED(TrellisLED[_nr]);
  Serial.printf("set main buffer page: %d, x: %d, y: %d, color: %d, trellisNr: %d\n", _page, _x, _y, color, TrellisLED[_nr]);
}
void trellis_recall_main_buffer(int _page)
{

  for (int i = 0; i < TRELLIS_PADS_X_DIM; i++)
  {
    for (int y = 0; y < TRELLIS_PADS_Y_DIM; y++)
    {
      // if (trellisScreen <= TRELLIS_SCREEN_SEQUENCER_CLIP_8)
      {
        int _nr = i + (y * TRELLIS_PADS_X_DIM);

        if (trellisMainGridBuffer[_page][i][y] > 0)
          trellis.setLED(TrellisLED[_nr]);
        else
          trellis.clrLED(TrellisLED[_nr]);
        // Serial.printf("recall main buffer page: %d, x: %d, y: %d, color: %d, trellisNr: %d\n", _page, i, y, trellisMainGridBuffer[_page][i][y], TrellisLED[_nr]);
      }
    }
  }
  Serial.println("load main buffer");
}
int trellis_get_main_buffer(int _page, int _x, int _y)
{
  // Serial.println("return main buffer");
  return trellisMainGridBuffer[_page][_x][_y];
}
void trellis_clear_main_buffer(int _page)
{

  for (int x = 0; x < TRELLIS_PADS_X_DIM; x++)
  {
    for (int y = 0; y < TRELLIS_PADS_Y_DIM; y++)
    {
      int _nr = x + (y * TRELLIS_PADS_X_DIM);
      trellisMainGridBuffer[_page][x][y] = TRELLIS_BLACK;
      trellis.clrLED(TrellisLED[_nr]);
    }
  }
}

void trellis_show_clockbar(byte trackNr, byte step)
{
  if (myClock.isPlaying)
  {
    if (trellisScreen <= TRELLIS_SCREEN_SEQUENCER_CLIP_8 || trellisScreen >= TRELLIS_SCREEN_ARRANGER_1)
    {
      trellisShowClockPixel[trackNr] = false;
      int color;

      if (allTracks[trackNr]->get_recordState())
        color = TRELLIS_RED;
      else
        color = TRELLIS_ORANGE;

      if (trellisScreen <= TRELLIS_SCREEN_SEQUENCER_CLIP_8)
      {
        byte _nr = step + (trackNr * NUM_STEPS);
        trellis.setLED(TrellisLED[_nr]);
        if (step > 0)
        {
          if (trellis_get_main_buffer(allTracks[trackNr]->parameter[SET_CLIP2_EDIT], step - 1, trackNr) > 0)
            trellis.setLED(TrellisLED[_nr - 1]);
          else
            trellis.clrLED(TrellisLED[_nr - 1]);
        }
        if (step == 0)
        {
          if (trellis_get_main_buffer(allTracks[trackNr]->parameter[SET_CLIP2_EDIT], (allTracks[trackNr]->parameter[SET_SEQUENCE_LENGTH] / 6) - 1, trackNr) > 0)
            trellis.setLED(TrellisLED[((allTracks[trackNr]->parameter[SET_SEQUENCE_LENGTH] / 6) - 1) + (trackNr * TRELLIS_PADS_X_DIM)]);
          else
            trellis.clrLED(TrellisLED[((allTracks[trackNr]->parameter[SET_SEQUENCE_LENGTH] / 6) - 1) + (trackNr * TRELLIS_PADS_X_DIM)]);
          // byte oldNr = (allTracks[trackNr]->parameter[SET_SEQUENCE_LENGTH] / 6) - 1;
          // neotrellis.setPixelColor(oldNr, trackNr, trellis_get_main_buffer(allTracks[trackNr]->parameter[SET_CLIP2_EDIT], oldNr, trackNr));
          //  Serial.println(oldNr);
        }
      }
      else
      {
        // if (step % 16 == 0)
        {
          byte _nr = myClock.barTick + (trackNr * NUM_STEPS);
          trellis.setLED(TrellisLED[_nr]);

          if (myClock.barTick > 0)
          {
            if (trellis_get_main_buffer(trellisScreen, myClock.barTick - 1, trackNr) > 0)
              trellis.setLED(TrellisLED[_nr - 1]);
            else
              trellis.clrLED(TrellisLED[_nr - 1]);
          }
          // neotrellis.setPixelColor(myClock.barTick - 1, trackNr, trellis_get_main_buffer(trellisScreen, myClock.barTick - 1, trackNr));
          if (myClock.barTick == 0)
          {
            if (trellis_get_main_buffer(trellisScreen, myClock.endOfLoop - 1, trackNr) > 0)
              trellis.setLED(TrellisLED[(myClock.endOfLoop - 1) + (trackNr * TRELLIS_PADS_X_DIM)]);
            else
              trellis.clrLED(TrellisLED[(myClock.endOfLoop - 1) + (trackNr * TRELLIS_PADS_X_DIM)]);
            //  neotrellis.setPixelColor(myClock.endOfLoop - 1, trackNr, trellis_get_main_buffer(trellisScreen, myClock.endOfLoop - 1, trackNr));
            // Serial.println(oldNr);
          }
        }
      }
      // trellis.writeDisplay();
      // neotrellis_show();
    }
  }
}