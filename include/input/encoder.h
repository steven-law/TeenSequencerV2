#ifndef ENCODER_H
#define ENCODER_H

#include "Arduino.h"
#include <Encoder.h>
#include <Bounce2.h>

#include "projectVariables.h"

extern Encoder Enc1;
extern Encoder Enc2;
extern Encoder Enc3;
extern Encoder Enc4;
extern Encoder *allEncoders[NUM_ENCODERS];
extern bool enc_moved[NUM_ENCODERS];
extern int encoded[NUM_ENCODERS];
extern bool enc_button[NUM_ENCODERS];
extern long oldEnc[4];
extern const uint8_t BUTTON_PINS[NUM_ENCODERS];
extern Bounce *encButtons;
extern void encoder_setup(int dly);
extern void readEncoders();
void encoder_SetCursor(uint8_t deltaX, uint8_t maxY);

#endif