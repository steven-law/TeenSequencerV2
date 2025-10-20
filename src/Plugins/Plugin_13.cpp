#include <Plugins/Plugin_13.h>

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

    MixGain.gain(1);

    setParameterNames("W~Form", MIDI_CC_RANGE, "Volume", MIDI_CC_RANGE, "0", 0, "0", 0,
                      "0", 0, "0", 0, "0", 0, "0", 0,
                      "Filter-Freq", MIDI_CC_RANGE, "Resonance", MIDI_CC_RANGE, "Sweep", MIDI_CC_RANGE, "Type", 3,
                      "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE, "ADSR", MIDI_CC_RANGE);
    set_preset(0,
               1, 80, 0, 0,
               0, 0, 0, 0,
               60, 0, 30, 0,
               0, 0, 127, 20);
    change_preset();
}
void Plugin_13::noteOn(uint8_t notePlayed, float velocity, uint8_t voice)
{
    float velo = (velocity * (MixerGain / MIDI_CC_RANGE_FLOAT));
    MixGain.gain(velo);
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
void Plugin_13::assign_parameter(uint8_t pot)
{
    uint8_t value = get_Potentiometer(pot);
    switch (pot)
    {
    case 0:
    {
        int reworkWaveform;
        reworkWaveform = value;
        if (reworkWaveform > 64)
            clearcustomWaveform(5);
        if (reworkWaveform <= 64)
            smooth_customWaveform(5);
    }
    break;
    case 1:
    {
        float ampl = value / MIDI_CC_RANGE_FLOAT;
        fMixer.gain(0, 0);
        fMixer.gain(1, 0);
        fMixer.gain(2, 0);
        fMixer.gain(get_Potentiometer(11), ampl);
    }
    break;
    case 2:

        break;
    case 3:

        break;
    case 4:
    {
        if (ts.touched())
            draw_voice_waveform(1, 1);
    }
    break;
    case 5:
    {
        if (ts.touched())
            draw_voice_waveform(1, 1);
    }
    break;
    case 6:
    {
        if (ts.touched())
            draw_voice_waveform(1, 1);
    }
    break;
    case 7:
    {
        if (ts.touched())
            draw_voice_waveform(1, 1);
    }
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
        fMixer.gain(value, get_Potentiometer(1));
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

void Plugin_13::draw_voice_waveform(uint8_t XPos, uint8_t YPos)
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

Plugin_13 plugin_13("Draw", 13);
