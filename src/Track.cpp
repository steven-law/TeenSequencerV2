#include <Arduino.h>
#include "Track.h"

// File myFile;
//  MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI1);
//   midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> *midi_out_serial = &MIDI1;

void Track::update(int PixelX, uint8_t gridY)
{
    // MIDI1.read();

    bar_to_edit = ((PixelX - SEQ_GRID_LEFT) / STEP_FRAME_W) + (BARS_PER_PAGE * (arrangerpage));
    tick_to_edit = (PixelX - SEQ_GRID_LEFT) / PIXEL_PER_TICK;
    voice_to_edit = gridY - 1;
    // save_track();
    // load_track();
}
void Track::save_track(uint8_t songNr)
{
    char filename[20];
    sprintf(filename, "%02d_%02d.dat", songNr, my_Arranger_Y_axis);

    if (SD.exists(filename))
        SD.remove(filename);
    myTrackFile = SD.open(filename, FILE_WRITE);
    if (!myTrackFile)
        return;

    uint16_t color = trackColor[my_Arranger_Y_axis - 1];
    myTrackFile.write((uint8_t *)&color, sizeof(color));
    for (int c = 0; c < MAX_CLIPS; c++)
    {
        myTrackFile.write(clip[c].seqLength);
        myTrackFile.write(clip[c].clockDivision);
        myTrackFile.write(clip[c].playMode);
        myTrackFile.write(clip[c].scale);
        myTrackFile.write(clip[c].midiChOut);
        for (int t = 0; t < MAX_TICKS; t++)
        {
            for (int v = 0; v < MAX_VOICES; v++)
            {
                myTrackFile.write(clip[c].tick[t].voice[v]);
                myTrackFile.write(clip[c].tick[t].velo[v]);
                myTrackFile.write(clip[c].tick[t].stepFX);
                myTrackFile.write(clip[c].tick[t].noteLength[v]);
                myTrackFile.write(clip[c].tick[t].startTick[v]);
            }
        }
    }

    for (int i = 0; i < MAX_BARS; i++)
    {
        myTrackFile.write(clip_to_play[i]);
        myTrackFile.write(noteOffset[i]);
        myTrackFile.write(barVelocity[i]);
        myTrackFile.write(barProbabilty[i]);
        myTrackFile.write(play_presetNr_Playmode_ccChannel[i]);
        myTrackFile.write(play_presetNr_Plugin_ccValue[i]);
    }
    for (int t = 0; t < NUM_PARAMETERS; t++)
    {
        myTrackFile.write(parameter[t]);
        for (int p = 0; p < NUM_PRESETS + 1; p++)
        {
            myTrackFile.write(CCchannel[p][t]);
            myTrackFile.write(CCvalue[p][t]);
            for (int m = 0; m < NUM_PLAYMODES; m++)
            {
                myTrackFile.write(seqMod_value[m][p][t]);
            }
        }
    }

    myTrackFile.write(mixGainPot);
    myTrackFile.write(mixDryPot);
    myTrackFile.write(mixFX1Pot);
    myTrackFile.write(mixFX2Pot);
    myTrackFile.write(mixFX3Pot);
    myTrackFile.write((uint8_t)(performNoteOffset + 64));

    myTrackFile.flush();
    myTrackFile.close();
    Serial.printf("saved track: %d\n", my_Arranger_Y_axis - 1);
    if (parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
    {
        save_plugin(songNr, parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1));
    }
}
void Track::load_track(uint8_t songNr)
{
    char filename[20];
    sprintf(filename, "%02d_%02d.dat", songNr, my_Arranger_Y_axis);

    myTrackFile = SD.open(filename, FILE_READ);
    if (!myTrackFile)
        return;
    uint16_t color;
    myTrackFile.read((uint8_t *)&color, sizeof(color));
    trackColor[my_Arranger_Y_axis - 1] = color;
    trellisTrackColor[my_Arranger_Y_axis - 1] = rgb565to24(color);
    for (int c = 0; c < MAX_CLIPS; c++)
    {
        clip[c].seqLength = myTrackFile.read();
        clip[c].clockDivision = myTrackFile.read();
        clip[c].playMode = myTrackFile.read();
        clip[c].scale = myTrackFile.read();
        clip[c].midiChOut = myTrackFile.read();
        for (int t = 0; t < MAX_TICKS; t++)
        {
            for (int v = 0; v < MAX_VOICES; v++)
            {
                clip[c].tick[t].voice[v] = myTrackFile.read();
                clip[c].tick[t].velo[v] = myTrackFile.read();
                clip[c].tick[t].stepFX = myTrackFile.read();
                clip[c].tick[t].noteLength[v] = myTrackFile.read();
                clip[c].tick[t].startTick[v] = myTrackFile.read();
                if (clip[c].tick[t].voice[v] < NO_NOTE)
                {
                    trellisOut.set_main_buffer(c, (t / TICKS_PER_STEP), (my_Arranger_Y_axis - 1), trellisTrackColor[my_Arranger_Y_axis - 1]);
                }
            }
        }
    }

    for (int i = 0; i < MAX_BARS; i++)
    {
        clip_to_play[i] = myTrackFile.read();
        noteOffset[i] = myTrackFile.read();
        barVelocity[i] = myTrackFile.read();
        barProbabilty[i] = myTrackFile.read();
        play_presetNr_Playmode_ccChannel[i] = myTrackFile.read();
        play_presetNr_Plugin_ccValue[i] = myTrackFile.read();
        if (clip_to_play[i] <= NUM_USER_CLIPS)
        {
            // Serial.println((i / 16) + TRELLIS_SCREEN_ARRANGER_1);
            trellisOut.set_main_buffer((i / 16) + TRELLIS_SCREEN_ARRANGER_1, i % 16, (my_Arranger_Y_axis - 1), trellisTrackColor[my_Arranger_Y_axis - 1] + (clip_to_play[i] * 20));
        }
    }
    for (int t = 0; t < NUM_PARAMETERS; t++)
    {
        parameter[t] = myTrackFile.read();
        for (int p = 0; p < NUM_PRESETS + 1; p++)
        {
            CCchannel[p][t] = myTrackFile.read();
            CCvalue[p][t] = myTrackFile.read();
            for (int m = 0; m < NUM_PLAYMODES; m++)
            {
                seqMod_value[m][p][t] = myTrackFile.read();
            }
        }
        trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER1, t, my_Arranger_Y_axis - 1, TRELLIS_BLACK);
        trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER, t, my_Arranger_Y_axis - 1, TRELLIS_BLACK);
    }

    mixGainPot = myTrackFile.read();
    trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER1, mixGainPot / 8, my_Arranger_Y_axis - 1, trellisTrackColor[my_Arranger_Y_axis - 1]);
    mixDryPot = myTrackFile.read();
    trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER, mixGainPot / 42, my_Arranger_Y_axis - 1, TRELLIS_PINK);
    mixFX1Pot = myTrackFile.read();
    trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER, (mixGainPot / 42) + 4, my_Arranger_Y_axis - 1, TRELLIS_OLIVE);
    mixFX2Pot = myTrackFile.read();
    trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER, (mixGainPot / 42) + 8, my_Arranger_Y_axis - 1, TRELLIS_AQUA);
    mixFX3Pot = myTrackFile.read();
    trellisOut.set_main_buffer(TRELLIS_SCREEN_MIXER, (mixGainPot / 42) + 12, my_Arranger_Y_axis - 1, TRELLIS_ORANGE);
    performNoteOffset = (int8_t)(myTrackFile.read()) - 64;
    myTrackFile.close();
    Serial.printf("loaded track: %d\n", my_Arranger_Y_axis - 1);
    if (parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
    {
        load_plugin(songNr, parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1));
    }
}

