/**
 * @file    engine_controller.cpp
 * @brief   Controllers package entry point code
 *
 *
 *
 * @date Feb 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
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

#include "sensor_chart.h"
#include "main.h"
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
#include "tunerstudio.h"
#include "injector_central.h"
#include "rfiutil.h"
#include "engine_math.h"
#include "wave_analyzer.h"
#include "allsensors.h"
#include "electronic_throttle.h"
#include "map_averaging.h"
#include "malfunction_central.h"
#include "malfunction_indicator.h"
#include "engine.h"
#include "algo.h"
#include "LocalVersionHolder.h"
#include "alternatorController.h"
#include "fuel_math.h"
#include "settings.h"
#include "aux_pid.h"

#if HAL_USE_ADC || defined(__DOXYGEN__)
#include "AdcConfiguration.h"
#endif /* HAL_USE_ADC */

#if EFI_PROD_CODE || defined(__DOXYGEN__)
#include "pwm_generator.h"
#include "adc_inputs.h"
#include "efilib2.h"
#include "PwmTester.h"
#include "pwm_generator.h"
#include "lcd_controller.h"
#include "pin_repository.h"
#include "tachometer.h"
#endif /* EFI_PROD_CODE */

extern bool hasFirmwareErrorFlag;
extern EnginePins enginePins;

persistent_config_container_s persistentState CCM_OPTIONAL;

const persistent_config_s *config = &persistentState.persistentConfiguration;

/**
 * todo: it really looks like these fields should become 'static', i.e. private
 * the whole 'extern ...' pattern is less then perfect, I guess the 'God object' Engine
 * would be a smaller evil. Whatever is needed should be passed into methods/modules/files as an explicit parameter.
 */
const engine_configuration_s *engineConfiguration = &persistentState.persistentConfiguration.engineConfiguration;
const board_configuration_s *boardConfiguration = &persistentState.persistentConfiguration.engineConfiguration.bc;

/**
 * CH_FREQUENCY is the number of system ticks in a second
 */

static virtual_timer_t periodicSlowTimer;
static virtual_timer_t periodicFastTimer;

static LoggingWithStorage logger("Engine Controller");

#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)

/**
 * todo: this should probably become 'static', i.e. private, and propagated around explicitly?
 */
Engine _engine CCM_OPTIONAL;
Engine * engine = &_engine;
#endif /* EFI_PROD_CODE */

/**
 * I am not sure if this needs to be configurable.
 *
 * Also technically the whole feature might be implemented as cranking fuel coefficient curve by TPS.
 */
#define CLEANUP_MODE_TPS 90

static msg_t csThread(void) {
	chRegSetThreadName("status");
#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
	while (true) {
		int rpm = getRpmE(engine);
		int is_cranking = isCrankingR(rpm);
		int is_running = rpm > 0 && !is_cranking;
		if (is_running) {
			// blinking while running
			enginePins.runningPin.setValue(0);
			chThdSleepMilliseconds(50);
			enginePins.runningPin.setValue(1);
			chThdSleepMilliseconds(50);
		} else {
			// constant on while cranking and off if engine is stopped
			enginePins.runningPin.setValue(is_cranking);
			chThdSleepMilliseconds(100);
		}
	}
#endif /* EFI_SHAFT_POSITION_INPUT */
	return -1;
}

#if EFI_PROD_CODE || defined(__DOXYGEN__)
Overflow64Counter halTime;

/**
 * 64-bit result would not overflow, but that's complex stuff for our 32-bit MCU
 */
//todo: macro to save method invocation
efitimeus_t getTimeNowUs(void) {
	return getTimeNowNt() / (CORE_CLOCK / 1000000);
}

