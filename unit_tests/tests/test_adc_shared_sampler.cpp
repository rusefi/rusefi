#include "pch.h"
#include "adc_shared_sampler.h"

TEST(AdcSharedSampler, StartupAndCompleteBatchOnly) {
	AdcSharedSampler<2, 2> sampler;
	uint16_t samples[] = {100, 2000, 300, 4000};
	EXPECT_EQ(-1, sampler.read(0, 0, 6));
	ASSERT_TRUE(sampler.tryStart(true, 10));
	EXPECT_FALSE(sampler.tryStart(true, 11));
	EXPECT_EQ(-1, sampler.read(1, 11, 6));
	sampler.complete(samples, 12);
	EXPECT_FALSE(sampler.active());
	EXPECT_EQ(200, sampler.read(0, 12, 6));
	EXPECT_EQ(3000, sampler.read(1, 12, 6));
	EXPECT_EQ(-1, sampler.read(2, 12, 6));
	EXPECT_EQ(-1, sampler.read(0, 11, 6));
	// Repeated reads cannot keep a stalled DMA result alive.
	EXPECT_EQ(200, sampler.read(0, 17, 6));
	EXPECT_EQ(-1, sampler.read(0, 18, 6));
}

TEST(AdcSharedSampler, KnockPreemptsPartialTemperatureBatch) {
	AdcSharedSampler<2, 2> sampler;
	uint16_t samples[] = {100, 200, 100, 200};
	ASSERT_TRUE(sampler.tryStart(true, 0));
	sampler.complete(samples, 1);
	ASSERT_TRUE(sampler.tryStart(true, 2));
	// Only one DMA row was overwritten when the knock window starts.
	samples[0] = 4000;
	samples[1] = 4000;
	sampler.cancel();
	EXPECT_FALSE(sampler.active());
	// A cancelled batch cannot publish even if a completion is delivered.
	sampler.complete(samples, 3);
	EXPECT_EQ(100, sampler.read(0, 3, 6));
	EXPECT_EQ(200, sampler.read(1, 3, 6));
	EXPECT_FALSE(sampler.tryStart(false, 4));
	EXPECT_EQ(-1, sampler.read(0, 7, 6));
	// Knock has completed: the next complete temperature batch recovers.
	ASSERT_TRUE(sampler.tryStart(true, 8));
	samples[2] = 2000;
	samples[3] = 1000;
	sampler.complete(samples, 9);
	EXPECT_EQ(3000, sampler.read(0, 9, 6));
	EXPECT_EQ(2500, sampler.read(1, 9, 6));
}

TEST(AdcSharedSampler, ErrorAndLostInterruptRecovery) {
	AdcSharedSampler<1, 2> sampler;
	uint16_t samples[] = {4095, 4095};
	ASSERT_TRUE(sampler.tryStart(true, 0));
	sampler.complete(samples, 1);
	EXPECT_EQ(4095, sampler.read(0, 1, 6));
	ASSERT_TRUE(sampler.tryStart(true, 2));
	EXPECT_FALSE(sampler.timedOut(3, 2));
	EXPECT_TRUE(sampler.timedOut(4, 2));
	sampler.cancel();
	sampler.fail();
	EXPECT_EQ(-1, sampler.read(0, 4, 6));
	ASSERT_TRUE(sampler.tryStart(true, 5));
	sampler.fail();
	EXPECT_FALSE(sampler.active());
	sampler.complete(samples, 6);
	EXPECT_EQ(-1, sampler.read(0, 6, 6));
	ASSERT_TRUE(sampler.tryStart(true, 7));
	sampler.complete(samples, 8);
	EXPECT_EQ(4095, sampler.read(0, 8, 6));
}

TEST(AdcSharedSampler, SustainedKnockOwnershipNeverStartsOrRefreshesSlowBatch) {
	AdcSharedSampler<1, 2> sampler;
	uint16_t samples[] = {1234, 1234};
	ASSERT_TRUE(sampler.tryStart(true, 0));
	sampler.complete(samples, 1);
	for (int64_t now = 2; now < 100; now++) {
		EXPECT_FALSE(sampler.tryStart(false, now));
		EXPECT_EQ(now < 7 ? 1234 : -1, sampler.read(0, now, 6));
	}
}
