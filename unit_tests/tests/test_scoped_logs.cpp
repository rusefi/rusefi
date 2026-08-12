#include "pch.h"
#include "engine_test_helper.h"

TEST(Framework, ScopedUnitTestCreateLogs) {
    bool originalValue = getUnitTestCreateLogs();

    {
        ScopedUnitTestCreateLogs scoped(!originalValue);
        EXPECT_EQ(getUnitTestCreateLogs(), !originalValue);
    }

    EXPECT_EQ(getUnitTestCreateLogs(), originalValue);

    setUnitTestCreateLogs(true);
    {
        ScopedUnitTestCreateLogs scoped(false);
        EXPECT_EQ(getUnitTestCreateLogs(), false);
    }
    EXPECT_EQ(getUnitTestCreateLogs(), true);

    setUnitTestCreateLogs(false);
    {
        ScopedUnitTestCreateLogs scoped(true);
        EXPECT_EQ(getUnitTestCreateLogs(), true);
    }
    EXPECT_EQ(getUnitTestCreateLogs(), false);

    // Restore original value
    setUnitTestCreateLogs(originalValue);
}
