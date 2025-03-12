#include "output/tft.h"
#include <font_Arial.h>

// Teensy 4.1 PINOUT
// Pinout for screen
#define TIRQ_PIN 15                                                       // alternate Pins: any digital pin
#define TFT_DC 9                                                          // alternate Pins 9, 10, 20, 21
#define TFT_CS 10                                                         // alternate Pins 9, 15, 20, 21
#define TFT_RST 255                                                       // 255 = unused, connect to 3.3V
#define TFT_MOSI 11                                                       // shareable
#define TFT_SCLK 13                                                       // shareable
#define TFT_MISO 12                                                       // shareable
Adafruit_ST7796S_kbv tft = Adafruit_ST7796S_kbv(TFT_CS, TFT_DC, TFT_RST); //, TFT_MOSI, TFT_SCLK, TFT_MISO); // initiate TFT-Srceen
//Adafruit_ST7796S_kbv tft = Adafruit_ST7796S_kbv(TFT_CS, TFT_DC,  TFT_MOSI, TFT_SCLK, TFT_RST, TFT_MISO);
// ILI9341_t3n tft =                  ILI9341_t3n(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO); // initiate TFT-Srceen
//  screen
unsigned long infoboxTimeAtCall = 0;
unsigned long infoboxTimeAtPress = 0;
unsigned long infoboxWaitingTime = 1000;
bool infoboxShow = false;
bool infoboxClear = false;

// DMAMEM uint16_t tft_frame_buffer[ILI9341_TFTWIDTH * ILI9341_TFTHEIGHT];

void tft_setup(int dly)
{
    tft.begin();
tft.setSPISpeed(26000000);

    tft.setRotation(3);

    // tft.setFrameBuffer(tft_frame_buffer);
    // tft.useFrameBuffer(true);

    tft.fillScreen(ILI9341_BLACK); // Xmin, Ymin, Xlength, Ylength, color
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(1);
    tft.setCursor(0, 3);
    Serial.println("Initializing Touchscreen...");
    tft.println("Initializing Touchscreen...");
    // tft.updateScreen();
    delay(dly);
}
void clearWorkSpace()
{                                                                                                         // clear the whole grid from Display
    tft.fillRect(STEP_FRAME_W, STEP_FRAME_H, STEP_FRAME_W * 21, STEP_FRAME_H * 13 + 4, ILI9341_DARKGREY); // Xmin, Ymin, Xlength, Ylength, color
                                                                                                          // tft.fillRect(STEP_FRAME_W, STEP_FRAME_H, STEP_FRAME_W, STEP_FRAME_H * 12, ILI9341_DARKGREY);
}

void drawActiveRecording(bool _recState)
{
    int _color = ILI9341_LIGHTGREY;
    if (_recState)
        _color = ILI9341_RED;
    tft.drawRect(STEP_FRAME_W * POSITION_RECORD_BUTTON, 0, STEP_FRAME_W, STEP_FRAME_H, ILI9341_WHITE);
    tft.fillCircle(STEP_FRAME_W * POSITION_RECORD_BUTTON + 12, 10, 6, _color);
}
void drawActivePlaying(bool _playstate)
{
    int _color = ILI9341_DARKGREY;
    if (_playstate)
        _color = ILI9341_GREEN;
    tft.fillRect(STEP_FRAME_W * POSITION_PLAY_BUTTON, 0, STEP_FRAME_W * 2, STEP_FRAME_H, _color);
    tft.drawRect(STEP_FRAME_W * POSITION_PLAY_BUTTON, 0, STEP_FRAME_W * 2, STEP_FRAME_H, ILI9341_LIGHTGREY); // PLAY RECT FRAME
    tft.fillTriangle(
        STEP_FRAME_W * POSITION_PLAY_BUTTON + 12 + 6,
        3,
        STEP_FRAME_W * POSITION_PLAY_BUTTON + 12 + 6,
        13,
        STEP_FRAME_W * POSITION_PLAY_BUTTON + 22 + 6,
        8,
        ILI9341_LIGHTGREY); // x1, y1, x2, y2, x3, y3
}
void drawPositionCounter()
{
    // draw phrasenumber
    tft.fillRect(STEP_FRAME_W * POSITION_STOP_BUTTON + 1, 2, STEP_FRAME_W * 3 - 2, STEP_FRAME_H - 3, ILI9341_DARKGREY);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    // tft.setFont(&FreeSans9pt7b);
    tft.setCursor(STEP_FRAME_W * POSITION_STOP_BUTTON + 4, 3 + FONT_OFFSET);
    if (!myClock.isPlaying)
    {
        tft.print("0:0");
    }
    else
    {
        tft.print(myClock.barTick + 1);
        tft.print(":");
        tft.print(myClock.stepTick + 1);
    }
}

