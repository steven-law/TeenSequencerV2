#ifndef TFT_H
#define TFT_H
#include <ILI9341_t3n.h>
#include <ili9341_t3n_font_Arial.h> // from ILI9341_t3
#include "Track.h"
#include <projectVariables.h>
#include <ownLibs/myClock.h>

// Startscreen
#define POSITION_ARR_BUTTON 18

#define POSITION_SCALE_BUTTON 16
#define POSITION_LOAD_BUTTON 15
#define POSITION_SAVE_BUTTON 13
#define POSITION_STOP_BUTTON 10
#define POSITION_PLAY_BUTTON 8
#define POSITION_RECORD_BUTTON 7
#define STARTUPSCREEN 0
#define STEP_SEQUENCER_VIEW 1
#define ARRANGMENT_VIEW 2

#define POSITION_BAR_BUTTON 5

#define GRID_LENGTH_HOR 256
#define GRID_LENGTH_VERT 192

#define SONG_POSITION_POINTER_Y 228
#define BAR_POSITION_POINTER_Y 232
#define STEP_POSITION_POINTER_Y 236
#define POSITION_POINTER_THICKNESS 3

#define phraseSegmentLength 16
#define INFO_BOX_WIDTH 200
#define INFO_BOX_HEIGTH 120
#define INFOBOX_OFFSET 60
#define INFOBOX_TEXT_OFFSET 80

#define OCTAVE_CHANGE_LEFTMOST 18
#define OCTAVE_CHANGE_RIGHTMOST 20
#define OCTAVE_CHANGE_UP_TOPMOST 2
#define OCTAVE_CHANGE_UP_BOTTOMMOST 3
#define OCTAVE_CHANGE_DOWN_TOPMOST 4
#define OCTAVE_CHANGE_DOWN_BOTTOMMOST 5

#define OCTAVE_CHANGE_TEXT 3

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
#define TRELLIS_BLACK 0x000000
// #define TRELLIS_LIGHTBLUE #ADD8E6

#define TRELLIS_1 0x000066
#define TRELLIS_2 579

extern ILI9341_t3n tft;
// Display
extern unsigned long infoboxTimeAtCall;
extern unsigned long infoboxTimeAtPress;
extern unsigned long infoboxWaitingTime;
extern bool infoboxShow;
extern bool infoboxClear;

void tft_setup(int dly);
void clearWorkSpace();
void startUpScreen();

void get_infobox_background();
void set_infobox_background(int _DisplayTime);
void reset_infobox_background();
void set_infobox_next_line(byte _lineNumber); //_lineNumber must be bigger 0
void show_active_page_info(const char *_pagename, byte _pagenumber);
void moveCursor(int pixelOnX, int pixelOnY,  int cursorDeltaX, int cursorDeltaY);

void tft_show();

void drawPot(int XPos, byte YPos, int dvalue, const char *dname);
void drawEnvelope(byte YPos, byte attack, byte decay, byte sustain, byte release);
void draw_sequencer_arranger_parameter(byte _track, byte _encoder, const char *_name, int _value, const char *_valuedName);
void draw_value_box(byte lastPRow, byte XPos, byte YPos, byte offest_X, int offset_Y, int _value, const char *name, int color, byte _size, bool drawRect, bool drawFilling);

//clock
    void drawstepPosition();
    void drawbarPosition();

// songmode

void gridSongMode(int songpageNumber);
void drawsongmodepageselector();
void draw_arranger_parameters(byte lastProw);
void draw_arrangment_line(byte _trackNr, byte _bar);
void draw_arrangment_lines(byte _track, byte _page); // b= active page

void draw_offset_arranger(byte _trackNr, byte _bar);
void draw_clipNr_arranger(byte _trackNr, byte _bar);

// step sequencer
void drawStepSequencerStatic();
void draw_Notenames();
void drawOctaveTriangle();
void draw_Clipselector();

void draw_stepSequencer_parameters(byte lastProw);
void draw_note_on_tick(byte _note, byte _when);
void draw_notes_in_grid();

void draw_MIDI_CC(byte XPos, byte YPos);
void draw_MIDI_CC_screen();
void draw_edit_presetNr_ccChannel(byte n, byte lastProw);
void draw_edit_presetNr_ccValue(byte n, byte lastProw);

void draw_mixer();
void draw_mixer_FX_page1();
void draw_mixer_FX_page2();

#endif