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
#include "loggingcentral.h"

enum class FrontDirection : uint8_t {
	UP,
	DOWN
};

void addEngineSnifferTdcEvent(int rpm);
void addEngineSnifferLogicAnalyzerEvent(int laIndex, FrontDirection frontDirection);
/**
 * @param wheelIndex 0 or 1
 * @triggerEventIndex index from sync point, from 0 to number of teeth in engine cycle
 */
void addEngineSnifferCrankEvent(int wheelIndex, int triggerEventIndex, FrontDirection frontDirection);
void addEngineSnifferVvtEvent(int vvtIndex, FrontDirection frontDirection);
void addEngineSnifferOutputPinEvent(NamedOutputPin *pin, FrontDirection frontDirection);

#if EFI_ENGINE_SNIFFER

/**
 * This is the number of events in the digital chart which would be displayed
 * on the 'digital sniffer' pane
 */
#if EFI_PROD_CODE
#define WAVE_LOGGING_SIZE 5000
#else
#define WAVE_LOGGING_SIZE 35000
#endif

/**
 * @brief	rusEfi console sniffer data buffer
 */
class WaveChart : public LogBuffer<WAVE_LOGGING_SIZE> {
	void free(void) override;

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

