#ifndef TRACK_H
#define TRACK_H

#include <Arduino.h>

#include <MIDI.h>
#include <SD.h>
#include "projectVariables.h"

#include "output/tft.h"
#include "input/trellis.h"

//
//#include "Plugins/plugin_List.h"
// //#include "hardware/tftClass.h"

// void draw_sequencer_option(byte x, const char *nameshort, int value, byte enc, const char *pluginName);
//class tftClass;
#define NOTES_PER_OCTAVE 12
#define MIDI_CC_RANGE 127

#define NO_NOTE 128

// potrow 0
#define ENCODER_STEP_FX 2
// potrow 1
#define ENCODER_SEQUENCE_LENGTH 0
#define ENCODER_STEP_DIVISION 1
#define ENCODER_STEP_LENGTH 2
// potrow 2
#define ENCODER_OCTAVE 3
#define ENCODER_SEQ_MODE 0
#define ENCODER_MIDICH_OUT 1
#define ENCODER_CLIP2_EDIT 3

#define NUM_PARAMETERS 16


extern File myFile;
// Encoder Pins
extern bool enc_moved[4];
extern int encoded[4];
//extern int encoder_colour[4];
extern bool change_plugin_row;
extern byte activeScreen;
extern int pixelTouchX;
extern int gridTouchY;
extern byte active_track;
void sendNoteOn(byte Note, byte Velo, byte Channel);
void sendNoteOff(byte Note, byte Velo, byte Channel);
void sendControlChange(byte control, byte value, byte channel);
void trellis_set_main_buffer(int _page, int _x, int _y, int color);
void neotrellis_show();
// extern midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> MIDI1;
class Track
{

public:
    byte my_Arranger_Y_axis;
    byte parameter[16]{0, 0, 128, 99, 96, 1, 3, 4, 0, 0, 0, 0};
    // Stepsequencer
    struct tick_t
    {
        byte voice[MAX_VOICES];
        byte velo[MAX_VOICES];
        byte stepFX;
    };
    struct clip_t
    {
        tick_t tick[MAX_TICKS];
    };
    clip_t *clip = nullptr;
    byte CCvalue[NUM_PRESETS + 1][16];
    byte CCchannel[NUM_PRESETS + 1][16];
    byte edit_presetNr_ccChannel = 0;
    byte edit_presetNr_ccValue = 0;

    bool recordState = false;

    // mixer
    byte mixGainPot = 127;
    float mixGain = 1;
    byte mixDryPot;
    float mixDry = 1;
    byte mixFX1Pot;
    float mixFX1 = 0;
    byte mixFX2Pot;
    float mixFX2 = 0;
    byte mixFX3Pot;
    float mixFX3 = 0;
    bool muted;
    bool soloed;
    bool muteThruSolo;
    bool performIsActive = false;
    int performNoteOffset = 0;
    bool cvNoteOn;
    bool cvNoteOff;
    byte cvNote;
    byte bar_to_edit = 0;

    // arranger
    int internal_clock = -1;
    int internal_clock_bar = 0;
    int external_clock_bar = 0;
    byte clip_to_play[256];
    int noteOffset[256];
    byte barVelocity[256];

    byte play_presetNr_ccChannel[256];
    byte play_presetNr_ccValue[256];
    Track( byte Y)
    {
        // MIDI1.setHandleNoteOn(myNoteOn);
        // MIDI1.setHandleNoteOff(myNoteOff);
        // MIDI1.begin();
        // SD.begin(BUILTIN_SDCARD);
        my_Arranger_Y_axis = Y;
        MIDI_channel_in = Y;
        parameter[SET_MIDICH_OUT] = Y;
        // allocate tracks0-7 "array"
        clip = static_cast<clip_t *>(calloc(MAX_CLIPS, sizeof(clip_t)));
        // fill the tracks0-7 "array"

        for (int c = 0; c < MAX_CLIPS; c++)
        {
            for (int t = 0; t < MAX_TICKS; t++)
            {
                for (int v = 0; v < MAX_VOICES; v++)
                {
                    clip[c].tick[t].voice[v] = NO_NOTE;
                    clip[c].tick[t].velo[v] = 0;
                    clip[c].tick[t].stepFX = 128;
                }
            }
        }
        if (clip == nullptr)
        {
            // Fehlerbehandlung, wenn calloc fehlschlägt
            Serial.println("Memory allocation failed");
        }

        for (int i = 0; i < 256; i++)
        {
            clip_to_play[i] = 8;
            noteOffset[i] = 0;
            barVelocity[i] = 127;
            play_presetNr_ccChannel[i] = 8;
            play_presetNr_ccValue[i] = 8;
        }
        for (int p = 0; p < NUM_PRESETS + 1; p++)
        {
            for (int t = 0; t < 16; t++)
            {
                CCchannel[p][t] = 128;
                CCvalue[p][t] = 0;
            }
        }
    }

