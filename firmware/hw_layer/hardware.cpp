/**
 * @file    hardware.cpp
 * @brief   Hardware package entry point
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "global.h"

#include "trigger_input.h"
#include "servo.h"

#if EFI_PROD_CODE
#include "adc_inputs.h"
#include "can_hw.h"
#include "hardware.h"
#include "io_pins.h"
#include "rtc_helper.h"
#include "rfiutil.h"
#include "injector_central.h"
#include "vehicle_speed.h"
#include "yaw_rate_sensor.h"
#include "pin_repository.h"
#include "max31855.h"
#include "smart_gpio.h"
#include "accelerometer.h"
#include "eficonsole.h"
#include "console_io.h"

#include "mpu_util.h"
//#include "usb_msd.h"

#include "AdcConfiguration.h"
#include "electronic_throttle.h"
#include "board_test.h"
#include "mcp3208.h"
#include "hip9011.h"
#include "histogram.h"
#include "mmc_card.h"
#include "neo6m.h"
#include "lcd_HD44780.h"
#include "settings.h"
#include "algo.h"
#include "joystick.h"
#include "cdm_ion_sense.h"
#include "trigger_central.h"
#include "svnversion.h"
#include "engine_configuration.h"
#include "aux_pid.h"

#if EFI_MC33816
#include "mc33816.h"
#endif /* EFI_MC33816 */

#if EFI_SPEED_DENSITY
#include "map_averaging.h"
#endif /* EFI_SPEED_DENSITY */

#if EFI_INTERNAL_FLASH
#include "flash_main.h"
#endif /* EFI_INTERNAL_FLASH */

EXTERN_ENGINE
;
extern bool hasFirmwareErrorFlag;

static mutex_t spiMtx;

/**
 * this depends on patch to chdebug.c
+extern int maxNesting;
   ch.dbg.isr_cnt++;
+  if (ch.dbg.isr_cnt > maxNesting)
+          maxNesting = ch.dbg.isr_cnt;
   port_unlock_from_isr();
 *
 */
// todo: rename this to 'rusefiMaxISRNesting' one day
int maxNesting = 0;

#if HAL_USE_SPI
extern bool isSpiInitialized[5];

/**
 * #311 we want to test RTC before engine start so that we do not test it while engine is running
 */
bool rtcWorks = true;

/**
 * Only one consumer can use SPI bus at a given time
 */
void lockSpi(spi_device_e device) {
	UNUSED(device);
	efiAssertVoid(CUSTOM_ERR_6674, getCurrentRemainingStack() > 128, "lockSpi");
	// todo: different locks for different SPI devices!
	chMtxLock(&spiMtx);
}

void unlockSpi(void) {
	chMtxUnlock(&spiMtx);
}

static void initSpiModules(board_configuration_s *boardConfiguration) {
	UNUSED(boardConfiguration);
	if (CONFIGB(is_enabled_spi_1)) {
		 turnOnSpi(SPI_DEVICE_1);
	}
	if (CONFIGB(is_enabled_spi_2)) {
		turnOnSpi(SPI_DEVICE_2);
	}
	if (CONFIGB(is_enabled_spi_3)) {
		turnOnSpi(SPI_DEVICE_3);
	}
}

/**
 * @return NULL if SPI device not specified
 */
SPIDriver * getSpiDevice(spi_device_e spiDevice) {
	if (spiDevice == SPI_NONE) {
		return NULL;
	}
#if STM32_SPI_USE_SPI1
	if (spiDevice == SPI_DEVICE_1) {
		return &SPID1;
	}
#endif
#if STM32_SPI_USE_SPI2
	if (spiDevice == SPI_DEVICE_2) {
		return &SPID2;
	}
#endif
#if STM32_SPI_USE_SPI3
	if (spiDevice == SPI_DEVICE_3) {
		return &SPID3;
	}
#endif
	firmwareError(CUSTOM_ERR_UNEXPECTED_SPI, "Unexpected SPI device: %d", spiDevice);
	return NULL;
}
#endif

#if HAL_USE_I2C
#if defined(STM32F7XX)
// values calculated with STM32CubeMX tool, 100kHz I2C clock for Nucleo-767 @168 MHz, PCK1=42MHz
#define HAL_I2C_F7_100_TIMINGR 0x00A0A3F7
static I2CConfig i2cfg = { HAL_I2C_F7_100_TIMINGR, 0, 0 };	// todo: does it work?
#else /* defined(STM32F4XX) */
static I2CConfig i2cfg = { OPMODE_I2C, 100000, STD_DUTY_CYCLE, };
#endif /* defined(STM32F4XX) */

