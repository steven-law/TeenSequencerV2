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

    int trellisMainGridBuffer[TRELLIS_MAX_PAGES][TRELLIS_PADS_X_DIM][TRELLIS_PADS_Y_DIM];
    void setup(int dly);
    void clearMainGridNow();
    void writeDisplay();
    void setBrightness(uint8_t brigthness);
    void set_main_buffer(int _page, int _x, int _y, int color);
    void recall_main_buffer(int _page);
    int get_main_buffer(int _page, int _x, int _y);
    void show_clockbar(uint8_t trackNr, uint8_t step);
    void drawSelectClip2Edit();
    void drawLoadSavePage();
    void drawSelectMixerPage();
    void drawSelectArrangerPage();
    void drawPreviousScreen();
    void trellis_assign_start_buffer(uint8_t _lastPotRow);
    void drawPiano();
};

extern MyTrellis trellisOut;

#endif // TRELLISLED_H