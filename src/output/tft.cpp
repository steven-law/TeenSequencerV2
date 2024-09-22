#include "output/tft.h"

// Teensy 4.1 PINOUT
// Pinout for screen
#define TIRQ_PIN 15                                                                   // alternate Pins: any digital pin
#define CS_PIN 14                                                                     // alternate Pins: any digital pin
#define TFT_DC 9                                                                      // alternate Pins 9, 10, 20, 21
#define TFT_CS 10                                                                     // alternate Pins 9, 15, 20, 21
#define TFT_RST 255                                                                   // 255 = unused, connect to 3.3V
#define TFT_MOSI 11                                                                   // shareable
#define TFT_SCLK 13                                                                   // shareable
#define TFT_MISO 12                                                                   // shareable
ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO); // initiate TFT-Srceen
// screen
unsigned long infoboxTimeAtCall = 0;
unsigned long infoboxTimeAtPress = 0;
unsigned long infoboxWaitingTime = 1000;
bool infoboxShow = false;
bool infoboxClear = false;

void tft_setup(int dly)
{
    tft.begin();
    tft.setRotation(3);
    // ts.begin();
    // ts.setRotation(1);
    // tft.setFrameBuffer(fb1);
    // tft.useFrameBuffer(true);

    tft.fillScreen(ILI9341_BLACK); // Xmin, Ymin, Xlength, Ylength, color
    tft.setTextColor(ILI9341_WHITE);
    tft.setFont(Arial_8);
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
void startUpScreen()
{

    // static Display rendering
    tft.fillScreen(ILI9341_DARKGREY);

    tft.setFont(Arial_9);

    // songmode button
    tft.setTextColor(ILI9341_BLACK);
    tft.fillRect(1, 1, 15, TRACK_FRAME_H, ILI9341_MAGENTA); // Xmin, Ymin, Xlength, Ylength, color
    tft.setCursor(4, 3);
    tft.print("S");

    // Drumtrack button
    tft.fillRect(1, STEP_FRAME_H + 8, 15, TRACK_FRAME_H, trackColor[0]); // Xmin, Ymin, Xlength, Ylength, color
    tft.setCursor(4, TRACK_FRAME_H + 8);
    tft.print("D");

    // other tracks buttons
    for (int otherTracks = 2; otherTracks <= 8; otherTracks++)
    {
        tft.fillRect(1, TRACK_FRAME_H * otherTracks, 15, TRACK_FRAME_H, trackColor[otherTracks - 1]); // Xmin, Ymin, Xlength, Ylength, color
        tft.setCursor(4, TRACK_FRAME_H * otherTracks + 6);
        tft.print(otherTracks);
    }
    // Mixer button
    tft.fillRect(1, STEP_FRAME_H * 13 + 8, 15, TRACK_FRAME_H, ILI9341_LIGHTGREY); // Xmin, Ymin, Xlength, Ylength, color
    tft.setCursor(3, STEP_FRAME_H * 13 + 14);
    tft.print("M");

    // scale Select
    tft.drawRect(STEP_FRAME_W * POSITION_SCALE_BUTTON, 0, STEP_FRAME_W * 2, STEP_FRAME_H, ILI9341_WHITE);

    // arrangment Select
    tft.drawRect(STEP_FRAME_W * POSITION_ARR_BUTTON, 0, STEP_FRAME_W * 2, STEP_FRAME_H, ILI9341_WHITE);

    // record button & Rectangle
    tft.drawRect(STEP_FRAME_W * POSITION_RECORD_BUTTON, 0, STEP_FRAME_W, STEP_FRAME_H, ILI9341_WHITE);
    tft.fillCircle(STEP_FRAME_W * POSITION_RECORD_BUTTON + 7, 7, 6, ILI9341_LIGHTGREY);

    // Play button & Rectangle
    tft.drawRect(STEP_FRAME_W * POSITION_PLAY_BUTTON, 0, STEP_FRAME_W * 2, STEP_FRAME_H, ILI9341_WHITE);                                                                     // PLAY RECT FRAME
    tft.fillTriangle(STEP_FRAME_W * POSITION_PLAY_BUTTON + 12, 3, STEP_FRAME_W * POSITION_PLAY_BUTTON + 12, 13, STEP_FRAME_W * POSITION_PLAY_BUTTON + 22, 8, ILI9341_GREEN); // x1, y1, x2, y2, x3, y3

    // stop button & Rectangle
    tft.drawRect(STEP_FRAME_W * POSITION_STOP_BUTTON, 0, STEP_FRAME_W, STEP_FRAME_H, ILI9341_WHITE);
    tft.fillRect(STEP_FRAME_W * POSITION_STOP_BUTTON + 3, 3, 10, 10, ILI9341_LIGHTGREY);

    // barcounter & Rectangle
    tft.drawRect(STEP_FRAME_W * POSITION_BAR_BUTTON, 0, STEP_FRAME_W * 2, STEP_FRAME_H, ILI9341_WHITE);

    // tempo button & Rectangle
    tft.drawRect(STEP_FRAME_W * POSITION_BPM_BUTTON, 0, STEP_FRAME_W * 2, STEP_FRAME_H, ILI9341_WHITE);

    // save button & Rectangle
    tft.drawRect(STEP_FRAME_W * POSITION_SAVE_BUTTON, 0, STEP_FRAME_W * 2, STEP_FRAME_H, ILI9341_WHITE);
    tft.fillRect(STEP_FRAME_W * POSITION_SAVE_BUTTON + 1, 1, STEP_FRAME_W * 2 - 2, STEP_FRAME_H - 2, ILI9341_ORANGE);
    tft.setTextColor(ILI9341_BLACK);
    tft.setFont(Arial_9);
    tft.setCursor(STEP_FRAME_W * POSITION_SAVE_BUTTON + 2, 3);
    tft.print("SAV");

    // load button & Rectangle
    tft.drawRect(STEP_FRAME_W * POSITION_LOAD_BUTTON, 0, STEP_FRAME_W, STEP_FRAME_H, ILI9341_WHITE);
    tft.fillRect(STEP_FRAME_W * POSITION_LOAD_BUTTON + 1, 1, STEP_FRAME_W - 2, STEP_FRAME_H - 2, ILI9341_GREENYELLOW);
    tft.setTextColor(ILI9341_BLACK);
    tft.setFont(Arial_9);
    tft.setCursor(STEP_FRAME_W * POSITION_LOAD_BUTTON + 4, 3);
    tft.print("L");
}
void show_active_track()
{
    draw_value_box(4, 3, 0, 0, 2, active_track + 1, NO_NAME, ILI9341_WHITE, 1, false, false);
    draw_value_box(4, 1, 0, 0, 2, NO_VALUE, "Trk:", ILI9341_WHITE, 3, false, false);
}
void show_active_page_info(const char *_pagename, byte _pagenumber)
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
    tft.setFont(Arial_10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(INFOBOX_TEXT_OFFSET, INFOBOX_TEXT_OFFSET);
    infoboxTimeAtCall = millis();
    infoboxShow = true;
}
void set_infobox_next_line(byte _lineNumber)
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
    Serial.println("redraw Display");
    // if (updateClock)
    {
        drawstepPosition();
        drawbarPosition();
        // updateClock = false;
    }
    tft.fillRect(70, lastPotRow * 4, 10, 3, ILI9341_ORANGE);

    tft.updateScreenAsync();
} // cursor
void drawstepPosition()
{

    // draw the songpointer positions
    for (int songPointerThickness = 0; songPointerThickness <= POSITION_POINTER_THICKNESS; songPointerThickness++)
    {
        for (int stepwidth = 1; stepwidth <= 16; stepwidth++)
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

    // draw phrasenumber
    tft.fillRect(STEP_FRAME_W * POSITION_BAR_BUTTON + 1, 2, STEP_FRAME_W * 2 - 2, STEP_FRAME_H - 3, ILI9341_DARKGREY);
    tft.setTextColor(ILI9341_WHITE);
    tft.setFont(Arial_9);
    tft.setCursor(STEP_FRAME_W * POSITION_BAR_BUTTON + 4, 3);
    tft.print(myClock.barTick);

    // drawbarPosition
    for (int songPointerThickness = 0; songPointerThickness <= POSITION_POINTER_THICKNESS; songPointerThickness++)
    {
        tft.drawPixel(myClock.barTick + STEP_FRAME_W * 2, (SONG_POSITION_POINTER_Y + songPointerThickness), ILI9341_GREEN);

        tft.drawFastHLine((myClock.barTick) * phraseSegmentLength + STEP_FRAME_W * 2, BAR_POSITION_POINTER_Y + songPointerThickness, phraseSegmentLength, ILI9341_GREEN);
        tft.drawFastHLine((myClock.barTick - 1) * phraseSegmentLength + STEP_FRAME_W * 2, BAR_POSITION_POINTER_Y + songPointerThickness, phraseSegmentLength, ILI9341_DARKGREY);
    }
    if (myClock.barTick == myClock.startOfLoop)
    {
        // pixelphrase = 0;
        tft.fillRect(STEP_FRAME_W * (myClock.endOfLoop + 2), BAR_POSITION_POINTER_Y, STEP_FRAME_W, POSITION_POINTER_THICKNESS + 1, ILI9341_DARKGREY);
        // tft->fillRect(STEP_FRAME_W * 2, GRID_POSITION_POINTER_Y, STEP_FRAME_W * 1, 4, ILI9341_GREEN);
    }
}
void moveCursor(int pixelOnX, int pixelOnY,  int cursorDeltaX, int cursorDeltaY)
{
   static int last_xPos;
    static int last_yPos;

    if (pixelOnX != last_xPos || pixelOnY != last_yPos)
    {
        int arranger_offset = 0;
        if (cursorDeltaY == TRACK_FRAME_H)
            if (pixelOnY >= 1)
                arranger_offset = 4;
        for (int pixel = 0; pixel < 16; pixel++)
        {
            tft.drawPixel(pixel + (cursorDeltaX * last_xPos), (cursorDeltaY * last_yPos) + 1 + arranger_offset, tftRAM[0][pixel]);  // draw upper line X1
            tft.drawPixel(pixel + (cursorDeltaX * last_xPos), (cursorDeltaY * last_yPos) + 15 + arranger_offset, tftRAM[1][pixel]); // draw bottom line X2
            tft.drawPixel((cursorDeltaX * last_xPos) + 1, pixel + (cursorDeltaY * last_yPos) + arranger_offset, tftRAM[2][pixel]);  // draw left line Y1
            tft.drawPixel((cursorDeltaX * last_xPos) + 15, pixel + (cursorDeltaY * last_yPos) + arranger_offset, tftRAM[3][pixel]); // draw right line Y2
        }
        for (int pixel = 0; pixel < 16; pixel++)
        {
            tftRAM[0][pixel] = tft.readPixel(pixel + (cursorDeltaX * pixelOnX), (cursorDeltaY * pixelOnY) + 1 + arranger_offset);  // save upper line
            tftRAM[1][pixel] = tft.readPixel(pixel + (cursorDeltaX * pixelOnX), (cursorDeltaY * pixelOnY) + 15 + arranger_offset); // save bottom line
            tftRAM[2][pixel] = tft.readPixel((cursorDeltaX * pixelOnX) + 1, pixel + (cursorDeltaY * pixelOnY) + arranger_offset);  // save left line
            tftRAM[3][pixel] = tft.readPixel((cursorDeltaX * pixelOnX) + 15, pixel + (cursorDeltaY * pixelOnY) + arranger_offset); // save right line
        }

        tft.drawRect((cursorDeltaX * pixelOnX) + 1, (cursorDeltaY * pixelOnY) + 1 + arranger_offset, STEP_FRAME_W - 1, STEP_FRAME_H - 1, ILI9341_WHITE);

        last_xPos = pixelOnX;
        last_yPos = pixelOnY;
    }
    //
}
void drawPot(int XPos, byte YPos, int dvalue, const char *dname)
{
    enc_moved[XPos] = false;
    // xposition, yposition, value 1-100, value to draw, name to draw, color
    // drawPot Variables
    static float circlePos[4];
    static float circlePos_old[4];
    static const char *dname_old[4];
    // byte fvalue = map(dvalue, 0, 127, min, max);
    int xPos;
    int color;
    if (XPos == 0)
    {
        xPos = 3;
        color = ILI9341_BLUE;
    }
    if (XPos == 1)
    {
        xPos = 7;
        color = ILI9341_RED;
    }
    if (XPos == 2)
    {
        xPos = 11;
        color = ILI9341_GREEN;
    }
    if (XPos == 3)
    {
        xPos = 15;
        color = ILI9341_WHITE;
    }
    if (YPos != lastPotRow)
        color = ILI9341_LIGHTGREY;

    int yPos = (YPos + 1) * 3;

    circlePos[XPos] = dvalue / 63.5;

    draw_value_box(YPos, xPos, yPos, 4, -3, dvalue, NO_NAME, color, 2, false, false);
    draw_value_box(YPos, xPos, yPos + 1, 0, 3, NO_VALUE, dname, color, 2, false, false);

    tft.setFont(Arial_8);
    tft.setTextColor(ILI9341_DARKGREY);
    tft.setCursor(STEP_FRAME_W * xPos, STEP_FRAME_H * (yPos + 1) + 3);
    tft.print(dname_old[XPos]);

    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(STEP_FRAME_W * xPos, STEP_FRAME_H * (yPos + 1) + 3);
    tft.print(dname);

    tft.fillCircle(STEP_FRAME_W * (xPos + 1) + 16 * cos((2.5 * circlePos_old[XPos]) + 2.25), STEP_FRAME_H * yPos + 16 * sin((2.5 * circlePos_old[XPos]) + 2.25), 4, ILI9341_DARKGREY);
    tft.drawCircle(STEP_FRAME_W * (xPos + 1), STEP_FRAME_H * yPos, 16, ILI9341_LIGHTGREY);
    tft.fillCircle(STEP_FRAME_W * (xPos + 1) + 16 * cos((2.5 * circlePos[XPos]) + 2.25), STEP_FRAME_H * yPos + 16 * sin((2.5 * circlePos[XPos]) + 2.25), 4, color);
    circlePos_old[XPos] = circlePos[XPos];

    dname_old[XPos] = dname;
}
void drawEnvelope(byte YPos, byte attack, byte decay, byte sustain, byte release)
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
    byte ypos = ((YPos + 1) * 3);
    int yPos = (ypos + 1) * STEP_FRAME_H;
    byte envStart = 48;
    byte envTop = yPos - 32;

    static byte old_attackEnd;
    static byte old_decayEnd;
    static byte old_sustainLevel;
    static byte old_sustainEnd;
    static byte old_releaseEnd;

    tft.drawLine(envStart, yPos, old_attackEnd, envTop, ILI9341_DARKGREY);
    tft.drawLine(old_attackEnd, envTop, old_decayEnd + old_attackEnd, old_sustainLevel, ILI9341_DARKGREY);
    tft.drawLine(old_decayEnd + old_attackEnd, old_sustainLevel, old_decayEnd + old_attackEnd + old_sustainEnd, old_sustainLevel, ILI9341_DARKGREY);
    tft.drawLine(old_decayEnd + old_attackEnd + old_sustainEnd, old_sustainLevel, old_decayEnd + old_attackEnd + old_sustainEnd + old_releaseEnd, yPos, ILI9341_DARKGREY);

    byte attackEnd = map(attack, 0, 127, 0, 50) + envStart;
    byte decayEnd = map(decay, 0, 127, 0, 30);
    byte sustainLevel = yPos - map(sustain, 0, 127, 0, 32);
    byte sustainEnd = 30;
    byte releaseEnd = map(release, 0, 127, 0, 50);

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
void draw_sequencer_arranger_parameter(byte _track, byte _encoder, const char *_name, int _value, const char *_valuedName)
{
    Serial.printf("drawing seq-Arr parameter %s\n", _name);
    byte _ypos = _encoder * 2;
    draw_value_box(0, SEQUENCER_OPTIONS_VERY_RIGHT, (_ypos) + 5, 4, 4, NO_VALUE, _name, encoder_colour[_encoder], 2, false, false);
    draw_value_box(0, SEQUENCER_OPTIONS_VERY_RIGHT, (_ypos) + 6, 4, 4, _value, NO_NAME, encoder_colour[_encoder], 2, true, false);
    if (_valuedName != "NO_NAME")
        draw_value_box(0, SEQUENCER_OPTIONS_VERY_RIGHT, (_ypos) + 6, 4, 4, NO_VALUE, _valuedName, encoder_colour[_encoder], 2, true, false);
}
void draw_value_box(byte lastPRow, byte XPos, byte YPos, byte offest_X, int offset_Y, int _value, const char *name, int color, byte _size, bool drawRect, bool drawFilling)
{

    int xPos = XPos * STEP_FRAME_W;
    byte yPos = YPos * STEP_FRAME_H;

    tft.setFont(Arial_8);
    // tft.setTextColor(ILI9341_DARKGREY);
    // tft.setCursor(xPos + offest_X, yPos + offset_Y);
    tft.fillRect(xPos, yPos + offset_Y, _size * STEP_FRAME_W + offest_X, STEP_FRAME_H, ILI9341_DARKGREY);
    // tft.print(old_value[index]);
    if (lastPotRow != lastPRow)
        color = ILI9341_LIGHTGREY;
    if (drawRect)
        tft.drawRect(xPos, yPos, _size * STEP_FRAME_W, STEP_FRAME_H, color);
    if (drawFilling)
        tft.fillRect(xPos, yPos, _size * STEP_FRAME_W, STEP_FRAME_H, color);

    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(xPos + offest_X, yPos + offset_Y);
    if (name != "NO_NAME")
        tft.print(name);
    if (_value != NO_VALUE)
        tft.print(_value);
    // tft.updateScreenAsync();
}

void drawsongmodepageselector()
{
    // draw 16 rects of 16x16px in the 13th row
    for (int pages = 2; pages < 18; pages++)
    {
        // drawActiveRect(pages, 13, 1, 1, selectPage == pages + 8, "", ILI9341_LIGHTGREY);
        tft.drawRect(STEP_FRAME_W * pages, STEP_FRAME_H * 13 + 4, STEP_FRAME_W, STEP_FRAME_H, ILI9341_WHITE);
        tft.setFont(Arial_8);
        tft.setTextColor(ILI9341_WHITE);
        tft.setCursor(STEP_FRAME_W * pages + 3, STEP_FRAME_H * 13 + 8);
        tft.print((pages - 2));
    }
}
void gridSongMode(int songpageNumber)
{ // static Display rendering
    // int page_phrase_start = songpageNumber * 16;
    // int page_phrase_end = (songpageNumber + 1) * 16;
    clearWorkSpace();
    drawsongmodepageselector();
    // drawActiveRect(18, 3, 2, 2, false, "clear", ILI9341_RED);

    // vertical pointer Lines
    int shownLines = 257 / phraseSegmentLength;
    for (int f = 0; f < shownLines; f++)
    {                                                                                                // do this for all phrases
        tft.drawFastVLine((f * phraseSegmentLength) + 32, STEP_FRAME_H + 4, STEP_FRAME_H * 12, 360); //(x, y-start, y-length, color)
        if (f % 4 == 0)
        {
            tft.drawFastVLine((f * phraseSegmentLength) + 32, STEP_FRAME_H + 4, STEP_FRAME_H * 12, 370); //(x, y-start, y-length, color)
        }
    }
    for (int i = 0; i < NUM_TRACKS; i++)
        draw_arrangment_lines(i, songpageNumber);
    change_plugin_row = true;
    draw_arranger_parameters(lastPotRow);

    show_active_page_info("Song:", songpageNumber);
}
void draw_arrangment_lines(byte _track, byte _page) // b= active page
{
    for (int i = 0; i < 16; i++)
    {
        draw_arrangment_line(_track, i + (BARS_PER_PAGE * (_page - SONGMODE_PAGE_1)));
        // Serial.printf("active page = %d, which bar = %d\n", b, i + (16 * (b - SONGMODE_PAGE_1)));
    }
}
void draw_arranger_parameters(byte lastProw)
{
    if (change_plugin_row)
    {
        tft.fillRect(18 * STEP_FRAME_W, 5 * STEP_FRAME_H, 20 * STEP_FRAME_W, 12 * STEP_FRAME_H, ILI9341_DARKGREY);
        change_plugin_row = false;
        if (lastProw == 0)
        {
            draw_sequencer_arranger_parameter(gridTouchY - 1, 0, "Bar", (pixelTouchX / 16) - 2, "NO_NAME");
            draw_sequencer_arranger_parameter(gridTouchY - 1, 1, "Track", gridTouchY - 1, "NO_NAME");
            draw_sequencer_arranger_parameter(gridTouchY - 1, 2, "Clip", allTracks[gridTouchY - 1]->clip_to_play[(pixelTouchX / 16) - 2], "NO_NAME");
            draw_sequencer_arranger_parameter(gridTouchY - 1, 3, "Trns", allTracks[gridTouchY - 1]->noteOffset[(pixelTouchX / 16) - 2], "NO_NAME");

            // drawOctaveNumber();
            // draw_velocity(3, 0);
        }
        if (lastProw == 1)
        {
            draw_sequencer_arranger_parameter(gridTouchY - 1, 0, "Bar", (pixelTouchX / 16) - 2, "NO_NAME");
            draw_sequencer_arranger_parameter(gridTouchY - 1, 1, "Track", gridTouchY - 1, "NO_NAME");
            draw_sequencer_arranger_parameter(gridTouchY - 1, 2, "Velo", allTracks[gridTouchY - 1]->barVelocity[(pixelTouchX / 16) - 2], "NO_NAME");
        }
        if (lastProw == 2)
        {
            draw_sequencer_arranger_parameter(gridTouchY - 1, 0, "Bar", (pixelTouchX / 16) - 2, "NO_NAME");
            draw_sequencer_arranger_parameter(gridTouchY - 1, 1, "Track", gridTouchY - 1, "NO_NAME");
            draw_sequencer_arranger_parameter(gridTouchY - 1, 2, "ccC", allTracks[gridTouchY - 1]->play_presetNr_ccChannel[(pixelTouchX / 16) - 2], "NO_NAME");
            draw_sequencer_arranger_parameter(gridTouchY - 1, 3, "ccV", allTracks[gridTouchY - 1]->play_presetNr_ccValue[(pixelTouchX / 16) - 2], "NO_NAME");
        }
    }
}
void draw_arrangment_line(byte _trackNr, byte _bar) // b= 0-255; which bar
{

    int minY = map(allTracks[_trackNr]->barVelocity[_bar], 0, 127, 0, 10);
    int _color;
    int _trelliscolor;
    if (allTracks[_trackNr]->clip_to_play[_bar] == MAX_CLIPS - 1)
    {
        _color = ILI9341_DARKGREY;
        _trelliscolor = TRELLIS_BLACK;
    }
    else
    {
        // for other clips
        _color = trackColor[allTracks[_trackNr]->my_Arranger_Y_axis - 1] + (allTracks[active_track]->clip_to_play[_bar] * 20);
        _trelliscolor = trellisTrackColor[allTracks[_trackNr]->my_Arranger_Y_axis - 1] + (allTracks[_trackNr]->clip_to_play[_bar] * 20);
    }
    for (int thickness = -10; thickness < 10; thickness++)
    {
        tft.drawFastHLine(((_bar - (16 * arrangerpage)) * STEP_FRAME_W + STEP_FRAME_W * 2) + 1, ((allTracks[_trackNr]->my_Arranger_Y_axis) * TRACK_FRAME_H + thickness) + 12, STEP_FRAME_W - 1, ILI9341_DARKGREY); //(x-start, y, length, color)
    }
    for (int thickness = -minY; thickness < minY; thickness++)
    {
        tft.drawFastHLine(((_bar - (16 * arrangerpage)) * STEP_FRAME_W + STEP_FRAME_W * 2) + 1, ((allTracks[_trackNr]->my_Arranger_Y_axis) * TRACK_FRAME_H + thickness) + 12, STEP_FRAME_W - 1, _color); //(x-start, y, length, color)
    }
    if (allTracks[_trackNr]->clip_to_play[_bar] < MAX_CLIPS - 1)
    {
        draw_clipNr_arranger(_trackNr, _bar);
        draw_offset_arranger(_trackNr, _bar);
    }
    trellis_set_main_buffer(arrangerpage + TRELLIS_SCREEN_ARRANGER_1, (_bar % 16), allTracks[_trackNr]->my_Arranger_Y_axis - 1, _trelliscolor);
}
void draw_offset_arranger(byte _trackNr, byte _bar)
{
    int xoffset;
    if (allTracks[_trackNr]->noteOffset[_bar] < 0)
        xoffset = 1;
    else
        xoffset = 5;
    // draw clipnumber in the arranger
    tft.setFont(Arial_8);
    tft.setTextColor(ILI9341_BLACK);
    tft.setCursor((_bar - (16 * arrangerpage)) * STEP_FRAME_W + STEP_FRAME_W * 2 + xoffset, (allTracks[_trackNr]->my_Arranger_Y_axis) * TRACK_FRAME_H + 11);
    tft.print(allTracks[_trackNr]->noteOffset[_bar]);
}
void draw_clipNr_arranger(byte _trackNr, byte _bar)
{
    // draw clipnumber in the arranger
    tft.setFont(Arial_8);
    tft.setTextColor(ILI9341_BLACK);
    tft.setCursor((_bar - (16 * arrangerpage)) * STEP_FRAME_W + STEP_FRAME_W * 2 + 2, (allTracks[_trackNr]->my_Arranger_Y_axis) * TRACK_FRAME_H + 6);
    tft.print(allTracks[_trackNr]->clip_to_play[_bar]);
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
        int step_Frame_X = i * 12;
        tft.drawFastVLine(step_Frame_X + STEP_FRAME_W * 2, STEP_FRAME_H, GRID_LENGTH_VERT, ILI9341_WHITE); //(x, y-start, length, color)
        if (i % 4 == 0)
        {
            tft.drawFastVLine((i * 12) + 32, STEP_FRAME_H, STEP_FRAME_H * 12, ILI9341_LIGHTGREY); //(x, y-start, y-length, color)
        }
    }
    for (int i = 0; i < 13; i++)
    { // hor lines
        int step_Frame_Y = i * 16;
        tft.drawFastHLine(STEP_FRAME_W * 2, step_Frame_Y + STEP_FRAME_H, NUM_STEPS * 12, ILI9341_WHITE); //(x-start, y, length, color)
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
void draw_Notenames()
{
    for (int n = 0; n < MAX_VOICES; n++)
    { // hor notes
        tft.fillRect(STEP_FRAME_W, STEP_FRAME_H * n + STEP_FRAME_H, STEP_FRAME_W, STEP_FRAME_H, trackColor[active_track]);
        tft.setCursor(20, STEP_FRAME_H * n + 20);
        tft.setFont(Arial_8);
        tft.setTextColor(ILI9341_BLACK);
        tft.setTextSize(1);
        tft.print(noteNames[n]);
    }
}
void draw_Clipselector()
{

    for (int ClipNr = 0; ClipNr < 8; ClipNr++)
    {
        tft.fillRect(STEP_FRAME_W * 2 * ClipNr + STEP_FRAME_W * 2 + 1, STEP_FRAME_H * 13 + 2, STEP_FRAME_W * 2 - 2, STEP_FRAME_H - 3, trackColor[active_track] + (ClipNr * 20));
        tft.setCursor(STEP_FRAME_W * 2 * ClipNr + STEP_FRAME_W * 2 + 4, STEP_FRAME_H * 13 + 4);
        tft.setFont(Arial_8);
        tft.setTextColor(ILI9341_BLACK);
        tft.setTextSize(1);
        tft.print("Clip ");
        tft.print(ClipNr);
    }
}
void draw_stepSequencer_parameters(byte lastProw)
{
    if (change_plugin_row)
    {
        tft.fillRect(18 * STEP_FRAME_W, 5 * STEP_FRAME_H, 20 * STEP_FRAME_W, 12 * STEP_FRAME_H, ILI9341_DARKGREY);
        change_plugin_row = false;
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
            draw_sequencer_arranger_parameter(active_track, 1, "sDiv", allTracks[active_track]->parameter[5], NO_NAME);
            draw_sequencer_arranger_parameter(active_track, 2, "stpL", allTracks[active_track]->parameter[6], NO_NAME);
            draw_sequencer_arranger_parameter(active_track, 3, "Oct", allTracks[active_track]->parameter[7], NO_NAME);

            // draw_stepSequencer_parameter_value(1, ENCODER_SEQUENCE_LENGTH, 1, allTracks[active_track]->parameter[4], "seqL");
            // draw_stepSequencer_parameter_value(1, ENCODER_STEP_DIVISION, 1, allTracks[active_track]->parameter[5], "sDiv");
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
void draw_note_on_tick(byte _note, byte _when)
{

    int _color;
    // byte Note = _note % NOTES_PER_OCTAVE;

    // Serial.printf("draw velocity: %d tick: %d for note: %d on voice: %d\n", velo, dr_X, note, i);
    byte note = allTracks[active_track]->clip[allTracks[active_track]->parameter[SET_CLIP2_EDIT]].tick[_when].voice[_note];
    byte velo = allTracks[active_track]->clip[allTracks[active_track]->parameter[SET_CLIP2_EDIT]].tick[_when].velo[_note];
    if (note == NO_NOTE)
        _color = ILI9341_DARKGREY;
    else
        _color = trackColor[active_track] + (allTracks[active_track]->parameter[SET_CLIP2_EDIT] * 20);
    if ((note >= allTracks[active_track]->parameter[SET_OCTAVE] * NOTES_PER_OCTAVE && note < (allTracks[active_track]->parameter[SET_OCTAVE] + 1) * NOTES_PER_OCTAVE) || note == NO_NOTE)
    {
        // Serial.printf("set _note= track: %d, note: %d, when: %d, color: %d \n", active_track, note, _when, _color);
        int minY = map(velo, 0, 127, 0, 4);
        int maxY = map(velo, 0, 127, 0, 5);
        for (int w = -minY; w < maxY; w++)
        {
            tft.drawPixel((_when * 2) + 32, ((_note + 1) * STEP_FRAME_H) + w + 8, _color);
            tft.drawPixel((_when * 2) + 32 + 1, ((_note + 1) * STEP_FRAME_H) + w + 8, _color);
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

void draw_edit_presetNr_ccValue(byte n, byte lastProw)
{
    draw_value_box(0, SEQUENCER_OPTIONS_VERY_RIGHT, (3 * 2) + 5, 0, 4, NO_VALUE, "vl-Set", encoder_colour[active_track], 2, false, false);
    draw_value_box(0, SEQUENCER_OPTIONS_VERY_RIGHT, (3 * 2) + 6, 4, 4, allTracks[active_track]->edit_presetNr_ccValue, NO_NAME, encoder_colour[active_track], 2, true, false);
}
void draw_edit_presetNr_ccChannel(byte n, byte lastProw)
{
    draw_value_box(0, SEQUENCER_OPTIONS_VERY_RIGHT, (2 * 2) + 5, 0, 4, NO_VALUE, "cc-Set", encoder_colour[active_track], 2, false, false);
    draw_value_box(0, SEQUENCER_OPTIONS_VERY_RIGHT, (2 * 2) + 6, 4, 4, allTracks[active_track]->edit_presetNr_ccChannel, NO_NAME, encoder_colour[active_track], 2, true, false);
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

        draw_edit_presetNr_ccChannel(2, 0);
        draw_edit_presetNr_ccValue(3, 0);
    }
}
void draw_MIDI_CC(byte XPos, byte YPos)
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
        drawPot(1, 0, allTracks[1]->mixDryPot, "Dry 2");
        drawPot(2, 0, allTracks[2]->mixDryPot, "Dry 3");
        drawPot(3, 0, allTracks[3]->mixDryPot, "Dry 4");

        drawPot(0, 1, allTracks[0]->mixFX1Pot, "FX1 D");
        drawPot(1, 1, allTracks[1]->mixFX1Pot, "FX1 2");
        drawPot(2, 1, allTracks[2]->mixFX1Pot, "FX1 3");
        drawPot(3, 1, allTracks[3]->mixFX1Pot, "FX1 4");

        drawPot(0, 2, allTracks[0]->mixFX2Pot, "FX2 D");
        drawPot(1, 2, allTracks[1]->mixFX2Pot, "FX2 2");
        drawPot(2, 2, allTracks[2]->mixFX2Pot, "FX2 3");
        drawPot(3, 2, allTracks[3]->mixFX2Pot, "FX2 4");

        drawPot(0, 3, allTracks[0]->mixFX3Pot, "FX3 D");
        drawPot(1, 3, allTracks[1]->mixFX3Pot, "FX3 2");
        drawPot(2, 3, allTracks[2]->mixFX3Pot, "FX3 3");
        drawPot(3, 3, allTracks[3]->mixFX3Pot, "FX3 4");
    }
}
void draw_mixer_FX_page2()
{

    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, allTracks[4]->mixDryPot, "Dry 5");
        drawPot(1, 0, allTracks[5]->mixDryPot, "Dry 6");
        drawPot(2, 0, allTracks[6]->mixDryPot, "Dry 7");
        drawPot(3, 0, allTracks[7]->mixDryPot, "Dry 8");

        drawPot(0, 1, allTracks[4]->mixFX1Pot, "FX1 5");
        drawPot(1, 1, allTracks[5]->mixFX1Pot, "FX1 6");
        drawPot(2, 1, allTracks[6]->mixFX1Pot, "FX1 7");
        drawPot(3, 1, allTracks[7]->mixFX1Pot, "FX1 8");

        drawPot(0, 2, allTracks[4]->mixFX2Pot, "FX1 5");
        drawPot(1, 2, allTracks[5]->mixFX2Pot, "FX1 6");
        drawPot(2, 2, allTracks[6]->mixFX2Pot, "FX1 7");
        drawPot(3, 2, allTracks[7]->mixFX2Pot, "FX1 8");

        drawPot(0, 3, allTracks[4]->mixFX3Pot, "FX1 5");
        drawPot(1, 3, allTracks[5]->mixFX3Pot, "FX1 6");
        drawPot(2, 3, allTracks[6]->mixFX3Pot, "FX1 7");
        drawPot(3, 3, allTracks[7]->mixFX3Pot, "FX1 8");
    }
}
