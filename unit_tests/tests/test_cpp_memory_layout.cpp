/*
 * test_cpp_memory_layout.cpp
 *
 * Jun 15, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

// in C++ struct is pretty much a class just fields are public by default
struct TestParent {
	int field0 = 540;
	float field1 = 333.33;
};

class TestPlainChild: public TestParent {
public:
	float getSum();
};

class TestPlainChildExtraFields: public TestParent {
public:
	float field3 = 35555;
	float field4 = 45555;
};


float TestPlainChild::getSum() {
	return field0 + field1;
}

class TestChildWithVirtual: public TestParent {
public:
	virtual float getSumVirtual() {
		return field0 + field1;
	}
};


TEST(CppMemoryLayout, PlainStruct) {
	TestPlainChild c;
	// validate field initializers just for fun
	ASSERT_EQ(540, c.field0);
	ASSERT_NEAR(333.33, c.field1, EPS4D);

	ASSERT_EQ(sizeof(c), sizeof(TestParent));

	int destimationInt = 1;
	// no virtual table since nothing virtual on TestPlainChild
	memcpy(&destimationInt, &c, 4);
	ASSERT_EQ(540, destimationInt);

	ASSERT_EQ((uintptr_t)&c.field0, (uintptr_t)&c);
}

static int valueWePointAt;

TEST(CppMemoryLayout, VirtualStruct) {
	TestChildWithVirtual c;

	// '4' in case of 32 bit target
	// '8' in case of 64 bit target
	int MAGIC_VTABLE_SIZE = sizeof(void*);

	// validate field initializers just for fun
	ASSERT_EQ(540, c.field0);
	ASSERT_NEAR(333.33, c.field1, EPS4D);

	ASSERT_EQ(sizeof(c), sizeof(TestParent) + MAGIC_VTABLE_SIZE);


	int destimationInt = 1;
	memcpy(&destimationInt, &c, 4);
	ASSERT_NE(540, destimationInt);

	// static_cast is smart to skip the vtable, we like static_cast
	TestParent *parent = static_cast<TestParent*>(&c);
	ASSERT_EQ((uintptr_t)&c.field0, (uintptr_t)parent);

	ASSERT_EQ(MAGIC_VTABLE_SIZE, (uintptr_t)&c.field0 - (uintptr_t)&c);


}

TEST(CppMemoryLayout, PlainExtraFieldsStruct) {
	TestPlainChildExtraFields c;

	ASSERT_EQ(sizeof(c), sizeof(TestParent) + 8);

	int destimationInt = 1;
	// parent fields go first
	memcpy(&destimationInt, &c, 4);
	ASSERT_EQ(540, destimationInt);
	ASSERT_EQ(0, (uintptr_t)&c.field0 - (uintptr_t)&c);
}

TEST(CppMemoryLayout, structSize) {
	ASSERT_EQ(1, sizeof(adc_channel_e)) << "adc_channel_e enum size";
	ASSERT_EQ(1, sizeof(pin_input_mode_e)) << "pin_input_mode_e enum size";
	ASSERT_EQ(1, sizeof(pin_output_mode_e)) << "pin_output_mode_e enum size";
	ASSERT_EQ(2, sizeof(brain_pin_e)) << "brain_pin_e enum size";
	ASSERT_EQ(12, sizeof(air_pressure_sensor_config_s));
	ASSERT_EQ(TOTAL_CONFIG_SIZE, sizeof(persistent_config_s));
}
