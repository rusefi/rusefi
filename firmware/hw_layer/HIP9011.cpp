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
 * @author Andrey Belomutskiy, (c) 2012-2018
 * @Spilly
 */

#include "main.h"
#include "engine.h"
#include "settings.h"
#include "pin_repository.h"
#include "hardware.h"
#include "rpm_calculator.h"
#include "trigger_central.h"
#include "hip9011_lookup.h"
#include "HIP9011.h"
#include "adc_inputs.h"
#include "efilib2.h"
#include "engine_controller.h"

#if EFI_HIP_9011 || defined(__DOXYGEN__)

static NamedOutputPin intHold(HIP_NAME);

extern uint32_t lastExecutionCount;
extern EnginePins enginePins;

uint32_t hipLastExecutionCount;

/**
 * band index is only send to HIP chip on startup
 */
static int currentBandIndex;
static int currentGainIndex = -1;
static int currentIntergratorIndex = -1;
static int settingUpdateCount = 0;
static int totalKnockEventsCount = 0;
static int currentPrescaler;
static float hipValueMax = 0;

static unsigned char tx_buff[1];
static unsigned char rx_buff[1];
int correctResponsesCount = 0;
int invalidResponsesCount = 0;
static char pinNameBuffer[16];
static float currentAngleWindowWidth;

/**
 * Int/Hold pin is controlled from scheduler call-backs which are set according to current RPM
 *
 * The following state makes sure that we only have SPI communication while not integrating and that we take
 * a good ADC reading after integrating.
 *
 * Once integration window is over, we wait for the 2nd ADC callback and then initiate SPI communication if needed
 *
 * hipOutput should be set to used FAST adc device
 */
static hip_state_e state = NOT_READY;

static scheduling_s startTimer[2];
static scheduling_s endTimer[2];

static Logging *logger;

// SPI_CR1_BR_1 // 5MHz
// SPI_CR1_CPHA Clock Phase
// todo: nicer method which would mention SPI speed explicitly?

static SPIConfig hipSpiCfg = { NULL,
/* HW dependent part.*/
NULL, 0,
SPI_CR1_MSTR |
//SPI_CR1_BR_1 // 5MHz
		SPI_CR1_CPHA | SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2 };

static void checkResponse(void) {
	if (tx_buff[0] == rx_buff[0]) {
		correctResponsesCount++;
	} else {
		invalidResponsesCount++;
	}
}

// this macro is only used on startup
#define SPI_SYNCHRONOUS(value) \
	spiSelect(driver); \
	tx_buff[0] = value; \
	spiExchange(driver, 1, tx_buff, rx_buff); \
	spiUnselect(driver); \
	checkResponse();

static SPIDriver *driver;

EXTERN_ENGINE
;

static float getBand(void) {
	return engineConfiguration->knockBandCustom == 0 ?
			BAND(engineConfiguration->cylinderBore) : engineConfiguration->knockBandCustom;
}

static char hipPinNameBuffer[16];

