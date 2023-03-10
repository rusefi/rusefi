#include "gtest/gtest.h"
#include "kline.h"

static int sourceTotal = 11;

TEST(kline, source) {
    ByteSource source = [] (uint8_t * buffer, int maxSize) {
        if (sourceTotal == 0)
            return (size_t)0;
        buffer[0] = sourceTotal;
        sourceTotal--;
        return (size_t)1;
    };

    uint8_t b[16];

    // there is a bit of pointer logic there so test coverage it is!
    int actual = readWhileGives(source, b, sizeof(b));
    ASSERT_EQ(actual, 11);
    ASSERT_EQ(b[0], 11);
    ASSERT_EQ(b[7], 4);
}
