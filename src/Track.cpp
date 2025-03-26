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
    SD.begin(BUILTIN_SDCARD);
    // Serial.println("in save mode:");
    neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;

    sprintf(_trackname, "%dtrack%d.txt\0", songNr, my_Arranger_Y_axis);
    // Serial.println(_trackname);

    // delete the file:
    // Serial.println("Removing:");
    SD.remove(_trackname);
    // Serial.println("Done:");

    // open the file.
    // Serial.println("Creating and opening:");
    this->myTrackFile = SD.open(_trackname, FILE_WRITE);
    // Serial.println(_trackname);
    // Serial.println("Done:");
    //  if the file opened okay, write to it:
    if (this->myTrackFile)
    {
        // save tracks
        // Serial.println("Writing track:");

        for (int c = 0; c < MAX_CLIPS; c++)
        {
            for (int t = 0; t <= MAX_TICKS; t++)
            {
                for (int v = 0; v < MAX_VOICES; v++)
                {
                    this->myTrackFile.print((char)this->clip[c].tick[t].voice[v]);
                    this->myTrackFile.print((char)this->clip[c].tick[t].velo[v]);
                    this->myTrackFile.print((char)this->clip[c].tick[t].stepFX[v]);
                }
                
            }
        }
        for (int t = 0; t <= MAX_TICKS; t++)
        {
            // Serial.printf("save track: %d, tick: %d, note: %d, channel out; %d\n", my_Arranger_Y_axis, t, this->clip[0].tick[t].voice[0], parameter[SET_MIDICH_OUT]);
        }
        // Serial.println("array saved:");
        for (int i = 0; i < MAX_BARS; i++)
        {
            this->myTrackFile.print((char)clip_to_play[i]);
            this->myTrackFile.print((char)noteOffset[i]);
            this->myTrackFile.print((char)barVelocity[i]);
            this->myTrackFile.print((char)play_presetNr_ccChannel[i]);
            this->myTrackFile.print((char)play_presetNr_ccValue[i]);
        }
        // Serial.println("song saved:");
        for (int p = 0; p < NUM_PRESETS + 1; p++)
        {
            for (int t = 0; t < NUM_PARAMETERS; t++)
            {
                this->myTrackFile.print((char)CCchannel[p][t]);
                this->myTrackFile.print((char)CCvalue[p][t]);
            }
        }
        // Serial.println("midi saved:");
        for (int i = 0; i < NUM_PARAMETERS; i++)
        {
            this->myTrackFile.print((char)parameter[i]);
            this->myTrackFile.print((char)SeqMod1Value[i]);
            this->myTrackFile.print((char)SeqMod2Value[i]);
            this->myTrackFile.print((char)SeqMod3Value[i]);
            this->myTrackFile.print((char)SeqMod4Value[i]);
            // Serial.printf("save parameter %d = %d\n", i, parameter[i]);
        }
        // Serial.println("settings & seqmodes saved:");
        this->myTrackFile.print((char)setStepFX);
        this->myTrackFile.print((char)mixGainPot);
        this->myTrackFile.print((char)mixDryPot);
        this->myTrackFile.print((char)mixFX1Pot);
        this->myTrackFile.print((char)mixFX2Pot);
        this->myTrackFile.print((char)mixFX3Pot);
        uint8_t _tempOffset = performNoteOffset + 64;
        this->myTrackFile.print((char)_tempOffset);
        // close the file:
        this->myTrackFile.close();
        // Serial.println("all saved:");
        Serial.printf("saved track:%d\n", my_Arranger_Y_axis);
    }
    else
    {
        // if the file didn't open, print an error:
        Serial.printf("ERROR save track:%d\n", my_Arranger_Y_axis);
    }
    delay(10);
    if (parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
    {
        save_plugin(songNr, parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1));
        // allPlugins[parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1)]->save_plugin(songNr);
    }

    // startUpScreen();
}
void Track::load_track(uint8_t songNr)
{
    for (int p = 0; p < TRELLIS_MAX_PAGES; p++)
    {
        for (int i = 0; i < NUM_STEPS; i++)
        {
            trellis_set_main_buffer(p, i, (my_Arranger_Y_axis - 1), TRELLIS_BLACK);
        }
    }
    SD.begin(BUILTIN_SDCARD);
    // Serial.println("in load mode");
    sprintf(_trackname, "%dtrack%d.txt\0", songNr, my_Arranger_Y_axis);
    //  Serial.println(_trackname);
    //  open the file for reading:
    this->myTrackFile = SD.open(_trackname, FILE_READ);
    // Serial.println(_trackname);
    if (this->myTrackFile)
    {
        // Serial.println("opening:");
        //  read from the file until there's nothing else in it:
        //  load track 1
        for (int c = 0; c < MAX_CLIPS; c++)
        {
            for (int t = 0; t <= MAX_TICKS; t++)
            {
                for (int v = 0; v < MAX_VOICES; v++)
                {
                    this->clip[c].tick[t].voice[v] = this->myTrackFile.read();
                    // Serial.printf("clip: %d, tick: %d, voice: %d, note: %d\n", c, t, v, this->array[0][t][0]);
                    this->clip[c].tick[t].velo[v] = this->myTrackFile.read();
                    this->clip[c].tick[t].stepFX[v] = this->myTrackFile.read();
                }
                
            }
        }
        for (int i = 0; i < MAX_TICKS; i++)
        {
            for (int v = 0; v < MAX_VOICES; v++)
            {
                if (clip[parameter[SET_CLIP2_EDIT]].tick[i].voice[v] < NO_NOTE)
                {
                    trellis_assign_main_buffer(parameter[SET_CLIP2_EDIT], (i / TICKS_PER_STEP), (my_Arranger_Y_axis - 1), trellisTrackColor[my_Arranger_Y_axis - 1]);
                }
            }
        }
        for (int t = 0; t <= MAX_TICKS; t++)
        {
            // Serial.printf("load track: %d, tick: %d, note: %d, channel out; %d\n", my_Arranger_Y_axis, t, this->clip[0].tick[t].voice[0], parameter[SET_MIDICH_OUT]);
        }
        // Serial.println("array loaded:");

        for (int i = 0; i < MAX_BARS; i++)
        {
            clip_to_play[i] = this->myTrackFile.read();
            noteOffset[i] = this->myTrackFile.read();
            barVelocity[i] = this->myTrackFile.read();
            play_presetNr_ccChannel[i] = this->myTrackFile.read();
            play_presetNr_ccValue[i] = this->myTrackFile.read();
            if (clip_to_play[i] <= NUM_USER_CLIPS)
            {
                // Serial.println((i / 16) + TRELLIS_SCREEN_ARRANGER_1);
                trellis_assign_main_buffer((i / 16) + TRELLIS_SCREEN_ARRANGER_1, i % 16, (my_Arranger_Y_axis - 1), trellisTrackColor[my_Arranger_Y_axis - 1] + (clip_to_play[i] * 20));
            }
        }
        // Serial.println("song loaded:");

        for (int p = 0; p < NUM_PRESETS + 1; p++)
        {
            for (int t = 0; t < NUM_PARAMETERS; t++)
            {
                CCchannel[p][t] = this->myTrackFile.read();
                CCvalue[p][t] = this->myTrackFile.read();
            }
        }
        // Serial.println("midi loaded:");

        for (int i = 0; i < NUM_PARAMETERS; i++)
        {
            parameter[i] = this->myTrackFile.read();
            SeqMod1Value[i] = this->myTrackFile.read();
            SeqMod2Value[i] = this->myTrackFile.read();
            SeqMod3Value[i] = this->myTrackFile.read();
            SeqMod4Value[i] = this->myTrackFile.read();
            //   Serial.printf("load parameter %d = %d\n", i, parameter[i]);
        }
        setStepFX = this->myTrackFile.read();
        mixGainPot = this->myTrackFile.read();
        mixDryPot = this->myTrackFile.read();
        mixFX1Pot = this->myTrackFile.read();
        mixFX2Pot = this->myTrackFile.read();
        mixFX3Pot = this->myTrackFile.read();
        int _tempOffset = this->myTrackFile.read();
        performNoteOffset = _tempOffset - 64;
        // Serial.println("settings loaded:");

        // startUpScreen();
        //  close the file:
        this->myTrackFile.close();
        Serial.printf("loaded track:%d\n", my_Arranger_Y_axis);
    }
    else
    {
        // if the file didn't open, print an error:
        Serial.printf("ERROR load track:%d\n", my_Arranger_Y_axis);
    }
    delay(10);
    if (parameter[SET_MIDICH_OUT] > NUM_MIDI_OUTPUTS)
    {
        load_plugin(songNr, parameter[SET_MIDICH_OUT] - (NUM_MIDI_OUTPUTS + 1));
    }
}

