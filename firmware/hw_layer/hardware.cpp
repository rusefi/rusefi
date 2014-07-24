/**
 * @file    hardware.cpp
 * @brief   Hardware package entry point
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "hardware.h"
#include "pin_repository.h"
#include "io_pins.h"
#include "rtc_helper.h"
#include "rfiutil.h"
#include "console_io.h"

#include "adc_inputs.h"

#include "trigger_input.h"
#include "eficonsole.h"
#include "board_test.h"

#include "mcp3208.h"
#include "HIP9011.h"
#include "can_hw.h"
#include "histogram.h"
#include "mmc_card.h"
#include "neo6m.h"
#include "lcd_HD44780.h"
#include "settings.h"
#include "algo.h"

#if EFI_INTERNAL_FLASH
#include "flash_main.h"
#endif /* EFI_INTERNAL_FLASH */

#include "trigger_central.h"
#include "svnversion.h"
#include "engine_configuration.h"
#include "ec2.h"

extern engine_configuration_s *engineConfiguration;
extern engine_configuration2_s * engineConfiguration2;
extern board_configuration_s *boardConfiguration;

static bool isSpiInitialized[5] = { false, false, false, false, false };

static void initSpiModule(SPIDriver *driver, ioportid_t sckPort, ioportmask_t sckPin, ioportid_t misoPort,
		ioportmask_t misoPin, ioportid_t mosiPort, ioportmask_t mosiPin, int af) {
	mySetPadMode("SPI clock", sckPort, sckPin, PAL_MODE_ALTERNATE(af));

	mySetPadMode("SPI master out", mosiPort, mosiPin, PAL_MODE_ALTERNATE(af));
	mySetPadMode("SPI master in ", misoPort, misoPin, PAL_MODE_ALTERNATE(af));
}

static Mutex spiMtx;

/**
 * Only one consumer can use SPI bus at a given time
 */
void lockSpi(spi_device_e device) {
	// todo: different locks for different SPI devices!
	chMtxLock(&spiMtx);
}

void unlockSpi(void) {
	chMtxUnlock();
}

void turnOnSpi(spi_device_e device) {
	if (isSpiInitialized[device])
		return; // already initialized
	isSpiInitialized[device] = true;
	if (device == SPI_DEVICE_1) {
#if STM32_SPI_USE_SPI1
//	scheduleMsg(&logging, "Turning on SPI1 pins");
		initSpiModule(&SPID1,
		EFI_SPI1_SCK_PORT, EFI_SPI1_SCK_PIN,
		EFI_SPI1_MISO_PORT, EFI_SPI1_MISO_PIN,
		EFI_SPI1_MOSI_PORT, EFI_SPI1_MOSI_PIN,
		EFI_SPI1_AF);
#endif
	}
	if (device == SPI_DEVICE_2) {
#if STM32_SPI_USE_SPI2
//	scheduleMsg(&logging, "Turning on SPI2 pins");
		initSpiModule(&SPID2,
				EFI_SPI2_SCK_PORT, EFI_SPI2_SCK_PIN,
				EFI_SPI2_MISO_PORT, EFI_SPI2_MISO_PIN,
				EFI_SPI2_MOSI_PORT, EFI_SPI2_MOSI_PIN,
				EFI_SPI2_AF);
#endif
	}
	if (device == SPI_DEVICE_3) {
#if STM32_SPI_USE_SPI3
//	scheduleMsg(&logging, "Turning on SPI3 pins");
		initSpiModule(&SPID3,
		EFI_SPI3_SCK_PORT, EFI_SPI3_SCK_PIN,
		EFI_SPI3_MISO_PORT, EFI_SPI3_MISO_PIN,
		EFI_SPI3_MOSI_PORT, EFI_SPI3_MOSI_PIN,
		EFI_SPI3_AF);
#endif
	}
}

void initSpiModules(void) {
	turnOnSpi(SPI_DEVICE_2);
	turnOnSpi(SPI_DEVICE_3);
}

static I2CConfig i2cfg = { OPMODE_I2C, 100000, STD_DUTY_CYCLE, };

void initI2Cmodule(void) {
	print("Starting I2C module\r\n");
	i2cInit();
	i2cStart(&I2CD1, &i2cfg);

	mySetPadMode("I2C clock", EFI_I2C_SCL_PORT, EFI_I2C_SCL_PIN,
	PAL_MODE_ALTERNATE(EFI_I2C_AF) | PAL_STM32_OTYPE_OPENDRAIN);
	mySetPadMode("I2C data", EFI_I2C_SDA_PORT, EFI_I2C_SDA_PIN,
	PAL_MODE_ALTERNATE(EFI_I2C_AF) | PAL_STM32_OTYPE_OPENDRAIN);
}

