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

#include "pch.h"

#include "hardware.h"
#include "trigger_central.h"
#include "hip9011_logic.h"
#include "hip9011.h"
#include "knock_logic.h"

#if EFI_PROD_CODE
#include "mpu_util.h"
#endif

#if EFI_HIP_9011

/*==========================================================================*/
/* Local definitions.														*/
/*==========================================================================*/

/*==========================================================================*/
/* Local variables and types.												*/
/*==========================================================================*/

#define PROTOCOL_HIP_NAME "HIP"

static NamedOutputPin intHold(PROTOCOL_HIP_NAME);
static NamedOutputPin Cs(PROTOCOL_HIP_NAME);

class Hip9011Hardware : public Hip9011HardwareInterface {
	int sendSyncCommand(uint8_t command, uint8_t *rx_ptr) override;

public:
	scheduling_s startTimer;
	scheduling_s endTimer;

private:
	int checkResponseDefMode(uint8_t tx, uint8_t rx);
	int checkResponseAdvMode(uint8_t tx, uint8_t rx);

	uint8_t rep_mask;
	uint8_t rep_value;
};

/* TODO: include following stuff in object */
/* wake semaphore */
static semaphore_t wake;

static Hip9011Hardware hardware;

HIP9011 instance(&hardware);

#if EFI_HIP_9011_DEBUG
	static float normalizedValue[HIP_INPUT_CHANNELS];
	static float normalizedValueMax[HIP_INPUT_CHANNELS];
#endif

// SPI_CR1_BR_1 // 5MHz
// SPI_CR1_CPHA Clock Phase
// todo: nicer method which would mention SPI speed explicitly?

#if EFI_PROD_CODE
static SPIDriver *spi;

