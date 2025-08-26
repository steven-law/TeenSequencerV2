// #include "Arduino.h"

#include "projectVariables.h"
File myFile;
uint16_t rgb24to565(uint32_t color24);
const int FlashChipSelect = 6; // digital pin for flash chip CS pin
int pixelTouchX;
int gridTouchY;
uint8_t lastPotRow;
bool change_row;
uint8_t activeScreen;
uint8_t active_track;
uint8_t arrangerpage;
bool change_plugin_row;
bool getArrangerFromPC = false;
bool i2c_busy = false;
const int encoder_colour[NUM_ENCODERS] = {ILI9341_BLUE, ILI9341_RED, ILI9341_GREEN, ILI9341_WHITE};
unsigned long neotrellisReadPreviousMillis = 0; // will store last time LED was updated
unsigned long updateMidiPreviousMillis = 0;     // will store last time LED was updated
uint16_t tftRAM[16][16];

const char FLASHMEM *songNames[16]{"Marshmallow", "KittyPitty", "DragonPunch", "Snozzle", "Wildbeast", "Worldpeace", "Jumanji", "WeAreApes", "MegaHit", "10", "11", "12", "13", "14", "15", "Auto-Save"};

char _trackname[20];

bool updateTFTScreen;

uint8_t FLASHMEM gateOutputPin[8]{22, 40, 38, 37, 36, 35, 34, 33};

bool trellisShowClockPixel[TRELLIS_PADS_Y_DIM];
int trackColor[9]{rgb24to565(TRELLIS_RED), rgb24to565(TRELLIS_PURPLE), rgb24to565(TRELLIS_OLIVE), rgb24to565(TRELLIS_YELLOW), rgb24to565(TRELLIS_BLUE), rgb24to565(9365295), rgb24to565(TRELLIS_AQUA), rgb24to565(TRELLIS_GREEN), rgb24to565(900909)};
const uint8_t eigthTo127[8]{0, 18, 37, 55, 73, 92, 110, 128};
uint8_t trellisPerformIndex[TRELLIS_PADS_X_DIM];
uint8_t performCC[TRELLIS_PADS_X_DIM];
bool trellisHeld[TRELLIS_PADS_X_DIM * TRELLIS_PADS_Y_DIM];
uint8_t lastPressedKey;
bool trellisIsPressed;
bool neotrellisPressed[X_DIM * Y_DIM];
const bool PROGMEM scales[NUM_SCALES][12]{
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // Chromatic
    {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1}, // Major (Ionian)
    {1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0}, // Natural Minor (Aeolian)
    {1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0}, // Dorian
    {1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0}, // Phrygian
    {1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0}, // Lydian
    {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0}, // Mixolydian
    {1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1}, // Locrian
    {1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0}, // Harmonic Minor
    {1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0}, // Melodic Minor (Ascending)
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0}, // Blues Scale
    {1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0}, // Hungarian Minor
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0}, // Whole Tone
    {1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0}, // Neapolitan Minor
    {1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1}, // Neapolitan Major
    {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1}, // Bebop Major
    {1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1}, // Bebop Minor
    {1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1}, // Bebop Dominant
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1}, // Bebop Melodic Minor
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1}, // Bebop Harmonic Minor
    {1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0}, // Persian Scale
    {1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1}, // Spanish Gypsy
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}  // Diminished (Octatonic)

};
const char FLASHMEM *scaleNames[NUM_SCALES]{"Chroma", "Major", "natMi", "Dorian", "Phryg", "Lydian", "Mixol", "Locria", "harMi", "melMi", "Blues", "Hungar", "Whole", "neapMi", "neapMa", "bepMa", "bepMi", "bepDo", "bepMM", "bepHM", "Persia", "Span", "Dimin"};

