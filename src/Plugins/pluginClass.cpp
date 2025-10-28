
#include <Plugins/pluginClass.h>
////#include "hardware/tftClass.h"
// class tftClass;

extern bool change_plugin_row;
// void drawPot(int XPos, uint8_t YPos, int dvalue, const char *dname);
//  plugins
float *note_frequency;
int tuning = 440;
void PluginControll::setup() {}
void PluginControll::noteOn(uint8_t notePlayed, float velocity, uint8_t voice) {}
void PluginControll::noteOff(uint8_t notePlayed, uint8_t voice) {}

void PluginControll::setParameterNames(const char *para1, const uint8_t max1, const char *para2, const uint8_t max2, const char *para3, const uint8_t max3, const char *para4, const uint8_t max4,
                                       const char *para5, const uint8_t max5, const char *para6, const uint8_t max6, const char *para7, const uint8_t max7, const char *para8, const uint8_t max8,
                                       const char *para9, const uint8_t max9, const char *para10, const uint8_t max10, const char *para11, const uint8_t max11, const char *para12, const uint8_t max12,
                                       const char *para13, const uint8_t max13, const char *para14, const uint8_t max14, const char *para15, const uint8_t max15, const char *para16, const uint8_t max16)
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
    parameterMax[0] = max1;
    parameterMax[1] = max2;
    parameterMax[2] = max3;
    parameterMax[3] = max4;
    parameterMax[4] = max5;
    parameterMax[5] = max6;
    parameterMax[6] = max7;
    parameterMax[7] = max8;
    parameterMax[8] = max9;
    parameterMax[9] = max10;
    parameterMax[10] = max11;
    parameterMax[11] = max12;
    parameterMax[12] = max13;
    parameterMax[13] = max14;
    parameterMax[14] = max15;
    parameterMax[15] = max16;
    // for (int i = 0; i < NUM_PARAMETERS; i++)
    // {
    //     Serial.printf("parameter: %d, name: %s\n", i, parameterNames[i]);
    // }
    Serial.printf("Setup Pl %d Done\n", myID);
    Serial.println();
    Serial.println();
}
void PluginControll::setFXParameterNames(const char *para1, const uint8_t max1, const char *para2, const uint8_t max2)
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
    parameterMax[0] = max1;
    parameterMax[1] = max2;
    parameterMax[2] = 0;
    parameterMax[3] = 0;
    parameterMax[4] = 0;
    parameterMax[5] = 0;
    parameterMax[6] = 0;
    parameterMax[7] = 0;
    parameterMax[8] = 0;
    parameterMax[9] = 0;
    parameterMax[10] = 0;
    parameterMax[11] = 0;
    parameterMax[12] = 0;
    parameterMax[13] = 0;
    parameterMax[14] = 0;
    parameterMax[15] = 0;
}

