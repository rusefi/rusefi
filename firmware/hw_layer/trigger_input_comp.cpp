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
#include "digital_input_hw.h"
#include "pin_repository.h"
#include "trigger_structure.h"
#include "trigger_central.h"
#include "engine_configuration.h"

#define TRIGGER_SUPPORTED_CHANNELS 2

extern bool hasFirmwareErrorFlag;

EXTERN_ENGINE
;
static Logging *logger;

static void comp_shaft_callback(COMPDriver *comp) {
	bool isRising = (comp_lld_get_status(comp) & COMP_IRQ_RISING) != 0;
	int isPrimary = (comp == EFI_COMP_PRIMARY_DEVICE);
	if (!isPrimary && !TRIGGER_SHAPE(needSecondTriggerInput)) {
		return;
	}
	trigger_event_e signal;
	if (isRising) {
		signal = isPrimary ? (engineConfiguration->invertPrimaryTriggerSignal ? SHAFT_PRIMARY_FALLING : SHAFT_PRIMARY_RISING) : 
			(engineConfiguration->invertSecondaryTriggerSignal ? SHAFT_SECONDARY_FALLING : SHAFT_SECONDARY_RISING);
	} else {
		signal = isPrimary ? (engineConfiguration->invertPrimaryTriggerSignal ? SHAFT_PRIMARY_RISING : SHAFT_PRIMARY_FALLING) : 
			(engineConfiguration->invertSecondaryTriggerSignal ? SHAFT_SECONDARY_RISING : SHAFT_SECONDARY_FALLING);
	}
	hwHandleShaftSignal(signal);

#ifdef EFI_TRIGGER_DEBUG_BLINK
	__blink(1);
#endif
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

	const float vRef = 5.0f;
	const float vSensorRef = 2.5f;	// 2.5V resistor divider
	// when VR sensor is silent, there's still some noise around vRef, so we need a small threshold to avoid false triggering
	const float noSignalThreshold = 0.05f;	
	
	const int maxDacValue = 255;
	const int vDac = (int)(int)efiRound(maxDacValue * (vSensorRef - noSignalThreshold) / vRef, 1.0f);
	
	int channel = EFI_COMP_TRIGGER_CHANNEL; // todo: use getInputCaptureChannel(hwPin);

	// todo: set pin mode to default (analog/comparator)
	//palSetPadMode(comp_channel_port[channel], comp_channel_pad[channel], PAL_MODE_INPUT_ANALOG);
	
	// no generic hal support for extended COMP configuration, so we use hal_lld layer...
	osalSysLock();
	comp_lld_set_dac_value(EFI_COMP_PRIMARY_DEVICE, vDac);
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
		if (CONFIGB(triggerInputPins)[i]
				!= activeConfiguration.bc.triggerInputPins[i]) {
			turnOffTriggerInputPin(activeConfiguration.bc.triggerInputPins[i]);
		}
	}
	if (engineConfiguration->camInput != activeConfiguration.camInput) {
		turnOffTriggerInputPin(activeConfiguration.camInput);
	}
#endif
}

void applyNewTriggerInputPins(void) {
	// first we will turn off all the changed pins
	stopTriggerInputPins();
	// then we will enable all the changed pins
	startTriggerInputPins();
}

#endif /* EFI_SHAFT_POSITION_INPUT && HAL_USE_COMP */
