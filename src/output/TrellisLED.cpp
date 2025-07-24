#include "output/TrellisLED.h"

void MyTrellis::drawTrackGains()
{
    trellis.clear();
    for (int s = 0; s < NUM_STEPS; s++)
    {
        for (int t = 0; t < NUM_TRACKS; t++)
        {
            if (trellisMainGridBuffer[TRELLIS_SCREEN_MIXER][s][t] > TRELLIS_BLACK)
            {
                int _nr = s + (t * TRELLIS_PADS_X_DIM);
                trellis.setLED(TrellisLED[_nr]);
            }
        }
    }
}
void MyTrellis::drawTrackGain(uint8_t trackNr, uint8_t value)
{
    static int8_t oldValue[NUM_TRACKS]{15, 15, 15, 15, 15, 15, 15, 15};
    trellisMainGridBuffer[TRELLIS_SCREEN_MIXER][value][trackNr] = trackColor[trackNr];

    int _old = oldValue[trackNr] + (trackNr * TRELLIS_PADS_X_DIM);
    trellis.clrLED(TrellisLED[_old]);
    int _new = value + (trackNr * TRELLIS_PADS_X_DIM);
    trellis.setLED(TrellisLED[_new]);
    oldValue[trackNr] = value;
}

void MyTrellis::drawSteps()
{
    trellis.clear();
    for (int s = 0; s < NUM_STEPS; s++)
    {
        for (int t = 0; t < NUM_TRACKS; t++)
        {
            if (trellisMainGridBuffer[t][s][t] > TRELLIS_BLACK)
            {
                int _nr = s + (t * TRELLIS_PADS_X_DIM);
                trellis.setLED(TrellisLED[_nr]);
            }
        }
    }
}
void MyTrellis::drawTrackStep(uint8_t trackNr, uint8_t stepNr, uint8_t _clipNr)
{
    trellisMainGridBuffer[_clipNr][stepNr][trackNr] = trackColor[trackNr] + 20 * _clipNr;
}

void MyTrellis::drawArranger(uint8_t _arrangerpage) {}
void MyTrellis::drawTrackArranger(uint8_t trackNr, uint8_t bar, uint8_t clipNr) {}

void MyTrellis::drawPerformer() {}
void MyTrellis::drawClipLauncher() {}

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
void MyTrellis::drawLoadSavePage()
{

    if (trellisScreen == TRELLIS_SCREEN_SAVE_LOAD)
        return;
    Serial.println(trellisScreen);
    clearMainGridNow();
    oldTrellisScreen = trellisScreen;
    trellisScreen = TRELLIS_SCREEN_SAVE_LOAD;
    for (int i = 0; i < MAX_SONGS; i++)
    {
        trellis.setLED(TrellisLED[i]);
        trellis.setLED(TrellisLED[i + TRELLIS_PADS_X_DIM]);
        trellis.setLED(TrellisLED[15 + TRELLIS_PADS_X_DIM]);
        trellis.setLED(TrellisLED[i + (3 * TRELLIS_PADS_X_DIM)]);
    }
    trellis.writeDisplay();
}
void MyTrellis::drawPreviousScreen()
{
    clearMainGridNow();
    trellisScreen = oldTrellisScreen;
}
void MyTrellis::clearMainGridNow()
{
    trellis.clear();
    trellis.writeDisplay();
}
void MyTrellis::writeNow()
{
    trellis.writeDisplay();
}
void MyTrellis::setBrightness(uint8_t brigthness){
    trellis.setBrightness(brigthness);

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
    trellis.writeDisplay();
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
    trellis.writeDisplay();
    trellis.writeDisplay();
}
void MyTrellis::drawPiano()
{
    if (trellisScreen != TRELLIS_SCREEN_PIANO)
        return;
    trellisScreen = TRELLIS_SCREEN_PIANO;
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
            trellis_set_main_buffer(TRELLIS_SCREEN_PIANO, x, y, _color);
        }
    }
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
    trellis.writeDisplay();
}

MyTrellis trellisOut;