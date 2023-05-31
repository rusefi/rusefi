/**
 * @file	engine_sniffer.h
 * @brief	rusEfi console wave sniffer
 *
 * @date Jun 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "rusefi_enums.h"

#include "datalogging.h"

void addEngineSnifferTdcEvent(int rpm);
void addEngineSnifferLogicAnalyzerEvent(int laIndex, bool isRise);
/**
 * @param wheelIndex 0 or 1
 * @triggerEventIndex index from sync point, from 0 to number of teeth in engine cycle
 */
void addEngineSnifferCrankEvent(int wheelIndex, int triggerEventIndex, bool isRise);
void addEngineSnifferVvtEvent(int vvtIndex, bool isRise);
void addEngineSnifferOutputPinEvent(NamedOutputPin *pin, bool isRise);

#if EFI_ENGINE_SNIFFER

/**
 * @brief	rusEfi console sniffer data buffer
 */
class WaveChart {
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
	// current number of events in buffer, see getSize()
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