    void set_stepSequencer_parameters(byte row);

    void set_note_on_tick(int x, int y);
    void draw_sequencer_modes(byte mode);

    void set_recordState(bool _status);
    bool get_recordState();
    void record_noteOn(byte Note, byte Velo, byte Channel);
    void record_noteOff(byte Note, byte Velo, byte Channel);

    void set_MIDI_CC(byte row);
    // update
    void update(int PixelX, byte gridY);
    void noteOn(byte Note, byte Velo, byte Channel);
    void noteOff(byte Note, byte Velo, byte Channel);
    void save_track(byte songNr);
    void load_track(byte songNr);
    // songmode
    void set_arranger_parameters(byte lastProw);
    void set_clip_to_play(byte n, byte b);
    void set_clip_to_play_trellis(byte _bar, byte _clipNr);
    void set_note_offset(byte _encoder, int b);
    void set_barVelocity(byte _encoder, int b);
    void set_play_presetNr_ccChannel(byte n, byte lastProw);
    void set_play_presetNr_ccValue(byte n, byte lastProw);
    //
    void play_sequencer_mode(byte cloock, byte start, byte end);
    void set_seq_mode_parameters(byte row);

    byte get_active_note(byte _clip, byte _tick, byte _voice);

private:
    void play_seq_mode0(byte cloock);
    void play_seq_mode1(byte cloock);
    void set_seq_mode1_parameters(byte row);
    void set_seq_mode1_value(byte XPos, byte YPos, const char *name);
    void draw_seq_mode1();

    void play_seq_mode2(byte cloock);
    void set_seq_mode2_parameters(byte row);
    void set_seq_mode2_value(byte XPos, byte YPos, const char *name, int min, int max);
    void draw_seq_mode2();

    void play_seq_mode3(byte cloock);
    void set_seq_mode3_parameters(byte row);
    void set_seq_mode3_value(byte XPos, byte YPos, const char *name, int min, int max);
    void draw_seq_mode3();

    void play_seq_mode4(byte cloock);
    void set_seq_mode4_parameters(byte row);
    void set_seq_mode4_value(byte XPos, byte YPos, const char *name);
    void draw_seq_mode4();


    byte MIDI_channel_in;

    byte note2set;
    byte setStepFX = 74;
    byte tickStart;
    byte noteToPlay[MAX_VOICES];

    byte sTick;

    bool internal_clock_is_on = false;

    int recordStartTick[MAX_VOICES];
    byte recordLastNote[MAX_VOICES];
    byte recordVelocity[MAX_VOICES];
    byte recordVoice;
    byte sequence_length = MAX_TICKS;

    byte search_free_voice = 0;
    byte oldNotesInArray[MAX_VOICES]{NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE};
    bool note_is_on[MAX_VOICES] = {true, true, true, true, true, true, true, true, true, true, true, true};
    bool ready_for_NoteOff[MAX_VOICES] = {false, false, false, false, false, false, false, false, false, false, false, false};

    byte SeqMod1Value[16]; // oct- ; oct+ ; vol- ; vol+
    byte SeqMod2Value[16];
    byte SeqMod3Value[16];
    byte SeqMod4Value[16];
    byte maxVal;

    // sequencer Modes

    // void play_seq_mode0(byte cloock);

    void set_stepSequencer_parameter_value(byte XPos, byte YPos, const char *name, byte min, byte max);
    void set_stepSequencer_parameter_text(byte XPos, byte YPos, const char *name, const char *text, byte min, byte max);
    // sequencer options:
    void set_CCvalue(byte XPos, byte YPos);
    void set_CCchannel(byte XPos, byte YPos);
    void set_edit_presetNr_ccChannel(byte n, byte lastProw);
    void set_edit_presetNr_ccValue(byte n, byte lastProw);

    // sequencer note input stuff
    void set_active_note(byte _clip, byte _tick, byte _voice, byte _note);
    void set_active_velo(byte _clip, byte _tick, byte _voice, byte _velo);
    byte get_active_velo(byte _clip, byte _tick, byte _voice);
    void set_active_stepFX(byte _clip, byte _tick, byte _voice, byte _stepFX);
    byte get_active_stepFX(byte _clip, byte _tick, byte _voice);
    void check_for_free_voices(byte onTick, byte cnote);

    // stepsequencer

    //----------------------------------------------------------------
    // arranger stuff
    void change_presets();

    // clip to play

    // note offset / note transpose

    // bar Velocity
};

extern Track *allTracks[8];
#endif