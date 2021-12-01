/**
 * @file	trigger_input_adc.cpp
 * @brief	Position sensor hardware layer, Using ADC and software comparator
 *
 * @date Jan 27, 2020
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */
 
#include "pch.h"
#include "trigger_input_adc.h"

#define voltsToAdcDivided(volts) (voltsToAdc(volts) / engineConfiguration->analogInputDividerCoefficient)


/*static*/ TriggerAdcDetector trigAdcState;

#define DELTA_THRESHOLD_CNT_LOW (GPT_FREQ_FAST / GPT_PERIOD_FAST / 32)		// ~1/32 second?
#define DELTA_THRESHOLD_CNT_HIGH (GPT_FREQ_FAST / GPT_PERIOD_FAST / 4)		// ~1/4 second?

// hardware-dependent part
#if (EFI_SHAFT_POSITION_INPUT && HAL_TRIGGER_USE_ADC && HAL_USE_ADC) || defined(__DOXYGEN__)

#include "digital_input_exti.h"

//!!!!!!!!!!
#define TRIGGER_ADC_DEBUG_LED TRUE

#ifdef TRIGGER_ADC_DEBUG_LED
#define TRIGGER_ADC_DEBUG_LED1_PORT GPIOH
#define TRIGGER_ADC_DEBUG_LED1_PIN 9
//#define DEBUG_OUTPUT_IGN1

void toggleLed(int led, int mode) {
#if 1
	static uint8_t st[5] = { 0 };
	if ((st[led] == 0 && mode == 0) || mode == 1) {
		palClearPad(TRIGGER_ADC_DEBUG_LED1_PORT, TRIGGER_ADC_DEBUG_LED1_PIN);
#ifdef DEBUG_OUTPUT_IGN1
		palClearPad(GPIOI, 8);
#endif
	}
	else if ((st[led] != 0 && mode == 0) || mode == -1) {
		palSetPad(TRIGGER_ADC_DEBUG_LED1_PORT, TRIGGER_ADC_DEBUG_LED1_PIN);
#ifdef DEBUG_OUTPUT_IGN1
		palSetPad(GPIOI, 8);
#endif
	}
   	st[led] = (st[led] + 1) % 2/*10*/; //!!!!!!!!!!!
#endif
}
#endif /* TRIGGER_ADC_DEBUG_LED */

// used for fast pin mode switching between ADC and EXTINT
static ioportid_t triggerInputPort;
static ioportmask_t triggerInputPin;

#ifndef PAL_MODE_EXTINT
#define PAL_MODE_EXTINT PAL_MODE_INPUT
#endif /* PAL_MODE_EXTINT */

void setTriggerAdcMode(triggerAdcMode_t adcMode) {
	palSetPadMode(triggerInputPort, triggerInputPin, 
		(adcMode == TRIGGER_ADC_ADC) ? PAL_MODE_INPUT_ANALOG : PAL_MODE_EXTINT);
	trigAdcState.curAdcMode = adcMode;
}

static void shaft_callback(void *arg) {
	// do the time sensitive things as early as possible!
	ioline_t pal_line = (ioline_t)arg;
	bool rise = (palReadLine(pal_line) == PAL_HIGH);

	trigAdcState.digitalCallback(stamp, true, rise);
}

static void cam_callback(void *) {
	// TODO: implement...
}

void triggerAdcCallback(triggerAdcSample_t value) {
	efitick_t stamp = getTimeNowNt();
	trigAdcState.analogCallback(stamp, value);
}


static int turnOnTriggerInputPin(const char *msg, int index, bool isTriggerShaft) {
	brain_pin_e brainPin = isTriggerShaft ?
		engineConfiguration->triggerInputPins[index] : engineConfiguration->camInputs[index];

	if (!isBrainPinValid(brainPin))
		return 0;

	trigAdcState.init(PASS_ENGINE_PARAMETER_SIGNATURE);

	triggerInputPort = getHwPort("trg", brainPin);
	triggerInputPin = getHwPin("trg", brainPin);

	ioline_t pal_line = PAL_LINE(triggerInputPort, triggerInputPin);
	efiPrintf("turnOnTriggerInputPin %s l=%d", hwPortname(brainPin), pal_line);
	
	efiExtiEnablePin(msg, brainPin, PAL_EVENT_MODE_BOTH_EDGES, isTriggerShaft ? shaft_callback : cam_callback, (void *)pal_line);
	
	// ADC mode is default, because we don't know if the wheel is already spinning
	setTriggerAdcMode(TRIGGER_ADC_ADC);

#ifdef TRIGGER_ADC_DEBUG_LED
	palSetPadMode(TRIGGER_ADC_DEBUG_LED1_PORT, TRIGGER_ADC_DEBUG_LED1_PIN, PAL_MODE_OUTPUT_PUSHPULL);
#ifdef DEBUG_OUTPUT_IGN1
	palSetPadMode(GPIOI, 8, PAL_MODE_OUTPUT_PUSHPULL);
#endif
#endif /* TRIGGER_ADC_DEBUG_LED */

	return 0;
}

