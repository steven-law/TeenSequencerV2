#include <ownLibs/myClock.h>

// ILI9341_t3n *MyClock::tft = nullptr; // Hier wird der Display-Pointer auf `nullptr` gesetzt, muss aber im Code richtig initialisiert werden
uint8_t MyClock::tempo = 120;
uint8_t MyClock::startOfLoop = 0;
uint8_t MyClock::endOfLoop = 4;
bool MyClock::isPlaying = false;
bool MyClock::syncToExtern = false;
uint8_t MyClock::MIDITick = -1;
uint8_t MyClock::stepTick = -1;
uint8_t MyClock::barTick = 0;
File MyClock::clockFile;
MyClock *MyClock::instance = nullptr;

MyClock::MyClock(int index)
{
    // MyClock::tft = display; // Initialisierung der statischen Variable
    instance = this; // Setzen der Instanz
}
void MyClock::setup()
{
    // Set the clock BPM to 120 BPM
    uClock.setTempo(tempo);
    uClock.init();                        // Inits the clock
    uClock.setOnSync24(onSync24Callback); // Set the callback function for the clock output to send MIDI Sync message.
    uClock.setOnStep(onStepCallback);     // Set the callback function for the step sequencer on 16ppqn
    uClock.setOnClockStart(onClockStart); // Set the callback function for MIDI Start and Stop messages.
    uClock.setOnClockStop(onClockStop);
}

void MyClock::onSync24Callback(uint32_t tick) // The callback function wich will be called by Clock each Pulse of 24PPQN clock resolution.
{
    // Send MIDI_CLOCK to external gears
    // Serial.write(MIDI_CLOCK);
    tick = tick % (MAX_TICKS);
    clock_to_notes(tick);
    MIDITick = tick;
    // Serial.printf("tick: %d\n", tick);
    if (syncToExtern)
        sendClock();

    //  Serial.println(tick); //passt
}
void MyClock::onStepCallback(uint32_t tick) // Each call represents exactly one step.
{

    tick = tick % NUM_STEPS;
    stepTick = tick;
    updateTFTScreen = true;

    if (tick == NUM_STEPS - 1)
    {
        barTick++;
        barTick = barTick % endOfLoop; // Setze wert2 mit Modulo-Operator zurück
    }
    // Serial.printf("Bar: %d\n", tick);

    for (int i = 0; i < NUM_TRACKS; i++)
    {
        trellisShowClockPixel[i] = true;
        trellisOut.show_clockbar(i, allTracks[i]->internal_clock / TICKS_PER_STEP);
    }
}

void MyClock::onClockStart() // The callback function wich will be called when clock starts by using Clock.start() method.
{
}
void MyClock::onClockStop() // The callback function wich will be called when clock stops by using Clock.stop() method.
{
}

void MyClock::set_tempo(uint8_t _encoder)
{
    if (enc_moved[_encoder])
    {
        change_plugin_row = true;
        tempo = constrain(tempo + encoded[_encoder], 50, 255);
        uClock.setTempo(tempo);
        draw_value_box(3, POSITION_BPM_BUTTON, 0, 4, 4, tempo, NO_NAME, ILI9341_WHITE, 2, true, false);
    }
}
void MyClock::set_syncToExtern(uint8_t _encoder)
{

    if (enc_moved[_encoder])
    {
        change_plugin_row = true;
        syncToExtern = constrain(syncToExtern + encoded[_encoder], 0, 1);
    }
}
void MyClock::set_start()
{
    uClock.start();
    isPlaying = true;
    if (syncToExtern)
        sendStart();
}
void MyClock::set_stop()
{
    uClock.stop();
    stepTick = -1;
    isPlaying = false;
    barTick = startOfLoop;
    if (syncToExtern)
        sendStop();
    allTracks[0]->save_track(15);
    allTracks[1]->save_track(15);
    allTracks[2]->save_track(15);
    allTracks[3]->save_track(15);
    allTracks[4]->save_track(15);
    allTracks[5]->save_track(15);
    allTracks[6]->save_track(15);
    allTracks[7]->save_track(15);
    myClock.save_clock(15);
}
void MyClock::draw_clock_option(uint8_t x, uint8_t v)
{
    /*tft->setCursor(STEP_FRAME_W * x + 2, 3);
    tft.setFont(Arial_10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(1);
    tft.fillRect(STEP_FRAME_W * x + 2, 1, STEP_FRAME_W * 2 - 4, STEP_FRAME_H - 2, ILI9341_DARKGREY);
    tft.print(v);
    tft.updateScreenAsync();*/
}