void startUpScreen()
{

    // static Display rendering
    tft.fillScreen(ILI9341_DARKGREY);

    // tft.setFont(&FreeSans9pt7b);
    tft.setTextSize(2);

    // songmode button
    tft.setTextColor(ILI9341_BLACK);
    tft.fillRect(1, 1, STEP_FRAME_W, TRACK_FRAME_H, ILI9341_MAGENTA); // Xmin, Ymin, Xlength, Ylength, color
    tft.setCursor(4, 13 + FONT_OFFSET);
    tft.print("S");

    // other tracks buttons
    for (int otherTracks = 1; otherTracks <= 8; otherTracks++)
    {
        tft.fillRect(1, TRACK_FRAME_H * otherTracks, STEP_FRAME_W, TRACK_FRAME_H, trackColor[otherTracks - 1]); // Xmin, Ymin, Xlength, Ylength, color
        tft.setCursor(4, TRACK_FRAME_H * otherTracks + 6 + FONT_OFFSET);
        tft.print(otherTracks);
    }
    // Mixer button
    tft.fillRect(1, STEP_FRAME_H * 13, STEP_FRAME_W, TRACK_FRAME_H, ILI9341_LIGHTGREY); // Xmin, Ymin, Xlength, Ylength, color
    tft.setCursor(3, STEP_FRAME_H * 13 + 14);
    tft.print("M");

    // record
    drawActiveRecording(false);

    // Play
    drawActivePlaying(false);
    // bpm counter
    tft.drawRect(STEP_FRAME_W * POSITION_STOP_BUTTON, 0, STEP_FRAME_W * 3, STEP_FRAME_H, ILI9341_LIGHTGREY);

    // tempo
    draw_value_box(3, POSITION_BPM_BUTTON, 0, 4, 4, myClock.tempo, NO_NAME, ILI9341_WHITE, 2, true, false);
    // start of loop
    draw_value_box(3, POSITION_START_LOOP_BUTTON, 0, 4, 4, myClock.startOfLoop, NO_NAME, ILI9341_WHITE, 2, true, false);
    // end of loop
    draw_value_box(3, POSITION_END_LOOP_BUTTON, 0, 4, 4, myClock.endOfLoop, NO_NAME, ILI9341_WHITE, 2, true, false);
}
void show_active_track()
{
    draw_value_box(4, 3, 0, 0, 2, active_track + 1, NO_NAME, ILI9341_WHITE, 1, false, false);
    draw_value_box(4, 1, 0, 0, 2, NO_VALUE, "Trk:", ILI9341_WHITE, 3, false, false);
}
void show_active_page_info(const char *_pagename, uint8_t _pagenumber)
{
    draw_value_box(0, 3, 0, 8, 3, _pagenumber, NO_NAME, ILI9341_WHITE, 1, false, false);
    draw_value_box(0, 1, 0, 2, 3, NO_VALUE, _pagename, ILI9341_WHITE, 2, false, false);
}
void get_infobox_background()
{
    infoboxTimeAtPress = millis();
    if (infoboxShow && infoboxTimeAtPress - infoboxTimeAtCall >= infoboxWaitingTime)
    {
        infoboxClear = true;
        infoboxShow = false;
        // infoboxTimeAtPress=0;
    }
}
void set_infobox_background(int _DisplayTime)
{
    infoboxWaitingTime = _DisplayTime;
    tft.fillRoundRect(INFOBOX_OFFSET, INFOBOX_OFFSET, INFO_BOX_WIDTH, INFO_BOX_HEIGTH, 5, ILI9341_BLACK);
    tft.drawRoundRect(INFOBOX_OFFSET, INFOBOX_OFFSET, INFO_BOX_WIDTH, INFO_BOX_HEIGTH, 5, ILI9341_WHITE);
    tft.setTextSize(1);
    // tft.setFont(&FreeSans12pt7b);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(INFOBOX_TEXT_OFFSET, INFOBOX_TEXT_OFFSET);
    infoboxTimeAtCall = millis();
    infoboxShow = true;
}
void set_infobox_next_line(uint8_t _lineNumber)
{
    tft.setCursor(INFOBOX_TEXT_OFFSET, INFOBOX_TEXT_OFFSET + (20 * _lineNumber));
}
void reset_infobox_background()
{
    // tft.updateScreenAsync();
    if (infoboxClear)
    {
        infoboxClear = false;

        clearWorkSpace();
        change_plugin_row = true;
    }
}
void tft_show()
{
    // Serial.println("redraw Display");
    if (myClock.isPlaying)
    {
        drawstepPosition();
        drawbarPosition();
        // updateClock = false;
    }
    tft.fillRect(STEP_FRAME_W * POSITION_POTROW_BUTTON, lastPotRow * 4, STEP_FRAME_W - 1, 3, ILI9341_ORANGE);
//tft.flush();
    // tft.updateScreenAsync();
} // cursor
void drawstepPosition()
{

    // draw the songpointer positions
    for (int songPointerThickness = 0; songPointerThickness <= POSITION_POINTER_THICKNESS; songPointerThickness++)
    {
        for (int stepwidth = 1; stepwidth <= STEP_FRAME_W; stepwidth++)
        {
            tft.drawFastHLine(myClock.stepTick * stepwidth + STEP_FRAME_W * 2, STEP_POSITION_POINTER_Y + songPointerThickness, STEP_FRAME_W, ILI9341_GREEN);
            tft.drawFastHLine((myClock.stepTick - 1) * stepwidth + STEP_FRAME_W * 2, STEP_POSITION_POINTER_Y + songPointerThickness, STEP_FRAME_W, ILI9341_DARKGREY);
        }
        if (myClock.stepTick == 0)
        {
            tft.drawFastHLine(STEP_FRAME_W * 17, STEP_POSITION_POINTER_Y + songPointerThickness, STEP_FRAME_W, ILI9341_DARKGREY);
        }
    }
    // Serial.print(current);
}
void drawbarPosition()
{

    // drawbarPosition
    for (int songPointerThickness = 0; songPointerThickness <= POSITION_POINTER_THICKNESS; songPointerThickness++)
    {
        tft.drawPixel(myClock.barTick + STEP_FRAME_W * 2, (SONG_POSITION_POINTER_Y + songPointerThickness), ILI9341_GREEN);

        tft.drawFastHLine((myClock.barTick) * STEP_FRAME_W + STEP_FRAME_W * 2, BAR_POSITION_POINTER_Y + songPointerThickness, STEP_FRAME_W, ILI9341_GREEN);
        tft.drawFastHLine((myClock.barTick - 1) * STEP_FRAME_W + STEP_FRAME_W * 2, BAR_POSITION_POINTER_Y + songPointerThickness, STEP_FRAME_W, ILI9341_DARKGREY);
    }
    if (myClock.barTick == myClock.startOfLoop)
    {
        // pixelphrase = 0;
        tft.fillRect(STEP_FRAME_W * (myClock.endOfLoop + 2), BAR_POSITION_POINTER_Y, STEP_FRAME_W, POSITION_POINTER_THICKNESS + 1, ILI9341_DARKGREY);
        // tft->fillRect(STEP_FRAME_W * 2, GRID_POSITION_POINTER_Y, STEP_FRAME_W * 1, 4, ILI9341_GREEN);
    }
}
void clear_positionPointer()
{
    tft.fillRect(STEP_FRAME_W * 2, SONG_POSITION_POINTER_Y, STEP_FRAME_W * NUM_STEPS, POSITION_POINTER_THICKNESS * 4, ILI9341_DARKGREY);
}
void moveCursor(int pixelOnX, int pixelOnY, int cursorDeltaX, int cursorDeltaY)
{
    static int last_xPos;
    static int last_yPos;

    if (pixelOnX != last_xPos || pixelOnY != last_yPos)
    {
        int arranger_offset = 0;
        if (cursorDeltaY == TRACK_FRAME_H)
            if (pixelOnY >= 1)
                arranger_offset = 4;

        for (int pixel = 0; pixel < STEP_FRAME_W; pixel++)
        {
            tft.drawPixel(pixel + (cursorDeltaX * last_xPos), (cursorDeltaY * last_yPos) + 1 + arranger_offset, ILI9341_DARKGREY);  // draw upper line X1
            tft.drawPixel(pixel + (cursorDeltaX * last_xPos), (cursorDeltaY * last_yPos) + 15 + arranger_offset, ILI9341_DARKGREY); // draw bottom line X2
            tft.drawPixel((cursorDeltaX * last_xPos) + 1, pixel + (cursorDeltaY * last_yPos) + arranger_offset, ILI9341_DARKGREY);  // draw left line Y1
            tft.drawPixel((cursorDeltaX * last_xPos) + 15, pixel + (cursorDeltaY * last_yPos) + arranger_offset, ILI9341_DARKGREY); // draw right line Y2

            // tft.drawPixel(pixel + (cursorDeltaX * last_xPos), (cursorDeltaY * last_yPos) + 1 + arranger_offset, tftRAM[0][pixel]);  // draw upper line X1
            // tft.drawPixel(pixel + (cursorDeltaX * last_xPos), (cursorDeltaY * last_yPos) + 15 + arranger_offset, tftRAM[1][pixel]); // draw bottom line X2
            // tft.drawPixel((cursorDeltaX * last_xPos) + 1, pixel + (cursorDeltaY * last_yPos) + arranger_offset, tftRAM[2][pixel]);  // draw left line Y1
            // tft.drawPixel((cursorDeltaX * last_xPos) + 15, pixel + (cursorDeltaY * last_yPos) + arranger_offset, tftRAM[3][pixel]); // draw right line Y2
        }
        //  for (int pixel = 0; pixel < 16; pixel++)
        //  {
        //      tftRAM[0][pixel] = tft.readPixel(pixel + (cursorDeltaX * pixelOnX), (cursorDeltaY * pixelOnY) + 1 + arranger_offset);  // save upper line
        //      tftRAM[1][pixel] = tft.readPixel(pixel + (cursorDeltaX * pixelOnX), (cursorDeltaY * pixelOnY) + 15 + arranger_offset); // save bottom line
        //      tftRAM[2][pixel] = tft.readPixel((cursorDeltaX * pixelOnX) + 1, pixel + (cursorDeltaY * pixelOnY) + arranger_offset);  // save left line
        //      tftRAM[3][pixel] = tft.readPixel((cursorDeltaX * pixelOnX) + 15, pixel + (cursorDeltaY * pixelOnY) + arranger_offset); // save right line
        //  }

        tft.drawRect((cursorDeltaX * last_xPos) + 1, (cursorDeltaY * last_yPos) + 1 + arranger_offset, STEP_FRAME_W - 1, STEP_FRAME_H - 1, ILI9341_DARKGREY);

        //  uint8_t test = tft.readPixel(18, 18); // save right line
        tft.drawRect((cursorDeltaX * pixelOnX) + 1, (cursorDeltaY * pixelOnY) + 1 + arranger_offset, STEP_FRAME_W - 1, STEP_FRAME_H - 1, ILI9341_WHITE);
        //  Serial.printf("read pixel = %d\n", test);
        last_xPos = pixelOnX;
        last_yPos = pixelOnY;
    }
    //
}

