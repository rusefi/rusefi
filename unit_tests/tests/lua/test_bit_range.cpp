#include "pch.h"



TEST(BitRangeTest, getBitRangeMsb) {

    uint8_t data[] = {0xAA, 0xBB, 0xCC, 0xDD};
    EXPECT_EQ(getBitRangeMsb(data, 24, 32), 0xAA'BB'CC'DDul);
    EXPECT_EQ(getBitRangeMsb(data, 24, 28), 0x0A'BB'CC'DDul);
    EXPECT_EQ(getBitRangeMsb(data, 28, 28), 0xAA'BB'CC'Dul);
}

TEST(BitRangeTest, setBitRangeMsb) {

    uint8_t data1[4] {}, data2[4] {}, data3[4] {};

    setBitRangeMsb(data1, 24, 32, 0xAA'BB'CC'DDul);
    EXPECT_THAT(data1, testing::ElementsAre(0xAA, 0xBB, 0xCC, 0xDD));

    setBitRangeMsb(data2, 24, 28, 0x0A'BB'CC'DDul);
    EXPECT_THAT(data2, testing::ElementsAre(0x0A, 0xBB, 0xCC, 0xDD));

    setBitRangeMsb(data3, 28, 28, 0xAA'BB'CC'Dul);
    EXPECT_THAT(data3, testing::ElementsAre(0xAA, 0xBB, 0xCC, 0xD0));
}


TEST(BitRangeTest, getBitRangeLsb) {

    uint8_t data[] = {0xAA, 0xBB, 0xCC, 0xDD};
    EXPECT_EQ(getBitRangeLsb(data, 0, 32), 0xDD'CC'BB'AAul);
    EXPECT_EQ(getBitRangeLsb(data, 0, 28), 0x0D'CC'BB'AAul);
    EXPECT_EQ(getBitRangeLsb(data, 4, 28), 0xDD'CC'BB'Aul);
}

TEST(BitRangeTest, setBitRangeLsb) {

    uint8_t data1[4] {}, data2[4] {}, data3[4] {};

    setBitRangeLsb(data1, 0, 32, 0xDD'CC'BB'AAul);
    EXPECT_THAT(data1, testing::ElementsAre(0xAA, 0xBB, 0xCC, 0xDD));

    setBitRangeLsb(data2, 0, 28, 0x0D'CC'BB'AAul);
    EXPECT_THAT(data2, testing::ElementsAre(0xAA, 0xBB, 0xCC, 0x0D));

    setBitRangeLsb(data3, 4, 28, 0xDD'CC'BB'Aul);
    EXPECT_THAT(data3, testing::ElementsAre(0xA0, 0xBB, 0xCC, 0xDD));
}
