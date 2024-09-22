#ifndef PLUGIN_MIDI_H
#define PLUGIN_MIDI_H

#include <Arduino.h>
////#include "hardware/tftClass.h"
//class tftClass;
//void clearWorkSpace();
extern bool change_plugin_row; 
class Plugin_Midi
{
public:
    void noteOn(byte note, byte velocity, byte channel)
    {
        usbMIDI.sendNoteOn(note, 127, channel); // Send a Note (pitch 42, velo 127 on channel 1)
    }
    void noteOff(byte note, byte velocity, byte channel)
    {
        usbMIDI.sendNoteOff(note, 0, channel); // Send a Note (pitch 42, velo 127 on channel 1)
    }
    void set_parameters(byte row)
    {
        draw_plugin();
        if (row == 0)
        {
        }

        if (row == 1)
        {
        }

        if (row == 2)
        {
        }

        if (row == 3)
        {
        }
    }
    void draw_plugin()
    {
        if (change_plugin_row)
        {
            change_plugin_row = false;
            // Serial.println("drawing MIDI plugin");
        }
    }
};


#endif