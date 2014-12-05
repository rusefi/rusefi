/**
 * @file	HIP9011.c
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
 *	SPI frequency: 5MHz
 *
 * @date Nov 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "engine.h"

#if EFI_HIP_9011

#define HIP9011_CS_PORT GPIOE
#define HIP9011_CS_PIN 11

static Logging logger;

static THD_WORKING_AREA(htThreadStack, UTILITY_THREAD_STACK_SIZE);

static int callbackc = 0;

static void spiCallback(SPIDriver *spip) {
	spiUnselectI(spip);

	scheduleMsg(&logger, "spiCallback HIP=%d", callbackc++);

}

// SPI_CR1_BR_1 // 5MHz

static const SPIConfig spicfg = { spiCallback,
/* HW dependent part.*/
HIP9011_CS_PORT,
HIP9011_CS_PIN,
//SPI_CR1_MSTR |
//SPI_CR1_BR_1 // 5MHz
		SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2 };

static unsigned char tx_buff[8];
static unsigned char rx_buff[8];

static SPIDriver *driver = &SPID2;

// 0b01110001
#define HIP_ADVANCED_MODE 0x71

static msg_t ivThread(int param) {
	chRegSetThreadName("HIP");

	int counter = 0;

//	tx_buff[0] = 0b11100001;

	tx_buff[0] = HIP_ADVANCED_MODE;

	tx_buff[4] = 0xF8; // 0b11111000;

	while (TRUE) {
		chThdSleepMilliseconds(10);

		scheduleMsg(&logger, "poking HIP=%d", counter++);

		spiSelect(driver);

		spiStartExchange(driver, 8, tx_buff, rx_buff);
//		spiUnselect(driver);

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
static const int integratorValues[INT_TIME_COUNT] = { 40, 45, 50, 55, 60, 65, 70, 75, 80, 90, 100, 110, 120, 130, 140,
		150, 160, 180, 200, 220, 240, 260, 280, 300, 320, 360, 400, 440, 480, 520, 560, 600 };

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
#define DESIRED_OUTPUT_VALUE 2.2f
static void prepareRpmLookup(engine_configuration_s *engineConfiguration) {
	for (int i = 0; i < INT_TIME_COUNT; i++) {
		float windowWidthMult = (engineConfiguration->knockDetectionWindowEnd
				- engineConfiguration->knockDetectionWindowStart) / 360.0f;
		// '60000000' because revolutions per MINUTE in uS conversion

		rpmLookup[i] = 60000000.0f / (integratorValues[i] * 2 * PIF * DESIRED_OUTPUT_VALUE * windowWidthMult);
	}
}

void initHip9011(void) {
	if (!boardConfiguration->isHip9011Enabled)
		return;
	initLogging(&logger, "HIP driver");

	print("Starting HIP9011/TPIC8101 driver\r\n");
	spiStart(driver, &spicfg);

	chThdCreateStatic(htThreadStack, sizeof(htThreadStack), NORMALPRIO, (tfunc_t) ivThread, NULL);
}

#endif