//static char txbuf[1];

static void sendI2Cbyte(int addr, int data) {
//	i2cAcquireBus(&I2CD1);
//	txbuf[0] = data;
//	i2cMasterTransmit(&I2CD1, addr, txbuf, 1, NULL, 0);
//	i2cReleaseBus(&I2CD1);
}

void initHardware(Logging *logger) {
	printMsg(logger, "initHardware()");
	// todo: enable protection. it's disabled because it takes
	// 10 extra seconds to re-flash the chip
	//flashProtect();

	chMtxInit(&spiMtx);


#if EFI_HISTOGRAMS
	/**
	 * histograms is a data structure for CPU monitor, it does not depend on configuration
	 */
	initHistogramsModule();
#endif /* EFI_HISTOGRAMS */

	/**
	 * We need the LED_ERROR pin even before we read configuration
	 */
	initPrimaryPins();

	if (hasFirmwareError())
		return;

	initDataStructures(engineConfiguration);

#if EFI_INTERNAL_FLASH

	palSetPadMode(CONFIG_RESET_SWITCH_PORT, CONFIG_RESET_SWITCH_PIN, PAL_MODE_INPUT_PULLUP);

	initFlash();
	/**
	 * this call reads configuration from flash memory or sets default configuration
	 * if flash state does not look right.
	 */
	if (SHOULD_INGORE_FLASH()) {
		engineConfiguration->engineType = FORD_ASPIRE_1996;
		resetConfigurationExt(logger, engineConfiguration->engineType, engineConfiguration, engineConfiguration2,
				boardConfiguration);
		writeToFlash();
	} else {
		readFromFlash();
	}
#else
	engineConfiguration->engineType = FORD_ASPIRE_1996;
	resetConfigurationExt(logger, engineConfiguration->engineType, engineConfiguration, engineConfiguration2, boardConfiguration);
#endif /* EFI_INTERNAL_FLASH */

	if (hasFirmwareError())
		return;

	mySetPadMode("board test", getHwPort(boardConfiguration->boardTestModeJumperPin),
			getHwPin(boardConfiguration->boardTestModeJumperPin), PAL_MODE_INPUT_PULLUP);
	bool isBoardTestMode = GET_BOARD_TEST_MODE_VALUE();

	initAdcInputs(isBoardTestMode);

	if (isBoardTestMode) {
		initBoardTest();
		efiAssertVoid(FALSE, "board test done");
	}

	initRtc();

	initOutputPins();

#if EFI_HIP_9011
	initHip9011();
#endif /* EFI_HIP_9011 */

#if EFI_CAN_SUPPORT
	initCan();
#endif /* EFI_CAN_SUPPORT */

//	init_adc_mcp3208(&adcState, &SPID2);
//	requestAdcValue(&adcState, 0);

	// todo: figure out better startup logic
	initTriggerCentral();

#if EFI_SHAFT_POSITION_INPUT
	initShaftPositionInputCapture();
#endif /* EFI_SHAFT_POSITION_INPUT */

	initSpiModules();

#if EFI_FILE_LOGGING
	initMmcCard();
#endif /* EFI_FILE_LOGGING */

//	initFixedLeds();

	//	initBooleanInputs();

#if EFI_UART_GPS
	initGps();
#endif

#if ADC_SNIFFER
	initAdcDriver();
#endif

#if EFI_HD44780_LCD
//	initI2Cmodule();
	lcd_HD44780_init();
	if (hasFirmwareError())
		return;

	lcd_HD44780_print_string(VCS_VERSION);

#endif /* EFI_HD44780_LCD */

	addConsoleActionII("i2c", sendI2Cbyte);

//	while (true) {
//		for (int addr = 0x20; addr < 0x28; addr++) {
//			sendI2Cbyte(addr, 0);
//			int err = i2cGetErrors(&I2CD1);
//			print("I2C: err=%x from %d\r\n", err, addr);
//			chThdSleepMilliseconds(5);
//			sendI2Cbyte(addr, 255);
//			chThdSleepMilliseconds(5);
//		}
//	}

	printMsg(logger, "initHardware() OK!");
}
