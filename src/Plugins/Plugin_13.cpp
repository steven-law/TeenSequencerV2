#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "projectVariables.h"

#include <Plugins/Plugin_13.h>

extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
extern float *note_frequency;
extern int tuning;
void clearWorkSpace();
extern const char *filterName[4];

void Plugin_13::setup()
{

    dc.amplitude(1);

    waveform.begin(WAVEFORM_ARBITRARY);
    waveform.amplitude(1);
    waveform.arbitraryWaveform(pl13_customWaveform, 172.0);
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

    MixGain.gain(1);
    potentiometer[presetNr][0] = 1;
    potentiometer[presetNr][1] = 48;
    potentiometer[presetNr][8] = 60;
    potentiometer[presetNr][9] = 0;
    potentiometer[presetNr][10] = 30;
    potentiometer[presetNr][11] = 0;
    potentiometer[presetNr][12] = 5;
    potentiometer[presetNr][13] = 0;
    potentiometer[presetNr][14] = 127;
    potentiometer[presetNr][15] = 20;
    // SongVol.gain(1);
}
void Plugin_13::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    float frequency = note_frequency[notePlayed] * tuning;
    Serial.printf("pl13: notePlayed= %d\n", notePlayed);
    waveform.frequency(frequency);
    Fenv.noteOn();
    Aenv.noteOn();
}
void Plugin_13::noteOff(uint8_t notePlayed, uint8_t voice)
{
    Fenv.noteOff();
    Aenv.noteOff();
}
void Plugin_13::set_parameters(uint8_t row)
{
    draw_plugin();
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        draw_voice_waveform(0, 5, "Waveform");
        if (row == 0)
        {

            set_voice_waveform(0, 0, "W~Form");
            set_voice_amplitude(1, 0, "Volume");
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
void Plugin_13::draw_plugin()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        clearWorkSpace();
        // Serial.println("drawing plugin 2");
        drawPot(0, 0, potentiometer[presetNr][0], "W~Form");
        drawPot(1, 0, potentiometer[presetNr][1], "Volume");
        redraw_customWaveform(5);

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
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 11, 4, 4, NO_VALUE, "Prset", ILI9341_BLUE, 2, false, false);
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 12, 4, 4, presetNr, NO_NAME, ILI9341_BLUE, 2, true, false);
    }
}
void Plugin_13::change_preset()
{
    assign_voice_waveform(potentiometer[presetNr][0]);
    assign_voice_amplitude(potentiometer[presetNr][1]);

    assign_filter_frequency(potentiometer[presetNr][8]);
    assign_filter_resonance(potentiometer[presetNr][9]);
    assign_filter_sweep(potentiometer[presetNr][10]);
    selectFilterType(potentiometer[presetNr][11]);

    assign_envelope_attack(potentiometer[presetNr][12], 1000);
    assign_envelope_decay(potentiometer[presetNr][13], 500);
    assign_envelope_sustain(potentiometer[presetNr][14]);
    assign_envelope_release(potentiometer[presetNr][15], 2000);
}
void Plugin_13::set_voice_waveform(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        int reworkWaveform;
        reworkWaveform = get_Potentiometer(XPos, YPos, "W~Form");
        if (reworkWaveform > 64)
            clearcustomWaveform(5);
        if (reworkWaveform <= 64)
            smooth_customWaveform(5);
    }
    // draw singleCycle Waveform BY HAND
}
void Plugin_13::draw_voice_waveform(uint8_t XPos, uint8_t YPos, const char *name)
{

    /*if (ts.touched())
    {
        int xPos_SingleCyclePixel;
        int yPos_SingleCyclePixel;
        int oldCustomWaveformValue;
        static int old_yPos_SingleCyclePixel;
        TS_Point p = ts.getPoint();
        int SCdrawX = map(p.x, TS_MINX, TS_MAXX, 0, 320);
        int SCdrawY = map(p.y, TS_MINY, TS_MAXY, 0, 240);

        //  if (!enc_moved[0] && !enc_moved[1]) {
        xPos_SingleCyclePixel = constrain(SCdrawX, 32, 288);
        yPos_SingleCyclePixel = constrain(SCdrawY, 16, 224);

        //  if (1 < gridTouchX && gridTouchX < 18 && 0 < gridTouchY && gridTouchY < 13) {
        int arrayPos = constrain(xPos_SingleCyclePixel - 32, 0, 255);
        int singleCycleValue = map(yPos_SingleCyclePixel, 16, 224, 32768, -32768);
        pl13_customWaveform[arrayPos] = singleCycleValue;
        Serial.printf("ArrayPos=%d, sampleData=%i\n", arrayPos, singleCycleValue);
        // tft.drawPixel(xPos_SingleCyclePixel, yPos_SingleCyclePixel, ILI9341_WHITE);
        tft.drawLine(pl13_oldCustomWaveformXPos, (old_yPos_SingleCyclePixel / 5) + ((YPos)*STEP_FRAME_H), xPos_SingleCyclePixel, (yPos_SingleCyclePixel / 5) + ((YPos)*STEP_FRAME_H), ILI9341_WHITE);
        oldCustomWaveformValue = singleCycleValue;
        pl13_oldCustomWaveformXPos = xPos_SingleCyclePixel;
        old_yPos_SingleCyclePixel = yPos_SingleCyclePixel;
    }*/
    if (ts.touched())
    {
        static int oldX = -1, oldY = -1;
        TS_Point p = ts.getPoint();

        // Touch-Koordinaten skalieren
        int x = constrain(map(p.x, TS_MINX, TS_MAXX, STEP_FRAME_W * 2, (STEP_FRAME_W * 2) + 256), STEP_FRAME_W * 2, (STEP_FRAME_W * 2) + 256);
        int y = constrain(map(p.y, TS_MINY, TS_MAXY, STEP_FRAME_H, TFT_HEIGHT - STEP_FRAME_H), STEP_FRAME_H, TFT_HEIGHT - STEP_FRAME_H);
        static int draw_old_x = -1;
        static int draw_old_y = -1;
        // Array-Position bestimmen
        int arrayPos = x - STEP_FRAME_W * 2;
        int sampleVal = map(y, STEP_FRAME_H, TFT_HEIGHT - STEP_FRAME_H, 32768, -32768);

        // Falls Punkte übersprungen wurden, fehlende Werte interpolieren
        if (oldX != -1 && oldX != arrayPos)
        {
            int minX = min(oldX, arrayPos);
            int maxX = max(oldX, arrayPos);
            int minY = (oldX < arrayPos) ? oldY : sampleVal;
            int maxY = (oldX < arrayPos) ? sampleVal : oldY;

            for (int i = minX; i <= maxX; i++)
            {
                float factor = (float)(i - minX) / (maxX - minX);
                int interpolatedVal = minY + factor * (maxY - minY);
                pl13_customWaveform[i] = interpolatedVal;

                Serial.printf("Interpolated ArrayPos=%d, sampleData=%i\n", i, interpolatedVal);
            }
        }

        // Aktuelle Position speichern
        pl13_customWaveform[arrayPos] = sampleVal;
        Serial.printf("ArrayPos=%d, sampleData=%i\n", arrayPos, sampleVal);

        // Linie zeichnen für besseren visuellen Effekt
        if (oldX != -1 && oldY != -1)
        {
            tft.drawLine(draw_old_x, draw_old_y, x, y, ILI9341_WHITE);
        }

        // Letzten Punkt speichern
        oldX = arrayPos;
        oldY = sampleVal;
        draw_old_x = x;
        draw_old_y = y;
    }
}
void Plugin_13::smooth_customWaveform(uint8_t YPos)
{
    for (int i = 0; i < GRID_LENGTH_HOR; i++)
    {

        if (pl13_customWaveform[i] == 0)
            for (int b = i; b < GRID_LENGTH_HOR; b++)
                pl13_customWaveform[i] = pl13_customWaveform[i - 1] + (((pl13_customWaveform[b] / (b - (i - 1)))) * (i - (i - 1)));
    }
    redraw_customWaveform(YPos);
}
// draw waveforms
void Plugin_13::clearcustomWaveform(uint8_t YPos)
{
    // pl12WaveformAssigned = false;
    for (int i = 0; i < GRID_LENGTH_HOR; i++)
    {
        pl13_customWaveform[i] = 0;
    }
    pl13_oldCustomWaveformValue = 0;
    pl13_oldCustomWaveformXPos = 32;
    redraw_customWaveform(YPos);
}

