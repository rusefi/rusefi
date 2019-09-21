/**
 * @file    engine_controller.cpp
 * @brief   Controllers package entry point code
 *
 *
 *
 * @date Feb 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2019
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "global.h"
#include "os_access.h"
#if EFI_SENSOR_CHART
#include "sensor_chart.h"
#endif
#include "engine_configuration.h"
#include "trigger_central.h"
#include "engine_controller.h"
#include "fsio_core.h"
#include "fsio_impl.h"
#include "idle_thread.h"
#include "rpm_calculator.h"
#include "signal_executor.h"
#include "main_trigger_callback.h"
#include "io_pins.h"
#include "flash_main.h"
#if EFI_TUNER_STUDIO
#include "tunerstudio.h"
#endif
#include "injector_central.h"
#include "os_util.h"
#include "engine_math.h"
#if EFI_WAVE_ANALYZER
#include "wave_analyzer.h"
#endif
#include "allsensors.h"
#include "electronic_throttle.h"
#include "map_averaging.h"
#include "malfunction_central.h"
#include "malfunction_indicator.h"
#include "engine.h"
#include "algo.h"
#include "local_version_holder.h"
#include "alternator_controller.h"
#include "fuel_math.h"
#include "settings.h"
#include "aux_pid.h"
#include "accelerometer.h"
#include "counter64.h"

#if HAL_USE_ADC
#include "AdcConfiguration.h"
#endif /* HAL_USE_ADC */

#if defined(EFI_BOOTLOADER_INCLUDE_CODE)
#include "bootloader/bootloader.h"
#endif /* EFI_BOOTLOADER_INCLUDE_CODE */

#if EFI_PROD_CODE || EFI_SIMULATOR
#include "periodic_task.h"
#endif

#include "init.h"
#if EFI_PROD_CODE
#include "pwm_generator.h"
#include "adc_inputs.h"

#include "pwm_tester.h"
#include "pwm_generator.h"
#include "lcd_controller.h"
#include "pin_repository.h"
#include "tachometer.h"
#endif /* EFI_PROD_CODE */

#if EFI_CJ125
#include "cj125.h"
#endif /* EFI_CJ125 */

// this method is used by real firmware and simulator and unit test
void mostCommonInitEngineController(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if !EFI_UNIT_TEST
	initSensors();
#endif

	initSensors(sharedLogger PASS_ENGINE_PARAMETER_SUFFIX);

	initAccelEnrichment(sharedLogger PASS_ENGINE_PARAMETER_SUFFIX);

#if EFI_FSIO
	initFsioImpl(sharedLogger PASS_ENGINE_PARAMETER_SUFFIX);
#endif /* EFI_FSIO */

#if EFI_IDLE_CONTROL
	startIdleThread(sharedLogger PASS_ENGINE_PARAMETER_SUFFIX);
#endif /* EFI_IDLE_CONTROL */
}

EXTERN_ENGINE;

#if EFI_ENABLE_MOCK_ADC
void setMockVoltage(int hwChannel, float voltage DECLARE_ENGINE_PARAMETER_SUFFIX) {
	engine->engineState.mockAdcState.setMockVoltage(hwChannel, voltage PASS_ENGINE_PARAMETER_SUFFIX);
}
#endif

#if !EFI_UNIT_TEST

extern bool hasFirmwareErrorFlag;
extern EnginePins enginePins;


static void doPeriodicSlowCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE);

class PeriodicFastController : public PeriodicTimerController {
	void PeriodicTask() override {
		engine->periodicFastCallback();
	}

	int getPeriodMs() override {
		return FAST_CALLBACK_PERIOD_MS;
	}
};

class PeriodicSlowController : public PeriodicTimerController {
	void PeriodicTask() override {
		doPeriodicSlowCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	}

	int getPeriodMs() override {
		// we need at least protection from zero value while resetting configuration
		int periodMs = maxI(50, CONFIGB(generalPeriodicThreadPeriodMs));
		return periodMs;
	}
};

static PeriodicFastController fastController;
static PeriodicSlowController slowController;

static LoggingWithStorage logger("Engine Controller");

