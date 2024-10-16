#include <Arduino.h>
#include "projectVariables.h"


#include "Track.h"

void Track::set_stepSequencer_parameters(uint8_t row)
{

    switch (row)
    {
    case 0:
        encoder_SetCursor(parameter[SET_STEP_LENGTH] * 2, 14); // Encoder: 0+1
        set_stepSequencer_parameter_value(0, 0, "Tick", 0, 160);
        set_stepSequencer_parameter_value(1, 0, "Note", 0, 14);


        set_stepSequencer_parameter_value(ENCODER_STEP_FX, 0, CCnames[setStepFX], 0, 128);
        set_stepSequencer_parameter_value(3, 0, "Velo", 1, 128);
        break;
    case 1:
        set_stepSequencer_parameter_value(ENCODER_SEQUENCE_LENGTH, 1, "seqL", 1, MAX_TICKS);
        set_stepSequencer_parameter_value(ENCODER_CLOCK_DIVISION, 1, "cDiv", 1, 16);
        set_stepSequencer_parameter_value(ENCODER_STEP_LENGTH, 1, "stpL", 1, MAX_TICKS);
        set_stepSequencer_parameter_value(ENCODER_OCTAVE, 1, "Oct", 0, 11);

        break;
    case 2:
        set_stepSequencer_parameter_text(ENCODER_SEQ_MODE, 2, "sMod", seqModname[parameter[SET_SEQ_MODE]], 0, NUM_PLAYMODES-1);
        set_stepSequencer_parameter_text(ENCODER_MIDICH_OUT, 2, "MCh", channelOutNames[parameter[SET_MIDICH_OUT]], 0, MAX_OUTPUTS);
        set_stepSequencer_parameter_value(ENCODER_CLIP2_EDIT, 2, "Clip", 0, NUM_USER_CLIPS);

        break;
    case 3:
    default:
        break;
    }
    draw_stepSequencer_parameters(row);
}

void Track::set_stepSequencer_parameter_value(uint8_t XPos, uint8_t YPos, const char *name, uint8_t min, uint8_t max)
{

    if (enc_moved[XPos])
    {
        uint8_t index = XPos + (YPos * NUM_ENCODERS);
        //enc_moved[XPos] = false;
        parameter[index] = constrain(parameter[index] + encoded[XPos], min, max);
        Serial.printf("parameter: %d, value: %d, name %s\n", index, parameter[index], name);
        if (index == SET_OCTAVE || index == SET_CLIP2_EDIT)
        {
            draw_notes_in_grid();
        }
        //draw_sequencer_arranger_parameter(my_Arranger_Y_axis - 1, XPos, name, parameter[index], "NO_NAME");
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
        //draw_sequencer_arranger_parameter(my_Arranger_Y_axis - 1, XPos, name, NO_VALUE, text);
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
        sendControlChange(CCchannel[edit_presetNr_ccChannel][n], CCvalue[edit_presetNr_ccValue][n], parameter[SET_MIDICH_OUT]);
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
        set_edit_presetNr_ccChannel(2, 0);
        set_edit_presetNr_ccValue(3, 0);
    }
    draw_MIDI_CC_screen();
}
void Track::set_edit_presetNr_ccChannel(uint8_t n, uint8_t lastProw)
{
    if (enc_moved[n])
    {
        edit_presetNr_ccChannel = constrain(edit_presetNr_ccChannel + encoded[n], 0, NUM_PRESETS - 1);

        change_plugin_row = true;
        draw_MIDI_CC_screen();
        draw_edit_presetNr_ccChannel(n, lastProw);
       // enc_moved[n] = false;
    }
}
void Track::set_edit_presetNr_ccValue(uint8_t n, uint8_t lastProw)
{
    if (enc_moved[n])
    {
        edit_presetNr_ccValue = constrain(edit_presetNr_ccValue + encoded[n], 0, NUM_PRESETS - 1);

        change_plugin_row = true;
        draw_MIDI_CC_screen();
        draw_edit_presetNr_ccValue(n, lastProw);
       // enc_moved[n] = false;
    }
}



