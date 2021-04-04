/**
 * @file	HIP9011.cpp
 * @brief	HIP9011/TPIC8101 driver
 *
 * Jan 2017 status:
 *  1) seems to be kind of working - reacts to parameter changes and does produce variable output
 *  2) only one (first) channel is currently used
 *  3) engine control does not yet react to knock since very little actual testing - no engine runs with proven knock yet
 *
 *
 * http://rusefi.com/forum/viewtopic.php?f=4&t=400
 * http://rusefi.com/forum/viewtopic.php?f=5&t=778
 *
 *	pin1	VDD
 *	pin2	GND
 *
 *	pin8	Chip Select - CS
 *	pin11	Slave Data Out - MISO
 *	pin12	Slave Data In - MOSI
 *	pin13	SPI clock - SCLK
 *
 *
 * http://www.ti.com/lit/ds/symlink/tpic8101.pdf
 * http://www.intersil.com/content/dam/Intersil/documents/hip9/hip9011.pdf
 * http://www.intersil.com/content/dam/Intersil/documents/an97/an9770.pdf
 * http://e2e.ti.com/cfs-file/__key/telligent-evolution-components-attachments/00-26-01-00-00-42-36-40/TPIC8101-Training.pdf
 *
 * max SPI frequency: 5MHz max
 *
 * @date Nov 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 * @Spilly
 */

#include "global.h"
#include "engine.h"
#include "settings.h"
#include "hardware.h"
#include "rpm_calculator.h"
#include "trigger_central.h"
#include "hip9011_logic.h"
#include "hip9011.h"
#include "adc_inputs.h"
#include "perf_trace.h"
#include "thread_priority.h"

#include "engine_controller.h"

#if EFI_PROD_CODE
#include "pin_repository.h"
#include "mpu_util.h"
#include "os_util.h"
#endif

#if EFI_HIP_9011

/*==========================================================================*/
/* Local definitions.														*/
/*==========================================================================*/

/*==========================================================================*/
/* Local variables and types.												*/
/*==========================================================================*/

static NamedOutputPin intHold(PROTOCOL_HIP_NAME);
static NamedOutputPin Cs(PROTOCOL_HIP_NAME);

class Hip9011Hardware : public Hip9011HardwareInterface {
	int sendSyncCommand(uint8_t command, uint8_t *rx_ptr) override;
};

/* TODO: include following stuff in object */
/* wake semaphore */
static semaphore_t wake;

static SPIDriver *spi;

static Hip9011Hardware hardware;

static float hipValueMax = 0;

HIP9011 instance(&hardware);

static scheduling_s endTimer;

static Logging *logger;

// SPI_CR1_BR_1 // 5MHz
// SPI_CR1_CPHA Clock Phase
// todo: nicer method which would mention SPI speed explicitly?

#if EFI_PROD_CODE
static SPIConfig hipSpiCfg = {
	.circular = false,
	.end_cb = NULL,
	.ssport = NULL,
	.sspad = 0,
	.cr1 =
		SPI_CR1_8BIT_MODE |
		SPI_CR1_MSTR |
		SPI_CR1_CPHA |
		//SPI_CR1_BR_1 // 5MHz
		SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2,
	.cr2 =
		SPI_CR2_8BIT_MODE
};
#endif /* EFI_PROD_CODE */

/*==========================================================================*/
/* Forward declarations														*/
/*==========================================================================*/

static void hip_addconsoleActions(void);

/*==========================================================================*/
/* Local functions.															*/
/*==========================================================================*/

static int checkResponse(uint8_t tx, uint8_t rx) {
	/* TODO: implement response check for Advanced SPI mode too */
	if (tx == rx) {
		instance.correctResponsesCount++;
		return 0;
	} else {
		instance.invalidResponsesCount++;
		return -1;
	}
}

int Hip9011Hardware::sendSyncCommand(uint8_t tx, uint8_t *rx_ptr) {
	int ret;
	uint8_t rx;

	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	/* Setup transfer parameters. */
	spiStart(spi, &hipSpiCfg);
	/* Slave Select assertion. */
	spiSelect(spi);
	/* Transfer */
	rx = spiPolledExchange(spi, tx);
	/* Slave Select de-assertion. */
	spiUnselect(spi);
	/* Ownership release. */
	spiReleaseBus(spi);
	/* received data */
	if (rx_ptr) {
		*rx_ptr = rx;
	}
	/* check response */
	if (instance.adv_mode == false) {
		/* only default SPI mode SDO is directly equals the SDI (echo function) */
		ret = checkResponse(tx, rx);

		if (ret)
			return ret;
	}

	return 0;
}