void drawPot(int XPos, uint8_t YPos, int dvalue, const char *dname)
{
    static float circlePos[4];
    static float circlePos_old[4];

    // XPos und YPos Farben und Positionen definieren
    const int xPosValues[4] = {3, 7, 11, 15};
    const int colors[4] = {ILI9341_BLUE, ILI9341_RED, ILI9341_GREEN, ILI9341_WHITE};

    int xPos = xPosValues[XPos];
    int color = (YPos != lastPotRow) ? ILI9341_LIGHTGREY : colors[XPos];
    int yPos = (YPos + 1) * 3;

    circlePos[XPos] = dvalue / 63.5;

    // Zeichnen der Wertbox
    draw_value_box(YPos, xPos, yPos + 1, 0, 3, NO_VALUE, dname, color, 2, false, false);

    // Zeichnen des Rechtecks
    tft.fillRect((xPos * STEP_FRAME_W), (yPos * STEP_FRAME_H) - 4, 2 * STEP_FRAME_W, 10, ILI9341_DARKGREY);

    // Textwert anzeigen
    // tft.setFont(&FreeSans9pt7b);
    tft.setTextSize(1);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor((STEP_FRAME_W * xPos) + 7, (yPos * STEP_FRAME_H) - 3);
    tft.print(dvalue);

    // Kreis zeichnen (alte und neue Position)
    tft.fillCircle(STEP_FRAME_W * (xPos + 1) + 16 * cos((2.5 * circlePos_old[XPos]) + 2.25),
                   STEP_FRAME_H * yPos + 16 * sin((2.5 * circlePos_old[XPos]) + 2.25), 4, ILI9341_DARKGREY);
    tft.drawCircle(STEP_FRAME_W * (xPos + 1), STEP_FRAME_H * yPos, 16, ILI9341_LIGHTGREY);
    tft.fillCircle(STEP_FRAME_W * (xPos + 1) + 16 * cos((2.5 * circlePos[XPos]) + 2.25),
                   STEP_FRAME_H * yPos + 16 * sin((2.5 * circlePos[XPos]) + 2.25), 4, color);

    // Speichern der alten Werte
    circlePos_old[XPos] = circlePos[XPos];
}
void drawEnvelope(uint8_t YPos, uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release)
{
    // int yPos;
    int colorA = ILI9341_BLUE;
    int colorD = ILI9341_RED;
    int colorS = ILI9341_GREEN;
    int colorR = ILI9341_WHITE;

    if (YPos != lastPotRow)
    {
        colorA = ILI9341_LIGHTGREY;
        colorD = ILI9341_LIGHTGREY;
        colorS = ILI9341_LIGHTGREY;
        colorR = ILI9341_LIGHTGREY;
    }
    uint8_t ypos = ((YPos + 1) * 3);
    int yPos = (ypos + 1) * STEP_FRAME_H;
    uint8_t envStart = 48;
    uint8_t envTop = yPos - 32;

    static uint8_t old_attackEnd;
    static uint8_t old_decayEnd;
    static uint8_t old_sustainLevel;
    static uint8_t old_sustainEnd;
    static uint8_t old_releaseEnd;

    tft.drawLine(envStart, yPos, old_attackEnd, envTop, ILI9341_DARKGREY);
    tft.drawLine(old_attackEnd, envTop, old_decayEnd + old_attackEnd, old_sustainLevel, ILI9341_DARKGREY);
    tft.drawLine(old_decayEnd + old_attackEnd, old_sustainLevel, old_decayEnd + old_attackEnd + old_sustainEnd, old_sustainLevel, ILI9341_DARKGREY);
    tft.drawLine(old_decayEnd + old_attackEnd + old_sustainEnd, old_sustainLevel, old_decayEnd + old_attackEnd + old_sustainEnd + old_releaseEnd, yPos, ILI9341_DARKGREY);

    uint8_t attackEnd = map(attack, 0, 127, 0, 50) + envStart;
    uint8_t decayEnd = map(decay, 0, 127, 0, 30);
    uint8_t sustainLevel = yPos - map(sustain, 0, 127, 0, 32);
    uint8_t sustainEnd = 30;
    uint8_t releaseEnd = map(release, 0, 127, 0, 50);

    tft.drawLine(envStart, yPos, attackEnd, envTop, colorA);
    tft.drawLine(attackEnd, envTop, decayEnd + attackEnd, sustainLevel, colorD);
    tft.drawLine(decayEnd + attackEnd, sustainLevel, decayEnd + attackEnd + sustainEnd, sustainLevel, colorS);
    tft.drawLine(decayEnd + attackEnd + sustainEnd, sustainLevel, decayEnd + attackEnd + sustainEnd + releaseEnd, yPos, colorR);

    draw_value_box(YPos, 14, ypos - 1, 4, 4, attack, NO_NAME, colorA, 2, true, false);
    draw_value_box(YPos, 16, ypos - 1, 4, 4, decay, NO_NAME, colorD, 2, true, false);
    draw_value_box(YPos, 14, ypos, 4, 4, sustain, NO_NAME, colorS, 2, true, false);
    draw_value_box(YPos, 16, ypos, 4, 4, release, NO_NAME, colorR, 2, true, false);

    old_attackEnd = attackEnd;
    old_decayEnd = decayEnd;
    old_sustainLevel = sustainLevel;
    old_sustainEnd = sustainEnd;
    old_releaseEnd = releaseEnd;
}
void draw_sequencer_arranger_parameter(uint8_t _track, uint8_t _encoder, const char *_name, int _value, const char *_valuedName)
{
    Serial.printf("drawing seq-Arr parameter %s\n", _name);
    uint8_t _ypos = _encoder * 2;
    draw_value_box(0, SEQUENCER_OPTIONS_VERY_RIGHT, (_ypos) + 5, 4, 4, NO_VALUE, _name, encoder_colour[_encoder], 2, false, false);
    draw_value_box(0, SEQUENCER_OPTIONS_VERY_RIGHT, (_ypos) + 6, 4, 4, _value, NO_NAME, encoder_colour[_encoder], 2, true, false);
    if (_valuedName != "NO_NAME")
        draw_value_box(0, SEQUENCER_OPTIONS_VERY_RIGHT, (_ypos) + 6, 4, 4, NO_VALUE, _valuedName, encoder_colour[_encoder], 2, true, false);
}

