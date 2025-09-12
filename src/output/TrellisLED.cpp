#include "output/TrellisLED.h"

// in use
void MyTrellis::setup(int dly)
{
  pinMode(TRELLIS_INT_PIN, INPUT);
  digitalWrite(TRELLIS_INT_PIN, HIGH);
  trellis.begin(0x70, 0x72, 0x73, 0x74, 0x71, 0x75, 0x76, 0x77);
  Serial.println("trellis Setup done");
  tft.println("trellis Setup done");
  delay(dly);
}
void MyTrellis::trellis_assign_start_buffer(uint8_t _lastPotRow)
{
  for (int i = 0; i < TRELLIS_PADS_Y_DIM - 1; i++)
  {
    set_main_buffer(TRELLIS_SCREEN_PERFORM, 0, i, TRELLIS_TEAL);
    set_main_buffer(TRELLIS_SCREEN_PERFORM, 1, i, TRELLIS_PINK);
    set_main_buffer(TRELLIS_SCREEN_PERFORM, 2, i, TRELLIS_OLIVE);
    set_main_buffer(TRELLIS_SCREEN_PERFORM, 3, i, TRELLIS_AQUA);
    set_main_buffer(TRELLIS_SCREEN_PERFORM, 4, i, TRELLIS_ORANGE);
    set_main_buffer(TRELLIS_SCREEN_PERFORM, 5, i, TRELLIS_OLIVE);
    set_main_buffer(TRELLIS_SCREEN_PERFORM, 6, i, TRELLIS_OLIVE);
    set_main_buffer(TRELLIS_SCREEN_PERFORM, 7, i, TRELLIS_AQUA);
    set_main_buffer(TRELLIS_SCREEN_PERFORM, 8, i, TRELLIS_AQUA);
    set_main_buffer(TRELLIS_SCREEN_PERFORM, 9, i, TRELLIS_ORANGE);
    set_main_buffer(TRELLIS_SCREEN_PERFORM, 10, i, TRELLIS_ORANGE);
    set_main_buffer(TRELLIS_SCREEN_PERFORM, 11, i, TRELLIS_PINK);
    set_main_buffer(TRELLIS_SCREEN_PERFORM, 12, i, TRELLIS_PINK);
    set_main_buffer(TRELLIS_SCREEN_PERFORM, 13, i, TRELLIS_BLUE);
    set_main_buffer(TRELLIS_SCREEN_PERFORM, 14, i, TRELLIS_RED);
    set_main_buffer(TRELLIS_SCREEN_PERFORM, 15, i, TRELLIS_GREEN);

    set_main_buffer(TRELLIS_SCREEN_MIXER, 3, i, TRELLIS_PINK);
    set_main_buffer(TRELLIS_SCREEN_MIXER, 7, i, TRELLIS_OLIVE);
    set_main_buffer(TRELLIS_SCREEN_MIXER, 11, i, TRELLIS_AQUA);
    set_main_buffer(TRELLIS_SCREEN_MIXER, 15, i, TRELLIS_ORANGE);
  }

  set_main_buffer(TRELLIS_SCREEN_PERFORM, _lastPotRow * 4, 7, TRELLIS_BLUE);
  set_main_buffer(TRELLIS_SCREEN_PERFORM, _lastPotRow * 4 + 1, 7, TRELLIS_RED);
  set_main_buffer(TRELLIS_SCREEN_PERFORM, _lastPotRow * 4 + 2, 7, TRELLIS_GREEN);
  set_main_buffer(TRELLIS_SCREEN_PERFORM, _lastPotRow * 4 + 3, 7, TRELLIS_WHITE);

  // neotrellis_recall_control_buffer();
  Serial.println("trellis setup done");
}
void MyTrellis::drawLoadSavePage()
{

  if (trellisScreen != TRELLIS_SCREEN_SAVE_LOAD)
    return;
  Serial.println(trellisScreen);
  clearMainGridNow();

  for (int i = 0; i < MAX_SONGS; i++)
  {
    trellis.setLED(TrellisLED[i]);
    trellis.setLED(TrellisLED[i + TRELLIS_PADS_X_DIM]);
    trellis.setLED(TrellisLED[15 + TRELLIS_PADS_X_DIM]);
    trellis.setLED(TrellisLED[i + (3 * TRELLIS_PADS_X_DIM)]);
  }
  // writeDisplay();
}
void MyTrellis::drawPreviousScreen()
{
  clearMainGridNow();
  trellisScreen = oldTrellisScreen;
  recall_main_buffer();
}
void MyTrellis::clearMainGridNow()
{
  trellis.clear();
  writeDisplay();
}
void MyTrellis::writeDisplay()
{
  trellis.writeDisplay();
}
void MyTrellis::set_main_buffer(int _page, int _x, int _y, int color)
{
  // Serial.printf("set main buffer page: %d, x: %d, y: %d, color: %d\n", _page, _x, _y, color);
  trellisMainGridBuffer[_page][_x][_y] = color;
  if (trellisScreen == TRELLIS_SCREEN_SAVE_LOAD)
    return;
  int _nr = _x + (_y * TRELLIS_PADS_X_DIM);
  trellis.setLED(TrellisLED[_nr]);
  if (trellisMainGridBuffer[_page][_x][_y] == TRELLIS_BLACK)
    trellis.clrLED(TrellisLED[_nr]);
  // Serial.printf("set main buffer page: %d, x: %d, y: %d, color: %d, trellisNr: %d\n", _page, _x, _y, color, TrellisLED[_nr]);
}

