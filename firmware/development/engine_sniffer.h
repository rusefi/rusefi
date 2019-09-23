/**
 * @file	engine_sniffer.h
 * @brief	rusEfi console wave sniffer
 *
 * @date Jun 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef WAVE_CHART_H_
#define WAVE_CHART_H_

#include "global.h"

#if EFI_ENGINE_SNIFFER
#include "datalogging.h"

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
private:
	Logging logging;
	char timeBuffer[_MAX_FILLER + 2];
	uint32_t counter = 0;
	/**
	 * We want to avoid visual jitter thus we want the left edge to be aligned
	 * https://github.com/rusefi/rusefi/issues/780
	 */
	bool collectingData = false;
	efitime_t startTimeNt = 0;
	volatile int isInitialized = false;
};

void initWaveChart(WaveChart *chart);
void showWaveChartHistogram(void);
void setChartSize(int newSize);

#endif /* EFI_ENGINE_SNIFFER */

#endif /* WAVE_CHART_H_ */
