#include "pch.h"
#include "rusefi_lua.h"
#include "lua_lib.h"

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

  EXPECT_NEAR_M3(getBitRangeMoto(data, 17, 10), 0x03FF);

  EXPECT_NEAR_M3(getBitRangeLsb(data, 16, 2), 3);
  EXPECT_NEAR_M3(getBitRangeLsb(data, 24, 8), 0xFF);

  // todo: setBitRangeMoto(data, 17, 10, 0x03FF);
  // todo: assert data is empty now?
}
