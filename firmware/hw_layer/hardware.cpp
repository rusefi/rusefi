/**
 * @file    hardware.cpp
 * @brief   Hardware package entry point
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "main.h"
#include "adc_inputs.h"
#include "can_hw.h"
#include "console_io.h"
#include "hardware.h"
#include "io_pins.h"
#include "pin_repository.h"
#include "rtc_helper.h"
#include "rfiutil.h"
#include "injector_central.h"
#include "vehicle_speed.h"

#include "trigger_input.h"
#include "eficonsole.h"
#include "max31855.h"

#if EFI_PROD_CODE
#include "AdcConfiguration.h"
#include "board_test.h"
#include "mcp3208.h"
#include "HIP9011.h"
#include "histogram.h"
#include "mmc_card.h"
#include "neo6m.h"
#include "lcd_HD44780.h"
#include "settings.h"
#include "algo.h"
#include "joystick.h"
#include "trigger_central.h"
#include "svnversion.h"
#include "engine_configuration.h"
#include "CJ125.h"
#endif

#if EFI_SPEED_DENSITY
#include "map_averaging.h"
#endif /* EFI_SPEED_DENSITY */

#if EFI_INTERNAL_FLASH
#include "flash_main.h"
#endif /* EFI_INTERNAL_FLASH */

EXTERN_ENGINE
;
extern bool hasFirmwareErrorFlag;

static Mutex spiMtx;

int maxNesting = 0;

#if HAL_USE_SPI || defined(__DOXYGEN__)
static bool isSpiInitialized[5] = { false, false, false, false, false };

// todo: use larger value if LSE is available, make this a boardConfiguration option
int lseTimeout = 0;

/**
 * Only one consumer can use SPI bus at a given time
 */
void lockSpi(spi_device_e device) {
	efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "lockSpi");
	// todo: different locks for different SPI devices!
	chMtxLock(&spiMtx);
}

void unlockSpi(void) {
	chMtxUnlock();
}

static void initSpiModules(board_configuration_s *boardConfiguration) {
	if (boardConfiguration->is_enabled_spi_2) {
		turnOnSpi(SPI_DEVICE_2);
	}
	if (boardConfiguration->is_enabled_spi_3) {
		turnOnSpi(SPI_DEVICE_3);
	}
}

SPIDriver * getSpiDevice(spi_device_e spiDevice) {
	if (spiDevice == SPI_NONE) {
		return NULL;
	}
#if STM32_SPI_USE_SPI1 || defined(__DOXYGEN__)
	if (spiDevice == SPI_DEVICE_1) {
		return &SPID1;
	}
#endif
#if STM32_SPI_USE_SPI2 || defined(__DOXYGEN__)
	if (spiDevice == SPI_DEVICE_2) {
		return &SPID2;
	}
#endif
#if STM32_SPI_USE_SPI3 || defined(__DOXYGEN__)
	if (spiDevice == SPI_DEVICE_3) {
		return &SPID3;
	}
#endif
	firmwareError("Unexpected SPI device: %d", spiDevice);
	return NULL;
}
#endif

#if HAL_USE_I2C || defined(__DOXYGEN__)
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

#endif

static Logging *sharedLogger;

#if EFI_PROD_CODE

extern AdcDevice fastAdc;

#define TPS_IS_SLOW -1

static int fastMapSampleIndex;
static int hipSampleIndex;
static int tpsSampleIndex;

extern int tpsFastAdc;

/**
 * This method is not in the adc* lower-level file because it is more business logic then hardware.
 */
void adc_callback_fast(ADCDriver *adcp, adcsample_t *buffer, size_t n) {
	efiAssertVoid(getRemainingStack(chThdSelf()) > 64, "lowstck12a");

	(void) buffer;
	(void) n;
	/**
	 * Note, only in the ADC_COMPLETE state because the ADC driver fires an
	 * intermediate callback when the buffer is half full.
	 * */
	if (adcp->state == ADC_COMPLETE) {
		/**
		 * this callback is executed 10 000 times a second, it needs to be as fast as possible
		 */
		efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "lowstck#9b");

#if EFI_MAP_AVERAGING
		mapAveragingCallback(fastAdc.samples[fastMapSampleIndex]);
#endif /* EFI_MAP_AVERAGING */
#if EFI_HIP_9011 || defined(__DOXYGEN__)
		if (boardConfiguration->isHip9011Enabled) {
			hipAdcCallback(fastAdc.samples[hipSampleIndex]);
		}
#endif
		if (tpsSampleIndex != TPS_IS_SLOW) {
//			tpsFastAdc = fastAdc.samples[tpsSampleIndex];
		}
	}
}

