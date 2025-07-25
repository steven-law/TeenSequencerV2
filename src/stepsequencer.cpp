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
            set_stepSequencer_parameter_value(ENCODER_STEP_FX, 3, CCnames[parameter[14]], 0, 129); // yPos=3 to get parameter 14
        else if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
            set_stepSequencer_parameter_value(ENCODER_STEP_FX, 0, CCnames[parameter[14]], 0, 128); // yPos=0 to get parameter 2
        set_stepSequencer_parameter_value(3, 0, "Velo", 0, 127);
        break;
    case 1:
        set_stepSequencer_parameter_value(ENCODER_SEQUENCE_LENGTH, 1, "seqL", 6, MAX_TICKS);
        set_stepSequencer_parameter_value(ENCODER_CLOCK_DIVISION, 1, "cDiv", 1, MAX_TICKS);
        set_stepSequencer_parameter_value(ENCODER_STEP_LENGTH, 1, "stpL", 1, MAX_TICKS);
        set_stepSequencer_parameter_value(ENCODER_OCTAVE, 1, "Oct", 0, 11);

        break;
    case 2:

        set_stepSequencer_parameter_value(ENCODER_SEQ_MODE, 2, "sMod", 0, NUM_PLAYMODES - 1);
        set_stepSequencer_parameter_value(ENCODER_SCALE, 2, "scal", 0, NUM_SCALES - 1);
        set_stepSequencer_parameter_value(ENCODER_MIDICH_OUT, 2, "MCh", 0, MAX_OUTPUTS);
        if (neotrellisPressed[TRELLIS_BUTTON_SHIFT] && enc_moved[ENCODER_CLIP2_EDIT])
        {
            copy_clip();
        }
        set_stepSequencer_parameter_value(ENCODER_CLIP2_EDIT, 2, "Clip", 0, NUM_USER_CLIPS);

        break;
    case 3:
        set_stepSequencer_parameter_value(ENCODER_PROBABILITY, 3, "Offset", 0, MAX_TICKS);
        set_stepSequencer_parameter_value(ENCODER_HUMANIZE, 3, "rotVc", -1, 1);

    default:
        break;
    }
    draw_stepSequencer_parameters();
}
void Track::copy_clip()
{
    uint8_t nextClip = parameter[SET_CLIP2_EDIT] + encoded[ENCODER_CLIP2_EDIT];
    for (int s = 0; s < MAX_TICKS; s++)
    {
        for (int v = 0; v < MAX_VOICES; v++)
        {

            clip[nextClip].tick[s].voice[v] = clip[parameter[SET_CLIP2_EDIT]].tick[s].voice[v];
            clip[nextClip].tick[s].startTick[v] = clip[parameter[SET_CLIP2_EDIT]].tick[s].startTick[v];
            clip[nextClip].tick[s].noteLength[v] = clip[parameter[SET_CLIP2_EDIT]].tick[s].noteLength[v];
            clip[nextClip].tick[s].velo[v] = clip[parameter[SET_CLIP2_EDIT]].tick[s].velo[v];
            clip[nextClip].tick[s].stepFX = clip[parameter[SET_CLIP2_EDIT]].tick[s].stepFX;
            if (clip[nextClip].tick[s].voice[v] < NO_NOTE)
                trellisOut.set_main_buffer(nextClip, s / TICKS_PER_STEP, my_Arranger_Y_axis - 1, trellisTrackColor[my_Arranger_Y_axis - 1]);
        }
        Serial.printf("tick: %d, newStepFX: %d, oldStepFX: %d\n", s, clip[nextClip].tick[s].stepFX, clip[parameter[SET_CLIP2_EDIT]].tick[s].stepFX);
    }
    clip[nextClip].clockDivision = clip[parameter[SET_CLIP2_EDIT]].clockDivision;
    parameter[SET_CLOCK_DIVISION] = clip[parameter[SET_CLIP2_EDIT]].clockDivision;

    clip[nextClip].midiChOut = clip[parameter[SET_CLIP2_EDIT]].midiChOut;
    parameter[SET_MIDICH_OUT] = clip[parameter[SET_CLIP2_EDIT]].midiChOut;

    clip[nextClip].playMode = clip[parameter[SET_CLIP2_EDIT]].playMode;
    parameter[SET_SEQ_MODE] = clip[parameter[SET_CLIP2_EDIT]].playMode;

    clip[nextClip].scale = clip[parameter[SET_CLIP2_EDIT]].scale;
    parameter[SET_SCALE] = clip[parameter[SET_CLIP2_EDIT]].scale;

    clip[nextClip].seqLength = clip[parameter[SET_CLIP2_EDIT]].seqLength;
    parameter[SET_SEQUENCE_LENGTH] = clip[parameter[SET_CLIP2_EDIT]].seqLength;
}
void Track::set_stepSequencer_parameter_value(uint8_t XPos, uint8_t YPos, const char *name, int min, uint8_t max)
{
    if (!(gridTouchY > 0 && gridTouchY <= 13 && pixelTouchX >= SEQ_GRID_LEFT))
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
            trellisOut.drawPiano();
            draw_notes_in_grid();
        }
        break;
        case SET_HUMANIZE:
        {
            if (clip != nullptr)
            {
                if (voice_to_edit == 12)
                {
                    for (int v = 0; v < MAX_VOICES; v++)
                    {
                        rotateVoiceInClip(clip[parameter[SET_CLIP2_EDIT]], v, encoded[ENCODER_HUMANIZE], clip[parameter[SET_CLIP2_EDIT]].seqLength);
                    }
                }
                else
                    rotateVoiceInClip(clip[parameter[SET_CLIP2_EDIT]], voice_to_edit, encoded[ENCODER_HUMANIZE], clip[parameter[SET_CLIP2_EDIT]].seqLength);
            }
            else
            {
                Serial.println("clip is null!");
            }

            // draw_Notenames();
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
            trellisOut.drawPiano();
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
            int start_tick = 0;
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

// MIDI CC
void Track::set_CCchannel(uint8_t XPos, uint8_t YPos)
{
    int n = XPos + (YPos * NUM_ENCODERS);
    if (enc_moved[XPos])
    {
        CCchannel[edit_presetNr_ccChannel][n] = constrain(CCchannel[edit_presetNr_ccChannel][n] + encoded[XPos], 1, 129);
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
void Track::rotateVoiceInClip(clip_t &clip, int voiceIndex, int rotation, int maxTicks)
{
    // Temporäre Arrays für alle Parameter
    uint8_t voiceTemp[MAX_TICKS];
    uint8_t veloTemp[MAX_TICKS];
    uint8_t noteLengthTemp[MAX_TICKS];
    uint8_t startTickTemp[MAX_TICKS];
    uint8_t stepFXTemp[MAX_TICKS]; // Wird nur gespeichert, wenn voiceIndex == 0
    uint8_t trellisTemp[NUM_STEPS];
    Serial.println("Saving to temp arrays...");

    for (int i = 0; i < maxTicks; ++i)
    {
        voiceTemp[i] = clip.tick[i].voice[voiceIndex];
        veloTemp[i] = clip.tick[i].velo[voiceIndex];
        noteLengthTemp[i] = clip.tick[i].noteLength[voiceIndex];
        startTickTemp[i] = clip.tick[i].startTick[voiceIndex];
        trellisTemp[i / TICKS_PER_STEP] = trellisOut.get_main_buffer(parameter[SET_CLIP2_EDIT], i / TICKS_PER_STEP, my_Arranger_Y_axis - 1);
        stepFXTemp[i] = clip.tick[i].stepFX;
    }

    Serial.println("Rotating temp arrays...");

    rotateIntArray(voiceTemp, maxTicks, rotation);
    rotateIntArray(veloTemp, maxTicks, rotation);
    rotateIntArray(noteLengthTemp, maxTicks, rotation);
    rotateIntArray(startTickTemp, maxTicks, rotation);
    rotateIntArray(trellisTemp, NUM_STEPS, rotation);

    rotateIntArray(stepFXTemp, maxTicks, rotation);

    Serial.println("Writing rotated data back...");

    for (int i = 0; i < maxTicks; ++i)
    {
        clip.tick[i].voice[voiceIndex] = voiceTemp[i];
        clip.tick[i].velo[voiceIndex] = veloTemp[i];
        clip.tick[i].noteLength[voiceIndex] = noteLengthTemp[i];
        static int oldNote;
        int temptick = -1;
        if (oldNote != clip.tick[i].voice[voiceIndex])
        {
            temptick = i;
            oldNote = clip.tick[i].voice[voiceIndex];
            Serial.printf("oldNote= %d\n", oldNote);
        }
        clip.tick[i].startTick[voiceIndex] = temptick;

        int trelliscolorTemp = TRELLIS_BLACK;

        //  if (clip.tick[i].voice[voiceIndex] == NO_NOTE)
        //  {
        //      trelliscolorTemp = TRELLIS_BLACK;
        //  }
        //  if (clip.tick[i].voice[voiceIndex] < NO_NOTE)
        //  {
        //      trelliscolorTemp = trellisTrackColor[my_Arranger_Y_axis - 1];
        //  }

        for (int l = 0; l < TICKS_PER_STEP; l++)
        {
            if (clip.tick[i + l].voice[voiceIndex] < NO_NOTE)
            {
                trelliscolorTemp = trellisTrackColor[my_Arranger_Y_axis - 1];
                break;
            }
        }

        if (i % TICKS_PER_STEP == 0)
            trellisOut.set_main_buffer(parameter[SET_CLIP2_EDIT], i / TICKS_PER_STEP, my_Arranger_Y_axis - 1, trelliscolorTemp);
        clip.tick[i].stepFX = stepFXTemp[i];
        if (clip.tick[i].voice[voiceIndex] != NO_NOTE && clip.tick[i].startTick[voiceIndex] < MAX_TICKS && clip.tick[i].velo[voiceIndex] != 0)
            Serial.printf("note= %d, start= %d, length= %d,  velo= %d\n", clip.tick[i].voice[voiceIndex], clip.tick[i].startTick[voiceIndex], clip.tick[i].noteLength[voiceIndex], clip.tick[i].velo[voiceIndex]);
    }
    trellisOut.writeDisplay();
    Serial.println("Done rotating voice and associated parameters.");
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
        return;

    clip_t *clipPtr = clip;
    tick_t *tickPtr = clipPtr[parameter[SET_CLIP2_EDIT]].tick;

    uint8_t note2set = _note;
    int _voice = _note % 12;
    uint8_t velocity = parameter[SET_VELO2SET];
    uint8_t stepFX = parameter[SET_STEP_FX];

    uint8_t existingNote = tickPtr[_startTick].voice[_voice];
    bool isNewNote = (existingNote == NO_NOTE);
    int existingStartTick = tickPtr[_startTick].startTick[_voice];
    int existingLength = tickPtr[existingStartTick].noteLength[_voice];
    int trellisColor = TRELLIS_BLACK;
    Serial.printf("trelliscolor: %d\n", trellisColor);
    // Serial.println(isNewNote ? "true" : "false");
    // Serial.printf("existingNote = %d, newNote = %d\n", existingNote, note2set);
    // Prüfen, ob _startTick innerhalb einer bestehenden Note liegt
    if (existingNote != NO_NOTE &&
        existingStartTick >= 0 && existingStartTick < 96 &&
        _startTick >= existingStartTick &&
        _startTick < existingStartTick + existingLength)
    {
        // Note löschen
        for (int i = 0; i < existingLength; i++)
        {
            int tickToClear = existingStartTick + i;
            if (tickToClear >= 96)
                break;

            tickPtr[tickToClear].voice[_voice] = NO_NOTE;
            tickPtr[tickToClear].velo[_voice] = 0;
            tickPtr[tickToClear].startTick[_voice] = MAX_TICKS;
            tickPtr[tickToClear].stepFX = 0;

            for (int v = 0; v < MAX_VOICES; v++)
            {
                if (get_note_parameter(tickPtr[_startTick].voice, v) < NO_NOTE)
                {
                    trellisColor = trellisTrackColor[my_Arranger_Y_axis - 1];
                    Serial.printf("voice: %d, note: %d, trelliscolor: %d\n", v, get_note_parameter(tickPtr[_startTick].voice, v), trellisColor);
                    break;
                }
            }
            trellisOut.set_main_buffer(parameter[SET_CLIP2_EDIT], tickToClear / TICKS_PER_STEP, my_Arranger_Y_axis - 1, trellisColor);
        }
        Serial.printf("note is erased: note: %d, startTick: %d, length: %d, velo: %d, stepFx: %d\n",
                      tickPtr[_startTick].voice[_voice],
                      tickPtr[_startTick].startTick[_voice],
                      tickPtr[_startTick].noteLength[_voice],
                      tickPtr[_startTick].velo[_voice],
                      tickPtr[_startTick].stepFX);
        tickPtr[existingStartTick].noteLength[_voice] = 0;

        // Eventuell noch die Note-Visualisierung löschen
        if (active_track == my_Arranger_Y_axis - 1 && activeScreen == INPUT_FUNCTIONS_FOR_SEQUENCER)
        {
            erase_note_on_tick(_voice, existingStartTick, existingLength);
        }

        return; // Kein Setzen einer neuen Note in diesem Fall
    }

    // Neue Note setzen
    if (isNewNote)
    {
        tickPtr[_startTick].noteLength[_voice] = length;

        for (int i = 0; i < length; i++)
        {
            int onTick = _startTick + i;
            if (onTick >= 96)
                break;

            tickPtr[onTick].voice[_voice] = note2set;
            tickPtr[onTick].velo[_voice] = velocity;
            tickPtr[onTick].startTick[_voice] = _startTick;
            tickPtr[onTick].stepFX = stepFX;
            trellisOut.set_main_buffer(parameter[SET_CLIP2_EDIT], onTick / TICKS_PER_STEP, my_Arranger_Y_axis - 1, trellisTrackColor[my_Arranger_Y_axis - 1]);
            // Farbe fürs Display
        }
        Serial.printf("note is set to: note: %d, startTick: %d, length: %d, velo: %d, stepFx: %d\n",
                      tickPtr[_startTick].voice[_voice],
                      tickPtr[_startTick].startTick[_voice],
                      tickPtr[_startTick].noteLength[_voice],
                      tickPtr[_startTick].velo[_voice],
                      tickPtr[_startTick].stepFX);
        // Zeichnen der Note
        if (active_track == my_Arranger_Y_axis - 1 && activeScreen == INPUT_FUNCTIONS_FOR_SEQUENCER)
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
        trellisOut.set_main_buffer(parameter[SET_CLIP2_EDIT], (i / TICKS_PER_STEP), (my_Arranger_Y_axis - 1), TRELLIS_BLACK);
    }
    if (activeScreen == INPUT_FUNCTIONS_FOR_SEQUENCER)
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
