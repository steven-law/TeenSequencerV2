#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "projectVariables.h"

#include <Plugins/Plugin_11.h>

extern bool change_plugin_row;
void clearWorkSpace();


void Plugin_11::setup()
{  
    NoteGain.gain(0);
    MixGain.gain(1);
    SongVol.gain(1);
}
void Plugin_11::noteOn(byte notePlayed, float velocity, byte voice)
    
{
NoteGain.gain(1);
}
void Plugin_11::noteOff(byte notePlayed, byte voice)
{
    NoteGain.gain(0);

}
void Plugin_11::set_parameters(byte row)
{
    draw_plugin();
    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        if (row == 0)
        {
            set_voice_amplitude(1, 0, "Nothing");
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
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        set_presetNr();
    }
}
void Plugin_11::draw_plugin()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
         clearWorkSpace();
        // Serial.println("drawing plugin 2");
        drawPot(1, 0, potentiometer[presetNr][1], "Nothing");
        //draw_sequencer_option(SEQUENCER_OPTIONS_VERY_RIGHT, "Prset", presetNr, 3, 0);
    }
}



void Plugin_11::set_voice_amplitude(byte XPos, byte YPos, const char *name)
{
    if (enc_moved[XPos])
    {
       get_Potentiometer(XPos, YPos, name);
    }
}

void Plugin_11::change_preset(){}
Plugin_11 plugin_11("Ext", 9);
