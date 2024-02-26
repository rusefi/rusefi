/**
 * @file    hardware.cpp
 * @brief   Hardware package entry point
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"


#include "trigger_input.h"
#include "can_hw.h"
#include "hardware.h"
#include "rtc_helper.h"
#include "bench_test.h"
#include "yaw_rate_sensor.h"
#include "pin_repository.h"
#include "max31855.h"
#include "logic_analyzer.h"
#include "smart_gpio.h"
#include "accelerometer.h"
#include "eficonsole.h"
#include "console_io.h"
#include "sensor_chart.h"
#include "serial_hw.h"
#include "idle_thread.h"
#include "odometer.h"
#include "kline.h"
#include "dac.h"

#if EFI_PROD_CODE
#include "mpu_util.h"
#endif /* EFI_PROD_CODE */

#include "mmc_card.h"

#include "AdcConfiguration.h"
#include "idle_hardware.h"
#include "mcp3208.h"
#include "hip9011.h"
#include "histogram.h"
#include "gps_uart.h"
#include "sent.h"
#include "cdm_ion_sense.h"
#include "trigger_central.h"
#include "svnversion.h"
#include "vvt.h"
#include "trigger_emulator_algo.h"
#include "boost_control.h"
#include "software_knock.h"
#include "trigger_scope.h"
#include "init.h"
#if EFI_MC33816
#include "mc33816.h"
#endif /* EFI_MC33816 */
#if EFI_WS2812
#include "WS2812.h"
#endif /* EFI_WS2812 */

#if EFI_MAP_AVERAGING
#include "map_averaging.h"
#endif

#if (EFI_STORAGE_INT_FLASH == TRUE) || (EFI_STORAGE_MFS == TRUE)
#include "flash_main.h"
#endif

#if HAL_USE_PAL && EFI_PROD_CODE
#include "digital_input_exti.h"
#endif // HAL_USE_PAL

#if EFI_CAN_SUPPORT
#include "can_vss.h"
#endif

#if HAL_USE_SPI
extern bool isSpiInitialized[SPI_TOTAL_COUNT];

/**
 * Only one consumer can use SPI bus at a given time
 */
void lockSpi(spi_device_e device) {
	efiAssertVoid(ObdCode::CUSTOM_STACK_SPI, hasLotsOfRemainingStack(), "lockSpi");
	spiAcquireBus(getSpiDevice(device));
}

void unlockSpi(spi_device_e device) {
	spiReleaseBus(getSpiDevice(device));
}