const char *bankNames[NUM_SAMPLE_BANKS] FLASHMEM = {"K", "C", "H", "S", "P", "F", "X", "X"};
// char  **CCnames;
const char FLASHMEM *CCnames[162]{"CC0", "CC1", "CC2", "CC3", "CC4", "CC5", "CC6", "CC7", "CC8", "CC9",
                                  "CC10", "CC11", "CC12", "CC13", "CC14", "CC15", "CC16", "CC17", "CC18", "CC19",
                                  "CC20", "CC21", "CC22", "CC23", "CC24", "CC25", "CC26", "CC27", "CC28", "CC29",
                                  "CC30", "CC31", "CC32", "CC33", "CC34", "CC35", "CC36", "CC37", "CC38", "CC39",
                                  "CC40", "CC41", "CC42", "CC43", "CC44", "CC45", "CC46", "CC47", "CC48", "CC49",
                                  "CC50", "CC51", "CC52", "CC53", "CC54", "CC55", "CC56", "CC57", "CC58", "CC59",
                                  "CC60", "CC61", "CC62", "CC63", "CC64", "CC65", "CC66", "CC67", "CC68", "CC69",
                                  "CC70", "CC71", "CC72", "CC73", "CC74", "CC75", "CC76", "CC77", "CC78", "CC79",
                                  "CC80", "CC81", "CC82", "CC83", "CC84", "CC85", "CC86", "CC87", "CC88", "CC89",
                                  "CC90", "CC91", "CC92", "CC93", "CC94", "CC95", "CC96", "CC97", "CC98", "CC99",
                                  "CC100", "CC101", "CC102", "CC103", "CC104", "CC105", "CC106", "CC107", "CC108", "CC109",
                                  "CC110", "CC111", "CC112", "CC113", "CC114", "CC115", "CC116", "CC117", "CC118", "CC119",
                                  "CC120", "CC121", "CC122", "CC123", "CC124", "CC125", "CC126", "CC127", "PrgCh", "none", 
                                  "Plg1","Plg2","Plg3","Plg4","Plg5","Plg6","Plg7","Plg8",
                                  "Plg9","Plg10","Plg11","Plg12","Plg13","Plg14","Plg15","Plg16",
                                  "Seq1","Seq2","Seq3","Seq4","Seq5","Seq6","Seq7","Seq8",
                                  "Seq9","Seq10","Seq11","Seq12","Seq13","Seq14","Seq15","Seq16"};

const char FLASHMEM *seqModname[NUM_PLAYMODES]{"Step", "Rand", "Drop", "BitRd", "PotS", "Beats", "Ecld", "Rcld", ".mid", "LFO", "psyB"};
const char FLASHMEM *channelOutNames[MAX_OUTPUTS + 1]{"CV", "SR1", "SR2", "SR3", "SR4", "SR5", "SR6", "SR7", "SR8",
                                                      "SR9", "SR10", "SR11", "SR12", "SR13", "SR14", "SR15", "SR16",
                                                      "UD1", "UD2", "UD3", "UD4", "UD5", "UD6", "UD7", "UD8",
                                                      "UD9", "UD10", "UD11", "UD12", "UD13", "UD14", "UD15", "UD16",
                                                      "UH1", "UH2", "UH3", "UH4", "UH5", "UH6", "UH7", "UH8",
                                                      "UH9", "UH10", "UH11", "UH12", "UH13", "UH14", "UH15", "UH16",
                                                      "Strg", "1OSC", "FM2", "mDrm", "Drum", "Adtv", "Boom", "dTun", "Nord", "SF2", "PWM", "Raw", "Draw", "Rec"};
