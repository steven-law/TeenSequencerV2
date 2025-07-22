#ifndef TRELLISLED_H
#define TRELLISLED_H

#include <Arduino.h>
#include <Audio.h>
#include "projectVariables.h"
#include "ownLibs/Adafruit_Trellis.h"
extern Adafruit_TrellisSet trellis;
extern int trellisMainGridBuffer[TRELLIS_MAX_PAGES][TRELLIS_PADS_X_DIM][TRELLIS_PADS_Y_DIM];
extern const uint8_t TrellisLED[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM];
class MyTrellis
{
public:
    void drawTrackGains();
    void drawTrackGain(uint8_t trackNr, uint8_t value); // value= 0-15
    void drawTrackFXs();
    void drawTrackFX(uint8_t trackNr, uint8_t FxType, uint8_t value); // value= 0-3

    void drawSelectClip2Edit( uint8_t activeClip);
    void drawSteps();
    void drawTrackStep(uint8_t trackNr, uint8_t stepNr, uint8_t _clipNr);

    void drawArranger(uint8_t _arrangerpage);
    void drawTrackArranger(uint8_t trackNr, uint8_t bar, uint8_t clipNr);

    void drawPerformer();
    void drawClipLauncher();

    void drawLoadSavePage();
    void drawSelectMixerPage();
    void drawSelectArrangerPage();
};

#endif // TRELLISLED_H