void Track::play_sequencer_mode(uint8_t cloock, uint8_t start, uint8_t end)
{
    // if (cloock == 0) // fehler hier
    {
        external_clock_bar = myClock.barTick;
    }
    if (cloock % (clip[clip_to_play[external_clock_bar]].clockDivision + performClockDivision) == 0)
    {
        internal_clock++;
        if (internal_clock >= constrain(clip[clip_to_play[external_clock_bar]].seqLength / performSeqLengthDiv, 6, MAX_TICKS))
        {
            internal_clock = 0;
        }

        if (internal_clock == 0)
        {
            internal_clock_bar++;
            change_presets();
            if (internal_clock_bar % clip[clip_to_play[external_clock_bar]].clockDivision == 0)
                internal_clock_bar = external_clock_bar;
            // Serial.printf("seqlength: %d\n", clip[clip_to_play[external_clock_bar]].seqLength);
            // Serial.printf(" track: %d, clip: %d\n", my_Arranger_Y_axis - 1, clip_to_play[external_clock_bar]);
        }
        if (internal_clock_bar >= end)
            internal_clock_bar = start;
        internal_clock_is_on = true;
    }
    else
        internal_clock_is_on = false;

    //  if (external_clock_bar >= end)
    //      external_clock_bar = start;

    // if (my_Arranger_Y_axis == 1)
    // {
    //   Serial.printf("internal bar:  %d, external bar:  %d\n", internal_clock_bar, external_clock_bar);
    //     Serial.printf("internal tick: %d, external tick: %d\n", internal_clock, cloock);
    //     Serial.println();
    // }
    //   Serial.println(internal_clock_bar);
    if (internal_clock_is_on)
    {
        if (clip_to_play[external_clock_bar] <= NUM_USER_CLIPS)
        {

            {
                // Serial.printf("internalbar=%d, externalbar= %d\n",internal_clock_bar,external_clock_bar );
                // Serial.printf("internalClock=%d, externalClock= %d\n", internal_clock, cloock);
                switch (clip[clip_to_play[internal_clock_bar]].playMode)
                {
                case 0:
                    play_seq_mode0(internal_clock);
                    break;
                case 1:
                    play_seq_mode1(internal_clock);
                    break;
                case 2:
                    play_seq_mode2(internal_clock);
                    break;
                case 3:
                    play_seq_mode3(internal_clock);
                    break;
                case 4:
                    play_seq_mode4(internal_clock);
                    break;
                case 5:
                    play_seq_mode5(internal_clock);
                    break;
                case 6:
                    play_seq_mode6(internal_clock);
                    break;
                case 7:
                    play_seq_mode7(internal_clock);
                    break;
                case 8:
                    play_seq_mode8(internal_clock);
                    break;
                case 9:
                    play_seq_mode9(internal_clock);
                    break;
                case 10:
                    play_seq_mode10(internal_clock);
                    break;
                }
            }
        }
    }
    // }
}

