#include <Arduino.h>
#include "projectVariables.h"
#include "Track.h"
extern MidiTrack myMidi[NUM_TRACKS];
// hello test
void Track::set_presetNr()
{
    if (enc_moved[PRESET_ENCODER])
    {
        PMpresetNr = constrain(PMpresetNr + encoded[PRESET_ENCODER], 0, NUM_PLUGIN_PRESETS - 1);
        change_plugin_row = true;
        Serial.println("change playmode preset");
        draw_sequencer_modes(parameter[SET_SEQ_MODE]);
    }
}
void Track::set_seq_mode_value(uint8_t modeindex, uint8_t XPos, uint8_t YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int n = XPos + (YPos * NUM_ENCODERS);
        seqMod_value[modeindex][PMpresetNr][n] = constrain(seqMod_value[modeindex][PMpresetNr][n] + encoded[XPos], min, max); // mode 3 =MIDI_CC_RANGE * 2 //mode4 =NO_NOTE //others=MIDI_CC_RANGE
        drawPot(XPos, YPos, seqMod_value[modeindex][PMpresetNr][n], name);
    }
}
void Track::rotateIntArray(uint8_t arr[], int maxSteps, int rotation)
{
    int r = rotation * (-1);
    int temp[MAX_TICKS]; // Hilfsarray für die Rotation
    for (int i = 0; i < maxSteps; i++)
    {
        temp[i] = arr[(i - r + maxSteps) % maxSteps];
        //  Serial.printf("rotation: %d,maxsteps. %d,i: %d arr[i]: %d, temp[i]: %d\n", rotation, maxSteps, i, arr[i], temp[i]);
    }

    for (int i = 0; i < maxSteps; i++)
    {
        arr[i] = temp[i];
    }
}
uint8_t Track::get_random_Note_in_scale()
{
    int randomNote;
    const uint8_t clipIndex = clip_to_play[internal_clock_bar];

    // Wiederhole, bis eine gültige Note gefunden wurde
    do
    {
        randomNote = random(0, 12);
    } while (!scales[clip[clipIndex].midiChOut][randomNote]);
    return randomNote;
}
uint8_t Track::transpose_to_scale(uint8_t noteToChange)
{
    const uint8_t clipIndex = clip_to_play[internal_clock_bar];
    const uint8_t scaleChannel = clip[clipIndex].midiChOut;
    uint8_t noteName = noteToChange % 12;

    // Wenn Note bereits in Skala, zurückgeben
    if (scales[scaleChannel][noteName])
    {
        return noteName;
    }

    // Suche nächsthöhere Note in Skala (zyklisch von 0–11)
    for (uint8_t i = 1; i < 12; ++i)
    {
        uint8_t nextNote = (noteName + i) % 12;
        if (scales[scaleChannel][nextNote])
        {
            return nextNote;
        }
    }

    // Fallback (theoretisch unnötig, wenn Skala mindestens 1 Note enthält)
    return noteName;
}
void Track::play_seq_mode0(uint8_t cloock)
{
    const uint8_t clipIndex = clip_to_play[external_clock_bar];
    const auto &tick = clip[clipIndex].tick[cloock];

    const float velocityScale = (barVelocity[external_clock_bar] / 127.0f) * (mixGainPot / 127.0f);
    const uint8_t noteOffsetValue = noteOffset[external_clock_bar] + performNoteOffset;

    for (int v = 0; v < MAX_VOICES; v++)
    {
        uint8_t noteParam = get_note_parameter(tick.voice, v);
        if (get_note_parameter(tick.startTick, v) == cloock && noteParam < NO_NOTE && random(126) < barProbabilty[external_clock_bar])
        {
            {

                { // Wahrscheinlichkeit
                    noteToPlay[v] = noteParam + noteOffsetValue;
                    uint8_t Velo = get_note_parameter(tick.velo, v) * velocityScale;
                    uint8_t StepFX = get_note_parameter(&tick.stepFX, v);
                    noteOffAt[v] = constrain(tick.startTick[v] + tick.noteLength[v], 0, MAX_TICKS - 1);

                    note_is_on[v] = true;
                    sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut);
                    noteOn(noteToPlay[v], Velo, clip[clipIndex].midiChOut);
                }
            }
        }
        if (noteOffAt[v] == cloock && note_is_on[v])
        {
            note_is_on[v] = false;
            noteOffAt[v] = -1;
            noteOff(noteToPlay[v], 0, clip[clipIndex].midiChOut);
            //  if (my_Arranger_Y_axis == 1)
            //    Serial.printf("tick: %d   stop Note: %d, start: %d, length: %d\n", cloock, noteToPlay[v], tick.startTick[v], tick.noteLength[v]);
            noteToPlay[v] = NO_NOTE;
        }
    }
}
// random
void Track::play_seq_mode1(uint8_t cloock)
{
    const uint8_t clipIndex = clip_to_play[internal_clock_bar];
    const auto &tick = clip[clipIndex].tick[cloock];
    uint8_t noteParam = get_note_parameter(tick.voice, 0);
    uint8_t playPresetNr = play_presetNr_Playmode_ccChannel[external_clock_bar];

    if (get_note_parameter(tick.startTick, 0) == cloock && noteParam < NO_NOTE && random(126) < barProbabilty[external_clock_bar])

    {

        static int stepCount;                                                // max Steps
        stepCount = (stepCount + 1) % seqMod_value[1][playPresetNr][4];      // max Steps
        bool useMemory = ((random(126)) < seqMod_value[1][playPresetNr][5]); // dejavu
        int noteToSend = useMemory ? seqModNoteMemory[stepCount] : get_random_Note_in_scale();
        int note2save = noteToSend + (random(seqMod_value[1][playPresetNr][0], seqMod_value[1][playPresetNr][1] + 1) * 12) + noteOffset[external_clock_bar] + performNoteOffset;
        noteToPlay[0] = seqModNoteMemory[stepCount];
        noteOffAt[0] = constrain(tick.startTick[0] + tick.noteLength[0], 0, MAX_TICKS - 1);
        uint8_t Velo = random(seqMod_value[1][playPresetNr][2], seqMod_value[1][playPresetNr][3]) * (barVelocity[external_clock_bar] / MIDI_CC_RANGE_FLOAT) * (mixGainPot / MIDI_CC_RANGE_FLOAT);
        uint8_t StepFX = random(seqMod_value[1][playPresetNr][8], seqMod_value[1][playPresetNr][9]);
        sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut);
        noteOn(noteToPlay[0], Velo, clip[clipIndex].midiChOut); // Send a Note
        Serial.printf("pl1 Note: %d, Velo: %d, StartTick: %d, EndTick: %d, StepFX: %d\n", noteToPlay[0], Velo, tick.startTick[0], noteOffAt[0], StepFX);

        // save new note into array
        if (!useMemory)
        {
            seqModNoteMemory[stepCount] = note2save;
            // Serial.printf("PM1 = save note: %d on step: %d\n", seqModNoteMemory[stepCount], stepCount);
            //  velocityMemory[stepCount] = volSend;
        }
    }

    if (noteOffAt[0] == cloock)
    {
        noteOffAt[0] = -1;
        noteOff(noteToPlay[0], 0, clip[clipIndex].midiChOut); // Send a Note (pitch 42, velo 127 on channel 1)
        // Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
        noteToPlay[0] = NO_NOTE;
    }
}
void Track::set_seq_mode1_parameters()
{ // random
    /*
        0  octmin
        1  octmax
        2  volmin
        3  volmax
        4  maxsteps
        5  dejaVu
        6  rotate
        7  save to clip
        8  StepFX -
        9  StepFX +
          */
    draw_seq_mode1();
    if (lastPotRow == 3 && enc_button[3]) // save to clip
    {
        clear_active_clip();
        enc_button[3] = false;

        for (int s = 0; s < seqMod_value[1][PMpresetNr][4]; s++)
        {
            int _note = seqModNoteMemory[s];
            set_note_on_tick(s * TICKS_PER_STEP, _note, parameter[SET_STEP_LENGTH]);
        }
    }
    if (lastPotRow == 1) // lock sequence immedietly
    {
        if (enc_button[3])
        {
            seqMod_value[1][PMpresetNr][5] = 127;
            change_plugin_row = true;
        }
        if (enc_moved[2])
            rotateIntArray(seqModNoteMemory, seqMod_value[1][PMpresetNr][4], encoded[2]);
    }
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        switch (lastPotRow)
        {
        case 0:
        {

            set_seq_mode_value(1, 0, 0, "Oct -", 0, 11);
            set_seq_mode_value(1, 1, 0, "Oct +", 0, 11);
            set_seq_mode_value(1, 2, 0, "Vol -", 0, MIDI_CC_RANGE);
            set_seq_mode_value(1, 3, 0, "Vol +", 0, MIDI_CC_RANGE);
        }
        break;
        case 1:
        {
            set_seq_mode_value(1, 0, 1, "maxSteps", 0, NUM_STEPS);
            set_seq_mode_value(1, 1, 1, "Dejavu", 0, MIDI_CC_RANGE);
            set_seq_mode_value(1, 2, 1, "Rotate", 0, 32);
        }
        break;
        case 2:
        {
            set_seq_mode_value(1, 0, 2, "StepFX -", 0, MIDI_CC_RANGE + 1);
            set_seq_mode_value(1, 1, 2, "StepFX +", 0, MIDI_CC_RANGE + 1);
        }
        break;
        case 3:
        {
        }
        break;
        default:
            break;
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
        set_presetNr();
}
void Track::draw_seq_mode1()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;

        drawPot(0, 0, seqMod_value[1][PMpresetNr][0], "Oct -");
        drawPot(1, 0, seqMod_value[1][PMpresetNr][1], "Oct +");
        drawPot(2, 0, seqMod_value[1][PMpresetNr][2], "Vol -");
        drawPot(3, 0, seqMod_value[1][PMpresetNr][3], "Vol +");

        drawPot(0, 1, seqMod_value[1][PMpresetNr][4], "maxSteps");
        drawPot(1, 1, seqMod_value[1][PMpresetNr][5], "Dejavu");
        drawPot(2, 1, seqMod_value[1][PMpresetNr][6], "Rotate");

        drawPot(0, 2, seqMod_value[1][PMpresetNr][8], "StepFX -");
        drawPot(1, 2, seqMod_value[1][PMpresetNr][9], "StepFX +");
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 11, 4, 4, NO_VALUE, "Prset", ILI9341_BLUE, 2, false, false);
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 12, 4, 4, PMpresetNr, NO_NAME, ILI9341_BLUE, 2, true, false);
    }
}
// dropseq
void Track::play_seq_mode2(uint8_t cloock)
{
    const uint8_t clipIndex = clip_to_play[internal_clock_bar];
    uint8_t playPresetNr = play_presetNr_Playmode_ccChannel[external_clock_bar];
    const auto &tick = clip[clipIndex].tick[cloock];
    uint8_t noteParam = get_note_parameter(tick.voice, 0);
    static uint8_t maxValIndex;
    static uint8_t analogReadArray[12];
    uint8_t cc23 = seqMod_value[2][playPresetNr][2];
    uint8_t cc24 = seqMod_value[2][playPresetNr][3];
    uint8_t thisOctave = 4;

    if (get_note_parameter(tick.startTick, 0) == cloock && noteParam < NO_NOTE && random(126) < barProbabilty[external_clock_bar])
    {

        maxVal = 0;
        if (seqMod_value[2][playPresetNr][2] <= seqMod_value[2][playPresetNr][3])
        {
            for (int i = 0; i < 12; i++)
            {
                if (analogReadArray[i] > maxVal)
                {
                    maxVal = analogReadArray[i];
                    maxValIndex = i;
                    thisOctave = random(cc23, cc24);
                }
            }
        }

        if (seqMod_value[2][playPresetNr][2] > seqMod_value[2][playPresetNr][3])
        {
            for (int i = 11; i >= 0; i--)
            {
                if (analogReadArray[i] > maxVal)
                {
                    maxVal = analogReadArray[i];
                    maxValIndex = i;
                    thisOctave = random(cc24, cc23);
                }
            }
        }

        noteOffAt[0] = constrain(tick.startTick[0] + tick.noteLength[0], 0, MAX_TICKS - 1);
        noteToPlay[0] = (maxValIndex) + (thisOctave * 12) + noteOffset[external_clock_bar] + performNoteOffset;
        seqModNoteMemory[cloock / TICKS_PER_STEP] = noteToPlay[0];
        // Serial.printf("octave:%d maxValIndex: %d, noteToplay: %d\n", thisOctave, maxValIndex, noteToPlay[0]);
        uint8_t Velo = get_note_parameter(clip[clip_to_play[internal_clock_bar]].tick[cloock].velo, 0) * (barVelocity[external_clock_bar] / 127.00) * (mixGainPot / 127.00);
        uint8_t StepFX = get_note_parameter(&clip[clip_to_play[internal_clock_bar]].tick[cloock].stepFX, 0);

        sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut);
        noteOn(noteToPlay[0], Velo, clip[clipIndex].midiChOut);
        Serial.printf("pl2 Note: %d, Velo: %d, StartTick: %d, EndTick: %d, StepFX: %d\n", noteToPlay[0], Velo, tick.startTick[0], noteOffAt[0], StepFX);
        analogReadArray[maxValIndex] = (analogReadArray[maxValIndex] - seqMod_value[2][playPresetNr][0]);
        // Serial.println(maxValIndex);
    }
    // NoteOff
    if (noteOffAt[0] == cloock)
    {
        noteOffAt[0] = -1;
        noteOff(noteToPlay[0], 0, clip[clipIndex].midiChOut);
        noteToPlay[0] = NO_NOTE;
        // Serial.println(track[i].notePlayed[0]);
    }

    if (analogReadArray[maxValIndex] <= seqMod_value[2][playPresetNr][1])
    {
        for (int i = 0; i < 12; i++)
        {
            analogReadArray[i] = seqMod_value[2][playPresetNr][i + 4];
        }
    }
}
void Track::set_seq_mode2_parameters()
{
    draw_seq_mode2();
    if (lastPotRow == 3 && enc_button[3]) // save to clip
    {
        clear_active_clip();
        enc_button[3] = false;

        for (int s = 0; s < NUM_STEPS; s++)
        {
            int _note = seqModNoteMemory[s];
            set_note_on_tick(s * TICKS_PER_STEP, _note, parameter[SET_STEP_LENGTH]);
        }
    }
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        switch (lastPotRow)
        {
        case 0:
        {
            set_seq_mode_value(2, 0, 0, "Drop", 0, NO_NOTE);
            set_seq_mode_value(2, 1, 0, "Rst @", 0, NO_NOTE);
            set_seq_mode_value(2, 2, 0, "Oct -", 0, NO_NOTE);
            set_seq_mode_value(2, 3, 0, "Oct +", 0, NO_NOTE);
        }
        break;
        case 1:
        {
            set_seq_mode_value(2, 0, 1, "C", 0, NO_NOTE);
            set_seq_mode_value(2, 1, 1, "C#", 0, NO_NOTE);
            set_seq_mode_value(2, 2, 1, "D", 0, NO_NOTE);
            set_seq_mode_value(2, 3, 1, "D#", 0, NO_NOTE);
        }
        break;
        case 2:
        {
            set_seq_mode_value(2, 0, 2, "E", 0, NO_NOTE);
            set_seq_mode_value(2, 1, 2, "F", 0, NO_NOTE);
            set_seq_mode_value(2, 2, 2, "F#", 0, NO_NOTE);
            set_seq_mode_value(2, 3, 2, "G", 0, NO_NOTE);
        }
        break;
        case 3:
        {
            set_seq_mode_value(2, 0, 3, "G#", 0, NO_NOTE);
            set_seq_mode_value(2, 1, 3, "A", 0, NO_NOTE);
            set_seq_mode_value(2, 2, 3, "A#", 0, NO_NOTE);
            set_seq_mode_value(2, 3, 3, "B", 0, NO_NOTE);
        }
        break;
        default:
            break;
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
        set_presetNr();
}
void Track::draw_seq_mode2()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, seqMod_value[2][PMpresetNr][0], "Drop");
        drawPot(1, 0, seqMod_value[2][PMpresetNr][1], "Rst @");
        drawPot(2, 0, seqMod_value[2][PMpresetNr][2], "Oct -");
        drawPot(3, 0, seqMod_value[2][PMpresetNr][3], "Oct +");

        drawPot(0, 1, seqMod_value[2][PMpresetNr][4], "C");
        drawPot(1, 1, seqMod_value[2][PMpresetNr][5], "C#");
        drawPot(2, 1, seqMod_value[2][PMpresetNr][6], "D");
        drawPot(3, 1, seqMod_value[2][PMpresetNr][7], "D#");

        drawPot(0, 2, seqMod_value[2][PMpresetNr][8], "E");
        drawPot(1, 2, seqMod_value[2][PMpresetNr][9], "F");
        drawPot(2, 2, seqMod_value[2][PMpresetNr][10], "F#");
        drawPot(3, 2, seqMod_value[2][PMpresetNr][11], "G");

        drawPot(0, 3, seqMod_value[2][PMpresetNr][12], "G#");
        drawPot(1, 3, seqMod_value[2][PMpresetNr][13], "A");
        drawPot(2, 3, seqMod_value[2][PMpresetNr][14], "A#");
        drawPot(3, 3, seqMod_value[2][PMpresetNr][15], "B");
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 11, 4, 4, NO_VALUE, "Prset", ILI9341_BLUE, 2, false, false);
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 12, 4, 4, PMpresetNr, NO_NAME, ILI9341_BLUE, 2, true, false);
    }
}
// bitread
void Track::play_seq_mode3(uint8_t cloock)
{
    const uint8_t clipIndex = clip_to_play[internal_clock_bar];
    const auto &tick = clip[clipIndex].tick[cloock];
    uint8_t seq3_clock = cloock / (TICKS_PER_STEP * 2);
    uint8_t playPresetNr = play_presetNr_Playmode_ccChannel[external_clock_bar];

    for (int v = 0; v < MAX_VOICES; v++)
    {

        if (bitRead(seqMod_value[3][playPresetNr][v], seq3_clock) && cloock % (TICKS_PER_STEP * 2) == 0 && random(126) < barProbabilty[external_clock_bar])
        {
            //  if (note_is_on[v])
            //  {
            //      note_is_on[v] = false;
            //      noteOff(noteToPlay[v], 0, parameter[clip[clipIndex].midiChOut]); // Send a Note (pitch 42, velo 127 on channel 1)
            //        Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
            //  }
            if (!note_is_on[v])
            {
                noteToPlay[v] = v + (parameter[SET_OCTAVE] * 12) + noteOffset[external_clock_bar] + performNoteOffset;
                // uint8_t Velo = get_note_parameter(clip[clip_to_play[internal_clock_bar]].tick[cloock].velo, v) * (barVelocity[external_clock_bar] / 127.00) * (mixGainPot / 127.00);
                uint8_t Velo = random(seqMod_value[3][playPresetNr][12], seqMod_value[3][playPresetNr][13]) * (barVelocity[external_clock_bar] / 127.00) * (mixGainPot / 127.00);
                uint8_t StepFX = random(seqMod_value[3][playPresetNr][14], seqMod_value[3][playPresetNr][15]);
                noteOffAt[v] = constrain(cloock + tick.noteLength[v], 0, MAX_TICKS - 1);
                note_is_on[v] = true;
                sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut);
                noteOn(noteToPlay[v], Velo, clip[clipIndex].midiChOut); // Send a Note (pitch 42, velo 127 on channel 1)
                // Serial.printf("ON   tick: %d, clock: %d, voice: %d, note: %d, seqModValue: %d\n", cloock, seq3_clock, v, noteToPlay[v], seqMod_value[3][v]);
                Serial.printf("pl3 Note: %d, Velo: %d, StartTick: %d, EndTick: %d, StepFX: %d\n", noteToPlay[v], Velo, cloock, noteOffAt[v], StepFX);
            }
        }

        // if (!bitRead(seqMod_value[3][v], seq3_clock))
        if (noteOffAt[v] == cloock) // bitread is an 8step sequencer(resolution is 2xTICKS_PER_STEP), so if seq3_clock is halfway to the next step, we send noteOffs to active notes
        {
            if (note_is_on[v])
            {
                note_is_on[v] = false;
                noteOff(noteToPlay[v], 0, clip[clipIndex].midiChOut); // Send a Note (pitch 42, velo 127 on channel 1)
                //  Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
                noteToPlay[v] = NO_NOTE;
            }
        }
    }
}
void Track::set_seq_mode3_parameters()
{
    draw_seq_mode3();
    if (lastPotRow == 3 && enc_button[3]) // save to clip
    {
        clear_active_clip();
        enc_button[3] = false;
        for (int v = 0; v < MAX_VOICES; v++)
        {
            for (int s = 0; s < 8; s++)
            {
                if (bitRead(seqMod_value[3][PMpresetNr][v], s))
                {
                    int _note = v + (parameter[SET_OCTAVE] * NOTES_PER_OCTAVE);
                    set_note_on_tick(s * 12, _note, parameter[SET_STEP_LENGTH]);
                }
            }
        }
    }
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        switch (lastPotRow)
        {
        case 0:
        {
            set_seq_mode_value(3, 0, 0, noteNames[0], 0, 255);
            set_seq_mode_value(3, 1, 0, noteNames[1], 0, 255);
            set_seq_mode_value(3, 2, 0, noteNames[2], 0, 255);
            set_seq_mode_value(3, 3, 0, noteNames[3], 0, 255);
        }
        break;
        case 1:
        {
            set_seq_mode_value(3, 0, 1, noteNames[4], 0, 255);
            set_seq_mode_value(3, 1, 1, noteNames[5], 0, 255);
            set_seq_mode_value(3, 2, 1, noteNames[6], 0, 255);
            set_seq_mode_value(3, 3, 1, noteNames[7], 0, 255);
        }
        break;
        case 2:
        {
            set_seq_mode_value(3, 0, 2, noteNames[8], 0, 255);
            set_seq_mode_value(3, 1, 2, noteNames[9], 0, 255);
            set_seq_mode_value(3, 2, 2, noteNames[10], 0, 255);
            set_seq_mode_value(3, 3, 2, noteNames[11], 0, 255);
        }
        break;
        case 3:
        {
            set_seq_mode_value(3, 0, 3, "Vol -", 0, MIDI_CC_RANGE);
            set_seq_mode_value(3, 1, 3, "Vol +", 0, MIDI_CC_RANGE);
            set_seq_mode_value(3, 2, 3, "StepFX -", 0, MIDI_CC_RANGE + 1);
            set_seq_mode_value(3, 3, 3, "StepFX +", 0, MIDI_CC_RANGE + 1);
        }
        break;
        default:
            break;
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
        set_presetNr();
}
void Track::draw_seq_mode3()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, seqMod_value[3][PMpresetNr][0], noteNames[0]);
        drawPot(1, 0, seqMod_value[3][PMpresetNr][1], noteNames[1]);
        drawPot(2, 0, seqMod_value[3][PMpresetNr][2], noteNames[2]);
        drawPot(3, 0, seqMod_value[3][PMpresetNr][3], noteNames[3]);

        drawPot(0, 1, seqMod_value[3][PMpresetNr][4], noteNames[4]);
        drawPot(1, 1, seqMod_value[3][PMpresetNr][5], noteNames[5]);
        drawPot(2, 1, seqMod_value[3][PMpresetNr][6], noteNames[6]);
        drawPot(3, 1, seqMod_value[3][PMpresetNr][7], noteNames[7]);

        drawPot(0, 2, seqMod_value[3][PMpresetNr][8], noteNames[8]);
        drawPot(1, 2, seqMod_value[3][PMpresetNr][9], noteNames[9]);
        drawPot(2, 2, seqMod_value[3][PMpresetNr][10], noteNames[10]);
        drawPot(3, 2, seqMod_value[3][PMpresetNr][11], noteNames[11]);

        drawPot(0, 3, seqMod_value[3][PMpresetNr][12], "Vol -");
        drawPot(1, 3, seqMod_value[3][PMpresetNr][13], "Vol +");
        drawPot(2, 3, seqMod_value[3][PMpresetNr][14], "StepFX -");
        drawPot(3, 3, seqMod_value[3][PMpresetNr][15], "StepFX +");
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 11, 4, 4, NO_VALUE, "Prset", ILI9341_BLUE, 2, false, false);
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 12, 4, 4, PMpresetNr, NO_NAME, ILI9341_BLUE, 2, true, false);
    }
}
// 16 step pot sequencer
void Track::play_seq_mode4(uint8_t cloock)
{
    const uint8_t clipIndex = clip_to_play[internal_clock_bar];
    uint8_t playPresetNr = play_presetNr_Playmode_ccChannel[external_clock_bar];

    uint8_t seq3_clock = cloock / TICKS_PER_STEP;
    bool seq4_bool = (cloock + 1) % 6;

    if (seq3_clock == 16)
        seq3_clock = 0;

    if (seq4_bool)
    {
        if (!note_is_on[0] && random(126) < barProbabilty[external_clock_bar])
        {
            noteToPlay[0] = seqMod_value[4][playPresetNr][seq3_clock] + noteOffset[external_clock_bar] + performNoteOffset;
            uint8_t Velo = 99 * (barVelocity[external_clock_bar] / 127) * (mixGainPot / 127.00);
            uint8_t StepFX = 128;
            note_is_on[0] = true;
            noteOffAt[0] = constrain(cloock + parameter[SET_STEP_LENGTH], 0, MAX_TICKS - 1);
            sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut);
            noteOn(noteToPlay[0], Velo, clip[clipIndex].midiChOut); // Send a Note (pitch 42, velo 127 on channel 1)
                                                                    // Serial.printf("ON   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
            Serial.printf("pl4 Note: %d, Velo: %d, StartTick: %d, EndTick: %d, StepFX: %d\n", noteToPlay[0], Velo, cloock, noteOffAt[0], StepFX);
        }
    }

    else if (!seq4_bool)
    {
        if (noteOffAt[0] == cloock)
        {
            if (note_is_on[0])
            {
                note_is_on[0] = false;
                noteOff(noteToPlay[0], 0, clip[clipIndex].midiChOut); // Send a Note (pitch 42, velo 127 on channel 1)
                // Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
                noteToPlay[0] = NO_NOTE;
            }
        }
    }
}
void Track::set_seq_mode4_parameters()
{
    draw_seq_mode4();
    if (lastPotRow == 3 && enc_button[3]) // save to clip
    {
        clear_active_clip();
        enc_button[3] = false;

        for (int s = 0; s < NUM_STEPS; s++)
        {
            int _note = (seqMod_value[4][PMpresetNr][s]);
            set_note_on_tick(s * TICKS_PER_STEP, _note, parameter[SET_STEP_LENGTH]);
        }
    }
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        switch (lastPotRow)
        {
        case 0:
        {
            set_seq_mode_value(4, 0, 0, "1", 0, NO_NOTE);
            set_seq_mode_value(4, 1, 0, "2", 0, NO_NOTE);
            set_seq_mode_value(4, 2, 0, "3", 0, NO_NOTE);
            set_seq_mode_value(4, 3, 0, "4", 0, NO_NOTE);
        }
        break;
        case 1:
        {
            set_seq_mode_value(4, 0, 1, "5", 0, NO_NOTE);
            set_seq_mode_value(4, 1, 1, "6", 0, NO_NOTE);
            set_seq_mode_value(4, 2, 1, "7", 0, NO_NOTE);
            set_seq_mode_value(4, 3, 1, "8", 0, NO_NOTE);
        }
        break;
        case 2:
        {
            set_seq_mode_value(4, 0, 2, "9", 0, NO_NOTE);
            set_seq_mode_value(4, 1, 2, "10", 0, NO_NOTE);
            set_seq_mode_value(4, 2, 2, "11", 0, NO_NOTE);
            set_seq_mode_value(4, 3, 2, "12", 0, NO_NOTE);
        }
        break;
        case 3:
        {
            set_seq_mode_value(4, 0, 3, "13", 0, NO_NOTE);
            set_seq_mode_value(4, 1, 3, "14", 0, NO_NOTE);
            set_seq_mode_value(4, 2, 3, "15", 0, NO_NOTE);
            set_seq_mode_value(4, 3, 3, "16", 0, NO_NOTE);
        }
        break;
        default:
            break;
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
        set_presetNr();
}
void Track::draw_seq_mode4()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, seqMod_value[4][PMpresetNr][0], "1");
        drawPot(1, 0, seqMod_value[4][PMpresetNr][1], "2");
        drawPot(2, 0, seqMod_value[4][PMpresetNr][2], "3");
        drawPot(3, 0, seqMod_value[4][PMpresetNr][3], "4");

        drawPot(0, 1, seqMod_value[4][PMpresetNr][4], "5");
        drawPot(1, 1, seqMod_value[4][PMpresetNr][5], "6");
        drawPot(2, 1, seqMod_value[4][PMpresetNr][6], "7");
        drawPot(3, 1, seqMod_value[4][PMpresetNr][7], "8");

        drawPot(0, 2, seqMod_value[4][PMpresetNr][8], "9");
        drawPot(1, 2, seqMod_value[4][PMpresetNr][9], "10");
        drawPot(2, 2, seqMod_value[4][PMpresetNr][10], "11");
        drawPot(3, 2, seqMod_value[4][PMpresetNr][11], "12");

        drawPot(0, 3, seqMod_value[4][PMpresetNr][12], "13");
        drawPot(1, 3, seqMod_value[4][PMpresetNr][13], "14");
        drawPot(2, 3, seqMod_value[4][PMpresetNr][14], "15");
        drawPot(3, 3, seqMod_value[4][PMpresetNr][15], "16");
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 11, 4, 4, NO_VALUE, "Prset", ILI9341_BLUE, 2, false, false);
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 12, 4, 4, PMpresetNr, NO_NAME, ILI9341_BLUE, 2, true, false);
    }
}

