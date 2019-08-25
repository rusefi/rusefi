/*
 * test_cpp_memory_layout.cpp
 *
 * Jun 15, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "unit_test_framework.h"

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


TEST(misc, cppPlainStructMemoryLayout) {
	TestPlainChild c;
	// validate field initializers just for fun
	ASSERT_EQ(540, c.field0);
	ASSERT_NEAR(333.33, c.field1, EPS4D);

	ASSERT_EQ(sizeof(c), sizeof(TestParent));

	int destimationInt = 1;
	// no virtual table since nothing virtual on TestPlainChild
	memcpy(&destimationInt, &c, 4);
	ASSERT_EQ(540, destimationInt);

	ASSERT_EQ(0, (int)&c.field0 - (int)&c);
}

static int valueWePointAt;

TEST(misc, cppVirtualStructMemoryLayout) {
	TestChildWithVirtual c;

	int * valuePointer = &valueWePointAt;
	int pointerSize = sizeof(valuePointer);


	// '4' in case of 32 bit target
	// '8' in case of 64 bit target
	// this '8' is totally compiler and platform dependent
	int MAGIC_VTABLE_SIZE = pointerSize;

	// validate field initializers just for fun
	ASSERT_EQ(540, c.field0);
	ASSERT_NEAR(333.33, c.field1, EPS4D);

	ASSERT_EQ(sizeof(c), sizeof(TestParent) + MAGIC_VTABLE_SIZE);


	int destimationInt = 1;
	memcpy(&destimationInt, &c, 4);
	ASSERT_NE(540, destimationInt);

	// static_cast is smart to skip the vtable, we like static_cast
	TestParent *parent = static_cast<TestParent*>(&c);
	ASSERT_EQ(0, (int)&c.field0 - (int)parent);

	ASSERT_EQ(MAGIC_VTABLE_SIZE, (int)&c.field0 - (int)&c);


}

TEST(misc, cppPlainExtraFieldsStructMemoryLayout) {
	TestPlainChildExtraFields c;

	ASSERT_EQ(sizeof(c), sizeof(TestParent) + 8);

	int destimationInt = 1;
	// parent fields go first
	memcpy(&destimationInt, &c, 4);
	ASSERT_EQ(540, destimationInt);
	ASSERT_EQ(0, (int)&c.field0 - (int)&c);
}