void Track::set_seq_mode_parameters(uint8_t row)
{
    set_seqmode_parameters(clip[parameter[SET_CLIP2_EDIT]].playMode);
}
void Track::draw_sequencer_modes(uint8_t mode)
{
    change_plugin_row = true;
    if (clip[parameter[SET_CLIP2_EDIT]].playMode > 0)
        draw_seq_mode(clip[parameter[SET_CLIP2_EDIT]].playMode);
}

void Track::noteOn(uint8_t Note, uint8_t Velo, uint8_t Channel)
{
    // Serial.printf("sending noteOn: %d, velo: %d channel: %d\n", Note, Velo, Channel);

    {
        if (!muted && !muteThruSolo)
        {
            sendNoteOn(my_Arranger_Y_axis - 1, Note, Velo, Channel);
            if (recordState)
                record_noteOn(Note, Velo, Channel);
        }
    }
}
void Track::noteOff(uint8_t Note, uint8_t Velo, uint8_t Channel)
{
    // Serial.printf("sending noteOff: %d, velo: %d channel: %d\n", Note, Velo, Channel);
    // if (!muted && !muteThruSolo)
    {
        sendNoteOff(my_Arranger_Y_axis - 1, Note, Velo, Channel);
        if (recordState)
            record_noteOff(Note, Velo, Channel);
    }
}
void Track::record_noteOn(uint8_t Note, uint8_t Velo, uint8_t Channel)
{
    recordVoice = Note % NOTES_PER_OCTAVE;
    recordLastNote[recordVoice] = Note;
    recordVelocity[recordVoice] = Velo;
    recordStartTick[recordVoice] = internal_clock;
}
void Track::record_noteOff(uint8_t Note, uint8_t Velo, uint8_t Channel)
{
    recordVoice = Note % NOTES_PER_OCTAVE;
    if (recordLastNote[recordVoice] == Note)
    {
        int _length;
        // int _velo = recordVelocity[recordVoice];
        // int _voice = recordVoice;
        int _start = recordStartTick[recordVoice];
        clip[parameter[SET_CLIP2_EDIT]].tick[_start].velo[recordVoice] = recordVelocity[recordVoice];
        if (_start < internal_clock)
            _length = internal_clock - _start;
        else
            for (int i = recordStartTick[recordVoice]; i < MAX_TICKS; i++)
            {

                // clip[parameter[SET_CLIP2_EDIT]].tick[recordStartTick[recordVoice]].startTick[recordVoice] = recordStartTick[recordVoice];
                // clip[parameter[SET_CLIP2_EDIT]].tick[i].voice[recordVoice] = Note;
                // clip[parameter[SET_CLIP2_EDIT]].tick[i].velo[recordVoice] = recordVelocity[recordVoice];
                // clip[parameter[SET_CLIP2_EDIT]].tick[recordStartTick[recordVoice]].noteLength[recordVoice] =
                _length = i;
                //  _length =
                //  (internal_clock + recordStartTick[recordVoice] >= (MAX_TICKS - 1)) ? ((MAX_TICKS - 1) - recordStartTick[recordVoice]) : i;

                Serial.printf("recorded notelength: %d\n", _length);
            }
        set_note_on_tick(_start, Note, _length);
        // set_note_on_tick(clip[parameter[SET_CLIP2_EDIT]].tick[recordStartTick[recordVoice]].startTick[recordVoice], Note, clip[parameter[SET_CLIP2_EDIT]].tick[recordStartTick[recordVoice]].noteLength[recordVoice]);
    }
}
//---------------------------arranger stuff-------------------------------------
void Track::set_arranger_parameters()
{

    switch (lastPotRow)
    {
    case 0:
        encoder_SetCursor(STEP_FRAME_W, 8); // Encoder: 0,1
        set_bar_parameter(2, pixelTouchX, clip_to_play, 0, NUM_USER_CLIPS + 1, "Clip");
        set_bar_parameter(3, pixelTouchX, noteOffset, -99, 99, "Trns");
        break;
    case 1:
        encoder_SetCursor(STEP_FRAME_W, 8); // Encoder: 0,1
        set_bar_parameter(2, pixelTouchX, barProbabilty, 0, 127, "Prob");
        set_bar_parameter(3, pixelTouchX, barVelocity, 0, 127, "Velo");
        break;
    case 2:
        encoder_SetCursor(STEP_FRAME_W, 8); // Encoder: 0,1
        set_bar_parameter(2, pixelTouchX, play_presetNr_Playmode_ccChannel, 0, NUM_PRESETS, "PM/ccC");
        set_bar_parameter(3, pixelTouchX, play_presetNr_Plugin_ccValue, 0, NUM_PRESETS, "Pl/ccV");
        break;
    case 3:
        // --
        // tempo
        // loopStart
        // loopEnd
        break;
    default:
        break;
    }
    copy_bar();
    draw_arranger_parameters();
}
void Track::set_bar_parameter(uint8_t _encoder, int b, int *parameterArray, int minValue, int maxValue, const char *label)
{
    if (myClock.isPlaying && bar_to_edit == myClock.barTick)
        return;
    if (bar_to_edit < 0)
        return;
    if (gridTouchY == my_Arranger_Y_axis)
    {
        uint8_t when = (((b - SEQ_GRID_LEFT) / STEP_FRAME_W) + (BARS_PER_PAGE * arrangerpage));
        if (enc_moved[_encoder])
        {
            int _when = constrain(parameterArray[when] + encoded[_encoder], minValue, maxValue);
            parameterArray[when] = _when;
            for (int i = 0; i < clip[clip_to_play[_when]].clockDivision; i++)
            {
                if (enc_moved[2] && lastPotRow == 0)
                    clip_to_play[when + i] = _when;
                draw_arrangment_line(my_Arranger_Y_axis - 1, when + i);
            }

            bar_for_copying = bar_to_edit;
            Serial.printf("Set %s: %d, when: %d\n", label, parameterArray[when], when);
        }
    }
}
void Track::change_presets() // change presets, happens when the next bar starts
{
    for (int i = 0; i < 16; i++)
    {
        if (CCchannel[play_presetNr_Playmode_ccChannel[internal_clock_bar]][i] < 128)
        {
            int trackChannel = clip[clip_to_play[internal_clock_bar]].midiChOut;
            Serial.print("cc:");
            Serial.println(CCchannel[play_presetNr_Playmode_ccChannel[internal_clock_bar]][i]);
            sendControlChange(CCchannel[play_presetNr_Playmode_ccChannel[internal_clock_bar]][i], CCvalue[play_presetNr_Plugin_ccValue[internal_clock_bar]][i], trackChannel);
            Serial.printf("Trackbar Track: %d,clip2play: %d externalbar: %d internalBar: %d\n", my_Arranger_Y_axis - 1, clip_to_play[external_clock_bar], external_clock_bar, internal_clock_bar);
        }
    }
    refresh_mode8();
}
void Track::clear_arrangment()
{
    if (gridTouchY == my_Arranger_Y_axis)
    {
        for (int i = 0; i < MAX_BARS; i++)
        {
            clip_to_play[i] = MAX_CLIPS - 1;
            noteOffset[i] = 0;
            barVelocity[i] = 0;
            barProbabilty[i] = 128;
            play_presetNr_Playmode_ccChannel[i] = NUM_PRESETS;
            play_presetNr_Plugin_ccValue[bar_to_edit + i] = NUM_PRESETS;
        }
        draw_arrangment_lines(my_Arranger_Y_axis - 1, arrangerpage);
    }
}
void Track::copy_bar() // copy the last edited barParameters to the desired bar (clip, Transpose, Veleocity, ccChannel, ccValue)
{
    if (neotrellisPressed[TRELLIS_BUTTON_ENTER])
    {
        // for (int i = 0; i < clip[parameter[SET_CLIP2_EDIT]].clockDivision; i++)
        int i = 0;
        {
            clip_to_play[bar_to_edit + i] = clip_to_play[bar_for_copying];
            noteOffset[bar_to_edit + i] = noteOffset[bar_for_copying];
            barVelocity[bar_to_edit + i] = barVelocity[bar_for_copying];
            play_presetNr_Playmode_ccChannel[bar_to_edit + i] = play_presetNr_Playmode_ccChannel[bar_for_copying];
            play_presetNr_Plugin_ccValue[bar_to_edit + i] = play_presetNr_Plugin_ccValue[bar_for_copying];
            barProbabilty[bar_to_edit + i] = barProbabilty[bar_for_copying];

            draw_arrangment_line(my_Arranger_Y_axis - 1, bar_to_edit + i);
        }
    }
}
void Track::set_clip_to_play_trellis(uint8_t _bar, uint8_t _clipNr) // Songmode: sets a desired clip on a bar with only trellis input (bar = X, Clip = Y)
{

    clip_to_play[_bar] = _clipNr;
    draw_arrangment_line(my_Arranger_Y_axis - 1, _bar);

    // updateTFTScreen = true;
    // draw_sequencer_arranger_parameter(my_Arranger_Y_axis - 1, 2, "Clip", clip_to_play[bar_to_edit], "NO_NAME");
}

Track track1(1);
Track track2(2);
Track track3(3);
Track track4(4);

Track track5(5);
Track track6(6);
Track track7(7);
Track track8(8);

Track *allTracks[8]{&track1, &track2, &track3, &track4, &track5, &track6, &track7, &track8};