void draw_value_box(uint8_t lastPRow, uint8_t XPos, uint8_t YPos, uint8_t offest_X, int offset_Y, int _value, const char *name, int color, uint8_t _size, bool drawRect, bool drawFilling)
{
    int xPos = XPos * STEP_FRAME_W;
    int yPos = YPos * STEP_FRAME_H;
    int Color = (lastPotRow != lastPRow) ? ILI9341_LIGHTGREY : color; // Direkt die Farbe setzen
    tft.setTextSize(1);
    // tft.setFont(&FreeSans9pt7b);
    tft.fillRect(xPos, yPos, _size * STEP_FRAME_W, STEP_FRAME_H, ILI9341_DARKGREY);
    // Rechteck und Füllung zeichnen
    if (drawFilling)
    {
        tft.fillRect(xPos, yPos, _size * STEP_FRAME_W, STEP_FRAME_H, Color);
    }
    if (drawRect)
    {
        tft.drawRect(xPos, yPos, _size * STEP_FRAME_W, STEP_FRAME_H, Color);
    }

    // Extra Bedingung für letzte Zeile
    if (lastPRow == 5)
    {
        tft.drawRect(xPos, yPos, _size * STEP_FRAME_W, STEP_FRAME_H, ILI9341_WHITE);
        tft.fillRect(xPos, yPos, _size * STEP_FRAME_W, STEP_FRAME_H, color);
    }

    // Text zeichnen
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(xPos + offest_X, yPos + offset_Y + FONT_OFFSET);

    if (name != "NO_NAME")
    {
        tft.setTextSize(1);
        tft.print(name); // Wenn Name nicht "NO_NAME", drucken
    }
    if (_value != NO_VALUE && _value < 900)
    {
        tft.setTextSize(2);
        tft.print(_value); // Wenn Wert nicht NO_VALUE, drucken
    }
}

void drawsongmodepageselector(int songpageNumber)
{
    // draw 16 rects of 16x16px in the 13th row
    for (int pages = 2; pages < 18; pages++)
    {
        // drawActiveRect(pages, 13, 1, 1, selectPage == pages + 8, "", ILI9341_LIGHTGREY);
        tft.fillRect(STEP_FRAME_W * pages, STEP_FRAME_H * 13 + 4, STEP_FRAME_W, STEP_FRAME_H, ILI9341_DARKGREY);
        if (pages == songpageNumber + 2)
            tft.fillRect(STEP_FRAME_W * (songpageNumber + 2), STEP_FRAME_H * 13 + 4, STEP_FRAME_W, STEP_FRAME_H, ILI9341_LIGHTGREY);

        tft.drawRect(STEP_FRAME_W * pages, STEP_FRAME_H * 13 + 4, STEP_FRAME_W, STEP_FRAME_H, ILI9341_WHITE);
        // tft.setFont(&FreeSans9pt7b);
        tft.setTextSize(2);
        tft.setTextColor(ILI9341_BLACK);
        tft.setCursor(STEP_FRAME_W * pages + 1, STEP_FRAME_H * 13 + FONT_OFFSET + 8);
        tft.print((pages - 1));
    }
}
void gridSongMode(int songpageNumber)
{ // static Display rendering
    // int page_phrase_start = songpageNumber * 16;
    // int page_phrase_end = (songpageNumber + 1) * 16;
    clearWorkSpace();

    // drawActiveRect(18, 3, 2, 2, false, "clear", ILI9341_RED);

    // vertical pointer Lines
    for (int f = 0; f < BARS_PER_PAGE; f++)
    {                                                                                                         // do this for all phrases
        tft.drawFastVLine((f * STEP_FRAME_W) + (STEP_FRAME_W * 2), STEP_FRAME_H + 4, STEP_FRAME_H * 12, 360); //(x, y-start, y-length, color)
        if (f % 4 == 0)
        {
            tft.drawFastVLine((f * STEP_FRAME_W) + (STEP_FRAME_W * 2), STEP_FRAME_H + 4, STEP_FRAME_H * 12, 370); //(x, y-start, y-length, color)
        }
    }
    for (int i = 0; i < NUM_TRACKS; i++)
        draw_arrangment_lines(i, songpageNumber);
    change_plugin_row = true;
    draw_arranger_parameters(lastPotRow);
    drawsongmodepageselector(songpageNumber);
    show_active_page_info("Song:", songpageNumber + 1);
}
void draw_arrangment_lines(uint8_t _track, uint8_t _page) // b= active page
{
    for (int i = 0; i < 16; i++)
    {
        draw_arrangment_line(_track, i + (BARS_PER_PAGE * (_page - SONGMODE_PAGE_1)));
        // Serial.printf("active page = %d, which bar = %d\n", b, i + (16 * (b - SONGMODE_PAGE_1)));
    }
}
void draw_arranger_parameters(uint8_t lastProw)
{
    if (change_plugin_row)
    {
        // drawOctaveNumber();
        tft.fillRect(18 * STEP_FRAME_W, 5 * STEP_FRAME_H, 20 * STEP_FRAME_W, 12 * STEP_FRAME_H, ILI9341_DARKGREY);
        change_plugin_row = false;
        if (lastProw == 0)
        {
            draw_sequencer_arranger_parameter(gridTouchY - 1, 0, "Bar", ((pixelTouchX / STEP_FRAME_W) + (arrangerpage * BARS_PER_PAGE)) - 1, "NO_NAME");
            draw_sequencer_arranger_parameter(gridTouchY - 1, 1, "Trk", gridTouchY - 1, "NO_NAME");
            draw_sequencer_arranger_parameter(gridTouchY - 1, 2, "Clip", allTracks[gridTouchY - 1]->clip_to_play[((pixelTouchX / STEP_FRAME_W) + (arrangerpage * BARS_PER_PAGE)) - 2], "NO_NAME");
            draw_sequencer_arranger_parameter(gridTouchY - 1, 3, "Trns", allTracks[gridTouchY - 1]->noteOffset[((pixelTouchX / STEP_FRAME_W) + (arrangerpage * BARS_PER_PAGE)) - 2], "NO_NAME");

            //
            // draw_velocity(3, 0);
        }
        if (lastProw == 1)
        {
            draw_sequencer_arranger_parameter(gridTouchY - 1, 0, "Bar", ((pixelTouchX / STEP_FRAME_W) + (arrangerpage * BARS_PER_PAGE)) - 1, "NO_NAME");
            draw_sequencer_arranger_parameter(gridTouchY - 1, 1, "Trk", gridTouchY - 1, "NO_NAME");
            draw_sequencer_arranger_parameter(gridTouchY - 1, 2, "Clip", allTracks[gridTouchY - 1]->clip_to_play[((pixelTouchX / STEP_FRAME_W) + (arrangerpage * BARS_PER_PAGE)) - 2], "NO_NAME");
            draw_sequencer_arranger_parameter(gridTouchY - 1, 3, "Velo", allTracks[gridTouchY - 1]->barVelocity[((pixelTouchX / STEP_FRAME_W) + (arrangerpage * BARS_PER_PAGE)) - 2], "NO_NAME");
        }
        if (lastProw == 2)
        {
            draw_sequencer_arranger_parameter(gridTouchY - 1, 0, "Bar", ((pixelTouchX / STEP_FRAME_W) + (arrangerpage * BARS_PER_PAGE)) - 1, "NO_NAME");
            draw_sequencer_arranger_parameter(gridTouchY - 1, 1, "Trk", gridTouchY - 1, "NO_NAME");
            draw_sequencer_arranger_parameter(gridTouchY - 1, 2, "ccC", allTracks[gridTouchY - 1]->play_presetNr_ccChannel[((pixelTouchX / STEP_FRAME_W) + (arrangerpage * BARS_PER_PAGE)) - 2], "NO_NAME");
            draw_sequencer_arranger_parameter(gridTouchY - 1, 3, "ccV", allTracks[gridTouchY - 1]->play_presetNr_ccValue[((pixelTouchX / STEP_FRAME_W) + (arrangerpage * BARS_PER_PAGE)) - 2], "NO_NAME");
        }
    }
}