EXTERN_ENGINE;

static int hip_wake_driver(void)
{
    /* Entering a reentrant critical zone.*/
    syssts_t sts = chSysGetStatusAndLockX();
	chSemSignalI(&wake);
	if (!port_is_isr_context()) {
		/**
		 * chSemSignalI above requires rescheduling
		 * interrupt handlers have implicit rescheduling
		 */
		chSchRescheduleS();
	}
    /* Leaving the critical zone.*/
    chSysRestoreStatusX(sts);

	return 0;
}

static void startIntegration(void *) {
	if (instance.state == READY_TO_INTEGRATE) {
		/**
		 * SPI communication is only allowed while not integrating, so we postpone the exchange
		 * until we are done integrating
		 */
		instance.state = IS_INTEGRATING;
		intHold.setHigh();
	}
}

static void endIntegration(void *) {
	/**
	 * isIntegrating could be 'false' if an SPI command was pending thus we did not integrate during this
	 * engine cycle
	 */
	if (instance.state == IS_INTEGRATING) {
		intHold.setLow();
		instance.state = WAITING_FOR_ADC_TO_SKIP;
	}
}

/**
 * Ignition callback used to start HIP integration and schedule finish
 */
void hip9011_startKnockSampling(uint8_t cylinderNumber, efitick_t nowNt) {
	if (!CONFIG(isHip9011Enabled))
		return;

	/* overrun? */
	if (instance.state != READY_TO_INTEGRATE) {
		instance.overrun++;
		return;
	}

	if (cylinderNumber == instance.expectedCylinderNumber) {
		/* save currect cylinder */
		instance.cylinderNumber = cylinderNumber;
		startIntegration(NULL);

		/* TODO: reference to knockDetectionWindowStart */
		scheduleByAngle(&endTimer, nowNt,
				engineConfiguration->knockDetectionWindowEnd - engineConfiguration->knockDetectionWindowStart,
				&endIntegration);
	} else {
		/* out of sync */
		if (instance.expectedCylinderNumber >= 0)
			instance.unsync++;
		/* save currect cylinder */
		instance.cylinderNumber = cylinderNumber;
		/* Skip integration, call driver task to prepare for next cylinder */
		instance.state = NOT_READY;
		hip_wake_driver();
	}
}

void hipAdcCallback(adcsample_t adcValue) {
	if (instance.state == WAITING_FOR_ADC_TO_SKIP) {
		instance.state = WAITING_FOR_RESULT_ADC;
	} else if (instance.state == WAITING_FOR_RESULT_ADC) {
		/* offload calculations to driver thread */
		instance.raw_value = adcValue;
		instance.state = NOT_READY;
		hip_wake_driver();
	}
}

static int hip_testAdvMode(void) {
	int ret;
	uint8_t ret0, ret1, ret2;

	/* do not care about configuration values, we meed replyes only.
	 * correct values will be uploaded later */

	/* A control byte is written to the SDI and shifted with the MSB
	 * first. The response byte on the SDO is shifted out with the MSB
	 * first. The response byte corresponds to the previous command.
	 * Therefore, the SDI shifts in a control byte n and shifts out a
	 * response command byte n − 1. */
	ret = instance.hw->sendSyncCommand(SET_BAND_PASS_CMD(0), NULL);
	if (ret)
		return ret;
	ret = instance.hw->sendSyncCommand(SET_GAIN_CMD(0), &ret0);
	if (ret)
		return ret;
	ret = instance.hw->sendSyncCommand(SET_INTEGRATOR_CMD(0), &ret1);
	if (ret)
		return ret;
	ret = instance.hw->sendSyncCommand(SET_INTEGRATOR_CMD(0), &ret2);
	if (ret)
		return ret;

	/* magic reply bytes from DS Table 2 */
	if ((ret0 == SET_BAND_PASS_REP) &&
		(ret1 == SET_GAIN_REP) &&
		(ret2 == SET_INTEGRATOR_REP))
		return 0;

	return -1;
}