// beatArray
void Track::play_seq_mode5(uint8_t cloock)
{
    const uint8_t clipIndex = clip_to_play[internal_clock_bar];
    uint8_t playPresetNr = play_presetNr_Playmode_ccChannel[external_clock_bar];

    uint8_t seq3_clock = cloock / TICKS_PER_STEP;
    bool seq4_bool = cloock % TICKS_PER_STEP;
    if (seq3_clock == 16)
        seq3_clock = 0;

    if (seq4_bool)
    {
        for (int v = 0; v < MAX_VOICES; v++)
        {
            if (beatArray[seqMod_value[5][playPresetNr][v]][seq3_clock] && random(126) < barProbabilty[external_clock_bar])
            {

                if (!note_is_on[v])
                {
                    noteToPlay[v] = (v + (parameter[SET_OCTAVE] * NOTES_PER_OCTAVE)) + noteOffset[external_clock_bar] + performNoteOffset;
                    uint8_t Velo = random(seqMod_value[5][playPresetNr][12], seqMod_value[5][playPresetNr][13]) * (barVelocity[external_clock_bar] / 127.00) * (mixGainPot / 127.00);
                    // uint8_t Velo = 99 * (barVelocity[external_clock_bar] / 127) * (mixGainPot / 127.00);
                    uint8_t StepFX = random(seqMod_value[5][playPresetNr][14], seqMod_value[5][playPresetNr][15]);
                    note_is_on[v] = true;
                    sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut);
                    noteOn(noteToPlay[v], Velo, clip[clipIndex].midiChOut); // Send a Note (pitch 42, velo 127 on channel 1)
                    noteOffAt[v] = constrain(cloock + parameter[SET_STEP_LENGTH], 0, MAX_TICKS - 1);
                    Serial.printf("pl5 Note: %d, Velo: %d, StartTick: %d, EndTick: %d, StepFX: %d\n", noteToPlay[v], Velo, cloock, noteOffAt[v], StepFX);

                    // Serial.printf("ON   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
                    // clip[parameter[SET_CLIP2_EDIT]].tick[seq3_clock * TICKS_PER_STEP].voice[v] = noteToPlay[v];
                }
            }
        }
    }

    if (!seq4_bool)
    {
        for (int i = 0; i < MAX_VOICES; i++)
        {
            if (noteOffAt[i] == cloock)
            {
                if (note_is_on[i])
                {
                    note_is_on[i] = false;
                    noteOff(noteToPlay[i], 0, clip[clipIndex].midiChOut); // Send a Note (pitch 42, velo 127 on channel 1)
                    // Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
                    noteToPlay[i] = NO_NOTE;
                }
            }
        }
    }
}
void Track::set_seq_mode5_parameters()
{
    draw_seq_mode5();
    if (lastPotRow == 3 && enc_button[3]) // save to clip
    {
        enc_button[3] = false;
        clear_active_clip();
        for (int v = 0; v < MAX_VOICES; v++)
        {
            for (int s = 0; s < NUM_STEPS; s++)
            {
                if (beatArray[seqMod_value[5][PMpresetNr][v]][s])
                {
                    int _note = v + (parameter[SET_OCTAVE] * NOTES_PER_OCTAVE);
                    set_note_on_tick(s * TICKS_PER_STEP, _note, parameter[SET_STEP_LENGTH]);
                    Serial.printf("pl5 copy to clip note: %d, step: %d, \n", _note, s);
                }
            }
        }
    }
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        switch (lastPotRow)
        {
        case 0:
        {
            set_seq_mode_value(5, 0, 0, "1", 0, MIDI_CC_RANGE);
            set_seq_mode_value(5, 1, 0, "2", 0, MIDI_CC_RANGE);
            set_seq_mode_value(5, 2, 0, "3", 0, MIDI_CC_RANGE);
            set_seq_mode_value(5, 3, 0, "4", 0, MIDI_CC_RANGE);
        }
        break;
        case 1:
        {
            set_seq_mode_value(5, 0, 1, "5", 0, MIDI_CC_RANGE);
            set_seq_mode_value(5, 1, 1, "6", 0, MIDI_CC_RANGE);
            set_seq_mode_value(5, 2, 1, "7", 0, MIDI_CC_RANGE);
            set_seq_mode_value(5, 3, 1, "8", 0, MIDI_CC_RANGE);
        }
        break;
        case 2:
        {
            set_seq_mode_value(5, 0, 2, "9", 0, MIDI_CC_RANGE);
            set_seq_mode_value(5, 1, 2, "10", 0, MIDI_CC_RANGE);
            set_seq_mode_value(5, 2, 2, "11", 0, MIDI_CC_RANGE);
            set_seq_mode_value(5, 3, 2, "12", 0, MIDI_CC_RANGE);
        }
        break;
        case 3:
        {
            set_seq_mode_value(5, 0, 3, "Vol -", 0, MIDI_CC_RANGE);
            set_seq_mode_value(5, 1, 3, "Vol +", 0, MIDI_CC_RANGE);
            set_seq_mode_value(5, 2, 3, "StepFX -", 0, MIDI_CC_RANGE + 1);
            set_seq_mode_value(5, 3, 3, "StepFX +", 0, MIDI_CC_RANGE + 1);
        }
        break;
        default:
            break;
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
        set_presetNr();
}
void Track::draw_seq_mode5()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, seqMod_value[5][PMpresetNr][0], "1");
        drawPot(1, 0, seqMod_value[5][PMpresetNr][1], "2");
        drawPot(2, 0, seqMod_value[5][PMpresetNr][2], "3");
        drawPot(3, 0, seqMod_value[5][PMpresetNr][3], "4");

        drawPot(0, 1, seqMod_value[5][PMpresetNr][4], "5");
        drawPot(1, 1, seqMod_value[5][PMpresetNr][5], "6");
        drawPot(2, 1, seqMod_value[5][PMpresetNr][6], "7");
        drawPot(3, 1, seqMod_value[5][PMpresetNr][7], "8");

        drawPot(0, 2, seqMod_value[5][PMpresetNr][8], "9");
        drawPot(1, 2, seqMod_value[5][PMpresetNr][9], "10");
        drawPot(2, 2, seqMod_value[5][PMpresetNr][10], "11");
        drawPot(3, 2, seqMod_value[5][PMpresetNr][11], "12");

        drawPot(0, 3, seqMod_value[5][PMpresetNr][12], "Vol -");
        drawPot(1, 3, seqMod_value[5][PMpresetNr][13], "Vol +");
        drawPot(2, 3, seqMod_value[5][PMpresetNr][14], "StepFX -");
        drawPot(3, 3, seqMod_value[5][PMpresetNr][15], "StepFX +");
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 11, 4, 4, NO_VALUE, "Prset", ILI9341_BLUE, 1.75, false, false);
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 12, 4, 4, PMpresetNr, NO_NAME, ILI9341_BLUE, 1.75, true, false);
        //  drawPot(0, 3, seqMod_value[4][12], "13");
        //   drawPot(1, 3, seqMod_value[4][13], "14");
        //   drawPot(2, 3, seqMod_value[4][14], "15");
        //   drawPot(3, 3, seqMod_value[4][15], "16");
    }
}

