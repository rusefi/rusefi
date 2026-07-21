// Host tests for the tooth logger buffer lifecycle (ToothLoggerBufferPool):
// big-buffer ownership, free/filled queue cycling, relative timestamps,
// 5 second staleness flush, and stop/restart behavior.
//
// Note: EngineTestHelper's own EnableToothLogger() call uses the EFI_UNIT_TEST
// stub in tooth_logger.cpp (events vector) and never touches the big buffer,
// so each test is free to run its own pool instance.

#include "pch.h"

#include "tooth_logger_buffer.h"

BigBufferUser getBigBufferCurrentUser();

namespace {

bool s_toothReady = false;

void readyCallback(bool ready) {
	s_toothReady = ready;
}

composite_logger_s unswap(const composite_logger_s& raw) {
	composite_logger_s c;
	c.x = SWAP_UINT64(raw.x);
	return c;
}

} // namespace

TEST(ToothLoggerBuffer, StartStopBigBufferOwnership) {
	ToothLoggerBufferPool pool;

	EXPECT_EQ(getBigBufferCurrentUser(), BigBufferUser::None);
	ASSERT_TRUE(pool.startI());
	EXPECT_EQ(getBigBufferCurrentUser(), BigBufferUser::ToothLogger);

	// Freshly started: nothing pending
	EXPECT_FALSE(pool.hasDataI());
	EXPECT_EQ(pool.getFilled(TIME_IMMEDIATE), nullptr);

	pool.stopI();
	EXPECT_EQ(getBigBufferCurrentUser(), BigBufferUser::None);
}

TEST(ToothLoggerBuffer, StartFailsWhenBigBufferBusy) {
	BigBufferHandle other = getBigBuffer(BigBufferUser::PerfTrace);
	ASSERT_TRUE(bool(other));

	ToothLoggerBufferPool pool;
	EXPECT_FALSE(pool.startI());

	// The failed start must not have disturbed the existing user
	EXPECT_EQ(getBigBufferCurrentUser(), BigBufferUser::PerfTrace);
}

TEST(ToothLoggerBuffer, FillPostsBufferWithRelativeTimestamps) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	s_toothReady = false;
	ToothLoggerBufferPool pool{readyCallback};
	ASSERT_TRUE(pool.startI());

	composite_logger_s state{};
	state.priLevel = true;

	efitick_t base = getTimeNowNt();

	// One entry shy of full: nothing posted yet
	for (size_t i = 0; i < toothLoggerEntriesPerBuffer - 1; i++) {
		pool.appendI(state, base + US2NT(i * 100));
	}
	EXPECT_FALSE(s_toothReady);
	EXPECT_TRUE(pool.hasDataI());
	EXPECT_EQ(pool.getFilled(TIME_IMMEDIATE), nullptr);

	// The final entry fills the buffer and posts it
	pool.appendI(state, base + US2NT((toothLoggerEntriesPerBuffer - 1) * 100));
	EXPECT_TRUE(s_toothReady);

	CompositeBuffer* buf = pool.getFilled(TIME_IMMEDIATE);
	ASSERT_NE(buf, nullptr);
	EXPECT_EQ(buf->startTime.get(), base);
	EXPECT_EQ(buf->nextIdx, toothLoggerEntriesPerBuffer);

	// Timestamps are stored as microsecond offsets from startTime
	EXPECT_EQ(unswap(buf->buffer[0]).timestamp, 0u);
	EXPECT_EQ(unswap(buf->buffer[1]).timestamp, 100u);
	EXPECT_EQ(unswap(buf->buffer[toothLoggerEntriesPerBuffer - 1]).timestamp,
		(toothLoggerEntriesPerBuffer - 1) * 100u);
	EXPECT_TRUE(unswap(buf->buffer[0]).priLevel);

	// Returning the last outstanding buffer clears the ready indication
	pool.returnBufferI(buf);
	EXPECT_FALSE(s_toothReady);

	pool.stopI();
}

TEST(ToothLoggerBuffer, StaleBufferFlushedAfter5Seconds) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	s_toothReady = false;
	ToothLoggerBufferPool pool{readyCallback};
	ASSERT_TRUE(pool.startI());

	composite_logger_s state{};

	pool.appendI(state, getTimeNowNt());
	EXPECT_FALSE(s_toothReady);

	// A partially-filled buffer goes stale after 5 seconds and is posted on
	// the next append even though it is nowhere near full
	eth.moveTimeForwardSec(6);
	pool.appendI(state, getTimeNowNt());
	EXPECT_TRUE(s_toothReady);

	CompositeBuffer* buf = pool.getFilled(TIME_IMMEDIATE);
	ASSERT_NE(buf, nullptr);
	EXPECT_EQ(buf->nextIdx, 2u);
	EXPECT_EQ(unswap(buf->buffer[1]).timestamp, 6'000'000u);

	pool.returnBufferI(buf);
	pool.stopI();
}