void MyTrellis::recall_main_buffer()
{
  // Serial.printf("recall main buffer page: %d\n", _page);
  for (int i = 0; i < TRELLIS_PADS_X_DIM; i++)
  {
    for (int y = 0; y < TRELLIS_PADS_Y_DIM; y++)
    {
      {
        int _nr = i + (y * TRELLIS_PADS_X_DIM);
        if (trellisMainGridBuffer[trellisScreen][i][y] > 0)
          trellis.setLED(TrellisLED[_nr]);
        else
          trellis.clrLED(TrellisLED[_nr]);
        // Serial.printf("recall main buffer page: %d, x: %d, y: %d, color: %d, trellisNr: %d\n", _page, i, y, trellisMainGridBuffer[_page][i][y], TrellisLED[_nr]);
      }
    }
  }
  trellis.writeDisplay();
}
int MyTrellis::get_main_buffer(int _page, int _x, int _y)
{
  // Serial.printf("get main buffer page: %d\n", _page);
  return trellisMainGridBuffer[_page][_x][_y];
}
void MyTrellis::show_clockbar(uint8_t trackNr, uint8_t step)
{
  if (myClock.isPlaying)
  {
    if (trellisScreen <= TRELLIS_SCREEN_SEQUENCER_CLIP_8 || trellisScreen >= TRELLIS_SCREEN_ARRANGER_1)
    {
      trellisShowClockPixel[trackNr] = false;

      if (trellisScreen <= TRELLIS_SCREEN_SEQUENCER_CLIP_8)
      {
        uint8_t _nr = step + (trackNr * NUM_STEPS);
        trellis.setLED(TrellisLED[_nr]);
        if (step > 0)
        {
          if (get_main_buffer(allTracks[trackNr]->parameter[SET_CLIP2_EDIT], step - 1, trackNr) > 0)
            trellis.setLED(TrellisLED[_nr - 1]);
          else
            trellis.clrLED(TrellisLED[_nr - 1]);
        }
        if (step == 0)
        {
          int trackSeqLength = allTracks[trackNr]->clip[allTracks[trackNr]->clip_to_play[allTracks[trackNr]->internal_clock_bar]].seqLength;
          if (get_main_buffer(allTracks[trackNr]->parameter[SET_CLIP2_EDIT], (trackSeqLength / TICKS_PER_STEP) - 1, trackNr) > 0)
            trellis.setLED(TrellisLED[((trackSeqLength / TICKS_PER_STEP) - 1) + (trackNr * TRELLIS_PADS_X_DIM)]);
          else
            trellis.clrLED(TrellisLED[((trackSeqLength / TICKS_PER_STEP) - 1) + (trackNr * TRELLIS_PADS_X_DIM)]);
        }
      }
      else if (trellisScreen >= TRELLIS_SCREEN_ARRANGER_1 && trellisScreen < TRELLIS_SCREEN_ARRANGER_16)
      {
        // if (step % 16 == 0)
        {
          uint8_t _nr = (myClock.barTick % 16) + (trackNr * NUM_STEPS);
          trellis.setLED(TrellisLED[_nr]);
          // Serial.printf("clock bar trellisScreen = %d\n", trellisScreen);
          if (myClock.barTick > 0)
          {
            if (get_main_buffer(trellisScreen, (myClock.barTick % 16) - 1, trackNr) > 0)
              trellis.setLED(TrellisLED[_nr - 1]);
            else
              trellis.clrLED(TrellisLED[_nr - 1]);
          }
          if (myClock.barTick % 16 == 0)
          {
            if (get_main_buffer(trellisScreen, NUM_STEPS - 1, trackNr) > 0)
              trellis.setLED(TrellisLED[(NUM_STEPS - 1) + (trackNr * TRELLIS_PADS_X_DIM)]);
            else
              trellis.clrLED(TrellisLED[(NUM_STEPS - 1) + (trackNr * TRELLIS_PADS_X_DIM)]);
            // Serial.println(oldNr);
          }
        }
      }
      // neotrellis_show();
    }
  }
}