// Euclid
void Track::play_seq_mode6(uint8_t cloock)
{
    const uint8_t clipIndex = clip_to_play[internal_clock_bar];
    uint8_t playPresetNr = play_presetNr_Playmode_ccChannel[external_clock_bar];

    uint8_t seq3_clock = cloock / TICKS_PER_STEP;
    bool seq4_bool = cloock % 6;
    if (seq3_clock == 16)
        seq3_clock = 0;

    if (seq4_bool)
    {
        for (int v = 0; v < MAX_VOICES; v++)
        {
            if (beatArrayPM6[v][seq3_clock] && random(126) < barProbabilty[external_clock_bar])
            {

                if (!note_is_on[v])
                {
                    noteToPlay[v] = (v + (parameter[SET_OCTAVE] * NOTES_PER_OCTAVE)) + noteOffset[external_clock_bar] + performNoteOffset;
                    uint8_t Velo = random(seqMod_value[6][playPresetNr][12], seqMod_value[6][playPresetNr][13]) * (barVelocity[external_clock_bar] / 127.00) * (mixGainPot / 127.00);
                    uint8_t StepFX = random(seqMod_value[6][playPresetNr][14], seqMod_value[6][playPresetNr][15]);
                    note_is_on[v] = true;
                    sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut);
                    noteOn(noteToPlay[v], Velo, clip[clipIndex].midiChOut); // Send a Note (pitch 42, velo 127 on channel 1)
                    noteOffAt[v] = constrain(cloock + parameter[SET_STEP_LENGTH], 0, MAX_TICKS - 1);
                    Serial.printf("pl6 Note: %d, Velo: %d, StartTick: %d, EndTick: %d, StepFX: %d\n", noteToPlay[v], Velo, cloock, noteOffAt[v], StepFX);

                    // Serial.printf("ON   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
                }
            }
        }
    }

    if (!seq4_bool)
    {
        for (int i = 0; i < MAX_VOICES; i++)
        {
            if (noteOffAt[i] == cloock)
            {
                if (note_is_on[i])
                {
                    note_is_on[i] = false;
                    noteOff(noteToPlay[i], 0, clip[clipIndex].midiChOut); // Send a Note (pitch 42, velo 127 on channel 1)
                    // Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
                    noteToPlay[i] = NO_NOTE;
                }
            }
        }
    }
}
void Track::set_seq_mode6_parameters()
{
    draw_seq_mode6();
    if (lastPotRow == 3 && enc_button[3]) // save to clip
    {
        enc_button[3] = false;
        for (int v = 0; v < MAX_VOICES; v++)
        {
            clear_active_clip();
            for (int s = 0; s < NUM_STEPS; s++)
            {
                if (beatArrayPM6[v][s])
                {
                    int _note = v + (parameter[SET_OCTAVE] * NOTES_PER_OCTAVE);
                    set_note_on_tick(s * TICKS_PER_STEP, _note, parameter[SET_STEP_LENGTH]);
                }
            }
        }
    }
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        switch (lastPotRow)
        {
        case 0:
        {
            set_seq_mode6_value(0, 0, "1");
            set_seq_mode6_value(1, 0, "2");
            set_seq_mode6_value(2, 0, "3");
            set_seq_mode6_value(3, 0, "4");
        }
        break;
        case 1:
        {
            set_seq_mode6_value(0, 1, "5");
            set_seq_mode6_value(1, 1, "6");
            set_seq_mode6_value(2, 1, "7");
            set_seq_mode6_value(3, 1, "8");
        }
        break;
        case 2:
        {
            set_seq_mode6_value(0, 2, "9");
            set_seq_mode6_value(1, 2, "10");
            set_seq_mode6_value(2, 2, "11");
            set_seq_mode6_value(3, 2, "12");
        }
        break;
        case 3:
        {
            set_seq_mode6_value(0, 3, "Vol -");
            set_seq_mode6_value(1, 3, "Vol  +");
            set_seq_mode6_value(2, 3, "StepFX -");
            set_seq_mode6_value(3, 3, "StepFX +");
        }
        break;
        default:
            break;
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
        set_presetNr();
}
void Track::set_seq_mode6_value(uint8_t XPos, uint8_t YPos, const char *name)
{
    int n = XPos + (YPos * NUM_ENCODERS);
    if (enc_moved[XPos] && !neotrellisPressed[TRELLIS_BUTTON_ENTER])
    {

        seqMod_value[6][PMpresetNr][n] = constrain(seqMod_value[6][PMpresetNr][n] + encoded[XPos], 0, MIDI_CC_RANGE + 1);
        drawPot(XPos, YPos, seqMod_value[6][PMpresetNr][n], name);
        if (lastPotRow != 3)
        {
            int bucket = 0;
            int _numSteps = map(seqMod_value[6][PMpresetNr][n], 0, 128, 0, NUM_STEPS);
            for (int i = 0; i < NUM_STEPS; i++)
            {
                bucket += _numSteps;
                if (bucket >= NUM_STEPS)
                {
                    beatArrayPM6[n][i] = 1; // Beat
                    bucket -= NUM_STEPS;
                }
                else
                {
                    beatArrayPM6[n][i] = 0; // Kein Beat
                }
            }
        }
    }

    if (enc_moved[XPos] && neotrellisPressed[TRELLIS_BUTTON_ENTER])
    {
        SeqMod6Value2[n] = constrain(SeqMod6Value2[n] + encoded[XPos], 0, MIDI_CC_RANGE + 1);
        drawPot(XPos, YPos, SeqMod6Value2[n], "offset");
        //  int _numSteps = map(seqMod_value[6][n], 0, 127, 0, NUM_STEPS);
        int _offset[MAX_VOICES];
        _offset[n] = SeqMod6Value2[n];
        int temp[NUM_STEPS];

        // Kopiere die Sequenz mit Offset in ein temporäres Array
        for (int i = 0; i < NUM_STEPS; i++)
        {
            temp[(i + _offset[n]) % NUM_STEPS] = beatArrayPM6[n][i];
        }

        // Kopiere das temporäre Array zurück in das ursprüngliche Sequenz-Array
        for (int i = 0; i < NUM_STEPS; i++)
        {
            beatArrayPM6[n][i] = temp[i];
        }
    }
}
void Track::draw_seq_mode6()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, seqMod_value[6][PMpresetNr][0], "1");
        drawPot(1, 0, seqMod_value[6][PMpresetNr][1], "2");
        drawPot(2, 0, seqMod_value[6][PMpresetNr][2], "3");
        drawPot(3, 0, seqMod_value[6][PMpresetNr][3], "4");

        drawPot(0, 1, seqMod_value[6][PMpresetNr][4], "5");
        drawPot(1, 1, seqMod_value[6][PMpresetNr][5], "6");
        drawPot(2, 1, seqMod_value[6][PMpresetNr][6], "7");
        drawPot(3, 1, seqMod_value[6][PMpresetNr][7], "8");

        drawPot(0, 2, seqMod_value[6][PMpresetNr][8], "9");
        drawPot(1, 2, seqMod_value[6][PMpresetNr][9], "10");
        drawPot(2, 2, seqMod_value[6][PMpresetNr][10], "11");
        drawPot(3, 2, seqMod_value[6][PMpresetNr][11], "12");

        drawPot(0, 3, seqMod_value[6][PMpresetNr][12], "Vol -");
        drawPot(1, 3, seqMod_value[6][PMpresetNr][13], "Vol +");
        drawPot(2, 3, seqMod_value[6][PMpresetNr][14], "StepFX -");
        drawPot(3, 3, seqMod_value[6][PMpresetNr][15], "StepFX +");
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 11, 4, 4, NO_VALUE, "Prset", ILI9341_BLUE, 1.75, false, false);
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 12, 4, 4, PMpresetNr, NO_NAME, ILI9341_BLUE, 1.75, true, false);
    }
}

