#include "pch.h"
#include "timer.h"

TEST(util, timer) {
	EngineTestHelper eth(TEST_ENGINE);
	Timer timer;
	ASSERT_TRUE(timer.hasElapsedSec(3));
	timer.reset();
	ASSERT_FALSE(timer.hasElapsedSec(3));

	eth.moveTimeForwardSec(4);
	ASSERT_TRUE(timer.hasElapsedSec(3));
}