void adcTriggerTurnOffInputPin(brain_pin_e brainPin) {
	efiExtiDisablePin(brainPin);
}

void adcTriggerTurnOnInputPins() {
}

adc_channel_e getAdcChannelForTrigger(void) {
	// todo: add other trigger or cam channels?
	brain_pin_e brainPin = engineConfiguration->triggerInputPins[0];
	if (!isBrainPinValid(brainPin))
		return EFI_ADC_NONE;
	return getAdcChannel(brainPin);
}

void addAdcChannelForTrigger(void) {
	adc_channel_e ch = getAdcChannelForTrigger();
	if (isAdcChannelValid(ch)) {
		addChannel("TRIG", ch, ADC_FAST);
	}
}

void onTriggerChanged(efitick_t stamp, bool isPrimary, bool isRising) {
#ifdef TRIGGER_ADC_DEBUG_LED
	toggleLed(0, 0);
#endif /* TRIGGER_ADC_DEBUG_LED */

#if 1
	// todo: support for 3rd trigger input channel
	// todo: start using real event time from HW event, not just software timer?

	// call the main trigger handler
	hwHandleShaftSignal(isPrimary ? 0 : 1, isRising, stamp);
#endif
}

#endif /* EFI_SHAFT_POSITION_INPUT && HAL_TRIGGER_USE_ADC && HAL_USE_ADC */


void TriggerAdcDetector::init() {
	// todo: move some of these to config

	// we need to make at least minNumAdcMeasurementsPerTooth for 1 tooth (i.e. between two consequent events)
	const int minNumAdcMeasurementsPerTooth = 20;
	minDeltaTimeForStableAdcDetectionNt = US2NT(US_PER_SECOND_LL * minNumAdcMeasurementsPerTooth * GPT_PERIOD_FAST / GPT_FREQ_FAST);
	// we assume that the transition occurs somewhere in the middle of the measurement period, so we take the half of it
	stampCorrectionForAdc = US2NT(US_PER_SECOND_LL * GPT_PERIOD_FAST / GPT_FREQ_FAST / 2);

	// these thresholds allow to switch from ADC mode to EXTI mode, indicating the clamping of the signal
	switchingThresholdLow = voltsToAdcDivided(1.0f);
	switchingThresholdHigh = voltsToAdcDivided(4.0f);

	// used to filter out low signals
	minDeltaThresholdWeakSignal = voltsToAdcDivided(0.05f);	// 50mV
	// we need to shift the default threshold even for strong signals because of the possible loss of the first tooth (after the sync)
	minDeltaThresholdStrongSignal = voltsToAdcDivided(0.04f);	// 5mV

	const triggerAdcSample_t adcDeltaThreshold = voltsToAdcDivided(0.25f);
	adcDefaultThreshold = voltsToAdcDivided(3.4f);	// this corresponds to VREF1 on Hellen boards
	adcMinThreshold = adcDefaultThreshold - adcDeltaThreshold;
	adcMaxThreshold = adcDefaultThreshold - adcDeltaThreshold;

	reset();
}

void TriggerAdcDetector::reset() {
	switchingCnt = 0;
	switchingTeethCnt = 0;
	// when the strong signal becomes weak, we want to ignore the increased noise
	// so we create a dead-zone between the pos. and neg. thresholds
	zeroThreshold = minDeltaThresholdWeakSignal / 2;
	triggerAdcITerm = triggerAdcITermMin;

	adcThreshold = adcDefaultThreshold;

	isSignalWeak = true;
	integralSum = 0;
	transitionCooldownCnt = 0;
	prevValue = 0;	// not set
	prevStamp = 0;
	minDeltaThresholdCntPos = 0;
	minDeltaThresholdCntNeg = 0;
}

void TriggerAdcDetector::digitalCallback(efitick_t stamp, bool isPrimary, bool rise) {
	if (curAdcMode != TRIGGER_ADC_EXTI) {
		return;
	}

	onTriggerChanged(stamp, isPrimary, rise);

	if ((stamp - prevStamp) > minDeltaTimeForStableAdcDetectionNt) {
		switchingCnt++;
	} else {
		switchingCnt = 0;
		switchingTeethCnt = 0;
	}

	if (switchingCnt > 4) {
		switchingCnt = 0;
		// we need at least 3 wide teeth to be certain!
		// we don't want to confuse them with a sync.gap
		if (switchingTeethCnt++ > 3) {
			switchingTeethCnt = 0;
			prevValue = rise ? 1: -1;
			setTriggerAdcMode(TRIGGER_ADC_ADC);
		}
	}

	prevStamp = stamp;
}

