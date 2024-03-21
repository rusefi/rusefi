#include "pch.h"

#include "func_chain.h"
#include "init.h"
#include "lua_hooks.h"

struct AddOne final : public SensorConverter {
    SensorResult convert(float input) const {
        return input + 1;
    }
};

struct SubOne final : public SensorConverter {
    SensorResult convert(float input) const {
        return input - 1;
    }
};

struct Doubler final : public SensorConverter {
    SensorResult convert(float input) const {
        return input * 2;
    }
};

TEST(FunctionChain, TestSingle)
{
    FuncChain<AddOne> fc;

    {
        auto r = fc.convert(5);
        EXPECT_TRUE(r.Valid);
        EXPECT_EQ(r.Value, 6);
    }

    {
        auto r = fc.convert(10);
        EXPECT_TRUE(r.Valid);
        EXPECT_EQ(r.Value, 11);
    }
}

TEST(FunctionChain, TestDouble)
{
    // This computes fc(x) = (x + 1) * 2
    FuncChain<AddOne, Doubler> fc;

    {
        auto r = fc.convert(5);
        EXPECT_TRUE(r.Valid);
        EXPECT_EQ(r.Value, 12);
    }

    {
        auto r = fc.convert(10);
        EXPECT_TRUE(r.Valid);
        EXPECT_EQ(r.Value, 22);
    }
}

TEST(FunctionChain, TestTriple)
{
    // This computes fc(x) = ((x + 1) * 2) - 1
    FuncChain<AddOne, Doubler, SubOne> fc;

    {
        auto r = fc.convert(5);
        EXPECT_TRUE(r.Valid);
        EXPECT_EQ(r.Value, 11);
    }

    {
        auto r = fc.convert(10);
        EXPECT_TRUE(r.Valid);
        EXPECT_EQ(r.Value, 21);
    }
}

TEST(FunctionChain, TestGet)
{
    // No logic here - the test is that it compiles
    FuncChain<AddOne, Doubler, SubOne> fc;

    fc.get<AddOne>();
    fc.get<Doubler>();
    fc.get<SubOne>();
}

TEST(Sensor, OverrideValue) {
	EngineTestHelper eth(engine_type_e::HARLEY);
	// huh? i do not get this EXPECT_FALSE(Sensor::get(SensorType::Rpm).Valid);
	initOverrideSensors();

	Sensor::setMockValue(SensorType::Rpm, 1000);
	EXPECT_TRUE(Sensor::get(SensorType::Rpm).Valid);
	EXPECT_TRUE(Sensor::get(SensorType::DashOverrideRpm).Valid);

	ASSERT_DOUBLE_EQ(1000, Sensor::get(SensorType::Rpm).Value);
	ASSERT_DOUBLE_EQ(1000, Sensor::get(SensorType::DashOverrideRpm).Value);

	LuaOverrideSensor * sensor = (LuaOverrideSensor*)Sensor::getSensorOfType(SensorType::DashOverrideRpm);
	sensor->setOverrideValue(3);
	ASSERT_DOUBLE_EQ(3, Sensor::get(SensorType::DashOverrideRpm).Value);
	sensor->reset();
	ASSERT_DOUBLE_EQ(1000, Sensor::get(SensorType::DashOverrideRpm).Value);
}
