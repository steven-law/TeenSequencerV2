#ifndef TRACK_H
#define TRACK_H

#include <Arduino.h>

#include <MIDI.h>
#include <SD.h>
#include "projectVariables.h"

#include "output/tft.h"
#include "input/trellis.h"

//
// #include "Plugins/plugin_List.h"
// //#include "hardware/tftClass.h"

// void draw_sequencer_option(uint8_t x, const char *nameshort, int value, uint8_t enc, const char *pluginName);
// class tftClass;
#define NOTES_PER_OCTAVE 12
#define MIDI_CC_RANGE 127

#define NO_NOTE 128
#define MAX_BARS 256
// potrow 0
#define ENCODER_STEP_FX 2
// potrow 1
#define ENCODER_SEQUENCE_LENGTH 0
#define ENCODER_CLOCK_DIVISION 1
#define ENCODER_STEP_LENGTH 2
#define ENCODER_OCTAVE 3
// potrow 2
#define ENCODER_SEQ_MODE 0
#define ENCODER_SCALE 1
#define ENCODER_MIDICH_OUT 2
#define ENCODER_CLIP2_EDIT 3

#define ENCODER_PROBABILITY 0
#define ENCODER_HUMANIZE 1

#define NUM_PARAMETERS 16

extern File myFile;
// Encoder Pins
extern bool enc_moved[4];
extern int encoded[4];
// extern int encoder_colour[4];
extern bool change_plugin_row;
extern uint8_t activeScreen;
extern int pixelTouchX;
extern int gridTouchY;
extern uint8_t active_track;
void sendNoteOn(uint8_t _track, uint8_t Note, uint8_t Velo, uint8_t Channel);
void sendNoteOff(uint8_t _track, uint8_t Note, uint8_t Velo, uint8_t Channel);
void sendControlChange(uint8_t control, uint8_t value, uint8_t channel);
void trellis_set_main_buffer(int _page, int _x, int _y, int color);
void neotrellis_show();
// extern midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> MIDI1;
void save_plugin(uint8_t _songNr, uint8_t _pluginNr);
void load_plugin(uint8_t _songNr, uint8_t _pluginNr);
bool loadMidiFile(const char *filename, MidiTrack &track);
int getPPQN(const MidiTrack &track);
int fillNoteInfoAtTick(MidiTrack &track, int miditick);
class Track
{

public:
    File myTrackFile;
    uint8_t my_Arranger_Y_axis;
    uint8_t parameter[16]{0, 0, 128, 99, MAX_TICKS, 1, 3, 5, 0, 0, 0, 0, 0, 0, 129, 0};
    // Stepsequencer
    struct tick_t
    {
        uint8_t voice[MAX_VOICES];
        uint8_t velo[MAX_VOICES];
        uint8_t stepFX;
        uint8_t noteLength[MAX_VOICES];
        uint8_t startTick[MAX_VOICES];
    };
    struct clip_t
    {
        tick_t tick[MAX_TICKS];
        uint8_t seqLength = MAX_TICKS;
        uint8_t clockDivision = 1;
        uint8_t playMode = 0;
        uint8_t scale = 0;
        uint8_t midiChOut;
        // int8_t humanize = 0;
    };
    clip_t *clip = nullptr;

    uint8_t CCvalue[NUM_PRESETS + 1][16];
    uint8_t CCchannel[NUM_PRESETS + 1][16];
    uint8_t edit_presetNr_ccChannel = 0;
    uint8_t edit_presetNr_ccValue = 0;

    bool recordState = false;

    // mixer
    uint8_t mixGainPot = 127;
    float mixGain = 1;
    uint8_t mixDryPot = 127;
    float mixDry = 1;
    uint8_t mixFX1Pot = 127;
    float mixFX1 = 0;
    uint8_t mixFX2Pot = 127;
    float mixFX2 = 0;
    uint8_t mixFX3Pot = 127;
    float mixFX3 = 0;
    uint8_t mixFxPot[NUM_FX];
    float mixFX[NUM_FX];
    bool muted;
    bool soloed;
    bool muteThruSolo;
    bool performIsActive = false;
    int performNoteOffset = 0;
    int performClockDivision = 0;
    int bar_to_edit = 0;
    int tick_to_edit;
    int voice_to_edit;
    uint8_t noteToPlay[MAX_VOICES];