#if EFI_PROD_CODE || EFI_SIMULATOR

/**
 * todo: this should probably become 'static', i.e. private, and propagated around explicitly?
 */
Engine ___engine CCM_OPTIONAL;
Engine * engine = &___engine;
#endif /* EFI_PROD_CODE || EFI_SIMULATOR */

class EngineStateBlinkingTask : public PeriodicTimerController {
	int getPeriodMs() override {
		return 50;
	}

	void PeriodicTask() override {
		counter++;
		bool is_running = ENGINE(rpmCalculator).isRunning(PASS_ENGINE_PARAMETER_SIGNATURE);

		if (is_running) {
			// blink in running mode
			enginePins.runningLedPin.setValue(counter % 2);
		} else {
			int is_cranking = ENGINE(rpmCalculator).isCranking(PASS_ENGINE_PARAMETER_SIGNATURE);
			enginePins.runningLedPin.setValue(is_cranking);
		}
	}
private:
	int counter = 0;
};

static EngineStateBlinkingTask engineStateBlinkingTask;

#if EFI_PROD_CODE
static Overflow64Counter halTime;

/**
 * 64-bit result would not overflow, but that's complex stuff for our 32-bit MCU
 */
//todo: macro to save method invocation
efitimeus_t getTimeNowUs(void) {
	return getTimeNowNt() / (CORE_CLOCK / 1000000);
}

//todo: macro to save method invocation
efitick_t getTimeNowNt(void) {
#if EFI_PROD_CODE
    /* Entering a reentrant critical zone.*/
    syssts_t sts = chSysGetStatusAndLockX();
	efitime_t localH = halTime.state.highBits;
	uint32_t localLow = halTime.state.lowBits;

	uint32_t value = getTimeNowLowerNt();

	if (value < localLow) {
		// new value less than previous value means there was an overflow in that 32 bit counter
		localH += 0x100000000LL;
	}

	efitime_t result = localH + value;

    /* Leaving the critical zone.*/
    chSysRestoreStatusX(sts);
	return result;
#else /* EFI_PROD_CODE */
// todo: why is this implementation not used?
	/**
	 * this method is lock-free and thread-safe, that's because the 'update' method
	 * is atomic with a critical zone requirement.
	 *
	 * http://stackoverflow.com/questions/5162673/how-to-read-two-32bit-counters-as-a-64bit-integer-without-race-condition
	 */
	efitime_t localH;
	efitime_t localH2;
	uint32_t localLow;
	int counter = 0;
	do {
		localH = halTime.state.highBits;
		localLow = halTime.state.lowBits;
		localH2 = halTime.state.highBits;
#if EFI_PROD_CODE
		if (counter++ == 10000)
			chDbgPanic("lock-free frozen");
#endif /* EFI_PROD_CODE */
	} while (localH != localH2);
	/**
	 * We need to take current counter after making a local 64 bit snapshot
	 */
	uint32_t value = getTimeNowLowerNt();

	if (value < localLow) {
		// new value less than previous value means there was an overflow in that 32 bit counter
		localH += 0x100000000LL;
	}

	return localH + value;
#endif /* EFI_PROD_CODE */

}

/**
 * number of SysClock ticks in one ms
 */
#define TICKS_IN_MS  (CH_CFG_ST_FREQUENCY / 1000)


// todo: this overflows pretty fast!
efitimems_t currentTimeMillis(void) {
	// todo: migrate to getTimeNowUs? or not?
	return chVTGetSystemTimeX() / TICKS_IN_MS;
}

// todo: this overflows pretty fast!
efitimesec_t getTimeNowSeconds(void) {
	return currentTimeMillis() / 1000;
}

#endif /* EFI_PROD_CODE */

static void resetAccel(void) {
	engine->engineLoadAccelEnrichment.resetAE();
	engine->tpsAccelEnrichment.resetAE();
	engine->wallFuel.resetWF();
}

static int previousSecond;

#if EFI_CLOCK_LOCKS

typedef FLStack<int, 16> irq_enter_timestamps_t;

static irq_enter_timestamps_t irqEnterTimestamps;

