#ifndef MASTEROUT_H
#define MASTEROUT_H

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "projectVariables.h"
#include <Plugins/plugin_List.h>

#include "FX/FX_Section.h"
//#include "hardware/tftClass.h"
class tftClass;
extern bool change_plugin_row;

extern float *note_frequency;

// Encoder Pins
extern bool enc_moved[4];
extern int encoded[4];
//tftClass drawPot(int XPos, byte YPos, int dvalue, const char *dname);
byte getEncodervalue(byte XPos, byte YPos, const char *name, byte oldValue);
//void clearWorkSpace();

// TeensyDAW: begin automatically generated code
class Output
{
public:
    byte plugin_channel[NUM_TRACKS]; // this stores the track number that is related to the plugin number f.e plguin_channel[Plugin_0]= Track number 2
    FX_Section fx_section;
    AudioFilterStateVariable finalFilter;
    AudioOutputI2S i2s;
    AudioControlSGTL5000 sgtl5000;
    AudioConnection *patchCord[3]; // total patchCordCount:2 including array typed ones.

    // constructor (this is called when class-object is created)
    Output(byte i)
    {
        int pci = 0; // used only for adding new patchcords

        patchCord[pci++] = new AudioConnection(fx_section.endmixer, 0, finalFilter, 0);
        patchCord[pci++] = new AudioConnection(finalFilter, 0, i2s, 0);
        // patchCord[pci++] = new AudioConnection(plugin_3.modulator, 0, i2s, 0);
        patchCord[pci++] = new AudioConnection(finalFilter, 0, i2s, 1);
    }
    void setup()
    {

        sgtl5000.enable();
        sgtl5000.volume(1);
        fx_section.setup();

        for (int i = 0; i < NUM_PLUGINS; i++)
        {
            allPlugins[i]->setup();
        }
        Serial.println("masterOut setup done");
    }
    void noteOn(byte note, byte velo, byte _channel, byte voice)
    {
        // Serial.printf("recieve NoteON channel:%d\n", _channel);
        allPlugins[_channel]->noteOn(note, 1, voice);
    }
    void noteOff(byte note, byte velo, byte _channel, byte voice)
    {
        // Serial.printf("recieve NoteOFF channel:%d\n", _channel);
        allPlugins[_channel]->noteOff(note, voice);
    }

    void set_parameters(byte trackID, byte row)
    {
        // Serial.printf("set parameters track: %d, channel: %d\n", trackID, plugin_channel[trackID]);
        allPlugins[trackID]->set_parameters(row);
    }
    void draw_plugin(byte trackID, byte channel)
    {
        Serial.printf("draw plugin track: %d, channel: %d\n", trackID, plugin_channel[trackID]);
        change_plugin_row = true;
        allPlugins[trackID]->draw_plugin();
    }
    void set_active_plugin_for_track(byte trackID, byte channel)
    {
        plugin_channel[trackID] = channel;
    }

    // TeensyDAW: end automatically generated code
};

// extern Output MasterOut;
#endif //MASTEROUT_H