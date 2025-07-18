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
// #include "hardware/tftClass.h"
class tftClass;
extern bool change_plugin_row;

extern float *note_frequency;

// Encoder Pins
extern bool enc_moved[4];
extern int encoded[4];
// tftClass drawPot(int XPos, uint8_t YPos, int dvalue, const char *dname);
uint8_t getEncodervalue(uint8_t XPos, uint8_t YPos, const char *name, uint8_t oldValue);

// TeensyDAW: begin automatically generated code
class Output
{
public:
    uint8_t plugin_channel[NUM_TRACKS]; // this stores the track number that is related to the plugin number f.e plguin_channel[Plugin_0]= Track number 2
    FX_Section fx_section;
    AudioFilterStateVariable finalFilter;
    AudioOutputI2S i2s;
    AudioControlSGTL5000 sgtl5000;
    AudioConnection *patchCord[3]; // total patchCordCount:2 including array typed ones.

    // constructor (this is called when class-object is created)
    Output(uint8_t i)
    {
        int pci = 0; // used only for adding new patchcords

        patchCord[pci++] = new AudioConnection(fx_section.endmixer, 0, finalFilter, 0);
        patchCord[pci++] = new AudioConnection(finalFilter, 0, i2s, 0);
        // patchCord[pci++] = new AudioConnection(plugin_12.waveform, 0, i2s, 0);
        patchCord[pci++] = new AudioConnection(finalFilter, 0, i2s, 1);
    }
    void setup()
    {

        if (!sgtl5000.enable())
            Serial.println("AudioBoard NOT available");
        else
            Serial.println("AudioBoard available");
        // sgtl5000.inputSelect(AUDIO_INPUT_LINEIN);
        sgtl5000.volume(1);
        fx_section.setup();

        for (int i = 0; i < NUM_PLUGINS; i++)
        {
            allPlugins[i]->setup();
        }
        Serial.println("masterOut setup done");
    }
    void noteOn(uint8_t note, uint8_t velo, uint8_t _channel, uint8_t voice)
    {
        Serial.printf("recieve NoteON channel:%d, velocity: %d\n", _channel, velo);

        allPlugins[_channel]->noteOn(note, velo / MIDI_CC_RANGE_FLOAT, voice);
    }
    void noteOff(uint8_t note, uint8_t velo, uint8_t _channel, uint8_t voice)
    {
        // Serial.printf("recieve NoteOFF channel:%d\n", _channel);
        allPlugins[_channel]->noteOff(note, voice);
    }

    void set_parameters(uint8_t trackID, uint8_t row)
    {
        // Serial.printf("set parameters track: %d, channel: %d\n", trackID, plugin_channel[trackID]);
        allPlugins[trackID]->set_parameters(row);
    }
    void draw_plugin(uint8_t trackID, uint8_t channel)
    {
        Serial.printf("draw plugin track: %d, channel: %d\n", trackID, plugin_channel[trackID]);
        change_plugin_row = true;
        allPlugins[trackID]->draw_plugin();
    }
    void set_active_plugin_for_track(uint8_t trackID, uint8_t channel)
    {
        plugin_channel[trackID] = channel;
    }
    void setFXGain(int FxNr, int pluginNr, float gain) // 0=dry, 1= FX1, 2= FX2, 3=FX3
    {
        switch (FxNr)
        {
        case 0:
            fx_section.dry[pluginNr].gain(gain);
            break;
        case 1:
            fx_1.pl[pluginNr].gain(gain);
            break;
        case 2:
            fx_2.pl[pluginNr].gain(gain);
            break;
        case 3:
            fx_3.pl[pluginNr].gain(gain);
            break;
        default:
            break;
        }
    }
    // TeensyDAW: end automatically generated code
};

// extern Output MasterOut;
#endif // MASTEROUT_H