void irqEnterHook(void) {
	irqEnterTimestamps.push(getTimeNowLowerNt());
}

static int currentIrqDurationAccumulator = 0;
static int currentIrqCounter = 0;
/**
 * See also maxLockedDuration
 */
int perSecondIrqDuration = 0;
int perSecondIrqCounter = 0;
void irqExitHook(void) {
	int enterTime = irqEnterTimestamps.pop();
	currentIrqDurationAccumulator += (getTimeNowLowerNt() - enterTime);
	currentIrqCounter++;
}
#endif /* EFI_CLOCK_LOCKS */

static void invokePerSecond(void) {
#if EFI_CLOCK_LOCKS
	// this data transfer is not atomic but should be totally good enough
	perSecondIrqDuration = currentIrqDurationAccumulator;
	perSecondIrqCounter = currentIrqCounter;
	currentIrqDurationAccumulator = currentIrqCounter = 0;
#endif /* EFI_CLOCK_LOCKS */
}

static void doPeriodicSlowCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT
	efiAssertVoid(CUSTOM_ERR_6661, getCurrentRemainingStack() > 64, "lowStckOnEv");
#if EFI_PROD_CODE
	/**
	 * We need to push current value into the 64 bit counter often enough so that we do not miss an overflow
	 */
    /* Entering a reentrant critical zone.*/
    syssts_t sts = chSysGetStatusAndLockX();
	updateAndSet(&halTime.state, getTimeNowLowerNt());
    /* Leaving the critical zone.*/
    chSysRestoreStatusX(sts);
	int timeSeconds = getTimeNowSeconds();
	if (previousSecond != timeSeconds) {
		previousSecond = timeSeconds;
		invokePerSecond();
	}
#endif /* EFI_PROD_CODE */

	/**
	 * Update engine RPM state if needed (check timeouts).
	 */
	bool isSpinning = engine->rpmCalculator.checkIfSpinning(getTimeNowNt() PASS_ENGINE_PARAMETER_SUFFIX);
	if (!isSpinning) {
		engine->rpmCalculator.setStopSpinning(PASS_ENGINE_PARAMETER_SIGNATURE);
	}

	if (engine->rpmCalculator.isStopped(PASS_ENGINE_PARAMETER_SIGNATURE)) {
#if EFI_INTERNAL_FLASH
		writeToFlashIfPending();
#endif /* EFI_INTERNAL_FLASH */
		resetAccel();
	} else {
		updatePrimeInjectionPulseState(PASS_ENGINE_PARAMETER_SIGNATURE);
	}

	if (engine->versionForConfigurationListeners.isOld(engine->getGlobalConfigurationVersion())) {
		updateAccelParameters();
	}

	engine->periodicSlowCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
#endif
}

void initPeriodicEvents(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	slowController.Start();
	fastController.Start();
}

char * getPinNameByAdcChannel(const char *msg, adc_channel_e hwChannel, char *buffer) {
#if HAL_USE_ADC
	if (hwChannel == EFI_ADC_NONE) {
		strcpy(buffer, "NONE");
	} else {
		strcpy((char*) buffer, portname(getAdcChannelPort(msg, hwChannel)));
		itoa10(&buffer[2], getAdcChannelPin(hwChannel));
	}
#else
	strcpy(buffer, "NONE");
#endif
	return (char*) buffer;
}

static char pinNameBuffer[16];

#if HAL_USE_ADC
extern AdcDevice fastAdc;
#endif

static void printAnalogChannelInfoExt(const char *name, adc_channel_e hwChannel, float adcVoltage,
		float dividerCoeff) {
#if HAL_USE_ADC
	if (hwChannel == EFI_ADC_NONE) {
		scheduleMsg(&logger, "ADC is not assigned for %s", name);
		return;
	}

	if (fastAdc.isHwUsed(hwChannel)) {
		scheduleMsg(&logger, "fast enabled=%s", boolToString(CONFIGB(isFastAdcEnabled)));
	}

	float voltage = adcVoltage * dividerCoeff;
	scheduleMsg(&logger, "%s ADC%d %s %s adc=%.2f/input=%.2fv/divider=%.2f", name, hwChannel, getAdcMode(hwChannel),
			getPinNameByAdcChannel(name, hwChannel, pinNameBuffer), adcVoltage, voltage, dividerCoeff);
#endif
}

