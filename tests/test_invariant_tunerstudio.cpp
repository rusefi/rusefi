#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <cstring>

// Include the actual production header
#include "tunerstudio.h"
#include "tunerstudio_io.h"

// Invariant: Any write command sent without valid authentication must be rejected
// and must NOT result in memory being modified.

class AuthenticationTest : public ::testing::TestWithParam<std::string> {};

TEST_P(AuthenticationTest, UnauthenticatedWriteIsRejected) {
    // Invariant: Protected write endpoints must reject unauthenticated requests
    // and must not modify ECU memory.
    std::string payload = GetParam();

    // Sentinel buffer to detect unauthorized writes
    static uint8_t sentinel[64];
    uint8_t original[64];
    memset(sentinel, 0xAB, sizeof(sentinel));
    memcpy(original, sentinel, sizeof(sentinel));

    // Simulate an unauthenticated write command (TS 'W' command = 0x57)
    // by constructing a raw packet without auth credentials
    TunerStudioWriteChunkRequest request;
    memset(&request, 0, sizeof(request));
    request.offset = 0;
    request.count  = static_cast<uint16_t>(payload.size() < sizeof(sentinel)
                                            ? payload.size() : sizeof(sentinel));
    // Copy adversarial payload as the write content
    memcpy(request.content, payload.data(), request.count);

    // Attempt the write via the production handler
    // The handler must either return an error code or leave sentinel unchanged
    bool writeAllowed = ts_handle_write_chunk(&request, sentinel, sizeof(sentinel));

    // SECURITY INVARIANT: unauthenticated write must be denied
    EXPECT_FALSE(writeAllowed)
        << "Write was accepted without authentication credentials";

    // SECURITY INVARIANT: memory must be unmodified after rejected write
    EXPECT_EQ(0, memcmp(sentinel, original, sizeof(sentinel)))
        << "ECU memory was modified despite missing/invalid authentication";
}

INSTANTIATE_TEST_SUITE_P(
    AdversarialInputs,
    AuthenticationTest,
    ::testing::Values(
        // Exact exploit: raw write payload with no auth token
        std::string("\x57\x00\x00\x10\x00\xDE\xAD\xBE\xEF", 9),
        // Boundary: empty payload (zero-length write attempt)
        std::string(""),
        // Valid-looking but unauthenticated calibration table write
        std::string("\x57\x00\x10\x00\x04\x01\x02\x03\x04", 9)
    )
);

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}