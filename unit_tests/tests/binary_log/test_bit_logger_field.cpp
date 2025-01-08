//
// Created by kifir on 1/6/25.
//

#include "pch.h"

#include <array>

#include "log_field.h"

namespace {
class TestBitLoggerField : public ::testing::Test {
    protected:
        void SetUp() override;

        void checkBitLoggerField(bool expected, const char* context);
        void updateTestBit(bool value);

        std::unique_ptr<output_channels_s> m_testOutputChannels;
        std::unique_ptr<LogField> m_logField;
        std::array<char, 6> m_buffer;
    };

    void TestBitLoggerField::SetUp() {
        m_testOutputChannels = std::make_unique<output_channels_s>();

        const uint32_t testCoilStateBlockOffset = (
			reinterpret_cast<const char*>(&m_testOutputChannels->outputRequestPeriod)
				- reinterpret_cast<const char*>(m_testOutputChannels.get())
        ) - sizeof(uint32_t);
        ASSERT_EQ(
            testCoilStateBlockOffset,
            660
        ) << "`output_channels_generated.h` header was modified significantly. "
             << "Please check that coil state bits block precedes `outputRequestPeriod` field.";
        m_logField = std::make_unique<LogField>(*m_testOutputChannels, testCoilStateBlockOffset, 11, "coilState12", "");

        m_buffer.fill(0xAA);
        ASSERT_THAT(m_buffer, ::testing::ElementsAre(0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA));
    }

    void TestBitLoggerField::checkBitLoggerField(const bool expected, const char* const context) {
        EXPECT_EQ(1, m_logField->writeData(m_buffer.data(), nullptr)) << context;
        EXPECT_THAT(m_buffer, ::testing::ElementsAre((expected ? 0x01 : 0x00), 0xAA, 0xAA, 0xAA, 0xAA, 0xAA));
    }

    void TestBitLoggerField::updateTestBit(const bool value) {
        m_testOutputChannels->coilState12 = value;
    }

    TEST_F(TestBitLoggerField, checkBitSwitching) {
        checkBitLoggerField(false, "default");

        updateTestBit(true);
        checkBitLoggerField(true, "bit is on");

        updateTestBit(false);
        checkBitLoggerField(false, "bit is off");
    }
}