// Rclid
void Track::play_seq_mode7(uint8_t cloock)
{
    const uint8_t clipIndex = clip_to_play[internal_clock_bar];
    uint8_t playPresetNr = play_presetNr_Playmode_ccChannel[external_clock_bar];
    uint8_t seq3_clock = cloock / TICKS_PER_STEP;
    bool seq4_bool = cloock % 6;

    if (lastPotRow == 0) // lock sequence immedietly
    {
        if (enc_button[3])
        {
            enc_button[3] = false;
            seqMod_value[7][playPresetNr][2] = 127;
            change_plugin_row = true;
        }
        if (enc_moved[3])
            rotateIntArray(seqModNoteMemory, seqMod_value[7][playPresetNr][0], encoded[3]);
    }

    if (seq4_bool)
    {

        if (beatArrayPM7[seq3_clock])
        {

            if (!note_is_on[0] && random(126) < barProbabilty[external_clock_bar])
            {
                static int stepCount;                                                // max Steps
                stepCount = (stepCount + 1) % seqMod_value[7][playPresetNr][0];      // max Steps
                bool useMemory = ((random(126)) < seqMod_value[7][playPresetNr][2]); // dejavu
                int noteToSend = useMemory ? seqModNoteMemory[stepCount] : get_random_Note_in_scale();
                int note2save = noteToSend + (random(seqMod_value[7][playPresetNr][4], seqMod_value[7][playPresetNr][5] + 1) * 12) + noteOffset[external_clock_bar] + performNoteOffset;

                noteToPlay[0] = seqModNoteMemory[stepCount];
                // noteToPlay[0] = get_random_Note_in_scale() + (random(seqMod_value[7][4], seqMod_value[7][5]) * NOTES_PER_OCTAVE) + noteOffset[external_clock_bar] + performNoteOffset;
                uint8_t Velo = random(seqMod_value[7][playPresetNr][6], seqMod_value[7][playPresetNr][7]) * (barVelocity[external_clock_bar] / 127) * (mixGainPot / 127.00);
                uint8_t StepFX = random(seqMod_value[7][playPresetNr][8], seqMod_value[7][playPresetNr][9]) * (barVelocity[external_clock_bar] / 127) * (mixGainPot / 127.00);
                note_is_on[0] = true;
                note_is_on[0] = true;
                seqModNoteMemory[seq3_clock] = noteToPlay[0];
                sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut);
                noteOn(noteToPlay[0], Velo, clip[clipIndex].midiChOut); // Send a Note (pitch 42, velo 127 on channel 1)

                noteOffAt[0] = constrain(cloock + parameter[SET_STEP_LENGTH], 0, MAX_TICKS - 1);
                Serial.printf("pl5 Note: %d, Velo: %d, StartTick: %d, EndTick: %d, StepFX: %d\n", noteToPlay[0], Velo, cloock, noteOffAt[0], StepFX);
                if (!useMemory)
                {
                    seqModNoteMemory[stepCount] = note2save;
                    // Serial.printf("PM1 = save note: %d on step: %d\n", seqModNoteMemory[stepCount], stepCount);
                    //  velocityMemory[stepCount] = volSend;
                }
            }
        }
    }

    if (!seq4_bool)
    {
        if (noteOffAt[0] == cloock)
        {
            if (note_is_on[0])
            {
                note_is_on[0] = false;
                noteOff(noteToPlay[0], 0, clip[clipIndex].midiChOut); // Send a Note (pitch 42, velo 127 on channel 1)
                // Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
                noteToPlay[0] = NO_NOTE;
            }
        }
    }
}
void Track::set_seq_mode7_parameters()
{
    draw_seq_mode7();
    if (lastPotRow == 3 && enc_button[3]) // save to clip
    {
        clear_active_clip();
        enc_button[3] = false;

        for (int s = 0; s < NUM_STEPS; s++)
        {
            int _note = seqModNoteMemory[s];
            set_note_on_tick(s * TICKS_PER_STEP, _note, parameter[SET_STEP_LENGTH]);
        }
    }
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        switch (lastPotRow)
        {
        case 0:
        {
            set_seq_mode7_value(0, 0, "Steps");
            set_seq_mode7_value(1, 0, "Offset");
            set_seq_mode7_value(2, 0, "DejaVu");
            set_seq_mode7_value(3, 0, "rotate");
        }
        break;
        case 1:
        {
            set_seq_mode7_value(0, 1, "Oct -");
            set_seq_mode7_value(1, 1, "Oct +");
            set_seq_mode7_value(2, 1, "Velo -");
            set_seq_mode7_value(3, 1, "Velo +");
        }
        break;
        case 2:
        {
            set_seq_mode7_value(0, 2, "FX -");
            set_seq_mode7_value(1, 2, "FX +");
        }
        break;
        case 3:
        {
        }
        break;
        default:
            break;
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
        set_presetNr();
}
void Track::set_seq_mode7_value(uint8_t XPos, uint8_t YPos, const char *name)
{
    int n = XPos + (YPos * NUM_ENCODERS);
    if (enc_moved[XPos])
    {

        seqMod_value[7][PMpresetNr][n] = constrain(seqMod_value[7][PMpresetNr][n] + encoded[XPos], 0, MIDI_CC_RANGE);
        drawPot(XPos, YPos, seqMod_value[7][PMpresetNr][n], name);

        if (n == 0)
        {
            int bucket = 0;
            int _numSteps = map(seqMod_value[7][PMpresetNr][0], 0, 127, 0, NUM_STEPS);
            for (int i = 0; i < NUM_STEPS; i++)
            {
                bucket += _numSteps;
                if (bucket >= NUM_STEPS)
                {
                    beatArrayPM7[i] = 1; // Beat
                    bucket -= NUM_STEPS;
                }
                else
                {
                    beatArrayPM7[i] = 0; // Kein Beat
                }
            }
        }

        if (n == 1)
        {
            // int _numSteps = map(seqMod_value[7][0], 0, 127, 0, NUM_STEPS);
            int _offset;
            _offset = seqMod_value[7][PMpresetNr][1];
            int temp[NUM_STEPS];

            // Kopiere die Sequenz mit Offset in ein temporäres Array
            for (int i = 0; i < NUM_STEPS; i++)
            {
                temp[(i + _offset) % NUM_STEPS] = beatArrayPM7[i];
            }

            // Kopiere das temporäre Array zurück in das ursprüngliche Sequenz-Array
            for (int i = 0; i < NUM_STEPS; i++)
            {
                beatArrayPM7[i] = temp[i];
            }
        }
    }
}
void Track::draw_seq_mode7()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, seqMod_value[7][PMpresetNr][0], "Steps");
        drawPot(1, 0, seqMod_value[7][PMpresetNr][1], "Offset");
        drawPot(2, 0, seqMod_value[7][PMpresetNr][2], "DejaVu");
        drawPot(3, 0, seqMod_value[7][PMpresetNr][3], "rotate");

        drawPot(0, 1, seqMod_value[7][PMpresetNr][4], "Oct -");
        drawPot(1, 1, seqMod_value[7][PMpresetNr][5], "Oct +");
        drawPot(2, 1, seqMod_value[7][PMpresetNr][6], "Velo -");
        drawPot(3, 1, seqMod_value[7][PMpresetNr][7], "Velo +");

        drawPot(0, 2, seqMod_value[7][PMpresetNr][8], "FX -");
        drawPot(1, 2, seqMod_value[7][PMpresetNr][9], "FX +");
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 11, 4, 4, NO_VALUE, "Prset", ILI9341_BLUE, 1.75, false, false);
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 12, 4, 4, PMpresetNr, NO_NAME, ILI9341_BLUE, 1.75, true, false);
        // drawPot(0, 2, seqMod_value[7][8], "Velo -");
        //  drawPot(1, 2, seqMod_value[7][9], "Velo +");
        //  drawPot(2, 2, seqMod_value[7][10], "FX -");
        //  drawPot(3, 2, seqMod_value[7][11], "FX +");

        //  drawPot(0, 3, seqMod_value[4][12], "13");
        //   drawPot(1, 3, seqMod_value[4][13], "14");
        //   drawPot(2, 3, seqMod_value[4][14], "15");
        //   drawPot(3, 3, seqMod_value[4][15], "16");
    }
}