void Track::play_sequencer_mode(uint8_t cloock, uint8_t start, uint8_t end)
{
    if (cloock == 0) // fehler hier
    {
        external_clock_bar++;
    }
    if (cloock % (parameter[SET_CLOCK_DIVISION] + performClockDivision) == 0)
    {
        internal_clock++;
        if (internal_clock >= parameter[SET_SEQUENCE_LENGTH])
        {
            internal_clock = 0;
        }
        if (internal_clock == 0)
        {
            internal_clock_bar++;
            change_presets();
        }

        internal_clock_is_on = true;
    }
    else
        internal_clock_is_on = false;

    if (external_clock_bar >= end)
        external_clock_bar = start;
    if (internal_clock_bar >= end)
        internal_clock_bar = start;

    // if (my_Arranger_Y_axis == 1)
    // {
    //     Serial.printf("internal bar:  %d, external bar:  %d\n", internal_clock_bar, external_clock_bar);
    //     Serial.printf("internal tick: %d, external tick: %d\n", internal_clock, cloock);
    //     Serial.println();
    // }
    //   Serial.println(internal_clock_bar);
    if (internal_clock_is_on)
    {
        if (!muted && !muteThruSolo)
        {
            // Serial.printf("internalbar=%d, externalbar= %d\n",internal_clock_bar,external_clock_bar );
            // Serial.printf("internalClock=%d, externalClock= %d\n", internal_clock, cloock);
            if (parameter[SET_SEQ_MODE] == 0)
                play_seq_mode0(internal_clock);
            if (parameter[SET_SEQ_MODE] == 1)
                play_seq_mode1(internal_clock);
            if (parameter[SET_SEQ_MODE] == 2)
                play_seq_mode2(internal_clock);
            if (parameter[SET_SEQ_MODE] == 3)
                play_seq_mode3(internal_clock);
            if (parameter[SET_SEQ_MODE] == 4)
                play_seq_mode4(internal_clock);
            if (parameter[SET_SEQ_MODE] == 5)
                play_seq_mode5(internal_clock);
            if (parameter[SET_SEQ_MODE] == 6)
                play_seq_mode6(internal_clock);
            if (parameter[SET_SEQ_MODE] == 7)
                play_seq_mode7(internal_clock);
        }
    }
    // }
}

