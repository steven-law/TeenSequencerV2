#ifndef PROJECT_VARIABLES_H
#define PROJECT_VARIABLES_H
#include "Arduino.h"
#include "SD.h"
#include <ILI9341_t3n.h>

extern File myFile;

#define NO_VALUE 6789
#define NO_NAME "NO_NAME"

#define NUM_PLUGINS 11

#define NUM_TRACKS 8
#define NUM_STEPS 16

// encoder
#define NUM_ENCODERS 4
// encoder functions
#define INPUT_FUNCTIONS_FOR_CURSOR 0
#define INPUT_FUNCTIONS_FOR_SEQUENCER 1
#define INPUT_FUNCTIONS_FOR_SEQUENCER_ALT 2
#define INPUT_FUNCTIONS_FOR_ARRANGER 3
#define INPUT_FUNCTIONS_FOR_ARRANGER_ALT 4
#define INPUT_FUNCTIONS_FOR_PLUGIN 5
#define INPUT_FUNCTIONS_FOR_SEQUENCER_MODES 6
#define INPUT_FUNCTIONS_FOR_MIXER1 7
#define INPUT_FUNCTIONS_FOR_MIXER2 8
#define INPUT_FUNCTIONS_FOR_MIXER3 9
#define INPUT_FUNCTIONS_FOR_FX1 10
#define INPUT_FUNCTIONS_FOR_FX2 11
#define INPUT_FUNCTIONS_FOR_FX3 12
#define INPUT_FUNCTIONS_FOR_PERFORM 13
// clips
#define MAX_CLIPS 9
#define NUM_USER_CLIPS 7
#define NUM_PRESETS 8
#define MAX_TICKS 96
#define MAX_VOICES 12
#define MAX_SONGS 9

#define SET_STEP_FX 2
#define SET_VELO2SET 3
#define SET_SEQUENCE_LENGTH 4
#define SET_STEP_DIVIVISION 5
#define SET_STEP_LENGTH 6
#define SET_OCTAVE 7
#define SET_SEQ_MODE 8
#define SET_MIDICH_OUT 9

#define SET_CLIP2_EDIT 11
// pages
#define TRACK_1_PAGE 0
#define TRACK_2_PAGE 1
#define TRACK_3_PAGE 2
#define TRACK_4_PAGE 3
#define TRACK_5_PAGE 4
#define TRACK_6_PAGE 5
#define TRACK_7_PAGE 6
#define TRACK_8_PAGE 7
#define STARTUPSCREEN_PAGE 8
// #define EMPTY 9
#define SONGMODE_PAGE_1 0
#define SONGMODE_PAGE_2 1
#define SONGMODE_PAGE_3 2
#define SONGMODE_PAGE_4 3
#define SONGMODE_PAGE_5 4
#define SONGMODE_PAGE_6 5
#define SONGMODE_PAGE_7 6
#define SONGMODE_PAGE_8 7
#define SONGMODE_PAGE_9 8
#define SONGMODE_PAGE_10 9
#define SONGMODE_PAGE_11 10
#define SONGMODE_PAGE_12 11
#define SONGMODE_PAGE_13 12
#define SONGMODE_PAGE_14 13
#define SONGMODE_PAGE_15 14
#define SONGMODE_PAGE_16 15

#define STEP_FRAME_W 16
#define STEP_FRAME_H 16
#define TRACK_FRAME_H 24
// for arranger
#define BARS_PER_PAGE 16
// for sequencer
#define SEQ_GRID_LEFT 30
#define SEQ_GRID_RIGHT (18 * STEP_FRAME_W) - 2
#define SEQ_GRID_TOP 1
#define SEQ_GRID_BOTTOM 12
#define SEQUENCER_OPTIONS_VERY_RIGHT 18
#define SEQUENCER_OPTIONS_RIGHT 16

#define NUM_MIDI_OUTPUTS 48

