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