void initI2Cmodule(void) {
	print("Starting I2C module\r\n");
	i2cInit();
	i2cStart(&I2CD1, &i2cfg);

	efiSetPadMode("I2C clock", EFI_I2C_SCL_BRAIN_PIN, PAL_MODE_ALTERNATE(EFI_I2C_AF) | PAL_STM32_OTYPE_OPENDRAIN);
	efiSetPadMode("I2C data", EFI_I2C_SDA_BRAIN_PIN, PAL_MODE_ALTERNATE(EFI_I2C_AF) | PAL_STM32_OTYPE_OPENDRAIN);
}

//static char txbuf[1];

static void sendI2Cbyte(int addr, int data) {
	(void)addr;
	(void)data;
//	i2cAcquireBus(&I2CD1);
//	txbuf[0] = data;
//	i2cMasterTransmit(&I2CD1, addr, txbuf, 1, NULL, 0);
//	i2cReleaseBus(&I2CD1);
}

#endif

static Logging *sharedLogger;

#if EFI_PROD_CODE

#define TPS_IS_SLOW -1

static int fastMapSampleIndex;
static int hipSampleIndex;
static int tpsSampleIndex;

extern int tpsFastAdc;

#if HAL_USE_ADC
extern AdcDevice fastAdc;

/**
 * This method is not in the adc* lower-level file because it is more business logic then hardware.
 */
void adc_callback_fast(ADCDriver *adcp, adcsample_t *buffer, size_t n) {

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
		efiAssertVoid(CUSTOM_ERR_6676, getCurrentRemainingStack() > 128, "lowstck#9b");

#if EFI_MAP_AVERAGING
		mapAveragingAdcCallback(fastAdc.samples[fastMapSampleIndex]);
#endif /* EFI_MAP_AVERAGING */
#if EFI_HIP_9011
		if (CONFIGB(isHip9011Enabled)) {
			hipAdcCallback(fastAdc.samples[hipSampleIndex]);
		}
#endif
//		if (tpsSampleIndex != TPS_IS_SLOW) {
//			tpsFastAdc = fastAdc.samples[tpsSampleIndex];
//		}
	}
}
#endif /* HAL_USE_ADC */

static void calcFastAdcIndexes(void) {
#if HAL_USE_ADC
	fastMapSampleIndex = fastAdc.internalAdcIndexByHardwareIndex[engineConfiguration->map.sensor.hwChannel];
	hipSampleIndex =
			engineConfiguration->hipOutputChannel == EFI_ADC_NONE ?
					-1 : fastAdc.internalAdcIndexByHardwareIndex[engineConfiguration->hipOutputChannel];
	if (engineConfiguration->tpsAdcChannel != EFI_ADC_NONE) {
		tpsSampleIndex = fastAdc.internalAdcIndexByHardwareIndex[engineConfiguration->tpsAdcChannel];
	} else {
		tpsSampleIndex = TPS_IS_SLOW;
	}
#endif/* HAL_USE_ADC */
}

static void adcConfigListener(Engine *engine) {
	UNUSED(engine);
	// todo: something is not right here - looks like should be a callback for each configuration change?
	calcFastAdcIndexes();
}

void turnOnHardware(Logging *sharedLogger) {
#if EFI_SHAFT_POSITION_INPUT
	turnOnTriggerInputPins(sharedLogger);
#endif /* EFI_SHAFT_POSITION_INPUT */
}

static void unregisterPin(brain_pin_e currentPin, brain_pin_e prevPin) {
	if (currentPin != prevPin) {
		brain_pin_markUnused(prevPin);
	}
}

void stopSpi(spi_device_e device) {
#if HAL_USE_SPI
	if (!isSpiInitialized[device])
		return; // not turned on
	isSpiInitialized[device] = false;
	brain_pin_markUnused(getSckPin(device));
	brain_pin_markUnused(getMisoPin(device));
	brain_pin_markUnused(getMosiPin(device));
#endif /* HAL_USE_SPI */
}