void draw_arrangment_line(uint8_t _trackNr, uint8_t _bar)
{
    auto *track = allTracks[_trackNr]; // Zeiger für besseren Zugriff
    int minY = map(track->barVelocity[_bar], 0, 127, 0, 10);
    int clip = track->clip_to_play[_bar];

    // Standardfarben setzen
    int _color = ILI9341_DARKGREY;
    int _trelliscolor = TRELLIS_BLACK;

    // Falls Clip existiert, Farbe anpassen
    if (clip < MAX_CLIPS - 1)
    {
        int baseColorIndex = track->my_Arranger_Y_axis - 1;
        _color = trackColor[baseColorIndex] + (clip * 20);
        _trelliscolor = trellisTrackColor[baseColorIndex] + (clip * 20);
    }

    int x_start = ((_bar - (16 * arrangerpage)) * STEP_FRAME_W + STEP_FRAME_W * 2) + 1;
    int y_base = (track->my_Arranger_Y_axis * TRACK_FRAME_H) + 12;

    // Hintergrundlinie zeichnen
    for (int thickness = -15; thickness < 15; thickness++)
    {
        tft.drawFastHLine(x_start, y_base + thickness, STEP_FRAME_W - 1, ILI9341_DARKGREY);
    }

    // Aktive Clip-Farbe zeichnen
    for (int thickness = -minY; thickness < minY; thickness++)
    {
        tft.drawFastHLine(x_start, y_base + thickness, STEP_FRAME_W - 1, _color);
    }

    // Falls ein Clip existiert, Werte einzeichnen
    if (clip < MAX_CLIPS - 1)
    {
       
            draw_arrangerLine_value(_trackNr, _bar, clip, POSITION_TEXT_ARRANGERLINE_TOP);
            draw_arrangerLine_value(_trackNr, _bar, track->noteOffset[_bar], POSITION_TEXT_ARRANGERLINE_BOTTOM);
       
            draw_arrangerLine_value(_trackNr, _bar, track->play_presetNr_ccChannel[_bar], POSITION_CC_ARRANGERLINE_TOP);
            draw_arrangerLine_value(_trackNr, _bar, track->play_presetNr_ccValue[_bar], POSITION_CC_ARRANGERLINE_BOTTOM);
        
    }

    // Trellis LED Buffer aktualisieren
    trellis_set_main_buffer(arrangerpage + TRELLIS_SCREEN_ARRANGER_1, (_bar % 16), track->my_Arranger_Y_axis - 1, _trelliscolor);
    trellis_recall_main_buffer(arrangerpage + TRELLIS_SCREEN_ARRANGER_1);
}

void draw_arrangerLine_value(uint8_t _trackNr, uint8_t _bar, int value, int y_offset)
{
    int xoffset = 0;
    int x_offset = 0;

    if (allTracks[_trackNr]->noteOffset[_bar] < 0)
        xoffset = 1;
    else if (allTracks[_trackNr]->noteOffset[_bar] >= 0)
        xoffset = 7;
    if (y_offset == POSITION_TEXT_ARRANGERLINE_TOP)
        x_offset = 2;
    if (y_offset == POSITION_TEXT_ARRANGERLINE_BOTTOM)
        x_offset = xoffset;
    if (y_offset == POSITION_CC_ARRANGERLINE_TOP)
        x_offset = 12;
    if (y_offset == POSITION_CC_ARRANGERLINE_BOTTOM)
        x_offset = 18;
    tft.setTextSize(1);
    // tft.setFont(&FreeSans9pt7b);

    tft.setTextColor(ILI9341_BLACK);
    tft.setCursor((_bar - (BARS_PER_PAGE * arrangerpage)) * STEP_FRAME_W + STEP_FRAME_W * 2 + x_offset,
                  (allTracks[_trackNr]->my_Arranger_Y_axis) * TRACK_FRAME_H + y_offset);
    tft.print(value);
}

