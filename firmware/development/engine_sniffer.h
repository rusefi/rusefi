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
 * This is the size in bytes of the text buffer of the digital chart which would be displayed
 * on the 'digital sniffer' panel
 *
 * Each event is appended as "name!msg!time!" that is
 *   strlen(name) + strlen(msg) + digits(time) + 3 bytes
 *   name: 1-4 chars ('r', 't1', 'v1', output pin short name)
 *   msg:  1-5 chars ('u'/'d', 'u_123', rpm value)
 *   time: ENGINE_SNIFFER_UNIT_US ticks, 4-5 digits for a typical chart, up to 6 digits
 * which is 10-12 bytes for a typical event and up to ~18 bytes worst case.
 *
 * With the default engineConfiguration->engineChartSize = 180 events that is
 *   180 * 12 = ~2.2k typical, 180 * 15 = ~2.7k pessimistic, 180 * 18 = ~3.2k worst case
 * plus the header, the LOG_DELIMITER and WAVE_LOGGING_TAIL_GUARD bytes of tail reserve.
 *
 * Once the buffer is nearly full isFull() cuts the chart short, see WAVE_LOGGING_TAIL_GUARD
 */
#if EFI_PROD_CODE
#define WAVE_LOGGING_SIZE 5000
#else
#define WAVE_LOGGING_SIZE 35000
#endif

/* Each event consumes up to ~15 bytes */
#define WAVE_LOGGING_TAIL_GUARD	35

/**
 * @brief	rusEfi console sniffer data buffer
 *
 * The chart is its own logging central buffer: once full it is posted as-is with
 * loggingPostBuffer() and reused only after logging central gives it back via free()
 */
class WaveChart : public LogBuffer<WAVE_LOGGING_SIZE> {
	// see LogBufferBase::free
	void free(void) override;

public:
	WaveChart();
	void init();
	void addEvent3(const char *name, const char *msg);
	void reset();
	void startDataCollection();
	void publishIfFull();
	// hands this buffer over to logging central, see WaveChart::free
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
	// buffer is posted to logging central and waits for free(), do not touch it
	volatile bool isPublished = false;
	efitick_t startTimeNt = 0;
	volatile int isInitialized = false;
};

void initWaveChart(WaveChart *chart);
void setChartSize(int newSize);

#endif /* EFI_ENGINE_SNIFFER */

