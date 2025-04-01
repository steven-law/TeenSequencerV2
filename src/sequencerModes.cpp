#include <Arduino.h>
#include "projectVariables.h"
#include "Track.h"

void Track::set_seq_mode_value(uint8_t modeindex, uint8_t XPos, uint8_t YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int n = XPos + (YPos * NUM_ENCODERS);
        seqMod_value[modeindex][n] = constrain(seqMod_value[modeindex][n] + encoded[XPos], min, max); // mode 3 =MIDI_CC_RANGE * 2 //mode4 =NO_NOTE //others=MIDI_CC_RANGE
        drawPot(XPos, YPos, seqMod_value[modeindex][n], name);
    }
}

uint8_t Track::get_random_Note_in_scale()
{
    int randomNote;

    // Wiederhole, bis eine gültige Note gefunden wurde
    do
    {
        randomNote = random(0, 11);
    } while (!scales[parameter[SET_SCALE]][randomNote]);
    return randomNote;
}
void Track::play_seq_mode0(uint8_t cloock)
{
    // Serial.printf("track= %d,tick: %d, array note= %d on voice: %d\n", my_Arranger_Y_axis, cloock, this->array[clip_to_play[internal_clock_bar]][cloock][0], 0);
    for (int v = 0; v < MAX_VOICES; v++)
    {

        if (get_note_parameter(clip[clip_to_play[external_clock_bar]].tick[cloock].voice, v) < NO_NOTE)
        {
            if (!note_is_on[v])
            {
                if (random(126) < parameter[SET_PROBABILTY]) // probabilty
                {
                    noteToPlay[v] = get_note_parameter(clip[clip_to_play[internal_clock_bar]].tick[cloock].voice, v) + noteOffset[external_clock_bar] + performNoteOffset;
                    uint8_t Velo = get_note_parameter(clip[clip_to_play[internal_clock_bar]].tick[cloock].velo, v) * (barVelocity[external_clock_bar] / 127.00) * (mixGainPot / 127.00);
                    uint8_t StepFX = get_note_parameter(clip[clip_to_play[internal_clock_bar]].tick[cloock].stepFX, v);
                    note_is_on[v] = true;
                    sendControlChange(parameter[14], StepFX, parameter[SET_MIDICH_OUT]);
                    noteOn(noteToPlay[v], Velo, parameter[SET_MIDICH_OUT]);
                }
            }
        }
       else if (get_note_parameter(clip[clip_to_play[external_clock_bar]].tick[cloock].voice, v) == NO_NOTE)
        {
            if (note_is_on[v])
            {
                note_is_on[v] = false;
                noteOff(noteToPlay[v], 0, parameter[SET_MIDICH_OUT]);
            }
        }
    }
}
// random
void Track::play_seq_mode1(uint8_t cloock)
{
    /*
    0  rootnote
    1  scale
    2  octmin
    3  octmax
    4  maxsteps
    5  dejaVu
    6  probabilty
    8  volmin
    9  volmax
      */
    if (get_note_parameter(clip[clip_to_play[external_clock_bar]].tick[cloock].voice, 0) < NO_NOTE)
    {
        static int stepCount = (stepCount + 1) % seqMod_value[1][4];
        if (!note_is_on[0])
        {
            if (random(126) < seqMod_value[1][6]) // probabilty
            {
                bool useMemory = ((rand() % 127) < seqMod_value[1][5]); // dejavu
                int noteToSend = useMemory ? seqMod1NoteMemory[stepCount] : get_random_Note_in_scale();

                noteToPlay[0] = noteToSend + (random(seqMod_value[1][2], seqMod_value[1][3] + 1) * 12) + noteOffset[external_clock_bar] + performNoteOffset;
                uint8_t Velo = random(seqMod_value[1][8], seqMod_value[1][9]) * (barVelocity[external_clock_bar] / 127) * (mixGainPot / 127.00);
                note_is_on[0] = true;
                noteOn(noteToPlay[0], Velo, parameter[SET_MIDICH_OUT]); // Send a Note
                if (!useMemory)
                {
                    seqMod1NoteMemory[stepCount] = noteToPlay[0];
                    // velocityMemory[stepCount] = volSend;
                }
            }
        }
    }

    if (get_note_parameter(clip[clip_to_play[external_clock_bar]].tick[cloock].voice, 0) == NO_NOTE)
    {
        if (note_is_on[0])
        {
            note_is_on[0] = false;
            noteOff(noteToPlay[0], 0, parameter[SET_MIDICH_OUT]); // Send a Note (pitch 42, velo 127 on channel 1)
                                                                  // Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
        }
    }
}
void Track::set_seq_mode1_parameters(uint8_t row)
{ /*
    0  rootnote
    1  scale
    2  octmin
    3  octmax
    4  maxsteps
    5  dejaVu
    6  probabilty
    8  volmin
    9  volmax
      */
    draw_seq_mode1();
    if (row == 0)
    {
        set_seq_mode_value(1, 0, 0, "baseNote", 0, MIDI_CC_RANGE);
        set_seq_mode_value(1, 1, 0, "Scale", 0, MIDI_CC_RANGE);
        set_seq_mode_value(1, 2, 0, "Oct -", 0, MIDI_CC_RANGE);
        set_seq_mode_value(1, 3, 0, "Oct +", 0, MIDI_CC_RANGE);
    }
    if (row == 1)
    {

        set_seq_mode_value(1, 0, 1, "maxSteps", 0, NUM_STEPS);
        set_seq_mode_value(1, 1, 1, "Dejavu", 0, MIDI_CC_RANGE);
        set_seq_mode_value(1, 2, 1, "Probality", 0, MIDI_CC_RANGE);
    }
    if (row == 2)
    {

        set_seq_mode_value(1, 0, 2, "Vol -", 0, MIDI_CC_RANGE);
        set_seq_mode_value(1, 1, 2, "Vol +", 0, MIDI_CC_RANGE);
    }
}

