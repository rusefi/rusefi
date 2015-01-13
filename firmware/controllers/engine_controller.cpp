/**
 * @file    engine_controller.cpp
 * @brief   Controllers package entry point code
 *
 *
 *
 * @date Feb 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
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
#include "analog_chart.h"
#include "electronic_throttle.h"
#include "malfunction_indicator.h"
#include "map_averaging.h"
#include "malfunction_central.h"
#include "pin_repository.h"
#include "pwm_generator.h"
#include "adc_inputs.h"
#include "algo.h"
#include "efilib2.h"
#include "PwmTester.h"
#include "engine.h"
#include "pin_repository.h"
#include "pwm_generator.h"
#include "lcd_controller.h"

extern OutputPin outputs[IO_PIN_COUNT];
extern bool hasFirmwareErrorFlag;

persistent_config_container_s persistentState CCM_OPTIONAL;

/**
 * todo: it really looks like these fields should become 'static', i.e. private
 * the whole 'extern ...' pattern is less then perfect, I guess the 'God object' Engine
 * would be a smaller evil. Whatever is needed should be passed into methods/modules/files as an explicit parameter.
 */
engine_configuration_s *engineConfiguration = &persistentState.persistentConfiguration.engineConfiguration;
board_configuration_s *boardConfiguration = &persistentState.persistentConfiguration.engineConfiguration.bc;

/**
 * CH_FREQUENCY is the number of system ticks in a second
 */

static VirtualTimer everyMsTimer;

static Logging logger;

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
static engine_configuration2_s ec2 CCM_OPTIONAL;
engine_configuration2_s * engineConfiguration2 = &ec2;
#endif

#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)

/**
 * todo: this should probably become 'static', i.e. private, and propagated around explicitly?
 */
Engine _engine;
Engine * engine = &_engine;
#endif

/**
 * I am not sure if this needs to be configurable.
 *
 * Also technically the whole feature might be implemented as cranking fuel coefficient curve by TPS.
 */
#define CLEANUP_MODE_TPS 95

extern OutputPin runningPin;

static msg_t csThread(void) {
	chRegSetThreadName("status");
#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
	while (true) {
		int rpm = getRpm();
		int is_cranking = isCrankingR(rpm);
		int is_running = rpm > 0 && !is_cranking;
		if (is_running) {
			// blinking while running
			runningPin.setValue(0);
			chThdSleepMilliseconds(50);
			runningPin.setValue(1);
			chThdSleepMilliseconds(50);
		} else {
			// constant on while cranking and off if engine is stopped
			runningPin.setValue(is_cranking);
			chThdSleepMilliseconds(100);
		}
	}
#endif /* EFI_SHAFT_POSITION_INPUT */
	return -1;
}

static void updateErrorCodes(void) {
	/**
	 * technically we can set error codes right inside the getMethods, but I a bit on a fence about it
	 */
	setError(isValidIntakeAirTemperature(getIntakeAirTemperature(engine)),
			OBD_Intake_Air_Temperature_Circuit_Malfunction);
	setError(isValidCoolantTemperature(getCoolantTemperature(engine)),
			OBD_Engine_Coolant_Temperature_Circuit_Malfunction);
}

//static void fanRelayControl(void) {
//	if (boardConfiguration->fanPin == GPIO_UNASSIGNED)
//		return;
//
//	int isCurrentlyOn = getLogicPinValue(&en);
//	int newValue;
//	if (isCurrentlyOn) {
//		// if the fan is already on, we keep it on till the 'fanOff' temperature
//		newValue = getCoolantTemperature(engine) > engineConfiguration->fanOffTemperature;
//	} else {
//		newValue = getCoolantTemperature(engine) > engineConfiguration->fanOnTemperature;
//	}
//
//	if (isCurrentlyOn != newValue) {
//		if (isRunningBenchTest())
//			return; // let's not mess with bench testing
//		scheduleMsg(&logger, "FAN relay: %s", newValue ? "ON" : "OFF");
//		setOutputPinValue(FAN_RELAY, newValue);
//	}
//}

Overflow64Counter halTime;

efitimeus_t getTimeNowUs(void) {
	return getTimeNowNt() / (CORE_CLOCK / 1000000);
}

efitick_t getTimeNowNt(void) {
	return halTime.get();
}

