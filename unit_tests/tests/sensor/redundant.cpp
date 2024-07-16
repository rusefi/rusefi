#include "pch.h"

#include "redundant_sensor.h"

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

TEST_F(SensorRedundant, CheckIsRedundant)
{
	// Expect isRedundant
	{
		EXPECT_TRUE(dut.isRedundant());
	}
}

TEST_F(SensorRedundant, SetOnlyOneSensor)
{
	// Don't set any sensors - expect invalid
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
		EXPECT_EQ(result.Code, UnexpectedCode::Inconsistent);
	}

	// Set one sensor
	m1.set(24.0f);

	// Should still be invalid - only one is set!
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
	}

	// Set the other sensor
	m2.set(26.0f);

	// Should now be valid - and the average of both sensors
	{
		auto result = dut.get();
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 25.0f);
	}
}

TEST_F(SensorRedundant, CheckOnlySecondInvalid)
{
	// Set second sensor only
	m2.set(66.0f);

	// Should be invalid - only one is set!
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
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

TEST_F(SensorRedundantIgnoreSecond, CheckIsRedundant)
{
	// Expect not isRedundant
	{
		EXPECT_FALSE(dut.isRedundant());
	}
}

TEST_F(SensorRedundantIgnoreSecond, OnlyFirst)
{
	// Don't set any sensors - expect invalid
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
		EXPECT_EQ(result.Code, UnexpectedCode::Unknown);
	}

	// Set first sensor
	m1.set(44.0f);

	// Should be valid - we don't care about second sensor
	{
		auto result = dut.get();
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 44.0f);
	}

	// Set the second sensor too
	m2.set(46.0f);

	// Should be valid, but only get the value from m1
	{
		auto result = dut.get();
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 44.0f);
	}
}

TEST_F(SensorRedundantIgnoreSecond, CheckOnlySecondInvalid)
{
	// Set second sensor only
	m2.set(66.0f);

	// Should be invalid - should ignore second sensor
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
	}
}

class SensorRedundantPartialSecond : public ::testing::Test
{
protected:
	RedundantSensor dut;
	MockSensor m1, m2;

	SensorRedundantPartialSecond()
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

		dut.configure(5.0f, false, 50);
	}

	void TearDown() override
	{
		Sensor::resetRegistry();
	}
};

TEST_F(SensorRedundantPartialSecond, CheckIsRedundant)
{
	{
		EXPECT_TRUE(dut.isRedundant());
	}
}

TEST_F(SensorRedundantPartialSecond, SetNone)
{
	// Don't set any sensors - expect invalid
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
		EXPECT_EQ(result.Code, UnexpectedCode::Inconsistent);
	}
}

TEST_F(SensorRedundantPartialSecond, SetOnlyOneSensor)
{
	// Set first sensor
	m1.set(24.0f);

	// Should still be invalid - only one is set!
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
	}
}

TEST_F(SensorRedundantPartialSecond, SetTwoSensors)
{
	// Set first sensor
	m1.set(0.0f);
	// Set second sensor
	m2.set(0.0f);

	// Should now be valid, and output the primary
	{
		auto result = dut.get();
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 0.0f);
	}
}

TEST_F(SensorRedundantPartialSecond, CheckOnlySecondInvalid)
{
	// Set second sensor only
	m2.set(66.0f);

	// Should be invalid - only one is set!
	{
		auto result = dut.get();
		EXPECT_FALSE(result.Valid);
	}
}

TEST_F(SensorRedundantPartialSecond, DifferenceNone)
{
	// Set both sensors to the same value
	m1.set(10);
	m2.set(20);

	// Expect valid, and output the primary
	{
		auto result = dut.get();
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 10.0f);
	}
}

TEST_F(SensorRedundantPartialSecond, DifferenceNearLimit)
{
	// Set both sensors to nearly the limit (4.998 apart)
	m1.set(7.501f);
	m2.set(2 * 12.499f);

	// Expect valid, and output the primary
	{
		auto result = dut.get();
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 7.501f);
	}
}

TEST_F(SensorRedundantPartialSecond, DifferenceOverLimit)
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

TEST_F(SensorRedundantPartialSecond, DifferenceOverLimitSwapped)
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

TEST_F(SensorRedundantPartialSecond, PartialRedundancyRange)
{
	// Set the value like it's at 75%
	m1.set(75);
	m2.set(100);

	// Expect valid, and output the first
	{
		auto result = dut.get();
		EXPECT_TRUE(result.Valid);
		EXPECT_FLOAT_EQ(result.Value, 75.0f);
	}
}