//todo: macro to save method invocation
efitick_t getTimeNowNt(void) {
	return halTime.get();
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

static void cylinderCleanupControl(Engine *engine) {
#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
	bool newValue;
	if (engineConfiguration->isCylinderCleanupEnabled) {
		newValue = !engine->rpmCalculator.isRunning(PASS_ENGINE_PARAMETER_F) && getTPS(PASS_ENGINE_PARAMETER_F) > CLEANUP_MODE_TPS;
	} else {
		newValue = false;
	}
	if (newValue != engine->isCylinderCleanupMode) {
		engine->isCylinderCleanupMode = newValue;
		scheduleMsg(&logger, "isCylinderCleanupMode %s", boolToString(newValue));
	}
#endif
}

static LocalVersionHolder versionForConfigurationListeners;

static void periodicSlowCallback(Engine *engine);

static void scheduleNextSlowInvocation(void) {
	// schedule next invocation
	int periodMs = boardConfiguration->generalPeriodicThreadPeriod;
	if (periodMs == 0)
		periodMs = 50; // this might happen while resetting configuration
	chVTSetAny(&periodicSlowTimer, MS2ST(periodMs), (vtfunc_t) &periodicSlowCallback, engine);
}

static void periodicFastCallback(DECLARE_ENGINE_PARAMETER_F) {
	engine->periodicFastCallback();

	chVTSetAny(&periodicFastTimer, MS2ST(20), (vtfunc_t) &periodicFastCallback, engine);
}

static void resetAccel(void) {
	engine->engineLoadAccelEnrichment.reset();
	engine->tpsAccelEnrichment.reset();
	engine->wallFuel.reset();
}

static void periodicSlowCallback(Engine *engine) {
	efiAssertVoid(getRemainingStack(chThdGetSelfX()) > 64, "lowStckOnEv");
#if EFI_PROD_CODE
	/**
	 * We need to push current value into the 64 bit counter often enough so that we do not miss an overflow
	 */
	bool alreadyLocked = lockAnyContext();
	updateAndSet(&halTime.state, port_rt_get_counter_value());
	if (!alreadyLocked) {
		unlockAnyContext();
	}
#endif

	if (!engine->rpmCalculator.isRunning()) {
#if (EFI_PROD_CODE && EFI_ENGINE_CONTROL && EFI_INTERNAL_FLASH) || defined(__DOXYGEN__)
		writeToFlashIfPending();
#endif
		resetAccel();
	}

	if (versionForConfigurationListeners.isOld()) {
		updateAccelParameters();
		engine->engineState.warmupAfrPid.reset();
	}

	engine->watchdog();
	engine->updateSlowSensors();

#if (EFI_PROD_CODE && EFI_FSIO) || defined(__DOXYGEN__)
	runFsio();
#endif

	cylinderCleanupControl(engine);

	scheduleNextSlowInvocation();
}

void initPeriodicEvents(DECLARE_ENGINE_PARAMETER_F) {
	scheduleNextSlowInvocation();
	periodicFastCallback(PASS_ENGINE_PARAMETER_F);
}

char * getPinNameByAdcChannel(const char *msg, adc_channel_e hwChannel, char *buffer) {
#if HAL_USE_ADC || defined(__DOXYGEN__)
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

#if HAL_USE_ADC || defined(__DOXYGEN__)
extern AdcDevice fastAdc;
#endif

static void printAnalogChannelInfoExt(const char *name, adc_channel_e hwChannel, float adcVoltage,
		float dividerCoeff) {
#if HAL_USE_ADC || defined(__DOXYGEN__)
	if (hwChannel == EFI_ADC_NONE) {
		scheduleMsg(&logger, "ADC is not assigned for %s", name);
		return;
	}

	if (fastAdc.isHwUsed(hwChannel)) {
		scheduleMsg(&logger, "fast enabled=%s", boolToString(boardConfiguration->isFastAdcEnabled));
	}

	float voltage = adcVoltage * dividerCoeff;
	scheduleMsg(&logger, "%s ADC%d %s %s adc=%f/input=%fv/divider=%f", name, hwChannel, getAdcMode(hwChannel),
			getPinNameByAdcChannel(name, hwChannel, pinNameBuffer), adcVoltage, voltage, dividerCoeff);
#endif
}

static void printAnalogChannelInfo(const char *name, adc_channel_e hwChannel) {
#if HAL_USE_ADC || defined(__DOXYGEN__)
	printAnalogChannelInfoExt(name, hwChannel, getVoltage("print", hwChannel), engineConfiguration->analogInputDividerCoefficient);
#endif
}

static void printAnalogInfo(void) {
	scheduleMsg(&logger, "analogInputDividerCoefficient: %f", engineConfiguration->analogInputDividerCoefficient);

	printAnalogChannelInfo("hip9011", engineConfiguration->hipOutputChannel);
	printAnalogChannelInfo("fuel gauge", engineConfiguration->fuelLevelSensor);
	printAnalogChannelInfo("TPS", engineConfiguration->tpsAdcChannel);
	printAnalogChannelInfo("pPS", engineConfiguration->pedalPositionChannel);
	if (engineConfiguration->clt.adcChannel != EFI_ADC_NONE) {
		printAnalogChannelInfo("CLT", engineConfiguration->clt.adcChannel);
	}
	if (engineConfiguration->iat.adcChannel != EFI_ADC_NONE) {
		printAnalogChannelInfo("IAT", engineConfiguration->iat.adcChannel);
	}
	if (hasMafSensor()) {
		printAnalogChannelInfo("MAF", engineConfiguration->mafAdcChannel);
	}
	for (int i = 0; i < FSIO_ADC_COUNT ; i++) {
		adc_channel_e ch = engineConfiguration->fsioAdc[i];
		if (ch != EFI_ADC_NONE) {
			printAnalogChannelInfo("fsio", ch);
		}
	}

	printAnalogChannelInfo("AFR", engineConfiguration->afr.hwChannel);
	if (hasMapSensor(PASS_ENGINE_PARAMETER_F)) {
		printAnalogChannelInfo("MAP", engineConfiguration->map.sensor.hwChannel);
	}
	if (hasBaroSensor(PASS_ENGINE_PARAMETER_F)) {
		printAnalogChannelInfo("BARO", engineConfiguration->baroSensor.hwChannel);
	}
	if (engineConfiguration->externalKnockSenseAdc != EFI_ADC_NONE) {
		printAnalogChannelInfo("extKno", engineConfiguration->externalKnockSenseAdc);
	}

	printAnalogChannelInfo("A/C sw", engineConfiguration->acSwitchAdc);
	printAnalogChannelInfo("HIP9011", engineConfiguration->hipOutputChannel);
	printAnalogChannelInfoExt("Vbatt", engineConfiguration->vbattAdcChannel, getVoltage("vbatt", engineConfiguration->vbattAdcChannel),
			engineConfiguration->vbattDividerCoeff);
}

static THD_WORKING_AREA(csThreadStack, UTILITY_THREAD_STACK_SIZE);	// declare thread stack

#define isOutOfBounds(offset) ((offset<0) || (offset) >= (int) sizeof(engine_configuration_s))

static void getShort(int offset) {
	if (isOutOfBounds(offset))
		return;
	uint16_t *ptr = (uint16_t *) (&((char *) engineConfiguration)[offset]);
	uint16_t value = *ptr;
	/**
	 * this response is part of dev console API
	 */
	scheduleMsg(&logger, "short @%d is %d", offset, value);
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
	 * this response is part of dev console API
	 */
	scheduleMsg(&logger, "bit @%d/%d is %d", offset, bit, value);
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_F);
}

static void setShort(const int offset, const int value) {
	if (isOutOfBounds(offset))
		return;
	uint16_t *ptr = (uint16_t *) (&((char *) engineConfiguration)[offset]);
	*ptr = (uint16_t) value;
	getShort(offset);
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_F);
}

