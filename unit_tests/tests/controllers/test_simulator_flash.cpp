#include "gtest/gtest.h"
#include "../../../simulator/flash_storage.h"

#include <array>
#include <filesystem>
#include <fstream>

class SimulatorFlash : public testing::Test {
protected:
    static constexpr flashaddr_t address = 10190;
    bool ownsFile = false;

    void TearDown() override {
        if (ownsFile) {
            std::error_code error;
            std::filesystem::remove(makeFileName(address), error);
            EXPECT_FALSE(error) << error.message();
        }
    }
};

TEST_F(SimulatorFlash, IdenticalBytesPassVerification) {
    // Exercise the actual simulator verifier with every byte value, including
    // binary config/CRC bytes that can occur even with an ASCII-only Lua script.
    std::array<char, 256> buffer;
    for (size_t i = 0; i < buffer.size(); i++) {
        buffer[i] = static_cast<char>(i);
    }

    ASSERT_FALSE(std::filesystem::exists(makeFileName(address)));
    std::ofstream flash(makeFileName(address), std::ios::binary | std::ios::trunc);
    ASSERT_TRUE(flash.is_open());
    ownsFile = true;
    flash.write(buffer.data(), buffer.size());
    flash.close();
    ASSERT_TRUE(flash.good());

    std::array<char, 256> readBack{};
    std::ifstream input(makeFileName(address), std::ios::binary);
    input.read(readBack.data(), readBack.size());
    ASSERT_EQ(static_cast<size_t>(input.gcount()), buffer.size());
    ASSERT_EQ(readBack, buffer);

    // Control: identical bytes 0x00..0x7f pass, a real mismatch fails.
    EXPECT_TRUE(intFlashCompare(address, buffer.data(), 128));
    buffer[0] = 1;
    EXPECT_FALSE(intFlashCompare(address, buffer.data(), 128));
    buffer[0] = 0;

    // Regression coverage for https://github.com/rusefi/rusefi/pull/10190:
    // identical high-bit bytes must verify regardless of plain-char signedness.
    EXPECT_TRUE(intFlashCompare(address, buffer.data(), 129));
    EXPECT_TRUE(intFlashCompare(address, buffer.data(), buffer.size()));
}
