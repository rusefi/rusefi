#include "gtest/gtest.h"
#include "../../../simulator/flash_storage.h"

#include <array>
#include <filesystem>
#include <fstream>
#include <limits>

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

TEST_F(SimulatorFlash, IdenticalHighBitBytesFailVerificationOnSignedCharHosts) {
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

    // TDB coverage of https://github.com/rusefi/rusefi/pull/10190:
    // BUG: adding identical 0x80..0xff bytes fails on signed-char hosts.
    // Keep this green assertion of bad behavior until the separate fix changes
    // these expectations to unconditional success. Unsigned-char hosts are unaffected.
    EXPECT_EQ(!std::numeric_limits<char>::is_signed,
        intFlashCompare(address, buffer.data(), 129));
    EXPECT_EQ(!std::numeric_limits<char>::is_signed,
        intFlashCompare(address, buffer.data(), buffer.size()));
}
