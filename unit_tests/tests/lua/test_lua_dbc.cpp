#include "pch.h"
#include "rusefi_lua.h"
#include "lua_lib.h"

static int motorolaMagicFromDbc(int b, int length) {
    // https://github.com/ebroecker/canmatrix/wiki/signal-Byteorder
    // convert from lsb0 bit numbering to msb0 bit numbering (or msb0 to lsb0)
    b = b - (b % 8) + 7 - (b % 8);
    // convert from lsbit of signal data to msbit of signal data, when bit numbering is msb0
    b = b + length - 1;
    // convert from msbit of signal data to lsbit of signal data, when bit numbering is msb0
    b = b - (b % 8) + 7 - (b % 8);
    return b;
}

TEST(dbcMotorola, testFromDbc) {
    ASSERT_EQ(24, motorolaMagicFromDbc(27, 4));
    ASSERT_EQ(24, motorolaMagicFromDbc(30, 7));
    ASSERT_EQ(24, motorolaMagicFromDbc(31, 8));
    ASSERT_EQ(24, motorolaMagicFromDbc(17, 10));

    ASSERT_EQ(24, motorolaMagicFromDbc(17, 10));
}

TEST(dbcMotorola, testFromIntelIntoMotorolaDbc) {
	// todo?!
}

TEST(dbcMotorola, accessRange) {
  uint8_t data[] = { 0x00, 0x00, 0x03, 0xFF, 0x00, 0x00, 0x00, 0x00};

  EXPECT_NEAR_M3(getBitRangeMsb(data, motorolaMagicFromDbc(17, 10), 10), 0x03FF);

  EXPECT_NEAR_M3(getBitRangeLsb(data, 16, 2), 3);
  EXPECT_NEAR_M3(getBitRangeLsb(data, 24, 8), 0xFF);


}