#define CH_PLUGIN_1 NUM_MIDI_OUTPUTS + 1
#define CH_PLUGIN_2 NUM_MIDI_OUTPUTS + 2
#define CH_PLUGIN_3 NUM_MIDI_OUTPUTS + 3
#define CH_PLUGIN_4 NUM_MIDI_OUTPUTS + 4
#define CH_PLUGIN_5 NUM_MIDI_OUTPUTS + 5
#define CH_PLUGIN_6 NUM_MIDI_OUTPUTS + 6
#define CH_PLUGIN_7 NUM_MIDI_OUTPUTS + 7
#define CH_PLUGIN_8 NUM_MIDI_OUTPUTS + 8
#define CH_PLUGIN_9 NUM_MIDI_OUTPUTS + 9
#define MAX_OUTPUTS NUM_MIDI_OUTPUTS + NUM_PLUGINS
#define MIDI_CC_RANGE 127
#define MIDI_CC_RANGE_FLOAT 127.00

// plugins
#define NUM_PLUGIN_PRESETS 8
extern const char FLASHMEM *songNames[MAX_SONGS];

extern char _trackname[20];
extern int pixelTouchX;
extern int gridTouchY;
extern byte lastPotRow;
extern byte activeScreen;
extern byte trellisScreen;
extern byte active_track;
extern byte arrangerpage;
extern bool change_plugin_row;
extern const int encoder_colour[NUM_ENCODERS];
extern unsigned long neotrellisReadPreviousMillis; // will store last time LED was updated
extern uint16_t tftRAM[16][16];

#define Y_DIM 12 // number of rows of key
#define X_DIM 4  // number of columns of keys
#define TRELLIS_PADS_X_DIM 16
#define TRELLIS_PADS_Y_DIM 8
#define TRELLIS_INT_PIN 41
#define NUMTRELLIS 8
#define TRELLIS_BLACK 0x000000
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
#define numKeys (NUMTRELLIS * 16)
#define TRELLIS_SCREEN_SEQUENCER_CLIP_0 0
#define TRELLIS_SCREEN_SEQUENCER_CLIP_1 1
#define TRELLIS_SCREEN_SEQUENCER_CLIP_2 2
#define TRELLIS_SCREEN_SEQUENCER_CLIP_3 3
#define TRELLIS_SCREEN_SEQUENCER_CLIP_4 4
#define TRELLIS_SCREEN_SEQUENCER_CLIP_5 5
#define TRELLIS_SCREEN_SEQUENCER_CLIP_6 6
#define TRELLIS_SCREEN_SEQUENCER_CLIP_7 7
#define TRELLIS_SCREEN_SEQUENCER_CLIP_8 8

#define TRELLIS_SCREEN_PIANO 10
#define TRELLIS_SCREEN_SONGPAGE_SELECTION 11
#define TRELLIS_SCREEN_MIXER 12
#define TRELLIS_SCREEN_MIXER1 13
#define TRELLIS_SCREEN_PERFORM 14

#define TRELLIS_SCREEN_ARRANGER_1 15
#define TRELLIS_SCREEN_ARRANGER_2 16
#define TRELLIS_SONGMODE_PAGE_16 TRELLIS_SCREEN_ARRANGER_1 + 16

#define TRELLIS_SCREEN_STARTUP 99
#define TRELLIS_MAX_PAGES TRELLIS_SONGMODE_PAGE_16 + 1

extern bool trellisShowClockPixel[TRELLIS_PADS_Y_DIM];
extern int trellisTrackColor[9];
extern int trackColor[9];

extern byte trellisPerformIndex[TRELLIS_PADS_X_DIM];
extern byte performCC[TRELLIS_PADS_X_DIM];
extern bool neotrellisPressed[X_DIM * Y_DIM];
extern bool trellisPressed[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM];
extern byte FLASHMEM gateOutputPin[8];

extern bool updateTFTScreen;
extern const char FLASHMEM *CCnames[129];
extern const char FLASHMEM *seqModname[5];
extern const char FLASHMEM *channelOutNames[MAX_OUTPUTS + 1];
extern const char FLASHMEM *noteNames[12];
// screen
#define POSITION_BPM_BUTTON 11
#define POSITION_START_LOOP_BUTTON 16
#define POSITION_END_LOOP_BUTTON 18
#define TRELLIS_BUTTON_ENTER 8

#endif