static void showHipInfo(void) {
	if (!boardConfiguration->isHip9011Enabled) {
		scheduleMsg(logger, "hip9011 driver not active");
		return;
	}

	printSpiState(logger, boardConfiguration);
	scheduleMsg(logger, "enabled=%s state=%d bore=%.2fmm freq=%.2fkHz PaSDO=%d",
			boolToString(boardConfiguration->isHip9011Enabled),
			state,
			engineConfiguration->cylinderBore, getBand(),
			engineConfiguration->hip9011PrescalerAndSDO);

	char *outputName = getPinNameByAdcChannel("hip", engineConfiguration->hipOutputChannel, hipPinNameBuffer);

	scheduleMsg(logger, "band_index=%d gain %.2f/index=%d output=%s", currentBandIndex, boardConfiguration->hip9011Gain, currentGainIndex,
			outputName);
	scheduleMsg(logger, "integrator index=%d knockVThreshold=%.2f knockCount=%d maxKnockSubDeg=%.2f",
	            currentIntergratorIndex, engineConfiguration->knockVThreshold,
	            engine->knockCount, engineConfiguration->maxKnockSubDeg);

	const char * msg = invalidResponsesCount > 0 ? "NOT GOOD" : "ok";
	scheduleMsg(logger, "spi=%s IntHold@%s/%d response count=%d incorrect response=%d %s",
			getSpi_device_e(engineConfiguration->hip9011SpiDevice),
			hwPortname(boardConfiguration->hip9011IntHoldPin),
			boardConfiguration->hip9011IntHoldPinMode,
			correctResponsesCount, invalidResponsesCount,
			msg);
	scheduleMsg(logger, "CS@%s updateCount=%d", hwPortname(boardConfiguration->hip9011CsPin), settingUpdateCount);

	scheduleMsg(logger, "hip %.2fv/last=%.2f@%s/max=%.2f adv=%d",
			engine->knockVolts,
			getVoltage("hipinfo", engineConfiguration->hipOutputChannel),
			getPinNameByAdcChannel("hip", engineConfiguration->hipOutputChannel, pinNameBuffer),
			hipValueMax,
			boardConfiguration->useTpicAdvancedMode);
	scheduleMsg(logger, "mosi=%s", hwPortname(getMosiPin(engineConfiguration->hip9011SpiDevice)));
	scheduleMsg(logger, "miso=%s", hwPortname(getMisoPin(engineConfiguration->hip9011SpiDevice)));
	scheduleMsg(logger, "sck=%s", hwPortname(getSckPin(engineConfiguration->hip9011SpiDevice)));

	scheduleMsg(logger, "start %.2f end %.2f", engineConfiguration->knockDetectionWindowStart,
			engineConfiguration->knockDetectionWindowEnd);

	hipValueMax = 0;
	engine->printKnockState();
}

void setHip9011FrankensoPinout(void) {
	/**
	 * SPI on PB13/14/15
	 */
	//	boardConfiguration->hip9011CsPin = GPIOD_0; // rev 0.1

	boardConfiguration->isHip9011Enabled = true;
	boardConfiguration->hip9011CsPin = GPIOB_0; // rev 0.4
	boardConfiguration->hip9011CsPinMode = OM_OPENDRAIN;
	engineConfiguration->hip9011PrescalerAndSDO = 6; // 8MHz chip
	boardConfiguration->hip9011IntHoldPin = GPIOB_11;
	boardConfiguration->hip9011IntHoldPinMode = OM_OPENDRAIN;
	boardConfiguration->is_enabled_spi_2 = true;
	// todo: convert this to rusEfi, hardware-independent enum
	engineConfiguration->spi2SckMode = PAL_STM32_OTYPE_OPENDRAIN; // 4
	engineConfiguration->spi2MosiMode = PAL_STM32_OTYPE_OPENDRAIN; // 4
	engineConfiguration->spi2MisoMode = PAL_STM32_PUPDR_PULLUP; // 32

	boardConfiguration->hip9011Gain = 1;
	engineConfiguration->knockVThreshold = 4;
	engineConfiguration->maxKnockSubDeg = 20;


	if (!boardConfiguration->useTpicAdvancedMode) {
	    engineConfiguration->hipOutputChannel = EFI_ADC_10; // PC0
	}
}

static void startIntegration(void) {
	if (state == READY_TO_INTEGRATE) {
		/**
		 * SPI communication is only allowed while not integrating, so we postpone the exchange
		 * until we are done integrating
		 */
		state = IS_INTEGRATING;
		intHold.setHigh();
	}
}

static void endIntegration(void) {
	/**
	 * isIntegrating could be 'false' if an SPI command was pending thus we did not integrate during this
	 * engine cycle
	 */
	if (state == IS_INTEGRATING) {
		intHold.setLow();
		state = WAITING_FOR_ADC_TO_SKIP;
	}
}

/**
 * Shaft Position callback used to start or finish HIP integration
 */
static void intHoldCallback(trigger_event_e ckpEventType, uint32_t index DECLARE_ENGINE_PARAMETER_SUFFIX) {
	// this callback is invoked on interrupt thread
	if (index != 0)
		return;
	engine->m.beforeHipCb = GET_TIMESTAMP();

	int rpm = engine->rpmCalculator.rpmValue;
	if (!isValidRpm(rpm))
		return;

	int structIndex = getRevolutionCounter() % 2;
	// todo: schedule this based on closest trigger event, same as ignition works
	scheduleByAngle(rpm, &startTimer[structIndex], engineConfiguration->knockDetectionWindowStart,
			(schfunc_t) &startIntegration, NULL, &engine->rpmCalculator);
	hipLastExecutionCount = lastExecutionCount;
	scheduleByAngle(rpm, &endTimer[structIndex], engineConfiguration->knockDetectionWindowEnd,
			(schfunc_t) &endIntegration,
			NULL, &engine->rpmCalculator);
	engine->m.hipCbTime = GET_TIMESTAMP() - engine->m.beforeHipCb;
}

