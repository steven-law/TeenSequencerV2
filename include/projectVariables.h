#ifndef PROJECT_VARIABLES_H
#define PROJECT_VARIABLES_H
#include "Arduino.h"
#include "Audio.h"
#include "SD.h"
#include "ownLibs/Adafruit_ST7796S_kbv.h"

#define NEOTRELLIS_INT_PIN 39

#define TRELLIS_POTROW 0
#define TRELLIS_BUTTON_SAVELOAD 1
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
#define TRELLIS_BLACK 0x000000
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
#define TRELLIS_SCREEN_SEQUENCER_CLIP_8 8

#define TRELLIS_SCREEN_PIANO 10
#define TRELLIS_SCREEN_MIXER 12
#define TRELLIS_SCREEN_MIXER1 13
#define TRELLIS_SCREEN_PERFORM 14
#define TRELLIS_SCREEN_CLIPLAUNCHER 15

#define TRELLIS_SCREEN_ARRANGER_1 16
#define TRELLIS_SCREEN_ARRANGER_16 TRELLIS_SCREEN_ARRANGER_1 + 16
#define TRELLIS_SCREEN_PLAYMODE TRELLIS_SCREEN_ARRANGER_16 + 8
#define TRELLIS_SCREEN_PLUGIN TRELLIS_SCREEN_PLAYMODE + 1
#define TRELLIS_SCREEN_FX TRELLIS_SCREEN_PLUGIN + 1
#define TRELLIS_SCREEN_SQTL TRELLIS_SCREEN_FX + 1

#define TRELLIS_SCREEN_SELECT_CLIP2EDIT 95
#define TRELLIS_SCREEN_SELECT_MIXERPAGE 96
#define TRELLIS_SCREEN_SELECT_ARRANGERPAGE 97
#define TRELLIS_SCREEN_SAVE_LOAD 98
#define TRELLIS_SCREEN_STARTUP 99
#define TRELLIS_MAX_PAGES TRELLIS_SCREEN_SQTL + 1

extern File myFile;

extern const int FlashChipSelect; // digital pin for flash chip CS pin
#define NO_VALUE 6789
#define NO_NAME "NO_NAME"
#define NO_NOTE 128
#define NUM_PLUGINS 14
#define NUM_PLAYMODES 11
#define NUM_SCALES 23
#define NUM_CHORDS 34
#define NUM_CHORD_VOLUMES 81
#define NUM_FX 3
#define NUM_PARAMETERS 16

#define NUM_TRACKS 8
#define NUM_STEPS 16
#define NUM_MIDI_OUTPUTS 48
#define CH_PLUGIN_1 NUM_MIDI_OUTPUTS + 1
#define MAX_OUTPUTS NUM_MIDI_OUTPUTS + NUM_PLUGINS
#define NUM_PLUGIN_PRESETS 8
#define NUM_SAMPLE_BANKS 8
#define MIDI_CC_RANGE 127
#define MIDI_CC_RANGE_FLOAT 127.00
// encoder
#define NUM_ENCODERS 4
// encoder functions
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
#define INPUT_FUNCTIONS_FOR_CLIPLAUNCHER 15
#define INPUT_FUNCTIONS_FOR_SGTL 25
// clips
#define MAX_CLIPS 8
#define NUM_PRESETS 8
#define MAX_TICKS 96
#define TICKS_PER_STEP 6
#define MAX_VOICES 12
#define MAX_SONGS 9

// step sequencer parameters
#define SET_STEP_FX 2
#define SET_VELO2SET 3

#define SET_SEQUENCE_LENGTH 4
#define SET_CLOCK_DIVISION 5
#define SET_STEP_LENGTH 6
#define SET_OCTAVE 7

#define SET_SEQ_MODE 8
#define SET_SCALE 9
#define SET_MIDICH_OUT 10
#define SET_CLIP2_EDIT 11

#define SET_SWING 12
#define SET_HUMANIZE 13
#define SET_STEPFX_CHAN 14
#define PRESET_ENCODER 0