static void getBit(int offset, int bit) {
	if (isOutOfBounds(offset))
		return;
	int *ptr = (int *) (&((char *) engineConfiguration)[offset]);
	int value = (*ptr >> bit) & 1;
	/**
	 * this response is part of dev console API
	 */
	scheduleMsg(&logger, "bit @%d/%d is %d", offset, bit, value);
}

static void getInt(int offset) {
	if (isOutOfBounds(offset))
		return;
	int *ptr = (int *) (&((char *) engineConfiguration)[offset]);
	int value = *ptr;
	/**
	 * this response is part of dev console API
	 */
	scheduleMsg(&logger, "int @%d is %d", offset, value);
}

static void setInt(const int offset, const int value) {
	if (isOutOfBounds(offset))
		return;
	int *ptr = (int *) (&((char *) engineConfiguration)[offset]);
	*ptr = value;
	getInt(offset);
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_F);
}

static void getFloat(int offset) {
	if (isOutOfBounds(offset))
		return;
	float *ptr = (float *) (&((char *) engineConfiguration)[offset]);
	float value = *ptr;
	/**
	 * this response is part of dev console API
	 */
	scheduleMsg(&logger, "float @%d is %..100000f", offset, value);
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
	float *ptr = (float *) (&((char *) engine->engineConfiguration)[offset]);
	*ptr = value;
	getFloat(offset);
}