void Track::draw_seq_mode1()
{ /*
    0  rootnote
    1  scale
    2  octmin
    3  octmax

    4  maxsteps
    5  dejaVu
    6  probabilty

    8  volmin
    9  volmax
      */
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, seqMod_value[1][0], "baseNote");
        drawPot(1, 0, seqMod_value[1][1], "Scale");
        drawPot(2, 0, seqMod_value[1][2], "Oct -");
        drawPot(3, 0, seqMod_value[1][3], "Oct +");

        drawPot(0, 1, seqMod_value[1][4], "maxSteps");
        drawPot(1, 1, seqMod_value[1][5], "Dejavu");
        drawPot(2, 1, seqMod_value[1][6], "Probality");

        drawPot(0, 2, seqMod_value[1][8], "Vol -");
        drawPot(1, 2, seqMod_value[1][9], "Vol +");
    }
}
// dropseq
void Track::play_seq_mode2(uint8_t cloock)
{
    static uint8_t maxValIndex;
    static uint8_t analogReadArray[12];
    uint8_t cc23 = seqMod_value[2][2];
    uint8_t cc24 = seqMod_value[2][3];
    uint8_t thisOctave = 4;

    if (get_note_parameter(clip[clip_to_play[external_clock_bar]].tick[cloock].voice, 0) < NO_NOTE)
    {
        if (!note_is_on[0])
        {
            maxVal = 0;
            if (seqMod_value[2][2] <= seqMod_value[2][3])
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

            if (seqMod_value[2][2] > seqMod_value[2][3])
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

            Serial.printf("octave:%d\n", thisOctave);
            note_is_on[0] = true;
            noteToPlay[0] = (maxValIndex) + (thisOctave * 12) + noteOffset[external_clock_bar] + performNoteOffset;
            uint8_t Velo = get_note_parameter(clip[clip_to_play[internal_clock_bar]].tick[cloock].velo, 0) * (barVelocity[external_clock_bar] / 127.00) * (mixGainPot / 127.00);
            uint8_t StepFX = get_note_parameter(clip[clip_to_play[internal_clock_bar]].tick[cloock].stepFX, 0);

            sendControlChange(parameter[14], StepFX, parameter[SET_MIDICH_OUT]);
            noteOn(noteToPlay[0], Velo, parameter[SET_MIDICH_OUT]);
            // Serial.print(track[i].notePlayed[0]);
            // Serial.print("--");

            analogReadArray[maxValIndex] = (analogReadArray[maxValIndex] - seqMod_value[2][0]);
            // Serial.println(maxValIndex);
        }
    }
    // NoteOff
    if (get_note_parameter(clip[clip_to_play[external_clock_bar]].tick[cloock].voice, 0) == NO_NOTE)
    {
        if (note_is_on[0])
        {

            note_is_on[0] = false;
            noteOff(noteToPlay[0], 0, parameter[SET_MIDICH_OUT]);
            // Serial.println(track[i].notePlayed[0]);
        }
    }

    if (analogReadArray[maxValIndex] <= seqMod_value[2][1])
    {
        for (int i = 0; i < 12; i++)
        {
            analogReadArray[i] = seqMod_value[2][i + 4];
        }
    }
}
void Track::set_seq_mode2_parameters(uint8_t row)
{
    draw_seq_mode2();
    if (row == 0)
    {
        set_seq_mode_value(2, 0, 0, "Drop", 0, NO_NOTE);
        set_seq_mode_value(2, 1, 0, "Rst @", 0, NO_NOTE);
        set_seq_mode_value(2, 2, 0, "Oct -", 0, NO_NOTE);
        set_seq_mode_value(2, 3, 0, "Oct +", 0, NO_NOTE);
    }
    if (row == 1)
    {
        set_seq_mode_value(2, 0, 1, "C", 0, NO_NOTE);
        set_seq_mode_value(2, 1, 1, "C#", 0, NO_NOTE);
        set_seq_mode_value(2, 2, 1, "D", 0, NO_NOTE);
        set_seq_mode_value(2, 3, 1, "D#", 0, NO_NOTE);
    }
    if (row == 2)
    {
        set_seq_mode_value(2, 0, 2, "E", 0, NO_NOTE);
        set_seq_mode_value(2, 1, 2, "F", 0, NO_NOTE);
        set_seq_mode_value(2, 2, 2, "F#", 0, NO_NOTE);
        set_seq_mode_value(2, 3, 2, "G", 0, NO_NOTE);
    }
    if (row == 3)
    {
        set_seq_mode_value(2, 0, 3, "G#", 0, NO_NOTE);
        set_seq_mode_value(2, 1, 3, "A", 0, NO_NOTE);
        set_seq_mode_value(2, 2, 3, "A#", 0, NO_NOTE);
        set_seq_mode_value(2, 3, 3, "B", 0, NO_NOTE);
    }
}

