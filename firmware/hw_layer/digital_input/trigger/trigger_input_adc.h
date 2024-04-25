/**
 * @file	trigger_input_adc.h
 * @brief	Position sensor hardware layer, Using ADC and software comparator
 *
 * @date Jan 27, 2020
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */
 
#pragma once
#include "global.h"
#include "trigger_input.h"
#include "adc_inputs.h"


#define DELTA_THRESHOLD_CNT_LOW (GPT_FREQ_FAST / GPT_PERIOD_FAST / 32)		// ~1/32 second?
#define DELTA_THRESHOLD_CNT_HIGH (GPT_FREQ_FAST / GPT_PERIOD_FAST / 4)		// ~1/4 second?

class TriggerAdcDetector {
public:
	void init();
	void reset();

	void digitalCallback(efitick_t stamp, bool isPrimary, bool rise);
	void analogCallback(efitick_t stamp, triggerAdcSample_t value);

public:
	triggerAdcSample_t adcDefaultThreshold;
	triggerAdcSample_t adcMinThreshold;
	triggerAdcSample_t adcMaxThreshold;

	float triggerInputDividerCoefficient;

	float triggerAdcITermCoef = 1600.0f;
	float triggerAdcITermMin = 3.125e-8f;	// corresponds to rpm=25

	int transitionCooldown = 5;

	int analogToDigitalTransitionCnt;
	int digitalToAnalogTransitionCnt;

	triggerAdcMode_t curAdcMode = TRIGGER_ADC_NONE;
	float adcThreshold = adcDefaultThreshold;
	float triggerAdcITerm = triggerAdcITermMin;

	// these thresholds allow to switch from ADC mode (low-rpm) to EXTI mode (fast-rpm), indicating the clamping of the signal 
	triggerAdcSample_t switchingThresholdLow = 0, switchingThresholdHigh = 0;
	efidur_t minDeltaTimeForStableAdcDetectionNt;
	efidur_t stampCorrectionForAdc;
	int switchingCnt = 0, switchingTeethCnt = 0;
	int prevValue = 0;	// not set
	efitick_t prevStamp;

	// we need to distinguish between weak and strong signals because of different SNR and thresholds.
	bool isSignalWeak = true;
	int zeroThreshold = 0;

	// the 'center' of the signal is variable, so we need to adjust the thresholds.
	int minDeltaThresholdWeakSignal = 0, minDeltaThresholdStrongSignal = 0;

	// this is the number of measurements while we store the counter before we reset to 'isSignalWeak'
	int minDeltaThresholdCntPos = 0, minDeltaThresholdCntNeg = 0;
	int integralSum = 0;
	int transitionCooldownCnt = 0;

	int modeSwitchCnt = 0;
};
