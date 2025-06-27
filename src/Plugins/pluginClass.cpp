
#include <Arduino.h>
#include <projectVariables.h>
#include <Plugins/pluginClass.h>
////#include "hardware/tftClass.h"
// class tftClass;

extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
// void drawPot(int XPos, uint8_t YPos, int dvalue, const char *dname);
//  plugins
float *note_frequency;
int tuning = 440;
const char *filterName[4]{"LPF", "BPF", "HPF", "LPF2"};
void PluginControll::setup() {}
void PluginControll::noteOn(uint8_t notePlayed, float velocity, uint8_t voice) {}
void PluginControll::noteOff(uint8_t notePlayed, uint8_t voice) {}
void PluginControll::set_parameters(uint8_t row) {}
void PluginControll::draw_plugin() {}
void PluginControll::change_preset() {}
void PluginControll::set_gain(uint8_t gain) {}

void PluginControll::set_presetNr()
{
    if (enc_moved[PRESET_ENCODER])
    {
        presetNr = constrain(presetNr + encoded[PRESET_ENCODER], 0, NUM_PLUGIN_PRESETS - 1);
        change_plugin_row = true;
        draw_plugin();
    }
}
uint8_t PluginControll::get_Potentiometer(uint8_t XPos, uint8_t YPos, const char *name)
{
    int n = XPos + (YPos * NUM_ENCODERS);
    potentiometer[presetNr][n] = constrain(potentiometer[presetNr][n] + encoded[XPos], 0, MIDI_CC_RANGE);
    drawPot(XPos, YPos, potentiometer[presetNr][n], name);
    // Serial.println(potentiometer[presetNr][n]);
    return potentiometer[presetNr][n];
}

void PluginControll::save_plugin(uint8_t _songNr)
{
    // Serial.println("in save mode:");
    neotrellisPressed[TRELLIS_BUTTON_ENTER] = false;
    char filename[20];
    sprintf(filename, "%02d_plugin%02d.dat", _songNr, myID);
    Serial.println(filename);
    if (SD.exists(filename))
        SD.remove(filename);
    myFile = SD.open(filename, FILE_WRITE);

    if (myFile)
    {
        // save tracks
        // Serial.println("Writing track:");

        for (int p = 0; p < NUM_PLUGIN_PRESETS; p++)
        {

            for (int v = 0; v < 16; v++)
            {
                myFile.write(potentiometer[p][v]);
                // Serial.printf("save plugin parameter preset: %d, value: %d\n", p, this->potentiometer[p][v]);
            }
        }
        myFile.write(presetNr);
        // close the file:
        myFile.flush();
        myFile.close();
        // Serial.println("all saved:");
        Serial.println("plugin saving Done:");
    }
    else
    {
        // if the file didn't open, print an error:
        Serial.printf("ERROR save plugin:%d\n", myID);
    }
    delay(10);
}
void PluginControll::load_plugin(uint8_t _songNr)
{
    char filename[20];
    sprintf(filename, "%02d_plugin%02d.dat", _songNr, myID);
    // open the file.
    myFile = SD.open(filename, FILE_READ);

    // Serial.println("in save mode:");

    Serial.println(filename);

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
                potentiometer[p][v] = myFile.read();

                Serial.printf("load plugin parameter preset: %d, value: %d\n", p, potentiometer[p][v]);
            }
        }
        presetNr = myFile.read();
        // close the file:
        myFile.close();
        Serial.println("all loaded:");
    }
    else
    {
        // if the file didn't open, print an error:
        Serial.printf("error load plugin:%d\n", myID);
    }
    change_preset();
    Serial.println("plugin loading Done:");
}
