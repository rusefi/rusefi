#include "pch.h"
#include "can_msg_tx.h"

TEST(CanTxMessage, SetIntValueLsb) {
    // We need some setup because CanTxMessage destructor tries to send the message.
    // In unit tests, txCanBuffer is used.

    // Create a message
    CanTxMessage msg(CanCategory::NBC, 0x123, 8, /*bus*/0);

    uint32_t value = 0x12345678;
    msg.setIntValueLsb(value, 0);

    EXPECT_EQ(msg[0], 0x78);
    EXPECT_EQ(msg[1], 0x56);
    EXPECT_EQ(msg[2], 0x34);
    EXPECT_EQ(msg[3], 0x12);

    value = 0xAABBCCDD;
    msg.setIntValueLsb(value, 4);

    EXPECT_EQ(msg[4], 0xDD);
    EXPECT_EQ(msg[5], 0xCC);
    EXPECT_EQ(msg[6], 0xBB);
    EXPECT_EQ(msg[7], 0xAA);
}