#if EFI_ENABLE_MOCK_ADC || EFI_SIMULATOR

static void setMockVoltage(int hwChannel, float voltage) {
	engine->engineState.mockAdcState.setMockVoltage(hwChannel, voltage);
}

void setCltVoltage(float voltage) {
	setMockVoltage(engineConfiguration->clt.adcChannel, voltage);
}

void setIatVoltage(float voltage) {
	setMockVoltage(engineConfiguration->iat.adcChannel, voltage);
}

void setMafVoltage(float voltage) {
	setMockVoltage(engineConfiguration->mafAdcChannel, voltage);
}

void setAfrVoltage(float voltage) {
	setMockVoltage(engineConfiguration->afr.hwChannel, voltage);
}

void setTpsVoltage(float voltage) {
	setMockVoltage(engineConfiguration->tpsAdcChannel, voltage);
}

void setMapVoltage(float voltage) {
	setMockVoltage(engineConfiguration->map.sensor.hwChannel, voltage);
}

void setVBattVoltage(float voltage) {
	setMockVoltage(engineConfiguration->vbattAdcChannel, voltage);
}

static void initMockVoltage(void) {
#if EFI_SIMULATOR || defined(__DOXYGEN__)
	setCltVoltage(2);
#endif /* EFI_SIMULATOR */

#if EFI_SIMULATOR || defined(__DOXYGEN__)
	setIatVoltage(2);
#endif /* EFI_SIMULATOR */

}

#endif /* EFI_ENABLE_MOCK_ADC */

static void initConfigActions(void) {
	addConsoleActionSS("set_float", (VoidCharPtrCharPtr) setFloat);
	addConsoleActionII("set_int", (VoidIntInt) setInt);
	addConsoleActionII("set_short", (VoidIntInt) setShort);
	addConsoleActionSSS("set_bit", setBit);
	addConsoleActionI("get_float", getFloat);
	addConsoleActionI("get_int", getInt);
	addConsoleActionI("get_short", getShort);
	addConsoleActionII("get_bit", getBit);
}

// todo: move this logic somewhere else?
static void getKnockInfo(void) {
	adc_channel_e hwChannel = engineConfiguration->externalKnockSenseAdc;
	scheduleMsg(&logger, "externalKnockSenseAdc on ADC", getPinNameByAdcChannel("knock", hwChannel, pinNameBuffer));

	engine->printKnockState();
}

// this method is used by real firmware and simulator
void commonInitEngineController(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_S) {
	initConfigActions();
	initMockVoltage();

#if EFI_PROD_CODE || EFI_SIMULATOR || defined(__DOXYGEN__)
	initSignalExecutor();
#endif

#if EFI_SENSOR_CHART || defined(__DOXYGEN__)
	initSensorChart();
#endif /* EFI_SENSOR_CHART */

#if EFI_PROD_CODE || EFI_SIMULATOR || defined(__DOXYGEN__)
	// todo: this is a mess, remove code duplication with simulator
	initSettings(engineConfiguration);
#endif

#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
	if (engineConfiguration->isTunerStudioEnabled) {
		startTunerStudioConnectivity();
	}
#endif

	if (hasFirmwareError()) {
		return;
	}
	initSensors(sharedLogger PASS_ENGINE_PARAMETER_F);

#if EFI_FSIO || defined(__DOXYGEN__)
	initFsioImpl(sharedLogger PASS_ENGINE_PARAMETER);
#endif

	initAccelEnrichment(sharedLogger);

}

