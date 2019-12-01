/**
 * @file	trigger_input_comp.cpp
 * @brief	Position sensor hardware layer, Using hardware comparator
 *
 * @date Apr 13, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 * @author andreika <prometheus.pcb@gmail.com>
 */
 
#include "global.h"

#if (EFI_SHAFT_POSITION_INPUT && HAL_USE_COMP) || defined(__DOXYGEN__)

#include "hal_comp.h"

#include "trigger_input.h"
#include "digital_input_icu.h"


extern bool hasFirmwareErrorFlag;

EXTERN_ENGINE
;
static Logging *logger;

static int centeredDacValue = 127;

static const int dacNoiseDeltaMin = 1;	// = 5V * 1/256 (8-bit DAC) = ~20mV
static const int dacNoiseDeltaMax = 15;	// = ~300mV

// todo: interpolate between min and max depending on the signal level (adaptive hysteresis)
static const int dacNoiseDelta = dacNoiseDeltaMax;

static void comp_shaft_callback(COMPDriver *comp) {
	uint32_t status = comp_lld_get_status(comp);
	int isPrimary = (comp == EFI_COMP_PRIMARY_DEVICE);
	if (!isPrimary && !TRIGGER_SHAPE(needSecondTriggerInput)) {
		return;
	}
	trigger_event_e signal;
	if (status & COMP_IRQ_RISING) {
		signal = isPrimary ? (engineConfiguration->invertPrimaryTriggerSignal ? SHAFT_PRIMARY_FALLING : SHAFT_PRIMARY_RISING) : 
			(engineConfiguration->invertSecondaryTriggerSignal ? SHAFT_SECONDARY_FALLING : SHAFT_SECONDARY_RISING);
		hwHandleShaftSignal(signal);
		// shift the threshold down a little bit to avoid false-triggering (threshold hysteresis)
		comp_lld_set_dac_value(comp, centeredDacValue - dacNoiseDelta);
	}

	if (status & COMP_IRQ_FALLING) {
		signal = isPrimary ? (engineConfiguration->invertPrimaryTriggerSignal ? SHAFT_PRIMARY_RISING : SHAFT_PRIMARY_FALLING) : 
			(engineConfiguration->invertSecondaryTriggerSignal ? SHAFT_SECONDARY_RISING : SHAFT_SECONDARY_FALLING);
		hwHandleShaftSignal(signal);
		// shift the threshold up a little bit to avoid false-triggering (threshold hysteresis)
		comp_lld_set_dac_value(comp, centeredDacValue + dacNoiseDelta);
	}
}

// todo: add cam support?
#if 0
static void comp_cam_callback(COMPDriver *comp) {
	if (isRising) {
		hwHandleVvtCamSignal(TV_RISE);
	} else {
		hwHandleVvtCamSignal(TV_FALL);
	}
}
#endif

static COMPConfig comp_shaft_cfg = {
	COMP_OUTPUT_NORMAL, COMP_IRQ_BOTH,
	comp_shaft_callback,
	0
};

static bool isCompEnabled = false;

void turnOnTriggerInputPins(Logging *sharedLogger) {
	logger = sharedLogger;
	compInit();
	compStart(EFI_COMP_PRIMARY_DEVICE, &comp_shaft_cfg);

	applyNewTriggerInputPins();
}

void startTriggerInputPins(void) {
	//efiAssertVoid(CUSTOM_ERR_, !isCompEnabled, "isCompEnabled");

	constexpr float vSensorRef = 2.5f;	// 2.5V resistor divider; todo: migrate to settings?
	constexpr float maxDacValue = 255.0f;
	centeredDacValue = (int)efiRound(maxDacValue / engineConfiguration->adcVcc * vSensorRef, 1.0f);
	
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
	if (!isCompEnabled)
		return;
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
