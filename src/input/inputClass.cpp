#include "input/InputClass.h"
Encoder Enc1(4, 3);
Encoder Enc2(29, 28);
Encoder Enc3(26, 25);
Encoder Enc4(31, 32);
Encoder *allEncoders[NUM_ENCODERS]{&Enc1, &Enc2, &Enc3, &Enc4};
Bounce *encButtons = new Bounce[NUM_ENCODERS];
XPT2046_Touchscreen ts(CS_PIN, 15);
uint8_t InputClass::getValueFromInput(uint8_t xpos, uint8_t oldValue, uint8_t max)
{
    if (potTouched[xpos])
    {
        Serial.printf("input from touch pot: %d, value = %d\n", xpos, getValueFromTouch(xpos, max));
        return getValueFromTouch(xpos, max);
    }

    else if (enc_moved[xpos])
    {
        Serial.printf("input from encoder pot: %d, value = %d\n", xpos, getValueFromEncoder(xpos, oldValue, max));
        return getValueFromEncoder(xpos, oldValue, max);
    }
}
uint8_t InputClass::getValueFromTouch(uint8_t xpos, uint8_t max)
{
    return constrain(parameterTouchY[xpos], 0, max);
}

uint8_t InputClass::getValueFromEncoder(uint8_t xpos, uint8_t oldValue, uint8_t max)
{
    return constrain(oldValue + encoded[xpos], 0, max);
}

void InputClass::encoder_setup(int dly)
{

    for (int i = 0; i < NUM_ENCODERS; i++)
    {
        encButtons[i].attach(BUTTON_PINS[i], INPUT_PULLUP); // setup the bounce instance for the current button
        encButtons[i].interval(dly);                        // interval in ms
    }
    Serial.println("Initializing Encoders");
    // tft.println("Initializing Encoders");
    // tft.updateScreenAsync();
    delay(dly);
}
void InputClass::readEncoders()
{
    for (int i = 0; i < NUM_ENCODERS; i++)
    {
        encoded[i] = 0;
        enc_moved[i] = 0;
        long newEnc = allEncoders[i]->read() / 4;
        uint8_t encMultiplier = encButtons[i].read() ? 1 : 8;

        encButtons[i].update();
        if (encButtons[i].fell())
        {
            updateTFTScreen = true;
            enc_button[i] = true;
            Serial.printf("EncButton: %d\n", i);
        }
        else if (encButtons[i].rose())
        {
            enc_button[i] = false;
        }

        if (newEnc != oldEnc[i])
        {
            if (activeScreen == INPUT_FUNCTIONS_FOR_ARRANGER || activeScreen == INPUT_FUNCTIONS_FOR_SEQUENCER)
                change_plugin_row = true;
            enc_moved[i] = true;
            active[i] = true;

            updateTFTScreen = true;
            encoded[i] = (newEnc > oldEnc[i]) ? encMultiplier : -encMultiplier;
            oldEnc[i] = newEnc;
        }
    }
}
void InputClass::encoder_SetCursor(uint8_t deltaX, uint8_t maxY)

{

    if (enc_moved[0])
    {
        pixelTouchX = constrain(pixelTouchX + encoded[0] * deltaX, 0, 480 - STEP_FRAME_W);
        // mytft->draw_sequencer_arranger_parameter(gridTouchY - 1, 0, "Bar", pixelTouchX / 16 - 2, "NO_NAME");
        // Serial.printf("encoder: %d + deltaX: %d = pixelTouchX %d\n", encoded[0], deltaX, pixelTouchX);
        // enc_moved[0] = false;
    }
    if (enc_moved[1])
    {
        gridTouchY = constrain(gridTouchY + encoded[1], 0, maxY);
        // mytft->draw_sequencer_arranger_parameter(gridTouchY - 1, 1, "Track", gridTouchY - 1, "NO_NAME");
        // enc_moved[1] = false;
    }
}

void InputClass::touch_update()
{
    if (ts.touched())
    {
        //
        TS_Point p = ts.getPoint();
        int pX = map(p.x, TS_MINX, TS_MAXX, 0, 480 - STEP_FRAME_W / 2);
        if (activeScreen == INPUT_FUNCTIONS_FOR_ARRANGER || activeScreen == INPUT_FUNCTIONS_FOR_SEQUENCER)
        {
            if (pX < SEQ_GRID_RIGHT)
            {
                tsTouched = true;
                pixelTouchX = map(p.x, TS_MINX, TS_MAXX, 0, 480 - STEP_FRAME_W / 2);
                if (activeScreen == INPUT_FUNCTIONS_FOR_ARRANGER)
                    gridTouchY = map(p.y, TS_MINY + 200, TS_MAXY - 400, 0, 8);
                else
                    gridTouchY = map(p.y, TS_MINY, TS_MAXY, 0, 14);
            }
            else if (pX >= SEQ_GRID_RIGHT)
            {
                lastPotRow = map(p.y, TS_MINY + 200, TS_MAXY - 400, 0, 3);
                change_plugin_row = true;
                change_row = true;
            }
        }
        else
        {
            if (pX < SEQ_GRID_RIGHT)
            {
                tsTouched = true;
                parameterTouchX = constrain(map(p.x, TS_MINX + 200, TS_MAXX - 600, 0, 3), 0, 3);
                parameterTouchY[parameterTouchX] = constrain(map(p.y, TS_MINY + 150, TS_MAXY - 150, 127, 0), 0, 127);
                for (int i = 0; i < NUM_ENCODERS; i++)
                {
                    potTouched[i] = false;
                    active[i] = false;
                }
                active[parameterTouchX] = true;
                potTouched[parameterTouchX] = true;
                //  Serial.printf("parameterTouchX: %d, Y: %d\n", parameterTouchX, parameterTouchY[parameterTouchX]);
            }
            else if (pX >= SEQ_GRID_RIGHT)
            {
                lastPotRow = map(p.y, TS_MINY + 200, TS_MAXY - 400, 0, 3);
                change_plugin_row = true;
                change_row = true;
            }
        }
    }
    if (!ts.touched() && tsTouched)
    {
        tsTouched = false;
        for (int i = 0; i < NUM_ENCODERS; i++)
        {
            potTouched[i] = false;
            active[i] = false;
        }
    }
}
void InputClass::touch_setup()
{
    ts.begin();
    ts.setRotation(1);
}