void initEngineContoller(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_S) {
	addConsoleAction("analoginfo", printAnalogInfo);
	commonInitEngineController(sharedLogger);



#if EFI_PROD_CODE || defined(__DOXYGEN__)
	initPwmGenerator();
#endif

	initAlgo(sharedLogger, engineConfiguration);

#if EFI_WAVE_ANALYZER || defined(__DOXYGEN__)
	if (engineConfiguration->isWaveAnalyzerEnabled) {
		initWaveAnalyzer(sharedLogger);
	}
#endif /* EFI_WAVE_ANALYZER */

#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
	/**
	 * there is an implicit dependency on the fact that 'tachometer' listener is the 1st listener - this case
	 * other listeners can access current RPM value
	 */
	initRpmCalculator(sharedLogger, engine);
#endif /* EFI_SHAFT_POSITION_INPUT */

#if (EFI_PROD_CODE && EFI_ENGINE_CONTROL) || defined(__DOXYGEN__)
	initInjectorCentral(sharedLogger);
#endif /* EFI_PROD_CODE && EFI_ENGINE_CONTROL */

// multiple issues with this	initMapAdjusterThread();
	// periodic events need to be initialized after fuel&spark pins to avoid a warning
	initPeriodicEvents(PASS_ENGINE_PARAMETER_F);

	if (hasFirmwareError()) {
		return;
	}

	chThdCreateStatic(csThreadStack, sizeof(csThreadStack), LOWPRIO, (tfunc_t) csThread, NULL);

#if (EFI_PROD_CODE && EFI_ENGINE_CONTROL) || defined(__DOXYGEN__)
	/**
	 * This has to go after 'initInjectorCentral' and 'initInjectorCentral' in order to
	 * properly detect un-assigned output pins
	 */
	prepareShapes(PASS_ENGINE_PARAMETER_F);
#endif /* EFI_PROD_CODE && EFI_ENGINE_CONTROL */

#if EFI_PWM_TESTER || defined(__DOXYGEN__)
	initPwmTester();
#endif /* EFI_PWM_TESTER */

	initMalfunctionCentral();

#if EFI_ALTERNATOR_CONTROL || defined(__DOXYGEN__)
	initAlternatorCtrl(sharedLogger);
#endif

#if EFI_AUX_PID || defined(__DOXYGEN__)
	initAuxPid(sharedLogger);
#endif

#if EFI_ELECTRONIC_THROTTLE_BODY || defined(__DOXYGEN__)
	initElectronicThrottle();
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

#if EFI_MALFUNCTION_INDICATOR || defined(__DOXYGEN__)
	initMalfunctionIndicator();
#endif /* EFI_MALFUNCTION_INDICATOR */

#if EFI_MAP_AVERAGING || defined(__DOXYGEN__)
	if (engineConfiguration->isMapAveragingEnabled) {
		initMapAveraging(sharedLogger, engine);
	}
#endif /* EFI_MAP_AVERAGING */

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
	if (boardConfiguration->isEngineControlEnabled) {
		/**
		 * This method initialized the main listener which actually runs injectors & ignition
		 */
		initMainEventListener(sharedLogger, engine);
	}
#endif /* EFI_ENGINE_CONTROL */

#if EFI_IDLE_CONTROL || defined(__DOXYGEN__)
	startIdleThread(sharedLogger, engine);
#endif /* EFI_IDLE_CONTROL */

	if (engineConfiguration->externalKnockSenseAdc != EFI_ADC_NONE) {
		addConsoleAction("knockinfo", getKnockInfo);
	}

#if EFI_PROD_CODE
	addConsoleAction("reset_accel", resetAccel);
#endif /* EFI_PROD_CODE */

#if EFI_HD44780_LCD || defined(__DOXYGEN__)
	initLcdController();
#endif /* EFI_HD44780_LCD */

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	initTachometer();
#endif /* EFI_PROD_CODE */
}
