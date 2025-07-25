#ifndef NEOTRELLIS_H
#define NEOTRELLIS_H

#include "Arduino.h"
#include "Adafruit_NeoTrellis.h"
#include "ownLibs/Adafruit_Trellis.h"
#include <Wire.h>
#include "projectVariables.h"
#include "output/TrellisLED.h"
#include "ownLibs/myClock.h"
#include "output/tft.h"

#include "Track.h"
extern Adafruit_TrellisSet trellis;
void trellis_show_tft_mixer();
void trellis_play_mixer();
void trellis_perform();


extern bool neotrellisPressed[X_DIM * Y_DIM];

extern uint8_t trellisPianoTrack;
extern int trellisTrackColor[9];
extern int trellisControllBuffer[X_DIM][Y_DIM];

extern Adafruit_NeoTrellis t_array[Y_DIM / 4][X_DIM / 4];
extern Adafruit_MultiTrellis neotrellis;

extern const long trellisReadInterval;             // interval at which to blink (milliseconds)
extern const unsigned long neotrellisReadInterval; // interval at which to blink (milliseconds)
extern const long trellisRestartInterval;          // interval at which to blink (milliseconds)
extern unsigned long trellisRestartPreviousMillis; // will store last time LED was updated
extern unsigned long trellisReadPreviousMillis;    // will store last time LED was updated
extern bool trellisPressed[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM];
extern const uint8_t TrellisLED[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM];
extern uint8_t bar2edit;

TrellisCallback blink(keyEvent evt);
uint16_t rgb24to565(uint32_t color24);
void neotrellis_setup(int dly);

void neotrellis_set_control_buffer(int _x, int _y, int _color);
void neotrellis_recall_control_buffer();
void neotrellis_update();
void neotrellis_assign_start_buffer();
void neotrellis_set_potRow();
void neotrellis_start_clock();
void neotrellis_stop_clock();

// 2nd row
void neotrellis_SetCursor(uint8_t maxY);
// 3rd row
void trellis_show_arranger();
void neotrellis_perform_set_active();
void set_performCC(uint8_t XPos, uint8_t YPos, const char *name);

void set_perform_page(uint8_t row);

void neotrellis_show_tft_seqMode();
void neotrellis_show_tft_plugin();
void trellis_show_tft_mixer();
void trellis_play_clipLauncher();

void neo_trellis_select_trackClips();
void neo_trellis_set_brightness();
void neo_trellis_save_load();

void neotrellis_set_mute();
void neotrellis_set_solo();
void neotrellis_set_fast_record();
void neotrellis_set_piano();
void trellis_play_piano();
void trellis_set_arranger();
void trellis_setStepsequencer();

uint8_t getPressedKey();
bool isPressed();
void revertPressedKey();

//input stuff
void trellis_read();
void trellis_update();
#endif