void Track::draw_seq_mode2()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, seqMod_value[2][0], "Drop");
        drawPot(1, 0, seqMod_value[2][1], "Rst @");
        drawPot(2, 0, seqMod_value[2][2], "Oct -");
        drawPot(3, 0, seqMod_value[2][3], "Oct +");

        drawPot(0, 1, seqMod_value[2][4], "C");
        drawPot(1, 1, seqMod_value[2][5], "C#");
        drawPot(2, 1, seqMod_value[2][6], "D");
        drawPot(3, 1, seqMod_value[2][7], "D#");

        drawPot(0, 2, seqMod_value[2][8], "E");
        drawPot(1, 2, seqMod_value[2][9], "F");
        drawPot(2, 2, seqMod_value[2][10], "F#");
        drawPot(3, 2, seqMod_value[2][11], "G");

        drawPot(0, 3, seqMod_value[2][12], "G#");
        drawPot(1, 3, seqMod_value[2][13], "A");
        drawPot(2, 3, seqMod_value[2][14], "A#");
        drawPot(3, 3, seqMod_value[2][15], "B");
    }
}
// bitread
void Track::play_seq_mode3(uint8_t cloock)
{
    uint8_t seq3_clock = cloock / TICKS_PER_STEP;
    if (seq3_clock == 8)
        seq3_clock = 0;
    for (int v = 0; v < MAX_VOICES; v++)
    {
        if (bitRead(seqMod_value[3][v], seq3_clock))
        {
            if (!note_is_on[v])
            {
                noteToPlay[v] = v + (parameter[SET_OCTAVE] * 12) + noteOffset[external_clock_bar] + performNoteOffset;
                uint8_t Velo = get_note_parameter(clip[clip_to_play[internal_clock_bar]].tick[cloock].velo, v) * (barVelocity[external_clock_bar] / 127.00) * (mixGainPot / 127.00);
                uint8_t StepFX = get_note_parameter(clip[clip_to_play[internal_clock_bar]].tick[cloock].stepFX, v);

                note_is_on[v] = true;
                sendControlChange(parameter[14], StepFX, parameter[SET_MIDICH_OUT]);
                noteOn(noteToPlay[v], Velo, parameter[SET_MIDICH_OUT]); // Send a Note (pitch 42, velo 127 on channel 1)
                                                                        // Serial.printf("ON   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
            }
        }

        if (!bitRead(seqMod_value[3][v], seq3_clock))
        {
            if (note_is_on[v])
            {
                note_is_on[v] = false;
                noteOff(noteToPlay[v], 0, parameter[SET_MIDICH_OUT]); // Send a Note (pitch 42, velo 127 on channel 1)
                //  Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
            }
        }
    }
}
void Track::set_seq_mode3_parameters(uint8_t row)
{
    draw_seq_mode3();
    if (row == 0)
    {
        set_seq_mode_value(3, 0, 0, noteNames[0], 0, MIDI_CC_RANGE * 2);
        set_seq_mode_value(3, 1, 0, noteNames[1], 0, MIDI_CC_RANGE * 2);
        set_seq_mode_value(3, 2, 0, noteNames[2], 0, MIDI_CC_RANGE * 2);
        set_seq_mode_value(3, 3, 0, noteNames[3], 0, MIDI_CC_RANGE * 2);
        set_seq_mode_value(3, 0, 1, noteNames[4], 0, MIDI_CC_RANGE * 2);
        set_seq_mode_value(3, 1, 1, noteNames[5], 0, MIDI_CC_RANGE * 2);
        set_seq_mode_value(3, 2, 1, noteNames[6], 0, MIDI_CC_RANGE * 2);
        set_seq_mode_value(3, 3, 1, noteNames[7], 0, MIDI_CC_RANGE * 2);
        set_seq_mode_value(3, 0, 2, noteNames[8], 0, MIDI_CC_RANGE * 2);
        set_seq_mode_value(3, 1, 2, noteNames[9], 0, MIDI_CC_RANGE * 2);
        set_seq_mode_value(3, 2, 2, noteNames[10], 0, MIDI_CC_RANGE * 2);
        set_seq_mode_value(3, 3, 2, noteNames[11], 0, MIDI_CC_RANGE * 2);
    }
}

