#include "pch.h"
#include "rusefi_lua.h"
#include "lua_lib.h"

// `motorolaMagicFromDbc` function calculates data payload LST (Least Significant Bit) index from the corresponding
// `Bit start | length` values in DBC file. The following test checks that for some sample DBC file
// `motorolaMagicFromDbc` function returns values that match pictures we see if we open this DBC file with `Kvazer
// Database Editor` program.
TEST(dbcMotorola, testFromDbc) {
    ASSERT_EQ(24, motorolaMagicFromDbc(27, 4));
    ASSERT_EQ(24, motorolaMagicFromDbc(30, 7));
    ASSERT_EQ(24, motorolaMagicFromDbc(31, 8));
    ASSERT_EQ(24, motorolaMagicFromDbc(17, 10));
    ASSERT_EQ(24, motorolaMagicFromDbc(16, 9));
    ASSERT_EQ(22, motorolaMagicFromDbc(13, 8));
    ASSERT_EQ(22, motorolaMagicFromDbc(14, 9));
    ASSERT_EQ(23, motorolaMagicFromDbc(14, 8));
}

TEST(dbcMotorola, accessRange) {
    uint8_t data[] = { 0x00, 0x00, 0x03, 0xFF, 0x00, 0x00, 0x00, 0x00};

    EXPECT_EQ(getBitRangeMoto(data, 17, 10), 0x03FF);

    EXPECT_EQ(getBitRangeLsb(data, 16, 2), 3);
    EXPECT_EQ(getBitRangeLsb(data, 24, 8), 0xFF);

    setBitRangeMoto(data, 17, 10, 0x0234);
    EXPECT_THAT(data, testing::ElementsAre(0x00, 0x00, 0x02, 0x34, 0x00, 0x00, 0x00, 0x00));

    EXPECT_EQ(getBitRangeMoto(data, 17, 10), 0x0234);

    EXPECT_EQ(getBitRangeLsb(data, 16, 2), 2);
    EXPECT_EQ(getBitRangeLsb(data, 24, 8), 0x34);
}
