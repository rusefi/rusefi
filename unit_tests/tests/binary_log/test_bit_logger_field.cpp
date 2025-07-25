//
// Created by kifir on 1/6/25.
//

#include "pch.h"

#include <array>

#include "mlg_field.h"

namespace {
class BitLoggerFieldTest : public ::testing::Test {
        static constexpr uint8_t COIL_STATE_12_BIT_NUMBER = 11;
    protected:
        void SetUp() override;

        void checkBitLoggerField(bool expected, const char* context);
        void updateTestBit(bool value);

        std::unique_ptr<output_channels_s> m_testOutputChannels;
        std::unique_ptr<MLG::Entries::Field> m_logField;
        std::array<char, 6> m_buffer;
    };

    void BitLoggerFieldTest::SetUp() {
        m_testOutputChannels = std::make_unique<output_channels_s>();

        const uint32_t testCoilStateBlockOffset = (
			reinterpret_cast<const char*>(&m_testOutputChannels->outputRequestPeriod)
				- reinterpret_cast<const char*>(m_testOutputChannels.get())
        ) - sizeof(uint32_t);
        m_logField = std::make_unique<MLG::Entries::Field>(
            *m_testOutputChannels,
            testCoilStateBlockOffset,
            COIL_STATE_12_BIT_NUMBER,
            "coilState12",
            ""
        );

        m_buffer.fill(0xAA);
        ASSERT_THAT(m_buffer, ::testing::ElementsAre(0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA));
    }

    void BitLoggerFieldTest::checkBitLoggerField(const bool expected, const char* const context) {
        EXPECT_EQ(1u, m_logField->writeData(m_buffer.data(), nullptr)) << context;
        EXPECT_THAT(m_buffer, ::testing::ElementsAre((expected ? 0x01 : 0x00), 0xAA, 0xAA, 0xAA, 0xAA, 0xAA));
    }

    void BitLoggerFieldTest::updateTestBit(const bool value) {
        m_testOutputChannels->coilState12 = value;
    }

    // If the following test fails, at first please check that in `output_channels_generated.h` header:
    //  1. bits block with `coilState12` bit precedes `outputRequestPeriod` field
    //  2. `coilState12` bit number equals `BitLoggerFieldTest::COIL_STATE_12_BIT_NUMBER` constant
    TEST_F(BitLoggerFieldTest, checkBitSwitching) {
        checkBitLoggerField(false, "default");

        updateTestBit(true);
        checkBitLoggerField(true, "bit is on");

        updateTestBit(false);
        checkBitLoggerField(false, "bit is off");
    }
}
