#include <Arduino.h>
#include "projectVariables.h"
#include "Track.h"
extern MidiTrack myMidi[NUM_TRACKS];
// hello test
void Track::set_seqModValue(uint8_t param, uint8_t value)
{
    uint8_t mode = clip[parameter[SET_CLIP2_EDIT]].playMode;
    seqMod_value[mode][PMpresetNr][param] = value;
    if (activeScreen != INPUT_FUNCTIONS_FOR_SEQUENCER_MODES)
        return;

    uint8_t presetNr = play_presetNr_Playmode_ccChannel[external_clock_bar];

    int x = param % NUM_ENCODERS;
    int y = param / NUM_ENCODERS;
    const SeqModeParam &paramname = seqModeParams[mode][y][x];

    if (strlen(paramname.label) > 0)
    {
        trellisOut.drawPotentiometerValue(x, value);
        drawPot(x, y, seqMod_value[mode][PMpresetNr][x + y * 4], paramname.label);
        Serial.printf("draw Sepmod pot: %d value: %d\n", x + y * 4, seqMod_value[mode][PMpresetNr][x + y * 4]);
    }
}
uint8_t Track::get_seqModValue(uint8_t param)
{
    uint8_t mode = clip[parameter[SET_CLIP2_EDIT]].playMode;
    uint8_t presetNr = play_presetNr_Playmode_ccChannel[external_clock_bar];
    return seqMod_value[mode][presetNr][param];
}
void Track::set_presetNr()
{
    if (inputs.enc_moved[PRESET_ENCODER])
    {
        PMpresetNr = constrain(PMpresetNr + inputs.encoded[PRESET_ENCODER], 0, NUM_PLUGIN_PRESETS - 1);
        change_plugin_row = true;
        Serial.println("change playmode preset");
        draw_sequencer_modes(parameter[SET_SEQ_MODE]);
    }
}
void Track::set_seq_mode_value(uint8_t modeindex, uint8_t XPos, uint8_t YPos, const char *name, int min, int max)
{
    if (inputs.active[XPos])
    {
        int n = XPos + (YPos * NUM_ENCODERS);
        set_seqModValue(n, inputs.getValueFromInput(XPos, seqMod_value[modeindex][PMpresetNr][n], max)); // mode 3 =MIDI_CC_RANGE * 2 //mode4 =NO_NOTE //others=MIDI_CC_RANGE
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

// Generische Parameter-Funktion
void Track::set_seqmode_parameters(uint8_t mode)
{
    draw_seq_mode(mode);

    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        for (int x = 0; x < 4; x++)
        {
            const SeqModeParam &param = seqModeParams[mode][lastPotRow][x];
            if (strlen(param.label) > 0)
                set_seq_mode_value(mode, x, lastPotRow, param.label, param.min, param.max);
        }
    }
    else
    {
        set_presetNr();
    }
    if (mode == 1 || mode == 7) // lock sequence immedietly
    {
        if (lastPotRow == 1)
        {
            if (inputs.enc_button[3])
            {
                inputs.enc_button[3] = false;
                seqMod_value[1][play_presetNr_Playmode_ccChannel[external_clock_bar]][7] = 127;
                change_plugin_row = true;
            }
            if (inputs.enc_moved[2])
                rotateIntArray(seqModNoteMemory[0], seqMod_value[mode][play_presetNr_Playmode_ccChannel[external_clock_bar]][4], inputs.encoded[2]);
        }
    }
    // Set values for euclidian playmodes
    for (int x = 0; x < NUM_ENCODERS; x++)
    {
        for (int y = 0; y < NUM_ENCODERS; y++)
        {
            if (mode == 6)
            {
                set_seq_mode6_value(x, y);
            }
            if (mode == 7)
            {
                set_seq_mode7_value(x, y);
            }
        }
    }
    // playmode 8 select files
    if (mode == 8)
        select_file(0, 0);
    // copy playmode to clip
    if (lastPotRow == 3 && inputs.enc_button[3])
    {
        clear_active_clip();
        inputs.enc_button[3] = false;
        // Speichern in Clip, je nach Modus ggf. anpassen!
        for (int v = 0; v < MAX_VOICES; v++)
        {
            for (int s = 0; s < NUM_STEPS; s++)
            {
                int _note = seqModNoteMemory[v][s];
                set_note_on_tick(s * TICKS_PER_STEP, _note, parameter[SET_STEP_LENGTH]);
            }
        }
    }
}
// Generische Draw-Funktion
void Track::draw_seq_mode(uint8_t mode)
{
    if (!change_plugin_row)
        return;
    change_plugin_row = false;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            const SeqModeParam &param = seqModeParams[mode][y][x];
            if (strlen(param.label) > 0)
                drawPot(x, y, seqMod_value[mode][PMpresetNr][x + y * 4], param.label);
                Serial.printf("draw Sepmod whole pot: %d value: %d\n", x + y * 4, seqMod_value[mode][PMpresetNr][x + y * 4]);
        }
    }
    draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 11, 4, 4, NO_VALUE, "Prset", ILI9341_BLUE, 2, false, false);
    draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 12, 4, 4, PMpresetNr, NO_NAME, ILI9341_BLUE, 2, true, false);
}