void MyTrellis::setBrightness(uint8_t brigthness)
{
  trellis.setBrightness(brigthness);
  writeDisplay();
}
void MyTrellis::drawSelectArrangerPage()
{
  if (trellisScreen == TRELLIS_SCREEN_SELECT_ARRANGERPAGE)
    return;
  trellisScreen = TRELLIS_SCREEN_SELECT_ARRANGERPAGE;
  Serial.println(trellisScreen);
  trellis.clear();
  for (int i = 0; i < NUM_STEPS; i++)
  {
    trellis.setLED(TrellisLED[i]);
  }
  writeDisplay();
}
void MyTrellis::drawSelectMixerPage()
{
  if (trellisScreen == TRELLIS_SCREEN_SELECT_MIXERPAGE)
    return;
  trellisScreen = TRELLIS_SCREEN_SELECT_MIXERPAGE;
  Serial.println(trellisScreen);

  trellis.clear();
  for (int i = 0; i < 8; i++)
  {
    trellis.setLED(TrellisLED[i]);
  }
  trellis.setLED(TrellisLED[15]);
  writeDisplay();
}
void MyTrellis::drawPiano()
{
  if (trellisScreen != TRELLIS_SCREEN_PIANO)
    return;
  clearMainGridNow();
  for (int x = 0; x < NOTES_PER_OCTAVE; x++)
  {
    for (int y = 0; y < NUM_TRACKS; y++)
    {
      int _color = TRELLIS_BLACK;
      int trackScale = allTracks[trellisPianoTrack]->clip[allTracks[trellisPianoTrack]->parameter[SET_CLIP2_EDIT]].scale;
      if (scales[trackScale][x])
      {
        _color = TRELLIS_WHITE;
      }
      else if (x == 0)
      {
        _color = trellisTrackColor[trellisPianoTrack];
        if (allTracks[trellisPianoTrack]->get_recordState())
          _color = TRELLIS_RED;
      }
      set_main_buffer(TRELLIS_SCREEN_PIANO, x, y, _color);
    }
  }
}
void MyTrellis::drawPerformPotrow(uint8_t prow)
{
  for (int i = 0; i < NUM_STEPS; i++)
  {
    set_main_buffer(TRELLIS_SCREEN_PERFORM, i, 7, TRELLIS_BLACK);
  }
  set_main_buffer(TRELLIS_SCREEN_PERFORM, prow * 4, 7, TRELLIS_BLUE);
  set_main_buffer(TRELLIS_SCREEN_PERFORM, prow * 4 + 1, 7, TRELLIS_RED);
  set_main_buffer(TRELLIS_SCREEN_PERFORM, prow * 4 + 2, 7, TRELLIS_GREEN);
  set_main_buffer(TRELLIS_SCREEN_PERFORM, prow * 4 + 3, 7, TRELLIS_WHITE);
  writeDisplay();
}
void MyTrellis::drawPerform(uint8_t prow)
{
  if (getActiveScreen() != TRELLIS_SCREEN_PERFORM)
    return;

  for (int x = 0; x < NUM_STEPS; x++)
  {
    for (int y = 0; y < NUM_TRACKS; y++)
    {
      set_main_buffer(TRELLIS_SCREEN_PERFORM, x, y, TRELLIS_BLACK);
    }
    set_main_buffer(TRELLIS_SCREEN_PERFORM, x, trellisPerformIndex[x], TRELLIS_WHITE);
  }
  set_main_buffer(TRELLIS_SCREEN_PERFORM, prow * 4, 7, TRELLIS_BLUE);
  set_main_buffer(TRELLIS_SCREEN_PERFORM, prow * 4 + 1, 7, TRELLIS_RED);
  set_main_buffer(TRELLIS_SCREEN_PERFORM, prow * 4 + 2, 7, TRELLIS_GREEN);
  set_main_buffer(TRELLIS_SCREEN_PERFORM, prow * 4 + 3, 7, TRELLIS_WHITE);
  writeDisplay();
}
void MyTrellis::drawSelectClip2Edit()
{
  if (trellisScreen == TRELLIS_SCREEN_SELECT_CLIP2EDIT)
    return;
  trellisScreen = TRELLIS_SCREEN_SELECT_CLIP2EDIT;
  trellis.clear();

  for (int y = 0; y < NUM_TRACKS; y++)
  {
    trellis.setLED(TrellisLED[allTracks[y]->parameter[SET_CLIP2_EDIT] + (y * TRELLIS_PADS_X_DIM)]);
  }
  writeDisplay();
}
void MyTrellis::setActiveScreen(uint8_t screenNr)
{
  oldTrellisScreen = trellisScreen;
  trellisScreen = screenNr;
}
uint8_t MyTrellis::getActiveScreen()
{
  return trellisScreen;
}
void MyTrellis::drawPlaymode()
{

  for (int i = 0; i < NUM_ENCODERS; i++)
  {
    int pot = i + (lastPotRow * NUM_ENCODERS);
    int oldValuePos = allTracks[active_track]->get_seqModValue(pot) / 4.13f;

    int oldValueXPos = (oldValuePos % NUM_STEPS) + 1;
    int oldValueYPos = ((oldValuePos / NUM_STEPS) + (pot * 2)) % NUM_TRACKS;
    for (int t = 0; t < NUM_TRACKS; t++)
    {
      trellisOut.set_main_buffer(TRELLIS_SCREEN_PLAYMODE, i, t, TRELLIS_BLACK);
    }
    Serial.printf("drawPot x= %d, y= %d, value = %d, pot= %d \n", oldValueXPos, oldValueYPos, oldValuePos, pot);

    trellisOut.set_main_buffer(TRELLIS_SCREEN_PLAYMODE, oldValueXPos, oldValueYPos, encoder_colour[i]);
  }
  writeDisplay();
}