void applyNewHardwareSettings(void) {
    // all 'stop' methods need to go before we begin starting pins

#if EFI_SHAFT_POSITION_INPUT
	stopTriggerInputPins();
#endif /* EFI_SHAFT_POSITION_INPUT */

#if (HAL_USE_PAL && EFI_JOYSTICK)
	stopJoystickPins();
#endif /* HAL_USE_PAL && EFI_JOYSTICK */
       
	enginePins.stopInjectionPins();
    enginePins.stopIgnitionPins();
#if EFI_CAN_SUPPORT
	stopCanPins();
#endif /* EFI_CAN_SUPPORT */

#if EFI_HIP_9011
	stopHip9001_pins();
#endif /* EFI_HIP_9011 */

#if EFI_ELECTRONIC_THROTTLE_BODY
	bool etbRestartNeeded = isETBRestartNeeded();
	if (etbRestartNeeded) {
		stopETBPins();
	}
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

#if (BOARD_TLE6240_COUNT > 0)
	stopSmartCsPins();
#endif /* (BOARD_MC33972_COUNT > 0) */

#if EFI_VEHICLE_SPEED
	stopVSSPins();
#endif /* EFI_VEHICLE_SPEED */

#if EFI_AUX_PID
	stopAuxPins();
#endif /* EFI_AUX_PID */

	if (engineConfiguration->bc.is_enabled_spi_1 != activeConfiguration.bc.is_enabled_spi_1)
		stopSpi(SPI_DEVICE_1);

	if (engineConfiguration->bc.is_enabled_spi_2 != activeConfiguration.bc.is_enabled_spi_2)
		stopSpi(SPI_DEVICE_2);

	if (engineConfiguration->bc.is_enabled_spi_3 != activeConfiguration.bc.is_enabled_spi_3)
		stopSpi(SPI_DEVICE_3);

#if EFI_HD44780_LCD
	stopHD44780_pins();
#endif /* #if EFI_HD44780_LCD */

	unregisterPin(engineConfiguration->bc.clutchUpPin, activeConfiguration.bc.clutchUpPin);

	enginePins.unregisterPins();

#if EFI_SHAFT_POSITION_INPUT
	startTriggerInputPins();
#endif /* EFI_SHAFT_POSITION_INPUT */

#if (HAL_USE_PAL && EFI_JOYSTICK)
	startJoystickPins();
#endif /* HAL_USE_PAL && EFI_JOYSTICK */

#if EFI_HD44780_LCD
	startHD44780_pins();
#endif /* #if EFI_HD44780_LCD */

	enginePins.startInjectionPins();
	enginePins.startIgnitionPins();

#if EFI_CAN_SUPPORT
	startCanPins();
#endif /* EFI_CAN_SUPPORT */

#if EFI_HIP_9011
	startHip9001_pins();
#endif /* EFI_HIP_9011 */


#if EFI_ELECTRONIC_THROTTLE_BODY
	if (etbRestartNeeded) {
		startETBPins();
	}
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

#if EFI_VEHICLE_SPEED
	startVSSPins();
#endif /* EFI_VEHICLE_SPEED */

#if EFI_AUX_PID
	startAuxPins();
#endif /* EFI_AUX_PID */

	adcConfigListener(engine);
}

void setBor(int borValue) {
	scheduleMsg(sharedLogger, "setting BOR to %d", borValue);
	BOR_Set((BOR_Level_t)borValue);
	showBor();
}

void showBor(void) {
	scheduleMsg(sharedLogger, "BOR=%d", (int)BOR_Get());
}

