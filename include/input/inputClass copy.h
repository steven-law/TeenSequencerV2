#ifndef INPUTSYSTEM
#define INPUTSYSTEM
#include "Arduino.h"
#include "projectVariables.h"
#include <Encoder.h>
#include <Bounce2.h>
// encoder
extern Encoder Enc1;
extern Encoder Enc2;
extern Encoder Enc3;
extern Encoder Enc4;
extern Encoder *allEncoders[NUM_ENCODERS];
// Encoder Buttons
const uint8_t BUTTON_PINS[NUM_ENCODERS] = {2, 27, 24, 30};
extern Bounce *encButtons;

// touchscreen
#include <XPT2046_Touchscreen.h>
#define CS_PIN 14 // alternate Pins: any digital pin
// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 440
#define TS_MINY 300
#define TS_MAXX 3850
#define TS_MAXY 3800
extern XPT2046_Touchscreen ts;

// trellis
#include "input/trellis.h"
void trellis_update();
void revertPressedKey();

// mouse
#include <USBHost_t36.h>
extern USBHost myusb;
extern USBHIDParser hid1;
// USBHIDParser hid2(myusb);
extern MouseController mouse1;

class InputClass
{
private:
    uint8_t getValueFromTouch(uint8_t pot, uint8_t max);

    uint8_t getValueFromTrellis(uint8_t pot, uint8_t max);
    // encoder
    void encoder_setup(int dly);
    void readEncoders();
    long oldEnc[4] = {-999, -999, -999, -999};
    // touch
    void touch_setup();
    void touch_update();

    // mouse
    void detect_mouse();
    /* data */
public:
    bool active[NUM_ENCODERS] = {false, false, false, false};
    int parameterTouchX;
    bool potTouched[NUM_ENCODERS] = {false, false, false, false};
    int parameterTouchY[NUM_ENCODERS];
    uint8_t getValueFromInput(uint8_t pot, uint8_t oldValue, uint8_t max);
    uint8_t getValueFromEncoder(uint8_t pot, uint8_t oldValue, uint8_t max);
    bool enc_button[NUM_ENCODERS];
    bool enc_moved[NUM_ENCODERS]{0, 0, 0, 0};
    int encoded[NUM_ENCODERS];
    bool tsTouched = false;
    void encoder_SetCursor(uint8_t deltaX, uint8_t maxY);

    void mouse_update();
    InputClass(/* args */);
    ~InputClass();

    void setup()
    {
        encoder_setup(100);
        touch_setup();
        // trellis started in TrellisOutClass
    }
    void update()
    {
        // revertPressedKey();
        for (int i = 0; i < NUM_ENCODERS; i++)
            active[i] = false;
        readEncoders();
        touch_update();
        mouse_update();
    }
};

extern InputClass inputs;

#endif