TEST(ToothLoggerBuffer, StopWhileConsumerHoldsBufferThenRestartClean) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	s_toothReady = false;
	ToothLoggerBufferPool pool{readyCallback};
	ASSERT_TRUE(pool.startI());

	composite_logger_s state{};
	efitick_t base = getTimeNowNt();
	for (size_t i = 0; i < toothLoggerEntriesPerBuffer; i++) {
		pool.appendI(state, base + US2NT(i));
	}

	CompositeBuffer* buf = pool.getFilled(TIME_IMMEDIATE);
	ASSERT_NE(buf, nullptr);

	// Stop while the consumer still holds a buffer
	pool.stopI();
	EXPECT_EQ(getBigBufferCurrentUser(), BigBufferUser::None);

	// Returning the buffer after stop must be tolerated (queues are in reset,
	// the post is silently dropped)
	pool.returnBufferI(buf);

	// Restart: fresh state, logging works again
	ASSERT_TRUE(pool.startI());
	EXPECT_FALSE(pool.hasDataI());
	EXPECT_EQ(pool.getFilled(TIME_IMMEDIATE), nullptr);

	base = getTimeNowNt();
	for (size_t i = 0; i < toothLoggerEntriesPerBuffer; i++) {
		pool.appendI(state, base + US2NT(i));
	}
	EXPECT_NE(pool.getFilled(TIME_IMMEDIATE), nullptr);

	pool.stopI();
}

TEST(ToothLoggerBuffer, EntriesDroppedWhenAllBuffersFull) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	s_toothReady = false;
	ToothLoggerBufferPool pool{readyCallback};
	ASSERT_TRUE(pool.startI());

	composite_logger_s state{};

	// Fill every buffer in the pool with no consumer draining. Advance the
	// virtual clock along with the event timestamps: a startTime ahead of
	// "now" reads as a huge unsigned elapsed time in Timer::hasElapsedUs and
	// would trip the 5 second staleness flush on every append.
	for (size_t i = 0; i < ToothLoggerBufferPool::bufferCount * toothLoggerEntriesPerBuffer; i++) {
		eth.moveTimeForwardUs(1);
		pool.appendI(state, getTimeNowNt());
	}

	// Producers keep going: further entries are dropped, no crash, no overwrite
	eth.moveTimeForwardUs(1);
	pool.appendI(state, getTimeNowNt());
	eth.moveTimeForwardUs(1);
	pool.appendI(state, getTimeNowNt());

	// Exactly bufferCount buffers are waiting, each completely full
	size_t drained = 0;
	while (CompositeBuffer* buf = pool.getFilled(TIME_IMMEDIATE)) {
		EXPECT_EQ(buf->nextIdx, toothLoggerEntriesPerBuffer);
		pool.returnBufferI(buf);
		drained++;
	}
	EXPECT_EQ(drained, ToothLoggerBufferPool::bufferCount);

	// With buffers returned to the free list, logging resumes
	pool.appendI(state, getTimeNowNt());
	EXPECT_TRUE(pool.hasDataI());

	pool.stopI();
}

TEST(ToothLoggerBuffer, FlushCurrentTakesPartialBuffer) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	ToothLoggerBufferPool pool;
	ASSERT_TRUE(pool.startI());

	// Nothing to flush on a fresh pool
	EXPECT_EQ(pool.flushCurrentI(), nullptr);

	composite_logger_s state{};
	efitick_t base = getTimeNowNt();
	pool.appendI(state, base);
	pool.appendI(state, base + US2NT(100));
	pool.appendI(state, base + US2NT(200));

	CompositeBuffer* buf = pool.flushCurrentI();
	ASSERT_NE(buf, nullptr);
	EXPECT_EQ(buf->nextIdx, 3u);
	EXPECT_EQ(buf->startTime.get(), base);

	// The partial buffer is no longer the pool's problem
	EXPECT_FALSE(pool.hasDataI());
	EXPECT_EQ(pool.flushCurrentI(), nullptr);

	pool.returnBufferI(buf);
	pool.stopI();
}
