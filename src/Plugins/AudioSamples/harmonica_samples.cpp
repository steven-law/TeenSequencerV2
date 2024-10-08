#include "Plugins/AudioSamples/AudioSamples.h"

PROGMEM const AudioSynthWavetable::sample_data harmonica_samples[1] = {
	{
		(int16_t*)sample_0_harmonica_harmonicaa3, // sample
		true, // LOOP
		10, // LENGTH_BITS
		(1 << (32 - 10)) * WAVETABLE_CENTS_SHIFT(33) * 44100.0 / WAVETABLE_NOTE_TO_FREQUENCY(92) / AUDIO_SAMPLE_RATE_EXACT + 0.5, // PER_HERTZ_PHASE_INCREMENT
		((uint32_t)973 - 1) << (32 - 10), // MAX_PHASE
		((uint32_t)968 - 1) << (32 - 10), // LOOP_PHASE_END
		(((uint32_t)968 - 1) << (32 - 10)) - (((uint32_t)914 - 1) << (32 - 10)), // LOOP_PHASE_LENGTH
		uint16_t(UINT16_MAX * WAVETABLE_DECIBEL_SHIFT(-10.5)), // INITIAL_ATTENUATION_SCALAR
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // DELAY_COUNT
		uint32_t(6.20 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // ATTACK_COUNT
		uint32_t(20.01 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // HOLD_COUNT
		uint32_t(20.01 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // DECAY_COUNT
		uint32_t(100.02 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // RELEASE_COUNT
		int32_t((1.0 - WAVETABLE_DECIBEL_SHIFT(-0.0)) * AudioSynthWavetable::UNITY_GAIN), // SUSTAIN_MULT
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / (2 * AudioSynthWavetable::LFO_PERIOD)), // VIBRATO_DELAY
		uint32_t(0.1 * AudioSynthWavetable::LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // VIBRATO_INCREMENT
		(WAVETABLE_CENTS_SHIFT(0) - 1.0) * 4, // VIBRATO_PITCH_COEFFICIENT_INITIAL
		(1.0 - WAVETABLE_CENTS_SHIFT(0)) * 4, // VIBRATO_COEFFICIENT_SECONDARY
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / (2 * AudioSynthWavetable::LFO_PERIOD)), // MODULATION_DELAY
		uint32_t(6.1 * AudioSynthWavetable::LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // MODULATION_INCREMENT
		(WAVETABLE_CENTS_SHIFT(0) - 1.0) * 4, // MODULATION_PITCH_COEFFICIENT_INITIAL
		(1.0 - WAVETABLE_CENTS_SHIFT(0)) * 4, // MODULATION_PITCH_COEFFICIENT_SECOND
		int32_t(UINT16_MAX * (WAVETABLE_DECIBEL_SHIFT(1.6) - 1.0)) * 4, // MODULATION_AMPLITUDE_INITIAL_GAIN
		int32_t(UINT16_MAX * (1.0 - WAVETABLE_DECIBEL_SHIFT(-1.6))) * 4, // MODULATION_AMPLITUDE_FINAL_GAIN
	},
};

PROGMEM const uint32_t sample_0_harmonica_harmonicaa3[512] = {
0x00000000,0xfe50fc97,0x01450058,0x03e003c8,0x0639054e,0x076007ae,0x066806d2,0x02e4058d,
0xff59028b,0xfcfdfdb7,0xf88df9e0,0xf6acf8d0,0xf890f7d6,0xfa43f87b,0xfbbcfa4b,0xfe74fe06,
0x01890168,0x05a004bd,0x07f60628,0x091f08aa,0x06f608d3,0x040b0665,0xff9a0140,0xfa81fd24,
0xf719f953,0xf5f6f735,0xf673f694,0xf9cef7b2,0xfbebfadc,0x0097fed5,0x042b018e,0x08550452,
0x08c007c6,0x0a2a0a53,0x088e0bc3,0x03e90676,0xfe8d0219,0xf89cfc67,0xf680f73d,0xf47af4f9,
0xf626f524,0xf876f900,0xfba9fa33,0x00b5fea9,0x06370219,0x095105a8,0x0b820be5,0x0c2e0d69,
0x09890c11,0x029e071f,0xfd8300ca,0xf765f94c,0xf511f438,0xf48ff2cd,0xf5ebf371,0xf7d0f66a,
0xfbb8fa6a,0x015cffb2,0x066c048e,0x0b950a03,0x0e7a0c8a,0x0f830e48,0x08730ce8,0x024b06b0,
0xfaf9ff7a,0xf4a3f6df,0xf182f3c3,0xf182f1f7,0xf417f291,0xf7fbf60e,0xfccbfb33,0x0229fecd,
0x08ad04c3,0x0de20b01,0x12770ee8,0x10b21191,0x0a230ddb,0x013204c2,0xf84bfbc0,0xf0c0f407,
0xf054eff5,0xf109efa1,0xf1faf170,0xf830f63f,0xfee2fb50,0x040efe9b,0x09fd0768,0x12830db2,
0x15c5129a,0x11ad14fb,0x08050eb9,0xfdf202a5,0xf3c8fa14,0xee28f121,0xedc7ee71,0xeed3ee38,
0xf262ef6b,0xf94bf453,0xff36fb20,0x04e40176,0x0c560b65,0x17001189,0x1a2e168a,0x10a316a6,
0x05b80cc9,0xfb390081,0xf1a5f6f7,0xeccaeedf,0xed45ec96,0xec25ecc5,0xf13eed89,0xfa3ef5ef,
0xfecdfd7a,0x07ce037f,0x13e00cce,0x1aa31592,0x1cff1dfb,0x0f2714f9,0x03260873,0xf71dfe20,
0xef66f372,0xecb6ec00,0xea4cec35,0xe9d6e930,0xf27fed6c,0xf928f823,0x01bbfd05,0x0d420704,
0x17251246,0x20f41d8b,0x18db2260,0x0af71299,0x006704fc,0xf3b0f915,0xee4af07d,0xecfceace,
0xe77de9e0,0xe958e6a4,0xf461ee41,0xf91df875,0x03d400e9,0x11950a40,0x1da41561,0x26d623ca,
0x15e41f27,0x070c0e09,0xfab5032c,0xf0a3f4ab,0xeb9ff0b2,0xead3ea3d,0xe4e5e4e2,0xe887e503,
0xf5e7f071,0xfd36f678,0x072e041c,0x14c71077,0x27c41d0c,0x228a2e2b,0x11231983,0x066c0884,
0xf4ddfc35,0xf12ff11c,0xe8bfef16,0xe5f1eb09,0xe2a1e185,0xec23e4bf,0xf318f343,0xfffffa5e,
0x0cbf06ac,0x1c6813fb,0x3500265d,0x1cd9289c,0x0943153f,0xffe108bd,0xf225f51c,0xf281f1e1,
0xe7fbecae,0xdf24e7c3,0xe1e6e06a,0xf0ece7f3,0xf63df076,0x03d2fe48,0x0fdf0c8b,0x25731b19,
0x32ce3823,0x19ee1f16,0x0ba709c4,0xf7d302e3,0xf2d5f10c,0xf0a2f33f,0xe86de734,0xdc4ddf7f,
0xe1e3e054,0xee09ee10,0xfb56f2bd,0x0a41015e,0x18380f28,0x38922522,0x21953c5c,0x0b821cdf,
0x07490c05,0xf0d9f9a1,0xf5c6f2bc,0xe9faf3db,0xe2d4e58f,0xdf69d856,0xea95ddf7,0xefa7ebe2,
0xffbaf752,0x0d4c0725,0x203215ec,0x3f9338e0,0x1d8d2951,0x0c5c0fcb,0xfdf90aae,0xf161f143,
0xf757f71d,0xe407edcc,0xd705e3c8,0xdc2fdd8c,0xeb91e4f7,0xf7a1eb1d,0x06a1faff,0x12460a29,
0x34d51fd0,0x2ddb490a,0x113421ff,0x0dfa0dc9,0xf30000cd,0xf88def77,0xf2aff946,0xe473e232,
0xd96fd854,0xdf83dc9f,0xe71eea29,0xf8b9f3b8,0x096603aa,0x1d060e8b,0x4dc730f4,0x2379345b,
0x0d171500,0x0592108a,0xee54f495,0xfbdef74d,0xe484f775,0xdaebe2e7,0xdd51d6d5,0xe7fcdbf2,
0xf0fee3b3,0x005ff6a9,0x0a8a0818,0x2c94187e,0x3eb74e89,0x1825256e,0x12660d68,0xf78d097f,
0xf63ced45,0xfbd0fe1e,0xe039e8a1,0xd348ddec,0xd929ddda,0xe15ce620,0xf4f4eb5f,0x0790fc67,
0x1716060d,0x4fc026d3,0x2533489e,0x0b8a1d70,0x0d7c1483,0xedcffb2b,0xfebef4ff,0xee1ffec3,
0xe0aeddaa,0xdc70d113,0xe234d8b7,0xe65ce03e,0xf865f4d2,0x05e4072e,0x229212bf,0x51ae4b58,
0x1fff27cf,0x14700c13,0xff681218,0xf349ede4,0x0111ff47,0xdc9ff4ab,0xd18be0fb,0xd94ed9ed,
0xe024df5e,0xf41ae1d9,0x051ef577,0x0da80452,0x44d71f81,0x2a9c58fd,0x0dc5215c,0x15df1365,
0xf02002e8,0x0014f16f,0xf94202f3,0xe04ddcaa,0xd68ad3d7,0xdb72da82,0xdd0ee142,0xf394f329,
0x04760155,0x1ca708af,0x60553d6b,0x24552efe,0x116c105d,0x062619c2,0xef1cf2d6,0x0360002a,
0xdf86ff4a,0xd5f6de0a,0xdcecd24a,0xe249d8ea,0xf0b0d7f0,0xfeaff2e4,0x0436058b,0x371a17f0,
0x356165f0,0x117124f5,0x1d600f8a,0xf61c0abe,0xff11ed5b,0x0374057b,0xda9fe393,0xcf5ed9b1,
0xd761de85,0xd4e2e1e8,0xf1ccecd8,0x0660f9f8,0x15a701c6,0x682a2e57,0x24ba3cf7,0x0b3c16e8,
0x0d0d2040,0xecaef9bd,0x06a7fe6d,0xe9ce05df,0xdd2ad74e,0xdf03cc4d,0xe1f3d627,0xe82bd242,
0xf66af2d6,0xff500564,0x28fc113c,0x45f86735,0x19bf2404,0x218e08f2,0xfc2a1121,0xfc06ecb2,
0x07c408ec,0xd406f0e1,0xcb04dfca,0xd7aadcca,0xd304dfaa,0xf302e332,0x0551f19e,0x0e89fe06,
0x643c2246,0x21e04fef,0x06411ead,0x150c21b2,0xee00ffa1,0x09c3fa24,0xf69609f2,0xdfffd375,
0xdac5cbe6,0xddfdd810,0xde10d3e0,0xf05bf3a2,0xfdba0356,0x1ea409d5,0x592d5e0a,0x220e2159,
0x20d70515,0x02751896,0xf78bf10c,0x0b560bbe,0xd42efcd9,0xcd9cdf2f,0xd9f3d783,0xd5b7dbef,
0xf3c8d867,0x0163ede4,0x07d2fcde,0x575b1818,0x211861cf,0x040426b9,0x1ac71efd,0xf2870580,
0x0bacf659,0x01740d18,0xde4fd70f,0xd3a0d029,0xd9b5da56,0xd3f9d883,0xeb6df39c,0xfdedfdf8,
0x164703da,0x68224f41,0x282221f4,0x1c0704ae,0x06bf1d7c,0xf685f6c6,0x0f620aa0,0xd9d80572,
0xd3c1dc18,0xdcccd015,0xd9ebd714,0xf222d095,0xfb1feb12,0x01b7feb9,0x45ca123d,0x24b8700c,
0x04f82a14,0x212316f5,0xf8e008d0,0x097df740,0x09e51141,0xdac5de6d,0xcc75d681,0xd648dd35,
0xcd9cdb47,0xeb7befeb,0xfeaff694,0x0fbdff4e,0x72e43d6b,0x2a22266d,0x12d50899,0x09732315,
0xf9cdfb59,0x120c096b,0xe1ff0d6a,0xdab6da56,0xddabca5b,0xdc8dd471,0xedf2cc82,0xf2c7ece7,
0xfd0afe8a,0x34ae0d77,0x2b077555,0x0c6a291f,0x26590c51,0xfcd10bd9,0x08d9fb72,0x114112ee,
0xd885e72d,0xc809deab,0xd35edcf1,0xcc6fdc66,0xee09eaf4,0xfe9eee7a,0x0d8cfbba,0x760d2e04,
0x27df2fae,0x07261157,0x0c9526dc,0xfcc7fe19,0x1272099a,0xea771472,0xe074d7f8,0xdc5cc79f,
0xdc88d361,0xe9a2cbfb,0xee03ee22,0xfb9afe3b,0x2d0c0d11,0x2ffc75f3,0x115727d8,0x26dc0726,
0xfe190c95,0x099afcc7,0x14721272,0xd7f8ea77,0xc79fe074,0xd361dc5c,0xcbfbdc88,0xee22e9a2,
0xfe3bee03,0x0d11fb9a,0x75f32d0c,0x27d82ffc,0x08cd1157,0x1164167f,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
};