static int hip_init(void) {
	int ret;

	ret = instance.hw->sendSyncCommand(SET_PRESCALER_CMD(instance.prescaler), NULL);
	if (ret) {
		/* NOTE: hip9011/tpic8101 can be in default or advansed mode at this point
		 * If we supposed not to support advanced mode this is definitely error */
		if (!CONFIG(useTpicAdvancedMode))
			return ret;
	}

	/* ...othervice or when no error is reported lets try to switch to advanced mode */
	if (CONFIG(useTpicAdvancedMode)) {
		/* enable advanced mode */
		ret = instance.hw->sendSyncCommand(SET_ADVANCED_MODE_CMD, NULL);
		if (ret) {
			uint8_t rx;
			/* communication error is detected for default mode...
			 * may be we are in advanced mode already?
			 * Now we dont care for return value */
			instance.hw->sendSyncCommand(SET_ADVANCED_MODE_CMD, &rx);
			if (rx != SET_ADVANCED_MODE_REP) {
				/* this is realy a communication problem */
				return ret;
			}
		}

		/* now we should be in advanced mode... if chip supports...
		 * set advanced mode flag now so checkResponse will switch to
		 * advanced mode checkig (not implemented) */
		instance.adv_mode = true;

		ret = hip_testAdvMode();
		if (ret) {
			warning(CUSTOM_OBD_KNOCK_PROCESSOR, "TPIC/HIP does not support advanced mode");
			instance.adv_mode = false;
		}
	}

	instance.state = READY_TO_INTEGRATE;

	return 0;
}

static THD_WORKING_AREA(hipThreadStack, UTILITY_THREAD_STACK_SIZE);

static msg_t hipThread(void *arg) {
	int ret;
	UNUSED(arg);
	chRegSetThreadName("hip9011 worker");

	/* Acquire ownership of the bus. */
	spiAcquireBus(spi);
	// some time to let the hardware start
	Cs.setValue(true);
	chThdSleepMilliseconds(100);
	Cs.setValue(false);
	chThdSleepMilliseconds(100);
	Cs.setValue(true);
	/* Ownership release. */
	spiReleaseBus(spi);

	/* init semaphore */
	chSemObjectInit(&wake, 10);

	chThdSleepMilliseconds(100);

	do {
		/* retry until success */
		ret = hip_init();
		if (ret) {
			warning(CUSTOM_OBD_KNOCK_PROCESSOR, "TPIC/HIP does not respond: %d", ret);
			chThdSleepMilliseconds(10 * 1000);
		}
	} while (ret);

	while (1) {
		msg_t msg;

		/* load new/updated settings */
		instance.handleSettings(GET_RPM() DEFINE_PARAM_SUFFIX(PASS_HIP_PARAMS));
		/* switch input channel */
		instance.handleChannel(DEFINE_PARAM_SUFFIX(PASS_HIP_PARAMS));
		/* State */
		instance.state = READY_TO_INTEGRATE;

		msg = chSemWaitTimeout(&wake, TIME_INFINITE);
		if (msg == MSG_TIMEOUT) {
			/* ??? */
		} else {
			/* Check for correct cylinder/input */
			if (instance.cylinderNumber == instance.expectedCylinderNumber) {
				/* calculations */
				float knockVolts = instance.raw_value * adcToVolts(1) * CONFIG(analogInputDividerCoefficient);
				hipValueMax = maxF(knockVolts, hipValueMax);
				engine->knockLogic(knockVolts);

				/* TunerStudio */
				tsOutputChannels.knockLevels[instance.cylinderNumber] = knockVolts;
				tsOutputChannels.knockLevel = knockVolts;

				/* counters */
				instance.samples++;
			} else {
				/* out of sync event already calculated, nothing to do */
			}
		}
	}

	return -1;
}

/*==========================================================================*/
/* Exported functions.														*/
/*==========================================================================*/

void stopHip9001_pins() {
	intHold.deInit();
	Cs.deInit();
#if EFI_PROD_CODE
	hipSpiCfg.ssport = NULL;
#endif
}

void startHip9001_pins() {
	intHold.initPin("hip int/hold", CONFIG(hip9011IntHoldPin), &CONFIG(hip9011IntHoldPinMode));
	Cs.initPin("hip CS", CONFIG(hip9011CsPin), &CONFIG(hip9011CsPinMode));
}

