#include "pch.h"
#include "i2c_bb.h"

class BitbangI2cTest : public BitbangI2c {
    bool sda_get() override;
};

bool BitbangI2cTest::sda_get() {
    return true; // Always NACK
}

TEST(I2cBitbang, TestWriteNack) {
    BitbangI2cTest bb;
    bb.init(Gpio::A0, Gpio::A1, I2C_SPEED_100K);

    uint8_t data[] = {0x01, 0x02};
    msg_t res = bb.write(0x10, data, sizeof(data));

    // Should return MSG_RESET because palReadPad returns true (NACK)
    EXPECT_EQ(res, MSG_RESET);
}
