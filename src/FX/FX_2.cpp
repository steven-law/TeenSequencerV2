
#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "ownLibs/mixers.h"
#include "projectVariables.h"
#include <FX/FX_2.h>

void FX_2::setup()
{
    FX_mixer.gain(0, 1);
    FX_mixer.gain(1, 1);
    FX_mixer.gain(2, 1);
    FX_mixer.gain(3, 1);
    FX_mixer.gain(4, 1);
}
void FX_2::noteOn(uint8_t notePlayed, float velocity, uint8_t voice) {}
void FX_2::noteOff(uint8_t notePlayed, uint8_t voice) {}
void FX_2::set_parameters(uint8_t row)
{
    draw_plugin();
    if (row == 0)
    {
        set_BC_bits(0, 0, "BitDepth");
        set_BC_smplRate(1, 0, "SmpRate");
    }
}
void FX_2::draw_plugin()
{
    if (change_plugin_row)
    {
        change_plugin_row = false;
        drawPot(0, 0, potentiometer[presetNr][0], "BitDepth");
        drawPot(1, 0, potentiometer[presetNr][1], "SmpRate");
    }
}

void FX_2::set_BC_bits(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        bitcrusher.bits(map(get_Potentiometer(XPos,YPos,name), 0, 127, 1, 16));
    }
}
void FX_2::set_BC_smplRate(uint8_t XPos, uint8_t YPos, const char *name)
{
    if (enc_moved[XPos])
    {
        bitcrusher.sampleRate(map(get_Potentiometer(XPos,YPos,name), 0, 127, 1, 44100));
    }
}
void FX_2::change_preset(){
        bitcrusher.bits(map(potentiometer[presetNr][0], 0, 127, 1, 16));
        bitcrusher.sampleRate(map(potentiometer[presetNr][1], 0, 127, 1, 44100));


}