static void printAnalogChannelInfo(const char *name, adc_channel_e hwChannel) {
#if HAL_USE_ADC
	printAnalogChannelInfoExt(name, hwChannel, getVoltage("print", hwChannel), engineConfiguration->analogInputDividerCoefficient);
#endif
}

static void printAnalogInfo(void) {
	scheduleMsg(&logger, "analogInputDividerCoefficient: %.2f", engineConfiguration->analogInputDividerCoefficient);

	printAnalogChannelInfo("hip9011", engineConfiguration->hipOutputChannel);
	printAnalogChannelInfo("fuel gauge", engineConfiguration->fuelLevelSensor);
	printAnalogChannelInfo("TPS", engineConfiguration->tps1_1AdcChannel);
	printAnalogChannelInfo("pPS", engineConfiguration->throttlePedalPositionAdcChannel);
	if (engineConfiguration->clt.adcChannel != EFI_ADC_NONE) {
		printAnalogChannelInfo("CLT", engineConfiguration->clt.adcChannel);
	}
	if (engineConfiguration->iat.adcChannel != EFI_ADC_NONE) {
		printAnalogChannelInfo("IAT", engineConfiguration->iat.adcChannel);
	}
	if (hasMafSensor()) {
		printAnalogChannelInfo("MAF", engineConfiguration->mafAdcChannel);
	}
	for (int i = 0; i < FSIO_ANALOG_INPUT_COUNT ; i++) {
		adc_channel_e ch = engineConfiguration->fsioAdc[i];
		if (ch != EFI_ADC_NONE) {
			printAnalogChannelInfo("fsio", ch);
		}
	}

	printAnalogChannelInfo("AFR", engineConfiguration->afr.hwChannel);
	if (hasMapSensor(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		printAnalogChannelInfo("MAP", engineConfiguration->map.sensor.hwChannel);
	}
	if (hasBaroSensor(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		printAnalogChannelInfo("BARO", engineConfiguration->baroSensor.hwChannel);
	}
	if (engineConfiguration->externalKnockSenseAdc != EFI_ADC_NONE) {
		printAnalogChannelInfo("extKno", engineConfiguration->externalKnockSenseAdc);
	}

	printAnalogChannelInfo("OilP", engineConfiguration->oilPressure.hwChannel);

	printAnalogChannelInfo("A/C sw", engineConfiguration->acSwitchAdc);
	printAnalogChannelInfo("HIP9011", engineConfiguration->hipOutputChannel);
	printAnalogChannelInfoExt("Vbatt", engineConfiguration->vbattAdcChannel, getVoltage("vbatt", engineConfiguration->vbattAdcChannel),
			engineConfiguration->vbattDividerCoeff);
}

#define isOutOfBounds(offset) ((offset<0) || (offset) >= (int) sizeof(engine_configuration_s))

static void getShort(int offset) {
	if (isOutOfBounds(offset))
		return;
	uint16_t *ptr = (uint16_t *) (&((char *) engineConfiguration)[offset]);
	uint16_t value = *ptr;
	/**
	 * this response is part of rusEfi console API
	 */
	scheduleMsg(&logger, "short%s%d is %d", CONSOLE_DATA_PROTOCOL_TAG, offset, value);
}

static void getByte(int offset) {
	if (isOutOfBounds(offset))
		return;
	uint8_t *ptr = (uint8_t *) (&((char *) engineConfiguration)[offset]);
	uint8_t value = *ptr;
	/**
	 * this response is part of rusEfi console API
	 */
	scheduleMsg(&logger, "byte%s%d is %d", CONSOLE_DATA_PROTOCOL_TAG, offset, value);
}

static void onConfigurationChanged() {
#if EFI_TUNER_STUDIO
	// on start-up rusEfi would read from working copy of TS while
	// we have a lot of console commands which write into real copy of configuration directly
	// we have a bit of a mess here
	syncTunerStudioCopy();
#endif /* EFI_TUNER_STUDIO */
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
}

static void setBit(const char *offsetStr, const char *bitStr, const char *valueStr) {
	int offset = atoi(offsetStr);
	if (absI(offset) == absI(ERROR_CODE)) {
		scheduleMsg(&logger, "invalid offset [%s]", offsetStr);
		return;
	}
	if (isOutOfBounds(offset)) {
		return;
	}
	int bit = atoi(bitStr);
	if (absI(bit) == absI(ERROR_CODE)) {
		scheduleMsg(&logger, "invalid bit [%s]", bitStr);
		return;
	}
	int value = atoi(valueStr);
	if (absI(value) == absI(ERROR_CODE)) {
		scheduleMsg(&logger, "invalid value [%s]", valueStr);
		return;
	}
	int *ptr = (int *) (&((char *) engineConfiguration)[offset]);
	*ptr ^= (-value ^ *ptr) & (1 << bit);
	/**
	 * this response is part of rusEfi console API
	 */
	scheduleMsg(&logger, "bit%s%d/%d is %d", CONSOLE_DATA_PROTOCOL_TAG, offset, bit, value);
	onConfigurationChanged();
}

static void setShort(const int offset, const int value) {
	if (isOutOfBounds(offset))
		return;
	uint16_t *ptr = (uint16_t *) (&((char *) engineConfiguration)[offset]);
	*ptr = (uint16_t) value;
	getShort(offset);
	onConfigurationChanged();
}

static void setByte(const int offset, const int value) {
	if (isOutOfBounds(offset))
		return;
	uint8_t *ptr = (uint8_t *) (&((char *) engineConfiguration)[offset]);
	*ptr = (uint8_t) value;
	getByte(offset);
	onConfigurationChanged();
}

static void getBit(int offset, int bit) {
	if (isOutOfBounds(offset))
		return;
	int *ptr = (int *) (&((char *) engineConfiguration)[offset]);
	int value = (*ptr >> bit) & 1;
	/**
	 * this response is part of rusEfi console API
	 */
	scheduleMsg(&logger, "bit%s%d/%d is %d", CONSOLE_DATA_PROTOCOL_TAG, offset, bit, value);
}

static void getInt(int offset) {
	if (isOutOfBounds(offset))
		return;
	int *ptr = (int *) (&((char *) engineConfiguration)[offset]);
	int value = *ptr;
	/**
	 * this response is part of rusEfi console API
	 */
	scheduleMsg(&logger, "int%s%d is %d", CONSOLE_DATA_PROTOCOL_TAG, offset, value);
}

static void setInt(const int offset, const int value) {
	if (isOutOfBounds(offset))
		return;
	int *ptr = (int *) (&((char *) engineConfiguration)[offset]);
	*ptr = value;
	getInt(offset);
	onConfigurationChanged();
}

static void getFloat(int offset) {
	if (isOutOfBounds(offset))
		return;
	float *ptr = (float *) (&((char *) engineConfiguration)[offset]);
	float value = *ptr;
	/**
	 * this response is part of rusEfi console API
	 */
	scheduleMsg(&logger, "float%s%d is %.5f", CONSOLE_DATA_PROTOCOL_TAG, offset, value);
}

static void setFloat(const char *offsetStr, const char *valueStr) {
	int offset = atoi(offsetStr);
	if (absI(offset) == absI(ERROR_CODE)) {
		scheduleMsg(&logger, "invalid offset [%s]", offsetStr);
		return;
	}
	if (isOutOfBounds(offset))
		return;
	float value = atoff(valueStr);
	if (cisnan(value)) {
		scheduleMsg(&logger, "invalid value [%s]", valueStr);
		return;
	}
	float *ptr = (float *) (&((char *) engineConfiguration)[offset]);
	*ptr = value;
	getFloat(offset);
	onConfigurationChanged();
}

#if EFI_ENABLE_MOCK_ADC

void setMockCltVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setMockVoltage(engineConfiguration->clt.adcChannel, voltage);
}

void setMockIatVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setMockVoltage(engineConfiguration->iat.adcChannel, voltage);
}

void setMockMafVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setMockVoltage(engineConfiguration->mafAdcChannel, voltage);
}

void setMockAfrVoltage(float voltage) {
	setMockVoltage(engineConfiguration->afr.hwChannel, voltage);
}

void setMockThrottlePedalSensorVoltage(float voltage) {
	setMockVoltage(engineConfiguration->throttlePedalPositionAdcChannel, voltage);
}

void setMockThrottlePositionSensorVoltage(float voltage) {
	setMockVoltage(engineConfiguration->tps1_1AdcChannel, voltage);
}

void setMockMapVoltage(float voltage) {
	setMockVoltage(engineConfiguration->map.sensor.hwChannel, voltage);
}

void setMockVBattVoltage(float voltage) {
	setMockVoltage(engineConfiguration->vbattAdcChannel, voltage);
}

static void initMockVoltage(void) {
#if EFI_SIMULATOR
	setMockCltVoltage(2);
#endif /* EFI_SIMULATOR */

#if EFI_SIMULATOR
	setMockIatVoltage(2);
#endif /* EFI_SIMULATOR */

}

#endif /* EFI_ENABLE_MOCK_ADC */

static void initConfigActions(void) {
	addConsoleActionSS("set_float", (VoidCharPtrCharPtr) setFloat);
	addConsoleActionII("set_int", (VoidIntInt) setInt);
	addConsoleActionII("set_short", (VoidIntInt) setShort);
	addConsoleActionII("set_byte", (VoidIntInt) setByte);
	addConsoleActionSSS("set_bit", setBit);

	addConsoleActionI("get_float", getFloat);
	addConsoleActionI("get_int", getInt);
	addConsoleActionI("get_short", getShort);
	addConsoleActionI("get_byte", getByte);
	addConsoleActionII("get_bit", getBit);
}