void InputClass::mouse_update()
{

    if (mouse1.available())
    {

        if (mouse1.getButtons() == 4) // left = 1, right = 2, scroll = 4
            neotrellisPressed[TRELLIS_POTROW] = true;

        // moving
        pixelTouchX = constrain(pixelTouchX + mouse1.getMouseX(), 0, 304);
        active[pixelTouchX / 76] = true;
        static int countmouseY;
        int mouseY;
        countmouseY = (countmouseY + mouse1.getMouseY());
        if (countmouseY % 4 == 0)
            mouseY = mouse1.getMouseY();
        else
            mouseY = 0;
        if (activeScreen == INPUT_FUNCTIONS_FOR_ARRANGER)
            gridTouchY = constrain((gridTouchY + mouseY), 0, 8);
        else
            gridTouchY = constrain((gridTouchY + mouseY), 0, 14);
        // input for sequencer
        if (activeScreen == INPUT_FUNCTIONS_FOR_SEQUENCER)
        {
            if (pixelTouchX >= 18 * STEP_FRAME_W)
            {
                if (gridTouchY == 6 && mouse1.getWheel() != 0)
                {
                    enc_moved[0] = true;
                    encoded[0] = mouse1.getWheel();
                }
                if (gridTouchY == 8 && mouse1.getWheel() != 0)
                {
                    enc_moved[1] = true;
                    encoded[1] = mouse1.getWheel();
                }
                if (gridTouchY == 10 && mouse1.getWheel() != 0)
                {
                    enc_moved[2] = true;
                    encoded[2] = mouse1.getWheel();
                }
                if (gridTouchY == 12 && mouse1.getWheel() != 0)
                {
                    enc_moved[3] = true;
                    encoded[3] = mouse1.getWheel();
                }
            }
            // buttons
            if (mouse1.getButtons() == 1) // left = 1, right = 2, scroll = 4
                neotrellisPressed[TRELLIS_BUTTON_ENTER] = true;
        }
        // inputs for arranger
        if (activeScreen == INPUT_FUNCTIONS_FOR_ARRANGER)
        {
            if (pixelTouchX >= 18 * STEP_FRAME_W)
            {
                if (gridTouchY == 6 && mouse1.getWheel() != 0)
                {
                    enc_moved[0] = true;
                    encoded[0] = mouse1.getWheel();
                }
                if (gridTouchY == 8 && mouse1.getWheel() != 0)
                {
                    enc_moved[1] = true;
                    encoded[1] = mouse1.getWheel();
                }
                if (gridTouchY == 10 && mouse1.getWheel() != 0)
                {
                    enc_moved[2] = true;
                    encoded[2] = mouse1.getWheel();
                }
                if (gridTouchY == 12 && mouse1.getWheel() != 0)
                {
                    enc_moved[3] = true;
                    encoded[3] = mouse1.getWheel();
                }
            }
            else
            {

                if (mouse1.getWheel() != 0)
                {
                    enc_moved[2] = true;
                    encoded[2] = mouse1.getWheel();
                }
                if (mouse1.getButtons() == 1)
                {
                    enc_moved[2] = true;
                    encoded[2] = 0;
                }
                if (mouse1.getButtons() == 2)
                {
                    enc_moved[2] = true;
                    encoded[2] = MAX_CLIPS - 1;
                }
            }
        }
        mouse1.mouseDataClear();
    }

    // show Mouse Infobox
}

void InputClass::detect_mouse()
{
    static bool deviceConnected = false;
    if (mouse1.available() && !deviceConnected)
    {
        set_infobox_background(750);

        deviceConnected = true;
        tft.print("Mouse connected: ");
    }
    // if disconnected
    else if (!mouse1.available() && deviceConnected)
    {
        deviceConnected = false;
        set_infobox_background(750);
        tft.println("Mouse disconnected");
    }
    reset_infobox_background();
}
InputClass::InputClass()
{
}

InputClass::~InputClass()
{
}
InputClass inputs;