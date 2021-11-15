/**
 * @file	engine_sniffer.h
 * @brief	rusEfi console wave sniffer
 *
 * @date Jun 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"

#if EFI_ENGINE_SNIFFER
#include "datalogging.h"

/**
 * @brief	rusEfi console sniffer data buffer
 */
class WaveChart : public EnginePtr {
public:
	WaveChart();
	void init();
	void addEvent3(const char *name, const char *msg);
	void reset();
	void startDataCollection();
	void publishIfFull();
	void publish();
	bool isFull() const;
	bool isStartedTooLongAgo() const;
	// looks like this is only used by functional tests on real hardware
	efitick_t pauseEngineSnifferUntilNt = 0;
	int getSize();

private:
	Logging logging;
	char timeBuffer[_MAX_FILLER + 2];
	uint32_t counter = 0;
	/**
	 * We want to avoid visual jitter thus we want the left edge to be aligned
	 * https://github.com/rusefi/rusefi/issues/780
	 */
	bool collectingData = false;
	efitick_t startTimeNt = 0;
	volatile int isInitialized = false;
};

void initWaveChart(WaveChart *chart);
void setChartSize(int newSize);

#endif /* EFI_ENGINE_SNIFFER */

