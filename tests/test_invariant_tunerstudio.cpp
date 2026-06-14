#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <cstring>

// Include the actual production header
#include "firmware/console/binary/tunerstudio.h"

// Security invariant: any write/read command issued without valid authentication
// must be rejected. The protocol must not allow unauthenticated memcpy into ECU config.

class UnauthenticatedRequestTest : public ::testing::TestWithParam<std::string> {};

TEST_P(UnauthenticatedRequestTest, RejectsUnauthenticatedWriteCommand) {
    // Invariant: write commands without valid auth credentials must be rejected
    // (return error / non-OK status), never silently applied to ECU memory.
    std::string payload = GetParam();

    TunerStudioState ts;
    memset(&ts, 0, sizeof(ts));

    // Simulate an unauthenticated write command packet
    ts_channel_s channel;
    memset(&channel, 0, sizeof(channel));

    // Feed raw unauthenticated payload into the protocol handler
    int result = tunerStudioHandleCommand(&ts, &channel,
                                          reinterpret_cast<const uint8_t*>(payload.data()),
                                          static_cast<int>(payload.size()));

    // The handler MUST reject unauthenticated requests.
    // A secure implementation returns a non-success / error code.
    // Accepting the write (result == TS_OK or 0 meaning "applied") is the vulnerability.
    EXPECT_NE(result, TS_OK)
        << "SECURITY VIOLATION: unauthenticated write was accepted for payload: "
        << payload;
}

INSTANTIATE_TEST_SUITE_P(
    AdversarialInputs,
    UnauthenticatedRequestTest,
    ::testing::Values(
        // Exact exploit: raw write command 'W' with no auth token, targeting offset 0
        std::string("\x57\x00\x00\x00\x04\xDE\xAD\xBE\xEF", 9),
        // Boundary: minimal 1-byte write with no credentials
        std::string("\x57\x00\x00\x00\x01\xFF", 6),
        // Malformed/empty token followed by write opcode
        std::string("\x00\x00\x57\x00\x00\x00\x01\xAA", 8),
        // Valid-looking packet but missing auth header entirely
        std::string("\x57\x00\x10\x00\x08AAAAAAAA", 12)
    )
);

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}