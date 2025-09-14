#include <Plugins/Plugin_14.h>

void Plugin_14::setup()
{
    dc.amplitude(1);

    playMem.begin();
    playMem.enableInterpolation(true);
    inputGain.gain(0);

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
    potentiometer[presetNr][1] = 0;
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
    setParameterNames("Rec W~F", MIDI_CC_RANGE, "Rec Vol", MIDI_CC_RANGE, "Play W~F", MIDI_CC_RANGE, "Play Vol", MIDI_CC_RANGE,
                      "0", 0, "0", 0, "0", 0, "0", 0,
                      "Filter-Freq", MIDI_CC_RANGE, "Resonance", MIDI_CC_RANGE, "Sweep", MIDI_CC_RANGE, "Type", 2,
                      "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE);
}

void Plugin_14::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    float velo = (velocity * (MixerGain / MIDI_CC_RANGE_FLOAT));
    MixGain.gain(0, velo);
    MixGain.gain(1, velo);

    Serial.printf("pl14: notePlayed= %d\n", notePlayed);
    playMem.setPlaybackRate(note_frequency[notePlayed]);
    //  playMem.playRaw(_playFilename, 1);

    playMem.playRaw(_playFilename, 1);
    Fenv.noteOn();
    Aenv.noteOn();
    Serial.println(_playFilename);
}
void Plugin_14::noteOff(uint8_t notePlayed, uint8_t voice)
{
    Fenv.noteOff();
    Aenv.noteOff();
}

void Plugin_14::assign_parameter(uint8_t pot)
{

    show_peak();
    for (int i = 0; i < NUM_TRACKS; i++)
    {
        int trackChannel = allTracks[i]->clip[allTracks[i]->parameter[SET_CLIP2_EDIT]].midiChOut;
        if (allTracks[i]->get_recordState() && trackChannel == 62)
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
    uint8_t value = get_Potentiometer(pot);
    switch (pot)
    {
    case 0:
    {

        sprintf(_recFilename, "%d.raw", value);
        // newdigate::flashloader loader;
        // sample = loader.loadSample(_recFileName);
    }
    break;
    case 1:
    {
        float ampl = value / MIDI_CC_RANGE_FLOAT;
        inputGain.gain(ampl);
    }
    break;
    case 2:
    {
        sprintf(_playFilename, "%d.raw", value);
    }
    break;
    case 3:
    {
        float ampl = value / MIDI_CC_RANGE_FLOAT;
        fMixer.gain(0, 0);
        fMixer.gain(1, 0);
        fMixer.gain(2, 0);
        fMixer.gain(get_Potentiometer(11), ampl);
    }
    break;
    case 4:

        break;
    case 5:

        break;
    case 6:

        break;
    case 7:

        break;
    case 8:
    {
        int frequency = note_frequency[value] * tuning;
        filter.frequency(frequency);
    }
    break;
    case 9:
    {
        float reso = value / 25.40;
        filter.resonance(reso);
    }
    break;
    case 10:
    {
        float swp = value / 18.14;
        filter.octaveControl(swp);
    }
    break;
    case 11:
    {
        fMixer.gain(0, 0);
        fMixer.gain(1, 0);
        fMixer.gain(2, 0);
        fMixer.gain(3, 0);
        fMixer.gain(value, get_Potentiometer(3));
    }
    break;
    case 12:
    {
        int attack = map(value, 0, MIDI_CC_RANGE, 0, 1000);
        Fenv.attack(attack);
        Aenv.attack(attack);
    }
    break;
    case 13:
    {
        int decay = map(value, 0, MIDI_CC_RANGE, 0, 500);
        Fenv.decay(decay);
        Aenv.decay(decay);
    }
    break;
    case 14:
    {
        float ampl = value / MIDI_CC_RANGE_FLOAT;
        Fenv.sustain(ampl);
        Aenv.sustain(ampl);
    }
    break;
    case 15:
    {
        int release = map(value, 0, MIDI_CC_RANGE, 0, 2000);
        Fenv.release(release);
        Aenv.release(release);
    }
    break;
    default:
        break;
    }
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
    for (int i = 0; i < GRID_LENGTH_HOR; i++)
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
        tft.fillRect(32, YPos * STEP_FRAME_H, GRID_LENGTH_HOR, 41, ILI9341_DARKGREY);
        oldxPosPeak = 0;
    }
    // Serial.printf("xPos=%d, ypos=%i\n", xPosPeak, peakHeight);
    tft.drawLine(oldxPosPeak + 32, (oldPeakHeight) + ((YPos)*STEP_FRAME_H), xPosPeak + 32, (peakHeight) + ((YPos)*STEP_FRAME_H), ILI9341_WHITE);
    oldxPosPeak = xPosPeak;
    oldPeakHeight = peakHeight;
}
void Plugin_14::smooth_customWaveform(uint8_t YPos)
{
    for (int i = 0; i < GRID_LENGTH_HOR; i++)
    {

        if (pl14_customWaveform[i] == 0)
            for (int b = i; b < GRID_LENGTH_HOR; b++)
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
        memcpy(buffer, queue1.readBuffer(), GRID_LENGTH_HOR);
        queue1.freeBuffer();
        memcpy(buffer + GRID_LENGTH_HOR, queue1.readBuffer(), GRID_LENGTH_HOR);
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
        myFile.write((int *)queue1.readBuffer(), GRID_LENGTH_HOR);
        queue1.freeBuffer();
    }
    myFile.close();
    audio_rec_now = false;
    // audio_rec_rec = false;
}
Plugin_14 plugin_14("Rec", 14);