void Track::draw_seq_mode3()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, seqMod_value[3][0], noteNames[0]);
        drawPot(1, 0, seqMod_value[3][1], noteNames[1]);
        drawPot(2, 0, seqMod_value[3][2], noteNames[2]);
        drawPot(3, 0, seqMod_value[3][3], noteNames[3]);

        drawPot(0, 1, seqMod_value[3][4], noteNames[4]);
        drawPot(1, 1, seqMod_value[3][5], noteNames[5]);
        drawPot(2, 1, seqMod_value[3][6], noteNames[6]);
        drawPot(3, 1, seqMod_value[3][7], noteNames[7]);

        drawPot(0, 2, seqMod_value[3][8], noteNames[8]);
        drawPot(1, 2, seqMod_value[3][9], noteNames[9]);
        drawPot(2, 2, seqMod_value[3][10], noteNames[10]);
        drawPot(3, 2, seqMod_value[3][11], noteNames[11]);
    }
}
// 16 step pot sequencer
void Track::play_seq_mode4(uint8_t cloock)
{
    if (clip_to_play[external_clock_bar] <= NUM_USER_CLIPS)
    {
        uint8_t seq3_clock = cloock / TICKS_PER_STEP;
        bool seq4_bool = (cloock + 1) % 6;
        if (seq3_clock == 16)
            seq3_clock = 0;

        if (seq4_bool)
        {
            if (!note_is_on[0])
            {
                noteToPlay[0] = seqMod_value[4][seq3_clock] + noteOffset[external_clock_bar] + performNoteOffset;
                uint8_t Velo = 99 * (barVelocity[external_clock_bar] / 127) * (mixGainPot / 127.00);
                uint8_t StepFX = 128;
                note_is_on[0] = true;
                sendControlChange(parameter[14], StepFX, parameter[SET_MIDICH_OUT]);
                noteOn(noteToPlay[0], Velo, parameter[SET_MIDICH_OUT]); // Send a Note (pitch 42, velo 127 on channel 1)
                                                                        // Serial.printf("ON   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
            }
        }

        else if (!seq4_bool)
        {
            if (note_is_on[0])
            {
                note_is_on[0] = false;
                noteOff(noteToPlay[0], 0, parameter[SET_MIDICH_OUT]); // Send a Note (pitch 42, velo 127 on channel 1)
                                                                      // Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
            }
        }
    }
}
void Track::set_seq_mode4_parameters(uint8_t row)
{
    draw_seq_mode4();
    if (row == 0)
    {
        set_seq_mode_value(4, 0, 0, "1", 0, MIDI_CC_RANGE);
        set_seq_mode_value(4, 1, 0, "2", 0, MIDI_CC_RANGE);
        set_seq_mode_value(4, 2, 0, "3", 0, MIDI_CC_RANGE);
        set_seq_mode_value(4, 3, 0, "4", 0, MIDI_CC_RANGE);
    }
    if (row == 1)
    {
        set_seq_mode_value(4, 0, 1, "5", 0, MIDI_CC_RANGE);
        set_seq_mode_value(4, 1, 1, "6", 0, MIDI_CC_RANGE);
        set_seq_mode_value(4, 2, 1, "7", 0, MIDI_CC_RANGE);
        set_seq_mode_value(4, 3, 1, "8", 0, MIDI_CC_RANGE);
    }
    if (row == 2)
    {
        set_seq_mode_value(4, 0, 2, "9", 0, MIDI_CC_RANGE);
        set_seq_mode_value(4, 1, 2, "10", 0, MIDI_CC_RANGE);
        set_seq_mode_value(4, 2, 2, "11", 0, MIDI_CC_RANGE);
        set_seq_mode_value(4, 3, 2, "12", 0, MIDI_CC_RANGE);
    }
    if (row == 3)
    {
        set_seq_mode_value(4, 0, 3, "13", 0, MIDI_CC_RANGE);
        set_seq_mode_value(4, 1, 3, "14", 0, MIDI_CC_RANGE);
        set_seq_mode_value(4, 2, 3, "15", 0, MIDI_CC_RANGE);
        set_seq_mode_value(4, 3, 3, "16", 0, MIDI_CC_RANGE);
    }
}

