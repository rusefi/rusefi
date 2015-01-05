/**
 * @file	HIP9011.cpp
 * @brief	HIP9011/TPIC8101 driver
 *
 *	pin1	VDD
 *	pin2	GND
 *
 *	pin8	Chip Select - CS
 *	pin11	Slave Data Out - MISO-
 *	pin12	Slave Data In - MOSI
 *	pin13	SPI clock - SCLK
 *
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
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "engine.h"
#include "settings.h"
#include "pin_repository.h"
#include "hardware.h"
#include "rpm_calculator.h"
#include "trigger_central.h"
#include "hip9011_lookup.h"

#if EFI_HIP_9011 || defined(__DOXYGEN__)

#define HIP_DEBUG FALSE

extern pin_output_mode_e DEFAULT_OUTPUT;

/**
 * band index is only send to HIP chip on startup
 */
static int bandIndex;
static int currentGainIndex;
static int currentIntergratorIndex = -1;

/**
 * Int/Hold pin is controlled from scheduler callbacks which are set according to current RPM
 *
 * The following flags make sure that we only have SPI communication while not integrating
 */
static bool_t isIntegrating = false;
static bool_t needToSendSpiCommand = false;

static scheduling_s startTimer[2];
static scheduling_s endTimer[2];

static Logging logger;

static THD_WORKING_AREA(htThreadStack, UTILITY_THREAD_STACK_SIZE);

// SPI_CR1_BR_1 // 5MHz
// SPI_CR1_CPHA Clock Phase
// todo: nicer method which would mention SPI speed explicitly?

static SPIConfig spicfg = { NULL,
/* HW dependent part.*/
NULL, 0,
SPI_CR1_MSTR |
//SPI_CR1_BR_1 // 5MHz
		SPI_CR1_CPHA | SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2 };

static unsigned char tx_buff[1];
static unsigned char rx_buff[1];

#define SPI_SYNCHRONOUS(value) \
	spiSelect(driver); \
	tx_buff[0] = value; \
	spiExchange(driver, 1, tx_buff, rx_buff); \
	spiUnselect(driver);

// todo: make this configurable
static SPIDriver *driver = &SPID2;

static msg_t ivThread(int param) {
	chRegSetThreadName("HIP");

	while (true) {
		chThdSleepMilliseconds(10);

		// engine->rpmCalculator.rpmValue

//		int newValue = INTEGRATOR_INDEX;
//		if (newValue != intergratorIndex) {
//			intergratorIndex = newValue;
//			// todo: send new value, be sure to use non-synchnonious approach!
//
//		}
		// todo: move this into the end callback


//		scheduleMsg(&logger, "poking HIP=%d", counter++);

		spiSelect(driver);

//		// '0' for 4MHz
//		tx_buff[0] = SET_PRESCALER_CMD + 0 + 2;
//		spiExchange(driver, 1, tx_buff, rx_buff);
//
//		// '0' for channel #1
//		tx_buff[0] = SET_CHANNEL_CMD + 0;
//		spiExchange(driver, 1, tx_buff, rx_buff);
//
//		// band index depends on cylinder bore
//		tx_buff[0] = SET_BAND_PASS_CMD + bandIndex;
//		spiExchange(driver, 1, tx_buff, rx_buff);
//
//		// todo
//		tx_buff[0] = SET_GAIN_CMD + 41;
//		spiExchange(driver, 1, tx_buff, rx_buff);
//
//		tx_buff[0] = SET_ADVANCED_MODE;
//		spiExchange(driver, 1, tx_buff, rx_buff);

// BAND_PASS_CMD
		tx_buff[0] = 0x0 | (40 & 0x3F);
		spiExchange(driver, 1, tx_buff, rx_buff);

		// Set the gain 0b10000000
		tx_buff[0] = 0x80 | (49 & 0x3F);
		spiExchange(driver, 1, tx_buff, rx_buff);

		// Set the integration time constant 0b11000000
		tx_buff[0] = 0xC0 | (31 & 0x1F);
		spiExchange(driver, 1, tx_buff, rx_buff);

		// SET_ADVANCED_MODE 0b01110001
		tx_buff[0] = 0x71;
		spiExchange(driver, 1, tx_buff, rx_buff);

		spiUnselect(driver);

	}
#if defined __GNUC__
	return 0;
#endif

}

EXTERN_ENGINE
;

