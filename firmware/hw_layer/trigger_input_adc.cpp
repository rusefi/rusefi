/**
 * @file	trigger_input_adc.cpp
 * @brief	Position sensor hardware layer, Using ADC and software comparator
 *
 * @date Jan 27, 2020
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */
 
#include "pch.h"

#if (EFI_SHAFT_POSITION_INPUT && HAL_TRIGGER_USE_ADC && HAL_USE_ADC) || defined(__DOXYGEN__)

#include "trigger_input.h"
#include "digital_input_exti.h"

//!!!!!!!!!!!!!!!
extern "C" void toggleLed(int led, int mode);
#define BOARD_MOD1_PORT GPIOD
#define BOARD_MOD1_PIN 5

#if 0
static volatile int centeredDacValue = 127;
static volatile int toothCnt = 0;
static volatile int dacHysteresisMin = 1;	// = 5V * 1/256 (8-bit DAC) = ~20mV
static volatile int dacHysteresisMax = 15;	// = ~300mV
static volatile int dacHysteresisDelta = dacHysteresisMin;
static volatile int hystUpdatePeriodNumEvents = 116; // every ~1 turn of 60-2 wheel
static volatile efitick_t prevNt = 0;
// VR-sensor saturation stuff
static volatile float curVrFreqNt = 0, saturatedVrFreqNt = 0;
#endif

static const adcsample_t adcDefaultThreshold = (ADC_MAX_VALUE / 2);
static const adcsample_t adcMinThreshold = adcDefaultThreshold - 200;
static const adcsample_t adcMaxThreshold = adcDefaultThreshold + 200;

static float triggerAdcITermCoef = 1600.0f;
static float triggerAdcITermMin = 3.125e-8f;	// corresponds to rpm=25

static int transitionCooldown = 5;

#define DELTA_THRESHOLD_CNT_LOW (GPT_FREQ_FAST / GPT_PERIOD_FAST / 32)		// ~1/32 second?
#define DELTA_THRESHOLD_CNT_HIGH (GPT_FREQ_FAST / GPT_PERIOD_FAST / 4)		// ~1/4 second?

/*static */triggerAdcMode_t curAdcMode = TRIGGER_NONE;
/*static*/ float adcThreshold = adcDefaultThreshold;
static float triggerAdcITerm = triggerAdcITermMin;
// these thresholds allow to switch from ADC mode (low-rpm) to EXTI mode (fast-rpm), indicating the clamping of the signal 
static adcsample_t switchingThresholdLow = 0, switchingThresholdHigh = 0;
static efitick_t minDeltaTimeForStableAdcDetectionNt = 0;
static efitick_t stampCorrectionForAdc = 0;
static int switchingCnt = 0, switchingTeethCnt = 0;
static int prevValue = 0;	// not set
static efitick_t prevStamp = 0;
// we need to distinguish between weak and strong signals because of different SNR and thresholds.
static bool isSignalWeak = true;
static int zeroThreshold = 0;
// the 'center' of the signal is variable, so we need to adjust the thresholds.
static int minDeltaThresholdWeakSignal = 0, minDeltaThresholdStrongSignal = 0;
// this is the number of measurements while we store the counter before we reset to 'isSignalWeak'
static int minDeltaThresholdCntPos = 0, minDeltaThresholdCntNeg = 0;
static int integralSum = 0;
static int transitionCooldownCnt = 0;

// used for fast pin mode switching between ADC and EXTINT
static ioportid_t triggerInputPort;
static ioportmask_t triggerInputPin;


#if 0
// We want to interpolate between min and max depending on the signal level (adaptive hysteresis).
// But we don't want to measure the signal amplitude directly, so we estimate it by measuring the signal frequency:
// for VR sensors, the amplitude is inversely proportional to the tooth's 'time-width'.
// We find it by dividing the total time by the teeth count, and use the reciprocal value as signal frequency!
static void setHysteresis(int sign) {
	// update the hysteresis threshold, but not for every tooth
#ifdef EFI_TRIGGER_COMP_ADAPTIVE_HYSTERESIS
	if (toothCnt++ > hystUpdatePeriodNumEvents) {
		efitick_t nowNt = getTimeNowNt();
		curVrFreqNt = (float)toothCnt / (float)(nowNt - prevNt);
		dacHysteresisDelta = (int)efiRound(interpolateClamped(0.0f, dacHysteresisMin, saturatedVrFreqNt, dacHysteresisMax, curVrFreqNt), 1.0f);
		toothCnt = 0;
		prevNt = nowNt;
#ifdef TRIGGER_COMP_EXTREME_LOGGING
		efiPrintf("* f=%f d=%d", curVrFreqNt * 1000.0f, dacHysteresisDelta);
#endif /* TRIGGER_COMP_EXTREME_LOGGING */
	}
#endif /* EFI_TRIGGER_COMP_ADAPTIVE_HYSTERESIS */

	//comp_lld_set_dac_value(comp, centeredDacValue + dacHysteresisDelta * sign);
}
#endif

