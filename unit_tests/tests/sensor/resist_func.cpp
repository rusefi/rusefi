/*
 * @author Matthew Kennedy, (c) 2019
 */

#include "unit_test_framework.h"
#include "resistance_func.h"

TEST(resistance, OutOfRange)
{
    ResistanceFunc f;
    f.configure(5, 10000);

    // Something in the middle should be valid
    {
        auto r = f.convert(2.5f);
        ASSERT_TRUE(r.Valid);
    }

    // Something near 0.1v should be valid
    {
        auto r = f.convert(0.11f);
        EXPECT_TRUE(r.Valid);
    }

    // Something just under 0.1v should be invalid
    {
        auto r = f.convert(0.09f);
        EXPECT_FALSE(r.Valid);
    }

    // Something near 0.95 * 5v should be valid
    {
        auto r = f.convert(0.94f * 5);
        EXPECT_TRUE(r.Valid);
    }

    // Something just above 0.95 * 5v should be invalid
    {
        auto r = f.convert(0.96f * 5);
        EXPECT_FALSE(r.Valid);
    }
}

TEST(resistance, InRange)
{
    ResistanceFunc f;
    f.configure(5, 10000);

    // 1 volt -> 2500 ohms low side
    {
        auto r = f.convert(1.0f);
        EXPECT_TRUE(r.Valid);
        EXPECT_FLOAT_EQ(r.Value, 2500);
    }

    // 2 volt -> 6666.667 ohm ohms low side
    // 20k/3 gives us an exact result
    {
        auto r = f.convert(2.0f);
        EXPECT_TRUE(r.Valid);
        EXPECT_FLOAT_EQ(r.Value, 20000.0f / 3);
    }

    // 3 volt -> 15000 ohms low side
    {
        auto r = f.convert(3.0f);
        EXPECT_TRUE(r.Valid);
        EXPECT_FLOAT_EQ(r.Value, 15000);
    }

    // 4 volt -> 40000 ohms low side
    {
        auto r = f.convert(4.0f);
        EXPECT_TRUE(r.Valid);
        EXPECT_FLOAT_EQ(r.Value, 40000);
    }
}
