#include "pch.h"

#include "engine_sniffer.h"

extern WaveChart waveChart;

static size_t countTrailingDelimiters() {
	size_t count = 0;
	while (count < waveChart.used() && waveChart.getBuffer()[waveChart.used() - 1 - count] == LOG_DELIMITER[0]) {
		count++;
	}
	return count;
}

// A published chart stays queued in logging central until free() is called by the TS thread,
// status polls in the meantime must not append the closing delimiter and queue it again
TEST(EngineSniffer, publishIfFullWhileQueued) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engine->triggerCentral.isEngineSnifferEnabled = true;
	engineConfiguration->engineChartSize = 10;

	for (int i = 0; i < 10; i++) {
		waveChart.addEvent3("r", "u");
	}
	ASSERT_TRUE(waveChart.isFull());

	waveChart.publishIfFull();
	size_t publishedLen = waveChart.used();
	EXPECT_EQ(1u, countTrailingDelimiters());

	// status loop keeps polling before the buffer is transferred
	waveChart.publishIfFull();
	waveChart.publishIfFull();

	EXPECT_EQ(1u, countTrailingDelimiters());
	EXPECT_EQ(publishedLen, waveChart.used());

	// queued buffer must not be modified either
	waveChart.addEvent3("r", "d");
	EXPECT_EQ(10, waveChart.getSize());
}
