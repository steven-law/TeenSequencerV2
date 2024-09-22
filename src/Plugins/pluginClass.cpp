
#include <Arduino.h>
#include <projectVariables.h>
#include <Plugins/pluginClass.h>
////#include "hardware/tftClass.h"
//class tftClass;
extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
//void drawPot(int XPos, byte YPos, int dvalue, const char *dname);
// plugins
float *note_frequency;
int tuning = 440;
const char *filterName[4]{"LPF", "BPF", "HPF", "LPF2"};
void PluginControll::setup() {}
void PluginControll::noteOn(byte notePlayed, float velocity, byte voice) {}
void PluginControll::noteOff(byte notePlayed, byte voice) {}
void PluginControll::set_parameters(byte row) {}
void PluginControll::draw_plugin() {}
void PluginControll::change_preset() {}

void PluginControll::set_presetNr()
{
    if (enc_moved[PRESET_ENCODER])
    {
        presetNr = constrain(presetNr + encoded[PRESET_ENCODER], 0, NUM_PLUGIN_PRESETS - 1);
        change_plugin_row = true;
        draw_plugin();
    }
}
byte PluginControll::get_Potentiometer(byte XPos, byte YPos, const char *name)
{
    int n = XPos + (YPos * NUM_ENCODERS);
    potentiometer[presetNr][n] = constrain(potentiometer[presetNr][n] + encoded[XPos], 0, MIDI_CC_RANGE);
    drawPot(XPos, YPos, potentiometer[presetNr][n], name);
    // Serial.println(potentiometer[presetNr][n]);
    return potentiometer[presetNr][n];
}

void PluginControll::save_plugin(byte _songNr)
{
    SD.begin(BUILTIN_SDCARD);
    // Serial.println("in save mode:");
    neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;

    sprintf(_trackname, "%dplugin%d.txt\0", _songNr, myID);

    Serial.println(_trackname);

    // delete the file:
    // Serial.println("Removing:");
    SD.remove(_trackname);
    // Serial.println("Done:");

    // open the file.
    // Serial.println("Creating and opening:");
    myFile = SD.open(_trackname, FILE_WRITE);
    // Serial.println(_trackname);
    // Serial.println("Done:");
    //  if the file opened okay, write to it:
    if (myFile)
    {
        // save tracks
        // Serial.println("Writing track:");

        for (int p = 0; p < NUM_PLUGIN_PRESETS; p++)
        {

            for (int v = 0; v < 16; v++)
            {
                myFile.print((char)this->potentiometer[p][v]);
                Serial.printf("save plugin parameter preset: %d, value: %d\n", p, this->potentiometer[p][v]);
            }
        }
        myFile.print((char)this->presetNr);
        // close the file:
        myFile.close();
        // Serial.println("all saved:");
    }
    else
    {
        // if the file didn't open, print an error:
        Serial.println("error:");
    }
    Serial.println("plugin saving Done:");
}
void PluginControll::load_plugin(byte _songNr)
{
    SD.begin(BUILTIN_SDCARD);
    // Serial.println("in save mode:");

    sprintf(_trackname, "%dplugin%d.txt\0", _songNr, myID);
    Serial.println(_trackname);

    // open the file.
    // Serial.println("Creating and opening:");
    myFile = SD.open(_trackname, FILE_READ);
    // Serial.println(_trackname);
    // Serial.println("Done:");
    //  if the file opened okay, write to it:
    if (myFile)
    {
        // save tracks
        // Serial.println("Writing track:");

        for (int p = 0; p < NUM_PLUGIN_PRESETS; p++)
        {
            for (int v = 0; v < 16; v++)
            {
                this->potentiometer[p][v] = myFile.read();

                Serial.printf("load plugin parameter preset: %d, value: %d\n", p, this->potentiometer[p][v]);
            }
        }
        this->presetNr = myFile.read();
        // close the file:
        myFile.close();
        Serial.println("all loaded:");
    }
    else
    {
        // if the file didn't open, print an error:
        Serial.println("error:");
    }
    change_preset();
    Serial.println("plugin loading Done:");
}
