/**
 * @file	engine_sniffer.h
 * @brief	Dev console wave sniffer
 *
 * @date Jun 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef WAVE_CHART_H_
#define WAVE_CHART_H_

#include "global.h"

#if EFI_ENGINE_SNIFFER || defined(__DOXYGEN__)
#include "datalogging.h"

/**
 * @brief	Dev console sniffer data buffer
 */
class WaveChart {
public:
	WaveChart();
	void init();
	void publishChart();
	void resetWaveChart();
	bool_t isWaveChartFull();
	bool_t isStartedTooLongAgo();
	void publishChartIfFull();
	void addWaveChartEvent3(const char *name, const char *msg);
private:
	Logging logging;
	uint32_t counter;
	efitime_t startTimeNt;
	volatile int isInitialized;
};

void initWaveChart(WaveChart *chart);
void showWaveChartHistogram(void);
void setChartSize(int newSize);

#endif /* EFI_ENGINE_SNIFFER */

#endif /* WAVE_CHART_H_ */