static void showHipInfo(void) {
	printSpiState(&logger, boardConfiguration);
	scheduleMsg(&logger, "bore=%f freq=%f", engineConfiguration->cylinderBore, BAND(engineConfiguration->cylinderBore));

	scheduleMsg(&logger, "band_index=%d gain_index=%d", bandIndex, GAIN_INDEX(boardConfiguration->hip9011Gain));

	scheduleMsg(&logger, "integrator index=%d", currentIntergratorIndex);

	scheduleMsg(&logger, "spi= int=%s CS=%s", hwPortname(boardConfiguration->hip9011IntHoldPin),
			hwPortname(boardConfiguration->hip9011CsPin));
}

void setHip9011FrankensoPinout(void) {
	/**
	 * SPI on PB13/14/15
	 */
	boardConfiguration->isHip9011Enabled = true;
	boardConfiguration->hip9011CsPin = GPIOD_11;
	boardConfiguration->hip9011IntHoldPin = GPIOB_11;
	boardConfiguration->is_enabled_spi_2 = true;
}

static void startIntegration(void) {
	if (!needToSendSpiCommand) {
		/**
		 * SPI communication is only allowed while not integrading, so we initiate the exchange
		 * once we are done integrating
		 */
		isIntegrating = false;
		turnPinHigh(HIP9011_INT_HOLD);
	}
}

static void endIntegration(void) {
	/**
	 * isIntegrating could be 'false' if an SPI command was pending thus we did not integrate during this
	 * engine cycle
	 */
	if (isIntegrating) {
		turnPinLow(HIP9011_INT_HOLD);
		isIntegrating = false;
	}
}

/**
 * Shaft Position callback used to start or finish HIP integration
 */
static void intHoldCallback(trigger_event_e ckpEventType, uint32_t index DECLARE_ENGINE_PARAMETER_S) {
	// this callback is invoked on interrupt thread

	if (index != 0)
		return;

	int rpm = engine->rpmCalculator.rpmValue;
	if (!isValidRpm(rpm))
		return;

	int structIndex = getRevolutionCounter() % 2;
	// todo: schedule this based on closest trigger event, same as ignition works
	scheduleByAngle(rpm, &startTimer[structIndex], engineConfiguration->knockDetectionWindowStart,
			(schfunc_t) &startIntegration, NULL);
	scheduleByAngle(rpm, &endTimer[structIndex], engineConfiguration->knockDetectionWindowEnd,
			(schfunc_t) &endIntegration,
			NULL);
}

static void setGain(float value) {
	boardConfiguration->hip9011Gain = value;
	showHipInfo();
}

void initHip9011(void) {
	if (!boardConfiguration->isHip9011Enabled)
		return;
	initLogging(&logger, "HIP driver");

//	prepa engineConfiguration->knockDetectionWindowEnd	- engineConfiguration->knockDetectionWindowStart

//	driver = getSpiDevice(boardConfiguration->digitalPotentiometerSpiDevice);

	spicfg.ssport = getHwPort(boardConfiguration->hip9011CsPin);
	spicfg.sspad = getHwPin(boardConfiguration->hip9011CsPin);

	outputPinRegisterExt2("hip int/hold", HIP9011_INT_HOLD, boardConfiguration->hip9011IntHoldPin, &DEFAULT_OUTPUT);
	outputPinRegisterExt2("hip CS", SPI_CS_HIP9011, boardConfiguration->hip9011CsPin, &DEFAULT_OUTPUT);

	scheduleMsg(&logger, "Starting HIP9011/TPIC8101 driver");
	spiStart(driver, &spicfg);

	chThdCreateStatic(htThreadStack, sizeof(htThreadStack), NORMALPRIO,
			(tfunc_t) ivThread, NULL);
//#else
//	/**
//	 * for runtime we are re-starting SPI in non-synchronous mode
//	 */
//	spiStop(driver);
//	// todo spicfg.end_cb = spiEndCallback;
//	spiStart(driver, &spicfg);
//#endif /* HIP_DEBUG */

	bandIndex = getHip9011BandIndex(engineConfiguration->cylinderBore);

	addTriggerEventListener(&intHoldCallback, "DD int/hold", engine);

	// MISO PB14
//	palSetPadMode(GPIOB, 14, PAL_MODE_ALTERNATE(EFI_SPI2_AF) | PAL_STM32_PUDR_PULLUP);
	// MOSI PB15
//	palSetPadMode(GPIOB, 15, PAL_MODE_ALTERNATE(EFI_SPI2_AF) | PAL_STM32_OTYPE_OPENDRAIN);

	addConsoleAction("hipinfo", showHipInfo);
	addConsoleActionF("set_gain", setGain);
}

#endif
