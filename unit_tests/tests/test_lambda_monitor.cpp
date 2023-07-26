#include "pch.h"

struct MockLambdaMonitor : public LambdaMonitorBase {
	bool isGood = true;
	bool isCurrentlyGood(float /*rpm*/, float /*load*/) const override {
		return isGood;
	}

	bool isRestore = false;
	bool restoreConditionsMet(float /*rpm*/, float /*load*/) const override {
		return isRestore;
	}

	float getTimeout() const override {
		// Timeout of 1 second
		return 1;
	}

	MOCK_METHOD(float, getMaxAllowedLambda, (float rpm, float load), (const, override));
};

TEST(LambdaMonitor, Response) {
	MockLambdaMonitor mlm;

	extern int timeNowUs;
	int startTime = 1e6;
	timeNowUs = startTime;

	mlm.isGood = true;
	mlm.isRestore = false;
	mlm.update(2000, 50);
	EXPECT_TRUE(mlm.lambdaCurrentlyGood);
	EXPECT_FALSE(mlm.isCut());

	// now lambda will be bad, but we don't cut yet
	mlm.isGood = false;
	mlm.update(2000, 50);
	EXPECT_FALSE(mlm.lambdaCurrentlyGood);
	EXPECT_FALSE(mlm.isCut());

	// 0.9 second later, still not cut
	timeNowUs = startTime + 0.9e6;
	mlm.update(2000, 50);
	EXPECT_FALSE(mlm.lambdaCurrentlyGood);
	EXPECT_FALSE(mlm.isCut());

	// 1.1 second later, cut!
	timeNowUs = startTime + 1.1e6;
	mlm.update(2000, 50);
	EXPECT_FALSE(mlm.lambdaCurrentlyGood);
	EXPECT_TRUE(mlm.isCut());

	// Lambda goes back to normal, but restore conditions not met so we should stay cut
	mlm.isGood = true;
	mlm.update(2000, 50);
	EXPECT_TRUE(mlm.lambdaCurrentlyGood);
	EXPECT_TRUE(mlm.isCut());

	mlm.isRestore = true;
	mlm.update(2000, 50);
	EXPECT_TRUE(mlm.lambdaCurrentlyGood);
	EXPECT_FALSE(mlm.isCut());
}