efitimems_t currentTimeMillis(void) {
	// todo: migrate to getTimeNowUs? or not?
	return chTimeNow() / TICKS_IN_MS;
}

int getTimeNowSeconds(void) {
	return chTimeNow() / CH_FREQUENCY;
}

static void cylinderCleanupControl(Engine *engine) {
	bool newValue;
	if (engineConfiguration->isCylinderCleanupEnabled) {
		newValue = isCrankingE(engine) && getTPS(PASS_ENGINE_PARAMETER_F) > CLEANUP_MODE_TPS;
	} else {
		newValue = false;
	}
	if (newValue != engine->isCylinderCleanupMode) {
		engine->isCylinderCleanupMode = newValue;
		scheduleMsg(&logger, "isCylinderCleanupMode %s", boolToString(newValue));
	}
}

static void onEvenyGeneralMilliseconds(Engine *engine) {
	/**
	 * We need to push current value into the 64 bit counter often enough so that we do not miss an overflow
	 */
	bool alreadyLocked = lockAnyContext();
	updateAndSet(&halTime.state, hal_lld_get_counter_value());
	if (!alreadyLocked) {
		unlockAnyContext();
	}

	if (!engine->rpmCalculator.isRunning())
		writeToFlashIfPending();

	engine->watchdog();
	engine->updateSlowSensors();

	runFsio();

	updateErrorCodes();

	cylinderCleanupControl(engine);

	// schedule next invocation
	chVTSetAny(&everyMsTimer, boardConfiguration->generalPeriodicThreadPeriod * TICKS_IN_MS,
			(vtfunc_t) &onEvenyGeneralMilliseconds, engine);
}

static void initPeriodicEvents(Engine *engine) {
	// schedule first invocation
	chVTSetAny(&everyMsTimer, boardConfiguration->generalPeriodicThreadPeriod * TICKS_IN_MS,
			(vtfunc_t) &onEvenyGeneralMilliseconds, engine);
}

char * getPinNameByAdcChannel(adc_channel_e hwChannel, char *buffer) {
#if HAL_USE_ADC || defined(__DOXYGEN__)
	strcpy((char*) buffer, portname(getAdcChannelPort(hwChannel)));
	itoa10(&buffer[2], getAdcChannelPin(hwChannel));
#else
	strcpy(buffer, "NONE");
#endif
	return (char*) buffer;
}

static char pinNameBuffer[16];

static void printAnalogChannelInfoExt(const char *name, adc_channel_e hwChannel, float adcVoltage) {
#if HAL_USE_ADC || defined(__DOXYGEN__)
	float voltage = adcVoltage * engineConfiguration->analogInputDividerCoefficient;
	scheduleMsg(&logger, "%s ADC%d %s %s rawValue=%f/divided=%fv", name, hwChannel, getAdcMode(hwChannel),
			getPinNameByAdcChannel(hwChannel, pinNameBuffer), adcVoltage, voltage);
#endif
}

static void printAnalogChannelInfo(const char *name, adc_channel_e hwChannel) {
#if HAL_USE_ADC || defined(__DOXYGEN__)
	if (hwChannel != EFI_ADC_NONE) {
		printAnalogChannelInfoExt(name, hwChannel, getVoltage(hwChannel));
	}
#endif
}

static void printAnalogInfo(void) {
	printAnalogChannelInfo("hip9011", EFI_ADC_10);
	printAnalogChannelInfo("TPS", engineConfiguration->tpsAdcChannel);
	printAnalogChannelInfo("CLT", engineConfiguration->cltAdcChannel);
	if (engineConfiguration->hasIatSensor) {
		printAnalogChannelInfo("IAT", engineConfiguration->iatAdcChannel);
	}
	printAnalogChannelInfo("MAF", engineConfiguration->mafAdcChannel);
	printAnalogChannelInfo("AFR", engineConfiguration->afrSensor.afrAdcChannel);
	printAnalogChannelInfo("MAP", engineConfiguration->map.sensor.hwChannel);
	if (engineConfiguration->hasBaroSensor) {
		printAnalogChannelInfo("BARO", engineConfiguration->baroSensor.hwChannel);
	}
	printAnalogChannelInfo("A/C sw", engineConfiguration->acSwitchAdc);
	printAnalogChannelInfoExt("Vbatt", engineConfiguration->vbattAdcChannel, getVBatt(engineConfiguration));
}