void Track::set_presetNr()
{
    if (inputs.active[PRESET_ENCODER])
    {
        PMpresetNr = inputs.getValueFromEncoder(PRESET_ENCODER, PMpresetNr, NUM_PLUGIN_PRESETS - 1);
        // PMpresetNr = constrain(PMpresetNr + inputs.encoded[PRESET_ENCODER], 0, NUM_PLUGIN_PRESETS - 1);
        change_plugin_row = true;
        Serial.println("change playmode preset");
        draw_sequencer_modes(parameter[SET_SEQ_MODE]);
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
                    sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut, my_Arranger_Y_axis - 1);
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
        int noteToSend = useMemory ? seqModNoteMemory[0][stepCount] : get_random_Note_in_scale();
        int note2save = noteToSend + (random(seqMod_value[1][playPresetNr][0], seqMod_value[1][playPresetNr][1] + 1) * 12) + noteOffset[external_clock_bar] + performNoteOffset;
        noteToPlay[0] = seqModNoteMemory[0][stepCount];
        noteOffAt[0] = constrain(tick.startTick[0] + tick.noteLength[0], 0, MAX_TICKS - 1);
        uint8_t Velo = random(seqMod_value[1][playPresetNr][2], seqMod_value[1][playPresetNr][3]) * (barVelocity[external_clock_bar] / MIDI_CC_RANGE_FLOAT) * (mixGainPot / MIDI_CC_RANGE_FLOAT);
        uint8_t StepFX = random(seqMod_value[1][playPresetNr][8], seqMod_value[1][playPresetNr][9]);
        sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut, my_Arranger_Y_axis - 1);
        noteOn(noteToPlay[0], Velo, clip[clipIndex].midiChOut); // Send a Note
        Serial.printf("pl1 Note: %d, Velo: %d, StartTick: %d, EndTick: %d, StepFX: %d\n", noteToPlay[0], Velo, tick.startTick[0], noteOffAt[0], StepFX);

        // save new note into array
        if (!useMemory)
        {
            seqModNoteMemory[0][stepCount] = note2save;
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
        seqModNoteMemory[0][cloock / TICKS_PER_STEP] = noteToPlay[0];
        // Serial.printf("octave:%d maxValIndex: %d, noteToplay: %d\n", thisOctave, maxValIndex, noteToPlay[0]);
        uint8_t Velo = get_note_parameter(clip[clip_to_play[internal_clock_bar]].tick[cloock].velo, 0) * (barVelocity[external_clock_bar] / 127.00) * (mixGainPot / 127.00);
        uint8_t StepFX = get_note_parameter(&clip[clip_to_play[internal_clock_bar]].tick[cloock].stepFX, 0);

        sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut, my_Arranger_Y_axis - 1);
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
            if (!note_is_on[v] && seqMod_value[3][playPresetNr][v] < NO_NOTE)
            {
                noteToPlay[v] = v + (parameter[SET_OCTAVE] * NOTES_PER_OCTAVE) + noteOffset[external_clock_bar] + performNoteOffset;
                seqModNoteMemory[v][cloock / TICKS_PER_STEP] = noteToPlay[v];
                // uint8_t Velo = get_note_parameter(clip[clip_to_play[internal_clock_bar]].tick[cloock].velo, v) * (barVelocity[external_clock_bar] / 127.00) * (mixGainPot / 127.00);
                uint8_t Velo = random(seqMod_value[3][playPresetNr][12], seqMod_value[3][playPresetNr][13]) * (barVelocity[external_clock_bar] / 127.00) * (mixGainPot / 127.00);
                uint8_t StepFX = random(seqMod_value[3][playPresetNr][14], seqMod_value[3][playPresetNr][15]);
                noteOffAt[v] = constrain(cloock + TICKS_PER_STEP - 1, 0, MAX_TICKS - 1);
                note_is_on[v] = true;
                sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut, my_Arranger_Y_axis - 1);
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
// 16 step pot sequencer
void Track::play_seq_mode4(uint8_t cloock)
{
    const uint8_t clipIndex = clip_to_play[internal_clock_bar];
    uint8_t playPresetNr = play_presetNr_Playmode_ccChannel[external_clock_bar];

    uint8_t seq3_clock = cloock / TICKS_PER_STEP;
    bool seq4_bool = (cloock + 1) % 6;

    if (seq3_clock == 16)
        seq3_clock = 0;

    if (cloock % TICKS_PER_STEP == 0)
    {
        if (!note_is_on[0] && random(126) < barProbabilty[external_clock_bar])
        {
            noteToPlay[0] = seqMod_value[4][playPresetNr][seq3_clock] + noteOffset[external_clock_bar] + performNoteOffset;
            uint8_t Velo = 99 * (barVelocity[external_clock_bar] / 127) * (mixGainPot / 127.00);
            uint8_t StepFX = 128;
            seqModNoteMemory[0][cloock / TICKS_PER_STEP] = noteToPlay[0];
            note_is_on[0] = true;
            noteOffAt[0] = constrain(cloock + parameter[SET_STEP_LENGTH], 0, MAX_TICKS - 1);
            sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut, my_Arranger_Y_axis - 1);
            noteOn(noteToPlay[0], Velo, clip[clipIndex].midiChOut); // Send a Note (pitch 42, velo 127 on channel 1)
                                                                    // Serial.printf("ON   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
            Serial.printf("pl4 Note: %d, Velo: %d, StartTick: %d, EndTick: %d, StepFX: %d\n", noteToPlay[0], Velo, cloock, noteOffAt[0], StepFX);
        }
    }

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
// beatArray
void Track::play_seq_mode5(uint8_t cloock)
{
    const uint8_t clipIndex = clip_to_play[internal_clock_bar];
    uint8_t playPresetNr = play_presetNr_Playmode_ccChannel[external_clock_bar];

    uint8_t seq3_clock = cloock / TICKS_PER_STEP;
    if (seq3_clock == 16)
        seq3_clock = 0;

    if (cloock % TICKS_PER_STEP == 0)
    {
        Serial.println("pl5 bool active");
        for (int v = 0; v < MAX_VOICES; v++)
        {
            if (beatArray[seqMod_value[5][playPresetNr][v]][seq3_clock] && random(126) < barProbabilty[external_clock_bar])
            {
                if (!note_is_on[v])
                {
                    noteToPlay[v] = (v + (parameter[SET_OCTAVE] * NOTES_PER_OCTAVE)) + noteOffset[external_clock_bar] + performNoteOffset;
                    seqModNoteMemory[v][cloock / TICKS_PER_STEP] = noteToPlay[v];
                    uint8_t Velo = random(seqMod_value[5][playPresetNr][12], seqMod_value[5][playPresetNr][13]) * (barVelocity[external_clock_bar] / 127.00) * (mixGainPot / 127.00);
                    uint8_t StepFX = random(seqMod_value[5][playPresetNr][14], seqMod_value[5][playPresetNr][15]);
                    note_is_on[v] = true;
                    sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut, my_Arranger_Y_axis - 1);
                    noteOn(noteToPlay[v], Velo, clip[clipIndex].midiChOut); // Send a Note (pitch 42, velo 127 on channel 1)
                    noteOffAt[v] = constrain(cloock + parameter[SET_STEP_LENGTH], 0, MAX_TICKS - 1);
                    Serial.printf("pl5 Note: %d, Velo: %d, StartTick: %d, EndTick: %d, StepFX: %d\n", noteToPlay[v], Velo, cloock, noteOffAt[v], StepFX);
                    // Serial.printf("ON   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
                }
            }
        }
    }

    for (int i = 0; i < MAX_VOICES; i++)
    {
        if (noteOffAt[i] == cloock)
        {
            note_is_on[i] = false;
            noteOff(noteToPlay[i], 0, clip[clipIndex].midiChOut); // Send a Note (pitch 42, velo 127 on channel 1)
            // Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
            noteToPlay[i] = NO_NOTE;
        }
    }
}
// Euclid
void Track::play_seq_mode6(uint8_t cloock)
{
    uint8_t clipIndex = clip_to_play[internal_clock_bar];
    uint8_t playPresetNr = play_presetNr_Playmode_ccChannel[external_clock_bar];

    uint8_t seq3_clock = cloock / TICKS_PER_STEP;
    if (seq3_clock == 16)
        seq3_clock = 0;

    if (cloock % TICKS_PER_STEP == 0)
    {
        Serial.println("pl6 bool active");
        for (int v = 0; v < MAX_VOICES; v++)
        {
            if (beatArrayPM6[v][seq3_clock] && random(126) < barProbabilty[external_clock_bar])
            {

                if (!note_is_on[v])
                {
                    noteToPlay[v] = (v + (parameter[SET_OCTAVE] * NOTES_PER_OCTAVE)) + noteOffset[external_clock_bar] + performNoteOffset;
                    seqModNoteMemory[v][cloock / TICKS_PER_STEP] = noteToPlay[v];
                    uint8_t Velo = random(seqMod_value[6][playPresetNr][12], seqMod_value[6][playPresetNr][13]) * (barVelocity[external_clock_bar] / 127.00) * (mixGainPot / 127.00);
                    uint8_t StepFX = random(seqMod_value[6][playPresetNr][14], seqMod_value[6][playPresetNr][15]);
                    note_is_on[v] = true;
                    sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut, my_Arranger_Y_axis - 1);
                    noteOn(noteToPlay[v], Velo, clip[clipIndex].midiChOut); // Send a Note (pitch 42, velo 127 on channel 1)
                    noteOffAt[v] = constrain(cloock + parameter[SET_STEP_LENGTH], 0, MAX_TICKS - 1);
                    Serial.printf("pl6 Note: %d, Velo: %d, StartTick: %d, EndTick: %d, StepFX: %d\n", noteToPlay[v], Velo, cloock, noteOffAt[v], StepFX);

                    // Serial.printf("ON   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
                }
            }
        }
    }
    for (int i = 0; i < MAX_VOICES; i++)
    {
        if (noteOffAt[i] == cloock)
        {
            note_is_on[i] = false;
            noteOff(noteToPlay[i], 0, clip[clipIndex].midiChOut); // Send a Note (pitch 42, velo 127 on channel 1)
            // Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
            noteToPlay[i] = NO_NOTE;
            noteOffAt[i] = -1;
        }
    }
}
void Track::set_seq_mode6_value(uint8_t XPos, uint8_t YPos)
{
    int n = XPos + (YPos * NUM_ENCODERS);
    if (inputs.enc_moved[XPos] && !neotrellisPressed[TRELLIS_BUTTON_ENTER])
    {
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

    if (inputs.enc_moved[XPos] && neotrellisPressed[TRELLIS_BUTTON_ENTER])
    {
        SeqMod6Value2[n] = constrain(SeqMod6Value2[n] + inputs.encoded[XPos], 0, MIDI_CC_RANGE + 1);
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
// Rclid
void Track::play_seq_mode7(uint8_t cloock)
{
    const uint8_t clipIndex = clip_to_play[internal_clock_bar];
    uint8_t playPresetNr = play_presetNr_Playmode_ccChannel[external_clock_bar];
    uint8_t seq3_clock = cloock / TICKS_PER_STEP;
    bool seq4_bool = cloock % 6;

    if (cloock % TICKS_PER_STEP)
    {

        if (beatArrayPM7[seq3_clock])
        {
            if (!note_is_on[0] && random(126) < barProbabilty[external_clock_bar])
            {
                static int stepCount;                                                // max Steps
                stepCount = (stepCount + 1) % seqMod_value[7][playPresetNr][0];      // max Steps
                bool useMemory = ((random(126)) < seqMod_value[7][playPresetNr][2]); // dejavu
                int noteToSend = useMemory ? seqModNoteMemory[0][stepCount] : get_random_Note_in_scale();
                int note2save = noteToSend + (random(seqMod_value[7][playPresetNr][4], seqMod_value[7][playPresetNr][5] + 1) * 12) + noteOffset[external_clock_bar] + performNoteOffset;

                noteToPlay[0] = seqModNoteMemory[0][stepCount];
                // noteToPlay[0] = get_random_Note_in_scale() + (random(seqMod_value[7][4], seqMod_value[7][5]) * NOTES_PER_OCTAVE) + noteOffset[external_clock_bar] + performNoteOffset;
                uint8_t Velo = random(seqMod_value[7][playPresetNr][6], seqMod_value[7][playPresetNr][7]) * (barVelocity[external_clock_bar] / 127) * (mixGainPot / 127.00);
                uint8_t StepFX = random(seqMod_value[7][playPresetNr][8], seqMod_value[7][playPresetNr][9]) * (barVelocity[external_clock_bar] / 127) * (mixGainPot / 127.00);
                note_is_on[0] = true;
                seqModNoteMemory[0][seq3_clock] = noteToPlay[0];
                sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut, my_Arranger_Y_axis - 1);
                noteOn(noteToPlay[0], Velo, clip[clipIndex].midiChOut); // Send a Note (pitch 42, velo 127 on channel 1)

                noteOffAt[0] = constrain(cloock + parameter[SET_STEP_LENGTH], 0, MAX_TICKS - 1);
                Serial.printf("pl5 Note: %d, Velo: %d, StartTick: %d, EndTick: %d, StepFX: %d\n", noteToPlay[0], Velo, cloock, noteOffAt[0], StepFX);
                if (!useMemory)
                {
                    seqModNoteMemory[0][stepCount] = note2save;
                    // Serial.printf("PM1 = save note: %d on step: %d\n", seqModNoteMemory[stepCount], stepCount);
                    //  velocityMemory[stepCount] = volSend;
                }
            }
        }
    }

    if (noteOffAt[0] == cloock)
    {
        note_is_on[0] = false;
        noteOff(noteToPlay[0], 0, clip[clipIndex].midiChOut); // Send a Note (pitch 42, velo 127 on channel 1)
        // Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
        noteToPlay[0] = NO_NOTE;
        noteOffAt[0] = -1;
    }
}
void Track::set_seq_mode7_value(uint8_t XPos, uint8_t YPos)
{
    int n = XPos + (YPos * NUM_ENCODERS);
    if (inputs.enc_moved[XPos])
    {

        seqMod_value[7][PMpresetNr][n] = constrain(seqMod_value[7][PMpresetNr][n] + inputs.encoded[XPos], 0, MIDI_CC_RANGE);

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
            seqModNoteMemory[v][cloock / TICKS_PER_STEP] = noteToPlay[v];
            // noteOffAt[0] = tick.startTick[0] + tick.noteLength[0];
            uint8_t Velo = noteInfo[v][1] * (barVelocity[external_clock_bar] / MIDI_CC_RANGE_FLOAT) * (mixGainPot / MIDI_CC_RANGE_FLOAT);
            uint8_t StepFX = random(seqMod_value[8][playPresetNr][1], seqMod_value[8][playPresetNr][2]);
            sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut, my_Arranger_Y_axis - 1);
            noteOn(noteToPlay[v], Velo, clip[clipIndex].midiChOut); // Send a Note
            noteOffAt[v] = startTick + length;
            Serial.printf("pl8 Note: %d, Velo: %d, StartTick: %d, EndTick: %d, StepFX: %d\n", noteToPlay[v], Velo, startTick, noteOffAt[v], StepFX);

            //    Serial.printf("PL8 NOte: %d, Vel: %d, Start: %d, Len: %d, noteOffAt: %d, voice: %d, tick: %d\n", noteToPlay[v], Velo, startTick, length, noteOffAt[v], v, songTick);
        }
    }
}
void Track::select_file(uint8_t XPos, uint8_t YPos)
{
    if (inputs.enc_moved[XPos])
    {
        int n = XPos + (YPos * NUM_ENCODERS);
        seqMod_value[8][PMpresetNr][n] = constrain(seqMod_value[8][PMpresetNr][n] + inputs.encoded[XPos], 0, 127); // mode 3 =MIDI_CC_RANGE * 2 //mode4 =NO_NOTE //others=MIDI_CC_RANGE
        char filename[20];
        sprintf(filename, "%d.mid", seqMod_value[8][PMpresetNr][n]);
        loadMidiFile(filename, myMidi[my_Arranger_Y_axis - 1]);
        seqMod_value[8][PMpresetNr][3] = getPPQN(myMidi[my_Arranger_Y_axis - 1]) / 24;
        Serial.printf("select filename: %s, value: %d\n", filename, seqMod_value[8][PMpresetNr][0]);
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
        sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut, my_Arranger_Y_axis - 1);
        noteOn(noteToPlay[0], Velo, clip[clipIndex].midiChOut); // Send a Note
        seqModNoteMemory[0][cloock / TICKS_PER_STEP] = noteToPlay[0];
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

        sendControlChange(parameter[14], StepFX, clip[clipIndex].midiChOut, my_Arranger_Y_axis - 1);
        noteToPlay[0] = finalNote;
        seqModNoteMemory[0][cloock / TICKS_PER_STEP] = finalNote;
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
