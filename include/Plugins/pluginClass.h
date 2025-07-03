#ifndef PLUGIN_CLASS
#define PLUGIN_CLASS
#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <projectVariables.h>
#include "ownLibs/mixers.h"

#include "input/trellis.h"
#include "output/tft.h"


#define SAMPLE_ROOT 69
#define MAX_RESONANCE 5.00
#define MAX_FILTERSWEEP 7.00
////#include "hardware/tftClass.h"
////class tftClass;
    
//extern AudioInputI2S audioInput;
extern float *note_frequency;
extern int tuning;
extern const char *filterName[4];
extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
//void drawPot(int XPos, uint8_t YPos, int dvalue, const char *dname);

class PluginControll
{
public:
    uint8_t myID;
    uint8_t potentiometer[NUM_PLUGIN_PRESETS][16];
    uint8_t presetNr = 0;
    uint8_t MixerGain;
   // AudioAnalyzePeak dacOut;

    const char *name;
    PluginControll(const char *Name, uint8_t ID)
    {
        name = Name;
        myID = ID;
        for (int p=0;p<NUM_PLUGIN_PRESETS;p++){
            for (int i=0;i<16;i++){
                potentiometer[p][i]=1;
            }
        }
    }
    virtual ~PluginControll() = default;
    virtual const char *get_Name() { return name; }
    virtual uint8_t get_ID() { return myID; }

    virtual void setup() = 0;
    virtual void noteOn(uint8_t notePlayed, float velocity, uint8_t voice) = 0;
    virtual void noteOff(uint8_t notePlayed, uint8_t voice) = 0;
    virtual void set_parameters(uint8_t row) = 0;
    virtual void draw_plugin() = 0;
    virtual void change_preset() = 0;
    virtual void set_gain(uint8_t gain) = 0;

    virtual void set_presetNr();
    virtual void save_plugin(uint8_t _songNr);
    virtual void load_plugin(uint8_t _songNr);
    virtual uint8_t get_Potentiometer(uint8_t XPos, uint8_t YPos, const char *name);
};
extern PluginControll *allPlugins[NUM_PLUGINS];

#endif // PLUGIN_CLASS