static THD_WORKING_AREA(csThreadStack, UTILITY_THREAD_STACK_SIZE);	// declare thread stack

#define isOutOfBounds(offset) ((offset<0) || (offset) >= sizeof(engine_configuration_s))

static void setInt(const int offset, const int value) {
	if (isOutOfBounds(offset))
		return;
	int *ptr = (int *) (&((char *) engine->engineConfiguration)[offset]);
	*ptr = value;
	scheduleMsg(&logger, "setting int @%d to %d", offset, value);
}

static void getInt(int offset) {
	if (isOutOfBounds(offset))
		return;
	int *ptr = (int *) (&((char *) engine->engineConfiguration)[offset]);
	int value = *ptr;
	scheduleMsg(&logger, "int @%d is %d", offset, value);
}

static void getFloat(int offset) {
	if (isOutOfBounds(offset))
		return;
	float *ptr = (float *) (&((char *) engine->engineConfiguration)[offset]);
	float value = *ptr;
	scheduleMsg(&logger, "float @%d is %f", offset, value);
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
	scheduleMsg(&logger, "setting float @%d to %f", offset, value);
}

void initEngineContoller(Engine *engine) {
	if (hasFirmwareError()) {
		return;
	}
	initLogging(&logger, "Engine Controller");

	initSensors(engine);

	initPwmGenerator();

#if EFI_ANALOG_CHART
	initAnalogChart();
#endif /* EFI_ANALOG_CHART */

	initAlgo(engineConfiguration);

#if EFI_WAVE_ANALYZER
	if (engineConfiguration->isWaveAnalyzerEnabled) {
		initWaveAnalyzer();
	}
#endif /* EFI_WAVE_ANALYZER */

#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
	/**
	 * there is an implicit dependency on the fact that 'tachometer' listener is the 1st listener - this case
	 * other listeners can access current RPM value
	 */
	initRpmCalculator(engine);
#endif /* EFI_SHAFT_POSITION_INPUT */

#if EFI_TUNER_STUDIO
	if (engineConfiguration->isTunerStudioEnabled) {
		startTunerStudioConnectivity();
	}
#endif

// multiple issues with this	initMapAdjusterThread();
	initPeriodicEvents(engine);

	chThdCreateStatic(csThreadStack, sizeof(csThreadStack), LOWPRIO, (tfunc_t) csThread, NULL);

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
	initInjectorCentral(engine);
	initIgnitionCentral();
#endif

#if EFI_PWM_TESTER
	initPwmTester();
#endif

	/**
	 * This has to go after 'initInjectorCentral' and 'initInjectorCentral' in order to
	 * properly detect un-assigned output pins
	 */
	prepareShapes(engine);

	initMalfunctionCentral();

#if EFI_ELECTRONIC_THROTTLE_BODY
	initElectronicThrottle();
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

#if EFI_MALFUNCTION_INDICATOR || defined(__DOXYGEN__)
	if (engineConfiguration->isMilEnabled) {
		initMalfunctionIndicator(engine);
	}
#endif /* EFI_MALFUNCTION_INDICATOR */

#if EFI_MAP_AVERAGING || defined(__DOXYGEN__)
	if (engineConfiguration->isMapAveragingEnabled) {
		initMapAveraging(engine);
	}
#endif /* EFI_MAP_AVERAGING */

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
	if (boardConfiguration->isEngineControlEnabled) {
		/**
		 * This method initialized the main listener which actually runs injectors & ignition
		 */
		initMainEventListener(engine);
	}
#endif /* EFI_ENGINE_CONTROL */

#if EFI_IDLE_CONTROL || defined(__DOXYGEN__)
	if (engineConfiguration->isIdleThreadEnabled) {
		startIdleThread(engine);
	}
#endif

	addConsoleAction("analoginfo", printAnalogInfo);

	addConsoleActionSS("set_float", (VoidCharPtrCharPtr) setFloat);
	addConsoleActionII("set_int", (VoidIntInt) setInt);
	addConsoleActionI("get_float", getFloat);
	addConsoleActionI("get_int", getInt);

	initFsioImpl(engine);

	initLcdController();
}