    // arranger
    int internal_clock = 0;
    int internal_clock_bar = -1;
    int external_clock_bar = 0;
    int clip_to_play[MAX_BARS];
    int noteOffset[MAX_BARS];
    int barVelocity[MAX_BARS];
    int barProbabilty[MAX_BARS] = {127};
    int play_presetNr_Playmode_ccChannel[MAX_BARS];
    int play_presetNr_Plugin_ccValue[MAX_BARS];
    int bar_for_copying;
    Track(uint8_t Y)
    {
        // MIDI1.setHandleNoteOn(myNoteOn);
        // MIDI1.setHandleNoteOff(myNoteOff);
        // MIDI1.begin();
        // SD.begin(BUILTIN_SDCARD);
        my_Arranger_Y_axis = Y;
        parameter[SET_MIDICH_OUT] = Y;
        // allocate tracks0-7 "array"
        clip = static_cast<clip_t *>(calloc(MAX_CLIPS, sizeof(clip_t)));
        // fill the tracks0-7 "array"

        for (int c = 0; c < MAX_CLIPS; c++)
        {
            clip[c].seqLength = MAX_TICKS;
            clip[c].clockDivision = 1;
            clip[c].playMode = 0;
            clip[c].scale = 0;
            clip[c].midiChOut = Y;
            if (my_Arranger_Y_axis == 1)
            {
                parameter[SET_MIDICH_OUT] = 13;
                clip[c].midiChOut = 13;
            }

            for (int t = 0; t < MAX_TICKS; t++)
            {
                for (int v = 0; v < MAX_VOICES; v++)
                {
                    clip[c].tick[t].voice[v] = NO_NOTE;
                    clip[c].tick[t].velo[v] = 0;
                    clip[c].tick[t].stepFX = 129;
                    clip[c].tick[t].noteLength[v] = 0;
                    clip[c].tick[t].startTick[v] = MAX_TICKS + 1;
                }
            }
        }
        if (clip == nullptr)
        {
            // Fehlerbehandlung, wenn calloc fehlschlägt
            Serial.println("Memory allocation failed");
        }

        for (int i = 0; i < MAX_BARS; i++)
        {
            clip_to_play[i] = 8;
            noteOffset[i] = 0;
            barVelocity[i] = 127;
            play_presetNr_Playmode_ccChannel[i] = 0;
            play_presetNr_Plugin_ccValue[i] = 0;
            barProbabilty[i] = 127;
        }
        for (int p = 0; p < NUM_PRESETS + 1; p++)
        {
            for (int t = 0; t < 16; t++)
            {
                CCchannel[p][t] = 129;
                CCvalue[p][t] = 0;
            }
        }
        if (my_Arranger_Y_axis == 1)
        {
            parameter[SET_OCTAVE] = 5;
            parameter[SET_MIDICH_OUT] = 13;
        }
    }

    // update
    void update(int PixelX, uint8_t gridY);
    void set_recordState(bool _status);
    bool get_recordState();
    void noteOn(uint8_t Note, uint8_t Velo, uint8_t Channel);
    void noteOff(uint8_t Note, uint8_t Velo, uint8_t Channel);
    void record_noteOn(uint8_t Note, uint8_t Velo, uint8_t Channel);
    void record_noteOff(uint8_t Note, uint8_t Velo, uint8_t Channel);
    void save_track(uint8_t songNr);
    void load_track(uint8_t songNr);
    // songmode
    void set_arranger_parameters();
    void set_clip_to_play_trellis(uint8_t _bar, uint8_t _clipNr);
    void set_bar_parameter(uint8_t _encoder, int b, int *parameterArray, int minValue, int maxValue, const char *label);
    void copy_bar();
    void clear_arrangment();
    // stepsequencer
    void set_note_on_tick(int _startTick, int _note, int length);
    uint8_t get_note_parameter(const uint8_t *parameterArray, uint8_t _voice);
    void set_stepSequencer_parameters();
    void clear_active_clip();
    // seqmodes
    void draw_sequencer_modes(uint8_t mode);
    void play_sequencer_mode(uint8_t cloock, uint8_t start, uint8_t end);
    void set_seq_mode_parameters(uint8_t row);
    // midi CC
    void set_MIDI_CC(uint8_t row);

private:
    int noteOffAt[MAX_VOICES] = {-1};
    bool internal_clock_is_on = false;

