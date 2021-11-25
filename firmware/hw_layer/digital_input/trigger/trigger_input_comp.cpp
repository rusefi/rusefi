/**
 * @file	trigger_input_comp.cpp
 * @brief	Position sensor hardware layer, Using hardware comparator
 *
 * @date Apr 13, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 * @author andreika <prometheus.pcb@gmail.com>
 */
 
#include "pch.h"

#if (EFI_SHAFT_POSITION_INPUT && HAL_USE_COMP) || defined(__DOXYGEN__)

#include "hal_comp.h"

#include "trigger_input.h"
#include "digital_input_icu.h"

static volatile int centeredDacValue = 127;
static volatile int toothCnt = 0;
static volatile int dacHysteresisMin = 1;	// = 5V * 1/256 (8-bit DAC) = ~20mV
static volatile int dacHysteresisMax = 15;	// = ~300mV
static volatile int dacHysteresisDelta = dacHysteresisMin;
static volatile int hystUpdatePeriodNumEvents = 116; // every ~1 turn of 60-2 wheel
static volatile efitick_t prevNt = 0;
// VR-sensor saturation stuff
static volatile float curVrFreqNt = 0, saturatedVrFreqNt = 0;

// We want to interpolate between min and max depending on the signal level (adaptive hysteresis).
// But we don't want to measure the signal amplitude directly, so we estimate it by measuring the signal frequency:
// for VR sensors, the amplitude is inversely proportional to the tooth's 'time-width'.
// We find it by dividing the total time by the teeth count, and use the reciprocal value as signal frequency!
static void setHysteresis(COMPDriver *comp, int sign) {
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
	comp_lld_set_dac_value(comp, centeredDacValue + dacHysteresisDelta * sign);
}

static void comp_shaft_callback(COMPDriver *comp) {
	efitick_t stamp = getTimeNowNt();
	
	uint32_t status = comp_lld_get_status(comp);
	int isPrimary = (comp == EFI_COMP_PRIMARY_DEVICE);

	trigger_event_e signal;
	if (status & COMP_IRQ_RISING) {
		hwHandleShaftSignal(isPrimary ? 0 : 1, true, stamp);
		// shift the threshold down a little bit to avoid false-triggering (threshold hysteresis)
		setHysteresis(comp, -1);
	}

	if (status & COMP_IRQ_FALLING) {
		hwHandleShaftSignal(isPrimary ? 0 : 1, false, stamp);
		// shift the threshold up a little bit to avoid false-triggering (threshold hysteresis)
		setHysteresis(comp, 1);
	}
}

// todo: add cam support?
#if 0
static void comp_cam_callback(COMPDriver *comp) {
	efitick_t stamp = getTimeNowNt();

	if (isRising) {
		hwHandleVvtCamSignal(TV_RISE, stamp, index);
	} else {
		hwHandleVvtCamSignal(TV_FALL, stamp, index);
	}
}
#endif

static COMPConfig comp_shaft_cfg = {
	COMP_OUTPUT_NORMAL, COMP_IRQ_BOTH,
	comp_shaft_callback,
	0
};

static bool isCompEnabled = false;

void turnOnTriggerInputPins() {
	compInit();
	compStart(EFI_COMP_PRIMARY_DEVICE, &comp_shaft_cfg);

	applyNewTriggerInputPins();
}

static int getDacValue(uint8_t voltage) {
	constexpr float maxDacValue = 255.0f;	// 8-bit DAC
	return (int)efiRound(maxDacValue * (float)voltage * VOLTAGE_1_BYTE_PACKING_DIV / engineConfiguration->adcVcc, 1.0f);
}

void startTriggerInputPins(void) {
	//efiAssertVoid(CUSTOM_ERR_, !isCompEnabled, "isCompEnabled");
	if (isCompEnabled) {
		efiPrintf("startTIPins(): already enabled!");
		return;
	}

	centeredDacValue = getDacValue(engineConfiguration->triggerCompCenterVolt);	// usually 2.5V resistor divider
	
	dacHysteresisMin = getDacValue(engineConfiguration->triggerCompHystMin);	// usually ~20mV
	dacHysteresisMax = getDacValue(engineConfiguration->triggerCompHystMax);	// usually ~300mV
	dacHysteresisDelta = dacHysteresisMin;
	
	// 20 rpm (60_2) = 1000*60/((2*60)*20) = 25 ms for 1 tooth event
	float satRpm = engineConfiguration->triggerCompSensorSatRpm * RPM_1_BYTE_PACKING_MULT;
	hystUpdatePeriodNumEvents = engine->triggerCentral.triggerShape.getSize();	// = 116 for "60-2" trigger wheel
	float saturatedToothDurationUs = 60.0f * US_PER_SECOND_F / satRpm / hystUpdatePeriodNumEvents;
	saturatedVrFreqNt = 1.0f / US2NT(saturatedToothDurationUs);
	
	efiPrintf("startTIPins(): cDac=%d hystMin=%d hystMax=%d satRpm=%.0f satFreq*1k=%f period=%d", 
		centeredDacValue, dacHysteresisMin, dacHysteresisMax, satRpm, saturatedVrFreqNt * 1000.0f, hystUpdatePeriodNumEvents);
#ifdef EFI_TRIGGER_COMP_ADAPTIVE_HYSTERESIS
	efiPrintf("startTIPins(): ADAPTIVE_HYSTERESIS enabled!");
#endif /* EFI_TRIGGER_COMP_ADAPTIVE_HYSTERESIS */
		
	int channel = EFI_COMP_TRIGGER_CHANNEL; // todo: use getInputCaptureChannel(hwPin);

	// todo: set pin mode to default (analog/comparator)
	//palSetPadMode(comp_channel_port[channel], comp_channel_pad[channel], PAL_MODE_INPUT_ANALOG);
	
	// no generic hal support for extended COMP configuration, so we use hal_lld layer...
	osalSysLock();
	comp_lld_set_dac_value(EFI_COMP_PRIMARY_DEVICE, centeredDacValue);
	comp_lld_channel_enable(EFI_COMP_PRIMARY_DEVICE, channel);
    osalSysUnlock();
    
	compEnable(EFI_COMP_PRIMARY_DEVICE);
	isCompEnabled = true;
}

void stopTriggerInputPins(void) {
	if (!isCompEnabled) {
		efiPrintf("stopTIPins(): already disabled!");
		return;
	}

	efiPrintf("stopTIPins();");

	compDisable(EFI_COMP_PRIMARY_DEVICE);
	isCompEnabled = false;
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

#endif /* EFI_SHAFT_POSITION_INPUT && HAL_USE_COMP */
