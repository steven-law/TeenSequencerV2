#include <Arduino.h>
#include "projectVariables.h"

#include "Track.h"

void Track::set_stepSequencer_parameters()
{

    switch (lastPotRow)
    {
    case 0:

        encoder_SetCursor(parameter[SET_STEP_LENGTH] * 3, 14); // Encoder: 0+1
        set_stepSequencer_parameter_value(0, 0, "Tick", 0, 160);
        set_stepSequencer_parameter_value(1, 0, "Note", 0, 14);
        if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
            set_stepSequencer_parameter_value(ENCODER_STEP_FX, 3, CCnames[parameter[14]], 0, 128); // yPos=3 to get parameter 14
        else if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
            set_stepSequencer_parameter_value(ENCODER_STEP_FX, 0, CCnames[parameter[14]], 0, 128); // yPos=0 to get parameter 2
        set_stepSequencer_parameter_value(3, 0, "Velo", 0, 127);
        break;
    case 1:
        set_stepSequencer_parameter_value(ENCODER_SEQUENCE_LENGTH, 1, "seqL", 1, MAX_TICKS);
        set_stepSequencer_parameter_value(ENCODER_CLOCK_DIVISION, 1, "cDiv", 1, MAX_TICKS);
        set_stepSequencer_parameter_value(ENCODER_STEP_LENGTH, 1, "stpL", 1, MAX_TICKS);
        set_stepSequencer_parameter_value(ENCODER_OCTAVE, 1, "Oct", 0, 11);

        break;
    case 2:

        set_stepSequencer_parameter_value(ENCODER_SEQ_MODE, 2, "sMod", 0, NUM_PLAYMODES - 1);
        set_stepSequencer_parameter_value(ENCODER_SCALE, 2, "scal", 0, NUM_SCALES - 1);

        set_stepSequencer_parameter_value(ENCODER_MIDICH_OUT, 2, "MCh", 0, MAX_OUTPUTS);
        set_stepSequencer_parameter_value(ENCODER_CLIP2_EDIT, 2, "Clip", 0, NUM_USER_CLIPS);

        break;
    case 3:
        set_stepSequencer_parameter_value(ENCODER_PROBABILITY, 3, "Offset", 0, MAX_TICKS);

    default:
        break;
    }
    draw_stepSequencer_parameters();
}

void Track::set_stepSequencer_parameter_value(uint8_t XPos, uint8_t YPos, const char *name, uint8_t min, uint8_t max)
{
    if (!(gridTouchY > 0 && gridTouchY < 13 && pixelTouchX > SEQ_GRID_LEFT))
        return;
    if (enc_moved[XPos])
    {
        uint8_t index = XPos + (YPos * NUM_ENCODERS);
        // enc_moved[XPos] = false;
        parameter[index] = constrain(parameter[index] + encoded[XPos], min, max);
        Serial.printf("parameter: %d, value: %d, name %s\n", index, parameter[index], name);
        switch (index)
        {
        case SET_OCTAVE:
        case SET_CLIP2_EDIT:
        {
            draw_Notenames();
            trellis_show_piano();
            draw_notes_in_grid();
        }
        break;
        case SET_SEQUENCE_LENGTH:
        {
            clip[parameter[SET_CLIP2_EDIT]].seqLength = parameter[index];
        }
        break;
        case SET_CLOCK_DIVISION:
        {
            clip[parameter[SET_CLIP2_EDIT]].clockDivision = parameter[index];
        }
        break;
        case SET_SCALE:
        {
            clip[parameter[SET_CLIP2_EDIT]].scale = parameter[index];
            draw_Notenames();
            trellis_show_piano();
        }
        break;
        case SET_SEQ_MODE:
        {
            clip[parameter[SET_CLIP2_EDIT]].playMode = parameter[index];
        }
        break;

        case SET_MIDICH_OUT:
        {
            clip[parameter[SET_CLIP2_EDIT]].midiChOut = parameter[index];
        }
        case SET_VELO2SET:
        case SET_STEP_FX:
        {
            // Notenlänge bestimmen
            // int start_tick = clip[parameter[SET_CLIP2_EDIT]].tick[tick_to_edit].startTick[voice_to_edit];
            int start_tick;
            for (int i = tick_to_edit; i >= 0; i--)
            {
                if (clip[parameter[SET_CLIP2_EDIT]].tick[i].voice[voice_to_edit] == NO_NOTE)
                {
                    start_tick = i + 1;

                    break;
                }
            }
            int note_length = clip[parameter[SET_CLIP2_EDIT]].tick[start_tick].noteLength[voice_to_edit];
            Serial.printf("starttick:%d, length: %d\n", start_tick, note_length);
            // Änderungen anwenden
            for (int i = start_tick; i < (start_tick + note_length); i++)
            {
                // int tick = tick_to_edit + i;
                if (index == SET_VELO2SET)
                    clip[parameter[SET_CLIP2_EDIT]].tick[i].velo[voice_to_edit] = parameter[SET_VELO2SET];
                else
                    clip[parameter[SET_CLIP2_EDIT]].tick[i].stepFX = parameter[SET_STEP_FX];
            }
            erase_note_on_tick(voice_to_edit, start_tick, note_length);
            draw_note_on_tick(voice_to_edit, start_tick);
        }
        break;
        default:
            // Optional: Fehlerbehandlung oder Logging, falls `index` eine unerwartete Zahl ist
            break;
        }
        // draw_sequencer_arranger_parameter(my_Arranger_Y_axis - 1, XPos, name, parameter[index], "NO_NAME");
    }
}

