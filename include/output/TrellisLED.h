#ifndef TRELLISLED_H
#define TRELLISLED_H

#include <Arduino.h>
#include <Audio.h>
#include "projectVariables.h"
#include "output/tft.h"
#include "Track.h"
#include "ownLibs/Adafruit_Trellis.h"
// #include "input/trellis.h"
extern Adafruit_TrellisSet trellis;
extern int trellisMainGridBuffer[TRELLIS_MAX_PAGES][TRELLIS_PADS_X_DIM][TRELLIS_PADS_Y_DIM];
extern const uint8_t TrellisLED[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM];

class MyTrellis
{
public:
    uint8_t oldTrellisScreen = 0;
    MyTrellis()
    {
    }
    void setup(int dly);
    void clearMainGridNow();
    void writeNow();
    void setBrightness(uint8_t brigthness);
    void drawTrackGains();
    void drawTrackGain(uint8_t trackNr, uint8_t value); // value= 0-15

    void drawSelectClip2Edit( );
    void drawSteps();
    void drawTrackStep(uint8_t trackNr, uint8_t stepNr, uint8_t _clipNr);

    void drawArranger(uint8_t _arrangerpage);
    void drawTrackArranger(uint8_t trackNr, uint8_t bar, uint8_t clipNr);

    void drawPerformer();
    void drawClipLauncher();

    void drawLoadSavePage();
    void drawSelectMixerPage();
    void drawSelectArrangerPage();
    void drawPreviousScreen();
    void trellis_assign_start_buffer(uint8_t _lastPotRow);
    void drawPiano();
};

extern MyTrellis trellisOut;

#endif // TRELLISLED_H