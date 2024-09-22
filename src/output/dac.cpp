#include "output/dac.h"


Adafruit_MCP4728 mcp;
Adafruit_MCP4728 mcp2;

void dac_setup(int dly){
      // Try to initialize!
  if (!mcp.begin(0x64, &Wire1))
  {
    Serial.println("Failed to find MCP4728 chip");
    while (1)
    {
      delay(10);
    }
  }
  // Try to initialize!
  if (!mcp2.begin(0x67, &Wire1))
  {
    Serial.println("Failed to find MCP4728 chip2");
    while (1)
    {
      delay(10);
    }
  }
  //GATE OUTPUT PIN Setup
 for (int i = 0; i < 8; i++)
    {
        pinMode(gateOutputPin[i], OUTPUT);
    }
  Serial.println("DAC´s started");
}