void TriggerAdcDetector::analogCallback(efitick_t stamp, triggerAdcSample_t value) {
	if (curAdcMode != TRIGGER_ADC_ADC) {
		return;
	}

	// <1V or >4V?
	if (value >= switchingThresholdHigh || value <= switchingThresholdLow) {
		switchingCnt++;
	} else {
		switchingCnt = 0;
		switchingTeethCnt = 0;
	}

	int delta = value - adcThreshold;
	int aDelta = absI(delta);
	if (isSignalWeak) {
		// todo: detect if the sensor is disconnected (where the signal is always near 'ADC_MAX_VALUE')

		// filter out low signals (noise)
		if (delta >= minDeltaThresholdWeakSignal) {
			minDeltaThresholdCntPos++;
		}
		if (delta <= -minDeltaThresholdWeakSignal) {
			minDeltaThresholdCntNeg++;
		}
	} else {
		// we just had a strong signal, let's reset the counter
		if (delta >= minDeltaThresholdWeakSignal) {
			minDeltaThresholdCntPos = DELTA_THRESHOLD_CNT_HIGH;
		}
		if (delta <= -minDeltaThresholdWeakSignal) {
			minDeltaThresholdCntNeg = DELTA_THRESHOLD_CNT_HIGH;
		}
		minDeltaThresholdCntPos--;
		minDeltaThresholdCntNeg--;
		// we haven't seen the strong signal (pos or neg) for too long, maybe it's lost or too weak?
		if (minDeltaThresholdCntPos <= 0 || minDeltaThresholdCntNeg <= 0) {
			// reset to the weak signal mode
			reset();
			return;
		} 
	}

	// the threshold should always correspond to the averaged signal.
	integralSum += delta;
	// we need some limits for the integral sum
	// we use a simple I-regulator to move the threshold 
	adcThreshold += (float)integralSum * triggerAdcITerm;
	// limit the threshold for safety
	adcThreshold = maxF(minF(adcThreshold, adcMaxThreshold), adcMinThreshold);	

	// now to the transition part... First, we need a cooldown to pre-filter the transition noise
	if (transitionCooldownCnt-- < 0)
		transitionCooldownCnt = 0;

	// we need at least 2 different measurements to detect a transition
	if (prevValue == 0) {
		// we can take the measurement only from outside the dead-zone
		if (aDelta > minDeltaThresholdWeakSignal) {
			prevValue = (delta > 0) ? 1 : -1;
		} else {
			return;
		}
	}

	// detect the edge
	int transition = 0;
	if (delta > zeroThreshold && prevValue == -1)	{
		// a rising transition found!
		transition = 1;
	} 
	else if (delta <= -zeroThreshold && prevValue == 1) {
		// a falling transition found!
		transition = -1;
	}
	else {
		return;	// both are positive/negative/zero: not interested!
	}

 	if (isSignalWeak) {
		 if (minDeltaThresholdCntPos >= DELTA_THRESHOLD_CNT_LOW && minDeltaThresholdCntNeg >= DELTA_THRESHOLD_CNT_LOW) {
			// ok, now we have a legit strong signal, let's restore the threshold
			isSignalWeak = false;
			integralSum = 0;
			zeroThreshold = minDeltaThresholdStrongSignal;
		 } else {
			// we cannot trust the weak signal!
		 	return;
		 }
	}

	if (transitionCooldownCnt <= 0) {
		onTriggerChanged(stamp - stampCorrectionForAdc, true, transition == 1);
		// let's skip some nearest possible measurements:
		// the transition cannot be SO fast, but the jitter can!
		transitionCooldownCnt = transitionCooldown;

		// it should not accumulate too much
		integralSum = 0;

		// update triggerAdcITerm
		efitime_t deltaTimeUs = NT2US(stamp - prevStamp);
		if (deltaTimeUs > 200) {	// 200 us = ~2500 RPM (we don't need this correction for large RPM)
			triggerAdcITerm = 1.0f / (triggerAdcITermCoef * deltaTimeUs);
			triggerAdcITerm = maxF(triggerAdcITerm, triggerAdcITermMin);
		}
	}

	if (switchingCnt > 4) {
		switchingCnt = 0;
		// we need at least 3 high-signal teeth to be certain!
		if (switchingTeethCnt++ > 3) {
			switchingTeethCnt = 0;
			setTriggerAdcMode(TRIGGER_ADC_EXTI);
			// we don't want to loose the signal on return
			minDeltaThresholdCntPos = DELTA_THRESHOLD_CNT_HIGH;
			minDeltaThresholdCntNeg = DELTA_THRESHOLD_CNT_HIGH;
			// we want to reset the thresholds on return
			zeroThreshold = minDeltaThresholdStrongSignal;
			adcThreshold = adcDefaultThreshold;
			integralSum = 0;
			transitionCooldownCnt = 0;
			return;
		}
	}
	
	prevValue = transition;
	prevStamp = stamp;
}

