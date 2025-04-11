#ifndef MYCLOCK_H
#define MYCLOCK_H

#include "Arduino.h"
#include "ownLibs/Adafruit_ST7796S_kbv.h"
//#include <ILI9341_t3n.h>
//#include <ili9341_t3n_font_Arial.h> // from ILI9341_t3
//#include "hw_stuff.h"
//#include "hardware/tftClass.h"

#include "uClock.h"
#include "input/encoder.h"
#include <projectVariables.h>
#include "output/tft.h"
#include <MIDI.h>
#include <USBHost_t36.h>
//extern tftClass *mytft;
void sendStart();
void sendStop();
void sendClock();
void clock_to_notes(int _tick);
extern bool neotrellisPressed[X_DIM * Y_DIM];

class MyClock
{
public:
  //static ILI9341_t3n *tft; // Pointer to the display object
  static MyClock *instance;
  static uint8_t tempo;
  static uint8_t startOfLoop;
  static uint8_t endOfLoop;
  static uint8_t stepTick;
  static uint8_t barTick;
  static File clockFile;
  static bool isPlaying;
  static bool syncToExtern;
  MyClock(int index);
  void setup();

  static void onSync24Callback(uint32_t tick);
  static void onStepCallback(uint32_t tick);

  static void onClockStop();
  static void onClockStart();

  void set_tempo(uint8_t _encoder);
  void set_start();
  void set_stop();

  void draw_clock_option(uint8_t x, uint8_t v);

  void set_syncToExtern(uint8_t _encoder);
  void set_start_of_loop(uint8_t n);
  void set_end_of_loop(uint8_t n);
  void save_clock(uint8_t _songNr);
  void load_clock(uint8_t _songNr);
};
extern MyClock myClock;
#endif // MY_CLOCK