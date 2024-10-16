#include <Arduino.h>
#include "Track.h"
// File myFile;
//  MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI1);
//   midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> *midi_out_serial = &MIDI1;
void Track::update(int PixelX, uint8_t gridY)
{
    // MIDI1.read();

    bar_to_edit = ((PixelX - SEQ_GRID_LEFT) / STEP_FRAME_W) + (BARS_PER_PAGE * (arrangerpage));
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
                }
                this->myTrackFile.print((char)this->clip[c].tick[t].stepFX);
            }
        }
        for (int t = 0; t <= MAX_TICKS; t++)
        {
            // Serial.printf("save track: %d, tick: %d, note: %d, channel out; %d\n", my_Arranger_Y_axis, t, this->clip[0].tick[t].voice[0], parameter[SET_MIDICH_OUT]);
        }
        // Serial.println("array saved:");
        for (int i = 0; i < 256; i++)
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
                }
                this->clip[c].tick[t].stepFX = this->myTrackFile.read();
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

        for (int i = 0; i < 256; i++)
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
    if (cloock % MAX_TICKS == 0)
        external_clock_bar++;
    if (cloock % (parameter[SET_CLOCK_DIVISION] + performClockDivision) == 0)
    {
        internal_clock++;
        internal_clock_is_on = true;
    }
    else
        internal_clock_is_on = false;

    if (external_clock_bar >= end)
        external_clock_bar = start;
    if (internal_clock_bar >= end)
        internal_clock_bar = start;

    if (internal_clock >= parameter[SET_SEQUENCE_LENGTH])
    {
        internal_clock = 0;
    }
    if (internal_clock == 0)
    {
        internal_clock_bar++;
        change_presets();
    }
    // Serial.printf("bar: %d, tick: %d\n", internal_clock_bar, internal_clock);
    //  Serial.println(internal_clock_bar);
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

    // MIDI1.sendNoteOn(Note, Velo, Channel);
    // usbMIDI.sendNoteOn(Note, Velo, Channel);
    //
}
void Track::noteOff(uint8_t Note, uint8_t Velo, uint8_t Channel)
{
    // Serial.printf("sending noteOff: %d, velo: %d channel: %d\n", Note, Velo, Channel);
    sendNoteOff(my_Arranger_Y_axis - 1, Note, Velo, Channel);

    // MIDI1.sendNoteOn(Note, Velo, Channel);
    // usbMIDI.sendNoteOn(Note, Velo, Channel);
}
void Track::record_noteOn(uint8_t Note, uint8_t Velo, uint8_t Channel)
{
    recordVoice = Note % NOTES_PER_OCTAVE;
    recordLastNote[recordVoice] = Note;
    recordVelocity[recordVoice] = Velo;
    recordStartTick[recordVoice] = internal_clock;
    // clip[parameter[SET_CLIP2_EDIT]].tick[internal_clock].voice[0] = Note;
    // clip[parameter[SET_CLIP2_EDIT]].tick[internal_clock].velo[0] = Velo;
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
        set_clip_to_play(2, pixelTouchX);
        set_note_offset(3, pixelTouchX);
        break;
    case 1:
        encoder_SetCursor(STEP_FRAME_W, 8); // Encoder: 0,1
        set_barVelocity(2, pixelTouchX);
        // myClock.set_tempo(1);
        // myClock.set_start_of_loop(2); // Encoder: 2
        // myClock.set_end_of_loop(3);   // Encoder: 3
        break;
    case 2:
        encoder_SetCursor(STEP_FRAME_W, 8); // Encoder: 0,1
        set_play_presetNr_ccChannel(2, 2);
        set_play_presetNr_ccValue(3, 2);
        break;
    case 3:
    default:
        break;
    }
    draw_arranger_parameters(lastProw);
}
void Track::change_presets()
{
    for (int i = 0; i < 16; i++)
    {
        if (CCchannel[play_presetNr_ccChannel[internal_clock_bar]][i] < 128)
        {
            Serial.print("cc:");
            Serial.println(CCchannel[play_presetNr_ccChannel[internal_clock_bar]][i]);
            sendControlChange(CCchannel[play_presetNr_ccChannel[internal_clock_bar]][i], CCvalue[play_presetNr_ccValue[internal_clock_bar]][i], parameter[SET_MIDICH_OUT]);
        }
    }
    Serial.printf("Trackbar Track: %d,clip2play: %d externalbar: %d internalBar: %d\n", my_Arranger_Y_axis - 1, clip_to_play[external_clock_bar], external_clock_bar, internal_clock_bar);
}
void Track::set_clip_to_play(uint8_t n, uint8_t b)
{
    if (gridTouchY == my_Arranger_Y_axis)
    {
        if (enc_moved[n])
        {

            for (int i = 0; i < parameter[SET_CLOCK_DIVISION]; i++)
            {
                clip_to_play[bar_to_edit + i] = constrain(clip_to_play[bar_to_edit + i] + encoded[n], 0, NUM_USER_CLIPS + 1);
                draw_arrangment_line(my_Arranger_Y_axis - 1, bar_to_edit + i);
            }
            // updateTFTScreen = true;
            // draw_sequencer_arranger_parameter(my_Arranger_Y_axis - 1, n, "Clip", clip_to_play[bar_to_edit], "NO_NAME");
            //  enc_moved[n] = false;
        }
    }
}
void Track::set_clip_to_play_trellis(uint8_t _bar, uint8_t _clipNr)
{

    for (int i = 0; i < parameter[SET_CLOCK_DIVISION]; i++)
    {
        clip_to_play[_bar + i] = _clipNr;
        draw_arrangment_line(my_Arranger_Y_axis - 1, _bar + i);
    }
    // updateTFTScreen = true;
    // draw_sequencer_arranger_parameter(my_Arranger_Y_axis - 1, 2, "Clip", clip_to_play[bar_to_edit], "NO_NAME");
}
void Track::set_note_offset(uint8_t _encoder, int b)
{
    if (gridTouchY == my_Arranger_Y_axis)
    {
        uint8_t when = ((b - SEQ_GRID_LEFT) / STEP_FRAME_W) + (BARS_PER_PAGE * arrangerpage);
        if (enc_moved[_encoder])
        {
            int _tempOffset = constrain(noteOffset[when] + encoded[_encoder], -99, +99);

            for (int i = 0; i < parameter[SET_CLOCK_DIVISION]; i++)
            {
                noteOffset[when + i] = _tempOffset;
                draw_arrangment_line(my_Arranger_Y_axis - 1, when + i);
            }
            Serial.printf("set NOteOffset: %d, when: %d\n", noteOffset[when], when);
            // updateTFTScreen = true;
            // draw_sequencer_arranger_parameter(my_Arranger_Y_axis - 1, _encoder, "Trns", noteOffset[when], "NO_NAME");
            // enc_moved[_encoder] = false;
        }
    }
}
void Track::set_barVelocity(uint8_t _encoder, int b)
{
    if (gridTouchY == my_Arranger_Y_axis)
    {
        uint8_t when = ((b - SEQ_GRID_LEFT) / STEP_FRAME_W) + (BARS_PER_PAGE * arrangerpage);
        if (enc_moved[_encoder])
        {
            barVelocity[when] = constrain(barVelocity[when] + encoded[_encoder], 0, 127);
            for (int i = 0; i < parameter[SET_CLOCK_DIVISION]; i++)
            {
                draw_arrangment_line(my_Arranger_Y_axis - 1, when + i);
            }
            // enc_moved[_encoder] = false;
            // draw_sequencer_arranger_parameter(my_Arranger_Y_axis - 1, _encoder, "Velo", barVelocity[when], "NO_NAME");
        }
    }
}