void initHardware(Logging *l) {
	efiAssertVoid(CUSTOM_IH_STACK, getCurrentRemainingStack() > 256, "init h");
	sharedLogger = l;
	engine_configuration_s *engineConfiguration = engine->engineConfigurationPtr;
	efiAssertVoid(CUSTOM_EC_NULL, engineConfiguration!=NULL, "engineConfiguration");
	board_configuration_s *boardConfiguration = &engineConfiguration->bc;

	printMsg(sharedLogger, "initHardware()");
	// todo: enable protection. it's disabled because it takes
	// 10 extra seconds to re-flash the chip
	//flashProtect();

	chMtxObjectInit(&spiMtx);

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

#ifdef CONFIG_RESET_SWITCH_PORT
	palSetPadMode(CONFIG_RESET_SWITCH_PORT, CONFIG_RESET_SWITCH_PIN, PAL_MODE_INPUT_PULLUP);
#endif /* CONFIG_RESET_SWITCH_PORT */

	initFlash(sharedLogger);
	/**
	 * this call reads configuration from flash memory or sets default configuration
	 * if flash state does not look right.
	 *
	 * interesting fact that we have another read from flash before we get here
	 */
	if (SHOULD_INGORE_FLASH()) {
		engineConfiguration->engineType = DEFAULT_ENGINE_TYPE;
		resetConfigurationExt(sharedLogger, engineConfiguration->engineType PASS_ENGINE_PARAMETER_SUFFIX);
		writeToFlashNow();
	} else {
		readFromFlash();
	}
#else
	engineConfiguration->engineType = DEFAULT_ENGINE_TYPE;
	resetConfigurationExt(sharedLogger, engineConfiguration->engineType PASS_ENGINE_PARAMETER_SUFFIX);
#endif /* EFI_INTERNAL_FLASH */

	// it's important to initialize this pretty early in the game before any scheduling usages
	initSingleTimerExecutorHardware();

#if EFI_HD44780_LCD
//	initI2Cmodule();
	lcd_HD44780_init(sharedLogger);
	if (hasFirmwareError())
		return;

	lcd_HD44780_print_string(VCS_VERSION);

#endif /* EFI_HD44780_LCD */

	if (hasFirmwareError()) {
		return;
	}

#if EFI_SHAFT_POSITION_INPUT
	initTriggerDecoder();
#endif

	bool isBoardTestMode_b;
	if (CONFIGB(boardTestModeJumperPin) != GPIO_UNASSIGNED) {
		efiSetPadMode("board test", CONFIGB(boardTestModeJumperPin),
		PAL_MODE_INPUT_PULLUP);
		isBoardTestMode_b = (!efiReadPin(CONFIGB(boardTestModeJumperPin)));

		// we can now relese this pin, it is actually used as output sometimes
		brain_pin_markUnused(CONFIGB(boardTestModeJumperPin));
	} else {
		isBoardTestMode_b = false;
	}

#if HAL_USE_ADC
	initAdcInputs(isBoardTestMode_b);
#endif

#if EFI_BOARD_TEST
	if (isBoardTestMode_b) {
		// this method never returns
		initBoardTest();
	}
#endif /* EFI_BOARD_TEST */

	initRtc();

#if HAL_USE_SPI
	initSpiModules(boardConfiguration);
#endif
	// initSmartGpio depends on 'initSpiModules'
	initSmartGpio(PASS_ENGINE_PARAMETER_SIGNATURE);

	// output pins potentially depend on 'initSmartGpio'
	initOutputPins();

#if EFI_MC33816
	initMc33816();
#endif /* EFI_MC33816 */

#if EFI_MAX_31855
	initMax31855(sharedLogger, CONFIGB(max31855spiDevice), CONFIGB(max31855_cs));
#endif /* EFI_MAX_31855 */

#if EFI_CAN_SUPPORT
	initCan();
#endif /* EFI_CAN_SUPPORT */

//	init_adc_mcp3208(&adcState, &SPID2);
//	requestAdcValue(&adcState, 0);

#if EFI_SHAFT_POSITION_INPUT
	// todo: figure out better startup logic
	initTriggerCentral(sharedLogger);
#endif /* EFI_SHAFT_POSITION_INPUT */

	turnOnHardware(sharedLogger);

#if EFI_HIP_9011
	initHip9011(sharedLogger);
#endif /* EFI_HIP_9011 */

#if EFI_FILE_LOGGING
	initMmcCard();
#endif /* EFI_FILE_LOGGING */

#if EFI_MEMS
	initAccelerometer(PASS_ENGINE_PARAMETER_SIGNATURE);
#endif
//	initFixedLeds();


#if EFI_BOSCH_YAW
	initBoschYawRateSensor();
#endif /* EFI_BOSCH_YAW */

	//	initBooleanInputs();

#if EFI_UART_GPS
	initGps();
#endif

#if EFI_SERVO
	initServo();
#endif

#if ADC_SNIFFER
	initAdcDriver();
#endif

#if HAL_USE_I2C
	addConsoleActionII("i2c", sendI2Cbyte);
#endif


//	USBMassStorageDriver UMSD1;

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

#if EFI_VEHICLE_SPEED
	initVehicleSpeed(sharedLogger);
#endif

#if EFI_CDM_INTEGRATION
	cdmIonInit();
#endif

#if (HAL_USE_PAL && EFI_JOYSTICK)
	initJoystick(sharedLogger);
#endif /* HAL_USE_PAL && EFI_JOYSTICK */

	calcFastAdcIndexes();

	printMsg(sharedLogger, "initHardware() OK!");
}

#endif /* EFI_PROD_CODE */

#endif  /* EFI_PROD_CODE || EFI_SIMULATOR */

#if HAL_USE_SPI
// this is F4 implementation but we will keep it here for now for simplicity
int getSpiPrescaler(spi_speed_e speed, spi_device_e device) {
	switch (speed) {
	case _5MHz:
		return device == SPI_DEVICE_1 ? SPI_BaudRatePrescaler_16 : SPI_BaudRatePrescaler_8;
	case _2_5MHz:
		return device == SPI_DEVICE_1 ? SPI_BaudRatePrescaler_32 : SPI_BaudRatePrescaler_16;
	case _1_25MHz:
		return device == SPI_DEVICE_1 ? SPI_BaudRatePrescaler_64 : SPI_BaudRatePrescaler_32;

	case _150KHz:
		// SPI1 does not support 150KHz, it would be 300KHz for SPI1
		return SPI_BaudRatePrescaler_256;
	default:
		// unexpected
		return 0;
	}
}

#endif /* HAL_USE_SPI */
