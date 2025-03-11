#ifndef TFT_H
#define TFT_H
#include "ownLibs/Adafruit_ST7796S_kbv.h"
//#include <ILI9341_t3n.h>
//#include <ili9341_t3n_font_Arial.h> // from ILI9341_t3
#include "input/touch.h"
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
#define POSITION_POTROW_BUTTON 6
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

#define POSITION_TEXT_ARRANGERLINE_TOP 6
#define POSITION_TEXT_ARRANGERLINE_BOTTOM 11
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

//extern ILI9341_t3n tft;
extern Adafruit_ST7796S_kbv tft;

// Display
extern unsigned long infoboxTimeAtCall;
extern unsigned long infoboxTimeAtPress;
extern unsigned long infoboxWaitingTime;
extern bool infoboxShow;
extern bool infoboxClear;

void tft_setup(int dly);
void clearWorkSpace();
void startUpScreen();
void drawActiveRecording(bool _recState);
void drawActivePlaying(bool _playstate);
void drawPositionCounter();
void get_infobox_background();
void set_infobox_background(int _DisplayTime);
void reset_infobox_background();
void set_infobox_next_line(uint8_t _lineNumber); //_lineNumber must be bigger 0
void show_active_page_info(const char *_pagename, uint8_t _pagenumber);
void moveCursor(int pixelOnX, int pixelOnY, int cursorDeltaX, int cursorDeltaY);

void tft_show();

void drawPot(int XPos, uint8_t YPos, int dvalue, const char *dname);
void drawEnvelope(uint8_t YPos, uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release);
void draw_sequencer_arranger_parameter(uint8_t _track, uint8_t _encoder, const char *_name, int _value, const char *_valuedName);
void draw_value_box(uint8_t lastPRow, uint8_t XPos, uint8_t YPos, uint8_t offest_X, int offset_Y, int _value, const char *name, int color, uint8_t _size, bool drawRect, bool drawFilling);

// clock
void drawstepPosition();
void drawbarPosition();
void clear_positionPointer();

// songmode

void gridSongMode(int songpageNumber);
void drawsongmodepageselector(int songpageNumber);
void draw_arranger_parameters(uint8_t lastProw);
void draw_arrangment_line(uint8_t _trackNr, uint8_t _bar);
void draw_arrangment_lines(uint8_t _track, uint8_t _page); // b= active page
void draw_arrangerLine_value(uint8_t _trackNr, uint8_t _bar, int value, int y_offset);


// step sequencer
void drawStepSequencerStatic();
void draw_Notenames();
void drawOctaveTriangle();
void drawOctaveNumber();
void draw_Clipselector();

void draw_stepSequencer_parameters(uint8_t lastProw);
void draw_note_on_tick(uint8_t _note, uint8_t _when);
void draw_notes_in_grid();

void draw_MIDI_CC(uint8_t XPos, uint8_t YPos);
void draw_MIDI_CC_screen();
void draw_edit_presetNr_CC(const char *label, uint8_t value, uint8_t row_offset);

void draw_mixer();
void draw_mixer_FX_page1();
void draw_mixer_FX_page2();

void draw_clip_launcher();

#endif