void Track::set_seq_mode_parameters(uint8_t row)
{
    if (parameter[SET_SEQ_MODE] == 1)
        set_seq_mode1_parameters(row);
    if (parameter[SET_SEQ_MODE] == 2)
        set_seq_mode2_parameters(row);
    if (parameter[SET_SEQ_MODE] == 3)
        set_seq_mode3_parameters(row);
    if (parameter[SET_SEQ_MODE] == 4)
        set_seq_mode4_parameters(row);
    if (parameter[SET_SEQ_MODE] == 5)
        set_seq_mode5_parameters(row);
    if (parameter[SET_SEQ_MODE] == 6)
        set_seq_mode6_parameters(row);
    if (parameter[SET_SEQ_MODE] == 7)
        set_seq_mode7_parameters(row);
}
void Track::draw_sequencer_modes(uint8_t mode)
{
    clearWorkSpace();
    change_plugin_row = true;
    if (mode == 1)
        draw_seq_mode1();
    if (mode == 2)
        draw_seq_mode2();
    if (mode == 3)
        draw_seq_mode3();
    if (mode == 4)
        draw_seq_mode4();
    if (mode == 5)
        draw_seq_mode5();
    if (mode == 6)
        draw_seq_mode6();
    if (mode == 7)
        draw_seq_mode7();
}