void Track::draw_seq_mode4()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, seqMod_value[4][0], "1");
        drawPot(1, 0, seqMod_value[4][1], "2");
        drawPot(2, 0, seqMod_value[4][2], "3");
        drawPot(3, 0, seqMod_value[4][3], "4");

        drawPot(0, 1, seqMod_value[4][4], "5");
        drawPot(1, 1, seqMod_value[4][5], "6");
        drawPot(2, 1, seqMod_value[4][6], "7");
        drawPot(3, 1, seqMod_value[4][7], "8");

        drawPot(0, 2, seqMod_value[4][8], "9");
        drawPot(1, 2, seqMod_value[4][9], "10");
        drawPot(2, 2, seqMod_value[4][10], "11");
        drawPot(3, 2, seqMod_value[4][11], "12");

        drawPot(0, 3, seqMod_value[4][12], "13");
        drawPot(1, 3, seqMod_value[4][13], "14");
        drawPot(2, 3, seqMod_value[4][14], "15");
        drawPot(3, 3, seqMod_value[4][15], "16");
    }
}

// beatArray
void Track::play_seq_mode5(uint8_t cloock)
{
    if (clip_to_play[external_clock_bar] <= NUM_USER_CLIPS)
    {
        uint8_t seq3_clock = cloock / TICKS_PER_STEP;
        bool seq4_bool = cloock % TICKS_PER_STEP;
        if (seq3_clock == 16)
            seq3_clock = 0;

        if (seq4_bool)
        {
            for (int i = 0; i < MAX_VOICES; i++)
            {
                if (beatArray[seqMod_value[5][i]][seq3_clock])
                {

                    if (!note_is_on[i])
                    {
                        noteToPlay[i] = (i + (parameter[SET_OCTAVE] * NOTES_PER_OCTAVE)) + noteOffset[external_clock_bar] + performNoteOffset;
                        uint8_t Velo = 99 * (barVelocity[external_clock_bar] / 127) * (mixGainPot / 127.00);
                        uint8_t StepFX = 128;
                        note_is_on[i] = true;
                        sendControlChange(parameter[14], StepFX, parameter[SET_MIDICH_OUT]);
                        noteOn(noteToPlay[i], Velo, parameter[SET_MIDICH_OUT]); // Send a Note (pitch 42, velo 127 on channel 1)
                        // Serial.printf("ON   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
                        clip[parameter[SET_CLIP2_EDIT]].tick[seq3_clock * TICKS_PER_STEP].voice[i] = noteToPlay[i];
                    }
                }
            }
        }

        if (!seq4_bool)
        {
            for (int i = 0; i < MAX_VOICES; i++)
            {
                if (note_is_on[i])
                {
                    note_is_on[i] = false;
                    noteOff(noteToPlay[i], 0, parameter[SET_MIDICH_OUT]); // Send a Note (pitch 42, velo 127 on channel 1)
                                                                          // Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
                }
            }
        }
    }
}
void Track::set_seq_mode5_parameters(uint8_t row)
{
    draw_seq_mode5();
    if (row == 0)
    {
        set_seq_mode_value(5, 0, 0, "1", 0, MIDI_CC_RANGE);
        set_seq_mode_value(5, 1, 0, "2", 0, MIDI_CC_RANGE);
        set_seq_mode_value(5, 2, 0, "3", 0, MIDI_CC_RANGE);
        set_seq_mode_value(5, 3, 0, "4", 0, MIDI_CC_RANGE);
    }
    if (row == 1)
    {
        set_seq_mode_value(5, 0, 1, "5", 0, MIDI_CC_RANGE);
        set_seq_mode_value(5, 1, 1, "6", 0, MIDI_CC_RANGE);
        set_seq_mode_value(5, 2, 1, "7", 0, MIDI_CC_RANGE);
        set_seq_mode_value(5, 3, 1, "8", 0, MIDI_CC_RANGE);
    }
    if (row == 2)
    {
        set_seq_mode_value(5, 0, 2, "9", 0, MIDI_CC_RANGE);
        set_seq_mode_value(5, 1, 2, "10", 0, MIDI_CC_RANGE);
        set_seq_mode_value(5, 2, 2, "11", 0, MIDI_CC_RANGE);
        set_seq_mode_value(5, 3, 2, "12", 0, MIDI_CC_RANGE);
    }
    if (row == 3)
    {
        //  set_seq_mode5_value(0, 3, "13");
        //  set_seq_mode5_value(1, 3, "14");
        //   set_seq_mode5_value(2, 3, "15");
        //   set_seq_mode5_value(3, 3, "16");
    }
}