void MyTrellis::drawPlugin()
{
  int trackChannel = allTracks[active_track]->clip[allTracks[active_track]->parameter[SET_CLIP2_EDIT]].midiChOut;
  if (trackChannel <= NUM_MIDI_OUTPUTS)
    return;
  for (int i = 0; i < NUM_ENCODERS; i++)
  {
    int pot = i + (lastPotRow * NUM_ENCODERS);
    int oldValuePos = getPluginValue(pot) / 4.13f;
    int oldValueXPos = (oldValuePos % NUM_STEPS) + 1;
    int oldValueYPos = ((oldValuePos / NUM_STEPS) + (pot * 2)) % NUM_TRACKS;
    for (int t = 0; t < NUM_TRACKS; t++)
    {
      trellisOut.set_main_buffer(TRELLIS_SCREEN_PLUGIN, i, t, TRELLIS_BLACK);
    }
    // Serial.printf("drawPot x= %d, y= %d, value = %d, pot= %d \n", oldValueXPos, oldValueYPos, oldValuePos, pot);

    trellisOut.set_main_buffer(TRELLIS_SCREEN_PLUGIN, oldValueXPos, oldValueYPos, encoder_colour[i]);
  }
  // writeDisplay();
}
void MyTrellis::drawPotentiometerValue(uint8_t xpos, uint8_t value)
{
  for (int r = 0; r < 2; r++)
  {
    for (int c = 0; c < NUM_STEPS; c++)
    {
      set_main_buffer(trellisScreen, c, r + (xpos * 2), TRELLIS_BLACK);
    }
  }
  trellisOut.writeDisplay();

  // int oldValuePos = value / 4.12f;
  int oldValuePos = map(value, 0, MIDI_CC_RANGE, 0, 31);

  int oldValueXPos = (oldValuePos % NUM_STEPS);
  int oldValueYPos = ((oldValuePos / NUM_STEPS) + (xpos * 2)) % NUM_TRACKS;
  set_main_buffer(trellisScreen, oldValueXPos, oldValueYPos, encoder_colour[xpos]);
  writeDisplay();
}
void MyTrellis::drawMixerValue(uint8_t track, uint8_t value)
{

  for (int c = 0; c < NUM_STEPS; c++)
  {
    set_main_buffer(TRELLIS_SCREEN_MIXER1, c, track, TRELLIS_BLACK);
  }
  writeDisplay();

  int oldValuePos = map(value, 0, MIDI_CC_RANGE, 0, 15);
  int oldValueXPos = (oldValuePos % NUM_STEPS);

  set_main_buffer(TRELLIS_SCREEN_MIXER1, oldValueXPos, track, trackColor[track]);
  writeDisplay();
}
void MyTrellis::drawMixer(uint8_t track, uint8_t value)
{
  for (int c = 0; c < NUM_STEPS; c++)
  {
    set_main_buffer(TRELLIS_SCREEN_MIXER1, c, track, TRELLIS_BLACK);
  }
  int oldValuePos = map(value, 0, MIDI_CC_RANGE, 0, 15);
  int oldValueXPos = (oldValuePos % NUM_STEPS);

  set_main_buffer(TRELLIS_SCREEN_MIXER1, oldValueXPos, track, trackColor[track]);
  if (track == 7)
    writeDisplay();
}

MyTrellis trellisOut;