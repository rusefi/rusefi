#include "pch.h"

#include "redundant_sensor.h"
#include "redundant_ford_tps.h"

class SensorRedundant : public ::testing::Test
{
protected:
	RedundantSensor dut;
	MockSensor m1, m2;

	SensorRedundant()
		: dut(SensorType::Tps1, SensorType::Tps1Primary, SensorType::Tps1Secondary)
		, m1(SensorType::Tps1Primary)
		, m2(SensorType::Tps1Secondary)
	{
	}

	void SetUp() override
	{
		Sensor::resetRegistry();

		// Other tests verify registry function - don't re-test it here
		ASSERT_TRUE(dut.Register());
		ASSERT_TRUE(m1.Register());
		ASSERT_TRUE(m2.Register());

		dut.configure(5.0f, false);
	}

	void TearDown() override
	{
		Sensor::resetRegistry();
	}
};

TEST_F(SensorRedundant, SetOnlyOneSensor)
{
	// Don't set any sensors - expect invalid
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
	}

	// Set one sensor
	m1.set(24.0f);

	// Should still be invalid - only one is set!
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
	}
}

TEST_F(SensorRedundant, SetTwoSensors)
{
	// Don't set any sensors - expect invalid
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
	}

	// Set one sensor
	m1.set(24.0f);
	// Set the other sensor
	m2.set(26.0f);

	// Should now be valid - and the average of the two input
	{
		auto result = dut.get();
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 25.0f);

		EXPECT_TRUE(dut.isRedundant());
	}
}

TEST_F(SensorRedundant, DifferenceNone)
{
	// Set both sensors to the same value
	m1.set(10);
	m2.set(10);

	// Expect valid, and 10 output
	{
		auto result = dut.get();
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 10.0f);
	}
}

TEST_F(SensorRedundant, DifferenceNearLimit)
{
	// Set both sensors to nearly the limit (4.998 apart)
	m1.set(7.501f);
	m2.set(12.499f);

	// Expect valid, and 10 output
	{
		auto result = dut.get();
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 10.0f);
	}
}

TEST_F(SensorRedundant, DifferenceOverLimit)
{
	// Set both sensors barely over the limit (5.002 apart)
	m1.set(7.499f);
	m2.set(12.501f);

	// Expect invalid
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
	}
}

TEST_F(SensorRedundant, DifferenceOverLimitSwapped)
{
	// Now try it the other way (m1 > m2)
	m1.set(12.501f);
	m2.set(7.499f);

	// Expect invalid
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
	}
}


class SensorRedundantIgnoreSecond : public ::testing::Test
{
protected:
	RedundantSensor dut;
	MockSensor m1, m2;

	SensorRedundantIgnoreSecond()
		: dut(SensorType::Tps1, SensorType::Tps1Primary, SensorType::Tps1Secondary)
		, m1(SensorType::Tps1Primary)
		, m2(SensorType::Tps1Secondary)
	{
	}

	void SetUp() override
	{
		Sensor::resetRegistry();

		// Other tests verify registry function - don't re-test it here
		ASSERT_TRUE(dut.Register());
		ASSERT_TRUE(m1.Register());
		ASSERT_TRUE(m2.Register());

		dut.configure(5.0f, true);
	}

	void TearDown() override
	{
		Sensor::resetRegistry();
	}
};

TEST_F(SensorRedundantIgnoreSecond, OnlyFirst)
{
	// Don't set any sensors - expect invalid
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
	}

	// Set one sensor
	m1.set(44.0f);

	// Should be valid - we don't care about second sensor
	{
		auto result = dut.get();
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 44.0f);

		EXPECT_FALSE(dut.isRedundant());
	}
}

TEST_F(SensorRedundantIgnoreSecond, OnlySecond)
{
	// Don't set any sensors - expect invalid
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
	}

	// Set second sensor only
	m2.set(66.0f);

	// Should be invalid - should ignore second sensor
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
	}
}

TEST_F(SensorRedundantIgnoreSecond, SetBothIgnoreSecond)
{
	// Don't set any sensors - expect invalid
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
	}

	// Set both sensors
	m1.set(74.0f);
	m2.set(76.0f);

	// Should be valid, but only get the value from m1
	{
		auto result = dut.get();
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 74.0f);
	}
}

class SensorFordRedundantTps : public ::testing::Test
{
protected:
	RedundantFordTps dut;
	MockSensor m1, m2;

	SensorFordRedundantTps()
		: dut(SensorType::Tps1, SensorType::Tps1Primary, SensorType::Tps1Secondary)
		, m1(SensorType::Tps1Primary)
		, m2(SensorType::Tps1Secondary)
	{
	}

	void SetUp() override
	{
		Sensor::resetRegistry();

		// Other tests verify registry function - don't re-test it here
		ASSERT_TRUE(dut.Register());
		ASSERT_TRUE(m1.Register());
		ASSERT_TRUE(m2.Register());

		dut.configure(5.0f, 50);
	}

	void TearDown() override
	{
		Sensor::resetRegistry();
	}
};

TEST_F(SensorFordRedundantTps, SetOnlyOneSensor)
{
	// Don't set any sensors - expect invalid
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
	}

	// Set one sensor
	m1.set(24.0f);

	// Should still be invalid - only one is set!
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
	}
}

TEST_F(SensorFordRedundantTps, SetTwoSensors)
{
	// Don't set any sensors - expect invalid
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
	}

	// Set one sensor
	m1.set(12.0f);
	// Set the other sensor at double the first
	m2.set(28.0f);

	// Should now be valid - and the average of the two input
	{
		auto result = dut.get();
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 13.0f);

		EXPECT_TRUE(dut.isRedundant());
	}
}

TEST_F(SensorFordRedundantTps, DifferenceNone)
{
	// Set both sensors to the same value
	m1.set(10);
	m2.set(20);

	// Expect valid, and 10 output
	{
		auto result = dut.get();
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 10.0f);
	}
}

TEST_F(SensorFordRedundantTps, DifferenceNearLimit)
{
	// Set both sensors to nearly the limit (4.998 apart)
	m1.set(7.501f);
	m2.set(2 * 12.499f);

	// Expect valid, and 10 output
	{
		auto result = dut.get();
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 10.0f);
	}
}

TEST_F(SensorFordRedundantTps, DifferenceOverLimit)
{
	// Set both sensors barely over the limit (5.002 apart)
	m1.set(7.499f);
	m2.set(2 * 12.501f);

	// Expect invalid
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
	}
}

TEST_F(SensorFordRedundantTps, DifferenceOverLimitSwapped)
{
	// Now try it the other way (m1 > m2)
	m1.set(12.501f);
	m2.set(2 * 7.499f);

	// Expect invalid
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
	}
}

TEST_F(SensorFordRedundantTps, HighRange)
{
	// Set the throttle like it's at 75%
	m1.set(75);
	m2.set(100);

	// expect valid, at 75%
	{
		auto result = dut.get();
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 75.0f);
	}
}