#define TFT_WIDTH 480
#define TFT_HEIGHT 320
#define STEP_FRAME_W 24
#define STEP_FRAME_H 21
#define TRACK_FRAME_H 30
#define PIXEL_PER_TICK 3
// for arranger
#define BARS_PER_PAGE 16
// for sequencer
#define SEQ_GRID_LEFT (2 * STEP_FRAME_W)
#define SEQ_GRID_RIGHT (18 * STEP_FRAME_W) - 2
#define SEQ_GRID_TOP 1
#define SEQ_GRID_BOTTOM 12
#define SEQUENCER_OPTIONS_VERY_RIGHT 18
#define SEQUENCER_OPTIONS_RIGHT 16

extern const char FLASHMEM *songNames[16];

extern char _trackname[20];
extern int pixelTouchX;
extern int gridTouchY;
extern int parameterTouchX;
extern int parameterTouchY[NUM_ENCODERS];
extern uint8_t lastPotRow;
extern bool change_row;
extern uint8_t activeScreen;
extern uint8_t active_track;
extern uint8_t arrangerpage;
extern bool change_plugin_row;
extern const int encoder_colour[NUM_ENCODERS];
extern unsigned long neotrellisReadPreviousMillis; // will store last time LED was updated
extern unsigned long updateMidiPreviousMillis;     // will store last time LED was updated
extern bool getArrangerFromPC;
extern bool i2c_busy;
extern uint16_t tftRAM[16][16];
#define BEAT_ARRAY_SIZE 128
// extern const bool seqM_Beats[BEAT_ARRAY_SIZE][NUM_STEPS];
extern bool **beatArray;
extern bool **beatArrayPM6;
extern bool *beatArrayPM7;
extern uint8_t sgtlparameter[16];
#define Y_DIM 12 // number of rows of key
#define X_DIM 4  // number of columns of keys
#define TRELLIS_PADS_X_DIM 16
#define TRELLIS_PADS_Y_DIM 8
#define TRELLIS_INT_PIN 41
#define NUMTRELLIS 8

// Color definitions
#define ILI9341_BLACK 0x0000       /*   0,   0,   0 */
#define ILI9341_NAVY 0x000F        /*   0,   0, 128 */
#define ILI9341_DARKGREEN 0x03E0   /*   0, 128,   0 */
#define ILI9341_DARKCYAN 0x03EF    /*   0, 128, 128 */
#define ILI9341_MAROON 0x7800      /* 128,   0,   0 */
#define ILI9341_PURPLE 0x780F      /* 128,   0, 128 */
#define ILI9341_OLIVE 0x7BE0       /* 128, 128,   0 */
#define ILI9341_LIGHTGREY 0xC618   /* 192, 192, 192 */
#define ILI9341_DARKGREY 0x7BEF    /* 128, 128, 128 */
#define ILI9341_BLUE 0x001F        /*   0,   0, 255 */
#define ILI9341_GREEN 0x07E0       /*   0, 255,   0 */
#define ILI9341_CYAN 0x07FF        /*   0, 255, 255 */
#define ILI9341_RED 0xF800         /* 255,   0,   0 */
#define ILI9341_MAGENTA 0xF81F     /* 255,   0, 255 */
#define ILI9341_YELLOW 0xFFE0      /* 255, 255,   0 */
#define ILI9341_WHITE 0xFFFF       /* 255, 255, 255 */
#define ILI9341_ORANGE 0xFD20      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0xAFE5 /* 173, 255,  47 */
#define ILI9341_PINK 0xF81F
// #define TRELLIS_LIGHTBLUE #04681dff
// #define TRELLIS_LIGHTBLUE #5fe05aff
// #define TRELLIS_LIGHTBLUE #b83b8eff
// #define TRELLIS_LIGHTBLUE #ADD8E6
// #define TRELLIS_LIGHTBLUE #ADD8E6

#define numKeys (NUMTRELLIS * 16)

extern bool trellisShowClockPixel[TRELLIS_PADS_Y_DIM];
extern int trellisTrackColor[9];
extern int trackColor[9];
extern const uint8_t eigthTo127[8];
extern uint8_t trellisPerformIndex[TRELLIS_PADS_X_DIM];
extern uint8_t performCC[TRELLIS_PADS_X_DIM];
extern bool neotrellisPressed[X_DIM * Y_DIM];
extern bool trellisHeld[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM];
extern uint8_t lastPressedKey;
extern bool trellisIsPressed;
extern uint8_t FLASHMEM gateOutputPin[8];
extern const bool PROGMEM scales[NUM_SCALES][12];
extern const char FLASHMEM *scaleNames[NUM_SCALES];
extern bool updateTFTScreen;
extern const char *bankNames[NUM_SAMPLE_BANKS];
extern const char FLASHMEM *CCnames[162];
extern const char FLASHMEM *seqModname[NUM_PLAYMODES];
extern const char FLASHMEM *channelOutNames[MAX_OUTPUTS + 1];
extern const char FLASHMEM *noteNames[12];
extern const uint8_t chords[NUM_CHORDS][4];
extern const uint8_t chordVolumes[NUM_CHORD_VOLUMES][4];