// stepsequencer
void drawStepSequencerStatic()
{
    clearWorkSpace();
    draw_Notenames();
    drawOctaveTriangle();
    draw_Clipselector();
    // draw the Main Grid
    for (int i = 0; i < 17; i++)
    { // vert Lines
        int step_Frame_X = i * 18;
        tft.drawFastVLine(step_Frame_X + (STEP_FRAME_W * 2), STEP_FRAME_H, GRID_LENGTH_VERT, ILI9341_WHITE); //(x, y-start, length, color)
        if (i % 4 == 0)
        {
            tft.drawFastVLine(step_Frame_X + (STEP_FRAME_W * 2), STEP_FRAME_H, GRID_LENGTH_VERT, ILI9341_LIGHTGREY); //(x, y-start, y-length, color)
        }
    }
    for (int i = 0; i < 13; i++)
    { // hor lines
        int step_Frame_Y = i * STEP_FRAME_H;
        tft.drawFastHLine(STEP_FRAME_W * 2, step_Frame_Y + STEP_FRAME_H, NUM_STEPS * 18, ILI9341_WHITE); //(x-start, y, length, color)
    }
    // tft->asyncUpdateActive();
}
void drawOctaveTriangle()
{
    // draw Octavebuttons
    int leftmost = STEP_FRAME_W * OCTAVE_CHANGE_LEFTMOST;
    int rightmost = STEP_FRAME_W * OCTAVE_CHANGE_RIGHTMOST;
    int UP_topmost = STEP_FRAME_H * OCTAVE_CHANGE_UP_TOPMOST;
    int UP_bottommost = STEP_FRAME_H * OCTAVE_CHANGE_UP_BOTTOMMOST;
    int DOWN_topmost = STEP_FRAME_H * OCTAVE_CHANGE_DOWN_TOPMOST;
    int DOWN_bottommost = STEP_FRAME_H * OCTAVE_CHANGE_DOWN_BOTTOMMOST;
    tft.fillRect(leftmost + 1, STEP_FRAME_H * 2, STEP_FRAME_W * 2, STEP_FRAME_H * 3, ILI9341_DARKGREY);
    tft.fillTriangle(leftmost + 1, UP_bottommost, rightmost, UP_bottommost, leftmost + STEP_FRAME_W, UP_topmost, ILI9341_LIGHTGREY);        // octave arrow up
    tft.fillTriangle(leftmost + 1, DOWN_topmost, rightmost - 2, DOWN_topmost, leftmost + STEP_FRAME_W, DOWN_bottommost, ILI9341_LIGHTGREY); // x1, y1, x2, y2, x3, y3
}
void drawOctaveNumber()
{
    // draw the octave number
    tft.fillRect(STEP_FRAME_W * 18 + 1, STEP_FRAME_H * OCTAVE_CHANGE_TEXT, STEP_FRAME_W * 2, STEP_FRAME_H * 1 + 1, ILI9341_DARKGREY);
    tft.setCursor(STEP_FRAME_W * 18 + 11, STEP_FRAME_H * OCTAVE_CHANGE_TEXT + FONT_OFFSET);
    tft.setTextSize(3);
    // tft.setFont(&FreeSans18pt7b);
    if (lastPotRow != 1)
        tft.setTextColor(ILI9341_LIGHTGREY);
    else
        tft.setTextColor(ILI9341_WHITE);

    // tft.setTextSize(1);
    tft.print(allTracks[active_track]->parameter[SET_OCTAVE]);
}
void draw_Notenames()
{
    for (int n = 0; n < MAX_VOICES; n++)
    { // hor notes
        tft.fillRect(STEP_FRAME_W, STEP_FRAME_H * n + STEP_FRAME_H, STEP_FRAME_W, STEP_FRAME_H, trackColor[active_track]);
        tft.setCursor(STEP_FRAME_W + 1, STEP_FRAME_H * n + 24 + FONT_OFFSET);

        //  tft.setFont(&FreeSans9pt7b);
        tft.setTextColor(ILI9341_BLACK);
        tft.setTextSize(2);
        tft.print(noteNames[n]);
    }
}
void draw_Clipselector()
{

    for (int ClipNr = 0; ClipNr < 8; ClipNr++)
    {
        tft.fillRect(STEP_FRAME_W * 2 * ClipNr + STEP_FRAME_W * 2 + 1, STEP_FRAME_H * 13 + 2, STEP_FRAME_W * 2 - 2, STEP_FRAME_H - 3, trackColor[active_track] + (ClipNr * 20));
        tft.setCursor(STEP_FRAME_W * 2 * ClipNr + STEP_FRAME_W * 2 + 4, STEP_FRAME_H * 13 + 8);
        // tft.setFont(&FreeSans9pt7b);
        tft.setTextColor(ILI9341_BLACK);
        tft.setTextSize(1);
        tft.print("Clip ");
        tft.print(ClipNr);
    }
}
void draw_stepSequencer_parameters(uint8_t lastProw)
{
    if (change_plugin_row)
    {
        tft.fillRect(18 * STEP_FRAME_W, 5 * STEP_FRAME_H, 20 * STEP_FRAME_W, 12 * STEP_FRAME_H, ILI9341_DARKGREY);
        change_plugin_row = false;
        drawOctaveNumber();

        if (lastProw == 0)
        {
            draw_sequencer_arranger_parameter(active_track, 0, "Tick", allTracks[active_track]->parameter[0], NO_NAME);
            draw_sequencer_arranger_parameter(active_track, 1, "Note", allTracks[active_track]->parameter[1] + (allTracks[active_track]->parameter[SET_OCTAVE] * 12), NO_NAME);
            draw_sequencer_arranger_parameter(active_track, 2, CCnames[allTracks[active_track]->parameter[2]], allTracks[active_track]->parameter[2], NO_NAME);
            draw_sequencer_arranger_parameter(active_track, 3, "Velo", allTracks[active_track]->parameter[3], NO_NAME);
            // draw_stepSequencer_parameter_value(0, 0, 0, allTracks[active_track]->parameter[0], "Tick");
            // draw_stepSequencer_parameter_value(0, 1, 0, allTracks[active_track]->parameter[1], "Note");
            //  draw_stepSequencer_parameter_value(0, 2, 0, allTracks[active_track]->parameter[2], CCnames[allTracks[active_track]->parameter[2]]);
            //  draw_stepSequencer_parameter_value(0, 3, 0, allTracks[active_track]->parameter[3], "Velo");
        }
        if (lastProw == 1)
        {
            draw_sequencer_arranger_parameter(active_track, 0, "seqL", allTracks[active_track]->parameter[4], NO_NAME);
            draw_sequencer_arranger_parameter(active_track, 1, "cDiv", allTracks[active_track]->parameter[5], NO_NAME);
            draw_sequencer_arranger_parameter(active_track, 2, "stpL", allTracks[active_track]->parameter[6], NO_NAME);
            draw_sequencer_arranger_parameter(active_track, 3, "Oct", allTracks[active_track]->parameter[7], NO_NAME);

            // draw_stepSequencer_parameter_value(1, ENCODER_SEQUENCE_LENGTH, 1, allTracks[active_track]->parameter[4], "seqL");
            // draw_stepSequencer_parameter_value(1, ENCODER_CLOCK_DIVISION, 1, allTracks[active_track]->parameter[5], "cDiv");
            // draw_stepSequencer_parameter_value(1, ENCODER_STEP_LENGTH, 1, allTracks[active_track]->parameter[6], "stpL");
            //  draw_stepSequencer_parameter_value(1, ENCODER_OCTAVE, 1, allTracks[active_track]->parameter[7], "Oct");
        }
        if (lastProw == 2)
        {
            draw_sequencer_arranger_parameter(active_track, 0, "sMod", NO_VALUE, seqModname[allTracks[active_track]->parameter[8]]);
            draw_sequencer_arranger_parameter(active_track, 1, "MCh", NO_VALUE, channelOutNames[allTracks[active_track]->parameter[9]]);

            draw_sequencer_arranger_parameter(active_track, 3, "Clip", allTracks[active_track]->parameter[11], NO_NAME);

            // draw_stepSequencer_parameter_text(2, ENCODER_SEQ_MODE, 2, seqModname[allTracks[active_track]->parameter[8]], "sMod");
            // draw_stepSequencer_parameter_text(2, ENCODER_MIDICH_OUT, 2, channelOutNames[allTracks[active_track]->parameter[9]], "MCh");
            // draw_stepSequencer_parameter_value(2, ENCODER_CLIP2_EDIT, 2, allTracks[active_track]->parameter[11], "Clip");
        }
    }
}
void draw_note_on_tick(uint8_t _note, uint8_t _when)
{

    int _color;
    // uint8_t Note = _note % NOTES_PER_OCTAVE;

    // Serial.printf("draw velocity: %d tick: %d for note: %d on voice: %d\n", velo, dr_X, note, i);
    uint8_t note = allTracks[active_track]->clip[allTracks[active_track]->parameter[SET_CLIP2_EDIT]].tick[_when].voice[_note];
    uint8_t velo = allTracks[active_track]->clip[allTracks[active_track]->parameter[SET_CLIP2_EDIT]].tick[_when].velo[_note];
    if (note == NO_NOTE)
        _color = ILI9341_DARKGREY;
    else
        _color = trackColor[active_track] + (allTracks[active_track]->parameter[SET_CLIP2_EDIT] * 20);
    if ((note >= allTracks[active_track]->parameter[SET_OCTAVE] * NOTES_PER_OCTAVE && note < (allTracks[active_track]->parameter[SET_OCTAVE] + 1) * NOTES_PER_OCTAVE) || note == NO_NOTE)
    {
        // Serial.printf("set _note= track: %d, note: %d, when: %d, color: %d \n", active_track, note, _when, _color);
        int minY = map(velo, 0, 127, 0, (STEP_FRAME_H / 4));
        int maxY = map(velo, 0, 127, 0, (STEP_FRAME_H / 4) + 1);
        for (int w = -minY; w < maxY; w++)
        {
            tft.drawPixel((_when * 3) + (STEP_FRAME_W * 2), ((_note + 1) * STEP_FRAME_H) + w + 8, _color);
            tft.drawPixel((_when * 3) + (STEP_FRAME_W * 2) + 1, ((_note + 1) * STEP_FRAME_H) + w + 8, _color);
            tft.drawPixel((_when * 3) + (STEP_FRAME_W * 2) + 2, ((_note + 1) * STEP_FRAME_H) + w + 8, _color);
        }
    }
}
void draw_notes_in_grid()
{
    for (int i = 0; i < MAX_TICKS; i++)
    {
        for (int v = 0; v < MAX_VOICES; v++)
        {
            draw_note_on_tick(v, i);
        }
    }
}

