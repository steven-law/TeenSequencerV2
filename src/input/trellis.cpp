#include "input/trellis.h"

void export_midi_track(Track *track, int songNr, uint16_t ppqn = 24);
uint8_t trellisPianoTrack;
bool trellisPressed[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM];
int trellisTrackColor[9]{TRELLIS_RED, TRELLIS_PURPLE, TRELLIS_OLIVE, TRELLIS_YELLOW, TRELLIS_BLUE, 9365295, TRELLIS_AQUA, TRELLIS_GREEN, 900909};
int trellisControllBuffer[X_DIM][Y_DIM];
const long trellisReadInterval = 30;             // interval at which to blink (milliseconds)
const unsigned long neotrellisReadInterval = 30; // interval at which to blink (milliseconds)
const long trellisRestartInterval = 60000;       // interval at which to blink (milliseconds)
unsigned long trellisRestartPreviousMillis = 0;  // will store last time LED was updated
unsigned long trellisReadPreviousMillis = 0;     // will store last time LED was updated
uint8_t bar2edit;
const uint8_t TrellisLED[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM]{0, 1, 2, 3, 16, 17, 18, 19, 32, 33, 34, 35, 48, 49, 50, 51,
                                                                  4, 5, 6, 7, 20, 21, 22, 23, 36, 37, 38, 39, 52, 53, 54, 55,
                                                                  8, 9, 10, 11, 24, 25, 26, 27, 40, 41, 42, 43, 56, 57, 58, 59,
                                                                  12, 13, 14, 15, 28, 29, 30, 31, 44, 45, 46, 47, 60, 61, 62, 63,
                                                                  64, 65, 66, 67, 80, 81, 82, 83, 96, 97, 98, 99, 112, 113, 114, 115,
                                                                  68, 69, 70, 71, 84, 85, 86, 87, 100, 101, 102, 103, 116, 117, 118, 119,
                                                                  72, 73, 74, 75, 88, 89, 90, 91, 104, 105, 106, 107, 120, 121, 122, 123,
                                                                  76, 77, 78, 79, 92, 93, 94, 95, 108, 109, 110, 111, 124, 125, 126, 127};