void Track::set_stepSequencer_parameter_text(uint8_t XPos, uint8_t YPos, const char *name, const char *text, uint8_t min, uint8_t max)
{
    if (enc_moved[XPos])
    {
        uint8_t index = XPos + (YPos * NUM_ENCODERS);
        // enc_moved[XPos] = false;
        parameter[index] = constrain(parameter[index] + encoded[XPos], min, max);
        Serial.printf("parameter: %d, value: %d, name %s, text %s\n", index, parameter[index], name, text);
        switch (index)
        {

        case SET_SEQ_MODE:
        {
            clip[parameter[SET_CLIP2_EDIT]].playMode = parameter[index];
        }
        break;

        case SET_MIDICH_OUT:
        {
            clip[parameter[SET_CLIP2_EDIT]].midiChOut = parameter[index];
        }

        break;
        default:
            // Optional: Fehlerbehandlung oder Logging, falls `index` eine unerwartete Zahl ist
            break;
        }
        // draw_sequencer_arranger_parameter(my_Arranger_Y_axis - 1, XPos, name, NO_VALUE, text);
    }
}

// MIDI CC
void Track::set_CCchannel(uint8_t XPos, uint8_t YPos)
{
    int n = XPos + (YPos * NUM_ENCODERS);
    if (enc_moved[XPos])
    {
        CCchannel[edit_presetNr_ccChannel][n] = constrain(CCchannel[edit_presetNr_ccChannel][n] + encoded[XPos], 1, 128);
        draw_MIDI_CC(XPos, YPos);
        // enc_moved[XPos] = false;
    }
}
void Track::set_CCvalue(uint8_t XPos, uint8_t YPos)
{
    int n = XPos + (YPos * NUM_ENCODERS);
    if (enc_moved[XPos])
    {
        CCvalue[edit_presetNr_ccValue][n] = constrain(CCvalue[edit_presetNr_ccValue][n] + encoded[XPos], 0, 127);
        draw_MIDI_CC(XPos, YPos);
        sendControlChange(CCchannel[edit_presetNr_ccChannel][n], CCvalue[edit_presetNr_ccValue][n], clip[parameter[SET_CLIP2_EDIT]].midiChOut);
        // enc_moved[XPos] = false;
    }
}
void Track::set_MIDI_CC(uint8_t row)
{

    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT] && !neotrellisPressed[TRELLIS_BUTTON_ENTER])
    {
        if (row == 0)
        {
            set_CCvalue(0, 0);
            set_CCvalue(1, 0);
            set_CCvalue(2, 0);
            set_CCvalue(3, 0);
        }
        if (row == 1)
        {
            set_CCvalue(0, 1);
            set_CCvalue(1, 1);
            set_CCvalue(2, 1);
            set_CCvalue(3, 1);
        }
        if (row == 2)
        {
            set_CCvalue(0, 2);
            set_CCvalue(1, 2);
            set_CCvalue(2, 2);
            set_CCvalue(3, 2);
        }
        if (row == 3)
        {
            set_CCvalue(0, 3);
            set_CCvalue(1, 3);
            set_CCvalue(2, 3);
            set_CCvalue(3, 3);
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        if (row == 0)
        {
            set_CCchannel(0, 0);
            set_CCchannel(1, 0);
            set_CCchannel(2, 0);
            set_CCchannel(3, 0);
        }
        if (row == 1)
        {
            set_CCchannel(0, 1);
            set_CCchannel(1, 1);
            set_CCchannel(2, 1);
            set_CCchannel(3, 1);
        }
        if (row == 2)
        {
            set_CCchannel(0, 2);
            set_CCchannel(1, 2);
            set_CCchannel(2, 2);
            set_CCchannel(3, 2);
        }
        if (row == 3)
        {
            set_CCchannel(0, 3);
            set_CCchannel(1, 3);
            set_CCchannel(2, 3);
            set_CCchannel(3, 3);
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_ENTER])
    {
        set_edit_preset_CC(2, edit_presetNr_ccChannel, "cc-Set", 2);
        set_edit_preset_CC(3, edit_presetNr_ccValue, "vl-Set", 3);
        // set_edit_presetNr_ccChannel(2, 0);
        // set_edit_presetNr_ccValue(3, 0);
    }
    draw_MIDI_CC_screen();
}