void setMaxKnockSubDeg(int value) {
    engineConfiguration->maxKnockSubDeg = value;
    showHipInfo();
}

void setKnockThresh(float value) {
    engineConfiguration->knockVThreshold = value;
    showHipInfo();
}

void setPrescalerAndSDO(int value) {
	engineConfiguration->hip9011PrescalerAndSDO = value;
}

void setHipBand(float value) {
	engineConfiguration->knockBandCustom = value;
	showHipInfo();
}

void setHipGain(float value) {
	boardConfiguration->hip9011Gain = value;
	showHipInfo();
}

/**
 * this is the end of the non-synchronous exchange
 */
static void endOfSpiExchange(SPIDriver *spip) {
	spiUnselectI(driver);
	state = READY_TO_INTEGRATE;
	checkResponse();
}

static int getBandIndex(void) {
	float freq = getBand();
	return getHip9011BandIndex(freq);
}

void hipAdcCallback(adcsample_t value) {
	if (state == WAITING_FOR_ADC_TO_SKIP) {
		state = WAITING_FOR_RESULT_ADC;
	} else if (state == WAITING_FOR_RESULT_ADC) {
		engine->knockVolts = adcToVoltsDivided(value);
		hipValueMax = maxF(engine->knockVolts, hipValueMax);
		engine->knockLogic(engine->knockVolts);

		float angleWindowWidth =
		engineConfiguration->knockDetectionWindowEnd - engineConfiguration->knockDetectionWindowStart;

		if (angleWindowWidth != currentAngleWindowWidth) {
			currentAngleWindowWidth = angleWindowWidth;
		prepareHip9011RpmLookup(currentAngleWindowWidth);
		}

		int integratorIndex = getIntegrationIndexByRpm(engine->rpmCalculator.rpmValue);
		int gainIndex = getHip9011GainIndex(boardConfiguration->hip9011Gain);
		int bandIndex = getBandIndex();
		int prescalerIndex = engineConfiguration->hip9011PrescalerAndSDO;


		if (currentGainIndex != gainIndex) {
			currentGainIndex = gainIndex;
			tx_buff[0] = SET_GAIN_CMD + gainIndex;

			state = IS_SENDING_SPI_COMMAND;
			spiSelectI(driver);
			spiStartExchangeI(driver, 1, tx_buff, rx_buff);
		} else if (currentIntergratorIndex != integratorIndex) {
			currentIntergratorIndex = integratorIndex;
			tx_buff[0] = SET_INTEGRATOR_CMD + integratorIndex;

			state = IS_SENDING_SPI_COMMAND;
			spiSelectI(driver);
			spiStartExchangeI(driver, 1, tx_buff, rx_buff);
		} else if (currentBandIndex != bandIndex) {
			currentBandIndex = bandIndex;
			tx_buff[0] = SET_BAND_PASS_CMD + bandIndex;

			state = IS_SENDING_SPI_COMMAND;
			spiSelectI(driver);
			spiStartExchangeI(driver, 1, tx_buff, rx_buff);
		} else if (currentPrescaler != prescalerIndex) {
			currentPrescaler = prescalerIndex;
			tx_buff[0] = SET_PRESCALER_CMD + prescalerIndex;

			state = IS_SENDING_SPI_COMMAND;
			spiSelectI(driver);
			spiStartExchangeI(driver, 1, tx_buff, rx_buff);
		} else {
			state = READY_TO_INTEGRATE;
		}
	}
}
static bool needToInit = true;