// todo: move this logic somewhere else?
static void getKnockInfo(void) {
	adc_channel_e hwChannel = engineConfiguration->externalKnockSenseAdc;
	scheduleMsg(&logger, "externalKnockSenseAdc on ADC", getPinNameByAdcChannel("knock", hwChannel, pinNameBuffer));

	engine->printKnockState();
}

// this method is used by real firmware and simulator but not unit tests
void commonInitEngineController(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_SIMULATOR
	printf("commonInitEngineController\n");
#endif
	initConfigActions();
#if EFI_ENABLE_MOCK_ADC
	initMockVoltage();
#endif /* EFI_ENABLE_MOCK_ADC */

#if EFI_SENSOR_CHART
	initSensorChart();
#endif /* EFI_SENSOR_CHART */

#if EFI_PROD_CODE || EFI_SIMULATOR
	// todo: this is a mess, remove code duplication with simulator
	initSettings();
#endif

#if EFI_TUNER_STUDIO
	if (engineConfiguration->isTunerStudioEnabled) {
		startTunerStudioConnectivity();
	}
#endif /* EFI_TUNER_STUDIO */

	if (hasFirmwareError()) {
		return;
	}
	mostCommonInitEngineController(sharedLogger PASS_ENGINE_PARAMETER_SUFFIX);
}

