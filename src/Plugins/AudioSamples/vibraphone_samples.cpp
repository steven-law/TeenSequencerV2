#include "Plugins/AudioSamples/AudioSamples.h"

PROGMEM const AudioSynthWavetable::sample_data vibraphone_samples[2] = {
	{
		(int16_t*)sample_0_vibraphone_vibese2, // sample
		true, // LOOP
		10, // LENGTH_BITS
		(1 << (32 - 10)) * WAVETABLE_CENTS_SHIFT(24) * 44100.0 / WAVETABLE_NOTE_TO_FREQUENCY(73) / AUDIO_SAMPLE_RATE_EXACT + 0.5, // PER_HERTZ_PHASE_INCREMENT
		((uint32_t)781 - 1) << (32 - 10), // MAX_PHASE
		((uint32_t)777 - 1) << (32 - 10), // LOOP_PHASE_END
		(((uint32_t)777 - 1) << (32 - 10)) - (((uint32_t)696 - 1) << (32 - 10)), // LOOP_PHASE_LENGTH
		uint16_t(UINT16_MAX * WAVETABLE_DECIBEL_SHIFT(-8.2)), // INITIAL_ATTENUATION_SCALAR
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // DELAY_COUNT
		uint32_t(6.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // ATTACK_COUNT
		uint32_t(20.01 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // HOLD_COUNT
		uint32_t(5819.25 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // DECAY_COUNT
		uint32_t(1487.96 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // RELEASE_COUNT
		int32_t((1.0 - WAVETABLE_DECIBEL_SHIFT(-100.0)) * AudioSynthWavetable::UNITY_GAIN), // SUSTAIN_MULT
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / (2 * AudioSynthWavetable::LFO_PERIOD)), // VIBRATO_DELAY
		uint32_t(0.1 * AudioSynthWavetable::LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // VIBRATO_INCREMENT
		(WAVETABLE_CENTS_SHIFT(0) - 1.0) * 4, // VIBRATO_PITCH_COEFFICIENT_INITIAL
		(1.0 - WAVETABLE_CENTS_SHIFT(0)) * 4, // VIBRATO_COEFFICIENT_SECONDARY
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / (2 * AudioSynthWavetable::LFO_PERIOD)), // MODULATION_DELAY
		uint32_t(5.6 * AudioSynthWavetable::LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // MODULATION_INCREMENT
		(WAVETABLE_CENTS_SHIFT(0) - 1.0) * 4, // MODULATION_PITCH_COEFFICIENT_INITIAL
		(1.0 - WAVETABLE_CENTS_SHIFT(0)) * 4, // MODULATION_PITCH_COEFFICIENT_SECOND
		int32_t(UINT16_MAX * (WAVETABLE_DECIBEL_SHIFT(2.0) - 1.0)) * 4, // MODULATION_AMPLITUDE_INITIAL_GAIN
		int32_t(UINT16_MAX * (1.0 - WAVETABLE_DECIBEL_SHIFT(-2.0))) * 4, // MODULATION_AMPLITUDE_FINAL_GAIN
	},
	{
		(int16_t*)sample_1_vibraphone_vibese2, // sample
		true, // LOOP
		10, // LENGTH_BITS
		(1 << (32 - 10)) * WAVETABLE_CENTS_SHIFT(24) * 44100.0 / WAVETABLE_NOTE_TO_FREQUENCY(73) / AUDIO_SAMPLE_RATE_EXACT + 0.5, // PER_HERTZ_PHASE_INCREMENT
		((uint32_t)781 - 1) << (32 - 10), // MAX_PHASE
		((uint32_t)777 - 1) << (32 - 10), // LOOP_PHASE_END
		(((uint32_t)777 - 1) << (32 - 10)) - (((uint32_t)696 - 1) << (32 - 10)), // LOOP_PHASE_LENGTH
		uint16_t(UINT16_MAX * WAVETABLE_DECIBEL_SHIFT(-8.2)), // INITIAL_ATTENUATION_SCALAR
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // DELAY_COUNT
		uint32_t(6.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // ATTACK_COUNT
		uint32_t(20.01 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // HOLD_COUNT
		uint32_t(2439.64 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // DECAY_COUNT
		uint32_t(1487.96 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // RELEASE_COUNT
		int32_t((1.0 - WAVETABLE_DECIBEL_SHIFT(-100.0)) * AudioSynthWavetable::UNITY_GAIN), // SUSTAIN_MULT
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / (2 * AudioSynthWavetable::LFO_PERIOD)), // VIBRATO_DELAY
		uint32_t(0.1 * AudioSynthWavetable::LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // VIBRATO_INCREMENT
		(WAVETABLE_CENTS_SHIFT(0) - 1.0) * 4, // VIBRATO_PITCH_COEFFICIENT_INITIAL
		(1.0 - WAVETABLE_CENTS_SHIFT(0)) * 4, // VIBRATO_COEFFICIENT_SECONDARY
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / (2 * AudioSynthWavetable::LFO_PERIOD)), // MODULATION_DELAY
		uint32_t(5.6 * AudioSynthWavetable::LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // MODULATION_INCREMENT
		(WAVETABLE_CENTS_SHIFT(0) - 1.0) * 4, // MODULATION_PITCH_COEFFICIENT_INITIAL
		(1.0 - WAVETABLE_CENTS_SHIFT(0)) * 4, // MODULATION_PITCH_COEFFICIENT_SECOND
		int32_t(UINT16_MAX * (WAVETABLE_DECIBEL_SHIFT(2.0) - 1.0)) * 4, // MODULATION_AMPLITUDE_INITIAL_GAIN
		int32_t(UINT16_MAX * (1.0 - WAVETABLE_DECIBEL_SHIFT(-2.0))) * 4, // MODULATION_AMPLITUDE_FINAL_GAIN
	},
};

PROGMEM const uint32_t sample_0_vibraphone_vibese2[512] = {
0x00000000,0x003e002d,0x006e0058,0x00860084,0x0080007b,0x007f0081,0x007d007f,0x007d007f,
0x007f007d,0x007f007f,0x008d007d,0x008a0092,0x00db00bb,0x00e400e2,0x011200cc,0x01d40071,
0x09deffb6,0x02a00139,0x67a83628,0x47bd7648,0xf6ba14b3,0xb723d7e0,0xd37ebea6,0x9e44c009,
0xc4ac9c07,0x29d6f963,0x15d23d57,0xb361dc96,0x0241d2a6,0x31902115,0xf8221b31,0xd539d517,
0x1347ea36,0x42b145ea,0xf87f1ef6,0xe730e564,0x2738fbe6,0x1cbf37d4,0xbc9ee8fd,0xedc5be4b,
0x360228f3,0x23e23172,0x058e0d14,0x35810ce3,0x3bd7498a,0xeb2617ae,0xece5e130,0x15920480,
0x15b423cf,0xc5c6e1e1,0xf5eacbeb,0x250a1968,0x00a622c4,0xe6f9f257,0xf2c1df34,0x243e1448,
0xec7405d5,0xce1ad853,0xdd70d206,0xeebef122,0xe398eecd,0xef19d542,0x0fe30927,0x13180cd5,
0xfa111361,0xe403e6ec,0x0eb8fde9,0xfda6107a,0xed19e248,0xf508f289,0x18b60799,0xfd011540,
0x051df85d,0x1e8b155e,0x22d020ad,0x13691882,0xe9a0f831,0x16210640,0x011c132b,0x0608ff03,
0x1b650c38,0x33b524c2,0x3191362b,0x17481e4d,0x12d3262b,0x0cca06c9,0x0bb71163,0xe593f19c,
0xf6b6e67d,0xfb870258,0xfc9bf945,0xebad016d,0xf7fae74e,0x0529fd31,0xf7a0fc8f,0xe5e1efa8,
0xe680e3b8,0xff28fb59,0xcd9ce9d7,0xdfa3cd91,0xea29dfe0,0xf235ef63,0xddd6efad,0xd7a5d779,
0x09fef594,0xfea606fb,0xe4d3f0ec,0xeafbd7d6,0x18fb05ee,0x05cf1ba2,0x00ddfcdb,0x13ef0d6d,
0x30d21531,0x1f813668,0xfe91081c,0x10fb056e,0x18e22053,0x08e3131e,0x0382fde5,0x390b1689,
0x255836cd,0x1f5023bc,0x1edc1ee7,0x2d562588,0x217431ea,0xf425fe0c,0x0917f983,0xfe1907f9,
0xe508fcf2,0xd9acd979,0xf888e573,0xfc81ffe1,0xf1fbf0bf,0xe87dee0b,0xfb76eb66,0x00bc11d0,
0xd0c9e44b,0xd5d9c554,0xe555df5f,0xded7e599,0xbbb0ce3d,0xd84ac32d,0xf83ef406,0xfb7bfb2f,
0xe243e89b,0xfa36e283,0x1e4a1ad0,0xeb2e0817,0xea42e2b4,0x0cb2fb0b,0x28121c07,0x0f741b5f,
0x0ca905cc,0x2e7b2633,0x349237cf,0x0a5723bb,0x083dfaa8,0x399323ad,0x1b452f2b,0x13d210cf,
0x26382277,0x49fe3902,0x2d553fc9,0x1287159b,0x1ff215aa,0x1e392623,0xf2ea0faa,0xdc17e142,
0x05e7f3fd,0xf048fb56,0xe258e3d6,0xe52de232,0x0011f4bf,0xf7530787,0xcdb3db9f,0xe4ced38c,
0xf5e7eb68,0xce2de8e5,0xb0c4b879,0xda55c813,0xe1dfe795,0xd983d7db,0xd2add58b,0xfaa8dc74,
0x04020bbc,0xdc9af1ee,0xe9eede54,0x109cfcd4,0x120f1297,0xf7400056,0x180d0312,0x3029296b,
0x2d9f35c0,0x12602322,0x29eb1217,0x418c3b21,0x0efb2ac8,0x0f8e0a75,0x34f52207,0x38403f5d,
0x15712559,0x21080fd0,0x377e2bfb,0x26eb367a,0xfc33172a,0x07aff950,0x205919b4,0xeb560ac7,
0xe165dba9,0xf0b7e6df,0xf941fbe8,0xdf0cf264,0xda3bd6e8,0xf2b4e0b8,0xed84f34f,0xcc9ae1a5,
0xc1bdb95f,0xe398dd3c,0xcd28df12,0xc18ac1e9,0xcff3c290,0xefdddc57,0xef66fdf3,0xdbd7de7c,
0xf44ee73f,0x110b05a7,0xfcc90a13,0xeccbe962,0x1312fe02,0x15e41fda,0x117f132a,0x09c90bf4,
0x33cf189e,0x3d2f420c,0x16f42780,0x1f67154f,0x3eef3455,0x38fd4687,0x143e1b36,0x33c71e23,
0x3d403b20,0x297d386c,0x0f981c1a,0x24fd149c,0x38383d06,0xfef015fc,0xea19ec92,0xfbd0f1a9,
0xf57201c5,0xd4d8e5c0,0xe13dd302,0xf5a8ed4f,0xf622f674,0xcac9dfbf,0xd438c735,0xf099eece,
0xc416dedb,0xb908b4b4,0xc980be38,0xdd75d774,0xc9f5d4a7,0xd07dcb4f,0xf1b3e382,0x07d2fe11,
0xf0bbfdd4,0xed7ae14e,0x186f0a8f,0x08c71498,0xf9d700be,0x0aa60140,0x31ca1a6f,0x2d123607,
0x19d61da0,0x2c932436,0x521d426d,0x2b414703,0x17cf18e2,0x3df02493,0x33c03e22,0x207728b6,
0x184c1d73,0x3b3b28e2,0x34b33df5,0x0e962159,0x0fa907bf,0x198814fb,0xf96c14ed,0xd630e2b4,
0xed9fe0a8,0xee8cecca,0xdd02e613,0xd087d454,0xe12ad2d5,0xead7f210,0xc46eda4e,0xbc42b9f3,
0xd717c370,0xcda5d9fd,0xb248bed8,0xc188b604,0xdae2d102,0xef1be82c,0xd996e48b,0xeae7d55d,
0x0ccf0155,0xfb4c0bdc,0xf07cf2fc,0x03adf5ec,0x1be113be,0x1167158d,0x166e1121,0x2c5e1fb3,
0x4afa420f,0x33154770,0x2f3d23ae,0x4f334095,0x3da14a4f,0x20552d1d,0x277620c3,0x3e893659,
0x2f233db6,0x1dc91fc8,0x1f59190b,0x353c2dfd,0x0ab32aa5,0xec97f50d,0xfc43f2a9,0xf089fb03,
0xdc0ee305,0xcb44cced,0xe1e2d7cd,0xe629ea93,0xce05dd44,0xc4d1c17d,0xd946cdab,0xcd45dbff,
0xa9bbb26c,0xb910b523,0xc845c284,0xcea2ccaa,0xc731c749,0xdcdaca25,0xfea1f4db,0xf4fefb2a,
0xe7f2eea0,0x0896f5a4,0x14c71384,0x04e808af,0x0d7d06d0,0x23001a9e,0x38993025,0x296d340e,
0x3b212bc0,0x54c64a32,0x40444bcc,0x3804392e,0x417e3fd2,0x39f84186,0x28252d68,0x31912a50,
0x37183590,0x29da30ec,0x1b40250d,0x25dc1cbd,0x23cc2c3f,0xf88c0aa7,0xf15eef25,0xf59cf6f1,
0xe30bed5c,0xce3cdbcf,0xd76ccd3b,0xe75fe004,0xdc52e3fd,0xc23bcdee,0xcd1ec4e1,0xda3dd81e,
0xb27ccba0,0xb555aad9,0xc25db929,0xccadc828,0xc77fce82,0xca9ac775,0xf4ebdd3a,0xfb21fe8e,
0xee9df4fd,0xf5ace7fd,0x13860896,0x08d414b1,0x06d004e8,0x1a710d7d,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
};

PROGMEM const uint32_t sample_1_vibraphone_vibese2[512] = {
0x00000000,0x003e002d,0x006e0058,0x00860084,0x0080007b,0x007f0081,0x007d007f,0x007d007f,
0x007f007d,0x007f007f,0x008d007d,0x008a0092,0x00db00bb,0x00e400e2,0x011200cc,0x01d40071,
0x09deffb6,0x02a00139,0x67a83628,0x47bd7648,0xf6ba14b3,0xb723d7e0,0xd37ebea6,0x9e44c009,
0xc4ac9c07,0x29d6f963,0x15d23d57,0xb361dc96,0x0241d2a6,0x31902115,0xf8221b31,0xd539d517,
0x1347ea36,0x42b145ea,0xf87f1ef6,0xe730e564,0x2738fbe6,0x1cbf37d4,0xbc9ee8fd,0xedc5be4b,
0x360228f3,0x23e23172,0x058e0d14,0x35810ce3,0x3bd7498a,0xeb2617ae,0xece5e130,0x15920480,
0x15b423cf,0xc5c6e1e1,0xf5eacbeb,0x250a1968,0x00a622c4,0xe6f9f257,0xf2c1df34,0x243e1448,
0xec7405d5,0xce1ad853,0xdd70d206,0xeebef122,0xe398eecd,0xef19d542,0x0fe30927,0x13180cd5,
0xfa111361,0xe403e6ec,0x0eb8fde9,0xfda6107a,0xed19e248,0xf508f289,0x18b60799,0xfd011540,
0x051df85d,0x1e8b155e,0x22d020ad,0x13691882,0xe9a0f831,0x16210640,0x011c132b,0x0608ff03,
0x1b650c38,0x33b524c2,0x3191362b,0x17481e4d,0x12d3262b,0x0cca06c9,0x0bb71163,0xe593f19c,
0xf6b6e67d,0xfb870258,0xfc9bf945,0xebad016d,0xf7fae74e,0x0529fd31,0xf7a0fc8f,0xe5e1efa8,
0xe680e3b8,0xff28fb59,0xcd9ce9d7,0xdfa3cd91,0xea29dfe0,0xf235ef63,0xddd6efad,0xd7a5d779,
0x09fef594,0xfea606fb,0xe4d3f0ec,0xeafbd7d6,0x18fb05ee,0x05cf1ba2,0x00ddfcdb,0x13ef0d6d,
0x30d21531,0x1f813668,0xfe91081c,0x10fb056e,0x18e22053,0x08e3131e,0x0382fde5,0x390b1689,
0x255836cd,0x1f5023bc,0x1edc1ee7,0x2d562588,0x217431ea,0xf425fe0c,0x0917f983,0xfe1907f9,
0xe508fcf2,0xd9acd979,0xf888e573,0xfc81ffe1,0xf1fbf0bf,0xe87dee0b,0xfb76eb66,0x00bc11d0,
0xd0c9e44b,0xd5d9c554,0xe555df5f,0xded7e599,0xbbb0ce3d,0xd84ac32d,0xf83ef406,0xfb7bfb2f,
0xe243e89b,0xfa36e283,0x1e4a1ad0,0xeb2e0817,0xea42e2b4,0x0cb2fb0b,0x28121c07,0x0f741b5f,
0x0ca905cc,0x2e7b2633,0x349237cf,0x0a5723bb,0x083dfaa8,0x399323ad,0x1b452f2b,0x13d210cf,
0x26382277,0x49fe3902,0x2d553fc9,0x1287159b,0x1ff215aa,0x1e392623,0xf2ea0faa,0xdc17e142,
0x05e7f3fd,0xf048fb56,0xe258e3d6,0xe52de232,0x0011f4bf,0xf7530787,0xcdb3db9f,0xe4ced38c,
0xf5e7eb68,0xce2de8e5,0xb0c4b879,0xda55c813,0xe1dfe795,0xd983d7db,0xd2add58b,0xfaa8dc74,
0x04020bbc,0xdc9af1ee,0xe9eede54,0x109cfcd4,0x120f1297,0xf7400056,0x180d0312,0x3029296b,
0x2d9f35c0,0x12602322,0x29eb1217,0x418c3b21,0x0efb2ac8,0x0f8e0a75,0x34f52207,0x38403f5d,
0x15712559,0x21080fd0,0x377e2bfb,0x26eb367a,0xfc33172a,0x07aff950,0x205919b4,0xeb560ac7,
0xe165dba9,0xf0b7e6df,0xf941fbe8,0xdf0cf264,0xda3bd6e8,0xf2b4e0b8,0xed84f34f,0xcc9ae1a5,
0xc1bdb95f,0xe398dd3c,0xcd28df12,0xc18ac1e9,0xcff3c290,0xefdddc57,0xef66fdf3,0xdbd7de7c,
0xf44ee73f,0x110b05a7,0xfcc90a13,0xeccbe962,0x1312fe02,0x15e41fda,0x117f132a,0x09c90bf4,
0x33cf189e,0x3d2f420c,0x16f42780,0x1f67154f,0x3eef3455,0x38fd4687,0x143e1b36,0x33c71e23,
0x3d403b20,0x297d386c,0x0f981c1a,0x24fd149c,0x38383d06,0xfef015fc,0xea19ec92,0xfbd0f1a9,
0xf57201c5,0xd4d8e5c0,0xe13dd302,0xf5a8ed4f,0xf622f674,0xcac9dfbf,0xd438c735,0xf099eece,
0xc416dedb,0xb908b4b4,0xc980be38,0xdd75d774,0xc9f5d4a7,0xd07dcb4f,0xf1b3e382,0x07d2fe11,
0xf0bbfdd4,0xed7ae14e,0x186f0a8f,0x08c71498,0xf9d700be,0x0aa60140,0x31ca1a6f,0x2d123607,
0x19d61da0,0x2c932436,0x521d426d,0x2b414703,0x17cf18e2,0x3df02493,0x33c03e22,0x207728b6,
0x184c1d73,0x3b3b28e2,0x34b33df5,0x0e962159,0x0fa907bf,0x198814fb,0xf96c14ed,0xd630e2b4,
0xed9fe0a8,0xee8cecca,0xdd02e613,0xd087d454,0xe12ad2d5,0xead7f210,0xc46eda4e,0xbc42b9f3,
0xd717c370,0xcda5d9fd,0xb248bed8,0xc188b604,0xdae2d102,0xef1be82c,0xd996e48b,0xeae7d55d,
0x0ccf0155,0xfb4c0bdc,0xf07cf2fc,0x03adf5ec,0x1be113be,0x1167158d,0x166e1121,0x2c5e1fb3,
0x4afa420f,0x33154770,0x2f3d23ae,0x4f334095,0x3da14a4f,0x20552d1d,0x277620c3,0x3e893659,
0x2f233db6,0x1dc91fc8,0x1f59190b,0x353c2dfd,0x0ab32aa5,0xec97f50d,0xfc43f2a9,0xf089fb03,
0xdc0ee305,0xcb44cced,0xe1e2d7cd,0xe629ea93,0xce05dd44,0xc4d1c17d,0xd946cdab,0xcd45dbff,
0xa9bbb26c,0xb910b523,0xc845c284,0xcea2ccaa,0xc731c749,0xdcdaca25,0xfea1f4db,0xf4fefb2a,
0xe7f2eea0,0x0896f5a4,0x14c71384,0x04e808af,0x0d7d06d0,0x23001a9e,0x38993025,0x296d340e,
0x3b212bc0,0x54c64a32,0x40444bcc,0x3804392e,0x417e3fd2,0x39f84186,0x28252d68,0x31912a50,
0x37183590,0x29da30ec,0x1b40250d,0x25dc1cbd,0x23cc2c3f,0xf88c0aa7,0xf15eef25,0xf59cf6f1,
0xe30bed5c,0xce3cdbcf,0xd76ccd3b,0xe75fe004,0xdc52e3fd,0xc23bcdee,0xcd1ec4e1,0xda3dd81e,
0xb27ccba0,0xb555aad9,0xc25db929,0xccadc828,0xc77fce82,0xca9ac775,0xf4ebdd3a,0xfb21fe8e,
0xee9df4fd,0xf5ace7fd,0x13860896,0x08d414b1,0x06d004e8,0x1a710d7d,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
};
