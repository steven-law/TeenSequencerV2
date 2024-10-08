#include "Plugins/AudioSamples/AudioSamples.h"

PROGMEM const AudioSynthWavetable::sample_data gtfretnoise_samples[1] = {
	{
		(int16_t*)sample_0_gtfretnoise_guitarfret, // sample
		false, // LOOP
		12, // LENGTH_BITS
		(1 << (32 - 12)) * WAVETABLE_CENTS_SHIFT(-3) * 44100.0 / WAVETABLE_NOTE_TO_FREQUENCY(81) / AUDIO_SAMPLE_RATE_EXACT + 0.5, // PER_HERTZ_PHASE_INCREMENT
		((uint32_t)3571 - 1) << (32 - 12), // MAX_PHASE
		((uint32_t)3567 - 1) << (32 - 12), // LOOP_PHASE_END
		(((uint32_t)3567 - 1) << (32 - 12)) - (((uint32_t)7 - 1) << (32 - 12)), // LOOP_PHASE_LENGTH
		uint16_t(UINT16_MAX * WAVETABLE_DECIBEL_SHIFT(-0.0)), // INITIAL_ATTENUATION_SCALAR
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // DELAY_COUNT
		uint32_t(6.20 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // ATTACK_COUNT
		uint32_t(20.01 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // HOLD_COUNT
		uint32_t(20.01 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // DECAY_COUNT
		uint32_t(281.10 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // RELEASE_COUNT
		int32_t((1.0 - WAVETABLE_DECIBEL_SHIFT(-0.0)) * AudioSynthWavetable::UNITY_GAIN), // SUSTAIN_MULT
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / (2 * AudioSynthWavetable::LFO_PERIOD)), // VIBRATO_DELAY
		uint32_t(0.1 * AudioSynthWavetable::LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // VIBRATO_INCREMENT
		(WAVETABLE_CENTS_SHIFT(0) - 1.0) * 4, // VIBRATO_PITCH_COEFFICIENT_INITIAL
		(1.0 - WAVETABLE_CENTS_SHIFT(0)) * 4, // VIBRATO_COEFFICIENT_SECONDARY
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / (2 * AudioSynthWavetable::LFO_PERIOD)), // MODULATION_DELAY
		uint32_t(5.4 * AudioSynthWavetable::LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // MODULATION_INCREMENT
		(WAVETABLE_CENTS_SHIFT(0) - 1.0) * 4, // MODULATION_PITCH_COEFFICIENT_INITIAL
		(1.0 - WAVETABLE_CENTS_SHIFT(0)) * 4, // MODULATION_PITCH_COEFFICIENT_SECOND
		int32_t(UINT16_MAX * (WAVETABLE_DECIBEL_SHIFT(0) - 1.0)) * 4, // MODULATION_AMPLITUDE_INITIAL_GAIN
		int32_t(UINT16_MAX * (1.0 - WAVETABLE_DECIBEL_SHIFT(0))) * 4, // MODULATION_AMPLITUDE_FINAL_GAIN
	},
};

PROGMEM const uint32_t sample_0_gtfretnoise_guitarfret[1792] = {
0x00000000,0x17361ac6,0x05fc120d,0xf4f8fbfb,0xe107e5e6,0xd813dbd6,0xe0d8dbb5,0xf0c6e800,
0x03ecf998,0x0f8009da,0x028e07ce,0xf023fadc,0xe4cee648,0xe44fe293,0xf052e7b2,0xf67cecb6,
0x00c3fc27,0x07a80679,0xffff014f,0xe81bf287,0xd576dc6d,0xcb5dd0c8,0xd051cb98,0xd7d7da76,
0xe91be2fd,0x01fff780,0x062f0d2f,0xf6090361,0xe2e8e9e6,0xd64adbd0,0xddcfd702,0xe8fade40,
0xf649ec79,0xfbb4fd81,0xe923f4ff,0xd17edb04,0xcb40cafe,0xd17ccab6,0xe39cd773,0xf956eca4,
0x076b036e,0x064f0ea3,0xf42efbf7,0xda84e271,0xc606cc6c,0xc67fc4a9,0xd591cb5b,0xf5c5e073,
0x05f501f1,0x03570992,0xf119fbfa,0xd74ae49b,0xcea9d105,0xd162d1b7,0xe0c3d984,0xe470e50a,
0xe455df9d,0xe68dde90,0xe8aae6dc,0xf0aced52,0x076c0035,0x09ff0c80,0xf1df036c,0xd252e308,
0xc043c71a,0xb8aeb864,0xc999bcf4,0xebecd79a,0x0a3400e3,0x0ae40fbe,0xf9c5071d,0xdfa3ed7b,
0xcbc2d3cd,0xd2fed155,0xec08defd,0xecb0edd9,0xe1dde6d7,0xd9bcded3,0xd769d7d0,0xe648dd21,
0xf99bf05e,0xfae6fd1e,0xeb53fb39,0xdbc9e43a,0xcd4ad4e7,0xd605cd58,0xec1cdb6d,0xf769f380,
0xfdf5f9ca,0xf153f617,0xd340e6b9,0xcf61ce91,0xdf04d5c5,0xe8f2eb83,0xded7e3ca,0xe184e0d3,
0xdf45dd43,0xf0dce4b8,0x0864fa3c,0xf349ff68,0xd8f2e9b3,0xcd87d2ce,0xb85dbd5d,0xc7e7bbca,
0xeabcdcde,0xfe7cf9b6,0xff51fe1b,0xf02afddf,0xdb51e0ab,0xd7d0d2f1,0xe110dcda,0xd67be300,
0xd30cd825,0xd176ce66,0xec93de75,0xf919f81e,0xf63ff8d9,0xe894f224,0xd138e19d,0xcb81ca01,
0xdc7bd04a,0xf0e2e5af,0x01e7f08c,0xf4c003d1,0xd4c2e382,0xd925d532,0xe32adb7b,0xe623e814,
0xf9f5fa84,0xec0cf575,0xf5ace6a7,0xf6b9f8dc,0xe08df02b,0xe2c3deb9,0xe106de10,0xd66ad817,
0xed8ee170,0x044cfdd5,0x0c5a095f,0xf97202dc,0xe931ecba,0xdfcfdebe,0xe077ddae,0xe50debca,
0xdcd4e6bd,0xeb53dedd,0xfee6f612,0x08340494,0x0b470cbf,0xe80df6fa,0xdbcacd20,0xe874e912,
0x02e7f5db,0x131411d3,0xf5540847,0xe281e153,0xf07fe774,0xf613e9ae,0x06c003e9,0xf5f00517,
0xe541ecd9,0xf0b2e927,0xf7a2f17c,0xee26fcea,0xded8ee24,0xed1deaa5,0x093ff591,0x07fc0753,
0xe941fa0c,0xde28e059,0xe5a5e72f,0xfc16ec2d,0xf8ac01fb,0xf2b0f2f1,0xfb8bf1e3,0xfcc4f64f,
0x0cc9fd4c,0xe044f328,0xdb85d8ff,0xe9c4ec0f,0x0030fdad,0x0edf0add,0xe9cbf986,0xebb6eeb3,
0xec4fec20,0x0202f762,0xfa02fa3f,0xe49be1b5,0xf23ee8b1,0xfbf5f9af,0xeee0fe24,0xe49debd1,
0xe94ee796,0x037af949,0x007ffe51,0xf3f4f33b,0xe17ced1b,0x0039f420,0xffc80e9f,0xf2f7ef87,
0xecbfe942,0xf839e4fe,0x04230420,0xd937ed01,0xfc5aed11,0xf2acf226,0x090c02f6,0xebb60e06,
0xe394de09,0xf4fef142,0x099ef679,0xfa8808a5,0xdc97e4d4,0x0265eec1,0x054f0c1b,0xfbb10e8d,
0xf691f413,0xe7e8e511,0xf7f1ff9f,0xf6a20198,0xeedfee71,0xf356e8dc,0x1985064a,0xf1ce0e0c,
0xe6e5e6f2,0xf573ee37,0x0b8f0898,0xf6490fc4,0xf8a8f626,0xec66ed6f,0x056bf943,0xfec40b36,
0xf5e2f1d6,0x0643f92d,0x09c1097a,0xe6c3f504,0xee57e45a,0x1458076e,0x08e90d06,0xfb8f0aba,
0xebf0fd42,0x03c0f509,0x05c40db7,0xec78ffb1,0xf621eeac,0x14c61261,0xf1870719,0xeeceed07,
0xfedff8d6,0x05fb0b87,0xf7ec0071,0xde06f5f3,0xfef9ed57,0x1a0e1c9e,0xea6405f4,0xf461f3bd,
0x15c308a3,0xf6dc084d,0xedaff3d4,0x05b20a17,0x00430b4d,0xf3890e6f,0xdc45e589,0x1172f35a,
0x07d60d05,0xec0aff51,0x001fe6ef,0x05c7160e,0xfaa9f195,0xfca6012d,0x0e20f3d8,0x07f30b62,
0xfc69fac8,0xef76f582,0x18410ada,0xe41f026a,0xfee4d88b,0x0910074d,0x11031851,0xedae0289,
0x076fff87,0x059b0ac9,0x095527ce,0xf5cfebb8,0x2b720ae3,0x00840a17,0xe5e2ec20,0x11eefbd5,
0x1559264e,0xf849033c,0xd9b7ed7e,0x05cff44d,0x0835240f,0xfd02fe8b,0x070bf783,0x0ae013e0,
0xdb40dfc6,0x1196fc77,0x2cc80d3c,0xf92729e3,0xf002e58d,0xe5cce69a,0x222a20ef,0xf6340512,
0x11171b62,0x053ef8bb,0xdd43f7a5,0x1d92def2,0x1f5620ae,0x261523fe,0xc601e7ec,0x0684eb42,
0x0d4d0ec6,0x101a225d,0x034a0623,0xf3f40b3c,0xf8cbce28,0xfdea0031,0x38080bdc,0xe91b10cd,
0xf587ea0e,0xf31addc2,0x14222477,0x12550116,0xed8e211e,0xfd8fe6f2,0xe94ef99b,0x218610a3,
0x03b90e32,0xfc1b0e57,0xeb6ddd92,0xff50fc38,0x21df07d5,0xf4a91b95,0xf68af1a2,0xed37f8cc,
0x0db007dc,0x0f94052f,0xfc701910,0x03d1ef2b,0xef20f286,0x25f60921,0x0913107d,0xef1300bd,
0x0dfff789,0x03e50706,0x194e10e8,0x07b90353,0x09cb0434,0x01d4ff13,0x193c0962,0x1bc617a6,
0x00cb12cf,0x162e0c68,0x1db10cee,0x13f92607,0x1f0101ea,0x0c1b0e4a,0x0aa9202a,0x342a181f,
0x13a429fd,0x0ae01898,0x1ed00543,0x2bc13242,0x172d23d1,0x250b2486,0x1d4b321a,0x1c17162c,
0x36b739e9,0x2c013a62,0x05bb15a2,0x36682926,0x352b3491,0x16f21bf1,0x3cf23c49,0x26d00617,
0x23fa1fb6,0x3b364fe1,0x289723ab,0x198b2353,0x3a284dc6,0x230e2e7d,0x3a8b41c2,0x1d0e44fd,
0x2ba91939,0x3b28409d,0x27973d14,0x225a1cc6,0x462d3a1d,0x1f602b50,0x393f2429,0x1d0e3b4e,
0x27300a42,0x4970331a,0x1fa439d6,0x18ee1af1,0x443c226e,0x1a1726c9,0x32b22092,0x2a9a2f21,
0x0fde01bc,0x312337d1,0x2172389f,0x0f43fe20,0x2ca50d9e,0x1d582b30,0x335a1d58,0x1b9a30c0,
0x0554fd06,0x35e31daf,0x115b3549,0x073e01c5,0x220b0372,0xfcbb214b,0x29861cfa,0x1d6d17b4,
0xf21fe8f2,0x1e2a313c,0x1bab30b6,0x06e6f275,0x114409df,0x06750e57,0x22ac016a,0xfddf26a1,
0xff33f7af,0x2e16180a,0xfe6b2667,0x001a03ab,0x2133feba,0x054008f5,0x0e1a2848,0x0a101591,
0x120ada7f,0x18ea1de5,0x03fd1d18,0x167df57c,0x0fef0ec1,0x09cc2637,0x22b62352,0xf6af09f2,
0x14f2fb64,0x315b283d,0x0c0c0962,0x01481355,0x19281de9,0x36a10699,0x23ab11c4,0xeb9a1341,
0x2d7f18bb,0x3327238f,0xfe4d090c,0x018e25ef,0x1f7e2548,0x3bda0b75,0x10bf1bd7,0xfe79127e,
0x35723750,0x193d2f05,0x1ba61785,0x245a212a,0x21901e06,0x2be02831,0x1a841a85,0x326e04be,
0x2eb33cd0,0x0de622d3,0x31ea1d00,0x31492622,0x29262938,0x24f22475,0x23eb175f,0x426637b9,
0x1f04368b,0x137314ef,0x33fa3f92,0x2fdf4ae5,0x2dfd2345,0x243b0ff1,0x3c3c319b,0x3eba549e,
0x0ff8152c,0x37500ab1,0x44fd477e,0x2c3140fb,0x09df1b93,0x2bd82cd1,0x60f74b64,0x2c3f32f7,
0x00a2f3ea,0x3ff44448,0x44b8684c,0x221428db,0x1e5fffc4,0x53033ceb,0x421065b9,0xf3131cda,
0x37c10ad5,0x67105f75,0x3f403adf,0x08aa0573,0x31db2474,0x64617e23,0x250341cf,0x24b1e5b8,
0x5a233908,0x30ba7154,0x0842382c,0x20c0124e,0x744d42f1,0x388a6284,0x10d9fd8f,0x4bcc2450,
0x4d7a6c44,0x202545cd,0x2a270adc,0x6fb11e2f,0x522c6f80,0xf1f32436,0x384e2efa,0x62ec5b9f,
0x41504481,0x15301533,0x47992158,0x5b717728,0xfd1a3a28,0x31ca1cd6,0x60d64be8,0x533b4316,
0x168f2572,0x327c129c,0x653b6d24,0x12a63f38,0x31461070,0x4e483f6d,0x52444740,0x17f6357f,
0x2a12099b,0x655357ed,0x27293b3d,0x2ec013d0,0x42cc36dd,0x48cc455b,0x1d9e3f2c,0x269014e8,
0x5d675293,0x26dc36c7,0x320b20f3,0x300b3a09,0x4193464a,0x1da83f74,0x2e9d1ce5,0x504550bd,
0x1c932fe9,0x383b21ec,0x32fe30df,0x421b3ae3,0x1526326e,0x301f1fda,0x4230578a,0x18f12330,
0x3b5f23f7,0x3272304f,0x3a6f4032,0x008c2aa7,0x3de0220b,0x393c540b,0x153319d4,0x2e1f290a,
0x307d26a7,0x3ec43afb,0x04ec1726,0x3c0c2352,0x22005563,0x1bef1718,0x329a253f,0x34a21997,
0x33973950,0x00540dc9,0x46b2239b,0x1f114755,0x1be40f7c,0x225d31d5,0x2e6e227c,0x2f343b51,
0x06be01fb,0x477b264c,0x050b4125,0x19ab15da,0x1cfa304d,0x328b1a70,0x20633813,0x085e01c3,
0x43702fa9,0x05062677,0x20e80953,0x12b12b9e,0x21c722e1,0x0e803367,0x120c0213,0x39b73320,
0xff621b07,0x1ed6107d,0x12092835,0x23f321c7,0x064f2125,0x17dd080c,0x2b783392,0x02d60cd8,
0x22961105,0x121e2572,0x179623c2,0xfbed155e,0x1b9315ae,0x1ed83027,0x0639ffea,0x27f20fc4,
0x12cb1bdd,0x138b1a5f,0xff0a0b79,0x21b21cde,0x0b613072,0xfeb802a8,0x21a01df6,0x16d31b4c,
0x0aea0e0a,0x06380b36,0x218f24f9,0x013425b1,0xff2f02a2,0x1ec921b2,0x12161f88,0x01e90b65,
0x14740bc4,0x24862933,0xf7ff151f,0x08a50178,0x20221fd0,0x0cff1f7c,0x078e042d,0x20a50894,
0x1b602145,0xf53208fc,0x0e5f0699,0x189526c2,0xfa17197f,0x06b3fa2b,0x25110e51,0x17761d04,
0xf4e7fed6,0x18ad0e09,0x1a592c34,0xf6bb0f87,0x0fb8fd4c,0x1e1c1d1e,0x0a271d3d,0xf92df87a,
0x25e30a7e,0x132b27c6,0xefde0368,0x109c0ab7,0x13732144,0xf8901bcd,0xfb21fc94,0x2eb517f5,
0x0fcb15ab,0xfb0fe972,0x169807ea,0x02152591,0xef631ef5,0xff7df97a,0x2a192475,0xff2e1316,
0x0346eeea,0x17ef1416,0x157816a0,0xf4ce0187,0x11e3f209,0x18c92de9,0xe8640f51,0x0824fb6e,
0x215c0b33,0x167d081c,0xed78eea9,0x1f36f7d2,0x11a723b1,0xf1a8ee48,0x0dd5fa17,0x07af1665,
0xfb4d1ead,0xe7b2f332,0x314c048a,0x0aa50bc7,0x0657e222,0x109dfcd3,0x10700fc8,0xeb341146,
0xe8ebe358,0x18222271,0xec7b0dbb,0xfafcf4df,0x0965010e,0x13b1042e,0xeda8f14f,0x0286e200,
0x051727e3,0xd8c50354,0xe82d084d,0x051206e0,0x06b30a31,0xedf0e6fc,0x1cd1e864,0x07060c57,
0xfb2bdf56,0xf549f334,0xfdf50bcf,0xe0e113c1,0xe0a3f4c4,0x1f1efc28,0x0375f849,0x1263d3c2,
0x05b0e618,0x07afff51,0xe147fc16,0xeb34eeb7,0x02401582,0xe285fcf4,0xf9a1f8c7,0x0373f4a8,
0x106600ca,0xf30ee4b6,0x083debd4,0xfaa01029,0xe282f9d8,0xef7f0d0d,0xfbf0021f,0xfb340b19,
0xebc7eae6,0x0e1bfea3,0xf968ffac,0xff3beafd,0xf5480221,0x0050fb5d,0xe4e10aba,0xf227ee63,
0x04131273,0xf121f892,0x028df0a8,0xfc6ff41f,0x0db4f8c4,0xedd0f01a,0x03fbee6b,0xfc4d08c8,
0xf372f51d,0xf231ff85,0xf85df482,0x002b09b3,0xf2d9ec52,0x0f9cfa38,0xf4b0facc,0xf7cff845,
0xed1d003f,0xf8b4fedf,0xe2100da3,0xe83eea6b,0xfe7c0681,0xeb91fa54,0xfcccf204,0xf9f3e703,
0x0579f0fe,0xe27bfd88,0xecf2f060,0xed9d09d8,0xf16cf6c7,0xf3f4f95f,0xfac0f34c,0x067ff3ec,
0xf2dee549,0x0632dcb8,0xfb77eb97,0xef5df694,0xe36df8b4,0xf1edfd5b,0xf67df7cf,0xf000ea5c,
0xf84dec42,0xf5d9f093,0xeed7ee6f,0xeb8ce614,0xea08fc3c,0xdecdf851,0xdc56fc8a,0xf3edf4a6,
0x0264e978,0xefd8e4c9,0xfe61d430,0xf182fcad,0xe925e827,0xe0f9f773,0xddf1f5c7,0xeabdfffe,
0xd0cced89,0x085ae45a,0xf35aef87,0xf4c1d7f1,0xeb67e267,0xe9e1e997,0xe5e5fa31,0xcc03e582,
0xf89ef402,0xdceaef09,0xef09d874,0xea9ee4f7,0xf265deb2,0xe3e1ec53,0xdefac88e,0xf9f3f2fa,
0xd9b1de4a,0xe3d5dc5d,0xdfc3e397,0xf7d3e2af,0xd44ae4f3,0xe145d042,0xef5eff65,0xdf3bdb06,
0xf2efcfac,0xeaa7d8b5,0xf03ce91d,0xc3fee245,0xebb6dcd1,0xe700f9d5,0xda62d614,0xe425df1b,
0xe90bdfef,0xf180e93d,0xdabec9be,0xf82edbfd,0xd826f654,0xd225e431,0xe693e6e4,0xf9c7e28d,
0xe6a8e517,0xd64ad306,0xf819e9fd,0xe4deece9,0xe0bcd383,0xe32ae06d,0xeea4edde,0xe2cdeb53,
0xe2a0ce95,0x0208e803,0xddffe59b,0xe426e037,0xe4c3ed34,0xf779ed01,0xe53de650,0xec74c318,
0xfd4af0d5,0xd974ef29,0xed7fde77,0xf724e6bc,0xf49ce481,0xe145eff0,0xf310ce7a,0x0dd3e7d0,
0xe407e26d,0xe431ddfc,0xe4060124,0xf517f724,0xeefbf1a2,0xf980d79d,0xfa6ff578,0xe1ffef6f,
0xe3d5ea97,0xf380f4a8,0xfacff45a,0xe23ef1e3,0xf36bea38,0x0665fe65,0xf559f090,0xed93f45b,
0xe6a00290,0xf4d4fdb8,0xea6efc4d,0xf9bedf2f,0x07adf656,0xe82a01c7,0xef0e042b,0xf3b0fe25,
0x05f1f9a8,0xf412fbbb,0xf579f02b,0x0c6dfdfe,0xfeadf4f1,0xf73bf650,0xee7ffe38,0x0cfb0813,
0x006ffdaf,0xff81e347,0x0c1bf78a,0xfc3105d1,0xf6f3f64a,0xff55f04c,0x0c55ffc4,0xf6970a76,
0xfbdaf510,0x13e8f459,0x07bd0853,0xe633fecb,0xf2de0819,0x110a14a0,0xfd2501ff,0xf78bee38,
0x00de0502,0x04be0c8b,0xee15fa75,0xf9caf057,0x117e16aa,0xf8c70ffb,0x0595e69a,0x0f46f7c5,
0x024c06cb,0xe2420b16,0xfb7df4ba,0x2544073c,0xfe0afaa2,0xf36cebd1,0x0aa70d27,0x147a091e,
0xf66bf8ee,0xf20eefd2,0x110f0f5b,0xfceb05e4,0xf5e9e39c,0x046403c5,0xf8f71763,0xfab106a5,
0xfc39ea4e,0x0bd60428,0xebba1865,0xe593ee7b,0x0e3afb9e,0x06a901a8,0xf4d2fc31,0xf0d4f063,
0x122cfac2,0xff5808c5,0xe19aec72,0xfeafff40,0x025a0b1b,0x0220f412,0xec7ee868,0xf50ffd0f,
0x01a20d73,0xf326e526,0x075be03b,0xfb6f0ae7,0xf05701c4,0xf146ed7e,0xf5ace8f9,0xfbdf07aa,
0xe3a3fccc,0xfa50df00,0xfec9fae6,0xe766fef0,0xf0bcf550,0x003fe40b,0x00e3f8ed,0xddef0498,
0xed31e504,0x0c36ee0f,0xec76f0ca,0xe09df7c1,0xed6ff154,0x01a1ec8a,0xeb59fc83,0xdc1bf0ef,
0x04cff24a,0x013fee86,0xddccf3de,0xde2cf958,0x06a5e7b1,0xfcc8e536,0xd474e964,0xe0a3ff65,
0xfd25f857,0xfbdfeb79,0xe2b9eb6b,0xf121ec26,0x0626f435,0xf3a2e62f,0xe6dce511,0xf39be93a,
0x0720e8f2,0xe5e2eb19,0xd6c5ed72,0xf8dffcd5,0xfbe6f260,0xe458e488,0xd917f1f1,0xfe49feac,
0xfe25f7ed,0xe03adc9d,0xe83fe60b,0xfc7703d9,0xeac3ef35,0xe952dc97,0xfde9e4fe,0x0635fcda,
0xdf15f03e,0xdf4fe082,0xffd7f090,0xf4b205d5,0xd6d2ec1c,0xe329ea12,0x03a102cb,0xee5d09d3,
0xd7ece4b6,0xf3dee9ef,0x107b08c1,0xe83fff88,0xd83ddede,0xf854f4c0,0x087d1276,0xe1a7edc3,
0xe32adba6,0xfdd8f6f7,0x0fb80d25,0xe375e8d7,0xe7abccf0,0x0d830324,0xf81c1398,0xe9d5d939,
0xfa64d6fb,0x0078f6f3,0xf33b1a84,0xd61fe388,0x0523cb16,0x16f50a16,0xd33d0e10,0xe7dedcb7,
0x076ce697,0x0cdfeeea,0xeb8c1917,0xd183dbc5,0x166ce130,0x10b80b36,0xd37dedb7,0xe5f2dbdd,
0xf8c8f924,0x14e4f260,0xe96effd9,0xcd01dbff,0x0a42f427,0x0a0b0390,0xd3efe141,0xee3ee9e9,
0xf67afcb6,0x0ddaf272,0xebb8f7bd,0xda13d739,0xfe4cf0fb,0x09470775,0xdd2ce1c5,0xec44e09a,
0xea64f96c,0x047cec9e,0xf0b9fdd9,0xdb85e186,0xfd65dea3,0x05040437,0xd96cf0b0,0xf25bdada,
0xf03ce6ec,0xfaf4f067,0xf749086e,0xd6abd9f1,0xee19d313,0x0c1e0ae0,0xddc9f3cd,0xe09ecf21,
0xea6de1f6,0xf9a6f5be,0xf5370319,0xd444d546,0xe826c889,0x07690ec4,0xe1750579,0xd376c9af,
0xe517e3bb,0xf1efffee,0xec8f0a21,0xc100e37c,0xdab0cf2f,0x0a640187,0xe3230535,0xcc66d5e0,
0xf298d6d6,0xfecee747,0xfb68fd83,0xc6dce94b,0xdc9fca1a,0x0734f2b5,0xe17f0eb6,0xc2c7e6e2,
0xe4c2d9db,0xeb85ed2d,0x041dffc7,0xe382f2b0,0xd3d6c79a,0x051de2ff,0x06420380,0xd87de093,
0xe78ad50d,0xef72e6bd,0xfdb4ecb9,0xf297fb02,0xd296dc81,0xeba0d3fc,0x07940502,0xe1a104cf,
0xdbffdd4c,0xebe9eaba,0xf37ffab6,0xfaaffe88,0xea64fc65,0xde36d546,0xfbade57e,0x03671065,
0xe7abe39c,0xeb04d2ac,0x0118f749,0xfc13f362,0x0391f4b0,0xeedfef16,0xdc34d9eb,0x0deaf9ed,
0xff2b0794,0xd811e701,0xf9eae701,0xfe09f802,0xf44af87d,0x07df004a,0xe3c0f78a,0xebd2dbf5,
0x156cff3f,0x03800e31,0xe985dd37,0xfbfae4ec,0x0073fc9e,0x040eef85,0x01ba03fd,0xe0adef76,
0xedc4dba3,0x1c230a2b,0xfb281279,0xdf21e794,0x038be80b,0x033c0068,0x06e70413,0x0812ff6c,
0xdf5cea42,0xf681e6dc,0x224809e6,0xf894126d,0xe175e614,0x101de5e9,0x0d24ff3b,0x04c503d4,
0x06f1fadc,0xee8aec2c,0xf648eb33,0x230d0816,0x004f155f,0xe173e70b,0x09b6e464,0x1889085d,
0x069f074e,0x03d6f9f8,0xf995ecbc,0xf3f1ef16,0x17cc0351,0x0ad51ccc,0xe0c6f88d,0xfd08e27b,
0x0e920f3d,0x05ec11f2,0xf4e7ff28,0xf6bbfa6c,0xef60f708,0x060b05d9,0x188715a4,0xe0e10a4a,
0xf06bed07,0x0d50ffb4,0x0ad91824,0x060cfcea,0xf4edf953,0xfe510135,0xfdc4f28c,0x19e50ed6,
0x09090bc9,0xe959e4bc,0x0843eef6,0x152f0d20,0x00ec0a4c,0xf1cefe70,0x008c01d8,0xf527ff62,
0x047f0a51,0x16da1061,0xe7890268,0xfdd3ef7b,0x0ba5fd1e,0x0f5b1c7b,0x066bf498,0xf8f3f60f,
0x10befa91,0xffe8f972,0x0aa30d4b,0xf7950dd9,0xe4fa0ad9,0x036c0215,0x0d130a58,0x042d0a94,
0xfbc8fa16,0x017bfb03,0x04ce071a,0x0dd9f8aa,0x034c048a,0xfec70b94,0xeff0fe92,0x00430fe7,
0x0e8d0592,0xfbea05d1,0x0a76fb5e,0x0186fd9f,0x0a0d0a8d,0x081cf8fc,0xfdfb0ae0,0x03230750,
0xf1fa0516,0x064f0a94,0x07f1fd92,0x004dff3c,0x0c95ffb4,0x03a1016e,0x012b0273,0x08b101ed,
0xfbfb0838,0x036109c0,0xff25faa8,0x0a470eb8,0x06a2f54a,0xf6480554,0x15030669,0xedce0cb1,
0x03b903f0,0x00d1fb5d,0x09f517e3,0xfe8ff6af,0x007b008c,0x0ff20361,0xfb620aa5,0xff6ffd83,
0x106c0a42,0x053c08b1,0xfb2ef86b,0x0794f7b0,0x0e4b13f1,0x0018facf,0xf5c7fe83,0x1344fe62,
0x03090d16,0x058afc9c,0x03faff2f,0x12680a09,0xf853ff27,0x04dcfc4d,0x0c830595,0x0c3d0731,
0xf76ffa43,0x067a00ca,0x05bb0a76,0x029a09a5,0x00e2f881,0x09e6047a,0x055e03ed,0x023400f7,
0x0757fdea,0x090b0191,0x05270a7d,0x05a2fead,0xfffe0107,0x0649030e,0xfc020643,0x085bfeaa,
0xfec10172,0x0a4e047f,0xfe660580,0x039ffb36,0x00c1fea3,0x0a0b0d98,0xf6c10690,0x03fffcfe,
0x0121fef5,0x082a0ae6,0xfc83ff54,0x0505ffa3,0x0673047d,0x03a506f1,0xfb06fb12,0x02e7ff4e,
0x071706fe,0x024302cb,0xff2bfdb3,0x009ffe91,0x02970335,0x0032026a,0x06ac0643,0xff61fe85,
0x045e0308,0xfd0cfc83,0x031dfed3,0x0380faeb,0x09aa0936,0xfd6102d2,0xfd16ff53,0xfb0bff77,
0x06a90afb,0xf8940416,0xfd9500ab,0x02510368,0x02a6042b,0xf7540284,0xfab6fdd8,0x09d803d8,
0x04d508d3,0x03ab03e2,0xfc75fbf7,0x02bcfb7f,0xfb7d032d,0x02b0ff17,0xfdac05b0,0x0464fc8d,
0x010b0579,0xf9dafa6b,0xfd36007d,0x02050121,0x05620a06,0xff8d0146,0xff2efad9,0xfedcfc25,
0x00e9050d,0xff31000f,0xffeefafc,0x06e9ff1e,0x00ba0775,0xfa04fee4,0xf5a2f9d2,0x00c1f770,
0x095804dc,0xfc03fe8d,0xf5edfda7,0xf77afdcf,0x07360123,0xfccfff96,0xf972fa0d,0xfddfffa0,
0x076e034a,0x01bd05e7,0xf602f6e4,0xfa13f7ac,0x014ffe99,0x050c03e7,0xfba30086,0xf80ff836,
0xff49fef1,0xfe5cfebf,0xf74ffc42,0xf3acf8ad,0xfe16faa5,0x01e200a4,0xfc270434,0xf52bfdc2,
0xf416f2c8,0xff2ff634,0x05400761,0xfce40550,0xfb68f954,0xfcc1f57a,0x014900b5,0xf96e0435,
0xf44df4d6,0xfb20f680,0xff54ffe8,0x053f02ce,0xfc0805e3,0xeda4f4a1,0xf6e7f1c1,0xfdd6f728,
0x0219fec9,0xf5ac00a5,0xf471f04b,0xff3df246,0xfa4afed8,0xf7a6fc4a,0xf839f797,0xf8dff4e2,
0xfe99fe49,0xfbd50305,0xfe360139,0xf3c8f797,0xf19fec20,0xfb45f200,0xffd30184,0xfe640524,
0xf494eee8,0xf0a1edc7,0xf6cafe9c,0xfc20f95a,0xf7b2f70c,0xf012f0ba,0xf673f727,0xfdaffa7f,
0xfc97f5ac,0xfafafdd8,0xf495f8a9,0xee94e96e,0xfceaf6c4,0x005401fa,0xf756fe5b,0xf6edef6f,
0xee35f427,0xfd01fb3f,0x0053fdd0,0xf126f8d2,0xf5d4f143,0xf3cdf95e,0xf42af387,0xfe0af609,
0xfed6036b,0xf42af7f7,0xef72f52a,0xf0a9eb79,0xf9fcf113,0x060205d9,0xf551fdf9,0xf346f276,
0xfaf0f72d,0xfd20ff4e,0xfa39fab7,0xf6faf3ce,0xf7bbf709,0xf569f97d,0xf77df4e7,0xfe9cf9ac,
0xff3302cc,0xfbb3003e,0xf07cf4c8,0xec22ea57,0x0187f43c,0x049b0272,0xf69cfc21,0xf689f36f,
0xf809f735,0xf815f69f,0xfebefcea,0xfcc1ffd9,0xf6ecf872,0xf6d8f7fc,0xf5cbf95d,0xf857f3d8,
0xffe3fbc5,0xffc702bd,0xfa32febe,0xf56ff7fa,0xf3fdf3cb,0xf758f2de,0xfcb4fc35,0xfdeafe50,
0xf824f961,0xf5e9f65a,0xf5caf4cd,0xfb6ef89e,0x022dfe91,0xf705fbb4,0xf12bf41c,0xf648f016,
0xf8b4f880,0xfcfaf8c8,0x0030fc83,0xfe5a02cc,0xf8edf9b0,0xf2e0f504,0xf2f1f364,0xeeceef2e,
0xfcf2f738,0x043b017b,0xf8ed0175,0xf243f4e9,0xf786f487,0xfb2afac7,0xfa68f6a3,0xfc9ffc0d,
0xf79ffd47,0xeb5cf060,0xecf6e966,0xfd51f590,0xfef1ff83,0x020102c4,0x02530550,0xf98a00f3,
0xed98f24a,0xeb9aebc7,0xf443f25b,0xf82df54f,0x017bfccc,0x04f402c1,0xfbcb01ed,0xf7a2f5b7,
0xf82bf46a,0xfb95fb6c,0xfdadfa2d,0x0090ff6d,0xf72dfca9,0xede5f120,0xefc6e8ce,0xfc69f46f,
0x074d03e7,0x0c9b0813,0x0cfb0c0f,0x00ac0843,0xedfef8b9,0xed9fece2,0xf430efa4,0xfa1cf7c7,
0x00a6fefb,0x0b010676,0x0a3b0bc5,0xff380617,0xfa4efe2c,0xfe20faa3,0x00deffce,0x02a103a6,
0x0005036c,0xfc3bfe12,0xf5cff6dd,0xf775f637,0xffdffb0d,0x09d30350,0x11140f2c,0x103211eb,
0x0ab10da7,0xffa304b3,0xf7a6fbc0,0xf384f3b3,0xf8dff4dd,0x022efd5d,0x09e6065d,0x0c050b19,
0x0f510e1e,0x0bce0e17,0x05bd07ac,0x024c03e5,0x018000c8,0xffe7ffcf,0xfe68ff16,0x00a1fed9,
0x0443026f,0x05ff05aa,0x066f05e7,0x097a086f,0x088709f5,0x06fc0898,0x090c07a5,0x053705fa,
0x043905dc,0x036505bd,0xff1a02a6,0x000cfe8f,0x03f7024c,0x05980527,0x06280609,0x08140669,
0x0c9b0983,0x09270a09,0x06d1079f,0x04d2058a,0x035101d9,0x032b028a,0xff1a0212,0x0087ffcf,
0x04e301cd,0x05b605b6,0x07ea07d5,0x073908d5,0x065606c8,0x05730550,0x00e90413,0x00e200b7,
0x0295012d,0x038e01ee,0x078104c4,0x06db092b,0x03dd03f1,0x05790471,0x02d10575,0x008effc4,
0x03f7022e,0x027a03a3,0xff050021,0x00be0048,0x01ac00cf,0x048b02b2,0x04130481,0x047f0413,
0x077505a5,0x033905c0,0x00e301f4,0xffa6009f,0xfc94fe36,0xfb68fb47,0xfd1efbf4,0x00c5fedc,
0x025d01e7,0x02bc02a3,0x030f02cf,0x01fe0439,0xfff40121,0x011f009b,0x005201ba,0xfedfffba,
0xfe77fedf,0xfd6dfd96,0xfccbfc6e,0xfe68fce6,0xff65fe72,0xff2aff12,0xff1afec9,0xffd6ff65,
0xff44fffc,0xfd62fec1,0xfd20fd76,0xfc6bfca9,0xfb2cfc32,0xfb32fb36,0xfc3ffbf4,0xfd9bfcf8,
0xff8dff08,0x00f800d4,0x011c00af,0x008400fe,0xff3d0002,0xfd83fe94,0xfb1afc45,0xf943f9a1,
0xf81af87a,0xf71af770,0xf8e6f87d,0xfb69f9e7,0xfd51fbea,0xfe96fe84,0xffadff66,0xff8dff39,
0xfe70ff16,0xfd51fe32,0xfcdafdd0,0xfc64fcfb,0xfbc7fc6f,0xfb0ffbfc,0xfaebfacf,0xfbedfaeb,
0xfb88fb9e,0xfb2afbcb,0xfc0cfb40,0xfbcbfbc1,0xfbfbfbf7,0xfb77fbf1,0xfb42fb4d,0xfc8afbda,
0xfd84fd3c,0xfd0cfd73,0xfc39fc54,0xfaa3fba6,0xfa13fa8a,0xfa58fadd,0xfa5afa7a,0xfaeefa77,
0xfd36fc80,0xff05fe1b,0x0004ffb1,0xfffeffad,0xffa9ff8f,0xfef5ff5f,0xfddafe66,0xfc9efd42,
0xfac0fb91,0xf90cf9e9,0xf93bf920,0xf94df94e,0xf92ef931,0xfa5ef96f,0xfc0dfb0f,0xfd47fca9,
0xfe35fdb8,0xfe57fe61,0xff27febe,0xff72ff39,0xff80ff16,0xff38ff4e,0xfe4cfead,0xfe03fdcf,
0xfe66fe06,0xfe24fe43,0xfd98fe1b,0xfcc9fd3c,0xfc6efca6,0xfc2efc45,0xfbdafc35,0xfbf7fbda,
0xfc6efc58,0xfc79fc66,0xfce4fc79,0xfd39fd27,0xfd92fd43,0xfdb6fd92,0xfde6fd9f,0xfde6fe05,
0xfdcbfde7,0xfd69fd8e,0xfd83fd9e,0xfdd9fdf9,0xfe1bfdf5,0xfe51fe27,0xfe24fe77,0xfe1bfe3f,
0xfe20fdff,0xfdaffdaf,0xfd3cfd64,0xfd38fd47,0xfd2bfd31,0xfd2ffd3c,0xfd27fd25,0xfd3bfd47,
0xfd79fd66,0xfd99fd8e,0xfda5fdb7,0xfda7fdb1,0xfdaffda9,0xfde1fdb8,0xfddcfde6,0xfdb6fdc9,
0xfd9efda0,0xfda0fda3,0xfda9fda3,0xfd98fda2,0xfd9bfd99,0xfd6cfd66,0xfd5ffd57,0xfd4afd54,
0xfd32fd4e,0xfd2efd25,0xfd31fd2a,0xfd4dfd44,0xfd38fd47,0xfd3dfd3b,0xfd4afd3c,0xfd39fd40,
0xfd43fd3d,0xfd44fd40,0xfd43fd42,0xfd39fd3c,0xfd38fd42,0xfd36fd44,0xfd28fd32,0xfd28fd23,
0xfd25fd21,0xfd12fd17,0xfd09fd10,0xfd05fd08,0xfcfbfcfe,0xfd3cfcd2,0xfc42fc0d,0xfac1fc49,
0x0000fb05,0x00000000,0x00000000,0x00000000,
};
