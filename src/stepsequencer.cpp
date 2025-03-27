#include <Arduino.h>
#include "projectVariables.h"

#include "Track.h"

void Track::set_stepSequencer_parameters(uint8_t row)
{

    switch (row)
    {
    case 0:
        encoder_SetCursor(parameter[SET_STEP_LENGTH] * 3, 14); // Encoder: 0+1
        set_stepSequencer_parameter_value(0, 0, "Tick", 0, 160);
        set_stepSequencer_parameter_value(1, 0, "Note", 0, 14);

        set_stepSequencer_parameter_value(ENCODER_STEP_FX, 0, CCnames[setStepFX], 0, 128);
        set_stepSequencer_parameter_value(3, 0, "Velo", 0, 127);
        break;
    case 1:
        set_stepSequencer_parameter_value(ENCODER_SEQUENCE_LENGTH, 1, "seqL", 1, MAX_TICKS);
        set_stepSequencer_parameter_value(ENCODER_CLOCK_DIVISION, 1, "cDiv", 1, MAX_TICKS);
        set_stepSequencer_parameter_value(ENCODER_STEP_LENGTH, 1, "stpL", 1, MAX_TICKS);
        set_stepSequencer_parameter_value(ENCODER_OCTAVE, 1, "Oct", 0, 11);

        break;
    case 2:
        set_stepSequencer_parameter_text(ENCODER_SEQ_MODE, 2, "sMod", seqModname[parameter[SET_SEQ_MODE]], 0, NUM_PLAYMODES - 1);
        set_stepSequencer_parameter_value(ENCODER_SCALE, 2, "scal", 0, NUM_SCALES - 1);

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
        // enc_moved[XPos] = false;
        parameter[index] = constrain(parameter[index] + encoded[XPos], min, max);
        Serial.printf("parameter: %d, value: %d, name %s\n", index, parameter[index], name);
        switch (index)
        {
        case SET_OCTAVE:
        case SET_CLIP2_EDIT:
            draw_notes_in_grid();
            break;

        case SET_SCALE:
            draw_Notenames();
            trellis_show_piano();
            break;

        case SET_VELO2SET:
        case SET_STEP_FX:
        {
            // Notenlänge bestimmen
            //   int note_length = 0;
            //   for (int i = tick_to_edit; i < MAX_TICKS && clip[parameter[SET_CLIP2_EDIT]].tick[i].voice[voice_to_edit] != NO_NOTE; i++)
            //   {
            //       note_length++;
            //   }

            // Änderungen anwenden
            //  for (int i = 0; i < note_length; i++)
            //  {
            //      int tick = tick_to_edit + i;
            //      if (index == SET_VELO2SET)
            //          clip[parameter[SET_CLIP2_EDIT]].tick[tick].velo[voice_to_edit] = parameter[SET_VELO2SET];
            //      else
            //          clip[parameter[SET_CLIP2_EDIT]].tick[tick].stepFX[voice_to_edit] = parameter[SET_STEP_FX];
            //  }
            //  erase_note_on_tick(voice_to_edit, tick_to_edit, note_length);
            //  draw_note_on_tick(voice_to_edit, tick_to_edit);
            int note = voice_to_edit + (parameter[SET_OCTAVE] * NOTES_PER_OCTAVE);
            if (index == SET_VELO2SET)
                setNoteParameterFromClip( tick_to_edit, note, parameter[SET_VELO2SET], 1);
            // clip[parameter[SET_CLIP2_EDIT]].tick[tick].velo[voice_to_edit] = parameter[SET_VELO2SET];
            else
                setNoteParameterFromClip( tick_to_edit, note, parameter[SET_STEP_FX], 2);
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
uint8_t Track::get_note_parameter(uint8_t *parameterArray, uint8_t _voice)
{
    return parameterArray[_voice];
}
int Track::getNoteParameterFromClip(int clipIndex, int cursorXPos, int cursorYPos, int index) // index: 0 pitch, 1 velocity, 2 stepFX, 3 noteLength
{
    if (clipIndex >= NUM_USER_CLIPS)
        return -1;
    return clips[clipIndex].getNoteParameter(cursorXPos, cursorYPos, index);
}

void Track::setNoteParameterFromClip(int cursorXPos, int cursorYPos, int value, int index) // index: 0 pitch, 1 velocity, 2 stepFX, 3 noteLength
{
    if (parameter[SET_CLIP2_EDIT] >= NUM_USER_CLIPS)
        return;
    clips[parameter[SET_CLIP2_EDIT]].setNoteParameter(cursorXPos, cursorYPos, value, index);
}
void Track::set_note_on_tick(int startTick, int pitch, int length){
    int velo = parameter[SET_VELO2SET];
    int stepfx = parameter[SET_STEP_FX];

    clips[parameter[SET_CLIP2_EDIT]].set_note_on_tick(pitch,velo,stepfx, startTick, length );
}
/*void Track::set_note_on_tick(int x, int voice)
{
    uint8_t note2set = voice + (parameter[SET_OCTAVE] * NOTES_PER_OCTAVE);
    uint8_t noteInClip = clip[parameter[SET_CLIP2_EDIT]].tick[x].voice[voice];
    // oldNote = (oldNote == note2set) ? NO_NOTE : (oldNote == NO_NOTE ? note2set : oldNote);

    for (int i = 0; i < parameter[SET_STEP_LENGTH]; i++)
    {

        uint8_t onTick = x + i;

        // Note löschen oder setzen
        if (noteInClip == note2set)
        {
            clip[parameter[SET_CLIP2_EDIT]].tick[onTick].voice[voice] = NO_NOTE;
            clip[parameter[SET_CLIP2_EDIT]].tick[onTick].velo[voice] = 0;
        }
        else if (noteInClip == NO_NOTE)
        {
            clip[parameter[SET_CLIP2_EDIT]].tick[onTick].voice[voice] = note2set;
            clip[parameter[SET_CLIP2_EDIT]].tick[onTick].velo[voice] = parameter[SET_VELO2SET];
        }

        // Parameter setzen

        clip[parameter[SET_CLIP2_EDIT]].tick[onTick].stepFX[voice] = parameter[SET_STEP_FX];

        // Farbe für Trellis bestimmen
        int trellisColor = TRELLIS_BLACK;
        for (int v = 0; v < MAX_VOICES; v++)
        {
            if (get_note_parameter(clip[parameter[SET_CLIP2_EDIT]].tick[onTick].voice, v) < NO_NOTE)
            {
                trellisColor = trellisTrackColor[my_Arranger_Y_axis - 1];
                break;
            }
        }
        // Serial.printf("Set NOte: %d on ntick: %d \n", clip[parameter[SET_CLIP2_EDIT]].tick[onTick].voice[voice], onTick);
        trellis_set_main_buffer(parameter[SET_CLIP2_EDIT], onTick / TICKS_PER_STEP, my_Arranger_Y_axis - 1, trellisColor);
    }
    if (active_track == my_Arranger_Y_axis - 1)
    {
        if (clip[parameter[SET_CLIP2_EDIT]].tick[x].voice[voice] == NO_NOTE)
            erase_note_on_tick(voice, x, parameter[SET_STEP_LENGTH]);
        else
        {
            draw_note_on_tick(voice, x);
        }
    }
}*/

void Track::clear_active_clip()
{
    // for (int i = 0; i < MAX_TICKS; i++)
    // {
    //     for (int n = 0; n < MAX_VOICES; n++)
    //     {

    //         this->clip[parameter[SET_CLIP2_EDIT]].tick[i].voice[n] = NO_NOTE;
    //         this->clip[parameter[SET_CLIP2_EDIT]].tick[i].velo[n] = 0;

    //         // draw_note_on_tick(n, i);
    //     }
    //     trellis_set_main_buffer(parameter[SET_CLIP2_EDIT], (i / TICKS_PER_STEP), (my_Arranger_Y_axis - 1), TRELLIS_BLACK);
    // }
    for (int i = 0; i <= clips->noteCount; i++)
        clips->delete_active_clip();
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
