#ifndef PLUGIN_CLASS
#define PLUGIN_CLASS
#include <SD.h>
#include <projectVariables.h>

#include "input/trellis.h"
#include "output/tft.h"

#define PRESET_ENCODER 3
#define SAMPLE_ROOT 69
#define MAX_RESONANCE 5.00
#define MAX_FILTERSWEEP 7.00
////#include "hardware/tftClass.h"
////class tftClass;
extern float *note_frequency;
extern int tuning;
extern const char *filterName[4];
extern bool enc_moved[4];
extern int encoded[4];
extern bool change_plugin_row;
//void drawPot(int XPos, byte YPos, int dvalue, const char *dname);

class PluginControll
{
public:
    byte myID;
    byte potentiometer[NUM_PLUGIN_PRESETS][16];
    byte presetNr = 0;

    const char *name;
    PluginControll(const char *Name, byte ID)
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
    virtual byte get_ID() { return myID; }

    virtual void setup() = 0;
    virtual void noteOn(byte notePlayed, float velocity, byte voice) = 0;
    virtual void noteOff(byte notePlayed, byte voice) = 0;
    virtual void set_parameters(byte row) = 0;
    virtual void draw_plugin() = 0;
    virtual void change_preset() = 0;

    virtual void set_presetNr();
    virtual void save_plugin(byte _songNr);
    virtual void load_plugin(byte _songNr);
    virtual byte get_Potentiometer(byte XPos, byte YPos, const char *name);
};
extern PluginControll *allPlugins[NUM_PLUGINS];

#endif // PLUGIN_CLASS

