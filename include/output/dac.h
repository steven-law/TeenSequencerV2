#ifndef DAC_H
#define DAC_H

#include <Adafruit_MCP4728.h>
#include "projectVariables.h"


extern Adafruit_MCP4728 mcp;
extern Adafruit_MCP4728 mcp2;

void dac_setup(int dly);
#endif // PLUGIN_LIST_H
