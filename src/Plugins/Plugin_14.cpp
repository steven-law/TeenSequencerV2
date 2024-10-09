
#include <Plugins/Plugin_14.h>

extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
void clearWorkSpace();
extern const char *filterName[4];

void Plugin_14::setup()
{

    dc.amplitude(1);

    playMem.begin();
     playMem.enableInterpolation(true);
  inputGain.gain(1);

    Fenv.delay(0);
    Fenv.attack(0);
    Fenv.hold(0);
    Fenv.decay(0);
    Fenv.sustain(1);
    Fenv.release(200);

    filter.frequency(2000);
    filter.resonance(1);
    filter.octaveControl(4);
    // ladder[i].frequency(2000);
    // ladder[i].resonance(1);
    // ladder[i].octaveControl(4);

    fMixer.gain(0, 1);
    fMixer.gain(1, 0);
    fMixer.gain(2, 0);
    fMixer.gain(3, 0);

    Aenv.delay(0);
    Aenv.attack(0);
    Aenv.hold(0);
    Aenv.decay(0);
    Aenv.sustain(1);
    Aenv.release(200);

    // mixer.gain(0, 1);

    MixGain.gain(0, 1);
    MixGain.gain(1, 1);
    potentiometer[presetNr][0] = 0;
    potentiometer[presetNr][1] = 127;
    potentiometer[presetNr][2] = 0;
    potentiometer[presetNr][3] = 127;
    potentiometer[presetNr][8] = 60;
    potentiometer[presetNr][9] = 0;
    potentiometer[presetNr][10] = 30;
    potentiometer[presetNr][11] = 0;
    potentiometer[presetNr][12] = 5;
    potentiometer[presetNr][13] = 0;
    potentiometer[presetNr][14] = 127;
    potentiometer[presetNr][15] = 20;
    // SongVol.gain(1);
    _playFilename = "0.RAW";
    _recFilename = "0.RAW";
}