void Track::draw_seq_mode5()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, seqMod_value[5][0], "1");
        drawPot(1, 0, seqMod_value[5][1], "2");
        drawPot(2, 0, seqMod_value[5][2], "3");
        drawPot(3, 0, seqMod_value[5][3], "4");

        drawPot(0, 1, seqMod_value[5][4], "5");
        drawPot(1, 1, seqMod_value[5][5], "6");
        drawPot(2, 1, seqMod_value[5][6], "7");
        drawPot(3, 1, seqMod_value[5][7], "8");

        drawPot(0, 2, seqMod_value[5][8], "9");
        drawPot(1, 2, seqMod_value[5][9], "10");
        drawPot(2, 2, seqMod_value[5][10], "11");
        drawPot(3, 2, seqMod_value[5][11], "12");

        //  drawPot(0, 3, seqMod_value[4][12], "13");
        //   drawPot(1, 3, seqMod_value[4][13], "14");
        //   drawPot(2, 3, seqMod_value[4][14], "15");
        //   drawPot(3, 3, seqMod_value[4][15], "16");
    }
}

// Euclid
void Track::play_seq_mode6(uint8_t cloock)
{
    uint8_t seq3_clock = cloock / TICKS_PER_STEP;
    bool seq4_bool = cloock % 6;
    if (seq3_clock == 16)
        seq3_clock = 0;

    if (seq4_bool)
    {
        for (int i = 0; i < MAX_VOICES; i++)
        {
            if (beatArrayPM6[i][seq3_clock])
            {

                if (!note_is_on[i])
                {
                    noteToPlay[i] = (i + (parameter[SET_OCTAVE] * NOTES_PER_OCTAVE)) + noteOffset[external_clock_bar] + performNoteOffset;
                    uint8_t Velo = 99 * (barVelocity[external_clock_bar] / 127) * (mixGainPot / 127.00);
                    uint8_t StepFX = 128;
                    note_is_on[i] = true;
                    sendControlChange(parameter[14], StepFX, parameter[SET_MIDICH_OUT]);
                    noteOn(noteToPlay[i], Velo, parameter[SET_MIDICH_OUT]); // Send a Note (pitch 42, velo 127 on channel 1)
                    // Serial.printf("ON   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
                }
            }
        }
    }

    if (!seq4_bool)
    {
        for (int i = 0; i < MAX_VOICES; i++)
        {
            if (note_is_on[i])
            {
                note_is_on[i] = false;
                noteOff(noteToPlay[i], 0, parameter[SET_MIDICH_OUT]); // Send a Note (pitch 42, velo 127 on channel 1)
                                                                      // Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
            }
        }
    }
}
void Track::set_seq_mode6_parameters(uint8_t row)
{
    draw_seq_mode6();
    if (row == 0)
    {
        set_seq_mode6_value(0, 0, "1");
        set_seq_mode6_value(1, 0, "2");
        set_seq_mode6_value(2, 0, "3");
        set_seq_mode6_value(3, 0, "4");
    }
    if (row == 1)
    {
        set_seq_mode6_value(0, 1, "5");
        set_seq_mode6_value(1, 1, "6");
        set_seq_mode6_value(2, 1, "7");
        set_seq_mode6_value(3, 1, "8");
    }
    if (row == 2)
    {
        set_seq_mode6_value(0, 2, "9");
        set_seq_mode6_value(1, 2, "10");
        set_seq_mode6_value(2, 2, "11");
        set_seq_mode6_value(3, 2, "12");
    }
    if (row == 3)
    {
        //  set_seq_mode6_value(0, 3, "13");
        //  set_seq_mode6_value(1, 3, "14");
        //   set_seq_mode6_value(2, 3, "15");
        //   set_seq_mode6_value(3, 3, "16");
    }
}
void Track::set_seq_mode6_value(uint8_t XPos, uint8_t YPos, const char *name)
{
    int n = XPos + (YPos * NUM_ENCODERS);
    if (enc_moved[XPos] && !neotrellisPressed[TRELLIS_BUTTON_ENTER])
    {

        seqMod_value[6][n] = constrain(seqMod_value[6][n] + encoded[XPos], 0, MIDI_CC_RANGE);
        drawPot(XPos, YPos, seqMod_value[6][n], name);
        int bucket = 0;
        int _numSteps = map(seqMod_value[6][n], 0, 127, 0, NUM_STEPS);
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

    if (enc_moved[XPos] && neotrellisPressed[TRELLIS_BUTTON_ENTER])
    {
        SeqMod6Value2[n] = constrain(SeqMod6Value2[n] + encoded[XPos], 0, MIDI_CC_RANGE);
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
        drawPot(0, 0, seqMod_value[6][0], "1");
        drawPot(1, 0, seqMod_value[6][1], "2");
        drawPot(2, 0, seqMod_value[6][2], "3");
        drawPot(3, 0, seqMod_value[6][3], "4");

        drawPot(0, 1, seqMod_value[6][4], "5");
        drawPot(1, 1, seqMod_value[6][5], "6");
        drawPot(2, 1, seqMod_value[6][6], "7");
        drawPot(3, 1, seqMod_value[6][7], "8");

        drawPot(0, 2, seqMod_value[6][8], "9");
        drawPot(1, 2, seqMod_value[6][9], "10");
        drawPot(2, 2, seqMod_value[6][10], "11");
        drawPot(3, 2, seqMod_value[6][11], "12");

        //  drawPot(0, 3, seqMod_value[4][12], "13");
        //   drawPot(1, 3, seqMod_value[4][13], "14");
        //   drawPot(2, 3, seqMod_value[4][14], "15");
        //   drawPot(3, 3, seqMod_value[4][15], "16");
    }
}

// Euclid
void Track::play_seq_mode7(uint8_t cloock)
{
    uint8_t seq3_clock = cloock / TICKS_PER_STEP;
    bool seq4_bool = cloock % 6;
    if (seq3_clock == 16)
        seq3_clock = 0;

    if (seq4_bool)
    {

        if (beatArrayPM7[seq3_clock])
        {

            if (!note_is_on[0])
            {
                noteToPlay[0] = random(seqMod_value[7][2], seqMod_value[7][3]) + noteOffset[external_clock_bar] + performNoteOffset;
                uint8_t Velo = random(seqMod_value[7][4], seqMod_value[7][5]) * (barVelocity[external_clock_bar] / 127) * (mixGainPot / 127.00);
                uint8_t StepFX = random(seqMod_value[7][6], seqMod_value[7][7]) * (barVelocity[external_clock_bar] / 127) * (mixGainPot / 127.00);
                note_is_on[0] = true;
                note_is_on[0] = true;
                sendControlChange(parameter[14], StepFX, parameter[SET_MIDICH_OUT]);
                noteOn(noteToPlay[0], Velo, parameter[SET_MIDICH_OUT]); // Send a Note (pitch 42, velo 127 on channel 1)
                // Serial.printf("ON   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
            }
        }
    }

    if (!seq4_bool)
    {

        if (note_is_on[0])
        {
            note_is_on[0] = false;
            noteOff(noteToPlay[0], 0, parameter[SET_MIDICH_OUT]); // Send a Note (pitch 42, velo 127 on channel 1)
                                                                  // Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
        }
    }
}
void Track::set_seq_mode7_parameters(uint8_t row)
{
    draw_seq_mode7();
    if (row == 0)
    {
        set_seq_mode7_value(0, 0, "Steps");
        set_seq_mode7_value(1, 0, "Offset");
        set_seq_mode7_value(2, 0, "Note -");
        set_seq_mode7_value(3, 0, "Note +");
    }
    if (row == 1)
    {
        set_seq_mode7_value(0, 1, "Velo -");
        set_seq_mode7_value(1, 1, "Velo +");
        set_seq_mode7_value(2, 1, "FX -");
        set_seq_mode7_value(3, 1, "FX +");
    }
    if (row == 2)
    {
        // set_seq_mode7_value(0, 2, "Velo -");
        // set_seq_mode7_value(1, 2, "Velo +");
        //  set_seq_mode7_value(2, 2, "FX -");
        //  set_seq_mode7_value(3, 2, "FX +");
    }
    if (row == 3)
    {
        //  set_seq_mode6_value(0, 3, "13");
        //  set_seq_mode6_value(1, 3, "14");
        //   set_seq_mode6_value(2, 3, "15");
        //   set_seq_mode6_value(3, 3, "16");
    }
}
void Track::set_seq_mode7_value(uint8_t XPos, uint8_t YPos, const char *name)
{
    int n = XPos + (YPos * NUM_ENCODERS);
    if (enc_moved[XPos])
    {

        seqMod_value[7][n] = constrain(seqMod_value[7][n] + encoded[XPos], 0, MIDI_CC_RANGE);
        drawPot(XPos, YPos, seqMod_value[7][n], name);

        if (n == 0)
        {
            int bucket = 0;
            int _numSteps = map(seqMod_value[7][0], 0, 127, 0, NUM_STEPS);
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
            _offset = seqMod_value[7][1];
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
        drawPot(0, 0, seqMod_value[7][0], "Steps");
        drawPot(1, 0, seqMod_value[7][1], "Offset");
        drawPot(2, 0, seqMod_value[7][2], "Note -");
        drawPot(3, 0, seqMod_value[7][3], "Note +");

        drawPot(0, 1, seqMod_value[7][4], "Velo -");
        drawPot(1, 1, seqMod_value[7][5], "Velo +");
        drawPot(2, 1, seqMod_value[7][6], "FX -");
        drawPot(3, 1, seqMod_value[7][7], "FX +");

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
