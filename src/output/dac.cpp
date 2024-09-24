#include "output/dac.h"

Adafruit_MCP4728 mcp;
Adafruit_MCP4728 mcp2;

void dac_setup(int dly)
{
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
  // GATE OUTPUT PIN Setup
  for (int i = 0; i < 8; i++)
  {
    pinMode(gateOutputPin[i], OUTPUT);
  }
  Serial.println("DAC´s started");
}
void sendNoteOn_CV_Gate(byte _track, byte _note)
{
  if (_note < 64)
  {
    if (_track == 0)
    {
      // byte _cvNoteValue=4095 / 64 * allTracks[0]->cvNote;
      mcp.setChannelValue(MCP4728_CHANNEL_A, 4095 / 64 * _note);
      digitalWrite(gateOutputPin[0], true);
      Serial.printf("cv Note On: %d\n", _note);
    }

    if (_track == 1)
    {
      mcp.setChannelValue(MCP4728_CHANNEL_B, 4095 / 64 * _note);
      digitalWrite(gateOutputPin[1], true);
    }

    if (_track == 2)
    {
      mcp.setChannelValue(MCP4728_CHANNEL_C, 4095 / 64 * _note);
      digitalWrite(gateOutputPin[2], true);
    }

    if (_track == 3)
    {
      mcp.setChannelValue(MCP4728_CHANNEL_D, 4095 / 64 * _note);
      digitalWrite(gateOutputPin[3], true);
    }
    if (_track == 4)
    {
      mcp2.setChannelValue(MCP4728_CHANNEL_A, 4095 / 64 * _note);
      digitalWrite(gateOutputPin[4], true);
    }
    if (_track == 5)
    {
      mcp2.setChannelValue(MCP4728_CHANNEL_B, 4095 / 64 * _note);
      digitalWrite(gateOutputPin[5], true);
    }
    if (_track == 6)
    {
      mcp2.setChannelValue(MCP4728_CHANNEL_C, 4095 / 64 * _note);
      digitalWrite(gateOutputPin[6], true);
    }
    if (_track == 7)
    {
      mcp2.setChannelValue(MCP4728_CHANNEL_D, 4095 / 64 * _note);
      digitalWrite(gateOutputPin[7], true);
    }
  }
}
void sendNoteOff_CV_Gate(byte _track, byte _note)
{

  digitalWrite(gateOutputPin[_track], false);
  Serial.println("cv Note Off");
}