void initHip9011(Logging *sharedLogger) {
	logger = sharedLogger;

	if (!CONFIG(isHip9011Enabled))
		return;

#if EFI_PROD_CODE
	spi = getSpiDevice(CONFIG(hip9011SpiDevice));
	if (spi == NULL) {
		// error already reported
		return;
	}

	hipSpiCfg.ssport = getHwPort("hip", CONFIG(hip9011CsPin));
	hipSpiCfg.sspad = getHwPin("hip", CONFIG(hip9011CsPin));
#endif /* EFI_PROD_CODE */

	startHip9001_pins();

	/* load settings */
	instance.prescaler = CONFIG(hip9011PrescalerAndSDO);

	scheduleMsg(logger, "Starting HIP9011/TPIC8101 driver");

	chThdCreateStatic(hipThreadStack, sizeof(hipThreadStack), PRIO_HIP9011, (tfunc_t)(void*) hipThread, NULL);

	hip_addconsoleActions();
}

/*==========================================================================*/
/* Debug functions.															*/
/*==========================================================================*/

static void showHipInfo(void) {
	if (!CONFIG(isHip9011Enabled)) {
		scheduleMsg(logger, "hip9011 driver not active");
		return;
	}

	scheduleMsg(logger, "enabled=%s state=%s channel=%d cylinder=%d next=%d",
		boolToString(CONFIG(isHip9011Enabled)),
		getHip_state_e(instance.state),
		instance.channelIdx, instance.cylinderNumber, instance.expectedCylinderNumber);

	scheduleMsg(logger, " bore=%.2fmm freq=%.2fkHz",
		engineConfiguration->cylinderBore,
		instance.getBand(PASS_HIP_PARAMS));

	scheduleMsg(logger, " band idx=%d integrator idx=%d  gain %.2f (idx %d) output=%s",
		instance.bandIdx,
		instance.intergratorIdx,
		engineConfiguration->hip9011Gain,
		instance.gainIdx,
		getAdc_channel_e(engineConfiguration->hipOutputChannel));

	scheduleMsg(logger, " PaSDO=0x%x",
		instance.prescaler);

	scheduleMsg(logger, " knockVThreshold=%.2f knockCount=%d maxKnockSubDeg=%.2f",
		engineConfiguration->knockVThreshold,
		engine->knockCount,
		engineConfiguration->maxKnockSubDeg);

	scheduleMsg(logger, " spi=%s IntHold@%s(0x%x) correct response=%d incorrect response=%d (%s)",
		getSpi_device_e(engineConfiguration->hip9011SpiDevice),
		hwPortname(CONFIG(hip9011IntHoldPin)),
		CONFIG(hip9011IntHoldPinMode),
		instance.correctResponsesCount,
		instance.invalidResponsesCount,
		instance.invalidResponsesCount > 0 ? "NOT GOOD" : "ok");

#if EFI_PROD_CODE
	scheduleMsg(logger, "hip %.2fv/last=%.2f/max=%.2f adv=%d",
		engine->knockVolts,
		getVoltage("hipinfo", engineConfiguration->hipOutputChannel),
		hipValueMax,
		CONFIG(useTpicAdvancedMode));
	scheduleMsg(logger, "hip9011 CS@%s",
		hwPortname(CONFIG(hip9011CsPin)));
	printSpiConfig(logger, "hip9011", CONFIG(hip9011SpiDevice));
#endif /* EFI_PROD_CODE */

	scheduleMsg(logger, "start %.2f end %.2f",
		engineConfiguration->knockDetectionWindowStart,
		engineConfiguration->knockDetectionWindowEnd);

	scheduleMsg(logger, "Status: samples %d overruns %d sync miss %d",
		instance.samples, instance.overrun, instance.unsync);

	hipValueMax = 0;
	engine->printKnockState();
}

static void setMaxKnockSubDeg(int value) {
    engineConfiguration->maxKnockSubDeg = value;
    showHipInfo();
}

static void setKnockThresh(float value) {
    engineConfiguration->knockVThreshold = value;
    showHipInfo();
}

static void setPrescalerAndSDO(int value) {
	engineConfiguration->hip9011PrescalerAndSDO = value;
}

static void setHipBand(float value) {
	engineConfiguration->knockBandCustom = value;
	showHipInfo();
}

static void setHipGain(float value) {
	engineConfiguration->hip9011Gain = value;
	showHipInfo();
}

static void hip_addconsoleActions(void) {
	addConsoleAction("hipinfo", showHipInfo);
	addConsoleActionF("set_gain", setHipGain);
	addConsoleActionF("set_band", setHipBand);
	addConsoleActionI("set_hip_prescalerandsdo", setPrescalerAndSDO);
    addConsoleActionF("set_knock_threshold", setKnockThresh);
    addConsoleActionI("set_max_knock_sub_deg", setMaxKnockSubDeg);
}

#endif /* EFI_HIP_9011 */
