#ifndef NEOTRELLIS_H
#define NEOTRELLIS_H

#include "Arduino.h"
#include "Adafruit_NeoTrellis.h"
#include "ownLibs/Adafruit_Trellis.h"
#include <Wire.h>
#include "projectVariables.h"
#include "ownLibs/myClock.h"
#include "output/tft.h"

#include "Track.h"
extern Adafruit_TrellisSet trellis;
void trellis_show_tft_mixer();
void trellis_play_mixer();
void trellis_perform();
#define NEOTRELLIS_INT_PIN 39


#define TRELLIS_POTROW 0
#define TRELLIS_BUTTON_RECORD 1
#define TRELLIS_START_CLOCK 2
#define TRELLIS_STOP_CLOCK 3

#define TRELLIS_BUTTON_LEFT 4
#define TRELLIS_BUTTON_RIGHT 5
#define TRELLIS_BUTTON_UP 6
#define TRELLIS_BUTTON_DOWN 7

#define TRELLIS_BUTTON_SHIFT 9
#define TRELLIS_BUTTON_PIANO 10
#define TRELLIS_BUTTON_SEQMODE 11

#define TRELLIS_BUTTON_ARRANGER 12
#define TRELLIS_BUTTON_MIXER 13
#define TRELLIS_BUTTON_PLUGIN 14
#define TRELLIS_BUTTON_SEQUENCER 15

// See https://www.w3schools.com/colors/colors_picker.asp
#define TRELLIS_RED 0xFF0000
#define TRELLIS_ORANGE 0xB34700
#define TRELLIS_YELLOW 0xFFFF00
#define TRELLIS_OLIVE 0x66DD00
#define TRELLIS_GREEN 0x008000
#define TRELLIS_AQUA 0x00FF66
#define TRELLIS_TEAL 0x00BFFF
#define TRELLIS_BLUE 0x0080FF
#define TRELLIS_NAVY 0x000080
#define TRELLIS_MAROON 0x800000
#define TRELLIS_PURPLE 0x800080
#define TRELLIS_PINK 0xFF66B3
#define TRELLIS_WHITE 0xFFFFFF

// #define TRELLIS_LIGHTBLUE #ADD8E6

#define TRELLIS_1 0x000066
#define TRELLIS_2 579

#define TRELLIS_SCREEN_PIANO 10
#define TRELLIS_SCREEN_SONGPAGE_SELECTION 11
#define TRELLIS_SCREEN_MIXER 12
#define TRELLIS_SCREEN_MIXER1 13
#define TRELLIS_SCREEN_PERFORM 14
#define TRELLIS_SCREEN_CLIPLAUNCHER 15

#define TRELLIS_SCREEN_ARRANGER_1 16
#define TRELLIS_SCREEN_ARRANGER_2 17
#define TRELLIS_SONGMODE_PAGE_16 TRELLIS_SCREEN_ARRANGER_1 + 16

#define TRELLIS_SCREEN_STARTUP 99
#define TRELLIS_MAX_PAGES TRELLIS_SONGMODE_PAGE_16 + 1

extern bool neotrellisPressed[X_DIM * Y_DIM];

extern uint8_t trellisPianoTrack;
extern int trellisTrackColor[9];
extern int trellisControllBuffer[X_DIM][Y_DIM];

extern Adafruit_NeoTrellis t_array[Y_DIM / 4][X_DIM / 4];
extern Adafruit_MultiTrellis neotrellis;

extern const long trellisReadInterval;             // interval at which to blink (milliseconds)
extern const long neotrellisReadInterval;          // interval at which to blink (milliseconds)
extern const long trellisRestartInterval;          // interval at which to blink (milliseconds)
extern unsigned long trellisRestartPreviousMillis; // will store last time LED was updated
extern unsigned long trellisReadPreviousMillis;    // will store last time LED was updated
extern bool trellisPressed[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM];
extern const uint8_t TrellisLED[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM];
extern int trellisMainGridBuffer[TRELLIS_MAX_PAGES][TRELLIS_PADS_X_DIM][TRELLIS_PADS_Y_DIM];
extern uint8_t bar2edit;

TrellisCallback blink(keyEvent evt);

void neotrellis_setup(int dly);
void neotrellis_assign_start_buffer();
void neotrellis_set_control_buffer(int _x, int _y, int _color);
void neotrellis_recall_control_buffer();
void neotrellis_update();

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
void neotrellis_show_piano();


void trellis_setup(int dly);
void trellis_read();
void trellis_update();
void trellis_writeDisplay();

void trellis_set_main_buffer(int _page, int _x, int _y, int color);
void trellis_assign_start_buffer(uint8_t _lastPotRow);
void trellis_clear_main_buffer(int _page);
int trellis_get_main_buffer(int _page, int _x, int _y);
void trellis_recall_main_buffer(int _page);

void trellis_show_clockbar(uint8_t trackNr, uint8_t step);
#endif