// MIDICC
void Track::set_play_presetNr_ccChannel(uint8_t n, uint8_t lastProw)
{
    if (enc_moved[n])
    {
        play_presetNr_ccChannel[bar_to_edit] = constrain(play_presetNr_ccChannel[bar_to_edit] + encoded[n], 0, NUM_PRESETS);
        Serial.printf("cc-Set: %d, vl-set: %d, bar: %d\n", play_presetNr_ccChannel[bar_to_edit], play_presetNr_ccValue[bar_to_edit], bar_to_edit);
        change_plugin_row = true;
        // draw_MIDI_CC_screen();
        // draw_sequencer_arranger_parameter(my_Arranger_Y_axis - 1, n, "ccC", play_presetNr_ccChannel[bar_to_edit], "NO_NAME");
        // enc_moved[n] = false;
    }
}
void Track::set_play_presetNr_ccValue(uint8_t n, uint8_t lastProw)
{
    if (enc_moved[n])
    {
        play_presetNr_ccValue[bar_to_edit] = constrain(play_presetNr_ccValue[bar_to_edit] + encoded[n], 0, NUM_PRESETS);
        Serial.printf("cc-Set: %d, vl-set: %d, bar: %d\n", play_presetNr_ccChannel[bar_to_edit], play_presetNr_ccValue[bar_to_edit], bar_to_edit);
        change_plugin_row = true;
        // draw_MIDI_CC_screen();
        // draw_sequencer_arranger_parameter(my_Arranger_Y_axis - 1, n, "ccC", play_presetNr_ccValue[bar_to_edit], "NO_NAME");

        // enc_moved[n] = false;
    }
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