void Track::set_edit_preset_CC(uint8_t n, uint8_t &presetVar, const char *label, uint8_t position)
{
    if (enc_moved[n])
    {
        presetVar = constrain(presetVar + encoded[n], 0, NUM_PRESETS - 1);
        change_plugin_row = true;
        draw_MIDI_CC_screen();
        draw_edit_presetNr_CC(label, presetVar, position);
    }
}

// helpers
// sequencer note input stuff
uint8_t Track::get_note_parameter(const uint8_t *parameterArray, uint8_t _voice)
{
    return parameterArray[_voice];
}

void Track::set_note_on_tick(int _startTick, int _note, int length)
{
    if (_startTick < 0 || _startTick >= 96 || clip == nullptr)
        return; // Clip darf nicht null sein!

    clip_t *clipPtr = clip;                                    // Richtiger Typ
    tick_t *tickPtr = clipPtr[parameter[SET_CLIP2_EDIT]].tick; // Zugriff auf das Tick-Array

    uint8_t note2set = _note;
    int _voice = _note % 12;
    // uint8_t note2set = voice + (parameter[SET_OCTAVE] * NOTES_PER_OCTAVE);
    uint8_t noteInClip = tickPtr[_startTick].voice[_voice];
    uint8_t velocity = parameter[SET_VELO2SET];
    uint8_t stepFX = parameter[SET_STEP_FX];

    bool isNoteClearing = (noteInClip == note2set);
    bool isNewNote = (noteInClip == NO_NOTE);

    if (isNewNote)
    {
        tickPtr[_startTick].noteLength[_voice] = length;
        tickPtr[_startTick].startTick[_voice] = _startTick;
    }

    for (int i = 0; i < length; i++)
    {
        uint8_t onTick = _startTick + i;

        if (isNoteClearing)
        {
            tickPtr[onTick].voice[_voice] = NO_NOTE;
            tickPtr[onTick].velo[_voice] = 0;
            tickPtr[_startTick].noteLength[_voice] = 0;
            tickPtr[_startTick].startTick[_voice] = MAX_TICKS;
        }
        else if (isNewNote)
        {
            tickPtr[onTick].voice[_voice] = note2set;
            tickPtr[onTick].velo[_voice] = velocity;
                }

        tickPtr[onTick].stepFX = stepFX;

        // Farbe für Trellis bestimmen (nur wenn nötig)
        int trellisColor = TRELLIS_BLACK;
        for (int v = 0; v < MAX_VOICES; v++)
        {
            if (get_note_parameter(tickPtr[onTick].voice, v) < NO_NOTE)
            {
                trellisColor = trellisTrackColor[my_Arranger_Y_axis - 1];
                break;
            }
        }
        trellis_set_main_buffer(parameter[SET_CLIP2_EDIT], onTick / TICKS_PER_STEP, my_Arranger_Y_axis - 1, trellisColor);
    }

    if (active_track == my_Arranger_Y_axis - 1 && activeScreen == INPUT_FUNCTIONS_FOR_SEQUENCER)
    {
        if (tickPtr[_startTick].voice[_voice] == NO_NOTE)
        {
            erase_note_on_tick(_voice, _startTick, length);
        }
        else
        {
            draw_note_on_tick(_voice, _startTick);
        }
    }
}
void Track::clear_active_clip()
{
    for (int i = 0; i < MAX_TICKS; i++)
    {
        for (int v = 0; v < MAX_VOICES; v++)
        {
            this->clip[parameter[SET_CLIP2_EDIT]].tick[i].voice[v] = NO_NOTE;
            this->clip[parameter[SET_CLIP2_EDIT]].tick[i].velo[v] = 0;
            this->clip[parameter[SET_CLIP2_EDIT]].tick[i].noteLength[v] = 0;
            this->clip[parameter[SET_CLIP2_EDIT]].tick[i].startTick[v] = MAX_TICKS;
            // draw_note_on_tick(n, i);
        }
        trellis_set_main_buffer(parameter[SET_CLIP2_EDIT], (i / TICKS_PER_STEP), (my_Arranger_Y_axis - 1), TRELLIS_BLACK);
    }
    draw_notes_in_grid();
}
// stepsequencer

void Track::set_recordState(bool _status)
{

    this->recordState = _status;
}
bool Track::get_recordState()
{
    return recordState;
}