void MyClock::set_start_of_loop(uint8_t n)
{
    if (enc_moved[n])
    {
        change_plugin_row = true;
        startOfLoop = constrain(startOfLoop + encoded[n], 0, 254);
        draw_value_box(3, POSITION_START_LOOP_BUTTON, 0, 4, 4, startOfLoop, NO_NAME, ILI9341_WHITE, 2, true, false);

        // enc_moved[n] = false;
    }
}

void MyClock::set_end_of_loop(uint8_t n)
{
    if (enc_moved[n])
    {
        change_plugin_row = true;
        endOfLoop = constrain(endOfLoop + encoded[n], 1, 255);

        draw_value_box(3, POSITION_END_LOOP_BUTTON, 0, 4, 4, endOfLoop, NO_NAME, ILI9341_WHITE, 2, true, false);

        // enc_moved[n] = false;
    }
}

void MyClock::save_clock(uint8_t _songNr)
{
    // Serial.println("in save mode:");
    neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;

    sprintf(_trackname, "%dclock.txt", _songNr);
    Serial.println(_trackname);

    // delete the file:
    // Serial.println("Removing:");
    SD.remove(_trackname);
    delay(10);
    // Serial.println("Done:");

    // open the file.
    // Serial.println("Creating and opening:");
    clockFile = SD.open(_trackname, FILE_WRITE);
    // Serial.println(_trackname);
    // Serial.println("Done:");
    //  if the file opened okay, write to it:
    if (clockFile)
    {
        // save tracks
        // Serial.println("Writing track:");
        // Serial.println("settings & seqmodes saved:");
        uint8_t _tempo = tempo / 2;
        uint8_t _startOfLoop = startOfLoop / 2;
        uint8_t _endOfLoop = endOfLoop / 2;
        clockFile.print((char)_tempo);
        clockFile.print((char)_startOfLoop);
        clockFile.print((char)_endOfLoop);
        // close the file:
        clockFile.close();
        // Serial.println("all saved:");
        Serial.println("clock saving Done:");
    }
    else
    {
        // if the file didn't open, print an error:
        Serial.println("ERROR load clock");
    }
    delay(10);

    // startUpScreen();
}
void MyClock::load_clock(uint8_t _songNr)
{
    uint8_t _tempo;
    uint8_t _startOfLoop;
    uint8_t _endOfLoop;
    // Serial.println("in load mode");
    sprintf(_trackname, "%dclock.txt", _songNr);
    Serial.println(_trackname);
    //  open the file for reading:
    clockFile = SD.open(_trackname, FILE_READ);
    // Serial.println(_trackname);
    if (this->clockFile)
    {
        // Serial.println("opening:");
        //  read from the file until there's nothing else in it:
        //  load track 1

        _tempo = clockFile.read();
        _startOfLoop = clockFile.read();
        _endOfLoop = clockFile.read();

        // Serial.println("settings loaded:");

        // startUpScreen();
        //  close the file:
        clockFile.close();
        tempo = _tempo * 2;
        startOfLoop = _startOfLoop * 2;
        endOfLoop = _endOfLoop * 2;
        uClock.setTempo(tempo);

        draw_value_box(3, POSITION_BPM_BUTTON, 0, 4, 4, tempo, NO_NAME, ILI9341_WHITE, 2, true, false);
        draw_value_box(3, POSITION_START_LOOP_BUTTON, 0, 4, 4, startOfLoop, NO_NAME, ILI9341_WHITE, 2, true, false);
        draw_value_box(3, POSITION_END_LOOP_BUTTON, 0, 4, 4, endOfLoop, NO_NAME, ILI9341_WHITE, 2, true, false);

        Serial.println("clock Loading done");
    }
    else
    {
        // if the file didn't open, print an error:
        Serial.println("ERROR clock Loading");
    }

    delay(10);
}

// MyClock myClock(&tft);