/*
 * @author Matthew Kennedy, (c) 2019
 */

#include "pch.h"

#include "resistance_func.h"

TEST(resistance, OutOfRange)
{
    ResistanceFunc f;
    f.configure(5, 10000, false);

    // Something in the middle should be valid
    {
        auto r = f.convert(2.5f);
        ASSERT_TRUE(r.Valid);
    }

    // Something near 0.05v should be valid
    {
        auto r = f.convert(0.051f);
        EXPECT_TRUE(r.Valid);
    }

    // Something just under 0.05v should be invalid
    {
        auto r = f.convert(0.049f);
        EXPECT_FALSE(r.Valid);
    }

    // Something near 0.98 * 5v should be valid
    {
        auto r = f.convert(0.97f * 5);
        EXPECT_TRUE(r.Valid);
    }

    // Something just above 0.98 * 5v should be invalid
    {
        auto r = f.convert(0.99f * 5);
        EXPECT_FALSE(r.Valid);
    }
}

TEST(resistance, InRange)
{
    ResistanceFunc f;
    f.configure(5, 10000, false);

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

TEST(resistance, LastResistanceCache)
{
    ResistanceFunc f;
    f.configure(5, 10000, false);

    // No conversion yet -> 0
    EXPECT_FLOAT_EQ(f.getLastResistance(), 0);

    // Valid conversion caches the computed resistance
    {
        auto r = f.convert(1.0f);
        EXPECT_TRUE(r.Valid);
        EXPECT_FLOAT_EQ(f.getLastResistance(), 2500);
    }

    // Failed conversion (dead short) resets the cache to 0
    {
        auto r = f.convert(0.01f);
        EXPECT_FALSE(r.Valid);
        EXPECT_FLOAT_EQ(f.getLastResistance(), 0);
    }

    // Valid again
    {
        auto r = f.convert(3.0f);
        EXPECT_TRUE(r.Valid);
        EXPECT_FLOAT_EQ(f.getLastResistance(), 15000);
    }

    // Failed conversion (open circuit) resets the cache to 0
    {
        auto r = f.convert(4.99f);
        EXPECT_FALSE(r.Valid);
        EXPECT_FLOAT_EQ(f.getLastResistance(), 0);
    }
}

TEST(resistance, PulldownMode)
{
    ResistanceFunc f;
    f.configure(5, 10000, true);

    // 4 volt -> 2500 ohms high side
    {
        auto r = f.convert(4.0f);
        EXPECT_TRUE(r.Valid);
        EXPECT_FLOAT_EQ(r.Value, 2500);
    }

    // 3 volt -> 6666.667 ohm ohms high side
    // 20k/3 gives us an exact result
    {
        auto r = f.convert(3.0f);
        EXPECT_TRUE(r.Valid);
        EXPECT_FLOAT_EQ(r.Value, 20000.0f / 3);
    }

    // 2 volt -> 15000 ohms high side
    {
        auto r = f.convert(2.0f);
        EXPECT_TRUE(r.Valid);
        EXPECT_FLOAT_EQ(r.Value, 15000);
    }

    // 1 volt -> 40000 ohms high side
    {
        auto r = f.convert(1.0f);
        EXPECT_TRUE(r.Valid);
        EXPECT_FLOAT_EQ(r.Value, 40000);
    }
}

// No battery sensor registered in the test registry -> the tracking mode
// must fall back to the fixed 5.0 V supply. NOTE: this test must run before
// TrackingBiasWithBatterySensor (google test preserves the declaration order).
TEST(resistance, TrackingBiasFallsBackWithoutBattery)
{
    ResistanceFunc f;
    f.configureTrackingBias(0.4f, 1500, false);

    // 1 V against the fallback 5.0 V supply: 1500 / (5 - 1) = 375 ohm
    auto r = f.convert(1.0f);
    EXPECT_TRUE(r.Valid);
    EXPECT_FLOAT_EQ(r.Value, 375);
}

TEST(resistance, TrackingBiasWithBatterySensor)
{
    MockSensor vbat(SensorType::BatteryVoltage);
    ASSERT_TRUE(vbat.Register());

    ResistanceFunc f;
    f.configureTrackingBias(0.4f, 1500, false);

    // 14.4 V battery -> VTRK = 5.76 V
    vbat.set(14.4f);

    // The m74_9 +5C case that used to read -30C: junction = 5.76 x 8414 / 9914 = 4.888 V
    {
        auto r = f.convert(4.888f);
        ASSERT_TRUE(r.Valid);
        EXPECT_NEAR(r.Value, 8414, 100);
    }

    // Warm engine 90C: junction = 5.76 x 230 / 1730 = 0.766 V -> ~230 ohm
    {
        auto r = f.convert(0.766f);
        ASSERT_TRUE(r.Valid);
        EXPECT_NEAR(r.Value, 230, 10);
    }

    // 12.5 V battery -> VTRK = 5.0 V, the exact same result as the fixed mode
    vbat.set(12.5f);
    {
        // junction at 25C (2796 ohm): 5.0 x 2796 / 4296 = 3.254 V
        auto r = f.convert(3.254f);
        ASSERT_TRUE(r.Valid);
        EXPECT_NEAR(r.Value, 2796, 20);
    }

    // Battery sensor invalid (0) -> fall back to 5.0 V
    vbat.invalidate();
    {
        auto r = f.convert(3.254f);
        ASSERT_TRUE(r.Valid);
        EXPECT_NEAR(r.Value, 2796, 20);
    }

    Sensor::resetRegistry();
}