static void calcFastAdcIndexes(void) {
	fastMapSampleIndex = fastAdc.internalAdcIndexByHardwareIndex[engineConfiguration->map.sensor.hwChannel];
	hipSampleIndex =
			engineConfiguration->hipOutputChannel == EFI_ADC_NONE ?
					-1 : fastAdc.internalAdcIndexByHardwareIndex[engineConfiguration->hipOutputChannel];
	if (engineConfiguration->tpsAdcChannel != EFI_ADC_NONE) {
		tpsSampleIndex = fastAdc.internalAdcIndexByHardwareIndex[engineConfiguration->tpsAdcChannel];
	} else {
		tpsSampleIndex = TPS_IS_SLOW;
	}
}

static void adcConfigListener(Engine *engine) {
	calcFastAdcIndexes();
}

void turnOnHardware(Logging *sharedLogger) {
#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
	turnOnTriggerInputPins(sharedLogger);
#endif /* EFI_SHAFT_POSITION_INPUT */
}

void applyNewHardwareSettings(void) {
#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
	applyNewTriggerInputPins();
#endif /* EFI_SHAFT_POSITION_INPUT */
	stopInjectionPins();
	stopIgnitionPins();

	startInjectionPins();
	startIgnitionPins();
}

void initHardware(Logging *l) {
	efiAssertVoid(getRemainingStack(chThdSelf()) > 256, "init h");
	sharedLogger = l;
	engine_configuration_s *engineConfiguration = engine->engineConfiguration;
	efiAssertVoid(engineConfiguration!=NULL, "engineConfiguration");
	board_configuration_s *boardConfiguration = &engineConfiguration->bc;

	printMsg(sharedLogger, "initHardware()");
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

	if (hasFirmwareError()) {
		return;
	}

#if EFI_INTERNAL_FLASH

	palSetPadMode(CONFIG_RESET_SWITCH_PORT, CONFIG_RESET_SWITCH_PIN, PAL_MODE_INPUT_PULLUP);

	initFlash(sharedLogger);
	/**
	 * this call reads configuration from flash memory or sets default configuration
	 * if flash state does not look right.
	 */
	if (SHOULD_INGORE_FLASH()) {
		engineConfiguration->engineType = DEFAULT_ENGINE_TYPE;
		resetConfigurationExt(sharedLogger, engineConfiguration->engineType PASS_ENGINE_PARAMETER);
		writeToFlashNow();
	} else {
		readFromFlash();
	}
#else
	engineConfiguration->engineType = DEFAULT_ENGINE_TYPE;
	resetConfigurationExt(sharedLogger, engineConfiguration->engineType PASS_ENGINE_PARAMETER);
#endif /* EFI_INTERNAL_FLASH */

	if (hasFirmwareError()) {
		return;
	}

#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
	initTriggerDecoder();
#endif

	mySetPadMode2("board test", boardConfiguration->boardTestModeJumperPin,
	PAL_MODE_INPUT_PULLUP);
	bool isBoardTestMode_b = (!palReadPad(getHwPort(boardConfiguration->boardTestModeJumperPin), getHwPin(boardConfiguration->boardTestModeJumperPin)));

	// we can now relese this pin, it is actually used as output sometimes
	unmarkPin(boardConfiguration->boardTestModeJumperPin);



#if HAL_USE_ADC || defined(__DOXYGEN__)
	initAdcInputs(isBoardTestMode_b);
#endif

	if (isBoardTestMode_b) {
		// this method never returns
		initBoardTest();
	}

	initRtc();

	initOutputPins();

#if EFI_MAX_31855
	initMax31855(sharedLogger, getSpiDevice(boardConfiguration->max31855spiDevice), boardConfiguration->max31855_cs);
#endif /* EFI_MAX_31855 */

#if EFI_CAN_SUPPORT
	initCan();
#endif /* EFI_CAN_SUPPORT */

//	init_adc_mcp3208(&adcState, &SPID2);
//	requestAdcValue(&adcState, 0);

#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
	// todo: figure out better startup logic
	initTriggerCentral(sharedLogger, engine);
#endif /* EFI_SHAFT_POSITION_INPUT */

	turnOnHardware(sharedLogger);

#if HAL_USE_SPI || defined(__DOXYGEN__)
	initSpiModules(boardConfiguration);
#endif

#if EFI_HIP_9011
	initHip9011(sharedLogger);
#endif /* EFI_HIP_9011 */

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
	lcd_HD44780_init(sharedLogger);
	if (hasFirmwareError())
		return;

	lcd_HD44780_print_string(VCS_VERSION);

#endif /* EFI_HD44780_LCD */

#if HAL_USE_I2C || defined(__DOXYGEN__)
	addConsoleActionII("i2c", sendI2Cbyte);
#endif

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

#if EFI_VEHICLE_SPEED || defined(__DOXYGEN__)
	initVehicleSpeed(sharedLogger);
#endif

#if HAL_USE_EXT || defined(__DOXYGEN__)
	initJoystick(sharedLogger);
#endif

	calcFastAdcIndexes();
	engine->addConfigurationListener(adcConfigListener);

#if EFI_CJ125 || defined(__DOXYGEN__)
	initCJ125();
#endif

	printMsg(sharedLogger, "initHardware() OK!");
}

#endif /* EFI_PROD_CODE */
