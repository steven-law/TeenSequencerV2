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
void MyTrellis::drawTrackFXs() {}
void MyTrellis::drawTrackFX(uint8_t trackNr, uint8_t FxType, uint8_t value) {}

void MyTrellis::drawSelectClip2Edit(uint8_t activeClip)
{
    trellis.clear();
    for (int t = 0; t < NUM_TRACKS; t++)
    {
        int _clip = activeClip + (t * TRELLIS_PADS_X_DIM);
        trellis.setLED(TrellisLED[_clip]);
    }
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

void MyTrellis::drawLoadSavePage() {}
void MyTrellis::drawSelectMixerPage() {}
void MyTrellis::drawSelectArrangerPage() {}