void draw_edit_presetNr_CC(const char *label, uint8_t value, uint8_t row_offset)
{
    draw_value_box(0, SEQUENCER_OPTIONS_VERY_RIGHT, (row_offset * 2) + 5, 0, 4, NO_VALUE, label, encoder_colour[active_track], 2, false, false);
    draw_value_box(0, SEQUENCER_OPTIONS_VERY_RIGHT, (row_offset * 2) + 6, 4, 4, value, NO_NAME, encoder_colour[active_track], 2, true, false);
}

void draw_MIDI_CC_screen()
{

    if (change_plugin_row)
    {
        clearWorkSpace();
        change_plugin_row = false;
        draw_MIDI_CC(0, 0);
        draw_MIDI_CC(1, 0);
        draw_MIDI_CC(2, 0);
        draw_MIDI_CC(3, 0);

        draw_MIDI_CC(0, 1);
        draw_MIDI_CC(1, 1);
        draw_MIDI_CC(2, 1);
        draw_MIDI_CC(3, 1);

        draw_MIDI_CC(0, 2);
        draw_MIDI_CC(1, 2);
        draw_MIDI_CC(2, 2);
        draw_MIDI_CC(3, 2);

        draw_MIDI_CC(0, 3);
        draw_MIDI_CC(1, 3);
        draw_MIDI_CC(2, 3);
        draw_MIDI_CC(3, 3);

        draw_edit_presetNr_CC("cc-Set", allTracks[active_track]->edit_presetNr_ccChannel, 2);
        draw_edit_presetNr_CC("vl-Set", allTracks[active_track]->edit_presetNr_ccValue, 3);
    }
}
void draw_MIDI_CC(uint8_t XPos, uint8_t YPos)
{
    int n = XPos + (YPos * NUM_ENCODERS);
    drawPot(XPos, YPos, allTracks[active_track]->CCvalue[allTracks[active_track]->edit_presetNr_ccValue][n], CCnames[allTracks[active_track]->CCchannel[allTracks[active_track]->edit_presetNr_ccChannel][n]]);
}

