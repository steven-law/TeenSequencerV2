#ifndef TOUCH_H
#define TOUCH_H


#include <XPT2046_Touchscreen.h>
#include <projectVariables.h>
extern XPT2046_Touchscreen ts;

#define CS_PIN 14                                                                     // alternate Pins: any digital pin
// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 440
#define TS_MINY 300
#define TS_MAXX 3850
#define TS_MAXY 3800

extern bool tsTouched ;
void touch_setup();
void touch_update();


#endif