static SPIConfig hipSpiCfg = {
	.circular = false,
#ifdef _CHIBIOS_RT_CONF_VER_6_1_
	.end_cb = NULL,
#else
	.slave = false,
	.data_cb = NULL,
	.error_cb = NULL,
#endif
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

#if EFI_HIP_9011_DEBUG
	static void hip_addconsoleActions();
#endif

/*==========================================================================*/
/* Local functions.															*/
/*==========================================================================*/

int Hip9011Hardware::checkResponseDefMode(uint8_t tx, uint8_t rx) {
	/* in default SPI mode SDO is directly equals the SDI (echo function) */
	if (tx == rx) {
		return 0;
	} else {
		return -1;
	}
}

int Hip9011Hardware::checkResponseAdvMode(uint8_t tx, uint8_t rx) {
	int ret = 0;

	/* check reply */
	if ((rx & rep_mask) != rep_value)
		ret = -1;

	/* extract mask and value for next reply */
	if ((tx & 0xe0) == SET_PRESCALER_CMD(0)){
		/* D7 to D0 of digital integrator output */
		rep_mask  = 0x00;
		rep_value = 0x00;
	} else if ((tx & 0xfe) == SET_CHANNEL_CMD(0)) {
		/* D9 to D8 of digital integrator output and six zeroes */
		rep_mask  = 0x3f;
		rep_value = 0x00;
	} else if ((tx & 0xc0) == SET_BAND_PASS_CMD(0)) {
		rep_mask  = 0xff;
		rep_value = SET_BAND_PASS_REP;
	} else if ((tx & 0xc0) == SET_GAIN_CMD(0)) {
		rep_mask  = 0xff;
		rep_value = SET_GAIN_REP;
	} else if ((tx & 0xe0) == SET_INTEGRATOR_CMD(0)) {
		rep_mask  = 0xff;
		rep_value = SET_INTEGRATOR_REP;
	} else if ((tx & 0xff) == SET_ADVANCED_MODE_CMD) {
		rep_mask  = 0xff;
		rep_value = SET_ADVANCED_MODE_REP;
	} else {
		/* unknown */
		rep_mask  = 0x00;
		rep_value = 0x00;
	}

	return ret;
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
	if (rx_ptr)
		*rx_ptr = rx;
	/* check response */
	if (instance.adv_mode)
		ret = checkResponseAdvMode(tx, rx);
	else
		ret = checkResponseDefMode(tx, rx);

	#if EFI_HIP_9011_DEBUG
		/* statistic counters */
		if (ret)
			instance.invalidResponsesCount++;
		else
			instance.correctResponsesCount++;
	#endif

	return ret;
}

static int hip_wake_driver()
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

static void startIntegration(HIP9011 *hip) {
	if (hip->state == READY_TO_INTEGRATE) {
		/**
		 * SPI communication is only allowed while not integrating, so we postpone the exchange
		 * until we are done integrating
		 */
		hip->state = IS_INTEGRATING;
		intHold.setHigh();
	} else {
		#if EFI_HIP_9011_DEBUG
			hip->overrun++;
		#endif
	}
}

static void endIntegration(HIP9011 *hip) {
	/**
	 * isIntegrating could be 'false' if an SPI command was pending thus we did not integrate during this
	 * engine cycle
	 */
	if (hip->state == IS_INTEGRATING) {
		intHold.setLow();

		hip->knockSampleTimestamp = getTimeNowNt();

		if (instance.adv_mode) {
			/* read value over SPI in thread mode */
			hip->state = NOT_READY;
			hip_wake_driver();
		} else {
			/* wait for ADC samples */
			hip->state = WAITING_FOR_ADC_TO_SKIP;
		}
	}
}

void onStartKnockSampling(uint8_t /*cylinderIndex*/, float /*samplingTimeSeconds*/, uint8_t /*channelIdx*/) {
	/* TODO: @dron0gus: not sure if we need the expectedCylinderNumber logic at all

	Something like this might be right:

	startIntegration(&instance);

	efitick_t windowLength = USF2NT(1e6 * samplingTimeSeconds);

	engine->executor.scheduleByTimestampNt("knock", &hardware.endTimer, getTimeNowNt() + windowLength, { endIntegration, &instance });
	*/
}

/**
 * Ignition callback used to start HIP integration and schedule finish
 */
void hip9011_onFireEvent(uint8_t cylinderNumber, efitick_t nowNt) {
	if (!engineConfiguration->isHip9011Enabled)
		return;

	/* We are not checking here for READY_TO_INTEGRATE state as
	 * previous integration may be still in progress, while
	 * we are scheduling next integration start only
	 * knockDetectionWindowStart from now.
	 * Check for correct state will be done at startIntegration () */

	if (cylinderNumber == instance.expectedCylinderNumber) {
		/* save currect cylinder */
		instance.cylinderNumber = cylinderNumber;

		/* smart books says we need to sence knock few degrees after TDC
		 * currently I have no idea how to hook to cylinder TDC in correct way.
		 * So schedule start of integration + knockDetectionWindowStart from fire event
		 * Keep this is mind when setting knockDetectionWindowStart */
		scheduleByAngle(&hardware.startTimer, nowNt,
				engineConfiguration->knockDetectionWindowStart,
				{ startIntegration, &instance });

		scheduleByAngle(&hardware.endTimer, nowNt,
				engineConfiguration->knockDetectionWindowEnd,
				{ endIntegration, &instance });
	} else {
		#if EFI_HIP_9011_DEBUG
			/* out of sync */
			if (instance.expectedCylinderNumber >= 0)
				instance.unsync++;
		#endif
		/* save currect cylinder */
		instance.cylinderNumber = cylinderNumber;
		/* Skip integration, call driver task to prepare for next cylinder */
		instance.state = NOT_READY;
		hip_wake_driver();
	}
}

void hipAdcCallback(float volts) {
	/* we read in digital mode */
	if (instance.adv_mode)
		return;
	if (instance.state == WAITING_FOR_ADC_TO_SKIP) {
		instance.state = WAITING_FOR_RESULT_ADC;
	} else if (instance.state == WAITING_FOR_RESULT_ADC) {
		/* offload calculations to driver thread */
		if (instance.channelIdx < HIP_INPUT_CHANNELS) {
			/* normalize to 0..HIP9011_DIGITAL_OUTPUT_MAX */
			instance.rawValue[instance.channelIdx] =
				volts * HIP9011_DIGITAL_OUTPUT_MAX / HIP9011_ANALOG_OUTPUT_MAX;
		}
		instance.state = NOT_READY;
		hip_wake_driver();
	}
}

static int hip_testAdvMode() {
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

static int hip_init() {
	int ret;

	ret = instance.hw->sendSyncCommand(SET_PRESCALER_CMD(instance.prescaler), NULL);
	if (ret) {
		/* NOTE: hip9011/tpic8101 can be in default or advansed mode at this point
		 * If we supposed not to support advanced mode this is definitely error */
		if (!engineConfiguration->useTpicAdvancedMode)
			return ret;
	}

	/* ...othervice or when no error is reported lets try to switch to advanced mode */
	if (engineConfiguration->useTpicAdvancedMode) {
		/* enable advanced mode */
		ret = instance.hw->sendSyncCommand(SET_ADVANCED_MODE_CMD, NULL);
		if (ret) {
			uint8_t rx;
			/* communication error is detected for default mode...
			 * may be we are in advanced mode already?
			 * Now we dont care for return value */
			instance.hw->sendSyncCommand(SET_ADVANCED_MODE_CMD, &rx);
			if (rx != SET_ADVANCED_MODE_REP) {
				/* this is really a communication problem */
				return ret;
			}
		}

		/* now we should be in advanced mode... if chip supports...
		 * set advanced mode flag now so checkResponse will switch to
		 * advanced mode checkig (not implemented) */
		instance.adv_mode = true;

		ret = hip_testAdvMode();
		if (ret) {
			warning(ObdCode::CUSTOM_OBD_KNOCK_PROCESSOR, "TPIC/HIP does not support advanced mode");
			instance.adv_mode = false;
		}
	}

	#if EFI_HIP_9011_DEBUG
		/* reset error counter now */
		instance.invalidResponsesCount = 0;
	#endif

	instance.state = READY_TO_INTEGRATE;

	return 0;
}

static THD_WORKING_AREA(hipThreadStack, UTILITY_THREAD_STACK_SIZE);

static msg_t hipThread(void *arg) {
	int ret;
	UNUSED(arg);
	chRegSetThreadName("hip9011 worker");

	/* This strange code was here before me.
	 * Not sure why we need it */
#if 0
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

	chThdSleepMilliseconds(100);
#endif

	do {
		/* retry until success */
		ret = hip_init();
		if (ret) {
			warning(ObdCode::CUSTOM_OBD_KNOCK_PROCESSOR, "TPIC/HIP does not respond: %d", ret);
			chThdSleepMilliseconds(10 * 1000);
		}
	} while (ret);

	while (1) {
		msg_t msg;

		/* load new/updated settings */
		instance.handleSettings(Sensor::getOrZero(SensorType::Rpm) DEFINE_PARAM_SUFFIX(PASS_HIP_PARAMS));
		/* in advanced more driver will set channel while reading integrator value */
		if (!instance.adv_mode) {
			/* switch input channel */
			instance.handleChannel(DEFINE_PARAM_SUFFIX(PASS_HIP_PARAMS));
		}
		/* State */
		instance.state = READY_TO_INTEGRATE;

		msg = chSemWaitTimeout(&wake, TIME_INFINITE);
		if (msg == MSG_TIMEOUT) {
			/* ??? */
		} else {
			int rawValue;
			/* check now, before readValueAndHandleChannel did not overwrite expectedCylinderNumber */
			bool correctCylinder = (instance.cylinderNumber == instance.expectedCylinderNumber);

			/* this needs to be called in any case to set proper channel for next cycle */
			if (instance.adv_mode) {
				rawValue = instance.readValueAndHandleChannel(DEFINE_PARAM_SUFFIX(PASS_HIP_PARAMS));

				/* spi communication issue? */
				if (rawValue < 0)
					continue;
			}

			/* check that we know channel for current measurement */
			int idx = instance.channelIdx;
			if (!(idx < HIP_INPUT_CHANNELS))
				continue;

			float knockNormalized = 0.0f;
			float knockVolts = 0.0f;

			/* calculations */
			if (instance.adv_mode) {
				instance.rawValue[idx] = rawValue;
			} else {
				/* get value stored by callback */
				rawValue = instance.rawValue[idx];
			}
			/* convert 10 bit integer value to 0.0 .. 1.0 float */
			knockNormalized = ((float)rawValue) / HIP9011_DIGITAL_OUTPUT_MAX;
			/* convert to magic volts
			 * TODO: remove conversion to volts */
			knockVolts = knockNormalized * HIP9011_ANALOG_OUTPUT_MAX;

			/* Check for correct cylinder/input */
			if (correctCylinder) {
				// TODO: convert knock level to dBv
				engine->module<KnockController>()->onKnockSenseCompleted(instance.cylinderNumber, knockVolts, instance.knockSampleTimestamp);

				#if EFI_HIP_9011_DEBUG
					/* debug */
					normalizedValue[idx] = knockNormalized;
					normalizedValueMax[idx] = std::max(knockNormalized, normalizedValueMax[idx]);
					/* counters */
					instance.samples++;
				#endif
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

void stopHip9011_pins() {
	intHold.deInit();
	Cs.deInit();
#if EFI_PROD_CODE
	spi = NULL;
	hipSpiCfg.ssport = NULL;
#endif
}

void startHip9011_pins() {
	intHold.initPin("hip int/hold", engineConfiguration->hip9011IntHoldPin, engineConfiguration->hip9011IntHoldPinMode);
	intHold.setValue(1);
	Cs.initPin("hip CS", engineConfiguration->hip9011CsPin, engineConfiguration->hip9011CsPinMode);
	Cs.setValue(1);

#if EFI_PROD_CODE
	spi = getSpiDevice(engineConfiguration->hip9011SpiDevice);
	if (spi == NULL) {
		// error already reported
		return;
	}
	hipSpiCfg.ssport = getHwPort("hip", engineConfiguration->hip9011CsPin);
	hipSpiCfg.sspad = getHwPin("hip", engineConfiguration->hip9011CsPin);
#endif /* EFI_PROD_CODE */
}

void initHip9011() {
	if (!engineConfiguration->isHip9011Enabled)
		return;

	startHip9011_pins();

	/* load settings */
	instance.prescaler = engineConfiguration->hip9011PrescalerAndSDO;

	efiPrintf("Starting HIP9011/TPIC8101 driver");

	/* init semaphore */
	chSemObjectInit(&wake, 10);
	chThdCreateStatic(hipThreadStack, sizeof(hipThreadStack), PRIO_HIP9011, (tfunc_t)(void*) hipThread, NULL);

	#if EFI_HIP_9011_DEBUG
		hip_addconsoleActions();
	#endif
}

/*==========================================================================*/
/* Debug functions.															*/
/*==========================================================================*/

#if EFI_HIP_9011_DEBUG

static const char *hip_state_names[] =
{
	"Not ready/calculating",
	"Ready for integration",
	"Integrating",
	"Waiting for first ADC sample",
	"Waiting for second ADC sample"
};

static void showHipInfo() {
	if (!engineConfiguration->isHip9011Enabled) {
		efiPrintf("hip9011 driver not active");
		return;
	}

	efiPrintf("HIP9011: enabled %s state %s",
		boolToString(engineConfiguration->isHip9011Enabled),
		hip_state_names[instance.state]);

	efiPrintf(" Advanced mode: enabled %d used %d",
		engineConfiguration->useTpicAdvancedMode,
		instance.adv_mode);

	efiPrintf(" Input Ch %d (cylinder %d next %d)",
		instance.channelIdx,
		instance.cylinderNumber,
		instance.expectedCylinderNumber);

	efiPrintf(" Cyl bore %.2fmm freq %.2fkHz band idx 0x%x",
		(float)engineConfiguration->cylinderBore,
		instance.getBand(PASS_HIP_PARAMS),
		instance.bandIdx);

	efiPrintf(" Integrator idx 0x%x",
		instance.intergratorIdx);

	efiPrintf(" Gain %.2f idx 0x%x",
		engineConfiguration->hip9011Gain,
		instance.gainIdx);

	efiPrintf(" PaSDO=0x%x",
		instance.prescaler);

	efiPrintf(" IntHold %s (mode 0x%x)",
		hwPortname(engineConfiguration->hip9011IntHoldPin),
		engineConfiguration->hip9011IntHoldPinMode);

	efiPrintf(" Spi %s CS %s (mode 0x%x)",
		getSpi_device_e(engineConfiguration->hip9011SpiDevice),
		hwPortname(engineConfiguration->hip9011CsPin),
		engineConfiguration->hip9011CsPinMode);

#if EFI_PROD_CODE
	printSpiConfig("hip9011", engineConfiguration->hip9011SpiDevice);
#endif /* EFI_PROD_CODE */

	efiPrintf(" SPI: good response %d incorrect response %d",
		instance.correctResponsesCount,
		instance.invalidResponsesCount);

	efiPrintf(" Counters: samples %d overruns %d sync miss %d",
		instance.samples, instance.overrun, instance.unsync);

	efiPrintf(" Window start %.2f end %.2f",
		engineConfiguration->knockDetectionWindowStart,
		engineConfiguration->knockDetectionWindowEnd);

	if (!instance.adv_mode) {
		efiPrintf(" Adc input %d (%.2f V)",
			(int)engineConfiguration->hipOutputChannel,
			getVoltage("hipinfo", engineConfiguration->hipOutputChannel));
	}

	for (int i = 0; i < HIP_INPUT_CHANNELS; i++) {
		efiPrintf("  input[%d] %d -> %.3f (max %.3f)",
			i,
			instance.rawValue[i],
			normalizedValue[i],
			normalizedValueMax[i]);
		normalizedValueMax[i] = 0.0;
	}
}

static void setPrescalerAndSDO(int value) {
	engineConfiguration->hip9011PrescalerAndSDO = value;
}

static void setHipGain(float value) {
	engineConfiguration->hip9011Gain = value;
	showHipInfo();
}

static void hip_addconsoleActions() {
	addConsoleAction("hipinfo", showHipInfo);
	addConsoleActionF("set_gain", setHipGain);
	addConsoleActionI("set_hip_prescalerandsdo", setPrescalerAndSDO);
}

#endif /* EFI_HIP_9011_DEBUG */

#endif /* EFI_HIP_9011 */