static void hipStartupCode(void) {
//	D[4:1] = 0000 : 4 MHz
//	D[4:1] = 0001 : 5 MHz
//	D[4:1] = 0010 : 6 MHz
//	D[4:1] = 0011 ; 8 MHz
//	D[4:1] = 0100 ; 10 MHz
//	D[4:1] = 0101 ; 12 MHz
//	D[4:1] = 0110 : 16 MHz
//	D[4:1] = 0111 : 20 MHz
//	D[4:1] = 1000 : 24 MHz


// 0 for 4MHz
// 6 for 8 MHz
	currentPrescaler = engineConfiguration->hip9011PrescalerAndSDO;
	SPI_SYNCHRONOUS(SET_PRESCALER_CMD + currentPrescaler);

	chThdSleepMilliseconds(10);

	// '0' for channel #1
	SPI_SYNCHRONOUS(SET_CHANNEL_CMD + 0);

	chThdSleepMilliseconds(10);

	// band index depends on cylinder bore
	SPI_SYNCHRONOUS(SET_BAND_PASS_CMD + currentBandIndex);

	chThdSleepMilliseconds(10);

	if (correctResponsesCount == 0) {
		warning(CUSTOM_OBD_KNOCK_PROCESSOR, "TPIC/HIP does not respond");
	}

	if (boardConfiguration->useTpicAdvancedMode) {
		// enable advanced mode for digital integrator output
		SPI_SYNCHRONOUS(SET_ADVANCED_MODE);

    	chThdSleepMilliseconds(10);
	}

	/**
	 * Let's restart SPI to switch it from synchronous mode into
	 * asynchronous mode
	 */
	spiStop(driver);
	hipSpiCfg.end_cb = endOfSpiExchange;
	spiStart(driver, &hipSpiCfg);
	state = READY_TO_INTEGRATE;
}

static THD_WORKING_AREA(hipTreadStack, UTILITY_THREAD_STACK_SIZE);

static msg_t hipThread(void *arg) {
	chRegSetThreadName("hip9011 init");

	// some time to let the hardware start
	enginePins.hipCs.setValue(true);
	chThdSleepMilliseconds(100);
	enginePins.hipCs.setValue(false);
	chThdSleepMilliseconds(100);
	enginePins.hipCs.setValue(true);

	while (true) {
		chThdSleepMilliseconds(100);

		if (needToInit) {
			hipStartupCode();
			needToInit = false;
		}
	}
	return -1;
}

void initHip9011(Logging *sharedLogger) {
	logger = sharedLogger;
	addConsoleAction("hipinfo", showHipInfo);
	if (!boardConfiguration->isHip9011Enabled)
		return;


	currentAngleWindowWidth =
	engineConfiguration->knockDetectionWindowEnd - engineConfiguration->knockDetectionWindowStart;

	prepareHip9011RpmLookup(currentAngleWindowWidth);

	driver = getSpiDevice(engineConfiguration->hip9011SpiDevice);

	hipSpiCfg.ssport = getHwPort("hip", boardConfiguration->hip9011CsPin);
	hipSpiCfg.sspad = getHwPin("hip", boardConfiguration->hip9011CsPin);

	intHold.initPin("hip int/hold", boardConfiguration->hip9011IntHoldPin,
			&boardConfiguration->hip9011IntHoldPinMode);
	enginePins.hipCs.initPin("hip CS", boardConfiguration->hip9011CsPin,
			&boardConfiguration->hip9011CsPinMode);

	scheduleMsg(logger, "Starting HIP9011/TPIC8101 driver");
	spiStart(driver, &hipSpiCfg);

	currentBandIndex = getBandIndex();

	/**
	 * this engine cycle callback would be scheduling actual integration start and end callbacks
	 */
	addTriggerEventListener(&intHoldCallback, "DD int/hold", engine);

	// MISO PB14
//	palSetPadMode(GPIOB, 14, PAL_MODE_ALTERNATE(EFI_SPI2_AF) | PAL_STM32_PUDR_PULLUP);
	// MOSI PB15
//	palSetPadMode(GPIOB, 15, PAL_MODE_ALTERNATE(EFI_SPI2_AF) | PAL_STM32_OTYPE_OPENDRAIN);

	addConsoleActionF("set_gain", setHipGain);
	addConsoleActionF("set_band", setHipBand);
	addConsoleActionI("set_hip_prescalerandsdo", setPrescalerAndSDO);
    addConsoleActionF("set_knock_threshold", setKnockThresh);
    addConsoleActionI("set_max_knock_sub_deg", setMaxKnockSubDeg);
	chThdCreateStatic(hipTreadStack, sizeof(hipTreadStack), NORMALPRIO, (tfunc_t) hipThread, NULL);
}

#endif /* EFI_HIP_9011 */
