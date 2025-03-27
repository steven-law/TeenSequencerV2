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
#define MAX_NOTES_PER_CLIP MAX_TICKS // Maximale Anzahl an Noten pro Clip

struct Note
{
    uint8_t pitch;     // MIDI-Note (0-127)
    uint8_t velocity;  // Anschlagstärke (0-127)
    uint8_t stepFX;    // stepFX fe. Filter CC74
    uint8_t startTick; // Position innerhalb des Clips (0-95)
    uint8_t length;    // Dauer der Note in Ticks
};

struct Clip
{
    Note notes[MAX_NOTES_PER_CLIP]; // Lineares Array für Noten
    uint16_t noteCount = 0;         // Aktuelle Anzahl an Noten

    // Funktion zum Hinzufügen einer Note
    bool set_note_on_tick(uint8_t pitch, uint8_t velocity, uint8_t stepFx, uint8_t startTick, uint8_t length)
    {
        if (noteCount >= MAX_NOTES_PER_CLIP)
            return false; // Verhindert Überlauf
        notes[noteCount++] = {pitch, velocity, stepFx, startTick, length};
        Serial.printf("create note: %d, velo: %d, stepFx: %d, length: %d\n ", pitch, velocity, stepFx, startTick, length);
        return true;
    }

    // Funktion zum Entfernen einer Note
    bool delete_note_on_tick(uint8_t pitch, uint8_t startTick)
    {
        for (uint16_t i = 0; i < noteCount; i++)
        {
            if (notes[i].pitch == pitch && notes[i].startTick == startTick)
            {
                notes[i] = notes[--noteCount]; // Überschreibt die Note mit der letzten Note
                return true;
            }
        }
        return false;
    }
    void delete_active_clip()
    {
        for (uint16_t i = 0; i < noteCount; i++)
        {

            noteCount = 0;
        }
    }
    void setNoteParameter(int cursorXPos, int cursorYPos, int value, int index)
    { // index: 0 note, 1 velocity, 2 stepFX
        for (uint16_t i = 0; i < noteCount; i++)
        {
            if (notes[i].startTick == cursorXPos && notes[i].pitch == cursorYPos)
            {
                switch (index)
                {
                case 0:
                    notes[i].pitch = value; // Ändere die Velocity
                case 1:
                    notes[i].velocity = value; // Ändere die Velocity
                case 2:
                    notes[i].length = value; // Ändere die Velocity
                    return;                  // Note gefunden, Funktion beenden
                }
            }
        }
    }
    int getNoteParameter(int cursorXPos, int cursorYPos, int index)
    {
        for (uint16_t i = 0; i < noteCount; i++)
        {
            if (notes[i].startTick == cursorXPos && notes[i].pitch == cursorYPos)
            {
                switch (index)
                {
                case 0:
                    return notes[i].pitch;
                case 1:
                    return notes[i].velocity;
                case 2:
                    return notes[i].stepFX;
                case 3:
                    return notes[i].length;
                default:
                    return -1; // Ungültiger Index
                }
            }
        }
        return -1; // Keine passende Note gefunden
    }
};
class Track
{

public:
    File myTrackFile;
    uint8_t my_Arranger_Y_axis;
    uint8_t parameter[16]{0, 0, 128, 99, MAX_TICKS, 1, 3, 4, 0, 0, 0, 0};
    // Stepsequencer
    // struct tick_t
    // {
    //     uint8_t voice[MAX_VOICES];
    //     uint8_t velo[MAX_VOICES];
    //     uint8_t stepFX[MAX_VOICES];
    // };
    // struct clip_t
    // {
    //     tick_t tick[MAX_TICKS];
    // };
    // clip_t *clip = nullptr;
    Clip clips[MAX_CLIPS]; // Ein Track enthält 8 Clips
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

    // arranger
    int internal_clock = -1;
    int internal_clock_bar = 0;
    int external_clock_bar = 0;
    int clip_to_play[MAX_BARS];
    int noteOffset[MAX_BARS];
    int barVelocity[MAX_BARS];
    int bar_for_copying;

