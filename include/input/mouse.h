#ifndef MOUSE_H
#define MOUSE_H
#include "Arduino.h"

#include <USBHost_t36.h>

#include "input/encoder.h"
#include "input/trellis.h"
extern USBHost myusb;

extern USBHIDParser hid1;
// USBHIDParser hid2(myusb);
extern MouseController mouse1;
void mouse(byte deltaX, byte maxY);
void detect_mouse();

#endif
