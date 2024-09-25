#include <Arduino.h>
#include "projectVariables.h"
#include "Track.h"

void Track::play_seq_mode0(byte cloock)
{
    // Serial.printf("track= %d,tick: %d, array note= %d on voice: %d\n", my_Arranger_Y_axis, cloock, this->array[clip_to_play[internal_clock_bar]][cloock][0], 0);
    for (int v = 0; v < MAX_VOICES; v++)
    {

        if (get_active_note(clip_to_play[external_clock_bar], cloock, v) < NO_NOTE)
        {
            if (!note_is_on[v])
            {
                //Serial.println("send Note");
                noteToPlay[v] = get_active_note(clip_to_play[internal_clock_bar], cloock, v) + noteOffset[external_clock_bar] + performNoteOffset;
                byte Velo = get_active_velo(clip_to_play[internal_clock_bar], cloock, v) * (barVelocity[external_clock_bar] / 127.00) * (mixGainPot/127.00);
                byte StepFX = get_active_stepFX(clip_to_play[internal_clock_bar], cloock, v);
                note_is_on[v] = true;
                sendControlChange(setStepFX, StepFX, parameter[SET_MIDICH_OUT]);
                
               // Serial.printf("get_note: %d + offset %d = %d\n", get_active_note(clip_to_play[internal_clock_bar], cloock, v),noteOffset[external_clock_bar],  noteToPlay[v] );
                noteOn(noteToPlay[v], Velo, parameter[SET_MIDICH_OUT]);
            }
        }
        if (get_active_note(clip_to_play[external_clock_bar], cloock, v) == NO_NOTE)
        {
            if (note_is_on[v])
            {
                note_is_on[v] = false;
                noteOff(noteToPlay[v], 0, parameter[SET_MIDICH_OUT]);

                // Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, v, noteToPlay[v]);
            }
        }
    }
}
// random
void Track::play_seq_mode1(byte cloock)
{

    if (get_active_note(clip_to_play[external_clock_bar], cloock, 0) < NO_NOTE)
    {
        if (!note_is_on[0])
        {
            noteToPlay[0] = random(0, 11) + (random(SeqMod1Value[0], SeqMod1Value[1] + 1) * 12) + noteOffset[external_clock_bar]+ performNoteOffset;
            byte Velo = random(SeqMod1Value[2], SeqMod1Value[3]) * (barVelocity[external_clock_bar] / 127)* (mixGainPot/127.00);
            note_is_on[0] = true;
            noteOn(noteToPlay[0], Velo, parameter[SET_MIDICH_OUT]); // Send a Note (pitch 42, velo 127 on channel 1)
                                                                    // Serial.printf("ON   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
        }
    }

    if (get_active_note(clip_to_play[external_clock_bar], cloock, 0) == NO_NOTE)
    {
        if (note_is_on[0])
        {
            note_is_on[0] = false;
            noteOff(noteToPlay[0], 0, parameter[SET_MIDICH_OUT]); // Send a Note (pitch 42, velo 127 on channel 1)
                                                                  // Serial.printf("OFF   tick: %d, voice: %d, note: %d\n", cloock, 0, noteToPlay[0]);
        }
    }
}
void Track::set_seq_mode1_parameters(byte row)
{
    draw_seq_mode1();
    if (row == 0)
    {
        set_seq_mode1_value(0, 0, "Oct -");
        set_seq_mode1_value(1, 0, "Oct +");
        set_seq_mode1_value(2, 0, "Vol -");
        set_seq_mode1_value(3, 0, "Vol +");
    }
}
void Track::set_seq_mode1_value(byte XPos, byte YPos, const char *name)
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
    }
}
// dropseq
void Track::play_seq_mode2(byte cloock)
{
    static byte maxValIndex;
    static byte analogReadArray[12];
    byte cc23 = SeqMod2Value[2];
    byte cc24 = SeqMod2Value[3];
    byte thisOctave;

    if (get_active_note(clip_to_play[internal_clock_bar], cloock, 0) < NO_NOTE)
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
            noteToPlay[0] = (maxValIndex) + (thisOctave * 12) + noteOffset[external_clock_bar]+ performNoteOffset;
            byte Velo = get_active_velo(clip_to_play[internal_clock_bar], cloock, 0) * (barVelocity[external_clock_bar] / 127)* (mixGainPot/127.00);
            byte StepFX = get_active_stepFX(clip_to_play[internal_clock_bar], cloock, 0);
            sendControlChange(setStepFX, StepFX, parameter[SET_MIDICH_OUT]);
            noteOn(noteToPlay[0], Velo, parameter[SET_MIDICH_OUT]);
            // Serial.print(track[i].notePlayed[0]);
            // Serial.print("--");

            analogReadArray[maxValIndex] = (analogReadArray[maxValIndex] - SeqMod2Value[0]);
            // Serial.println(maxValIndex);
        }
    }
    // NoteOff
    if (get_active_note(clip_to_play[internal_clock_bar], cloock, 0) == NO_NOTE)
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
void Track::set_seq_mode2_parameters(byte row)
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
void Track::set_seq_mode2_value(byte XPos, byte YPos, const char *name, int min, int max)
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
void Track::play_seq_mode3(byte cloock)
{
    byte seq3_clock = cloock / 6;
    if (seq3_clock == 8)
        seq3_clock = 0;
    for (int v = 0; v < MAX_VOICES; v++)
    {
        if (bitRead(SeqMod3Value[v], seq3_clock))
        {
            if (!note_is_on[v])
            {
                noteToPlay[v] = v + (parameter[SET_OCTAVE] * 12) + noteOffset[external_clock_bar]+ performNoteOffset;

                byte Velo = get_active_velo(clip_to_play[internal_clock_bar], cloock, v) * (barVelocity[external_clock_bar] / 127)* (mixGainPot/127.00);
                byte StepFX = get_active_stepFX(clip_to_play[internal_clock_bar], cloock, v);
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
void Track::set_seq_mode3_parameters(byte row)
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
void Track::set_seq_mode3_value(byte XPos, byte YPos, const char *name, int min, int max)
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
void Track::play_seq_mode4(byte cloock)
{
    byte seq3_clock = cloock / 6;
    bool seq4_bool = cloock % 6;
    if (seq3_clock == 16)
        seq3_clock = 0;

    if (seq4_bool)
    {
        if (!note_is_on[0])
        {
            noteToPlay[0] = SeqMod4Value[seq3_clock] + noteOffset[external_clock_bar]+ performNoteOffset;
            byte Velo = 99 * (barVelocity[external_clock_bar] / 127)* (mixGainPot/127.00);
            byte StepFX = 99;
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
void Track::set_seq_mode4_parameters(byte row)
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
void Track::set_seq_mode4_value(byte XPos, byte YPos, const char *name)
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