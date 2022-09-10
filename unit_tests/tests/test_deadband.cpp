#include "pch.h"

#include "deadband.h"

TEST(Deadband, OutsideDeadband) {
	Deadband<5> d;

	EXPECT_TRUE(d.gt(100, 0));
	EXPECT_FALSE(d.gt(0, 100));
}

TEST(Deadband, InsideDeadband) {
	Deadband<5> d;

	// stick the state to true
	EXPECT_TRUE(d.gt(10, 0));

	// Make sure it stays there while inside the deadband
	EXPECT_TRUE(d.gt(0, 0));
	EXPECT_TRUE(d.gt(0, 1));
	EXPECT_TRUE(d.gt(0, 2));
	EXPECT_TRUE(d.gt(0, 3));
	EXPECT_TRUE(d.gt(0, 4));
	EXPECT_TRUE(d.gt(0, 4.99));

	// Flip the state - it should now stick false
	EXPECT_FALSE(d.gt(0, 5.01));

	// Make sure it stays there while inside the deadband
	EXPECT_FALSE(d.gt(0, 4.99));
	EXPECT_FALSE(d.gt(0, 1));
	EXPECT_FALSE(d.gt(0, -1));
	EXPECT_FALSE(d.gt(0, -1));
	EXPECT_FALSE(d.gt(0, -4.99));

	// Now it should flip back
	EXPECT_TRUE(d.gt(0, -5.01));
}

TEST(Hysteresis, basic) {
	Hysteresis h;

	// Below 'rising', should stay false
	EXPECT_FALSE(h.test(15, 30, 20));
	EXPECT_FALSE(h.test(25, 30, 20));

	// over 'rising', should go true
	EXPECT_TRUE(h.test(31, 30, 20));

	// drop back below 'rising', should stay true
	EXPECT_TRUE(h.test(25, 30, 20));

	// drop below 'falling', should go false
	EXPECT_FALSE(h.test(15, 30, 20));
}
