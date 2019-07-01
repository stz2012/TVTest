#include "Global.h"
#include "Keys.h"
#include "Keyset.h"

static const Keyset_t Keyset_WOWOW = {
	0x02,
	// Keys
	0x08, 0xc6, 0x3a, 0x0c, 0x76, 0xa9, 0x67, 0xf2, 0xd4,
	0x07, 0x52, 0x45, 0x21, 0xc0, 0x4e, 0xbd, 0x59, 0xde,
	"WOWOW",
};

static const Keyset_t Keyset_StarChannelHD = {
	0x03,
	// Keys
	0x04, 0x19, 0x07, 0x70, 0x12, 0x19, 0x09, 0x76, 0x07,
	0x05, 0x64, 0x36, 0x03, 0x60, 0x09, 0x46, 0x64, 0x82,
	"Star Channel HD",
};

static const Keyset_t Keyset_E2_110CS = {
	0x17,
	// Keys
	0x06, 0xd9, 0x11, 0xe0, 0x19, 0x9a, 0x58, 0x7e, 0x91,
	0x07, 0xae, 0x35, 0x76, 0x13, 0x86, 0x8c, 0xbf, 0x3f,
	"SkyPerfecTV E2 / 110CS",
};

static const Keyset_t Keyset_Safetynet = {
	0x1d,
	// Keys
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0xdb, 0xa4, 0x96, 0xac, 0xec, 0xef, 0x81, 0x28,
	"Safenet",
};

static const Keyset_t Keyset_NHK = {
	0x1e,
	// Keys
	0x02, 0x8d, 0x82, 0x06, 0xc6, 0x2e, 0xb1, 0x41, 0x0d,
	0x01, 0x15, 0xf8, 0xc5, 0xbf, 0x84, 0x0b, 0x66, 0x94,
	"NHK",
};

static const Keyset_t Keyset_Email = {
	0x20,
	// Keys
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	"Email",
};

static const Keyset_t Keyset_NHK_Email = {
	0x01,
	// Keys
	0x02, 0x8d, 0x82, 0x06, 0xc6, 0x2e, 0xb1, 0x41, 0x0d,
	0x01, 0x15, 0xf8, 0xc5, 0xbf, 0x84, 0x0b, 0x66, 0x94,
	"NHK_Email",
};

s32 BCAS::Keys::RegisterAll(void)
{
	BCAS::Keyset::Register(&Keyset_WOWOW);
	BCAS::Keyset::Register(&Keyset_StarChannelHD);
	BCAS::Keyset::Register(&Keyset_E2_110CS);
	BCAS::Keyset::Register(&Keyset_Safetynet);
	BCAS::Keyset::Register(&Keyset_NHK);
	BCAS::Keyset::Register(&Keyset_Email);
	BCAS::Keyset::Register(&Keyset_NHK_Email);

	return 0;
}