static void initSpiModules() {
	if (engineConfiguration->is_enabled_spi_1) {
		 turnOnSpi(SPI_DEVICE_1);
	}
	if (engineConfiguration->is_enabled_spi_2) {
		turnOnSpi(SPI_DEVICE_2);
	}
	if (engineConfiguration->is_enabled_spi_3) {
		turnOnSpi(SPI_DEVICE_3);
	}
	if (engineConfiguration->is_enabled_spi_4) {
		turnOnSpi(SPI_DEVICE_4);
	}
	if (engineConfiguration->is_enabled_spi_5) {
		turnOnSpi(SPI_DEVICE_5);
	}
	if (engineConfiguration->is_enabled_spi_6) {
		turnOnSpi(SPI_DEVICE_6);
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
#if STM32_SPI_USE_SPI4
	if (spiDevice == SPI_DEVICE_4) {
		return &SPID4;
	}
#endif
#if STM32_SPI_USE_SPI5
	if (spiDevice == SPI_DEVICE_5) {
		return &SPID5;
	}
#endif
#if STM32_SPI_USE_SPI6
	if (spiDevice == SPI_DEVICE_6) {
		return &SPID6;
	}
#endif
	firmwareError(ObdCode::CUSTOM_ERR_UNEXPECTED_SPI, "Unexpected SPI device: %d", spiDevice);
	return NULL;
}
#endif // HAL_USE_SPI

#if HAL_USE_ADC

static FastAdcToken fastMapSampleIndex;
static FastAdcToken hipSampleIndex;

#if HAL_TRIGGER_USE_ADC
static FastAdcToken triggerSampleIndex;
#endif // HAL_TRIGGER_USE_ADC

extern AdcDevice fastAdc;

#ifdef FAST_ADC_SKIP
// No reason to enable if N = 1
static_assert(FAST_ADC_SKIP > 1);
static size_t fastAdcSkipCount = 0;
#endif // FAST_ADC_SKIP

/**
 * This method is not in the adc* lower-level file because it is more business logic then hardware.
 */
void onFastAdcComplete(adcsample_t*) {
	ScopePerf perf(PE::AdcCallbackFast);

#if HAL_TRIGGER_USE_ADC
	// we need to call this ASAP, because trigger processing is time-critical
	triggerAdcCallback(getFastAdc(triggerSampleIndex));
#endif /* HAL_TRIGGER_USE_ADC */

#ifdef FAST_ADC_SKIP
	// If we run the fast ADC _very_ fast for triggerAdcCallback's benefit, we may want to
	// skip most of the samples for the rest of the callback.
	if (fastAdcSkipCount++ == FAST_ADC_SKIP) {
		fastAdcSkipCount = 0;
	} else {
		return;
	}
#endif

	/**
	 * this callback is executed 10 000 times a second, it needs to be as fast as possible
	 */
	efiAssertVoid(ObdCode::CUSTOM_STACK_ADC, hasLotsOfRemainingStack(), "lowstck#9b");

#if EFI_SENSOR_CHART && EFI_SHAFT_POSITION_INPUT
	if (getEngineState()->sensorChartMode == SC_AUX_FAST1) {
		float voltage = getAdcValue("fAux1", engineConfiguration->auxFastSensor1_adcChannel);
		scAddData(engine->triggerCentral.getCurrentEnginePhase(getTimeNowNt()).value_or(0), voltage);
	}
#endif /* EFI_SENSOR_CHART */

#if EFI_MAP_AVERAGING
	mapAveragingAdcCallback(adcToVoltsDivided(getFastAdc(fastMapSampleIndex), engineConfiguration->map.sensor.hwChannel));
#endif /* EFI_MAP_AVERAGING */
#if EFI_HIP_9011
	if (engineConfiguration->isHip9011Enabled) {
		hipAdcCallback(adcToVoltsDivided(getFastAdc(hipSampleIndex), engineConfiguration->hipOutputChannel));
	}
#endif /* EFI_HIP_9011 */
}
#endif /* HAL_USE_ADC */

static void calcFastAdcIndexes() {
#if HAL_USE_ADC
	fastMapSampleIndex = enableFastAdcChannel("Fast MAP", engineConfiguration->map.sensor.hwChannel);
	hipSampleIndex = enableFastAdcChannel("HIP9011", engineConfiguration->hipOutputChannel);
#if HAL_TRIGGER_USE_ADC
	triggerSampleIndex = enableFastAdcChannel("Trigger ADC", getAdcChannelForTrigger());
#endif /* HAL_TRIGGER_USE_ADC */

#endif/* HAL_USE_ADC */
}

static void adcConfigListener() {
	// todo: something is not right here - looks like should be a callback for each configuration change?
	calcFastAdcIndexes();
}

void stopSpi(spi_device_e device) {
#if HAL_USE_SPI
	if (!isSpiInitialized[device]) {
		return; // not turned on
	}
	isSpiInitialized[device] = false;
	efiSetPadUnused(getSckPin(device));
	efiSetPadUnused(getMisoPin(device));
	efiSetPadUnused(getMosiPin(device));
#endif /* HAL_USE_SPI */
}

/**
 * this method is NOT currently invoked on ECU start
 * todo: reduce code duplication by moving more logic into startHardware method
 */

void applyNewHardwareSettings() {
    /**
     * All 'stop' methods need to go before we begin starting pins.
     *
     * We take settings from 'activeConfiguration' not 'engineConfiguration' while stopping hardware.
     * Some hardware is restart unconditionally on change of parameters while for some systems we make extra effort and restart only
     * relevant settings were changes.
     *
     */
	ButtonDebounce::stopConfigurationList();

#if EFI_PROD_CODE
	stopSensors();
#endif // EFI_PROD_CODE

#if EFI_PROD_CODE && EFI_SHAFT_POSITION_INPUT
	stopTriggerInputPins();
#endif /* EFI_SHAFT_POSITION_INPUT */

#if EFI_SENT_SUPPORT
	stopSent();
#endif // EFI_SENT_SUPPORT

#if EFI_CAN_SUPPORT
	stopCanPins();
#endif /* EFI_CAN_SUPPORT */

	stopKLine();

#if EFI_AUX_SERIAL
	stopAuxSerialPins();
#endif /* EFI_AUX_SERIAL */

#if EFI_HIP_9011
	stopHip9001_pins();
#endif /* EFI_HIP_9011 */

	stopHardware();

	if (isConfigurationChanged(is_enabled_spi_1)) {
		stopSpi(SPI_DEVICE_1);
	}

	if (isConfigurationChanged(is_enabled_spi_2)) {
		stopSpi(SPI_DEVICE_2);
	}

	if (isConfigurationChanged(is_enabled_spi_3)) {
		stopSpi(SPI_DEVICE_3);
	}

	if (isConfigurationChanged(is_enabled_spi_4)) {
		stopSpi(SPI_DEVICE_4);
	}

	if (isConfigurationChanged(is_enabled_spi_5)) {
		stopSpi(SPI_DEVICE_5);
	}

	if (isConfigurationChanged(is_enabled_spi_6)) {
		stopSpi(SPI_DEVICE_6);
	}

	if (isPinOrModeChanged(clutchUpPin, clutchUpPinMode)) {
		// bug? duplication with stopSwitchPins?
		efiSetPadUnused(activeConfiguration.clutchUpPin);
	}

#if EFI_SHAFT_POSITION_INPUT
	stopTriggerDebugPins();
#endif // EFI_SHAFT_POSITION_INPUT

	enginePins.unregisterPins();

#if EFI_PROD_CODE
	reconfigureSensors();
#endif /* EFI_PROD_CODE */

	ButtonDebounce::startConfigurationList();

	/*******************************************
	 * Start everything back with new settings *
	 ******************************************/
	startHardware();

#if EFI_PROD_CODE && (BOARD_EXT_GPIOCHIPS > 0)
	/* TODO: properly restart gpio chips...
	 * This is only workaround for "CS pin lost" bug
	 * see: https://github.com/rusefi/rusefi/issues/2107
	 * We should provide better way to gracefully stop all
	 * gpio chips: set outputs to safe state, release all
	 * on-chip resources (gpios, SPIs, etc) and then restart
	 * with updated settings.
	 * Following code just re-inits CS pins for all external
	 * gpio chips, but does not update CS pin definition in
	 * gpio chips private data/settings. So changing CS pin
	 * on-fly does not work */
	startSmartCsPins();
#endif /* (BOARD_EXT_GPIOCHIPS > 0) */

#if EFI_AUX_SERIAL
	startAuxSerialPins();
#endif /* EFI_AUX_SERIAL */

    startKLine();


#if EFI_HIP_9011
	startHip9001_pins();
#endif /* EFI_HIP_9011 */


#if EFI_PROD_CODE && EFI_IDLE_CONTROL
	if (isIdleHardwareRestartNeeded()) {
		 initIdleHardware();
	}
#endif

#if EFI_BOOST_CONTROL
	startBoostPin();
#endif
#if EFI_EMULATE_POSITION_SENSORS
	startTriggerEmulatorPins();
#endif /* EFI_EMULATE_POSITION_SENSORS */
#if EFI_LOGIC_ANALYZER
	startLogicAnalyzerPins();
#endif /* EFI_LOGIC_ANALYZER */
#if EFI_VVT_PID
	startVvtControlPins();
#endif /* EFI_VVT_PID */

#if EFI_SENT_SUPPORT
	startSent();
#endif

	adcConfigListener();
}

#if EFI_BOR_LEVEL
void setBor(int borValue) {
	efiPrintf("setting BOR to %d", borValue);
	BOR_Set((BOR_Level_t)borValue);
}
#endif /* EFI_BOR_LEVEL */

// This function initializes hardware that can do so before configuration is loaded
void initHardwareNoConfig() {
	efiAssertVoid(ObdCode::CUSTOM_IH_STACK, hasLotsOfRemainingStack(), "init h");

	efiPrintf("initHardware()");

#if EFI_PROD_CODE
	initPinRepository();
#endif

#if EFI_HISTOGRAMS
	/**
	 * histograms is a data structure for CPU monitor, it does not depend on configuration
	 */
	initHistogramsModule();
#endif /* EFI_HISTOGRAMS */

#if EFI_GPIO_HARDWARE
	/**
	 * We need the LED_ERROR pin even before we read configuration
	 */
	initPrimaryPins();
#endif // EFI_GPIO_HARDWARE

#if EFI_PROD_CODE && EFI_SIGNAL_EXECUTOR_ONE_TIMER
	// it's important to initialize this pretty early in the game before any scheduling usages
	initSingleTimerExecutorHardware();
#endif // EFI_PROD_CODE && EFI_SIGNAL_EXECUTOR_ONE_TIMER

#if EFI_PROD_CODE && EFI_RTC
	initRtc();
#endif // EFI_PROD_CODE && EFI_RTC

#if (EFI_STORAGE_INT_FLASH == TRUE) || (EFI_STORAGE_MFS == TRUE)
	initFlash();
#endif

#if EFI_FILE_LOGGING
	initEarlyMmcCard();
#endif // EFI_FILE_LOGGING

#if HAL_USE_PAL && EFI_PROD_CODE
	// this should be initialized before detectBoardType()
	efiExtiInit();
#endif // HAL_USE_PAL
}

void stopHardware() {
	stopSwitchPins();

#if EFI_PROD_CODE && (BOARD_EXT_GPIOCHIPS > 0)
	stopSmartCsPins();
#endif /* (BOARD_EXT_GPIOCHIPS > 0) */

#if EFI_LOGIC_ANALYZER
	stopLogicAnalyzerPins();
#endif /* EFI_LOGIC_ANALYZER */

#if EFI_EMULATE_POSITION_SENSORS
	stopTriggerEmulatorPins();
#endif /* EFI_EMULATE_POSITION_SENSORS */

#if EFI_VVT_PID
	stopVvtControlPins();
#endif /* EFI_VVT_PID */
}

/**
 * This method is invoked both on ECU start and configuration change
 * At the moment we have too many system which handle ECU start and configuration change separately
 * TODO: move move hardware code here
 */
void startHardware() {
	initStartStopButton();

#if EFI_PROD_CODE && EFI_SHAFT_POSITION_INPUT
	startTriggerInputPins();
#endif /* EFI_SHAFT_POSITION_INPUT */

#if EFI_ENGINE_CONTROL
	enginePins.startPins();
#endif /* EFI_ENGINE_CONTROL */

#if EFI_SHAFT_POSITION_INPUT
	validateTriggerInputs();

	startTriggerDebugPins();

#endif // EFI_SHAFT_POSITION_INPUT

	startSwitchPins();

#if EFI_CAN_SUPPORT
	startCanPins();
#endif /* EFI_CAN_SUPPORT */
}

// Weak link a stub so that every board doesn't have to implement this function
__attribute__((weak)) void boardInitHardware() { }
__attribute__((weak)) void boardInitHardwareExtra() { }

__attribute__((weak)) void setPinConfigurationOverrides() { }

#if HAL_USE_I2C
const I2CConfig i2cfg = {
    OPMODE_I2C,
    400000,
    FAST_DUTY_CYCLE_2,
};
#endif

void initHardware() {
	if (hasFirmwareError()) {
		return;
	}

#if STM32_I2C_USE_I2C3
	if (engineConfiguration->useEeprom) {
	    i2cStart(&EE_U2CD, &i2cfg);
	}
#endif // STM32_I2C_USE_I2C3

	boardInitHardware();
	boardInitHardwareExtra();

#if HAL_USE_ADC
	initAdcInputs();

	// wait for first set of ADC values so that we do not produce invalid sensor data
	waitForSlowAdc(1);
#endif /* HAL_USE_ADC */

#if EFI_SOFTWARE_KNOCK
	initSoftwareKnock();
#endif /* EFI_SOFTWARE_KNOCK */

#ifdef TRIGGER_SCOPE
	initTriggerScope();
#endif // TRIGGER_SCOPE

#if HAL_USE_SPI
	initSpiModules();
#endif /* HAL_USE_SPI */

#if (EFI_PROD_CODE && BOARD_EXT_GPIOCHIPS > 0) || EFI_SIMULATOR
	// initSmartGpio depends on 'initSpiModules'
	initSmartGpio();
#endif

	// output pins potentially depend on 'initSmartGpio'
	initMiscOutputPins();

#if EFI_MC33816
	initMc33816();
#endif /* EFI_MC33816 */

#if EFI_MAX_31855
	initMax31855(engineConfiguration->max31855spiDevice, engineConfiguration->max31855_cs);
#endif /* EFI_MAX_31855 */

#if EFI_CAN_SUPPORT
#if EFI_SIMULATOR
	// Set CAN device name
	CAND1.deviceName = "can0";
#endif

	initCan();
#endif /* EFI_CAN_SUPPORT */


#if EFI_PROD_CODE && EFI_SHAFT_POSITION_INPUT
	onEcuStartTriggerImplementation();
#endif /* EFI_SHAFT_POSITION_INPUT */
	onEcuStartDoSomethingTriggerInputPins();

#if EFI_HIP_9011
	initHip9011();
#endif /* EFI_HIP_9011 */

#if EFI_WS2812
	initWS2812();
#endif /* EFI_LED_WS2812 */

#if EFI_ONBOARD_MEMS
	initAccelerometer();
#endif

#if EFI_BOSCH_YAW
	initBoschYawRateSensor();
#endif /* EFI_BOSCH_YAW */

#if EFI_UART_GPS
	initGps();
#endif

#if EFI_AUX_SERIAL
	initAuxSerial();
#endif /* EFI_AUX_SERIAL */

#if EFI_CAN_SUPPORT
	initCanVssSupport();
#endif // EFI_CAN_SUPPORT

#if EFI_CDM_INTEGRATION
	cdmIonInit();
#endif // EFI_CDM_INTEGRATION

#if EFI_SENT_SUPPORT
	initSent();
#endif

	initKLine();

#if EFI_DAC
	initDac();
#endif

	calcFastAdcIndexes();

	startHardware();

	efiPrintf("initHardware() OK!");
}

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

void checkLastResetCause() {
#if EFI_PROD_CODE
	Reset_Cause_t cause = getMCUResetCause();
	const char *causeStr = getMCUResetCause(cause);
	efiPrintf("Last Reset Cause: %s", causeStr);

	// if reset by watchdog, signal a fatal error
	if (cause == Reset_Cause_IWatchdog || cause == Reset_Cause_WWatchdog) {
		firmwareError(ObdCode::OBD_PCM_Processor_Fault, "Watchdog Reset");
	}
#endif // EFI_PROD_CODE
}