// MIDI
void Track::play_seq_mode8(uint8_t cloock)
{
    const uint8_t clipIndex = clip_to_play[internal_clock_bar];
    uint8_t playPresetNr = play_presetNr_Playmode_ccChannel[external_clock_bar];

    unsigned long songTick = cloock + ((((internal_clock_bar) % seqMod_value[8][playPresetNr][5]) + seqMod_value[8][playPresetNr][4]) * MAX_TICKS);
    unsigned long readTick = songTick * seqMod_value[8][playPresetNr][3];
    // unsigned long songTick = cloock + (internal_clock_bar * MAX_TICKS);
    for (int v = 0; v < MAX_VOICES; v++)
    {
        if (noteOffAt[v] + 1 == songTick)
        {
            noteOffAt[v] = -2;
            noteOff(noteToPlay[v], 0, clip[clipIndex].midiChOut); // Send a Note (pitch 42, velo 127 on channel 1)
            // Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
            // Serial.printf("PL8 NOteOFF: %d,  Start: %d, Len: %d, voice: %d, tick %d\n", noteToPlay[v], noteInfo[v][2], noteInfo[v][3], v, songTick);
            noteToPlay[v] = NO_NOTE;
        }
    }
    int numVoices = fillNoteInfoAtTick(myMidi[my_Arranger_Y_axis - 1], readTick);

    for (int v = 0; v < numVoices; v++)
    {
        // Serial.printf("In file note: %d, velo: %d  Start: %d, Len: %d, readTick: %d, songTick: %d\n", noteInfo[v][0], noteInfo[v][1], noteInfo[v][2] / seqMod_value[8][playPresetNr][3], noteInfo[v][3] / seqMod_value[8][playPresetNr][3], readTick / seqMod_value[8][playPresetNr][3], songTick);
        unsigned long startTick = noteInfo[v][2] / seqMod_value[8][playPresetNr][3];

        if (startTick == songTick && random(126) < barProbabilty[external_clock_bar])
        {
            int length = noteInfo[v][3] / seqMod_value[8][playPresetNr][3];

            noteToPlay[v] = noteInfo[v][0] + noteOffset[external_clock_bar] + performNoteOffset;
            // noteOffAt[0] = tick.startTick[0] + tick.noteLength[0];
            uint8_t Velo = noteInfo[v][1] * (barVelocity[external_clock_bar] / MIDI_CC_RANGE_FLOAT) * (mixGainPot / MIDI_CC_RANGE_FLOAT);
            uint8_t StepFX = random(seqMod_value[8][playPresetNr][1], seqMod_value[8][playPresetNr][2]);
            sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut);
            noteOn(noteToPlay[v], Velo, clip[clipIndex].midiChOut); // Send a Note
            noteOffAt[v] = startTick + length;
            Serial.printf("pl8 Note: %d, Velo: %d, StartTick: %d, EndTick: %d, StepFX: %d\n", noteToPlay[v], Velo, startTick, noteOffAt[v], StepFX);

            //    Serial.printf("PL8 NOte: %d, Vel: %d, Start: %d, Len: %d, noteOffAt: %d, voice: %d, tick: %d\n", noteToPlay[v], Velo, startTick, length, noteOffAt[v], v, songTick);
        }
    }
}
void Track::select_file(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        int n = XPos + (YPos * NUM_ENCODERS);
        seqMod_value[8][PMpresetNr][n] = constrain(seqMod_value[8][PMpresetNr][n] + encoded[XPos], 0, 127); // mode 3 =MIDI_CC_RANGE * 2 //mode4 =NO_NOTE //others=MIDI_CC_RANGE
        char filename[20];
        sprintf(filename, "%d.mid", seqMod_value[8][PMpresetNr][n]);
        loadMidiFile(filename, myMidi[my_Arranger_Y_axis - 1]);
        seqMod_value[8][PMpresetNr][3] = getPPQN(myMidi[my_Arranger_Y_axis - 1]) / 24;
        Serial.printf("select filename: %s, value: %d\n", filename, seqMod_value[8][PMpresetNr][0]);
        drawPot(XPos, YPos, seqMod_value[8][PMpresetNr][n], name);
    }
}
void Track::refresh_mode8()
{
    if (parameter[SET_SEQ_MODE] != 8)
        return;
    char filename[20];
    sprintf(filename, "%d.mid", seqMod_value[8][play_presetNr_Playmode_ccChannel[external_clock_bar]][0]);
    loadMidiFile(filename, myMidi[my_Arranger_Y_axis - 1]);
    seqMod_value[8][play_presetNr_Playmode_ccChannel[myClock.barTick]][3] = getPPQN(myMidi[my_Arranger_Y_axis - 1]) / 24;
    Serial.printf("play filename: %s, value: %d, PMpresetNr: %d\n", filename, seqMod_value[8][play_presetNr_Playmode_ccChannel[external_clock_bar]][0], play_presetNr_Playmode_ccChannel[external_clock_bar]);
    if (my_Arranger_Y_axis == 1)
    {
        Serial.printf("internal bar:  %d, external bar:  %d\n", internal_clock_bar, external_clock_bar);
        // Serial.printf("uClock bar:  %d, external bar:  %d\n", myClock.barTick, external_clock_bar);
    }
}
void Track::set_seq_mode8_parameters()
{
    draw_seq_mode8();
    if (lastPotRow == 3 && enc_button[3]) // save to clip
    {
        clear_active_clip();
        int multiplier = seqMod_value[8][PMpresetNr][3]; // 40 for 960ppqn to 24ppqn conversion
        enc_button[3] = false;
        for (int s = 0; s < MAX_TICKS; s++)
        {
            int numVoices = fillNoteInfoAtTick(myMidi[my_Arranger_Y_axis - 1], (s + (seqMod_value[8][PMpresetNr][4] * MAX_TICKS)) * multiplier);
            for (int v = 0; v < numVoices; v++)
            {
                int _note = noteInfo[v][0];
                parameter[SET_VELO2SET] = noteInfo[v][1];
                parameter[SET_STEP_FX] = random(seqMod_value[8][PMpresetNr][1], seqMod_value[8][PMpresetNr][2]);
                int start = (noteInfo[v][2] / multiplier) - (seqMod_value[8][PMpresetNr][4] * MAX_TICKS);
                int length = noteInfo[v][3] / multiplier;
                Serial.printf("store PL8 note: %d, start: %d, length: %d, velo: %d\n", _note, start, length, parameter[SET_VELO2SET]);
                if (start == s)
                    set_note_on_tick(start, _note, length);
            }
        }
    }
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        switch (lastPotRow)
        {
        case 0:
        {

            select_file(0, 0, "File");
            set_seq_mode_value(8, 1, 0, "StepFX -", 0, MIDI_CC_RANGE + 1);
            set_seq_mode_value(8, 2, 0, "StepFX +", 0, MIDI_CC_RANGE + 1);
            set_seq_mode_value(8, 3, 0, "CMult", 1, MIDI_CC_RANGE);
        }
        break;
        case 1:
        {
            set_seq_mode_value(8, 0, 1, "StartBar", 0, 255);
            set_seq_mode_value(8, 1, 1, "loopBars", 1, 255);
            // set_seq_mode_value(1, 1, 1, "Dejavu", 0, MIDI_CC_RANGE);
            // set_seq_mode_value(1, 2, 1, "Rotate", 0, 32);
        }
        break;
        case 2:
        {
        }
        break;
        case 3:
        {
        }
        break;
        default:
            break;
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        Serial.println("shift pressed");
        set_presetNr();
    }
}
void Track::draw_seq_mode8()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;

        drawPot(0, 0, seqMod_value[8][PMpresetNr][0], "File");
        drawPot(1, 0, seqMod_value[8][PMpresetNr][1], "StepFX -");
        drawPot(2, 0, seqMod_value[8][PMpresetNr][2], "StepFX +");
        drawPot(3, 0, seqMod_value[8][PMpresetNr][3], "CMult");
        drawPot(0, 1, seqMod_value[8][PMpresetNr][4], "StartBar");
        drawPot(1, 1, seqMod_value[8][PMpresetNr][5], "loopBars");
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 11, 4, 4, NO_VALUE, "Prset", ILI9341_BLUE, 1.75, false, false);
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 12, 4, 4, PMpresetNr, NO_NAME, ILI9341_BLUE, 1.75, true, false);
    }
}
// LFO
void Track::play_seq_mode9(uint8_t cloock)
{
    const uint8_t clipIndex = clip_to_play[internal_clock_bar];
    const auto &tick = clip[clipIndex].tick[cloock];
    uint8_t noteParam = get_note_parameter(tick.voice, 0);
    uint8_t playPresetNr = play_presetNr_Playmode_ccChannel[external_clock_bar];
    float lfo = 0;
    if (get_note_parameter(tick.startTick, 0) == cloock && noteParam < NO_NOTE && random(126) < barProbabilty[external_clock_bar])
    {
        switch (seqMod_value[9][playPresetNr][10])
        {
        case 0:
            lfo = sin((M_PI * cloock + seqMod_value[9][playPresetNr][0]) / 180); // lfo_phase von 0 bis 1 über N Ticks
            break;
        case 1:
            lfo = cos((M_PI * cloock + seqMod_value[9][playPresetNr][0]) / 180); // lfo_phase von 0 bis 1 über N Ticks
            break;
        case 2:
            lfo = tan((M_PI * cloock + seqMod_value[9][playPresetNr][0]) / 180); // lfo_phase von 0 bis 1 über N Ticks
            break;
        case 3:
            lfo = -tan((M_PI * cloock + seqMod_value[9][playPresetNr][0]) / 180); // lfo_phase von 0 bis 1 über N Ticks
            break;
        case 4:
            lfo = lfo_semitone_tri((M_PI * cloock + seqMod_value[9][playPresetNr][0]) / 180); // lfo_phase von 0 bis 1 über N Ticks
            break;
        case 5:
            lfo = -lfo_semitone_tri((M_PI * cloock + seqMod_value[9][playPresetNr][0]) / 180); // lfo_phase von 0 bis 1 über N Ticks
            break;
        case 6:
            lfo = lfo_semitone_saw((M_PI * cloock + seqMod_value[9][playPresetNr][0]) / 180); // lfo_phase von 0 bis 1 über N Ticks
            break;
        case 7:
            lfo = -lfo_semitone_saw((M_PI * cloock + seqMod_value[9][playPresetNr][0]) / 180); // lfo_phase von 0 bis 1 über N Ticks
            break;
        case 8:
            lfo = random(-MIDI_CC_RANGE, MIDI_CC_RANGE) / MIDI_CC_RANGE; // lfo_phase von 0 bis 1 über N Ticks
            break;
        default:
            break;
        }
        Serial.printf("pm9 lfo: %02f, depth: %02f\n", lfo, lfo * seqMod_value[9][playPresetNr][1]);

        noteToPlay[0] = transpose_to_scale(noteParam + (lfo * seqMod_value[9][playPresetNr][1])) + 12 * parameter[SET_OCTAVE] + noteOffset[external_clock_bar] + performNoteOffset;
        noteOffAt[0] = constrain(tick.startTick[0] + tick.noteLength[0], 0, MAX_TICKS - 1);
        uint8_t Velo = random(seqMod_value[9][playPresetNr][2], seqMod_value[9][playPresetNr][3]) * (barVelocity[external_clock_bar] / MIDI_CC_RANGE_FLOAT) * (mixGainPot / MIDI_CC_RANGE_FLOAT);
        uint8_t StepFX = random(seqMod_value[9][playPresetNr][8], seqMod_value[9][playPresetNr][9]);
        sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut);
        noteOn(noteToPlay[0], Velo, clip[clipIndex].midiChOut); // Send a Note
        seqModNoteMemory[cloock / TICKS_PER_STEP] = noteToPlay[0];
        Serial.printf("pl9 Note: %d, Velo: %d, StartTick: %d, EndTick: %d, StepFX: %d\n", noteToPlay[0], Velo, tick.startTick[0], noteOffAt[0], StepFX);
    }

    if (noteOffAt[0] == cloock)
    {
        noteOffAt[0] = -1;
        noteOff(noteToPlay[0], 0, clip[clipIndex].midiChOut); // Send a Note (pitch 42, velo 127 on channel 1)
        // Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
        noteToPlay[0] = NO_NOTE;
    }
}
void Track::set_seq_mode9_parameters()
{
    draw_seq_mode9();
    if (lastPotRow == 3 && enc_button[3]) // save to clip
    {
        clear_active_clip();
        enc_button[3] = false;

        for (int s = 0; s < seqMod_value[9][PMpresetNr][4]; s++)
        {
            int _note = seqModNoteMemory[s];
            set_note_on_tick(s * TICKS_PER_STEP, _note, parameter[SET_STEP_LENGTH]);
        }
    }
    if (lastPotRow == 1) // lock sequence immedietly
    {
        if (enc_button[3])
        {
            seqMod_value[9][PMpresetNr][5] = 127;
            change_plugin_row = true;
        }
        if (enc_moved[2])
            rotateIntArray(seqModNoteMemory, seqMod_value[9][PMpresetNr][4], encoded[2]);
    }
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        switch (lastPotRow)
        {
        case 0:
        {

            set_seq_mode_value(9, 0, 0, "Degree", 0, MIDI_CC_RANGE);
            set_seq_mode_value(9, 1, 0, "Depth", 0, MIDI_CC_RANGE);
            set_seq_mode_value(9, 2, 0, "Vol -", 0, MIDI_CC_RANGE);
            set_seq_mode_value(9, 3, 0, "Vol +", 0, MIDI_CC_RANGE);
        }
        break;
        case 1:
        {
            set_seq_mode_value(9, 0, 1, "maxSteps", 0, NUM_STEPS);
            set_seq_mode_value(9, 1, 1, "Dejavu", 0, MIDI_CC_RANGE);
            set_seq_mode_value(9, 2, 1, "Rotate", 0, 32);
        }
        break;
        case 2:
        {
            set_seq_mode_value(9, 0, 2, "StepFX -", 0, MIDI_CC_RANGE + 1);
            set_seq_mode_value(9, 1, 2, "StepFX +", 0, MIDI_CC_RANGE + 1);
            set_seq_mode_value(9, 2, 2, "Type", 0, 8);
        }
        break;
        case 3:
        {
        }
        break;
        default:
            break;
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
        set_presetNr();
}
void Track::draw_seq_mode9()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;

        drawPot(0, 0, seqMod_value[9][PMpresetNr][0], "Speed");
        drawPot(1, 0, seqMod_value[9][PMpresetNr][1], "Depth");
        drawPot(2, 0, seqMod_value[9][PMpresetNr][2], "Vol -");
        drawPot(3, 0, seqMod_value[9][PMpresetNr][3], "Vol +");

        drawPot(0, 1, seqMod_value[9][PMpresetNr][4], "maxSteps");
        drawPot(1, 1, seqMod_value[9][PMpresetNr][5], "Dejavu");
        drawPot(2, 1, seqMod_value[9][PMpresetNr][6], "Rotate");

        drawPot(0, 2, seqMod_value[9][PMpresetNr][8], "StepFX -");
        drawPot(1, 2, seqMod_value[9][PMpresetNr][9], "StepFX +");
        drawPot(2, 2, seqMod_value[9][PMpresetNr][10], "Type");
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 11, 4, 4, NO_VALUE, "Prset", ILI9341_BLUE, 2, false, false);
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 12, 4, 4, PMpresetNr, NO_NAME, ILI9341_BLUE, 2, true, false);
    }
}

