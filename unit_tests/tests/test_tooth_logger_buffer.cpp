// Host tests for the tooth logger buffer lifecycle (ToothLoggerBufferPool):
// big-buffer ownership, free/filled queue cycling, relative timestamps,
// 5 second staleness flush, and stop/restart behavior.
//
// Note: EngineTestHelper's own EnableToothLogger() call uses the EFI_UNIT_TEST
// stub in tooth_logger.cpp (events vector) and never touches the big buffer,
// so each test is free to run its own pool instance.

#include "pch.h"

#include "tooth_logger_buffer.h"
#include "board_overrides.h"

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

TEST(ToothLoggerBuffer, CircularModeReusesBuffers) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	s_toothReady = false;
	ToothLoggerBufferPool pool{readyCallback};
	ASSERT_TRUE(pool.startI());
	pool.setCircularModeI(true);

	composite_logger_s state{};
	efitick_t now = getTimeNowNt();

	// 1. Fill all buffers exactly.
	// Each loop fills one buffer and posts it to filledBuffers.
	for (size_t b = 0; b < ToothLoggerBufferPool::bufferCount; b++) {
		for (size_t i = 0; i < toothLoggerEntriesPerBuffer; i++) {
			pool.appendI(state, now + US2NT(b * 10000 + i));
		}
	}

	// Now freeBuffers is empty, and filledBuffers has all bufferCount buffers.
	// m_currentBuffer is nullptr because the last appendI posted it.

	// 2. Append one more entry.
	// findBufferI will see m_currentBuffer == nullptr, freeBuffers empty,
	// and since m_circularMode is true, it will fetch from filledBuffers.
	pool.appendI(state, now + US2NT(1000000));

	// m_currentBuffer should now be non-null.
	CompositeBuffer* current = pool.getCurrent();
	ASSERT_NE(current, nullptr);
	EXPECT_EQ(current->nextIdx, 1u);

	// The filled queue should now have (bufferCount - 1) buffers.
	// We can't directly check the count, but we can try to drain it.
	for (size_t i = 0; i < ToothLoggerBufferPool::bufferCount - 1; i++) {
		CompositeBuffer* b = pool.getFilled(TIME_IMMEDIATE);
		EXPECT_NE(b, nullptr);
		// None of these should be the one we are currently filling
		EXPECT_NE(b, current);
	}
	// The queue should be empty now
	EXPECT_EQ(pool.getFilled(TIME_IMMEDIATE), nullptr);

	pool.stopI();
}

TEST(ToothLoggerBuffer, NormalModeDoesNotReuseBuffers) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	s_toothReady = false;
	ToothLoggerBufferPool pool{readyCallback};
	ASSERT_TRUE(pool.startI());
	pool.setCircularModeI(false); // Normal mode

	composite_logger_s state{};
	efitick_t now = getTimeNowNt();

	// Fill all buffers
	for (size_t b = 0; b < ToothLoggerBufferPool::bufferCount; b++) {
		for (size_t i = 0; i < toothLoggerEntriesPerBuffer; i++) {
			pool.appendI(state, now + US2NT(b * 10000 + i));
		}
	}

	// Now pool is full.
	// Try to append one more entry.
	pool.appendI(state, now + US2NT(1000000));

	// In normal mode, findBufferI returns nullptr if all buffers are full.
	// appendI does nothing if findBufferI returns nullptr.
	EXPECT_EQ(pool.getCurrent(), nullptr);

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

	eth.moveTimeForwardSec(1);
	pool.appendI(state, getTimeNowNt());
	// A partially-filled buffer goes stale after 5 seconds and is posted on
	// the next append even though it is nowhere near full
	eth.moveTimeForwardSec(6);
	pool.appendI(state, getTimeNowNt());
	EXPECT_TRUE(s_toothReady);

	CompositeBuffer* buf = pool.getFilled(TIME_IMMEDIATE);
	ASSERT_NE(buf, nullptr);
	// Only one old event in first buffer, second one is still pending
	EXPECT_EQ(buf->nextIdx, 2u);
	EXPECT_EQ(unswap(buf->buffer[1]).timestamp, 1'000'000u);

	pool.returnBufferI(buf);

	// Wait another timeout
	eth.moveTimeForwardSec(6);
	pool.appendI(state, getTimeNowNt());
	EXPECT_TRUE(s_toothReady);

	// Expecting second event in this buffer
	buf = pool.getFilled(TIME_IMMEDIATE);
	ASSERT_NE(buf, nullptr);
	EXPECT_EQ(buf->nextIdx, 1u);

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

