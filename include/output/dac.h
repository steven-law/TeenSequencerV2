#ifndef DAC_H
#define DAC_H

#include <Adafruit_MCP4728.h>
#include "projectVariables.h"


extern Adafruit_MCP4728 mcp;
extern Adafruit_MCP4728 mcp2;

void dac_setup(int dly);

void sendNoteOn_CV_Gate(uint8_t _track, uint8_t _note);
void sendNoteOff_CV_Gate(uint8_t _track, uint8_t _note);
void play_plugin_on_DAC(uint8_t _track, uint8_t _pluginNr);

#endif // PLUGIN_LIST_H