void draw_mixer()
{

    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, allTracks[0]->mixGainPot, "Tr D");
        draw_value_box(lastPotRow, 3, 5, 4, 4, NO_VALUE, "M", ILI9341_RED, 1, true, allTracks[0]->muted);
        draw_value_box(lastPotRow, 4, 5, 4, 4, NO_VALUE, "S", ILI9341_WHITE, 1, true, allTracks[0]->soloed);
        drawPot(1, 0, allTracks[1]->mixGainPot, "Tr 2");
        draw_value_box(lastPotRow, 7, 5, 4, 4, NO_VALUE, "M", ILI9341_RED, 1, true, allTracks[1]->muted);
        draw_value_box(lastPotRow, 8, 5, 4, 4, NO_VALUE, "S", ILI9341_WHITE, 1, true, allTracks[1]->soloed);
        drawPot(2, 0, allTracks[2]->mixGainPot, "Tr 3");
        draw_value_box(lastPotRow, 11, 5, 4, 4, NO_VALUE, "M", ILI9341_RED, 1, true, allTracks[2]->muted);
        draw_value_box(lastPotRow, 12, 5, 4, 4, NO_VALUE, "S", ILI9341_WHITE, 1, true, allTracks[2]->soloed);
        drawPot(3, 0, allTracks[3]->mixGainPot, "Tr 4");
        draw_value_box(lastPotRow, 15, 5, 4, 4, NO_VALUE, "M", ILI9341_RED, 1, true, allTracks[3]->muted);
        draw_value_box(lastPotRow, 16, 5, 4, 4, NO_VALUE, "S", ILI9341_WHITE, 1, true, allTracks[3]->soloed);

        drawPot(0, 2, allTracks[4]->mixGainPot, "Tr 5");
        draw_value_box(lastPotRow, 3, 11, 4, 4, NO_VALUE, "M", ILI9341_RED, 1, true, allTracks[4]->muted);
        draw_value_box(lastPotRow, 4, 11, 4, 4, NO_VALUE, "S", ILI9341_WHITE, 1, true, allTracks[4]->soloed);
        drawPot(1, 2, allTracks[5]->mixGainPot, "Tr 6");
        draw_value_box(lastPotRow, 7, 11, 4, 4, NO_VALUE, "M", ILI9341_RED, 1, true, allTracks[5]->muted);
        draw_value_box(lastPotRow, 8, 11, 4, 4, NO_VALUE, "S", ILI9341_WHITE, 1, true, allTracks[5]->soloed);
        drawPot(2, 2, allTracks[6]->mixGainPot, "Tr 7");
        draw_value_box(lastPotRow, 11, 11, 4, 4, NO_VALUE, "M", ILI9341_RED, 1, true, allTracks[6]->muted);
        draw_value_box(lastPotRow, 12, 11, 4, 4, NO_VALUE, "S", ILI9341_WHITE, 1, true, allTracks[6]->soloed);
        drawPot(3, 2, allTracks[7]->mixGainPot, "Tr 8");
        draw_value_box(lastPotRow, 15, 11, 4, 4, NO_VALUE, "M", ILI9341_RED, 1, true, allTracks[7]->muted);
        draw_value_box(lastPotRow, 16, 11, 4, 4, NO_VALUE, "S", ILI9341_WHITE, 1, true, allTracks[7]->soloed);
    }
}
void draw_mixer_FX_page1()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, allTracks[0]->mixDryPot, "Dry D");
        drawPot(1, 0, allTracks[0]->mixFX1Pot, "FX1 D");
        drawPot(2, 0, allTracks[0]->mixFX2Pot, "FX2 D");
        drawPot(3, 0, allTracks[0]->mixFX3Pot, "FX3 D");

        drawPot(0, 1, allTracks[1]->mixDryPot, "Dry 2");
        drawPot(1, 1, allTracks[1]->mixFX1Pot, "FX1 2");
        drawPot(2, 1, allTracks[1]->mixFX2Pot, "FX2 2");
        drawPot(3, 1, allTracks[1]->mixFX3Pot, "FX3 2");

        drawPot(0, 2, allTracks[2]->mixDryPot, "Dry 3");
        drawPot(1, 2, allTracks[2]->mixFX1Pot, "FX1 3");
        drawPot(2, 2, allTracks[2]->mixFX2Pot, "FX2 3");
        drawPot(3, 2, allTracks[2]->mixFX3Pot, "FX3 3");

        drawPot(0, 3, allTracks[3]->mixDryPot, "Dry 4");
        drawPot(1, 3, allTracks[3]->mixFX1Pot, "FX1 4");
        drawPot(2, 3, allTracks[3]->mixFX2Pot, "FX2 4");
        drawPot(3, 3, allTracks[3]->mixFX3Pot, "FX3 4");
    }
}
void draw_mixer_FX_page2()
{

    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, allTracks[4]->mixDryPot, "Dry 5");
        drawPot(1, 0, allTracks[4]->mixFX1Pot, "FX1 5");
        drawPot(2, 0, allTracks[4]->mixFX2Pot, "FX2 5");
        drawPot(3, 0, allTracks[4]->mixFX3Pot, "FX3 5");

        drawPot(0, 1, allTracks[5]->mixDryPot, "Dry 6");
        drawPot(1, 1, allTracks[5]->mixFX1Pot, "FX1 6");
        drawPot(2, 1, allTracks[5]->mixFX2Pot, "FX2 6");
        drawPot(3, 1, allTracks[5]->mixFX3Pot, "FX3 6");

        drawPot(0, 2, allTracks[6]->mixDryPot, "Dry 7");
        drawPot(1, 2, allTracks[6]->mixFX1Pot, "FX1 7");
        drawPot(2, 2, allTracks[6]->mixFX2Pot, "FX2 7");
        drawPot(3, 2, allTracks[6]->mixFX3Pot, "FX3 7");

        drawPot(0, 3, allTracks[7]->mixDryPot, "Dry 8");
        drawPot(1, 3, allTracks[7]->mixFX1Pot, "FX1 8");
        drawPot(2, 3, allTracks[7]->mixFX2Pot, "FX2 8");
        drawPot(3, 3, allTracks[7]->mixFX3Pot, "FX3 8");
    }
}

void draw_clip_launcher()
{
    Serial.println("drawing cliplauncher");
    tft.fillRect(10 * STEP_FRAME_W, 11 * STEP_FRAME_H, 80, 17, ILI9341_DARKGREY);
    // tft.setFont(&FreeSans18pt7b);
    tft.setTextSize(3);
    tft.setCursor(8 * STEP_FRAME_W, 11 * STEP_FRAME_H);
    tft.setTextColor(ILI9341_BLUE);
    tft.printf("BAR:%d\n", bar2edit);
    // clearWorkSpace();
    for (int t = 0; t < NUM_TRACKS; t++)
    {
        for (int c = 0; c < MAX_CLIPS; c++)
        {
            char *dname = "0";
            //  Serial.println("draw clip launcher");
            sprintf(dname, "T%d-C%d\0", t, c);
            if (allTracks[t]->clip_to_play[bar2edit] == c)
            {
                draw_value_box(5, c * 2 + 2, t + 2, 0, 3, NO_VALUE, dname, trackColor[t], 2, true, true);
                trellis_set_main_buffer(TRELLIS_SCREEN_CLIPLAUNCHER, c, t, trellisTrackColor[t]);
            }
            else
            {
                draw_value_box(5, c * 2 + 2, t + 2, 0, 3, NO_VALUE, dname, ILI9341_DARKGREY, 2, false, false);
                trellis_set_main_buffer(TRELLIS_SCREEN_CLIPLAUNCHER, c, t, TRELLIS_BLACK);
            }
        }
    }
}
