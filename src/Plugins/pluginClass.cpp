
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
void PluginControll::change_preset()
{
    for (int i = 0; i < MAX_VOICES; i++)
    {
        assign_parameter(i);
    }
}
void PluginControll::PluginParameters(uint8_t row)
{
    // draw_plugin();

    if (!neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        if (row == 0)
        {

            set_Encoder_parameter(0);
            set_Encoder_parameter(1);
            set_Encoder_parameter(2);
            set_Encoder_parameter(3);
        }

        if (row == 1)
        {

            set_Encoder_parameter(4);
            set_Encoder_parameter(5);
            set_Encoder_parameter(6);
            set_Encoder_parameter(7);
        }

        if (row == 2)
        {
            set_Encoder_parameter(8);
            set_Encoder_parameter(9);
            set_Encoder_parameter(10);
            set_Encoder_parameter(11);
        }

        if (row == 3)
        {
            set_Encoder_parameter(12);
            set_Encoder_parameter(13);
            set_Encoder_parameter(14);
            set_Encoder_parameter(15);
        }
    }
    if (neotrellisPressed[TRELLIS_BUTTON_SHIFT])
    {
        set_presetNr();
    }
}

uint8_t PluginControll::get_Potentiometer(uint8_t pot)
{
    return potentiometer[presetNr][pot];
}
void PluginControll::set_Potentiometer(uint8_t pot, uint8_t value)
{
    potentiometer[presetNr][pot] = value;
    if (!(activeScreen == INPUT_FUNCTIONS_FOR_PLUGIN || activeScreen == INPUT_FUNCTIONS_FOR_FX1 || activeScreen == INPUT_FUNCTIONS_FOR_FX2 || activeScreen == INPUT_FUNCTIONS_FOR_FX3))
        return;
    int Xpos = pot % NUM_ENCODERS;
    int Ypos = pot / NUM_ENCODERS;

    assign_parameter(pot);
    trellisOut.drawPotentiometerValue(Xpos, value);
    Serial.printf("parameter: %s, value: %d\n", parameterNames[pot], value);
    if (strcmp(parameterNames[pot], "0") != 0 && strcmp(parameterNames[pot], "1") != 0)
    {
        drawPot(Xpos, Ypos, value, parameterNames[pot]);
    }
    if (strcmp(parameterNames[pot], "1") == 0)
        drawEnvelope(3, potentiometer[presetNr][12], potentiometer[presetNr][13],
                     potentiometer[presetNr][14], potentiometer[presetNr][15]);
}
void PluginControll::set_Encoder_parameter(uint8_t pot)
{
    uint8_t XPos = pot % NUM_ENCODERS;
    if (enc_moved[XPos])
    {
        set_Potentiometer(pot, constrain(potentiometer[presetNr][pot] + encoded[XPos], 0, MIDI_CC_RANGE));

        // assign_mixer_gain(get_Potentiometer(XPos, YPos), n);
    }
    if (potTouched[XPos])
    {
        int pot = parameterTouchX + (lastPotRow * NUM_ENCODERS);
        int value = parameterTouchY[parameterTouchX];
        set_Potentiometer(pot, value);
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
        trellisOut.clearMainGridNow();
        for (int i = 0; i < NUM_PARAMETERS; i++)
        {
            if (strcmp(parameterNames[i], "0") != 0 && strcmp(parameterNames[i], "1") != 0)
            {
                int xPos = i % NUM_ENCODERS;
                int yPos = i / NUM_ENCODERS;
                drawPot(xPos, yPos, potentiometer[presetNr][i], parameterNames[i]);

               
                // Serial.printf("parameter: %d, name: %s\n", i, parameterNames[i]);
            }
        }

        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 11, 4, 4, NO_VALUE, "Preset", ILI9341_BLUE, 2, false, false);
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 12, 4, 4, presetNr, NO_NAME, ILI9341_BLUE, 2, true, false);
        if (IhaveADSR)
        {
            drawEnvelope(3, potentiometer[presetNr][12], potentiometer[presetNr][13],
                         potentiometer[presetNr][14], potentiometer[presetNr][15]);
        }
    }
}
void PluginControll::set_gain(uint8_t gain)
{
    MixerGain = gain;
    if (IamPoly)
        MixGain.gain(gain / MIDI_CC_RANGE_FLOAT);
}
void PluginControll::set_FilterFreq(uint8_t value)
{
    Serial.printf("set plugin %d to freq %f\n", myID, note_frequency[value]);
    performFilter.frequency(note_frequency[value] * tuning);
}
void PluginControll::set_FilterReso(uint8_t value)
{
    // float reso = (float)map(value, 0, MIDI_CC_RANGE, 0, 5.00f);
    float reso = value / 25.4;
    Serial.printf("set plugin %d to reso %f\n", myID, reso);
    performFilter.resonance(reso);
}