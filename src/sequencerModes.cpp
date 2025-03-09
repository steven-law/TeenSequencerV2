#include <Arduino.h>
#include "projectVariables.h"
#include "Track.h"

void Track::play_seq_mode0(uint8_t cloock)
{
    // Serial.printf("track= %d,tick: %d, array note= %d on voice: %d\n", my_Arranger_Y_axis, cloock, this->array[clip_to_play[internal_clock_bar]][cloock][0], 0);
    for (int v = 0; v < MAX_VOICES; v++)
    {

        if (get_note_parameter(clip[clip_to_play[external_clock_bar]].tick[cloock].voice, v) < NO_NOTE)
        {
            if (!note_is_on[v])
            {
                noteToPlay[v] = get_note_parameter(clip[clip_to_play[internal_clock_bar]].tick[cloock].voice, v);
                uint8_t Velo = get_note_parameter(clip[clip_to_play[internal_clock_bar]].tick[cloock].velo, v) * (barVelocity[external_clock_bar] / 127.00) * (mixGainPot / 127.00);
                uint8_t StepFX = get_note_parameter(&clip[clip_to_play[internal_clock_bar]].tick[cloock].stepFX, v);
                note_is_on[v] = true;
                sendControlChange(setStepFX, StepFX, parameter[SET_MIDICH_OUT]);
                noteOn(noteToPlay[v], Velo, parameter[SET_MIDICH_OUT]);
            }
        }
        if (get_note_parameter(clip[clip_to_play[external_clock_bar]].tick[cloock].voice, v) == NO_NOTE)
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

    if (get_note_parameter(clip[clip_to_play[external_clock_bar]].tick[cloock].voice, 0) < NO_NOTE)
    {
        if (!note_is_on[0])
        {
            if (random(127) > SeqMod1Value[4])
            {
                noteToPlay[0] = random(0, 11) + (random(SeqMod1Value[0], SeqMod1Value[1] + 1) * 12) + noteOffset[external_clock_bar] + performNoteOffset;
                uint8_t Velo = random(SeqMod1Value[2], SeqMod1Value[3]) * (barVelocity[external_clock_bar] / 127) * (mixGainPot / 127.00);
                note_is_on[0] = true;
                noteOn(noteToPlay[0], Velo, parameter[SET_MIDICH_OUT]); // Send a Note (pitch 42, velo 127 on channel 1)
                                                                        // Serial.printf("ON   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
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
{
    draw_seq_mode1();
    if (row == 0)
    {
        set_seq_mode1_value(0, 0, "Oct -");
        set_seq_mode1_value(1, 0, "Oct +");
        set_seq_mode1_value(2, 0, "Vol -");
        set_seq_mode1_value(3, 0, "Vol +");
    }
    if (row == 1)
    {

        set_seq_mode1_value(0, 1, "Prob");
    }
}
void Track::set_seq_mode1_value(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        int n = XPos + (YPos * NUM_ENCODERS);
        SeqMod1Value[n] = constrain(SeqMod1Value[n] + encoded[XPos], 0, MIDI_CC_RANGE);
        drawPot(XPos, YPos, SeqMod1Value[n], name);
    }
}
void Track::draw_seq_mode1()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, SeqMod1Value[0], "Oct -");
        drawPot(1, 0, SeqMod1Value[1], "Oct +");
        drawPot(2, 0, SeqMod1Value[2], "Vol -");
        drawPot(3, 0, SeqMod1Value[3], "Vol +");

        drawPot(0, 1, SeqMod1Value[4], "Prob");
    }
}
// dropseq
void Track::play_seq_mode2(uint8_t cloock)
{
    static uint8_t maxValIndex;
    static uint8_t analogReadArray[12];
    uint8_t cc23 = SeqMod2Value[2];
    uint8_t cc24 = SeqMod2Value[3];
    uint8_t thisOctave = 4;

    if (get_note_parameter(clip[clip_to_play[external_clock_bar]].tick[cloock].voice, 0) < NO_NOTE)
    {
        if (!note_is_on[0])
        {
            maxVal = 0;
            if (SeqMod2Value[2] <= SeqMod2Value[3])
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

            if (SeqMod2Value[2] > SeqMod2Value[3])
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
            uint8_t StepFX = get_note_parameter(&clip[clip_to_play[internal_clock_bar]].tick[cloock].stepFX, 0);
            // uint8_t Velo = get_active_velo(clip_to_play[internal_clock_bar], cloock, 0) * (barVelocity[external_clock_bar] / 127) * (mixGainPot / 127.00);
            // uint8_t StepFX = get_active_stepFX(clip_to_play[internal_clock_bar], cloock, 0);
            sendControlChange(setStepFX, StepFX, parameter[SET_MIDICH_OUT]);
            noteOn(noteToPlay[0], Velo, parameter[SET_MIDICH_OUT]);
            // Serial.print(track[i].notePlayed[0]);
            // Serial.print("--");

            analogReadArray[maxValIndex] = (analogReadArray[maxValIndex] - SeqMod2Value[0]);
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

    if (analogReadArray[maxValIndex] <= SeqMod2Value[1])
    {
        for (int i = 0; i < 12; i++)
        {
            analogReadArray[i] = SeqMod2Value[i + 4];
        }
    }
}
void Track::set_seq_mode2_parameters(uint8_t row)
{
    draw_seq_mode2();
    if (row == 0)
    {
        set_seq_mode2_value(0, 0, "Drop", 0, 0);
        set_seq_mode2_value(1, 0, "Rst @", 0, 0);
        set_seq_mode2_value(2, 0, "Oct -", 0, 0);
        set_seq_mode2_value(3, 0, "Oct +", 0, 0);
    }
    if (row == 1)
    {
        set_seq_mode2_value(0, 1, "C", 0, 0);
        set_seq_mode2_value(1, 1, "C#", 0, 0);
        set_seq_mode2_value(2, 1, "D", 0, 0);
        set_seq_mode2_value(3, 1, "D#", 0, 0);
    }
    if (row == 2)
    {
        set_seq_mode2_value(0, 2, "E", 0, 0);
        set_seq_mode2_value(1, 2, "F", 0, 0);
        set_seq_mode2_value(2, 2, "F#", 0, 0);
        set_seq_mode2_value(3, 2, "G", 0, 0);
    }
    if (row == 3)
    {
        set_seq_mode2_value(0, 3, "G#", 0, 0);
        set_seq_mode2_value(1, 3, "A", 0, 0);
        set_seq_mode2_value(2, 3, "A#", 0, 0);
        set_seq_mode2_value(3, 3, "B", 0, 0);
    }
}
void Track::set_seq_mode2_value(uint8_t XPos, uint8_t YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int n = XPos + (YPos * NUM_ENCODERS);
        SeqMod2Value[n] = constrain(SeqMod2Value[n] + encoded[XPos], 0, MIDI_CC_RANGE);
        drawPot(XPos, YPos, SeqMod2Value[n], name);
    }
}
void Track::draw_seq_mode2()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, SeqMod2Value[0], "Drop");
        drawPot(1, 0, SeqMod2Value[1], "Rst @");
        drawPot(2, 0, SeqMod2Value[2], "Oct -");
        drawPot(3, 0, SeqMod2Value[3], "Oct +");

        drawPot(0, 1, SeqMod2Value[4], "C");
        drawPot(1, 1, SeqMod2Value[5], "C#");
        drawPot(2, 1, SeqMod2Value[6], "D");
        drawPot(3, 1, SeqMod2Value[7], "D#");

        drawPot(0, 2, SeqMod2Value[8], "E");
        drawPot(1, 2, SeqMod2Value[9], "F");
        drawPot(2, 2, SeqMod2Value[10], "F#");
        drawPot(3, 2, SeqMod2Value[11], "G");

        drawPot(0, 3, SeqMod2Value[12], "G#");
        drawPot(1, 3, SeqMod2Value[13], "A");
        drawPot(2, 3, SeqMod2Value[14], "A#");
        drawPot(3, 3, SeqMod2Value[15], "B");
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
        if (bitRead(SeqMod3Value[v], seq3_clock))
        {
            if (!note_is_on[v])
            {
                noteToPlay[v] = v + (parameter[SET_OCTAVE] * 12) + noteOffset[external_clock_bar] + performNoteOffset;
                uint8_t Velo = get_note_parameter(clip[clip_to_play[internal_clock_bar]].tick[cloock].velo, v) * (barVelocity[external_clock_bar] / 127.00) * (mixGainPot / 127.00);
                uint8_t StepFX = get_note_parameter(&clip[clip_to_play[internal_clock_bar]].tick[cloock].stepFX, v);
                // uint8_t Velo = get_active_velo(clip_to_play[internal_clock_bar], cloock, v) * (barVelocity[external_clock_bar] / 127) * (mixGainPot / 127.00);
                // uint8_t StepFX = get_active_stepFX(clip_to_play[internal_clock_bar], cloock, v);
                note_is_on[v] = true;
                sendControlChange(setStepFX, StepFX, parameter[SET_MIDICH_OUT]);
                noteOn(noteToPlay[v], Velo, parameter[SET_MIDICH_OUT]); // Send a Note (pitch 42, velo 127 on channel 1)
                                                                        // Serial.printf("ON   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
            }
        }

        if (!bitRead(SeqMod3Value[v], seq3_clock))
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
        set_seq_mode3_value(0, 0, noteNames[0], 0, 0);
        set_seq_mode3_value(1, 0, noteNames[1], 0, 0);
        set_seq_mode3_value(2, 0, noteNames[2], 0, 0);
        set_seq_mode3_value(3, 0, noteNames[3], 0, 0);

        set_seq_mode3_value(0, 1, noteNames[4], 0, 0);
        set_seq_mode3_value(1, 1, noteNames[5], 0, 0);
        set_seq_mode3_value(2, 1, noteNames[6], 0, 0);
        set_seq_mode3_value(3, 1, noteNames[7], 0, 0);

        set_seq_mode3_value(0, 2, noteNames[8], 0, 0);
        set_seq_mode3_value(1, 2, noteNames[9], 0, 0);
        set_seq_mode3_value(2, 2, noteNames[10], 0, 0);
        set_seq_mode3_value(3, 2, noteNames[11], 0, 0);
    }
}
void Track::set_seq_mode3_value(uint8_t XPos, uint8_t YPos, const char *name, int min, int max)
{
    if (enc_moved[XPos])
    {
        int n = XPos + (YPos * NUM_ENCODERS);
        SeqMod3Value[n] = constrain(SeqMod3Value[n] + encoded[XPos], 0, MIDI_CC_RANGE * 2);
        drawPot(XPos, YPos, SeqMod3Value[n], name);
    }
}
void Track::draw_seq_mode3()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, SeqMod3Value[0], noteNames[0]);
        drawPot(1, 0, SeqMod3Value[1], noteNames[1]);
        drawPot(2, 0, SeqMod3Value[2], noteNames[2]);
        drawPot(3, 0, SeqMod3Value[3], noteNames[3]);

        drawPot(0, 1, SeqMod3Value[4], noteNames[4]);
        drawPot(1, 1, SeqMod3Value[5], noteNames[5]);
        drawPot(2, 1, SeqMod3Value[6], noteNames[6]);
        drawPot(3, 1, SeqMod3Value[7], noteNames[7]);

        drawPot(0, 2, SeqMod3Value[8], noteNames[8]);
        drawPot(1, 2, SeqMod3Value[9], noteNames[9]);
        drawPot(2, 2, SeqMod3Value[10], noteNames[10]);
        drawPot(3, 2, SeqMod3Value[11], noteNames[11]);
    }
}
// 16 step pot sequencer
void Track::play_seq_mode4(uint8_t cloock)
{
    uint8_t seq3_clock = cloock / TICKS_PER_STEP;
    bool seq4_bool = cloock % 6;
    if (seq3_clock == 16)
        seq3_clock = 0;

    if (seq4_bool)
    {
        if (!note_is_on[0])
        {
            noteToPlay[0] = SeqMod4Value[seq3_clock] + noteOffset[external_clock_bar] + performNoteOffset;
            uint8_t Velo = 99 * (barVelocity[external_clock_bar] / 127) * (mixGainPot / 127.00);
            uint8_t StepFX = 128;
            note_is_on[0] = true;
            sendControlChange(setStepFX, StepFX, parameter[SET_MIDICH_OUT]);
            noteOn(noteToPlay[0], Velo, parameter[SET_MIDICH_OUT]); // Send a Note (pitch 42, velo 127 on channel 1)
                                                                    // Serial.printf("ON   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
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
void Track::set_seq_mode4_parameters(uint8_t row)
{
    draw_seq_mode4();
    if (row == 0)
    {
        set_seq_mode4_value(0, 0, "1");
        set_seq_mode4_value(1, 0, "2");
        set_seq_mode4_value(2, 0, "3");
        set_seq_mode4_value(3, 0, "4");
    }
    if (row == 1)
    {
        set_seq_mode4_value(0, 1, "5");
        set_seq_mode4_value(1, 1, "6");
        set_seq_mode4_value(2, 1, "7");
        set_seq_mode4_value(3, 1, "8");
    }
    if (row == 2)
    {
        set_seq_mode4_value(0, 2, "9");
        set_seq_mode4_value(1, 2, "10");
        set_seq_mode4_value(2, 2, "11");
        set_seq_mode4_value(3, 2, "12");
    }
    if (row == 3)
    {
        set_seq_mode4_value(0, 3, "13");
        set_seq_mode4_value(1, 3, "14");
        set_seq_mode4_value(2, 3, "15");
        set_seq_mode4_value(3, 3, "16");
    }
}
void Track::set_seq_mode4_value(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        int n = XPos + (YPos * NUM_ENCODERS);
        SeqMod4Value[n] = constrain(SeqMod4Value[n] + encoded[XPos], 0, MIDI_CC_RANGE);
        drawPot(XPos, YPos, SeqMod4Value[n], name);
    }
}
void Track::draw_seq_mode4()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, SeqMod4Value[0], "1");
        drawPot(1, 0, SeqMod4Value[1], "2");
        drawPot(2, 0, SeqMod4Value[2], "3");
        drawPot(3, 0, SeqMod4Value[3], "4");

        drawPot(0, 1, SeqMod4Value[4], "5");
        drawPot(1, 1, SeqMod4Value[5], "6");
        drawPot(2, 1, SeqMod4Value[6], "7");
        drawPot(3, 1, SeqMod4Value[7], "8");

        drawPot(0, 2, SeqMod4Value[8], "9");
        drawPot(1, 2, SeqMod4Value[9], "10");
        drawPot(2, 2, SeqMod4Value[10], "11");
        drawPot(3, 2, SeqMod4Value[11], "12");

        drawPot(0, 3, SeqMod4Value[12], "13");
        drawPot(1, 3, SeqMod4Value[13], "14");
        drawPot(2, 3, SeqMod4Value[14], "15");
        drawPot(3, 3, SeqMod4Value[15], "16");
    }
}

