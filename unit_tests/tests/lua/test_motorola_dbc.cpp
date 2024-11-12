//
// Created by kifir on 11/11/24.
//

#include "pch.h"

namespace {
    class MotorolaDbcTest : public testing::Test {
    protected:
        void SetUp() override;

        void checkRange(
            int bitStart,
            int length,
            int testValue,
            int expectedLSB,
            const std::array<uint8_t, 8>& expectedData
        );
        uint8_t* getTestDataPtr();

        std::array<uint8_t, 8> testData;
    };

    void MotorolaDbcTest::SetUp() {
        testData.fill(0x00);
    }

    void MotorolaDbcTest::checkRange(
        const int bitStart,
        const int length,
        const int testValue,
        const int expectedLSB, // Least Significant Bit
        const std::array<uint8_t, 8>& expectedData
    ) {
        EXPECT_EQ(getBitRangeMoto(getTestDataPtr(), bitStart, length), 0x00); // 0000

        ASSERT_EQ(expectedLSB, motorolaMagicFromDbc(bitStart, length));
        setBitRangeMoto(getTestDataPtr(), bitStart, length, testValue);

        EXPECT_THAT(testData, testing::ElementsAreArray(expectedData));
        EXPECT_EQ(getBitRangeMoto(getTestDataPtr(), bitStart, length), testValue);
    }

    uint8_t* MotorolaDbcTest::getTestDataPtr() {
        return testData.data();
    }

    TEST_F(MotorolaDbcTest, testNewMessage001) {
        // Picture from Kvazer Database Editor for `27|4@0+`:
        //
        //     |  Bit Positions
        //     | 7 6 5 4 3 2 1 0
        // ----+----------------
        // B 0 |
        // y 1 |
        // t 2 |
        // e 3 |        <------|
        //   4 |
        // N 5 |
        // u 6 |
        // m 7 |
        // . 8 |
        checkRange(
            /* bitStart = */ 27,
            /* length = */ 4,
            /* testValue = */ 0xB, // 1011
            /* expectedLSB = */ 24,
            //     |  Bit Positions
            //     | 7 6 5 4 3 2 1 0
            // ----+----------------
            // B 0 | 0 0 0 0 0 0 0 0
            // y 1 | 0 0 0 0 0 0 0 0
            // t 2 | 0 0 0 0 0 0 0 0
            // e 3 | 0 0 0 0 1-0-1-1
            //   4 | 0 0 0 0 0 0 0 0
            // N 5 | 0 0 0 0 0 0 0 0
            // u 6 | 0 0 0 0 0 0 0 0
            // m 7 | 0 0 0 0 0 0 0 0
            // . 8 | 0 0 0 0 0 0 0 0
            /* expectedData = */ { 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00 }
        );
    }

    TEST_F(MotorolaDbcTest, testNewMessage002) {
        // Picture from Kvazer Database Editor for `30|7@0+`:
        //
        //     |  Bit Positions
        //     | 7 6 5 4 3 2 1 0
        // ----+----------------
        // B 0 |
        // y 1 |
        // t 2 |
        // e 3 |   <-----------|
        //   4 |
        // N 5 |
        // u 6 |
        // m 7 |
        // . 8 |
        checkRange(
            /* bitStart = */ 30,
            /* length = */ 7,
            /* testValue = */ 0x59, // 0101 1001
            /* expectedLSB = */ 24,
            //     |  Bit Positions
            //     | 7 6 5 4 3 2 1 0
            // ----+----------------
            // B 0 | 0 0 0 0 0 0 0 0
            // y 1 | 0 0 0 0 0 0 0 0
            // t 2 | 0 0 0 0 0 0 0 0
            // e 3 | 0 1-0-1-1-0-0-1
            //   4 | 0 0 0 0 0 0 0 0
            // N 5 | 0 0 0 0 0 0 0 0
            // u 6 | 0 0 0 0 0 0 0 0
            // m 7 | 0 0 0 0 0 0 0 0
            // . 8 | 0 0 0 0 0 0 0 0
            /* expectedData = */ { 0x00, 0x00, 0x00, 0x59, 0x00, 0x00, 0x00, 0x00 }
        );
    }

    TEST_F(MotorolaDbcTest, testNewMessage003) {
        // Picture from Kvazer Database Editor for `31|8@0+`:
        //
        //     |  Bit Positions
        //     | 7 6 5 4 3 2 1 0
        // ----+----------------
        // B 0 |
        // y 1 |
        // t 2 |
        // e 3 | <-------------|
        //   4 |
        // N 5 |
        // u 6 |
        // m 7 |
        // . 8 |
        checkRange(
            /* bitStart = */ 31,
            /* length = */ 8,
            /* testValue = */ 0xB3, // 1011 0011
            /* expectedLSB = */ 24,
            //     |  Bit Positions
            //     | 7 6 5 4 3 2 1 0
            // ----+----------------
            // B 0 | 0 0 0 0 0 0 0 0
            // y 1 | 0 0 0 0 0 0 0 0
            // t 2 | 0 0 0 0 0 0 0 0
            // e 3 | 1-0-1-1-0-0-1-1
            //   4 | 0 0 0 0 0 0 0 0
            // N 5 | 0 0 0 0 0 0 0 0
            // u 6 | 0 0 0 0 0 0 0 0
            // m 7 | 0 0 0 0 0 0 0 0
            // . 8 | 0 0 0 0 0 0 0 0
            /* expectedData = */ { 0x00, 0x00, 0x00, 0xB3, 0x00, 0x00, 0x00, 0x00 }
        );
    }

