#ifndef SAMPLE_LIST_H
#define SAMPLE_LIST_H
// Audio data converted from WAV file by wav2sketch
#include <Arduino.h>
#include <Audio.h>
extern const unsigned int AudioSampleTomtom[3489] PROGMEM;
extern const unsigned int AudioSampleSnare[2817] PROGMEM;
extern const unsigned int AudioSampleCashregister[5809] PROGMEM;
extern const unsigned int AudioSampleHihat[5953] PROGMEM;
extern const unsigned int AudioSampleKick[2561] PROGMEM;
extern const unsigned int AudioSampleP2[929] PROGMEM;
extern const unsigned int AudioSampleGong[27633] PROGMEM;
extern const unsigned int AudioSamplePongblip[737] PROGMEM;
extern const unsigned int AudioSampleClap[3649] PROGMEM;


//flute
extern const AudioSynthWavetable::instrument_data Flute_100kuint8_t;
//basicflute
extern const AudioSynthWavetable::sample_data BasicFlute1_samples[2];
const uint8_t BasicFlute1_ranges[] = {54, 127, };
const AudioSynthWavetable::instrument_data BasicFlute1 = {2, BasicFlute1_ranges, BasicFlute1_samples };
extern const uint32_t sample_0_BasicFlute1_BreathyFluteC2[28544];
extern const uint32_t sample_1_BasicFlute1_BreathyFluteA2[31616];
//FrenchHorn
extern const AudioSynthWavetable::sample_data FrenchHorns_samples[1];
const uint8_t FrenchHorns_ranges[] = {127, };
const AudioSynthWavetable::instrument_data FrenchHorns = {1, FrenchHorns_ranges, FrenchHorns_samples };
extern const uint32_t sample_0_FrenchHorns_FrenchHornsA4L[55040];
//Ocarina
extern const AudioSynthWavetable::sample_data Ocarina_samples[3];
const uint8_t Ocarina_ranges[] = {78, 101, 127, };
const AudioSynthWavetable::instrument_data Ocarina = {3, Ocarina_ranges, Ocarina_samples };
extern const uint32_t sample_0_Ocarina_OcarinaF4[1536];
extern const uint32_t sample_1_Ocarina_OcarinaF4[1536];
extern const uint32_t sample_2_Ocarina_OcarinaF6[256];
//Pizzicato
extern const AudioSynthWavetable::sample_data Pizzicato_samples[4];
const uint8_t Pizzicato_ranges[] = {68, 83, 93, 127, };
const AudioSynthWavetable::instrument_data Pizzicato = {4, Pizzicato_ranges, Pizzicato_samples };
extern const uint32_t sample_0_Pizzicato_PizzViolinE3[2944];
extern const uint32_t sample_1_Pizzicato_PizzViolinC4[2304];
extern const uint32_t sample_2_Pizzicato_PizzViolinE5[768];
extern const uint32_t sample_3_Pizzicato_PizzViolinE5[768];
//Viola
extern const AudioSynthWavetable::sample_data Viola_samples[8];
const uint8_t Viola_ranges[] = {58, 65, 68, 73, 79, 92, 96, 127, };
const AudioSynthWavetable::instrument_data Viola = {8, Viola_ranges, Viola_samples };
extern const uint32_t sample_0_Viola_ViolinBb2[768];
extern const uint32_t sample_1_Viola_ViolinD3[896];
extern const uint32_t sample_2_Viola_ViolinG3[768];
extern const uint32_t sample_3_Viola_ViolinC4[768];
extern const uint32_t sample_4_Viola_ViolinGb4[768];
extern const uint32_t sample_5_Viola_ViolinC5[640];
extern const uint32_t sample_6_Viola_ViolinEb5[768];
extern const uint32_t sample_7_Viola_ViolinEb6[512];
//bassoon
extern const AudioSynthWavetable::sample_data bassoon_samples[2];
const uint8_t bassoon_ranges[] = {88, 127, };
const AudioSynthWavetable::instrument_data bassoon = {2, bassoon_ranges, bassoon_samples };
extern const uint32_t sample_0_bassoon_bassoonc2[640];
extern const uint32_t sample_1_bassoon_enghorndx3[896];
//clarinett
extern const AudioSynthWavetable::sample_data clarinet_samples[2];
const uint8_t clarinet_ranges[] = {69, 127, };
const AudioSynthWavetable::instrument_data clarinet = {2, clarinet_ranges, clarinet_samples };
extern const uint32_t sample_0_clarinet_clarinetd2[384];
extern const uint32_t sample_1_clarinet_clarinetb2[384];
extern const AudioSynthWavetable::sample_data distortiongt_samples[4];
const uint8_t distortiongt_ranges[] = {62, 66, 72, 127, };
//distgt
const AudioSynthWavetable::instrument_data distortiongt = {4, distortiongt_ranges, distortiongt_samples };
extern const uint32_t sample_0_distortiongt_distgtra2[1024];
extern const uint32_t sample_1_distortiongt_distgtre3[768];
extern const uint32_t sample_2_distortiongt_distgtra3[640];
extern const uint32_t sample_3_distortiongt_distgtrd4[896];
//epiano
extern const AudioSynthWavetable::sample_data epiano_samples[4];
const uint8_t epiano_ranges[] = {48, 60, 80, 127, };
const AudioSynthWavetable::instrument_data epiano = {4, epiano_ranges, epiano_samples };
extern const uint32_t sample_0_epiano_coldglass7wave[128];
extern const uint32_t sample_1_epiano_coldglass7wave[128];
extern const uint32_t sample_2_epiano_coldglass7wave[128];
extern const uint32_t sample_3_epiano_epiano2ms[640];
//flute
extern const AudioSynthWavetable::sample_data flute_samples[1];
const uint8_t flute_ranges[] = {127, };
const AudioSynthWavetable::instrument_data flute = {1, flute_ranges, flute_samples };
extern const uint32_t sample_0_flute_flutec4[768];
//glockenspiel
extern const AudioSynthWavetable::sample_data glockenspiel_samples[2];
const uint8_t glockenspiel_ranges[] = {59, 127, };
const AudioSynthWavetable::instrument_data glockenspiel = {2, glockenspiel_ranges, glockenspiel_samples };
extern const uint32_t sample_0_glockenspiel_sinetick[128];
extern const uint32_t sample_1_glockenspiel_sinetick[128];
//gtfretnoise
extern const AudioSynthWavetable::sample_data gtfretnoise_samples[1];
const uint8_t gtfretnoise_ranges[] = {127, };
const AudioSynthWavetable::instrument_data gtfretnoise = {1, gtfretnoise_ranges, gtfretnoise_samples };
extern const uint32_t sample_0_gtfretnoise_guitarfret[1792];
//harmonica
extern const AudioSynthWavetable::sample_data harmonica_samples[1];
const uint8_t harmonica_ranges[] = {127, };
const AudioSynthWavetable::instrument_data harmonica = {1, harmonica_ranges, harmonica_samples };
extern const uint32_t sample_0_harmonica_harmonicaa3[512];
//harp
extern const AudioSynthWavetable::sample_data harp_samples[1];
const uint8_t harp_ranges[] = {127, };
const AudioSynthWavetable::instrument_data harp = {1, harp_ranges, harp_samples };
extern const uint32_t sample_0_harp_pluckharp[1792];
//mutedgtr
extern const AudioSynthWavetable::sample_data mutedgtr_samples[1];
const uint8_t mutedgtr_ranges[] = {127, };
const AudioSynthWavetable::instrument_data mutedgtr = {1, mutedgtr_ranges, mutedgtr_samples };
extern const uint32_t sample_0_mutedgtr_mgtr[512];
//nylonstrgtr
extern const AudioSynthWavetable::sample_data nylonstrgtr_samples[3];
const uint8_t nylonstrgtr_ranges[] = {56, 77, 127, };
const AudioSynthWavetable::instrument_data nylonstrgtr = {3, nylonstrgtr_ranges, nylonstrgtr_samples };
extern const uint32_t sample_0_nylonstrgtr_nguitrf2[1920];
extern const uint32_t sample_1_nylonstrgtr_nguitb2[2688];
extern const uint32_t sample_2_nylonstrgtr_acgtrb3[3200];
//oboe
extern const AudioSynthWavetable::sample_data oboe_samples[3];
const uint8_t oboe_ranges[] = {63, 68, 127, };
const AudioSynthWavetable::instrument_data oboe = {3, oboe_ranges, oboe_samples };
extern const uint32_t sample_0_oboe_oboecx3[512];
extern const uint32_t sample_1_oboe_oboefx3[640];
extern const uint32_t sample_2_oboe_oboeax3[512];
//overdrivegtr
extern const AudioSynthWavetable::sample_data overdrivegt_samples[3];
const uint8_t overdrivegt_ranges[] = {62, 66, 127, };
const AudioSynthWavetable::instrument_data overdrivegt = {3, overdrivegt_ranges, overdrivegt_samples };
extern const uint32_t sample_0_overdrivegt_distgtra2[1024];
extern const uint32_t sample_1_overdrivegt_distgtre3[768];
extern const uint32_t sample_2_overdrivegt_distgtra3[640];
//piano
extern const AudioSynthWavetable::sample_data piano_samples[4];
const uint8_t piano_ranges[] = {54, 74, 88, 127, };
const AudioSynthWavetable::instrument_data piano = {4, piano_ranges, piano_samples };
extern const uint32_t sample_0_piano_kpianob1[8704];
extern const uint32_t sample_1_piano_kpianocx4[10880];
extern const uint32_t sample_2_piano_kpianodx5[3200];
extern const uint32_t sample_3_piano_kpianof502[2304];
//recorder
extern const AudioSynthWavetable::sample_data recorder_samples[1];
const uint8_t recorder_ranges[] = {127, };
const AudioSynthWavetable::instrument_data recorder = {1, recorder_ranges, recorder_samples };
extern const uint32_t sample_0_recorder_recorderax2[768];
//standard drums
extern const AudioSynthWavetable::sample_data standard_DRUMS_samples[48];
const uint8_t standard_DRUMS_ranges[] = {27, 28, 28, 29, 30, 31, 33, 34, 35, 36, 37, 39, 42, 45, 47, 48, 50, 53, 54, 55, 56, 57, 58, 60, 61, 62, 63, 65, 67, 68, 69, 71, 72, 73, 74, 76, 78, 80, 81, 82, 83, 83, 84, 85, 85, 86, 87, 127, };
const AudioSynthWavetable::instrument_data standard_DRUMS = {48, standard_DRUMS_ranges, standard_DRUMS_samples };
extern const uint32_t sample_0_standard_DRUMS_filtersnap[256];
extern const uint32_t sample_1_standard_DRUMS_whitenoisewave[4224];
extern const uint32_t sample_2_standard_DRUMS_verbclickwave[640];
extern const uint32_t sample_3_standard_DRUMS_scratch[896];
extern const uint32_t sample_4_standard_DRUMS_guitarfret[1792];
extern const uint32_t sample_5_standard_DRUMS_stix[256];
extern const uint32_t sample_6_standard_DRUMS_sinetick[128];
extern const uint32_t sample_7_standard_DRUMS_verbclickwave[640];
extern const uint32_t sample_8_standard_DRUMS_coldglass12wave[128];
extern const uint32_t sample_9_standard_DRUMS_bd15[896];
extern const uint32_t sample_10_standard_DRUMS_verbclickwave[640];
extern const uint32_t sample_11_standard_DRUMS_snare24[2048];
extern const uint32_t sample_12_standard_DRUMS_snare24[2048];
extern const uint32_t sample_13_standard_DRUMS_floortombrite[3712];
extern const uint32_t sample_14_standard_DRUMS_hatopenms[5888];
extern const uint32_t sample_15_standard_DRUMS_floortombrite[3712];
extern const uint32_t sample_16_standard_DRUMS_crash5[6784];
extern const uint32_t sample_17_standard_DRUMS_chcrash[4864];
extern const uint32_t sample_18_standard_DRUMS_tamborine[1920];
extern const uint32_t sample_19_standard_DRUMS_crash5[6784];
extern const uint32_t sample_20_standard_DRUMS_cowbell[896];
extern const uint32_t sample_21_standard_DRUMS_crash5[6784];
extern const uint32_t sample_22_standard_DRUMS_vibraloop[512];
extern const uint32_t sample_23_standard_DRUMS_paisteping[6656];
extern const uint32_t sample_24_standard_DRUMS_mbongotone[1408];
extern const uint32_t sample_25_standard_DRUMS_quintoslap[1536];
extern const uint32_t sample_26_standard_DRUMS_quintotone[1664];
extern const uint32_t sample_27_standard_DRUMS_lowtumba[2048];
extern const uint32_t sample_28_standard_DRUMS_timpani[3968];
extern const uint32_t sample_29_standard_DRUMS_agogolotone[2304];
extern const uint32_t sample_30_standard_DRUMS_cabasastrk[1408];
extern const uint32_t sample_31_standard_DRUMS_maracas[1664];
extern const uint32_t sample_32_standard_DRUMS_sambawhistle[896];
extern const uint32_t sample_33_standard_DRUMS_guirodown[1408];
extern const uint32_t sample_34_standard_DRUMS_guiro2[1408];
extern const uint32_t sample_35_standard_DRUMS_bockclave[896];
extern const uint32_t sample_36_standard_DRUMS_woodblock[640];
extern const uint32_t sample_37_standard_DRUMS_quicadown[896];
extern const uint32_t sample_38_standard_DRUMS_triangle[1152];
extern const uint32_t sample_39_standard_DRUMS_cabasastrk[1408];
extern const uint32_t sample_40_standard_DRUMS_tamborine[1920];
extern const uint32_t sample_41_standard_DRUMS_chcrash[4864];
extern const uint32_t sample_42_standard_DRUMS_belltree[3840];
extern const uint32_t sample_43_standard_DRUMS_ebongostone[1664];
extern const uint32_t sample_44_standard_DRUMS_sinetick[128];
extern const uint32_t sample_45_standard_DRUMS_mbongotone[1408];
extern const uint32_t sample_46_standard_DRUMS_floortombrite[3712];
extern const uint32_t sample_47_standard_DRUMS_timpani[3968];
//steelstrg
extern const AudioSynthWavetable::sample_data steelstrgtr_samples[2];
const uint8_t steelstrgtr_ranges[] = {72, 127, };
const AudioSynthWavetable::instrument_data steelstrgtr = {2, steelstrgtr_ranges, steelstrgtr_samples };
extern const uint32_t sample_0_steelstrgtr_acgtrg2[2560];
extern const uint32_t sample_1_steelstrgtr_acgtrb3[3200];
//strg
extern const AudioSynthWavetable::sample_data strings_samples[3];
const uint8_t strings_ranges[] = {59, 72, 127, };
const AudioSynthWavetable::instrument_data strings = {3, strings_ranges, strings_samples };
extern const uint32_t sample_0_strings_stringsg2[4736];
extern const uint32_t sample_1_strings_stringsf3[4352];
extern const uint32_t sample_2_strings_stringsdx4[5376];
//timpani
extern const AudioSynthWavetable::sample_data timpani_samples[3];
const uint8_t timpani_ranges[] = {59, 72, 127, };
const AudioSynthWavetable::instrument_data timpani = {3, timpani_ranges, timpani_samples };
extern const uint32_t sample_0_timpani_timpani[3968];
extern const uint32_t sample_1_timpani_timpani[3968];
extern const uint32_t sample_2_timpani_timpani[3968];
//trombone
extern const AudioSynthWavetable::sample_data trombone_samples[4];
const uint8_t trombone_ranges[] = {52, 59, 64, 127, };
const AudioSynthWavetable::instrument_data trombone = {4, trombone_ranges, trombone_samples };
extern const uint32_t sample_0_trombone_tromb2[768];
extern const uint32_t sample_1_trombone_troma3[768];
extern const uint32_t sample_2_trombone_tromd4[640];
extern const uint32_t sample_3_trombone_tromg4[896];
//trumpet
extern const AudioSynthWavetable::sample_data trumpet_samples[5];
const uint8_t trumpet_ranges[] = {64, 69, 74, 79, 127, };
const AudioSynthWavetable::instrument_data trumpet = {5, trumpet_ranges, trumpet_samples };
extern const uint32_t sample_0_trumpet_htrumpetd2[896];
extern const uint32_t sample_1_trumpet_htrumpetg2[896];
extern const uint32_t sample_2_trumpet_htrumpetc3[896];
extern const uint32_t sample_3_trumpet_htrumpetf3[768];
extern const uint32_t sample_4_trumpet_htrumpetax3[896];
//tuba
extern const AudioSynthWavetable::sample_data tuba_samples[2];
const uint8_t tuba_ranges[] = {86, 127, };
const AudioSynthWavetable::instrument_data tuba = {2, tuba_ranges, tuba_samples };
extern const uint32_t sample_0_tuba_tubaax1[1024];
extern const uint32_t sample_1_tuba_tromg4[896];
//vibraphone
extern const AudioSynthWavetable::sample_data vibraphone_samples[2];
const uint8_t vibraphone_ranges[] = {94, 127, };
const AudioSynthWavetable::instrument_data vibraphone = {2, vibraphone_ranges, vibraphone_samples };
extern const uint32_t sample_0_vibraphone_vibese2[512];
extern const uint32_t sample_1_vibraphone_vibese2[512];





#endif