void PluginControll::set_presetNr()
{
    if (inputs.enc_moved[PRESET_ENCODER])
    {
        presetNr = constrain(presetNr + inputs.encoded[PRESET_ENCODER], 0, NUM_PLUGIN_PRESETS - 1);
        change_plugin_row = true;
        draw_plugin();
        change_preset();
    }
}
void PluginControll::set_preset(uint8_t preset,
                                uint8_t val0, uint8_t val1, uint8_t val2, uint8_t val3,
                                uint8_t val4, uint8_t val5, uint8_t val6, uint8_t val7,
                                uint8_t val8, uint8_t val9, uint8_t val10, uint8_t val11,
                                uint8_t val12, uint8_t val13, uint8_t val14, uint8_t val15)
{
    potentiometer[preset][0] = val0;
    potentiometer[preset][1] = val1;
    potentiometer[preset][2] = val2;
    potentiometer[preset][3] = val3;
    potentiometer[preset][4] = val4;
    potentiometer[preset][5] = val5;
    potentiometer[preset][6] = val6;
    potentiometer[preset][7] = val7;
    potentiometer[preset][8] = val8;
    potentiometer[preset][9] = val9;
    potentiometer[preset][10] = val10;
    potentiometer[preset][11] = val11;
    potentiometer[preset][12] = val12;
    potentiometer[preset][13] = val13;
    potentiometer[preset][14] = val14;
    potentiometer[preset][15] = val15;
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
    Serial.printf("get_Potentiometer for Plugin: %d %s, Parameter: %s, Value =  %d, PotNr: %d\n", myID, name, parameterNames[pot], potentiometer[presetNr][pot], pot);
    return potentiometer[presetNr][pot];
}
void PluginControll::set_Potentiometer(uint8_t pot, uint8_t value)
{
    potentiometer[presetNr][pot] = constrain(value, 0, parameterMax[pot]);
    if (!(activeScreen == INPUT_FUNCTIONS_FOR_PLUGIN || activeScreen == INPUT_FUNCTIONS_FOR_FX1 || activeScreen == INPUT_FUNCTIONS_FOR_FX2 || activeScreen == INPUT_FUNCTIONS_FOR_FX3 || activeScreen == INPUT_FUNCTIONS_FOR_SGTL))
        return;
    int Xpos = pot % NUM_ENCODERS;
    int Ypos = pot / NUM_ENCODERS;

    assign_parameter(pot);

    Serial.printf("set_Potentiometer for Plugin: %d %s, Parameter: %s, Value =  %d, PotNr: %d\n", myID, name, parameterNames[pot], potentiometer[presetNr][pot], pot);

    if (strcmp(parameterNames[pot], "0") != 0 && strcmp(parameterNames[pot], "ADSR") != 0)
    {
        drawPot(Xpos, Ypos, potentiometer[presetNr][pot], parameterNames[pot]);
    }
    if (strcmp(parameterNames[pot], "ADSR") == 0)
        drawEnvelope(Ypos, potentiometer[presetNr][(Ypos * NUM_ENCODERS)], potentiometer[presetNr][(Ypos * NUM_ENCODERS) + 1],
                     potentiometer[presetNr][(Ypos * NUM_ENCODERS) + 2], potentiometer[presetNr][(Ypos * NUM_ENCODERS) + 3]);

    trellisOut.drawPotentiometerValue(Xpos, value);
}
void PluginControll::set_Encoder_parameter(uint8_t pot)
{

    uint8_t XPos = pot % NUM_ENCODERS;
    if (inputs.active[XPos])
    {
        set_Potentiometer(pot, inputs.getValueFromInput(XPos, potentiometer[presetNr][pot], parameterMax[pot]));
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
        Serial.printf("draw plugin: %d %s\n", myID, name);
        change_plugin_row = false;
        // trellisOut.clearMainGridNow();
        int numpara = NUM_PARAMETERS;
        if (myID >= 20)
            numpara = 2;
        for (int i = 0; i < numpara; i++)
        {
            int xPos = i % NUM_ENCODERS;
            int yPos = i / NUM_ENCODERS;

            if (!(strcmp(parameterNames[i], "0") == 0 || strcmp(parameterNames[i], "ADSR") == 0))
            {
                drawPot(xPos, yPos, potentiometer[presetNr][i], parameterNames[i]);
                Serial.printf("parameter: %d, name: %s\n", i, parameterNames[i]);
            }
            else if (strcmp(parameterNames[i], "ADSR") == 0)
            {
                drawEnvelope(yPos, potentiometer[presetNr][(yPos * NUM_ENCODERS)], potentiometer[presetNr][(yPos * NUM_ENCODERS) + 1],
                             potentiometer[presetNr][(yPos * NUM_ENCODERS) + 2], potentiometer[presetNr][(yPos * NUM_ENCODERS) + 3]);
                Serial.printf("Adsr parameter: %d, name: %s\n", i, parameterNames[i]);
            }
        }

        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 11, 4, 4, NO_VALUE, "Preset", ILI9341_BLUE, 2, false, false);
        draw_value_box(3, SEQUENCER_OPTIONS_VERY_RIGHT, 12, 4, 4, presetNr, NO_NAME, ILI9341_BLUE, 2, true, false);
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