uint32_t ticks[] = {0, 24, 48, 72, 96, 120, 144, 168, 192, 216, 240, 264};
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
// void export_track_to_midi(Track* track, uint8_t songNr , uint16_t ppqn = 24);
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
    if (evt.bit.NUM < 16)
      trellisOut.recall_main_buffer();
  }

  // neotrellis_show();
  return 0;
}
uint16_t rgb24to565(uint32_t color24)
{
  uint8_t r = (color24 >> 16) & 0xFF;
  uint8_t g = (color24 >> 8) & 0xFF;
  uint8_t b = color24 & 0xFF;
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
uint32_t rgb565to24(uint16_t color565)
{
  uint8_t r = (color565 >> 11) & 0x1F; // 5 Bit
  uint8_t g = (color565 >> 5) & 0x3F;  // 6 Bit
  uint8_t b = color565 & 0x1F;         // 5 Bit

  // Hochskalieren auf 8 Bit (255)
  r = (r << 3) | (r >> 2); // von 5 → 8 Bit
  g = (g << 2) | (g >> 4); // von 6 → 8 Bit
  b = (b << 3) | (b >> 2); // von 5 → 8 Bit

  return (r << 16) | (g << 8) | b;
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
    if (allTracks[i]->get_recordState())
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

  trellisOut.trellis_assign_start_buffer(lastPotRow);
  neotrellis_show();
}
void neotrellis_set_control_buffer(int _x, int _y, int _color)
{

  trellisControllBuffer[_x][_y] = _color;
  neotrellis.setPixelColor(_x, _y, _color);

  neotrellis_show();

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

void neo_trellis_set_brightness() // trellisOut implemented
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
      trellisOut.setBrightness(0);
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
      trellisOut.setBrightness(3);
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
      trellisOut.setBrightness(6);
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
      trellisOut.setBrightness(15);
    }
  }
}
void neotrellis_show()
{

  neotrellis.show();
}
void neotrellis_update()
{

  neotrellis_start_clock();
  neotrellis_stop_clock();
  neotrellis_set_potRow();
  neotrellis_save_load();
  neo_trellis_set_brightness();
  neotrellis_set_mute();
  neotrellis_set_solo();
  neotrellis_set_fast_record();
  neo_trellis_select_trackClips();
  neo_trellis_select_mixer();
  neotrellis_show_seqMode();
  neotrellis_show_plugin();
  neotrellis_show_piano();
}
// 1st row
void neotrellis_set_potRow()
{
  if (neotrellisPressed[TRELLIS_POTROW] && !neotrellisPressed[TRELLIS_BUTTON_SAVELOAD])
  {
    neotrellisPressed[TRELLIS_POTROW] = false;
    change_plugin_row = true;
    change_row = true;
    lastPotRow = (lastPotRow + 1) % 4;
    if (trellisOut.getActiveScreen() == TRELLIS_SCREEN_PLAYMODE)
    {
      trellisOut.clearMainGridNow();
      trellisOut.drawPlaymode();
    }
    if (trellisOut.getActiveScreen() == TRELLIS_SCREEN_PLUGIN)
    {
      trellisOut.clearMainGridNow();
      trellisOut.drawPlugin();
    }
  }
}
void neotrellis_save_load() // trellisOut implemented
{

  if (neotrellisPressed[TRELLIS_BUTTON_SAVELOAD])
  {
    trellisOut.setActiveScreen(TRELLIS_SCREEN_SAVE_LOAD);
    trellisOut.drawLoadSavePage();
    neotrellisPressed[TRELLIS_BUTTON_SAVELOAD] = false;
  }
}
void neotrellis_start_clock()
{
  if (neotrellisPressed[TRELLIS_START_CLOCK] /*|| enc_button[2]*/)
  {
    myClock.set_start();
    drawActivePlaying(true);

    // Serial.println("Play");
    neotrellisPressed[TRELLIS_START_CLOCK] = false;
  }
}
void neotrellis_stop_clock()
{
  if (neotrellisPressed[TRELLIS_STOP_CLOCK] /*|| enc_button[3]*/)
  {
    myClock.set_stop();
    drawActivePlaying(false);

    // Serial.println("Stop");
    for (int i = 0; i < NUM_TRACKS; i++)
    {
      allTracks[i]->internal_clock = -1;
      allTracks[i]->internal_clock_bar = -1;
      for (int v = 0; v < MAX_VOICES; v++)
      {
        if (allTracks[i]->noteToPlay[v] < NO_NOTE)
          allTracks[i]->noteOff(allTracks[i]->noteToPlay[v], 0, allTracks[i]->clip[allTracks[i]->clip_to_play[allTracks[i]->external_clock_bar]].midiChOut);
      }
      // allTracks[i]->external_clock_bar = -1;
    }
    neotrellisPressed[TRELLIS_STOP_CLOCK] = false;
  }
}
// 2nd row
void neotrellis_SetCursor(uint8_t maxY)
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
void neotrellis_show_piano() // trellisOut implemented
{
  if (neotrellisPressed[TRELLIS_BUTTON_PIANO])
  {
    for (int i = 0; i < NUM_TRACKS; i++)
    {
      if (neotrellisPressed[3 + ((i + 4) * X_DIM)])
      {
        trellisPianoTrack = i;
        show_active_page_info("Piano", trellisPianoTrack + 1);
        neotrellis_set_control_buffer(2, 2, trellisTrackColor[trellisPianoTrack]);
        neotrellisPressed[3 + ((i + 4) * X_DIM)] = false;
        neotrellisPressed[TRELLIS_BUTTON_PIANO] = false;
        trellisOut.setActiveScreen(TRELLIS_SCREEN_PIANO);
        trellisOut.drawPiano();
        break;
      }
    }
  }
}
void neotrellis_show_seqMode()
{
  if (neotrellisPressed[TRELLIS_BUTTON_SEQMODE])
  {
    for (int i = 0; i < NUM_TRACKS; i++)
    {
      if (neotrellisPressed[3 + ((i + 4) * X_DIM)])
      {
        neotrellisPressed[3 + ((i + 4) * X_DIM)] = false;
        active_track = i;
        clearWorkSpace();
        show_active_page_info("Track", active_track + 1);

        int trackPlaymode = allTracks[active_track]->clip[allTracks[active_track]->parameter[SET_CLIP2_EDIT]].playMode;
        allTracks[active_track]->draw_sequencer_modes(trackPlaymode);
        activeScreen = INPUT_FUNCTIONS_FOR_SEQUENCER_MODES;
        trellisOut.setActiveScreen(TRELLIS_SCREEN_PLAYMODE);
        trellisOut.drawPlaymode();
        neotrellis_set_control_buffer(3, 2, trellisTrackColor[active_track]);
        // Serial.println("SeqMode selected");
        break;
      }
    }
  }
}
// 4th row
void trellis_show_arranger() // trellisOut implemented
{
  if (neotrellisPressed[TRELLIS_BUTTON_ARRANGER])
  {
    trellisOut.drawSelectArrangerPage();

    if (isPressed() && getPressedKey() < NUM_STEPS)
    {
      trellisOut.clearMainGridNow();
      arrangerpage = getPressedKey();
      revertPressedKey();
      neotrellisPressed[TRELLIS_BUTTON_ARRANGER] = false;
      trellisOut.setActiveScreen(arrangerpage + TRELLIS_SCREEN_ARRANGER_1);
      activeScreen = INPUT_FUNCTIONS_FOR_ARRANGER;
      startUpScreen();
      gridSongMode(arrangerpage);
      for (int y = 0; y < NUM_TRACKS; y++)
        trellisOut.get_main_buffer(trellisOut.getActiveScreen(), arrangerpage, y);
      trellisOut.recall_main_buffer();
    }
  }
}
void trellis_setClipsToArranger()
{
  if (trellisOut.getActiveScreen() < TRELLIS_SCREEN_ARRANGER_1 || trellisOut.getActiveScreen() >= TRELLIS_SCREEN_ARRANGER_16)
  {
    return;
  }

  for (int t = 0; t < NUM_TRACKS; t++)
  {
    int trackButton = 3 + (X_DIM * (t + 4));

    if (neotrellisPressed[trackButton])
    {
      Serial.printf("track = %d\n", t);
      trellis.clear();
      for (int x = 0; x < TRELLIS_PADS_X_DIM; x++)
      {
        uint8_t clipNr = allTracks[t]->clip_to_play[x + arrangerpage * 16];
        uint8_t ledIndex = x + clipNr * TRELLIS_PADS_X_DIM;
        trellis.setLED(TrellisLED[ledIndex]);
      }
      trellisOut.writeDisplay();

      if (isPressed())
      {
        uint8_t key = getPressedKey();
        uint8_t x = key % TRELLIS_PADS_X_DIM;
        uint8_t y = key / TRELLIS_PADS_X_DIM;

        uint8_t bar = x + arrangerpage * 16;
        uint8_t clipNr = y;
        revertPressedKey();
        neotrellisPressed[trackButton] = false;
        change_plugin_row = true;

        for (int i = 0; i < allTracks[t]->clip[clipNr].clockDivision; i++)
        {
          allTracks[t]->set_clip_to_play_trellis(bar + i, clipNr);
        }

        Serial.printf("Set trellis arranger track: %d, bar: %d, clipNr: %d method 1\n", t, bar, clipNr);
        trellis.clear();
        trellisOut.recall_main_buffer();
        //return;
      }
    }
    else
    {
      for (int x = 0; x < TRELLIS_PADS_X_DIM; x++)
      {
        for (int y = 0; y < TRELLIS_PADS_Y_DIM; y++)
        {
          uint8_t key = x + y * TRELLIS_PADS_X_DIM;

          if (trellisHeld[key])
          {
            uint8_t baseTrack = key / NUM_STEPS;
            uint8_t step = key % NUM_STEPS;
            uint8_t baseBar = step + arrangerpage * BARS_PER_PAGE;

            for (int i = 1; i < NUM_STEPS - step; i++)
            {
              for (int dt = 0; dt < NUM_TRACKS - baseTrack; dt++)
              {
                int nextKey = key + i + dt * TRELLIS_PADS_X_DIM;
                if (trellisPressed[nextKey])
                {
                  for (int b = 0; b <= i; b++)
                  {
                    for (int _t = 0; _t <= dt; _t++)
                    {
                      allTracks[baseTrack + _t]->set_clip_to_play_trellis(baseBar + b, gridTouchY);
                    }
                  }

                  trellisPressed[nextKey] = false;
                  change_plugin_row = true;
                  Serial.printf("Tied Arranger track: %d, bar: %d, clip: %d\n", baseTrack + dt, baseBar + i, gridTouchY);
                  trellisOut.recall_main_buffer();
                  return;
                }
              }
            }
          }

           if (trellisPressed[key])
          {
            uint8_t bar = x + arrangerpage * 16;
            uint8_t track = y;
            trellisPressed[key] = false;
            neotrellisPressed[trackButton] = false;
            change_plugin_row = true;

            for (int i = 0; i < allTracks[track]->clip[gridTouchY].clockDivision; i++)
            {
              allTracks[track]->set_clip_to_play_trellis(bar + i, gridTouchY);
            }

            Serial.printf("Set trellis arranger track: %d, bar: %d, clipNr: %d method 2\n", track, bar, gridTouchY);
            trellisOut.recall_main_buffer();
            return;
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
    }
    trellisOut.drawPerformPotrow(lastPotRow);
  }
}
void neotrellis_perform_set_active()
{
  if (trellisOut.getActiveScreen() != TRELLIS_SCREEN_PERFORM)
    return;

  for (int t = 0; t < NUM_TRACKS; t++)
  {
    if (neotrellisPressed[3 + ((t + 4) * X_DIM)])
    {

      if (!allTracks[t]->performIsActive)
      {
        neotrellis_set_control_buffer(3, t + 4, TRELLIS_WHITE);
        allTracks[t]->performIsActive = true;
        neotrellisPressed[3 + ((t + 4) * X_DIM)] = false;
        break;
      }
      else if (allTracks[t]->performIsActive)
      {
        neotrellis_set_control_buffer(3, t + 4, trellisTrackColor[t]);
        allTracks[t]->performIsActive = false;
        neotrellisPressed[3 + ((t + 4) * X_DIM)] = false;
        break;
      }
      Serial.printf("Perform active Track %d, \n", allTracks[t]->performIsActive ? "true" : "false");
    }
  }
}
void set_performCC(uint8_t XPos, uint8_t YPos, const char *name)
{
  if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
  {
    if (inputs.enc_moved[XPos])
    {
      uint8_t _nr = XPos + (YPos * 4);
      performCC[_nr] = constrain(performCC[_nr] + inputs.encoded[XPos], 0, 128);
      drawPot(XPos, YPos, performCC[_nr], name);
    }
  }
}
void set_perform_page(uint8_t row)
{
  draw_perform_page();
  switch (lastPotRow)
  {
  case 0:
  {
    set_performCC(0, 0, "CC");
    set_performCC(1, 0, "CC");
    set_performCC(2, 0, "CC");
    set_performCC(3, 0, "CC");
  }
  break;
  case 1:
  {
    set_performCC(0, 1, "CC");
    set_performCC(1, 1, "CC");
    set_performCC(2, 1, "CC");
    set_performCC(3, 1, "CC");
  }
  break;
  case 2:
  {
    set_performCC(0, 2, "CC");
    set_performCC(1, 2, "CC");
    set_performCC(2, 2, "CC");
    set_performCC(3, 2, "CC");
  }
  break;
  case 3:
  {
    set_performCC(0, 3, "CC");
    set_performCC(1, 3, "CC");
    set_performCC(2, 3, "CC");
    set_performCC(3, 3, "CC");
  }
  break;
  default:
    break;
  }
}

void neo_trellis_select_trackClips()
{
  if (!neotrellisPressed[TRELLIS_BUTTON_SEQUENCER])
    return;

  trellisOut.drawSelectClip2Edit();

  for (int y = 0; y < NUM_TRACKS; y++)
  {
    // Track-Auswahl
    if (neotrellisPressed[3 + (X_DIM * (y + 4))])
    {
      handleClipOrTrackSelection(y);
      break;
    }

    // Clip-Auswahl
    for (int x = 0; x < MAX_CLIPS-1; x++)
    {
      if (trellisPressed[x + (TRELLIS_PADS_X_DIM * y)])
      {
        handleClipOrTrackSelection(y, x);
        break;
      }
    }
  }
}
void trellis_setStepsToSequencer()
{
  uint8_t trellisNote = (gridTouchY > 0 && gridTouchY <= 12) ? (gridTouchY - 1) : 0;
  uint8_t track;
  uint8_t step;
  if (trellisOut.getActiveScreen() >= TRELLIS_SCREEN_SEQUENCER_CLIP_8)
    return;

  for (int x = 0; x < NUM_STEPS; x++)
  {
    for (int y = 0; y < NUM_TRACKS; y++)
    {
      uint8_t _nr = x + (y * TRELLIS_PADS_X_DIM);
      if (trellisHeld[_nr])
      {
        track = _nr / NUM_STEPS;
        step = _nr % NUM_STEPS;
        int keyTick = step * 6;

        for (int i = 1; i < NUM_STEPS - step; i++)
        {
          if (trellisPressed[_nr + i])
          {
            int _note = (trellisNote) + (allTracks[active_track]->parameter[SET_OCTAVE] * NOTES_PER_OCTAVE);

            allTracks[track]->set_note_on_tick(keyTick + allTracks[track]->parameter[SET_SWING], _note, (i * TICKS_PER_STEP) + allTracks[track]->parameter[SET_STEP_LENGTH]);
            trellisPressed[_nr + i] = false;
            change_plugin_row = true;
            //  Serial.printf("Tied Step: %d, Tick: %d, Track: %d, Note: %d, length: %d\n", step, keyTick, track, trellisNote + (allTracks[track]->parameter[SET_OCTAVE] * NOTES_PER_OCTAVE), i * TICKS_PER_STEP);
          }
        }
      }
      if (trellisPressed[_nr])
      {
        uint8_t track = _nr / NUM_STEPS;
        uint8_t step = _nr % NUM_STEPS;
        int keyTick = step * 6;
        // Setze die Note auf dem aktuellen Step
        int _note = (trellisNote) + (allTracks[active_track]->parameter[SET_OCTAVE] * NOTES_PER_OCTAVE);
        allTracks[track]->set_note_on_tick(keyTick + allTracks[track]->parameter[SET_SWING], _note, allTracks[track]->parameter[SET_STEP_LENGTH]);
        trellisPressed[_nr] = false;
        change_plugin_row = true;
        //  Serial.printf("Step: %d, Tick: %d, Track: %d, Note: %d\n", step, keyTick, track, trellisNote + (allTracks[track]->parameter[SET_OCTAVE] * NOTES_PER_OCTAVE));
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
      allTracks[i]->muted = !allTracks[i]->muted;
      neotrellis_set_control_buffer(0, i + 4, allTracks[i]->muted ? TRELLIS_BLACK : TRELLIS_1);
      Serial.printf("Track %d %s\n", i, allTracks[i]->muted ? "muted" : "unmuted");
    }
  }
}
void neotrellis_set_solo()
{
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    int btn = 1 + ((i + 4) * X_DIM);
    if (!neotrellisPressed[btn])
      continue;

    neotrellisPressed[btn] = false;
    bool newSoloState = !allTracks[i]->soloed;

    // LED Farbe setzen
    neotrellis_set_control_buffer(1, i + 4, newSoloState ? TRELLIS_ORANGE : TRELLIS_1);

    allTracks[i]->soloed = newSoloState;

    // Alle muteThruSolo Flags zurücksetzen
    for (int t = 0; t < NUM_TRACKS; t++)
      allTracks[t]->muteThruSolo = false;

    // Wenn mindestens ein Track solo ist → alle anderen muten
    bool anySolo = false;
    for (int t = 0; t < NUM_TRACKS; t++)
      if (allTracks[t]->soloed)
        anySolo = true;

    if (anySolo)
    {
      for (int t = 0; t < NUM_TRACKS; t++)
        if (!allTracks[t]->soloed)
          allTracks[t]->muteThruSolo = true;
    }

    // Debug-Ausgabe
    for (int t = 0; t < NUM_TRACKS; t++)
    {
      Serial.printf("%d mute=%s, solo=%s, muteThruSolo=%s\n",
                    t,
                    allTracks[t]->muted ? "true" : "false",
                    allTracks[t]->soloed ? "true" : "false",
                    allTracks[t]->muteThruSolo ? "true" : "false");
    }

    // break; // nur ein Button gleichzeitig behandeln
  }
}
void neotrellis_set_fast_record()
{
  for (int i = 0; i < NUM_TRACKS; i++)
  {
    int btn = 2 + ((i + 4) * X_DIM);
    if (!neotrellisPressed[btn])
      continue;
    neotrellisPressed[btn] = false;
    bool newState = !allTracks[i]->get_recordState();
    allTracks[i]->set_recordState(newState);
    neotrellis_set_control_buffer(2, i + 4, newState ? TRELLIS_RED : TRELLIS_1);
    drawActiveRecording(newState);
    Serial.printf("Track: %d, recordstate: %s\n", i, newState ? "on" : "off");
  }
}
void handleClipOrTrackSelection(int track, int clip)
{

  neotrellisPressed[TRELLIS_BUTTON_SEQUENCER] = false;

  active_track = track;
  show_active_page_info("Track", active_track + 1);
  change_plugin_row = true;
  activeScreen = INPUT_FUNCTIONS_FOR_SEQUENCER;

  if (clip >= 0)
  {
    allTracks[track]->parameter[SET_CLIP2_EDIT] = clip;
    trellisPressed[clip + (TRELLIS_PADS_X_DIM * track)] = false;
  }
  else
  {
    trellisOut.setActiveScreen(allTracks[active_track]->parameter[SET_CLIP2_EDIT]);
    neotrellisPressed[3 + (X_DIM * (track + 4))] = false;
  }
  drawStepSequencerStatic();
  draw_stepSequencer_parameters();
  draw_notes_in_grid();
  neotrellis_set_control_buffer(3, 3, trellisTrackColor[active_track]);
  trellisOut.clearMainGridNow();
  trellisOut.setActiveScreen(allTracks[active_track]->parameter[SET_CLIP2_EDIT]);
  trellisOut.recall_main_buffer();
}

// trellis input stuff
void trellis_update()
{
  trellis_save_load();
  trellis_play_piano();
  trellis_show_arranger();
  unsigned long trellisCurrentMillis = millis();
  if (trellisCurrentMillis - trellisReadPreviousMillis >= trellisReadInterval)
  {
    trellisReadPreviousMillis = trellisCurrentMillis;
    trellis_read();
  }
}

void trellis_read()
{
  if (!i2c_busy)
  {
    i2c_busy = true;
    // If a button was just pressed or released...
    if (trellis.readSwitches())
    {
      // go through every button
      for (uint8_t i = 0; i < numKeys; i++)
      {

        // if it was pressed, turn it on
        if (trellis.justPressed(TrellisLED[i]))
        {
          inputs.active[i / 32] = true;
          lastPressedKey = i;
          updateTFTScreen = true;
          trellisPressed[i] = true;
          trellisIsPressed = true;
          Serial.print("nr");
          Serial.println(i);
        }

        if (trellis.isKeyPressed(TrellisLED[i]))
        {
          trellisHeld[i] = true;
        }

        // if it was released, turn it off
        if (trellis.justReleased(TrellisLED[i]))
        {
          trellisPressed[i] = false;
          trellisHeld[i] = false;
          trellisIsPressed = false;
          lastPressedKey = 255;
        }
      }
    }
    i2c_busy = false;
  }
}
void trellis_play_piano()
{

  if (trellisOut.getActiveScreen() != TRELLIS_SCREEN_PIANO)
    return;

  auto track = allTracks[trellisPianoTrack];
  int trackChannel = track->clip[track->parameter[SET_CLIP2_EDIT]].midiChOut;
  static bool _holdNote[NUM_STEPS * NUM_TRACKS];
  static uint8_t _noteSend[MAX_VOICES];

  for (int x = 0; x < NUM_STEPS; x++)
  {
    for (int y = 0; y < NUM_TRACKS; y++)
    {
      uint8_t key = x + ((y * NUM_STEPS));
      if (x < NOTES_PER_OCTAVE)
      {
        if (trellisPressed[key] && !_holdNote[key])
        {
          uint8_t _octave = ((NUM_TRACKS - 1) - (key / NUM_STEPS)) + track->parameter[SET_OCTAVE];

          _noteSend[x] = x + (_octave * NOTES_PER_OCTAVE);
          _holdNote[key] = true;

          track->noteOn(_noteSend[x], 99, trackChannel);
          // Serial.printf("trellisPiano NoteON note:%d, octave:%d\n", _noteSend[x], _octave);
          break;
        }

        else if (!trellisPressed[key] && _holdNote[key])
        {
          _holdNote[key] = false;
          track->noteOff(_noteSend[x], 0, trackChannel);
          // Serial.printf("trellisPiano NoteOff note:%d, track:%d\n", _noteSend[x], trellisPianoTrack);
          break;
        }
      }
    }
  }
}
void trellis_play_clipLauncher()
{
  if (trellisOut.getActiveScreen() != INPUT_FUNCTIONS_FOR_CLIPLAUNCHER)
    return;

  if (inputs.enc_moved[0])
  {
    bar2edit = bar2edit + inputs.encoded[0];
    inputs.enc_moved[0] = false;
    draw_clip_launcher();
  }
  myClock.set_tempo(1);
  myClock.set_start_of_loop(2);
  myClock.set_end_of_loop(3);

  for (int t = 0; t < NUM_TRACKS; t++)
  {
    allTracks[t]->bar_to_edit = bar2edit;
    for (int c = 0; c < MAX_CLIPS; c++)
    {
      uint8_t _nr = c + (t * TRELLIS_PADS_X_DIM);
      if (trellisPressed[_nr])
      {
        Serial.printf("tp=%d\n", _nr);
        trellisPressed[_nr] = false;

        allTracks[t]->clip_to_play[bar2edit] = c;

        draw_active_clip_launcher(t, c);
        break;
      }
    }
  }
}
void trellis_save_load()
{
  if (trellisOut.getActiveScreen() != TRELLIS_SCREEN_SAVE_LOAD)
    return;
  if (isPressed())
  {
    // Save song
    if (getPressedKey() < MAX_SONGS)
    {
      uint8_t songNr = getPressedKey();
      draw_infobox_text("Saving song: ", songNames[songNr]);
      for (int t = 0; t < 8; t++)
        allTracks[t]->save_track(songNr);
      myClock.save_clock(songNr);
    }

    // Load song
    else if ((getPressedKey() >= NUM_STEPS && getPressedKey() < NUM_STEPS + MAX_SONGS) || getPressedKey() == 31)
    {
      uint8_t songNr = getPressedKey() - NUM_STEPS;
      draw_infobox_text("Loading song: ", songNames[songNr]);
      for (int t = 0; t < 8; t++)
        allTracks[t]->load_track(songNr);
      myClock.load_clock(songNr);
      neotrellis_setup(0);
      startUpScreen();
    }

    // Export to MIDI
    else if ((getPressedKey() >= 3 * NUM_STEPS && getPressedKey() < 3 * NUM_STEPS + MAX_SONGS))
    {
      uint8_t songNr = getPressedKey() - 3 * NUM_STEPS;
      draw_infobox_text("Saving song to MIDI: ", songNames[songNr]);
      for (int t = 0; t < 8; t++)
        export_midi_track(allTracks[t], songNr);
    }
    trellisOut.drawPreviousScreen();
    revertPressedKey();
  }
}

uint8_t getPressedKey()
{
  return lastPressedKey;
}
bool isPressed()
{

  return trellisIsPressed;
}
void revertPressedKey()
{
  if (trellisPressed[lastPressedKey])
  {
    trellisPressed[lastPressedKey] = false;
    trellisIsPressed = false;
    lastPressedKey = 255;
    // Serial.println("trellisreverted");
  }
}