void initEngineContoller(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_SIMULATOR
	printf("initEngineContoller\n");
#endif
	addConsoleAction("analoginfo", printAnalogInfo);
	commonInitEngineController(sharedLogger);

#if EFI_PROD_CODE
	initPwmGenerator();
#endif

	initAlgo(sharedLogger);

#if EFI_WAVE_ANALYZER
	if (engineConfiguration->isWaveAnalyzerEnabled) {
		initWaveAnalyzer(sharedLogger);
	}
#endif /* EFI_WAVE_ANALYZER */

#if EFI_CJ125
	/**
	 * this uses SimplePwm which depends on scheduler, has to be initialized after scheduler
	 */
	initCJ125(sharedLogger PASS_ENGINE_PARAMETER_SUFFIX);
#endif /* EFI_CJ125 */


#if EFI_SHAFT_POSITION_INPUT
	/**
	 * there is an implicit dependency on the fact that 'tachometer' listener is the 1st listener - this case
	 * other listeners can access current RPM value
	 */
	initRpmCalculator(sharedLogger PASS_ENGINE_PARAMETER_SUFFIX);
#endif /* EFI_SHAFT_POSITION_INPUT */

#if EFI_PROD_CODE && EFI_ENGINE_CONTROL
	initInjectorCentral(sharedLogger);
#endif /* EFI_PROD_CODE && EFI_ENGINE_CONTROL */

// multiple issues with this	initMapAdjusterThread();
	// periodic events need to be initialized after fuel&spark pins to avoid a warning
	initPeriodicEvents(PASS_ENGINE_PARAMETER_SIGNATURE);

	if (hasFirmwareError()) {
		return;
	}

	engineStateBlinkingTask.Start();

#if EFI_PROD_CODE && EFI_ENGINE_CONTROL
	/**
	 * This has to go after 'initInjectorCentral' and 'initInjectorCentral' in order to
	 * properly detect un-assigned output pins
	 */
	prepareShapes(PASS_ENGINE_PARAMETER_SIGNATURE);
#endif /* EFI_PROD_CODE && EFI_ENGINE_CONTROL */

#if EFI_PWM_TESTER
	initPwmTester();
#endif /* EFI_PWM_TESTER */

	initMalfunctionCentral();

#if EFI_ALTERNATOR_CONTROL
	initAlternatorCtrl(sharedLogger);
#endif

#if EFI_AUX_PID
	initAuxPid(sharedLogger);
#endif

#if EFI_ELECTRONIC_THROTTLE_BODY
	initElectronicThrottle();
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

#if EFI_MALFUNCTION_INDICATOR
	initMalfunctionIndicator();
#endif /* EFI_MALFUNCTION_INDICATOR */

#if EFI_MAP_AVERAGING
	if (engineConfiguration->isMapAveragingEnabled) {
		initMapAveraging(sharedLogger, engine);
	}
#endif /* EFI_MAP_AVERAGING */

	initEgoAveraging(PASS_ENGINE_PARAMETER_SIGNATURE);

#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT
	if (CONFIGB(isEngineControlEnabled)) {
		/**
		 * This method initialized the main listener which actually runs injectors & ignition
		 */
		initMainEventListener(sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);
	}
#endif /* EFI_ENGINE_CONTROL */

	if (engineConfiguration->externalKnockSenseAdc != EFI_ADC_NONE) {
		addConsoleAction("knockinfo", getKnockInfo);
	}

#if EFI_PROD_CODE
	addConsoleAction("reset_accel", resetAccel);
#endif /* EFI_PROD_CODE */

#if EFI_HD44780_LCD
	initLcdController();
#endif /* EFI_HD44780_LCD */

#if EFI_PROD_CODE
	initTachometer();
#endif /* EFI_PROD_CODE */
}

// these two variables are here only to let us know how much RAM is available, also these
// help to notice when RAM usage goes up - if a code change adds to RAM usage these variables would fail
// linking process which is the way to raise the alarm
#ifndef RAM_UNUSED_SIZE
#define RAM_UNUSED_SIZE 19000
#endif
#ifndef CCM_UNUSED_SIZE
#define CCM_UNUSED_SIZE 4600
#endif
static char UNUSED_RAM_SIZE[RAM_UNUSED_SIZE];
static char UNUSED_CCM_SIZE[CCM_UNUSED_SIZE] CCM_OPTIONAL;

/**
 * See also VCS_VERSION
 */
int getRusEfiVersion(void) {
	if (UNUSED_RAM_SIZE[0] != 0)
		return 123; // this is here to make the compiler happy about the unused array
	if (UNUSED_CCM_SIZE[0] * 0 != 0)
		return 3211; // this is here to make the compiler happy about the unused array
#if defined(EFI_BOOTLOADER_INCLUDE_CODE)
	// make bootloader code happy too
	if (initBootloader() != 0)
		return 123;
#endif /* EFI_BOOTLOADER_INCLUDE_CODE */
	return 20190919;
}
#endif /* EFI_UNIT_TEST */