// psy
void Track::play_seq_mode10(uint8_t cloock)
{
    const uint8_t clipIndex = clip_to_play[internal_clock_bar];
    const auto &tick = clip[clipIndex].tick[cloock];
    uint8_t playPresetNr = play_presetNr_Playmode_ccChannel[external_clock_bar];

    // --- Parameter aus seqMod_value[1][playPresetNr][x] ---

    uint8_t rootNote = seqMod_value[10][playPresetNr][1];
    uint8_t accentNote = seqMod_value[10][playPresetNr][4];
    uint8_t accentNote2 = seqMod_value[10][playPresetNr][6];
    uint8_t baseNote;
    bool shouldPlay = false;
    switch (seqMod_value[10][playPresetNr][0] % 4)
    {
    case 0: //  Offbeat xxXx
        if (cloock % 24 == 12)
        {
            shouldPlay = true;
            baseNote = rootNote;
        }
        if (cloock % 96 == seqMod_value[10][PMpresetNr][5] * TICKS_PER_STEP && accentNote != 0)
        {
            shouldPlay = true;
            baseNote = accentNote;
        }
        if (cloock % 96 == seqMod_value[10][PMpresetNr][7] * TICKS_PER_STEP && accentNote2 != 0)
        {
            shouldPlay = true;
            baseNote = accentNote2;
        }
        break;
    case 1: // gallop accent per bar
        if (cloock % 24 == 12)
        {
            shouldPlay = true;
            baseNote = rootNote;
        }
        if (cloock % 24 == 18)
        {
            shouldPlay = true;
            baseNote = rootNote;
        }
        if (cloock % 96 == seqMod_value[10][PMpresetNr][5] * TICKS_PER_STEP && accentNote != 0)
        {
            shouldPlay = true;
            baseNote = accentNote;
        }
        if (cloock % 96 == seqMod_value[10][PMpresetNr][7] * TICKS_PER_STEP && accentNote2 != 0)
        {
            shouldPlay = true;
            baseNote = accentNote2;
        }
        break;
    case 2: // gallop accent par quarter note
        if (cloock % 24 == 12)
        {
            shouldPlay = true;
            baseNote = rootNote;
        }
        if (cloock % 24 == 18 && accentNote != 0)
        {
            shouldPlay = true;
            baseNote = accentNote;
        }
        break;
    case 3: // Trioles XXX
        if (cloock % 8 == 0)
        {
            shouldPlay = true;
            baseNote = rootNote;
        }
        if (cloock % 96 == constrain(seqMod_value[10][PMpresetNr][5] * 8, 0, 88) && accentNote != 0)
        {
            shouldPlay = true;
            baseNote = accentNote;
        }
        if (cloock % 96 == constrain(seqMod_value[10][PMpresetNr][7] * 8, 0, 88) && accentNote2 != 0)
        {
            shouldPlay = true;
            baseNote = accentNote2;
        }
        break;
    }

    if (!note_is_on[0] && shouldPlay && random(126) < barProbabilty[external_clock_bar])
    {
        // Basisnote erzeugen (in Skala)
        uint8_t finalNote = baseNote + noteOffset[external_clock_bar] + performNoteOffset; // eigene Funktion
        uint8_t velocity = random(seqMod_value[10][playPresetNr][2], seqMod_value[10][playPresetNr][3]) * (barVelocity[external_clock_bar] / MIDI_CC_RANGE_FLOAT) * (mixGainPot / MIDI_CC_RANGE_FLOAT);
        uint8_t StepFX = random(seqMod_value[10][playPresetNr][8], seqMod_value[10][playPresetNr][9]);

        sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut);
        noteToPlay[0] = finalNote;
        seqModNoteMemory[cloock / TICKS_PER_STEP] = finalNote;
        noteOffAt[0] = constrain(cloock + parameter[SET_STEP_LENGTH], 0, MAX_TICKS - 1);
        note_is_on[0] = true;
        noteOn(finalNote, velocity, clip[clipIndex].midiChOut);
        Serial.printf("pl10 Note: %d, Velo: %d, StartTick: %d, EndTick: %d, StepFX: %d\n", noteToPlay[0], velocity, cloock, noteOffAt[0], StepFX);
    }

    if (note_is_on[0] && noteOffAt[0] == cloock)
    {
        note_is_on[0] = false;
        noteOffAt[0] = -1;
        noteOff(noteToPlay[0], 0, clip[clipIndex].midiChOut);
        noteToPlay[0] = NO_NOTE;
    }
}
void Track::set_seq_mode10_parameters()
{
    draw_seq_mode10();
    if (lastPotRow == 3 && enc_button[3]) // save to clip
    {
        clear_active_clip();
        enc_button[3] = false;

        for (int s = 0; s < NUM_STEPS; s++)
        {
            int _note = seqModNoteMemory[s];
            set_note_on_tick(s * TICKS_PER_STEP, _note, parameter[SET_STEP_LENGTH]);
        }
    }
    if (lastPotRow == 1) // lock sequence immedietly
    {
        if (enc_button[3])
        {
            seqMod_value[10][PMpresetNr][5] = 127;
            change_plugin_row = true;
        }
        if (enc_moved[2])
            rotateIntArray(seqModNoteMemory, seqMod_value[10][PMpresetNr][4], encoded[2]);
    }
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        switch (lastPotRow)
        {
        case 0:
        {

            set_seq_mode_value(10, 0, 0, "Style", 0, MIDI_CC_RANGE);
            set_seq_mode_value(10, 1, 0, "RootN", 0, MIDI_CC_RANGE);
            set_seq_mode_value(10, 2, 0, "Vol -", 0, MIDI_CC_RANGE);
            set_seq_mode_value(10, 3, 0, "Vol +", 0, MIDI_CC_RANGE);
        }
        break;
        case 1:
        {
            set_seq_mode_value(10, 0, 1, "AccNot1", 0, MIDI_CC_RANGE);
            set_seq_mode_value(10, 1, 1, "AccPos1", 0, NUM_STEPS);
            set_seq_mode_value(10, 2, 1, "AccNot2", 0, MIDI_CC_RANGE);
            set_seq_mode_value(10, 3, 1, "AccPos2", 0, NUM_STEPS);
        }
        break;
        case 2:
        {
            set_seq_mode_value(10, 0, 2, "StepFX -", 0, MIDI_CC_RANGE + 1);
            set_seq_mode_value(10, 1, 2, "StepFX +", 0, MIDI_CC_RANGE + 1);
            // set_seq_mode_value(10, 2, 2, "Style", 0, MIDI_CC_RANGE);
        }
        break;
        case 3:
        {
        }
        break;
        default:
            break;
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
        set_presetNr();
}
void Track::draw_seq_mode10()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;

        drawPot(0, 0, seqMod_value[10][PMpresetNr][0], "Style");
        drawPot(1, 0, seqMod_value[10][PMpresetNr][1], "RootN");
        drawPot(2, 0, seqMod_value[10][PMpresetNr][2], "Vol -");
        drawPot(3, 0, seqMod_value[10][PMpresetNr][3], "Vol +");

        drawPot(0, 1, seqMod_value[10][PMpresetNr][4], "AccNOt1");
        drawPot(1, 1, seqMod_value[10][PMpresetNr][5] + 1, "AccPos1");
        drawPot(2, 1, seqMod_value[10][PMpresetNr][6], "AccNot2");
        drawPot(3, 1, seqMod_value[10][PMpresetNr][7] + 1, "AccPos2");

        drawPot(0, 2, seqMod_value[10][PMpresetNr][8], "StepFX -");
        drawPot(1, 2, seqMod_value[10][PMpresetNr][9], "StepFX +");
        // drawPot(2, 2, seqMod_value[10][PMpresetNr][10], "Style");
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 11, 4, 4, NO_VALUE, "Prset", ILI9341_BLUE, 2, false, false);
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 12, 4, 4, PMpresetNr, NO_NAME, ILI9341_BLUE, 2, true, false);
    }
}