static void setTriggerAdcMode(triggerAdcMode_t adcMode) {
	palSetPadMode(triggerInputPort, triggerInputPin, 
		(adcMode == TRIGGER_ADC) ? PAL_MODE_INPUT_ANALOG : PAL_MODE_ALTERNATE(PAL_MODE_ALTERNATIVE_EXTINT));
	curAdcMode = adcMode;
}

static void onTriggerChanged(efitick_t stamp, bool isPrimary, bool isRising) {
	//!!!!!!!!!
	palWritePad(BOARD_MOD1_PORT, BOARD_MOD1_PIN, isRising ? 1 : 0);

	//toggleLed(2, (curAdcMode == TRIGGER_ADC) ? 0 : -1);
	//toggleLed(3, (curAdcMode == TRIGGER_EXTI) ? 0 : -1);

#if 1
	// todo: support for 3rd trigger input channel
	// todo: start using real event time from HW event, not just software timer?

	// call the main trigger handler
	hwHandleShaftSignal(isPrimary ? 0 : 1, isRising, stamp);
#endif
}


static void shaft_callback(void *arg, efitick_t stamp) {
	if (curAdcMode != TRIGGER_EXTI) {
		return;
	}
	
	// do the time sensitive things as early as possible!
	ioline_t pal_line = (ioline_t)arg;
	bool rise = (palReadLine(pal_line) == PAL_HIGH);

	onTriggerChanged(stamp, true, rise);

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
			setTriggerAdcMode(TRIGGER_ADC);
		}
	}

	prevStamp = stamp;
}

static void cam_callback(void *, efitick_t) {
}

// todo: add cam support?
#if 0
static void comp_cam_callback(COMPDriver *comp) {
	efitick_t stamp = getTimeNowNt();

	if (isRising) {
		hwHandleVvtCamSignal(TV_RISE, stamp);
	} else {
		hwHandleVvtCamSignal(TV_FALL, stamp);
	}
}
#endif

void turnOnTriggerInputPins() {
	applyNewTriggerInputPins();
}

#if 0
static int getDacValue(uint8_t voltage DECLARE_ENGINE_PARAMETER_SUFFIX) {
	constexpr float maxDacValue = 255.0f;	// 8-bit DAC
	return (int)efiRound(maxDacValue * (float)voltage * VOLTAGE_1_BYTE_PACKING_DIV / CONFIG(adcVcc), 1.0f);
}
#endif

