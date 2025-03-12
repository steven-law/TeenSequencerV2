#include "input/encoder.h"
#include "projectVariables.h"

// encoder
Encoder Enc1(4, 3);
Encoder Enc2(29, 28);
Encoder Enc3(26, 25);
Encoder Enc4(31, 32);
Encoder *allEncoders[NUM_ENCODERS]{&Enc1, &Enc2, &Enc3, &Enc4};
bool enc_moved[NUM_ENCODERS]{0, 0, 0, 0};
int encoded[NUM_ENCODERS];
bool enc_button[NUM_ENCODERS];
long oldEnc[4] = {-999, -999, -999, -999};

// Encoder Buttons
const uint8_t BUTTON_PINS[NUM_ENCODERS] = {2, 27, 24, 30};
Bounce *encButtons = new Bounce[NUM_ENCODERS];
void encoder_setup(int dly)
{

    for (int i = 0; i < NUM_ENCODERS; i++)
    {
        encButtons[i].attach(BUTTON_PINS[i], INPUT_PULLUP); // setup the bounce instance for the current button
        encButtons[i].interval(25);                         // interval in ms
    }
    Serial.println("Initializing Encoders");
    // tft.println("Initializing Encoders");
    // tft.updateScreenAsync();
    delay(dly);
}
void readEncoders()
{
    long newEnc[NUM_ENCODERS];
    uint8_t encMultiplier[NUM_ENCODERS];
    for (int i = 0; i < NUM_ENCODERS; i++)
    {
        encoded[i] = 0;
        enc_moved[i] = 0;
        newEnc[i] = allEncoders[i]->read() / 4;
        encMultiplier[i] = 1;

        // Update the Bounce instance :
        encButtons[i].update();
        // If it fell, flag the need to toggle the LED
        if (encButtons[i].fell())
        {
            updateTFTScreen = true;
            enc_button[i] = true;
            Serial.printf("EncButton: %d\n", i);
        }

        if (!encButtons[i].read())
            encMultiplier[i] = 8;

        if (newEnc[i] > oldEnc[i])
        {
            if (activeScreen == INPUT_FUNCTIONS_FOR_ARRANGER || activeScreen == INPUT_FUNCTIONS_FOR_SEQUENCER)
                change_plugin_row = true;
            enc_moved[i] = true;
            updateTFTScreen = true;
            encoded[i] = encMultiplier[i];
            oldEnc[i] = newEnc[i];
          //  Serial.printf("Encoder%d: %d mult: %d\n", i, encoded[i], encMultiplier[i]);
        }
        if (newEnc[i] < oldEnc[i])
        {
            if (activeScreen == INPUT_FUNCTIONS_FOR_ARRANGER || activeScreen == INPUT_FUNCTIONS_FOR_SEQUENCER)
                change_plugin_row = true;
            enc_moved[i] = true;
            updateTFTScreen = true;
            encoded[i] = -encMultiplier[i];
            oldEnc[i] = newEnc[i];
           // Serial.printf("Encoder%d: %d mult: %d\n", i, encoded[i], encMultiplier[i]);
        }
    }
    /*
    if (encButtons[0].fell())
    {
        //neotrellisPressed[TRELLIS_BUTTON_ENTER] = true;
        // Serial.printf("EncButton: %d\n", 0);
    }
    if (encButtons[0].rose())
    {
        neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;
        // Serial.printf("EncButton: %d\n", i);
    }*/
}
void encoder_SetCursor(uint8_t deltaX, uint8_t maxY)

{

    if (enc_moved[0])
    {
        pixelTouchX = constrain(pixelTouchX + encoded[0] * deltaX, 0, 480-STEP_FRAME_W);
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