    int recordStartTick[MAX_VOICES];
    uint8_t recordLastNote[MAX_VOICES];
    uint8_t recordVelocity[MAX_VOICES];
    uint8_t recordVoice;

    bool note_is_on[MAX_VOICES] = {false, false, true, true, true, true, true, true, true, true, true, true};
    uint8_t seqMod_value[NUM_PLAYMODES][NUM_PRESETS][16];
    uint8_t seqMod1NoteMemory[NUM_STEPS];
    uint8_t seqMod2NoteMemory[NUM_STEPS];
    uint8_t seqMod7NoteMemory[NUM_STEPS];
    uint8_t SeqMod6Value2[16];
    uint8_t PMpresetNr = 0;
    uint8_t maxVal;
    // stepsequencer
    void set_stepSequencer_parameter_value(uint8_t XPos, uint8_t YPos, const char *name, uint8_t min, uint8_t max);
    void set_stepSequencer_parameter_text(uint8_t XPos, uint8_t YPos, const char *name, const char *text, uint8_t min, uint8_t max);
    void copy_clip();
    // midi CC
    void set_CCvalue(uint8_t XPos, uint8_t YPos);
    void set_CCchannel(uint8_t XPos, uint8_t YPos);
    void set_edit_preset_CC(uint8_t n, uint8_t &presetVar, const char *label, uint8_t position);
    void change_presets();

    // seqmodes
    void set_presetNr();
    void set_seq_mode_value(uint8_t modeindex, uint8_t XPos, uint8_t YPos, const char *name, int min, int max);
    uint8_t get_random_Note_in_scale();
    void rotateIntArray(uint8_t arr[], int maxSteps, int rotation);
    void play_seq_mode0(uint8_t cloock);

    void play_seq_mode1(uint8_t cloock);
    void set_seq_mode1_parameters();
    void draw_seq_mode1();

    void play_seq_mode2(uint8_t cloock);
    void set_seq_mode2_parameters();
    void draw_seq_mode2();

    void play_seq_mode3(uint8_t cloock);
    void set_seq_mode3_parameters();
    void draw_seq_mode3();

    void play_seq_mode4(uint8_t cloock);
    void set_seq_mode4_parameters();
    void draw_seq_mode4();

    void play_seq_mode5(uint8_t cloock);
    void set_seq_mode5_parameters();
    void draw_seq_mode5();

    void play_seq_mode6(uint8_t cloock);
    void set_seq_mode6_parameters();
    void set_seq_mode6_value(uint8_t XPos, uint8_t YPos, const char *name);
    void draw_seq_mode6();

    void play_seq_mode7(uint8_t cloock);
    void set_seq_mode7_parameters();
    void set_seq_mode7_value(uint8_t XPos, uint8_t YPos, const char *name);
    void draw_seq_mode7();

    void play_seq_mode8(uint8_t cloock);
    void set_seq_mode8_parameters();
    void draw_seq_mode8();
    void select_file(uint8_t XPos, uint8_t YPos, const char *name);
    void refresh_mode8();

    void play_seq_mode9(uint8_t cloock);
    void set_seq_mode9_parameters();
    void draw_seq_mode9();
    int degree = 0;
};

extern Track *allTracks[8];
#endif