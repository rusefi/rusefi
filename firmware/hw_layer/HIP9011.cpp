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
 * http://www.ti.com/lit/ds/symlink/tpic8101.pdf
 * http://www.intersil.com/content/dam/Intersil/documents/hip9/hip9011.pdf
 * http://www.intersil.com/content/dam/Intersil/documents/an97/an9770.pdf
 * http://e2e.ti.com/cfs-file/__key/telligent-evolution-components-attachments/00-26-01-00-00-42-36-40/TPIC8101-Training.pdf
 *
 *	SPI frequency: 5MHz max
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

#if EFI_HIP_9011 || defined(__DOXYGEN__)

#define HIP_DEBUG FALSE

extern pin_output_mode_e DEFAULT_OUTPUT;

static int bandIndex;
static int intergratorIndex = -1;
static bool_t isHip9011Busy = false;

static scheduling_s startTimer[2];
static scheduling_s endTimer[2];

// 0b01000000
#define SET_PRESCALER_CMD 0x40
// 0b11100000
#define SET_CHANNEL_CMD 0xE0
// 0b00000000
#define SET_BAND_PASS_CMD 0x0

// 0b10000000
#define SET_GAIN_CMD 0x80

// 0b01110001
#define SET_ADVANCED_MODE 0x71

static Logging logger;

#if HIP_DEBUG
static THD_WORKING_AREA(htThreadStack, UTILITY_THREAD_STACK_SIZE);
#endif

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
		tx_buff[0] = 0b00000000 | (40 & 0x3F);
		spiExchange(driver, 1, tx_buff, rx_buff);

		// Set the gain
		tx_buff[0] = 0b10000000 | (49 & 0x3F);
		spiExchange(driver, 1, tx_buff, rx_buff);

		// Set the integration time constant
		tx_buff[0] = 0b11000000 | (31 & 0x1F);
		spiExchange(driver, 1, tx_buff, rx_buff);

		// SET_ADVANCED_MODE
		tx_buff[0] = 0b01110001;
		spiExchange(driver, 1, tx_buff, rx_buff);

		spiUnselect(driver);

	}
#if defined __GNUC__
	return 0;
#endif
}

EXTERN_ENGINE
;

#define INT_TIME_COUNT 32

/**
 * These are HIP9011 magic values - integrator time constants in uS
 */
static const int integratorValues[INT_TIME_COUNT] = { 40, 45, 50, 55, 60, 65,
		70, 75, 80, 90, 100, 110, 120, 130, 140, 150, 160, 180, 200, 220, 240,
		260, 280, 300, 320, 360, 400, 440, 480, 520, 560, 600 };

#define BAND_LOOKUP_SIZE 64

static const float bandFreqLookup[BAND_LOOKUP_SIZE] = { 1.22, 1.26, 1.31, 1.35,
		1.4, 1.45, 1.51, 1.57, 1.63, 1.71, 1.78, 1.87, 1.96, 2.07, 2.18, 2.31,
		2.46, 2.54, 2.62, 2.71, 2.81, 2.92, 3.03, 3.15, 3.28, 3.43, 3.59, 3.76,
		3.95, 4.16, 4.39, 4.66, 4.95, 5.12, 5.29, 5.48, 5.68, 5.9, 6.12, 6.37,
		6.64, 6.94, 7.27, 7.63, 8.02, 8.46, 8.95, 9.5, 10.12, 10.46, 10.83,
		11.22, 11.65, 12.1, 12.6, 13.14, 13.72, 14.36, 15.07, 15.84, 16.71,
		17.67, 18.76, 19.98 };

#define PIF 3.14159f

static float rpmLookup[INT_TIME_COUNT];

/**
 * 'TC is typically TINT/(2*Pi*VOUT)'
 * Knock Sensor Training TPIC8101, page 24
 *
 * We know the set of possible integration times, we know the knock detection window width
 *
 * 2.2 volts should
 *
 */
#define DESIRED_OUTPUT_VALUE 5.0f
static void prepareRpmLookup(engine_configuration_s *engineConfiguration) {
	for (int i = 0; i < INT_TIME_COUNT; i++) {
		float windowWidthMult = (engineConfiguration->knockDetectionWindowEnd
				- engineConfiguration->knockDetectionWindowStart) / 360.0f;
		// '60000000' because revolutions per MINUTE in uS conversion

		rpmLookup[i] = 60000000.0f
				/ (integratorValues[i] * 2 * PIF * DESIRED_OUTPUT_VALUE
						* windowWidthMult);
	}
}