static void resetTriggerDetector() {
	// todo: move some of these to config

	// we need to make at least minNumAdcMeasurementsPerTooth for 1 tooth (i.e. between two consequent events)
	const int minNumAdcMeasurementsPerTooth = 20;
	minDeltaTimeForStableAdcDetectionNt = US2NT(US_PER_SECOND_LL * minNumAdcMeasurementsPerTooth * GPT_PERIOD_FAST / GPT_FREQ_FAST);
	// we assume that the transition occurs somewhere in the middle of the measurement period, so we take the half of it
	stampCorrectionForAdc = US2NT(US_PER_SECOND_LL * GPT_PERIOD_FAST / GPT_FREQ_FAST / 2);
	// these thresholds allow to switch from ADC mode to EXTI mode, indicating the clamping of the signal
	switchingThresholdLow = voltsToAdc(1.0f);
	switchingThresholdHigh = voltsToAdc(4.0f);
	switchingCnt = 0;
	switchingTeethCnt = 0;
	// used to filter out low signals
	minDeltaThresholdWeakSignal = voltsToAdc(0.05f);	// 50mV
	// we need to shift the default threshold even for strong signals because of the possible loss of the first tooth (after the sync)
	minDeltaThresholdStrongSignal = voltsToAdc(0.04f);	// 5mV
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

static int turnOnTriggerInputPin(const char *msg, int index, bool isTriggerShaft) {
	brain_pin_e brainPin = isTriggerShaft ?
		CONFIG(triggerInputPins)[index] : engineConfiguration->camInputs[index];

	if (!isBrainPinValid(brainPin))
		return 0;
#if 0
	centeredDacValue = getDacValue(CONFIG(triggerCompCenterVolt) PASS_ENGINE_PARAMETER_SUFFIX);	// usually 2.5V resistor divider
	
	dacHysteresisMin = getDacValue(CONFIG(triggerCompHystMin) PASS_ENGINE_PARAMETER_SUFFIX);	// usually ~20mV
	dacHysteresisMax = getDacValue(CONFIG(triggerCompHystMax) PASS_ENGINE_PARAMETER_SUFFIX);	// usually ~300mV
	dacHysteresisDelta = dacHysteresisMin;
	
	// 20 rpm (60_2) = 1000*60/((2*60)*20) = 25 ms for 1 tooth event
	float satRpm = CONFIG(triggerCompSensorSatRpm) * RPM_1_BYTE_PACKING_MULT;
	hystUpdatePeriodNumEvents = ENGINE(triggerCentral.triggerShape).getSize();	// = 116 for "60-2" trigger wheel
	float saturatedToothDurationUs = 60.0f * US_PER_SECOND_F / satRpm / hystUpdatePeriodNumEvents;
	saturatedVrFreqNt = 1.0f / US2NT(saturatedToothDurationUs);
	
	efiPrintf("startTIPins(): cDac=%d hystMin=%d hystMax=%d satRpm=%.0f satFreq*1k=%f period=%d", 
		centeredDacValue, dacHysteresisMin, dacHysteresisMax, satRpm, saturatedVrFreqNt * 1000.0f, hystUpdatePeriodNumEvents);
#endif

	resetTriggerDetector();

	triggerInputPort = getHwPort("trg", brainPin);
	triggerInputPin = getHwPin("trg", brainPin);

	ioline_t pal_line = PAL_LINE(triggerInputPort, triggerInputPin);
	efiPrintf("turnOnTriggerInputPin %s l=%d", hwPortname(brainPin), pal_line);
	
	efiExtiEnablePin(msg, brainPin, PAL_EVENT_MODE_BOTH_EDGES, isTriggerShaft ? shaft_callback : cam_callback, (void *)pal_line);
	
	// ADC mode is default, because we don't know if the wheel is already spinning
	setTriggerAdcMode(TRIGGER_ADC);

	return 0;
}

void startTriggerInputPins(void) {
	for (int i = 0; i < TRIGGER_SUPPORTED_CHANNELS; i++) {
		if (isConfigurationChanged(triggerInputPins[i])) {
			const char * msg = (i == 0 ? "trigger#1" : (i == 1 ? "trigger#2" : "trigger#3"));
			turnOnTriggerInputPin(msg, i, true);
		}
	}
}


void stopTriggerInputPins(void) {
	efiPrintf("stopTIPins();");

#if 0
	for (int i = 0; i < TRIGGER_SUPPORTED_CHANNELS; i++) {
		if (isConfigurationChanged(bc.triggerInputPins[i])) {
			turnOffTriggerInputPin(activeConfiguration.bc.triggerInputPins[i]);
		}
	}
	if (isConfigurationChanged(camInput)) {
		turnOffTriggerInputPin(activeConfiguration.camInput);
	}
#endif
}

adc_channel_e getAdcChannelForTrigger(void) {
	// todo: add other trigger or cam channels?
	brain_pin_e brainPin = CONFIG(triggerInputPins)[0];
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

void triggerAdcCallback(adcsample_t value) {
	if (curAdcMode != TRIGGER_ADC) {
		return;
	}

	efitick_t stamp = getTimeNowNt();

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
			resetTriggerDetector();
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
		//!!!!!!!!!!
		//toggleLed(2, 0);

		return;	// both are positive/negative/zero: not interested!
	}

	//!!!!!!!!!!
	//toggleLed(2, -1);
	//!!!!!!!!!!
	//toggleLed(3, 0);

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
			setTriggerAdcMode(TRIGGER_EXTI);
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

#endif /* EFI_SHAFT_POSITION_INPUT && HAL_TRIGGER_USE_ADC && HAL_USE_ADC */
