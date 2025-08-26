
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
void PluginControll::change_preset() {}
void PluginControll::set_gain(uint8_t gain) {}

void PluginControll::setParameterNames(const char *para1, const char *para2, const char *para3, const char *para4,
                                       const char *para5, const char *para6, const char *para7, const char *para8,
                                       const char *para9, const char *para10, const char *para11, const char *para12,
                                       const char *para13, const char *para14, const char *para15, const char *para16)
{
    parameterNames[0] = para1;
    parameterNames[1] = para2;
    parameterNames[2] = para3;
    parameterNames[3] = para4;
    parameterNames[4] = para5;
    parameterNames[5] = para6;
    parameterNames[6] = para7;
    parameterNames[7] = para8;
    parameterNames[8] = para9;
    parameterNames[9] = para10;
    parameterNames[10] = para11;
    parameterNames[11] = para12;
    parameterNames[12] = para13;
    parameterNames[13] = para14;
    parameterNames[14] = para15;
    parameterNames[15] = para16;
    Serial.printf("my ID: %d\n", myID);
    // for (int i = 0; i < NUM_PARAMETERS; i++)
    // {
    //     Serial.printf("parameter: %d, name: %s\n", i, parameterNames[i]);
    // }
    Serial.printf("Setup Pl %d Done\n", myID);
    Serial.println();
    Serial.println();
}
void PluginControll::setFXParameterNames(const char *para1, const char *para2)
{
    parameterNames[0] = para1;
    parameterNames[1] = para2;
    parameterNames[2] = "0";
    parameterNames[3] = "0";
    parameterNames[4] = "0";
    parameterNames[5] = "0";
    parameterNames[6] = "0";
    parameterNames[7] = "0";
    parameterNames[8] = "0";
    parameterNames[9] = "0";
    parameterNames[10] = "0";
    parameterNames[11] = "0";
    parameterNames[12] = "0";
    parameterNames[13] = "0";
    parameterNames[14] = "0";
    parameterNames[15] = "0";
    // Serial.printf("my ID: %d\n", myID);
    // for (int i = 0; i < 2; i++)
    //{
    //     Serial.printf("parameter: %d, name: %s\n", i, parameterNames[i]);
    // }
    Serial.println();
    Serial.println();
}
void PluginControll::set_presetNr()
{
    if (enc_moved[PRESET_ENCODER])
    {
        presetNr = constrain(presetNr + encoded[PRESET_ENCODER], 0, NUM_PLUGIN_PRESETS - 1);
        change_plugin_row = true;
        draw_plugin();
    }
}
uint8_t PluginControll::get_Potentiometer(uint8_t XPos, uint8_t YPos)
{
    int n = XPos + (YPos * NUM_ENCODERS);
    set_Potentiometer(n, constrain(potentiometer[presetNr][n] + encoded[XPos], 0, MIDI_CC_RANGE));
    return potentiometer[presetNr][n];
}
void PluginControll::set_Potentiometer(uint8_t pot, uint8_t value)
{
    if (activeScreen != INPUT_FUNCTIONS_FOR_PLUGIN)
        return;
    int Xpos = pot % NUM_ENCODERS;
    int Ypos = pot / NUM_ENCODERS;
    potentiometer[presetNr][pot] = value;

    if (parameterNames[pot] != "0")
    {
        for (int r = 0; r < 2; r++)
        {
            for (int c = 0; c < NUM_STEPS; c++)
            {
                trellisOut.set_main_buffer(TRELLIS_SCREEN_PLUGIN, c, r + (Xpos * 2), TRELLIS_BLACK);
            }
        }
        trellisOut.writeDisplay();
        int oldValuePos = potentiometer[presetNr][pot] / 4.12f;
        int oldValueXPos = (oldValuePos % NUM_STEPS) + 1;
        int oldValueYPos = ((oldValuePos / NUM_STEPS) + (Xpos * 2)) % NUM_TRACKS;
        trellisOut.set_main_buffer(TRELLIS_SCREEN_PLUGIN, oldValueXPos, oldValueYPos, encoder_colour[Xpos]);
        trellisOut.writeDisplay();

        drawPot(Xpos, Ypos, potentiometer[presetNr][pot], parameterNames[pot]);
    }
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
void PluginControll::draw_plugin()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        for (int i = 0; i < NUM_PARAMETERS; i++)
        {
            if (parameterNames[i] != "0")
            {
                int xPos = i % NUM_ENCODERS;
                int yPos = i / NUM_ENCODERS;
                drawPot(xPos, yPos, potentiometer[presetNr][i], parameterNames[i]);
                // Serial.printf("parameter: %d, name: %s\n", i, parameterNames[i]);
            }
        }

        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 11, 4, 4, NO_VALUE, "Preset", ILI9341_BLUE, 2, false, false);
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 12, 4, 4, presetNr, NO_NAME, ILI9341_BLUE, 2, true, false);
        if (myID == 2 || myID == 3 || myID == 6 || myID == 8 || myID == 9 || myID == 10 || myID == 11 || myID == 12 || myID == 13 || myID == 14)
        {
            drawEnvelope(3, potentiometer[presetNr][12], potentiometer[presetNr][13],
                         potentiometer[presetNr][14], potentiometer[presetNr][15]);
        }
    }
}