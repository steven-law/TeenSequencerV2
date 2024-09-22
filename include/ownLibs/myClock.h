#ifndef MYCLOCK_H
#define MYCLOCK_H

#include "Arduino.h"
#include <ILI9341_t3n.h>
#include <ili9341_t3n_font_Arial.h> // from ILI9341_t3
//#include "hw_stuff.h"
//#include "hardware/tftClass.h"

#include "uClock.h"
#include "input/encoder.h"
#include <projectVariables.h>

//extern tftClass *mytft;
void sendClock();
void clock_to_notes(int _tick);
extern bool neotrellisPressed[X_DIM * Y_DIM];

class MyClock
{
public:
  //static ILI9341_t3n *tft; // Pointer to the display object
  static MyClock *instance;
  static byte tempo;
  static byte startOfLoop;
  static byte endOfLoop;
  static byte stepTick;
  static byte barTick;
  static bool isPlaying;
  MyClock(int index);
  void setup();

  static void onSync24Callback(uint32_t tick);
  static void onStepCallback(uint32_t tick);

  static void onClockStop();
  static void onClockStart();

  void set_tempo(byte _encoder);
  void set_start();
  void set_stop();

  void draw_clock_option(byte x, byte v);


  void set_start_of_loop(byte n);
  void set_end_of_loop(byte n);
  void save_clock(byte _songNr);
  void load_clock(byte _songNr);
};
extern MyClock myClock;
#endif // MY_CLOCK