// beatArray
void Track::play_seq_mode5(uint8_t cloock)
{
    uint8_t seq3_clock = cloock / TICKS_PER_STEP;
    bool seq4_bool = cloock % TICKS_PER_STEP;
    if (seq3_clock == 16)
        seq3_clock = 0;

    if (seq4_bool)
    {
        for (int i = 0; i < MAX_VOICES; i++)
        {
            if (beatArray[SeqMod5Value[i]][seq3_clock])
            {

                if (!note_is_on[i])
                {
                    noteToPlay[i] = (i + (parameter[SET_OCTAVE] * NOTES_PER_OCTAVE)) + noteOffset[external_clock_bar] + performNoteOffset;
                    uint8_t Velo = 99 * (barVelocity[external_clock_bar] / 127) * (mixGainPot / 127.00);
                    uint8_t StepFX = 128;
                    note_is_on[i] = true;
                    sendControlChange(setStepFX, StepFX, parameter[SET_MIDICH_OUT]);
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
void Track::set_seq_mode5_parameters(uint8_t row)
{
    draw_seq_mode5();
    if (row == 0)
    {
        set_seq_mode5_value(0, 0, "1");
        set_seq_mode5_value(1, 0, "2");
        set_seq_mode5_value(2, 0, "3");
        set_seq_mode5_value(3, 0, "4");
    }
    if (row == 1)
    {
        set_seq_mode5_value(0, 1, "5");
        set_seq_mode5_value(1, 1, "6");
        set_seq_mode5_value(2, 1, "7");
        set_seq_mode5_value(3, 1, "8");
    }
    if (row == 2)
    {
        set_seq_mode5_value(0, 2, "9");
        set_seq_mode5_value(1, 2, "10");
        set_seq_mode5_value(2, 2, "11");
        set_seq_mode5_value(3, 2, "12");
    }
    if (row == 3)
    {
        //  set_seq_mode5_value(0, 3, "13");
        //  set_seq_mode5_value(1, 3, "14");
        //   set_seq_mode5_value(2, 3, "15");
        //   set_seq_mode5_value(3, 3, "16");
    }
}
void Track::set_seq_mode5_value(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        int n = XPos + (YPos * NUM_ENCODERS);
        SeqMod5Value[n] = constrain(SeqMod5Value[n] + encoded[XPos], 0, MIDI_CC_RANGE);
        drawPot(XPos, YPos, SeqMod5Value[n], name);
    }
}
void Track::draw_seq_mode5()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, SeqMod5Value[0], "1");
        drawPot(1, 0, SeqMod5Value[1], "2");
        drawPot(2, 0, SeqMod5Value[2], "3");
        drawPot(3, 0, SeqMod5Value[3], "4");

        drawPot(0, 1, SeqMod5Value[4], "5");
        drawPot(1, 1, SeqMod5Value[5], "6");
        drawPot(2, 1, SeqMod5Value[6], "7");
        drawPot(3, 1, SeqMod5Value[7], "8");

        drawPot(0, 2, SeqMod5Value[8], "9");
        drawPot(1, 2, SeqMod5Value[9], "10");
        drawPot(2, 2, SeqMod5Value[10], "11");
        drawPot(3, 2, SeqMod5Value[11], "12");

        //  drawPot(0, 3, SeqMod4Value[12], "13");
        //   drawPot(1, 3, SeqMod4Value[13], "14");
        //   drawPot(2, 3, SeqMod4Value[14], "15");
        //   drawPot(3, 3, SeqMod4Value[15], "16");
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
                    sendControlChange(setStepFX, StepFX, parameter[SET_MIDICH_OUT]);
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

        SeqMod6Value[n] = constrain(SeqMod6Value[n] + encoded[XPos], 0, MIDI_CC_RANGE);
        drawPot(XPos, YPos, SeqMod6Value[n], name);
        int bucket = 0;
        int _numSteps = map(SeqMod6Value[n], 0, 127, 0, NUM_STEPS);
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
        //  int _numSteps = map(SeqMod6Value[n], 0, 127, 0, NUM_STEPS);
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
        drawPot(0, 0, SeqMod6Value[0], "1");
        drawPot(1, 0, SeqMod6Value[1], "2");
        drawPot(2, 0, SeqMod6Value[2], "3");
        drawPot(3, 0, SeqMod6Value[3], "4");

        drawPot(0, 1, SeqMod6Value[4], "5");
        drawPot(1, 1, SeqMod6Value[5], "6");
        drawPot(2, 1, SeqMod6Value[6], "7");
        drawPot(3, 1, SeqMod6Value[7], "8");

        drawPot(0, 2, SeqMod6Value[8], "9");
        drawPot(1, 2, SeqMod6Value[9], "10");
        drawPot(2, 2, SeqMod6Value[10], "11");
        drawPot(3, 2, SeqMod6Value[11], "12");

        //  drawPot(0, 3, SeqMod4Value[12], "13");
        //   drawPot(1, 3, SeqMod4Value[13], "14");
        //   drawPot(2, 3, SeqMod4Value[14], "15");
        //   drawPot(3, 3, SeqMod4Value[15], "16");
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
                noteToPlay[0] = random(SeqMod7Value[2], SeqMod7Value[3]) + noteOffset[external_clock_bar] + performNoteOffset;
                uint8_t Velo = random(SeqMod7Value[4], SeqMod7Value[5]) * (barVelocity[external_clock_bar] / 127) * (mixGainPot / 127.00);
                uint8_t StepFX = random(SeqMod7Value[6], SeqMod7Value[7]) * (barVelocity[external_clock_bar] / 127) * (mixGainPot / 127.00);
                note_is_on[0] = true;
                note_is_on[0] = true;
                sendControlChange(setStepFX, StepFX, parameter[SET_MIDICH_OUT]);
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

        SeqMod7Value[n] = constrain(SeqMod7Value[n] + encoded[XPos], 0, MIDI_CC_RANGE);
        drawPot(XPos, YPos, SeqMod7Value[n], name);

        if (n == 0)
        {
            int bucket = 0;
            int _numSteps = map(SeqMod7Value[0], 0, 127, 0, NUM_STEPS);
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
            // int _numSteps = map(SeqMod7Value[0], 0, 127, 0, NUM_STEPS);
            int _offset;
            _offset = SeqMod7Value[1];
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
        drawPot(0, 0, SeqMod7Value[0], "Steps");
        drawPot(1, 0, SeqMod7Value[1], "Offset");
        drawPot(2, 0, SeqMod7Value[2], "Note -");
        drawPot(3, 0, SeqMod7Value[3], "Note +");

        drawPot(0, 1, SeqMod7Value[4], "Velo -");
        drawPot(1, 1, SeqMod7Value[5], "Velo +");
        drawPot(2, 1, SeqMod7Value[6], "FX -");
        drawPot(3, 1, SeqMod7Value[7], "FX +");

        // drawPot(0, 2, SeqMod7Value[8], "Velo -");
        //  drawPot(1, 2, SeqMod7Value[9], "Velo +");
        //  drawPot(2, 2, SeqMod7Value[10], "FX -");
        //  drawPot(3, 2, SeqMod7Value[11], "FX +");

        //  drawPot(0, 3, SeqMod4Value[12], "13");
        //   drawPot(1, 3, SeqMod4Value[13], "14");
        //   drawPot(2, 3, SeqMod4Value[14], "15");
        //   drawPot(3, 3, SeqMod4Value[15], "16");
    }
}
