#ifndef TRELLISLED_H
#define TRELLISLED_H

#include <Arduino.h>
#include <Audio.h>
#include "projectVariables.h"
#include "output/tft.h"
#include "Track.h"
#include "ownLibs/Adafruit_Trellis.h"
extern Adafruit_TrellisSet trellis;
extern int trellisMainGridBuffer[TRELLIS_MAX_PAGES][TRELLIS_PADS_X_DIM][TRELLIS_PADS_Y_DIM];
extern const uint8_t TrellisLED[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM];

class MyTrellis
{
public:
    uint8_t trellisScreen;
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
    void recall_main_buffer();
    int get_main_buffer(int _page, int _x, int _y);
    void show_clockbar(uint8_t trackNr, uint8_t step);
    void setActiveScreen(uint8_t screenNr);
    uint8_t getActiveScreen();
    void drawSelectClip2Edit();
    void drawLoadSavePage();
    void drawSelectMixerPage();
    void drawSelectArrangerPage();
    void drawPreviousScreen();
    void trellis_assign_start_buffer(uint8_t _lastPotRow);
    void drawPiano();
    void drawPerformPotrow(uint8_t prow);
    void drawPerform(uint8_t prow);
    void drawPlaymode();
    void drawPlugin();
    void drawPotentiometerValue(uint8_t xpos, uint8_t value);
    void drawMixerValue(uint8_t track, uint8_t value);
};

extern MyTrellis trellisOut;

#endif // TRELLISLED_H