#define BAND(bore) (900 / (PIF * (bore) / 2))

#define INTEGRATOR_INDEX findIndex(rpmLookup, INT_TIME_COUNT, engine->rpmCalculator.rpmValue)

static void showHipInfo(void) {
	printSpiState(&logger, boardConfiguration);
	scheduleMsg(&logger, "bore=%f freq=%f", engineConfiguration->cylinderBore,
			BAND(engineConfiguration->cylinderBore));

	scheduleMsg(&logger, "band index=%d", bandIndex);

	scheduleMsg(&logger, "integrator index=%d", INTEGRATOR_INDEX);

	scheduleMsg(&logger, "spi= int=%s CS=%s",
			hwPortname(boardConfiguration->hip9011IntHoldPin),
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
	if(isHip9011Busy)
		return;


	turnPinHigh(HIP9011_INT_HOLD);

}

static void endIntegration(void) {

	turnPinLow(HIP9011_INT_HOLD);
	/**
	 * SPI communication is only allowed while not integrading, so we initiate the exchange
	 * once we are done inregratng
	 */

	isHip9011Busy = true;

	int newValue = INTEGRATOR_INDEX;
	if (newValue != intergratorIndex) {
		intergratorIndex = newValue;
		// todo: send new value, be sure to use non-synchnonious approach!

	}
	// todo: move this into the end callback
	isHip9011Busy = false;

}

/**
 * Shaft Position callback used to start or finish HIP integration
 */
static void intHoldCallback(trigger_event_e ckpEventType,
		uint32_t index DECLARE_ENGINE_PARAMETER_S) {
	// this callback is invoked on interrupt thread

	if (index != 0)
		return;

	int rpm = engine->rpmCalculator.rpmValue;
	if (!isValidRpm(rpm))
		return;

	int structIndex = getRevolutionCounter() % 2;
	// todo: schedule this based on closest trigger event, same as ignition works
	scheduleByAngle(rpm, &startTimer[structIndex],
			engineConfiguration->knockDetectionWindowStart,
			(schfunc_t) &startIntegration, NULL);
	scheduleByAngle(rpm, &endTimer[structIndex],
			engineConfiguration->knockDetectionWindowEnd,
			(schfunc_t) &endIntegration,
			NULL);
}

void initHip9011(void) {
	if (!boardConfiguration->isHip9011Enabled)
		return;
	initLogging(&logger, "HIP driver");

//	driver = getSpiDevice(boardConfiguration->digitalPotentiometerSpiDevice);

	spicfg.ssport = getHwPort(boardConfiguration->hip9011CsPin);
	spicfg.sspad = getHwPin(boardConfiguration->hip9011CsPin);


	outputPinRegisterExt2("hip int/hold", HIP9011_INT_HOLD,
			boardConfiguration->hip9011IntHoldPin, &DEFAULT_OUTPUT);
	outputPinRegisterExt2("hip CS", SPI_CS_HIP9011,
			boardConfiguration->hip9011CsPin, &DEFAULT_OUTPUT);

	scheduleMsg(&logger, "Starting HIP9011/TPIC8101 driver");
	spiStart(driver, &spicfg);
	/**
	 * Here we initialize the chip in synchronous mode
	 */



#if HIP_DEBUG
	chThdCreateStatic(htThreadStack, sizeof(htThreadStack), NORMALPRIO,
			(tfunc_t) ivThread, NULL);
#else
	/**
	 * for runtime we are re-starting SPI in non-synchronous mode
	 */
	spiStop(driver);
	// todo spicfg.end_cb = spiEndCallback;
	spiStart(driver, &spicfg);



#endif

	bandIndex = findIndex(bandFreqLookup, BAND_LOOKUP_SIZE,
			BAND(engineConfiguration->cylinderBore));

	addTriggerEventListener(&intHoldCallback, "DD int/hold", engine);

	// MISO PB14
//	palSetPadMode(GPIOB, 14, PAL_MODE_ALTERNATE(EFI_SPI2_AF) | PAL_STM32_PUDR_PULLUP);
	// MOSI PB15
//	palSetPadMode(GPIOB, 15, PAL_MODE_ALTERNATE(EFI_SPI2_AF) | PAL_STM32_OTYPE_OPENDRAIN);

	addConsoleAction("hipinfo", showHipInfo);
}

#endif
