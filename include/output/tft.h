#ifndef TFT_H
#define TFT_H
#include "ownLibs/Adafruit_ST7796S_kbv.h"
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
//#define POSITION_POTROW_BUTTON 6
#define POSITION_POTROW_BUTTON 475
#define STARTUPSCREEN 0
#define STEP_SEQUENCER_VIEW 1
#define ARRANGMENT_VIEW 2

#define POSITION_BAR_BUTTON 5

#define GRID_LENGTH_HOR 16 * STEP_FRAME_W
#define GRID_LENGTH_VERT 12 * STEP_FRAME_H

#define SONG_POSITION_POINTER_Y (14 * STEP_FRAME_H) + 4
#define BAR_POSITION_POINTER_Y (14 * STEP_FRAME_H) + 8
#define STEP_POSITION_POINTER_Y (14 * STEP_FRAME_H) + 12
#define POSITION_POINTER_THICKNESS 3

#define POSITION_TEXT_ARRANGERLINE_TOP 8
#define POSITION_CC_ARRANGERLINE_TOP 7
#define POSITION_TEXT_ARRANGERLINE_BOTTOM 14
#define POSITION_CC_ARRANGERLINE_BOTTOM 13
#define phraseSegmentLength 16
#define INFO_BOX_WIDTH 300
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



// extern ILI9341_t3n tft;
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
void draw_infobox(const char *info1, int8_t value1, const char *info2,int8_t value2);
void draw_infobox_text(const char *info1, const char * value1);
void moveCursor(int pixelOnX, int pixelOnY, int cursorDeltaX, int cursorDeltaY);
void draw_potRow();
void tft_show();
void tft_showTrackColors();
void drawPot(int XPos, uint8_t YPos, int dvalue, const char *dname);
void drawEnvelope(uint8_t YPos, uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release);
void draw_sequencer_arranger_parameter(uint8_t _track, uint8_t _encoder, const char *_name, int _value, const char *_valuedName);
void draw_value_box(uint8_t lastPRow, uint8_t XPos, uint8_t YPos, uint8_t offest_X, int offset_Y, int _value, const char *name, int color, float _size, bool drawRect, bool drawFilling);

// clock
void drawstepPosition();
void drawbarPosition();
void clear_positionPointer();

// songmode

void gridSongMode(int songpageNumber);
void drawsongmodepageselector(int songpageNumber);
void draw_arranger_parameters();
void draw_arrangment_line(uint8_t _trackNr, uint8_t _bar);
void draw_arrangment_lines(uint8_t _track, uint8_t _page); // b= active page
void draw_arrangerLine_value(uint8_t _trackNr, uint8_t _bar, int value, int y_offset);

// step sequencer
void drawStepSequencerStatic();
void draw_Notenames();
void drawOctaveTriangle();
void drawOctaveNumber();
void draw_Clipselector();

void draw_stepSequencer_parameters();
void draw_note_on_tick(uint8_t _voice, uint8_t _when);
void draw_notes_in_grid();
void erase_note_on_tick(uint8_t _voice, uint8_t _when, uint8_t note_length);
void erase_notes_in_grid(uint8_t _voice, uint8_t _when);


void draw_MIDI_CC(uint8_t XPos, uint8_t YPos);
void draw_MIDI_CC_screen();
void draw_edit_presetNr_CC(const char *label, uint8_t value, uint8_t row_offset);

void draw_mixer();
void draw_mixer_FX_page1();
void draw_mixer_FX_page2();

void draw_clip_launcher();

#endif