void Plugin_14::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    float frequency = note_frequency[notePlayed] * tuning;
    Serial.printf("pl14: notePlayed= %d\n", notePlayed);
     playMem.setPlaybackRate(note_frequency[notePlayed]);
    //  playMem.playRaw(_playFilename, 1);

    playMem.playRaw(_playFilename,1);
    Fenv.noteOn();
    Aenv.noteOn();
    Serial.println(_playFilename);
}
void Plugin_14::noteOff(uint8_t notePlayed, uint8_t voice)
{
    Fenv.noteOff();
    Aenv.noteOff();
}
void Plugin_14::set_parameters(uint8_t row)
{
    draw_plugin();
    show_peak();
    for (int i = 0; i < NUM_TRACKS; i++)
    {
        if (allTracks[i]->recordState && allTracks[i]->parameter[SET_MIDICH_OUT] == 62)
        {
            if (neotrellisPressed[3 + ((i + 4) * X_DIM)])
                startrecording = true;
            neotrellisPressed[3 + ((i + 4) * X_DIM)] = false;
        }
    }
    startRecording();
    if (audio_rec_now)
    {
        continueRecording();
        // drawActiveRect(CTRL_COL_1, CTRL_ROW_1, 2, 2, audio_rec_now, "Rec", ILI9341_RED);
    }
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        if (row == 0)
        {

            set_rec_waveform(0, 0, "Rec W~F");
             set_rec_amplitude(1, 0, "Rec Vol");
            set_voice_waveform(2, 0, "Play W~F");
            set_voice_amplitude(3, 0, "Play Vol");
        }

        if (row == 1)
        {
        }

        if (row == 2)
        {
            set_filter_frequency(0, 2, "Filt-Frq");
            set_filter_resonance(1, 2, "Resonance", 0, 5.00);
            set_filter_sweep(2, 2, "Sweep");
            set_filter_type(3, 2, filterName[potentiometer[presetNr][11]]);
        }

        if (row == 3)
        {
            set_envelope_ADSR(3, 1000, 500, 2000);
            // set_envelope_attack(0, 3, "Attack", 0, 1000);
            // set_envelope_sustain(2, 3, "Sustain");
            // set_envelope_decay(1, 3, "Decay", 0, 500);
            // set_envelope_release(3, 3, "Release", 0, 2000);
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        set_presetNr();
    }
}
void Plugin_14::draw_plugin()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        clearWorkSpace();
        // Serial.println("drawing plugin 2");
        drawPot(0, 0, potentiometer[presetNr][0], "Rec W~F");
        drawPot(1, 0, potentiometer[presetNr][1], "Rec Vol");
        drawPot(2, 0, potentiometer[presetNr][2], "Play W~F");
        drawPot(3, 0, potentiometer[presetNr][3], "Play Vol");
        // redraw_customWaveform(5);

        drawPot(0, 2, potentiometer[presetNr][8], "Filt-Frq");
        drawPot(1, 2, potentiometer[presetNr][9], "Resonance");
        drawPot(2, 2, potentiometer[presetNr][10], "Sweep");
        drawPot(3, 2, potentiometer[presetNr][11], filterName[potentiometer[presetNr][11]]);

        drawEnvelope(3, potentiometer[presetNr][12], potentiometer[presetNr][13],
                     potentiometer[presetNr][14], potentiometer[presetNr][15]);
        // drawPot(0, 3, potentiometer[presetNr][12], "Attack");
        // drawPot(1, 3, potentiometer[presetNr][13], "Decay");
        // drawPot(2, 3, potentiometer[presetNr][14], "Sustain");
        // drawPot(3, 3, potentiometer[presetNr][15], "Release");

        // draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "Prset", presetNr, 3, 0);
    }
}
void Plugin_14::change_preset()
{
    assign_rec_waveform(potentiometer[presetNr][0]);
     assign_rec_amplitude(potentiometer[presetNr][1]);
    assign_voice_waveform(potentiometer[presetNr][2]);
    assign_voice_amplitude(potentiometer[presetNr][3]);

    assign_filter_frequency(potentiometer[presetNr][8]);
    assign_filter_resonance(potentiometer[presetNr][9]);
    assign_filter_sweep(potentiometer[presetNr][10]);
    selectFilterType(potentiometer[presetNr][11]);

    assign_envelope_attack(potentiometer[presetNr][12], 1000);
    assign_envelope_decay(potentiometer[presetNr][13], 500);
    assign_envelope_sustain(potentiometer[presetNr][14]);
    assign_envelope_release(potentiometer[presetNr][15], 2000);
}
void Plugin_14::set_rec_waveform(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_rec_waveform(get_Potentiometer(XPos, YPos, name));
        enc_moved[XPos] = false;
    }
}
void Plugin_14::assign_rec_waveform(uint8_t value)
{

    sprintf(_recFilename, "%d.raw\0", value);
    // newdigate::flashloader loader;
    // sample = loader.loadSample(_recFileName);
}
void Plugin_14::set_rec_amplitude(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_rec_amplitude(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_14::assign_rec_amplitude(uint8_t value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;
  inputGain.gain(ampl);
}
void Plugin_14::set_voice_waveform(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_rec_waveform(get_Potentiometer(XPos, YPos, name));
        enc_moved[XPos] = false;
    }
}
void Plugin_14::assign_voice_waveform(uint8_t value)
{
    sprintf(_playFilename, "%d.raw\0", value);
}
void Plugin_14::set_voice_amplitude(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_voice_amplitude(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_14::assign_voice_amplitude(uint8_t value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;
    fMixer.gain(0, ampl);
    fMixer.gain(1, ampl);
    fMixer.gain(2, ampl);
}

void Plugin_14::show_peak()
{
    float peaker = peak1.read();

    if (peak1.available() && peaker > 0.001)
    {
        // level meter
        int actualPeak = map(peaker, 0.00, 1.00, 0, potentiometer[presetNr][1]);
        if (millis() % 1 == 0)
        {
            //  Serial.println(peaker, 3);
            drawPot(1, 0, actualPeak, "Rec Vol");
            draw_actual_waveform(5); // oscilloscope
        }
    }
}

// draw waveforms
void Plugin_14::clearcustomWaveform(uint8_t YPos)
{
    // pl12WaveformAssigned = false;
    for (int i = 0; i < 256; i++)
    {
        pl14_customWaveform[i] = 0;
    }
    pl14_oldCustomWaveformValue = 0;
    pl14_oldCustomWaveformXPos = 32;
    // redraw_customWaveform(YPos);
}
void Plugin_14::draw_actual_waveform(uint8_t YPos)
{
    float peaker = peak1.read();
    static uint8_t xPosPeak;
    int peakHeight = map(peaker, 0.00, 1.00, 41, 0);
    static int oldPeakHeight;
    static int oldxPosPeak;
    xPosPeak++;
    if (xPosPeak == 0)
    {
        tft.fillRect(32, YPos * STEP_FRAME_H, 256, 41, ILI9341_DARKGREY);
        oldxPosPeak = 0;
    }
    // Serial.printf("xPos=%d, ypos=%i\n", xPosPeak, peakHeight);
    tft.drawLine(oldxPosPeak + 32, (oldPeakHeight) + ((YPos)*STEP_FRAME_H), xPosPeak + 32, (peakHeight) + ((YPos)*STEP_FRAME_H), ILI9341_WHITE);
    oldxPosPeak = xPosPeak;
    oldPeakHeight = peakHeight;
}
void Plugin_14::smooth_customWaveform(uint8_t YPos)
{
    for (int i = 0; i < 256; i++)
    {

        if (pl14_customWaveform[i] == 0)
            for (int b = i; b < 256; b++)
                pl14_customWaveform[i] = pl14_customWaveform[i - 1] + (((pl14_customWaveform[b] / (b - (i - 1)))) * (i - (i - 1)));
    }
    //  redraw_customWaveform(YPos);
}

void Plugin_14::redraw_customWaveform(int8_t YPos)
{
    for (int i = 0; i < 255; i++)
    {
        // static int startY;
        int startY = map(pl14_customWaveform[i], 32768, -32768, 0, 41);
        int endY = map(pl14_customWaveform[i + 1], 32768, -32768, 0, 41);
        tft.drawLine(i + 32, startY + ((YPos)*STEP_FRAME_H), i + 33, endY + ((YPos)*STEP_FRAME_H), ILI9341_WHITE);
        // startY=endY;
    }
}

void Plugin_14::set_filter_frequency(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_filter_frequency(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_14::assign_filter_frequency(uint8_t value)
{

    int frequency = note_frequency[value] * tuning;

    filter.frequency(frequency);
    // ladder[i].frequency(frequency);
}

void Plugin_14::set_filter_resonance(uint8_t XPos, uint8_t YPos, const char *name, float min, float max)
{
    if (enc_moved[XPos])
    {
        assign_filter_resonance(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_14::assign_filter_resonance(uint8_t value)
{

    float reso = value / 25.40;

    filter.resonance(reso);
    // ladder[i].resonance(reso);
}
void Plugin_14::set_filter_sweep(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_filter_sweep(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_14::assign_filter_sweep(uint8_t value)
{
    float swp = value / 18.14;

    filter.octaveControl(swp);
    // ladder[i].octaveControl(swp);
}
void Plugin_14::set_filter_type(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        selectFilterType(constrain(get_Potentiometer(XPos, YPos, name), 0, 3));
    }
}
void Plugin_14::selectFilterType(uint8_t mixerchannel)
{

    fMixer.gain(0, 0);
    fMixer.gain(1, 0);
    fMixer.gain(2, 0);
    fMixer.gain(3, 0);
    fMixer.gain(mixerchannel, 1);
}

void Plugin_14::assign_envelope_attack(uint8_t value, int max)
{
    int attack = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.attack(attack);
    Aenv.attack(attack);
}
void Plugin_14::assign_envelope_decay(uint8_t value, int max)
{
    int decay = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.decay(decay);
    Aenv.decay(decay);
}
void Plugin_14::assign_envelope_sustain(uint8_t value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;

    Fenv.sustain(ampl);
    Aenv.sustain(ampl);
}
void Plugin_14::assign_envelope_release(uint8_t value, int max)
{
    int release = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.release(release);
    Aenv.release(release);
}
void Plugin_14::set_envelope_ADSR(uint8_t YPos, int maxA, int maxD, int maxR)
{

    if (enc_moved[0])
    {
        uint8_t rowIx = YPos * 4;
        potentiometer[presetNr][0 + rowIx] = constrain(potentiometer[presetNr][0 + rowIx] + encoded[0], 0, MIDI_CC_RANGE);
        assign_envelope_attack(potentiometer[presetNr][0 + rowIx], maxA);
        drawEnvelope(YPos, potentiometer[presetNr][0 + rowIx], potentiometer[presetNr][1 + rowIx],
                     potentiometer[presetNr][2 + rowIx], potentiometer[presetNr][3 + rowIx]);
    }
    if (enc_moved[1])
    {
        uint8_t rowIx = YPos * 4;
        potentiometer[presetNr][1 + rowIx] = constrain(potentiometer[presetNr][1 + rowIx] + encoded[1], 0, MIDI_CC_RANGE);
        assign_envelope_decay(potentiometer[presetNr][1 + rowIx], maxD);
        drawEnvelope(YPos, potentiometer[presetNr][0 + rowIx], potentiometer[presetNr][1 + rowIx],
                     potentiometer[presetNr][2 + rowIx], potentiometer[presetNr][3 + rowIx]);
    }
    if (enc_moved[2])
    {
        uint8_t rowIx = YPos * 4;
        potentiometer[presetNr][2 + rowIx] = constrain(potentiometer[presetNr][2 + rowIx] + encoded[2], 0, MIDI_CC_RANGE);
        assign_envelope_sustain(potentiometer[presetNr][2 + rowIx]);
        drawEnvelope(YPos, potentiometer[presetNr][0 + rowIx], potentiometer[presetNr][1 + rowIx],
                     potentiometer[presetNr][2 + rowIx], potentiometer[presetNr][3 + rowIx]);
    }
    if (enc_moved[3])
    {
        uint8_t rowIx = YPos * 4;
        potentiometer[presetNr][3 + rowIx] = constrain(potentiometer[presetNr][3 + rowIx] + encoded[3], 0, MIDI_CC_RANGE);
        assign_envelope_release(potentiometer[presetNr][3 + rowIx], maxR);
        drawEnvelope(YPos, potentiometer[presetNr][0 + rowIx], potentiometer[presetNr][1 + rowIx],
                     potentiometer[presetNr][2 + rowIx], potentiometer[presetNr][3 + rowIx]);
    }
}
void Plugin_14::set_envelope_attack(uint8_t XPos, uint8_t YPos, const char *name, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_attack(get_Potentiometer(XPos, YPos, name), max);
    }
}
void Plugin_14::set_envelope_decay(uint8_t XPos, uint8_t YPos, const char *name, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_decay(get_Potentiometer(XPos, YPos, name), max);
    }
}
void Plugin_14::set_envelope_sustain(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {

        assign_envelope_sustain(get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT);
    }
}
void Plugin_14::set_envelope_release(uint8_t XPos, uint8_t YPos, const char *name, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_release(get_Potentiometer(XPos, YPos, name), max);
    }
}

void Plugin_14::startRecording()
{
    if (startrecording)
    {
        Serial.println("startRecording");
        //  sprintf(_RecFile, "%d.RAW\0", AudioRecorder.selected_file);
        if (SD.exists(_recFilename))
        {
            // The SD library writes new data to the end of the
            // file, so to start a new recording, the old file
            // must be deleted before new data is written.
            SD.remove(_recFilename);
        }
        myFile = SD.open(_recFilename, FILE_WRITE);
        if (myFile)
        {
            queue1.begin();
            audio_rec_now = true;
        }
        startrecording = false;
    }
}

void Plugin_14::continueRecording()
{
    if (queue1.available() >= 2)
    {
        uint8_t buffer[512];
        // Fetch 2 blocks from the audio library and copy
        // into a 512 byte buffer.  The Arduino SD library
        // is most efficient when full 512 byte sector size
        // writes are used.
        memcpy(buffer, queue1.readBuffer(), 256);
        queue1.freeBuffer();
        memcpy(buffer + 256, queue1.readBuffer(), 256);
        queue1.freeBuffer();
        // write all 512 bytes to the SD card
        // elapsedMicros usec = 0;
        myFile.write(buffer, 512);
        // Uncomment these lines to see how long SD writes
        // are taking.  A pair of audio blocks arrives every
        // 5802 microseconds, so hopefully most of the writes
        // take well under 5802 us.  Some will take more, as
        // the SD library also must write to the FAT tables
        // and the SD card controller manages media erase and
        // wear leveling.  The queue1 object can buffer
        // approximately 301700 us of audio, to allow time
        // for occasional high SD card latency, as long as
        // the average write time is under 5802 us.
        // Serial.print("SD write, us=");
        // Serial.println(usec);
    }
}

void Plugin_14::stopRecording()
{

    Serial.println("stopRecording");
    queue1.end();

    while (queue1.available() > 0)
    {
        myFile.write((int *)queue1.readBuffer(), 256);
        queue1.freeBuffer();
    }
    myFile.close();
    audio_rec_now = false;
    // audio_rec_rec = false;
}
Plugin_14 plugin_14("Rec", 14);
