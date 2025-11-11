//
// Created by kifir on 11/8/24.
//

#include "pch.h"

#include "lua_lib.h"

// inspired by TEST(LuaE65, offtopicTestGetBitRangeMsb) from test_lua_e65.cpp
TEST(BitRangeMsbTest, offtopicTestGetBitRangeMsb) {
    //                 1001 1111 0100 0001 0011 0010 0010 0000 0010 0011 0011 0000 1111 1111 0100 0011
    //                 ^------------^
    uint8_t data[] = { 0x9F,     0x41,     0x32,     0x20,     0x23,     0x30,     0xFF,     0x43 };

    EXPECT_EQ(getBitRangeMsb(data, 12, 12), 0x9F4u); // 1001 1111 0100
}

// inspired by TEST(LuaE65, offtopicTestGetBitRangeMsb2) from test_lua_e65.cpp
TEST(BitRangeMsbTest, offtopicTestGetBitRangeMsb2) {
    //                 0111 0000 0000 0100 0001 1111
    //                 ^-----------------^
    uint8_t data[] = { 0x70,     0x04,     0x1F};
    EXPECT_EQ(getBitRangeMsb(data, 16, 16), 0x41Fu); // 0111 0000 0000 0100
}

// inspired by TEST(LuaE65, offtopicTestSetBitRangeMsb2) from test_lua_e65.cpp
TEST(BitRangeMsbTest, offtopicTestSetBitRangeMsb2) {
    //                    0 1000 0000 0111
    //                    v--------------v
    //                 0110 1000 0000 0111
    uint8_t data[] = { 0x68,     0x07 };
    setBitRangeMsb(data, 8, 13, 0x807); // 1000 0000 0111
    EXPECT_THAT(data, testing::ElementsAre(0x68, 0x07));

    EXPECT_EQ(getBitRangeMsb(data, 8, 13), 0x807u);
}

// inspired by TEST(LuaE65, offtopicTestSetBitRangeMsb3) from test_lua_e65.cpp
TEST(BitRangeMsbTest, offtopicTestSetBitRangeMsb3) {
    //                    0 0011 0000 0100
    //                    v--------------v
    //                 0110 1000 0000 0111
    uint8_t data[] = { 0x68,     0x07 };
    setBitRangeMsb(data, 8, 13, 0x304); // 0011 0000 0100
    EXPECT_THAT(data, testing::ElementsAre(0x63, 0x04));

    EXPECT_EQ(getBitRangeMsb(data, 8, 13), 0x304u);
}

// inspired by TEST(LuaE65, offtopicTestGetBitRangeMsb2) from test_lua_e65.cpp
TEST(BitRangeMsbTest, getBitRangeMsbTest) {
    //                 1001 1111 0000 0001 0011 0010 0010 0000 0010 0011 0110 0111 0100 0000 0000 0000
    //                                                                      ^--------------^
    uint8_t data[] = { 0x9F,     0x01,     0x32,     0x20,     0x23,     0x67,     0x40,     0x00 };

    EXPECT_EQ(getBitRangeMsb(data, 6 * 8, 13), 0x740u); // 0111 0100 0000
}

// inspired by TEST(LuaE65, setBitRangeMsbTest) from test_lua_e65.cpp
TEST(BitRangeMsbTest, setBitRangeMsbTest) {
    //                                                                      v--------------v
    //                 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
    uint8_t data[] = { 0x00,     0x00,     0x00,     0x00,     0x00,     0x00,     0x00,     0x00 };
    setBitRangeMsb(data, 6 * 8, 13, 0x740u); // 0111 0100 0000
    EXPECT_THAT(
        data,
        //                   0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0111 0100 0000 0000 0000
        testing::ElementsAre(0x00,     0x00,     0x00,     0x00,     0x00,     0x07,     0x40,     0x00)
    );

    EXPECT_EQ(getBitRangeMsb(data, 6 * 8, 13), 0x740u);
}

TEST(BitRangeMsbTest, getBitRangeMsbTest1) {
    //                0000 0000 1111 1111 0000 0000
    uint8_t data[] = {0x00,     0xFF,     0x00};

    //                0000 0000 1111 1111 0000 0000
    //                          ^-------^
    EXPECT_EQ(getBitRangeMsb(data, 8, 8), 0xFFu); // 1111 1111

    //                0000 0000 1111 1111 0000 0000
    //                        ^--------^
    EXPECT_EQ(getBitRangeMsb(data, 9, 8), 0x7Fu); // 0111 1111

    //                0000 0000 1111 1111 0000 0000
    //                           ^--------^
    EXPECT_EQ(getBitRangeMsb(data, 23, 8), 0xFEu); // 1111 1110

    //                0000 0000 1111 1111 0000 0000
    //                          ^---------^
    EXPECT_EQ(getBitRangeMsb(data, 23, 9), 0x1FEu); // 0001 1111 1110

    //                0000 0000 1111 1111 0000 0000
    //                        ^---------^
    EXPECT_EQ(getBitRangeMsb(data, 8, 9), 0x0FFu); // 0 1111 1111

    //                0000 0000 1111 1111 0000 0000
    //                        ^-----------^
    EXPECT_EQ(getBitRangeMsb(data, 23, 10), 0x1FEu); // 0001 1111 1110
}

TEST(BitRangeMsbTest, getBitRangeMsbTest2) {
    //                1111 1111 0111 1110 1111 1111
    uint8_t data[] = {0xFF,     0x7E,     0xFF};

    //                1111 1111 0111 1110 1111 1111
    //                          ^-------^
    EXPECT_EQ(getBitRangeMsb(data, 8, 8), 0x7Eu); // 0111 1110

    //                1111 1111 0111 1110 1111 1111
    //                        ^--------^
    EXPECT_EQ(getBitRangeMsb(data, 9, 8), 0xBFu); // 1011 1111

    //                1111 1111 0111 1110 1111 1111
    //                           ^--------^
    EXPECT_EQ(getBitRangeMsb(data, 23, 8), 0xFDu); // 1111 1101

    //                1111 1111 0111 1110 1111 1111
    //                          ^---------^
    EXPECT_EQ(getBitRangeMsb(data, 23, 9), 0x0FDu); // 0000 1111 1101

    //                1111 1111 0111 1110 1111 1111
    //                        ^---------^
    EXPECT_EQ(getBitRangeMsb(data, 8, 9), 0x17Eu); // 0001 0111 1110

    //                1111 1111 0111 1110 1111 1111
    //                        ^-----------^
    EXPECT_EQ(getBitRangeMsb(data, 23, 10), 0x2FDu); // 0010 1111 1101 
}