// screen
#define POSITION_BPM_BUTTON 14
#define POSITION_START_LOOP_BUTTON 16
#define POSITION_END_LOOP_BUTTON 18
#define TRELLIS_BUTTON_ENTER 8

extern int noteInfo[MAX_VOICES][4]; // noote, velo, start, length
// playmodes
struct MidiTrack
{
    File file;
    uint32_t trackStart;  // Position der Track-Daten im File
    uint32_t trackLength; // Länge des Track-Chunks
    int division;         // Ticks per Quarter Note (PPQN)
};
extern MidiTrack myMidi[NUM_TRACKS];
float lfo_semitone_tri(float phase);
float lfo_semitone_saw(float phase);
// Hilfsstruktur für die Anzeige-Labels und Wertebereiche
struct SeqModeParam
{
    const char *label;
    int max;
};

const SeqModeParam seqModeParams[12][4][4] = {
    // Step
    {
        {{"Oct -",  11}, {"Oct +", 11}, {"Vol -",  MIDI_CC_RANGE}, {"Vol +",  MIDI_CC_RANGE}},
        {{"maxSteps",  NUM_STEPS}, {"Dejavu",  MIDI_CC_RANGE}, {"Rotate",  32}, {"",  0}},
        {{"StepFX -",  MIDI_CC_RANGE + 1}, {"StepFX +",  MIDI_CC_RANGE + 1}, {"",  0}, {"",  0}},
        {{"",  0}, {"",  0}, {"",  0}, {"",  0}}},
    // Random
    {
        {{"Oct -",  11}, {"Oct +",  11}, {"Vol -",  MIDI_CC_RANGE}, {"Vol +",  MIDI_CC_RANGE}},
        {{"maxSteps",  NUM_STEPS}, {"Dejavu",  MIDI_CC_RANGE}, {"Rotate",  32}, {"",  0}},
        {{"StepFX -",  MIDI_CC_RANGE + 1}, {"StepFX +",  MIDI_CC_RANGE + 1}, {"",  0}, {"",  0}},
        {{"", 0}, {"", 0}, {"", 0}, {"", 0}}},
    // Dropseq
    {
        {{"Drop", NO_NOTE}, {"Rst @", NO_NOTE}, {"Oct -", NO_NOTE}, {"Oct +", NO_NOTE}},
        {{"C", NO_NOTE}, {"C#", NO_NOTE}, {"D", NO_NOTE}, {"D#", NO_NOTE}},
        {{"E", NO_NOTE}, {"F", NO_NOTE}, {"F#", NO_NOTE}, {"G", NO_NOTE}},
        {{"G#", NO_NOTE}, {"A", NO_NOTE}, {"A#", NO_NOTE}, {"B", NO_NOTE}}},
    // BitRd
    {
        {{"C", 255}, {"C#", 255}, {"D", 255}, {"D#", 255}},
        {{"E", 255}, {"F", 255}, {"F#", 255}, {"G", 255}},
        {{"G#", 255}, {"A", 255}, {"A#", 255}, {"B", 255}},
        {{"Vol -", MIDI_CC_RANGE}, {"Vol +", MIDI_CC_RANGE}, {"StepFX -", MIDI_CC_RANGE + 1}, {"StepFX +", MIDI_CC_RANGE + 1}}},
    // PotS
    {
        {{"ADSR", NO_NOTE}, {"2", NO_NOTE}, {"3", NO_NOTE}, {"4", NO_NOTE}},
        {{"5", NO_NOTE}, {"6", NO_NOTE}, {"7", NO_NOTE}, {"8", NO_NOTE}},
        {{"9", NO_NOTE}, {"10", NO_NOTE}, {"11", NO_NOTE}, {"12", NO_NOTE}},
        {{"13", NO_NOTE}, {"14", NO_NOTE}, {"15", NO_NOTE}, {"16", NO_NOTE}}},
    // Beats
    {
        {{"ADSR", MIDI_CC_RANGE}, {"2", MIDI_CC_RANGE}, {"3", MIDI_CC_RANGE}, {"4", MIDI_CC_RANGE}},
        {{"5", MIDI_CC_RANGE}, {"6", MIDI_CC_RANGE}, {"7", MIDI_CC_RANGE}, {"8", MIDI_CC_RANGE}},
        {{"9", MIDI_CC_RANGE}, {"10", MIDI_CC_RANGE}, {"11", MIDI_CC_RANGE}, {"12", MIDI_CC_RANGE}},
        {{"Vol -", MIDI_CC_RANGE}, {"Vol +", MIDI_CC_RANGE}, {"StepFX -", MIDI_CC_RANGE + 1}, {"StepFX +", MIDI_CC_RANGE + 1}}},
    // EuClid
    {
        {{"ADSR", MIDI_CC_RANGE + 1}, {"2", MIDI_CC_RANGE + 1}, {"3", MIDI_CC_RANGE + 1}, {"4", MIDI_CC_RANGE + 1}},
        {{"5", MIDI_CC_RANGE + 1}, {"6", MIDI_CC_RANGE + 1}, {"7", MIDI_CC_RANGE + 1}, {"8", MIDI_CC_RANGE + 1}},
        {{"9", MIDI_CC_RANGE + 1}, {"10", MIDI_CC_RANGE + 1}, {"11", MIDI_CC_RANGE + 1}, {"12", MIDI_CC_RANGE + 1}},
        {{"Vol -", MIDI_CC_RANGE + 1}, {"Vol +", MIDI_CC_RANGE + 1}, {"StepFX -", MIDI_CC_RANGE + 1}, {"StepFX +", MIDI_CC_RANGE + 1}}},
    // Rclid
    {
        {{"Steps", MIDI_CC_RANGE}, {"Offset", MIDI_CC_RANGE}, {"DejaVu", MIDI_CC_RANGE}, {"rotate", MIDI_CC_RANGE}},
        {{"Oct -", MIDI_CC_RANGE}, {"Oct +", MIDI_CC_RANGE}, {"Velo -", MIDI_CC_RANGE}, {"Velo +", MIDI_CC_RANGE}},
        {{"FX -", MIDI_CC_RANGE}, {"FX +", MIDI_CC_RANGE}, {"", 0}, {"", 0}},
        {{"", 0}, {"", 0}, {"", 0}, {"", 0}}},
    // Midi
    {
        {{"File", 127}, {"StepFX -", MIDI_CC_RANGE + 1}, {"StepFX +", MIDI_CC_RANGE + 1}, {"CMult",  MIDI_CC_RANGE}},
        {{"StartBar", 255}, {"loopBars",  255}, {"", 0}, {"", 0}},
        {{"", 0}, {"", 0}, {"", 0}, {"", 0}},
        {{"", 0}, {"", 0}, {"", 0}, {"", 0}}},
    // LFO
    {
        {{"Degree", MIDI_CC_RANGE}, {"Depth", MIDI_CC_RANGE}, {"Vol -", MIDI_CC_RANGE}, {"Vol +", MIDI_CC_RANGE}},
        {{"maxSteps", NUM_STEPS}, {"Dejavu", MIDI_CC_RANGE}, {"Rotate", 32}, {"", 0}},
        {{"StepFX -", MIDI_CC_RANGE + 1}, {"StepFX +", MIDI_CC_RANGE + 1}, {"Type", 8}, {"", 0}},
        {{"", 0}, {"", 0}, {"", 0}, {"", 0}}},
    // Psy
    {
        {{"Style", MIDI_CC_RANGE}, {"RootN", MIDI_CC_RANGE}, {"Vol -", MIDI_CC_RANGE}, {"Vol +", MIDI_CC_RANGE}},
        {{"AccNot1", MIDI_CC_RANGE}, {"AccPos1", NUM_STEPS}, {"AccNot2", MIDI_CC_RANGE}, {"AccPos2", NUM_STEPS}},
        {{"StepFX -", MIDI_CC_RANGE + 1}, {"StepFX +", MIDI_CC_RANGE + 1}, {"", 0}, {"", 0}},
        {{"", 0}, {"", 0}, {"", 0}, {"", 0}}}};
#endif