// appendI must capture the VBatt/ET/InstantMAP/TPS sensor snapshot at append
// time, per event: values changing between events must land in each entry's
// own sensorSnapshot slot instead of one flush-time reading for the buffer.
TEST(ToothLoggerBuffer, AppendSamplesSensorSnapshotPerEvent) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	ToothLoggerBufferPool pool;
	ASSERT_TRUE(pool.startI());

	composite_logger_s state{};
	efitick_t base = getTimeNowNt();

	// values chosen exactly representable at the x100 snapshot scaling
	Sensor::setMockValue(SensorType::BatteryVoltage, 12.5f);
	Sensor::setMockValue(SensorType::Clt, 80.25f);
	Sensor::setMockValue(SensorType::Tps1, 10.5f);
	engine->outputChannels.instantMAPValue = 55.5f;
	pool.appendI(state, base);

	Sensor::setMockValue(SensorType::BatteryVoltage, 13.75f);
	Sensor::setMockValue(SensorType::Clt, 95.5f);
	Sensor::setMockValue(SensorType::Tps1, 25.25f);
	engine->outputChannels.instantMAPValue = 60.0f;
	pool.appendI(state, base + US2NT(100));

	CompositeBuffer* buf = pool.flushCurrentI();
	ASSERT_NE(buf, nullptr);
	ASSERT_EQ(buf->nextIdx, 2u);

	EXPECT_FLOAT_EQ(buf->sensorSnapshot[0].vbatt, 12.5f);
	EXPECT_FLOAT_EQ(buf->sensorSnapshot[0].et, 80.25f);
	EXPECT_FLOAT_EQ(buf->sensorSnapshot[0].instantMap, 55.5f);
	EXPECT_FLOAT_EQ(buf->sensorSnapshot[0].tps, 10.5f);

	EXPECT_FLOAT_EQ(buf->sensorSnapshot[1].vbatt, 13.75f);
	EXPECT_FLOAT_EQ(buf->sensorSnapshot[1].et, 95.5f);
	EXPECT_FLOAT_EQ(buf->sensorSnapshot[1].instantMap, 60.0f);
	EXPECT_FLOAT_EQ(buf->sensorSnapshot[1].tps, 25.25f);

	pool.returnBufferI(buf);
	pool.stopI();
}

#if TOOTH_LOG_BOARD_PAYLOAD_SIZE > 0
// appendI must invoke the board payload sampler ONCE per appended event, at
// append time, storing the result in the entry's parallel-array slot; without
// a sampler installed the slot is zeroed.
TEST(ToothLoggerBuffer, AppendSamplesBoardPayloadPerEvent) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	struct SampleHookGuard {
		~SampleHookGuard() {
			custom_board_toothLogSample = std::nullopt;
		}
	} guard;

	// a static counter: each sampled event gets a distinct value
	static uint32_t s_sampleCounter;
	s_sampleCounter = 1000;
	custom_board_toothLogSample = [](void* dst) {
		uint32_t value = s_sampleCounter++;
		memcpy(dst, &value, sizeof(value));
	};

	ToothLoggerBufferPool pool;
	ASSERT_TRUE(pool.startI());

	composite_logger_s state{};
	efitick_t base = getTimeNowNt();
	for (size_t i = 0; i < toothLoggerEntriesPerBuffer; i++) {
		pool.appendI(state, base + US2NT(i * 100));
	}

	CompositeBuffer* buf = pool.getFilled(TIME_IMMEDIATE);
	ASSERT_NE(buf, nullptr);

	// one sample per event, in append order
	uint32_t value;
	memcpy(&value, buf->boardPayload[0], sizeof(value));
	EXPECT_EQ(value, 1000u);
	memcpy(&value, buf->boardPayload[toothLoggerEntriesPerBuffer - 1], sizeof(value));
	EXPECT_EQ(value, 1000u + toothLoggerEntriesPerBuffer - 1);
	EXPECT_EQ(s_sampleCounter, 1000u + toothLoggerEntriesPerBuffer);

	// without a sampler the slot is zeroed, not stale
	custom_board_toothLogSample = std::nullopt;
	pool.appendI(state, base + US2NT(toothLoggerEntriesPerBuffer * 100));
	CompositeBuffer* buf2 = pool.flushCurrentI();
	ASSERT_NE(buf2, nullptr);
	ASSERT_EQ(buf2->nextIdx, 1u);
	memcpy(&value, buf2->boardPayload[0], sizeof(value));
	EXPECT_EQ(value, 0u);

	pool.returnBufferI(buf);
	pool.stopI();
}
#endif // TOOTH_LOG_BOARD_PAYLOAD_SIZE