void Track::noteOn(uint8_t Note, uint8_t Velo, uint8_t Channel)
{
    // Serial.printf("sending noteOn: %d, velo: %d channel: %d\n", Note, Velo, Channel);
    sendNoteOn(my_Arranger_Y_axis - 1, Note, Velo, Channel);
    if (recordState)
        record_noteOn(Note, Velo, Channel);

    // MIDI1.sendNoteOn(Note, Velo, Channel);
    // usbMIDI.sendNoteOn(Note, Velo, Channel);
    //
}
void Track::noteOff(uint8_t Note, uint8_t Velo, uint8_t Channel)
{
    // Serial.printf("sending noteOff: %d, velo: %d channel: %d\n", Note, Velo, Channel);
    sendNoteOff(my_Arranger_Y_axis - 1, Note, Velo, Channel);
    if (recordState)
        record_noteOff(Note, Velo, Channel);
    // MIDI1.sendNoteOn(Note, Velo, Channel);
    // usbMIDI.sendNoteOn(Note, Velo, Channel);
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
        for (int i = recordStartTick[recordVoice]; i <= internal_clock; i++)
        {
            clip[parameter[SET_CLIP2_EDIT]].tick[i].voice[recordVoice] = Note;
            clip[parameter[SET_CLIP2_EDIT]].tick[i].velo[recordVoice] = recordVelocity[recordVoice];
            trellis_set_main_buffer(parameter[SET_CLIP2_EDIT], (i / TICKS_PER_STEP), (my_Arranger_Y_axis - 1), trellisTrackColor[my_Arranger_Y_axis - 1]);
        }
    }
}
//---------------------------arranger stuff-------------------------------------
void Track::set_arranger_parameters(uint8_t lastProw)
{

    switch (lastProw)
    {
    case 0:
        encoder_SetCursor(STEP_FRAME_W, 8); // Encoder: 0,1
        set_bar_parameter(2, pixelTouchX, clip_to_play, 0, NUM_USER_CLIPS + 1, "Clip");
        set_bar_parameter(3, pixelTouchX, noteOffset, -99, 99, "Trns");

        break;
    case 1:
        encoder_SetCursor(STEP_FRAME_W, 8); // Encoder: 0,1
        set_bar_parameter(2, pixelTouchX, clip_to_play, 0, NUM_USER_CLIPS + 1, "Clip");
        set_bar_parameter(3, pixelTouchX, barVelocity, 0, 127, "Velo");
        break;
    case 2:
        encoder_SetCursor(STEP_FRAME_W, 8); // Encoder: 0,1
        set_bar_parameter(2, pixelTouchX, play_presetNr_ccChannel, 0, NUM_PRESETS, "ccC");
        set_bar_parameter(3, pixelTouchX, play_presetNr_ccValue, 0, NUM_PRESETS, "ccV");
        break;
    case 3:
    default:
        break;
    }
    copy_bar();
    draw_arranger_parameters(lastProw);
}
void Track::set_bar_parameter(uint8_t _encoder, int b, int *parameterArray, int minValue, int maxValue, const char *label)
{
    if (gridTouchY == my_Arranger_Y_axis)
    {
        uint8_t when = (((b - SEQ_GRID_LEFT) / STEP_FRAME_W) + (BARS_PER_PAGE * arrangerpage));
        if (enc_moved[_encoder])
        {
            int _when = constrain(parameterArray[when] + encoded[_encoder], minValue, maxValue);
            parameterArray[when] = _when;
            for (int i = 0; i < parameter[SET_CLOCK_DIVISION]; i++)
            {
                if (enc_moved[2])
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
        if (CCchannel[play_presetNr_ccChannel[internal_clock_bar]][i] < 128)
        {
            Serial.print("cc:");
            Serial.println(CCchannel[play_presetNr_ccChannel[internal_clock_bar]][i]);
            sendControlChange(CCchannel[play_presetNr_ccChannel[internal_clock_bar]][i], CCvalue[play_presetNr_ccValue[internal_clock_bar]][i], parameter[SET_MIDICH_OUT]);
            Serial.printf("Trackbar Track: %d,clip2play: %d externalbar: %d internalBar: %d\n", my_Arranger_Y_axis - 1, clip_to_play[external_clock_bar], external_clock_bar, internal_clock_bar);
        }
    }
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
            play_presetNr_ccChannel[i] = NUM_PRESETS;
            play_presetNr_ccValue[bar_to_edit + i] = NUM_PRESETS;
        }
        draw_arrangment_lines(my_Arranger_Y_axis - 1, arrangerpage);
    }
}
void Track::copy_bar() // copy the last edited barParameters to the desired bar (clip, Transpose, Veleocity, ccChannel, ccValue)
{
    if (neotrellisPressed[TRELLIS_BUTTON_ENTER])
    {
        // for (int i = 0; i < parameter[SET_CLOCK_DIVISION]; i++)
        int i = 0;
        {
            clip_to_play[bar_to_edit + i] = clip_to_play[bar_for_copying];
            noteOffset[bar_to_edit + i] = noteOffset[bar_for_copying];
            barVelocity[bar_to_edit + i] = barVelocity[bar_for_copying];
            play_presetNr_ccChannel[bar_to_edit + i] = play_presetNr_ccChannel[bar_for_copying];
            play_presetNr_ccValue[bar_to_edit + i] = play_presetNr_ccValue[bar_for_copying];
            draw_arrangment_line(my_Arranger_Y_axis - 1, bar_to_edit + i);
        }
    }
}
void Track::set_clip_to_play_trellis(uint8_t _bar, uint8_t _clipNr) // Songmode: sets a desired clip on a bar with only trellis input (bar = X, Clip = Y)
{

    for (int i = 0; i < parameter[SET_CLOCK_DIVISION]; i++)
    {
        clip_to_play[_bar + i] = _clipNr;
        draw_arrangment_line(my_Arranger_Y_axis - 1, _bar + i);
    }
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