void Plugin_13::redraw_customWaveform(int8_t YPos)
{
    for (int i = 0; i < 255; i++)
    {
        // static int startY;
        int startY = map(pl13_customWaveform[i], 32768, -32768, 0, 60);
        int endY = map(pl13_customWaveform[i + 1], 32768, -32768, 0, 60);
        tft.drawLine(i + 32, startY + ((YPos)*STEP_FRAME_H), i + 33, endY + ((YPos)*STEP_FRAME_H), ILI9341_WHITE);
        // startY=endY;
    }
}
void Plugin_13::assign_voice_waveform(uint8_t value)
{
    uint8_t walveform = map(value, 0, MIDI_CC_RANGE, 0, 27);
}

void Plugin_13::set_voice_amplitude(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_voice_amplitude(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_13::assign_voice_amplitude(uint8_t value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;
    fMixer.gain(0, ampl);
    fMixer.gain(1, ampl);
    fMixer.gain(2, ampl);
}

void Plugin_13::set_filter_frequency(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_filter_frequency(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_13::assign_filter_frequency(uint8_t value)
{

    int frequency = note_frequency[value] * tuning;

    filter.frequency(frequency);
    // ladder[i].frequency(frequency);
}

void Plugin_13::set_filter_resonance(uint8_t XPos, uint8_t YPos, const char *name, float min, float max)
{
    if (enc_moved[XPos])
    {
        assign_filter_resonance(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_13::assign_filter_resonance(uint8_t value)
{

    float reso = value / 25.40;

    filter.resonance(reso);
    // ladder[i].resonance(reso);
}
void Plugin_13::set_filter_sweep(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        assign_filter_sweep(get_Potentiometer(XPos, YPos, name));
    }
}
void Plugin_13::assign_filter_sweep(uint8_t value)
{
    float swp = value / 18.14;

    filter.octaveControl(swp);
    // ladder[i].octaveControl(swp);
}
void Plugin_13::set_filter_type(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        selectFilterType(constrain(get_Potentiometer(XPos, YPos, name), 0, 3));
    }
}
void Plugin_13::selectFilterType(uint8_t mixerchannel)
{

    fMixer.gain(0, 0);
    fMixer.gain(1, 0);
    fMixer.gain(2, 0);
    fMixer.gain(3, 0);
    fMixer.gain(mixerchannel, 1);
}

void Plugin_13::assign_envelope_attack(uint8_t value, int max)
{
    int attack = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.attack(attack);
    Aenv.attack(attack);
}
void Plugin_13::assign_envelope_decay(uint8_t value, int max)
{
    int decay = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.decay(decay);
    Aenv.decay(decay);
}
void Plugin_13::assign_envelope_sustain(uint8_t value)
{
    float ampl = value / MIDI_CC_RANGE_FLOAT;

    Fenv.sustain(ampl);
    Aenv.sustain(ampl);
}
void Plugin_13::assign_envelope_release(uint8_t value, int max)
{
    int release = map(value, 0, MIDI_CC_RANGE, 0, max);

    Fenv.release(release);
    Aenv.release(release);
}
void Plugin_13::set_envelope_ADSR(uint8_t YPos, int maxA, int maxD, int maxR)
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
void Plugin_13::set_envelope_attack(uint8_t XPos, uint8_t YPos, const char *name, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_attack(get_Potentiometer(XPos, YPos, name), max);
    }
}
void Plugin_13::set_envelope_decay(uint8_t XPos, uint8_t YPos, const char *name, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_decay(get_Potentiometer(XPos, YPos, name), max);
    }
}
void Plugin_13::set_envelope_sustain(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {

        assign_envelope_sustain(get_Potentiometer(XPos, YPos, name) / MIDI_CC_RANGE_FLOAT);
    }
}
void Plugin_13::set_envelope_release(uint8_t XPos, uint8_t YPos, const char *name, int max)
{
    if (enc_moved[XPos])
    {
        assign_envelope_release(get_Potentiometer(XPos, YPos, name), max);
    }
}

Plugin_13 plugin_13("Draw", 12);