// helpers
// sequencer note input stuff
void Track::set_active_note(uint8_t _clip, uint8_t _tick, uint8_t _voice, uint8_t _note)
{
    this->clip[_clip].tick[_tick].voice[_voice] = _note;
}
uint8_t Track::get_active_note(uint8_t _clip, uint8_t _tick, uint8_t _voice)
{
    return this->clip[_clip].tick[_tick].voice[_voice];
}
void Track::set_active_velo(uint8_t _clip, uint8_t _tick, uint8_t _voice, uint8_t _velo)
{
    this->clip[_clip].tick[_tick].velo[_voice] = _velo;
}
uint8_t Track::get_active_velo(uint8_t _clip, uint8_t _tick, uint8_t _voice)
{
    return this->clip[_clip].tick[_tick].velo[_voice];
}
void Track::set_active_stepFX(uint8_t _clip, uint8_t _tick, uint8_t _voice, uint8_t _stepFX)
{
    this->clip[_clip].tick[_tick].stepFX = _stepFX;
}
uint8_t Track::get_active_stepFX(uint8_t _clip, uint8_t _tick, uint8_t _voice)
{
    return this->clip[_clip].tick[_tick].stepFX;
}
void Track::set_note_on_tick(int x, int y)
{
    uint8_t note2set;
    for (int i = 0; i < parameter[SET_STEP_LENGTH]; i++)
    {
        sTick = x + i;
        note2set = (y - 1) + (parameter[SET_OCTAVE] * NOTES_PER_OCTAVE);
        search_free_voice = (y - 1);
        this->check_for_free_voices(sTick, note2set);
    }
}
void Track::check_for_free_voices(uint8_t onTick, uint8_t newNote)
{
    Serial.printf("newNote: %d onTick: %d\n", newNote, onTick);

    // löschen der Note

    if (get_active_note(parameter[SET_CLIP2_EDIT], onTick, search_free_voice) == newNote)
    {

        set_active_note(parameter[SET_CLIP2_EDIT], onTick, search_free_voice, NO_NOTE);
        set_active_velo(parameter[SET_CLIP2_EDIT], onTick, search_free_voice, parameter[SET_VELO2SET]);
        set_active_stepFX(parameter[SET_CLIP2_EDIT], onTick, search_free_voice, parameter[SET_STEP_FX]);
    }
    // setzen neuer Note
    else if (get_active_note(parameter[SET_CLIP2_EDIT], onTick, search_free_voice) == NO_NOTE)
    {

        set_active_note(parameter[SET_CLIP2_EDIT], onTick, search_free_voice, newNote);
        set_active_velo(parameter[SET_CLIP2_EDIT], onTick, search_free_voice, parameter[SET_VELO2SET]);
        set_active_stepFX(parameter[SET_CLIP2_EDIT], onTick, search_free_voice, parameter[SET_STEP_FX]);
    }

    int trellisColor;
    for (int v = 0; v < MAX_VOICES; v++)
    {

        if (get_active_note(parameter[SET_CLIP2_EDIT], onTick, v) == NO_NOTE)
        {
            trellisColor = TRELLIS_BLACK;
            //   tftColor = ILI9341_DARKGREY;
            
        }
        else if (get_active_note(parameter[SET_CLIP2_EDIT], onTick, v) < NO_NOTE)
        {
            trellisColor = trellisTrackColor[my_Arranger_Y_axis - 1];
            // tftColor = trackColor[my_Arranger_Y_axis - 1] + (this->parameter[SET_CLIP2_EDIT] * 20);
            break;
        }
    }
    //uint8_t note = get_active_note(parameter[SET_CLIP2_EDIT], onTick, search_free_voice);
    if (active_track == my_Arranger_Y_axis - 1)
        draw_note_on_tick(search_free_voice, onTick);

    trellis_set_main_buffer(parameter[SET_CLIP2_EDIT], (onTick /  TICKS_PER_STEP), (my_Arranger_Y_axis - 1), trellisColor);
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