    TEST_F(MotorolaDbcTest, testNewMessage004) {
        // Picture from Kvazer Database Editor for `17|10@0+`:
        //
        //     |  Bit Positions
        //     | 7 6 5 4 3 2 1 0
        // ----+----------------
        // B 0 |
        // y 1 |
        // t 2 |             <--
        // e 3 | --------------|
        //   4 |
        // N 5 |
        // u 6 |
        // m 7 |
        // . 8 |
        checkRange(
            /* bitStart = */ 17,
            /* length = */ 10,
            /* testValue = */ 0x2C9, // 0010 1100 1001
            /* expectedLSB = */ 24,
            //     |  Bit Positions
            //     | 7 6 5 4 3 2 1 0
            // ----+----------------
            // B 0 | 0 0 0 0 0 0 0 0
            // y 1 | 0 0 0 0 0 0 0 0
            // t 2 | 0 0 0 0 0 0 1-0-
            // e 3 |-1-1-0-0-1-0-0-1
            //   4 | 0 0 0 0 0 0 0 0
            // N 5 | 0 0 0 0 0 0 0 0
            // u 6 | 0 0 0 0 0 0 0 0
            // m 7 | 0 0 0 0 0 0 0 0
            // . 8 | 0 0 0 0 0 0 0 0
            /* expectedData = */ { 0x00, 0x00, 0x02, 0xC9, 0x00, 0x00, 0x00, 0x00 }
        );
    }

    TEST_F(MotorolaDbcTest, testNewMessage005) {
        // Picture from Kvazer Database Editor for `16|9@0+`:
        //
        //     |  Bit Positions
        //     | 7 6 5 4 3 2 1 0
        // ----+----------------
        // B 0 |
        // y 1 |
        // t 2 |               <
        // e 3 | --------------|
        //   4 |
        // N 5 |
        // u 6 |
        // m 7 |
        // . 8 |
        checkRange(
            /* bitStart = */ 16,
            /* length = */ 9,
            /* testValue = */ 0x173, // 0001 0111 0011
            /* expectedLSB = */ 24,
            //     |  Bit Positions
            //     | 7 6 5 4 3 2 1 0
            // ----+----------------
            // B 0 | 0 0 0 0 0 0 0 0
            // y 1 | 0 0 0 0 0 0 0 0
            // t 2 | 0 0 0 0 0 0 0 1-
            // e 3 |-0-1-1-1-0-0-1-1
            //   4 | 0 0 0 0 0 0 0 0
            // N 5 | 0 0 0 0 0 0 0 0
            // u 6 | 0 0 0 0 0 0 0 0
            // m 7 | 0 0 0 0 0 0 0 0
            // . 8 | 0 0 0 0 0 0 0 0
            /* expectedData = */ { 0x00, 0x00, 0x01, 0x73, 0x00, 0x00, 0x00, 0x00 }
        );
    }

    TEST_F(MotorolaDbcTest, testNewMessage006Simplified) {
        std::array<uint8_t, 8> data { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

        const int bitStart = 13;
        const int length = 8;

        // Picture from Kvazer Database Editor for `13|8@0+`:
        //
        //     |  Bit Positions
        //     | 7 6 5 4 3 2 1 0
        // ----+----------------
        // B 0 |
        // y 1 |
        // t 2 |     <----------
        // e 3 | --|
        //   4 |
        // N 5 |
        // u 6 |
        // m 7 |
        // . 8 |
        EXPECT_EQ(getBitRangeMoto(data.data(), bitStart, length), 0x00); // 0000

        const int testValue = 0xB3; // 1011 0011
        setBitRangeMoto(data.data(), bitStart, length, testValue);

        ////     |  Bit Positions
        ////     | 7 6 5 4 3 2 1 0
        //// ----+----------------
        //// B 0 | 0 0 0 0 0 0 0 0
        //// y 1 | 0 0 0 0 0 0 0 0
        //// t 2 | 0 0 1-0-1-1-0-0-
        //// e 3 |-1-1 0 0 0 0 0 0
        ////   4 | 0 0 0 0 0 0 0 0
        //// N 5 | 0 0 0 0 0 0 0 0
        //// u 6 | 0 0 0 0 0 0 0 0
        //// m 7 | 0 0 0 0 0 0 0 0
        //// . 8 | 0 0 0 0 0 0 0 0
        const std::array<uint8_t, 8> expectedData = { 0x00, 0x2C, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00 };
        EXPECT_THAT(data, testing::ElementsAreArray(expectedData));
        EXPECT_EQ(getBitRangeMoto(data.data(), bitStart, length), testValue);
    }
}