    int play_presetNr_ccChannel[MAX_BARS];
    int play_presetNr_ccValue[MAX_BARS];
    Track(uint8_t Y)
    {
        // MIDI1.setHandleNoteOn(myNoteOn);
        // MIDI1.setHandleNoteOff(myNoteOff);
        // MIDI1.begin();
        // SD.begin(BUILTIN_SDCARD);
        my_Arranger_Y_axis = Y;
        parameter[SET_MIDICH_OUT] = Y;
        // allocate tracks0-7 "array"
        // clip = static_cast<clip_t *>(calloc(MAX_CLIPS, sizeof(clip_t)));
        // fill the tracks0-7 "array"

        // for (int c = 0; c < MAX_CLIPS; c++)
        // {
        //     for (int t = 0; t < MAX_TICKS; t++)
        //     {
        //         for (int v = 0; v < MAX_VOICES; v++)
        //         {
        //             clip[c].tick[t].voice[v] = NO_NOTE;
        //             clip[c].tick[t].velo[v] = 0;
        //             clip[c].tick[t].stepFX[v] = 128;
        //         }
        //     }
        // }
        // if (clip == nullptr)
        // {
        //     // Fehlerbehandlung, wenn calloc fehlschlägt
        //     Serial.println("Memory allocation failed");
        // }

        for (int i = 0; i < MAX_BARS; i++)
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
    void set_arranger_parameters(uint8_t lastProw);
    void set_clip_to_play_trellis(uint8_t _bar, uint8_t _clipNr);
    void set_bar_parameter(uint8_t _encoder, int b, int *parameterArray, int minValue, int maxValue, const char *label);
    void copy_bar();
    void clear_arrangment();
    // stepsequencer
    void set_note_on_tick(int startTick, int pitch, int length);
    uint8_t get_note_parameter(uint8_t *parameterArray, uint8_t _voice);
    void set_stepSequencer_parameters(uint8_t row);
    void clear_active_clip();
    // seqmodes
    void draw_sequencer_modes(uint8_t mode);
    void play_sequencer_mode(uint8_t cloock, uint8_t start, uint8_t end);
    void set_seq_mode_parameters(uint8_t row);
    // midi CC
    void set_MIDI_CC(uint8_t row);
    int getNoteParameterFromClip(int clipIndex, int cursorXPos, int cursorYPos, int index); // index: 0 pitch, 1 velocity, 2 stepFX, 3 noteLength
    void setNoteParameterFromClip(int cursorXPos, int cursorYPos, int value, int index);    // index: 0 pitch, 1 velocity, 2 stepFX, 3 noteLength
private:
    // stepsequencer
    void set_stepSequencer_parameter_value(uint8_t XPos, uint8_t YPos, const char *name, uint8_t min, uint8_t max);
    void set_stepSequencer_parameter_text(uint8_t XPos, uint8_t YPos, const char *name, const char *text, uint8_t min, uint8_t max);

    // midi CC
    void set_CCvalue(uint8_t XPos, uint8_t YPos);
    void set_CCchannel(uint8_t XPos, uint8_t YPos);
    void set_edit_preset_CC(uint8_t n, uint8_t &presetVar, const char *label, uint8_t position);
    void change_presets();

    void play_seq_mode0(uint8_t cloock);
    void play_seq_mode1(uint8_t cloock);
    void set_seq_mode1_parameters(uint8_t row);
    void set_seq_mode1_value(uint8_t XPos, uint8_t YPos, const char *name);
    void draw_seq_mode1();

    void play_seq_mode2(uint8_t cloock);
    void set_seq_mode2_parameters(uint8_t row);
    void set_seq_mode2_value(uint8_t XPos, uint8_t YPos, const char *name, int min, int max);
    void draw_seq_mode2();

    void play_seq_mode3(uint8_t cloock);
    void set_seq_mode3_parameters(uint8_t row);
    void set_seq_mode3_value(uint8_t XPos, uint8_t YPos, const char *name, int min, int max);
    void draw_seq_mode3();

    void play_seq_mode4(uint8_t cloock);
    void set_seq_mode4_parameters(uint8_t row);
    void set_seq_mode4_value(uint8_t XPos, uint8_t YPos, const char *name);
    void draw_seq_mode4();

    void play_seq_mode5(uint8_t cloock);
    void set_seq_mode5_parameters(uint8_t row);
    void set_seq_mode5_value(uint8_t XPos, uint8_t YPos, const char *name);
    void draw_seq_mode5();

    void play_seq_mode6(uint8_t cloock);
    void set_seq_mode6_parameters(uint8_t row);
    void set_seq_mode6_value(uint8_t XPos, uint8_t YPos, const char *name);
    void draw_seq_mode6();

    void play_seq_mode7(uint8_t cloock);
    void set_seq_mode7_parameters(uint8_t row);
    void set_seq_mode7_value(uint8_t XPos, uint8_t YPos, const char *name);
    void draw_seq_mode7();

    uint8_t setStepFX = 74;
    uint8_t noteToPlay[MAX_TICKS];

    bool internal_clock_is_on = false;

    int recordStartTick[MAX_TICKS];
     uint8_t recordLastNote[MAX_TICKS];
    // uint8_t recordVelocity[MAX_VOICES];
    uint8_t recordVoice;

    uint8_t oldNotesInArray[MAX_VOICES]{NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE};
    bool note_is_on[MAX_TICKS] = {true};
    bool ready_for_NoteOff[MAX_VOICES] = {false, false, false, false, false, false, false, false, false, false, false, false};

    uint8_t SeqMod1Value[16]; // oct- ; oct+ ; vol- ; vol+
    uint8_t SeqMod2Value[16];
    uint8_t SeqMod3Value[16];
    uint8_t SeqMod4Value[16];
    uint8_t SeqMod5Value[16];
    uint8_t SeqMod6Value[16];
    uint8_t SeqMod6Value2[16];
    uint8_t SeqMod7Value[16];
    uint8_t maxVal;
};

extern Track *allTracks[8];
#endif