const char FLASHMEM *noteNames[12]{"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
const uint8_t chords[NUM_CHORDS][4]{
    {0, 3, 7, 10}, // minor 7th (m7)
    {0, 4, 7, 10}, // dominant 7th (7)
    {0, 4, 7, 11}, // major 7th (maj7)
    {0, 3, 6, 10}, // half-diminished 7th (m7♭5 / Ø7)
    {0, 3, 6, 9},  // diminished 7th (dim7)

    {0, 4, 8, 10}, // augmented 7th (#5 7)
    {0, 4, 8, 11}, // augmented major 7th (maj7#5)
    {0, 5, 7, 10}, // 7sus4
    {0, 2, 7, 10}, // 7sus2
    {0, 3, 7, 11}, // minor major 7th (m(maj7))

    {0, 3, 7, 9},  // minor 6th (m6)
    {0, 4, 7, 9},  // major 6th (6)
    {0, 4, 7, 5},  // add11
    {0, 4, 7, 14}, // add9
    {0, 3, 7, 14}, // minor add9

    {0, 3, 6, 11}, // minor major 7♭5
    {0, 3, 6, 8},  // diminished with natural 6th
    {0, 4, 6, 10}, // dominant 7♭5
    {0, 4, 6, 11}, // major 7♭5
    {0, 4, 8, 9},  // 6#5

    {0, 3, 8, 10},  // m7#5
    {0, 5, 9, 12},  // sus4+9 (inversion of sus chords)
    {0, 2, 5, 9},   // quartal chord
    {0, 5, 10, 15}, // sus4 b7 m3 (hybrid voicing)
    {0, 7, 10, 14}, // power chord + b7 + 9

    {0, 7, 9, 14}, // 5 + 6 + 9 (major 6/9 voicing)
    {0, 3, 5, 7},  // m add11 (no 7)
    {0, 4, 5, 7},  // maj add11 (no 7)
    {0, 4, 6, 9},  // 6♭5 (maj6♭5)
    {0, 2, 4, 7},  // major add9 (triad + 9)

    {0, 2, 3, 7},  // minor add9 (triad + 9)
    {0, 1, 7, 10}, // 7♭9 (implied dom7♭9)
    {0, 4, 7, 13}, // maj7♭9 (less common)
    {0, 3, 7, 13}  // m(maj7♭9)
};
const uint8_t chordVolumes[NUM_CHORD_VOLUMES][4]{
    {0, 0, 0, 0},
    {0, 0, 0, 1},
    {0, 0, 0, 2},
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 1, 2},
    {0, 0, 2, 0},
    {0, 0, 2, 1},
    {0, 0, 2, 2},
    {0, 1, 0, 0},
    {0, 1, 0, 1},
    {0, 1, 0, 2},
    {0, 1, 1, 0},
    {0, 1, 1, 1},
    {0, 1, 1, 2},
    {0, 1, 2, 0},
    {0, 1, 2, 1},
    {0, 1, 2, 2},
    {0, 2, 0, 0},
    {0, 2, 0, 1},
    {0, 2, 0, 2},
    {0, 2, 1, 0},
    {0, 2, 1, 1},
    {0, 2, 1, 2},
    {0, 2, 2, 0},
    {0, 2, 2, 1},
    {0, 2, 2, 2},
    {1, 0, 0, 0},
    {1, 0, 0, 1},
    {1, 0, 0, 2},
    {1, 0, 1, 0},
    {1, 0, 1, 1},
    {1, 0, 1, 2},
    {1, 0, 2, 0},
    {1, 0, 2, 1},
    {1, 0, 2, 2},
    {1, 1, 0, 0},
    {1, 1, 0, 1},
    {1, 1, 0, 2},
    {1, 1, 1, 0},
    {1, 1, 1, 1},
    {1, 1, 1, 2},
    {1, 1, 2, 0},
    {1, 1, 2, 1},
    {1, 1, 2, 2},
    {1, 2, 0, 0},
    {1, 2, 0, 1},
    {1, 2, 0, 2},
    {1, 2, 1, 0},
    {1, 2, 1, 1},
    {1, 2, 1, 2},
    {1, 2, 2, 0},
    {1, 2, 2, 1},
    {1, 2, 2, 2},
    {2, 0, 0, 0},
    {2, 0, 0, 1},
    {2, 0, 0, 2},
    {2, 0, 1, 0},
    {2, 0, 1, 1},
    {2, 0, 1, 2},
    {2, 0, 2, 0},
    {2, 0, 2, 1},
    {2, 0, 2, 2},
    {2, 1, 0, 0},
    {2, 1, 0, 1},
    {2, 1, 0, 2},
    {2, 1, 1, 0},
    {2, 1, 1, 1},
    {2, 1, 1, 2},
    {2, 1, 2, 0},
    {2, 1, 2, 1},
    {2, 1, 2, 2},
    {2, 2, 0, 0},
    {2, 2, 0, 1},
    {2, 2, 0, 2},
    {2, 2, 1, 0},
    {2, 2, 1, 1},
    {2, 2, 1, 2},
    {2, 2, 2, 0},
    {2, 2, 2, 1},
    {2, 2, 2, 2}

};
bool **beatArray;
bool **beatArrayPM6;
bool *beatArrayPM7;
uint8_t sgtlparameter[16];
// touch
bool isTouched;

MidiTrack myMidi1;
MidiTrack myMidi2;
MidiTrack myMidi3;
MidiTrack myMidi4;
MidiTrack myMidi5;
MidiTrack myMidi6;
MidiTrack myMidi7;
MidiTrack myMidi8;
MidiTrack myMidi[NUM_TRACKS] = {
    myMidi1,
    myMidi1,
    myMidi1,
    myMidi1,
    myMidi1,
    myMidi1,
    myMidi1,
    myMidi1,
};

int noteInfo[MAX_VOICES][4] = {128, 0, 0, 0};

float lfo_semitone_tri(float phase)
{
    // triangle wave: -1.0 .. +1.0
    float tri = 2.0f * fabs(2.0f * (phase - floor(phase + 0.5f))) - 1.0f;
    // skalieren auf -12 bis +12
    return round(tri);
}
float lfo_semitone_saw(float phase)
{
    float saw = 2.0f * (phase - floor(phase + 0.5f)); // -1.0 .. +1.0
    return round(saw);
}