/**
 * @file	status_loop.cpp
 * @brief Human-readable protocol status messages
 *
 * http://rusefi.com/forum/viewtopic.php?t=263 Dev console overview
 * http://rusefi.com/forum/viewtopic.php?t=210 Commands overview
 *
 *
 * @date Mar 15, 2013
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
 *
 */

#include "main.h"
#include "status_loop.h"

#include "adc_inputs.h"
#if EFI_WAVE_ANALYZER || defined(__DOXYGEN__)
#include "wave_analyzer.h"
#endif

#include "trigger_central.h"
#include "engine_state.h"
#include "io_pins.h"
#include "efiGpio.h"
#include "mmc_card.h"
#include "console_io.h"
#include "malfunction_central.h"
#include "speed_density.h"

#include "advance_map.h"
#include "tunerstudio.h"
#include "fuel_math.h"
#include "main_trigger_callback.h"
#include "engine_math.h"
#include "idle_thread.h"
#include "engine_configuration.h"
#include "rfiutil.h"
#include "svnversion.h"
#include "engine.h"
#include "lcd_controller.h"
#include "settings.h"
#include "rusefi_outputs.h"

extern bool_t main_loop_started;

#if EFI_PROD_CODE || defined(__DOXYGEN__)
// todo: move this logic to algo folder!
#include "rtc_helper.h"
#include "lcd_HD44780.h"
#include "rusefi.h"
#include "pin_repository.h"
#include "flash_main.h"
#include "max31855.h"
#include "vehicle_speed.h"
#endif

extern engine_pins_s enginePins;
extern TriggerCentral triggerCentral;

static bool_t subscription[(int) RO_LAST_ELEMENT];

// this 'true' value is needed for simulator
static volatile bool fullLog = true;
int warningEnabled = true;
//int warningEnabled = FALSE;

extern board_configuration_s *boardConfiguration;
extern bool hasFirmwareErrorFlag;
#define FULL_LOGGING_KEY "fl"

static char LOGGING_BUFFER[700];
static Logging logger("status loop", LOGGING_BUFFER, sizeof(LOGGING_BUFFER));

static void setWarningEnabled(int value) {
	warningEnabled = value;
}

#if EFI_FILE_LOGGING || defined(__DOXYGEN__)
static LoggingWithStorage fileLogger("file logger");
#endif /* EFI_FILE_LOGGING */

static int logFileLineIndex = 0;
#define TAB "\t"

static void reportSensorF(Logging *log, bool fileFormat, const char *caption, const char *units, float value,
		int precision) {
	if (!fileFormat) {
#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
		debugFloat(log, caption, value, precision);
#endif /* EFI_PROD_CODE || EFI_SIMULATOR */
	} else {

#if EFI_FILE_LOGGING || defined(__DOXYGEN__)
		if (logFileLineIndex == 0) {
			append(log, caption);
			append(log, TAB);
		} else if (logFileLineIndex == 1) {
			append(log, units);
			append(log, TAB);
		} else {
			appendFloat(log, value, precision);
			append(log, TAB);
		}
#endif /* EFI_FILE_LOGGING */
	}
}

static void reportSensorI(Logging *log, bool fileFormat, const char *caption, const char *units, int value) {
	if (!fileFormat) {

#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
		debugInt(log, caption, value);
#endif /* EFI_PROD_CODE || EFI_SIMULATOR */
	} else {
#if EFI_FILE_LOGGING || defined(__DOXYGEN__)
		if (logFileLineIndex == 0) {
			append(log, caption);
			append(log, TAB);
		} else if (logFileLineIndex == 1) {
			append(log, units);
			append(log, TAB);
		} else {
			appendPrintf(log, "%d%s", value, TAB);
		}
#endif /* EFI_FILE_LOGGING */
	}
}

static const char* boolean2string(int value) {
	return value ? "YES" : "NO";
}

void printSensors(Logging *log, bool fileFormat, Engine *engine) {
	// current time, in milliseconds
	int nowMs = currentTimeMillis();
	float sec = ((float) nowMs) / 1000;
	reportSensorF(log, fileFormat, "time", "", sec, 3);

#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
	reportSensorI(log, fileFormat, "rpm", "RPM", getRpmE(engine));

	reportSensorF(log, fileFormat, "TRG_0_DUTY", "%", getTriggerDutyCycle(0), 2);
	reportSensorF(log, fileFormat, "TRG_1_DUTY", "%", getTriggerDutyCycle(1), 2);
#endif

	engine_configuration_s *engineConfiguration = engine->engineConfiguration;

	if (engineConfiguration->hasMafSensor) {
		reportSensorF(log, fileFormat, "maf", "V", getMaf(), 2);
		reportSensorF(log, fileFormat, "mafr", "kg/hr", getRealMaf(), 2);
	}


#if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
	if (engineConfiguration->hasMapSensor) {
		reportSensorF(log, fileFormat, "MAP", "kPa", getMap(), 2);
		reportSensorF(log, fileFormat, "map_r", "V", getRawMap(), 2);
	}
	if (engineConfiguration->hasBaroSensor) {
		reportSensorF(log, fileFormat, "baro", "kPa", getBaroPressure(), 2);
	}
	if (engineConfiguration->hasAfrSensor) {
		reportSensorF(log, fileFormat, "afr", "AFR", getAfr(), 2);
	}
#endif

#if EFI_VEHICLE_SPEED || defined(__DOXYGEN__)
	if (engineConfiguration->hasVehicleSpeedSensor) {
		reportSensorF(log, fileFormat, "vss", "kph", getVehicleSpeed(), 2);
	}
#endif /* EFI_PROD_CODE */
	reportSensorF(log, fileFormat, "vref", "V", getVRef(engineConfiguration), 2);
	reportSensorF(log, fileFormat, "vbatt", "V", getVBatt(engineConfiguration), 2);

	reportSensorF(log, fileFormat, "TP", "%", getTPS(PASS_ENGINE_PARAMETER_F), 2);

	if (engineConfiguration->hasCltSensor) {
		reportSensorF(log, fileFormat, "CLT", "C", getCoolantTemperature(engine), 2);
	}

	reportSensorF(log, fileFormat, "MAT", "C", getIntakeAirTemperature(engine), 2);

//	debugFloat(&logger, "tch", getTCharge1(tps), 2);

}

EXTERN_ENGINE
;

void writeLogLine(void) {
	if (!main_loop_started)
		return;
#if EFI_FILE_LOGGING || defined(__DOXYGEN__)
	resetLogging(&fileLogger);
	printSensors(&fileLogger, true, engine);

	if (isSdCardAlive()) {
		appendPrintf(&fileLogger, "\r\n");
		appendToLog(fileLogger.buffer);
		logFileLineIndex++;
	}
#endif /* EFI_FILE_LOGGING */
}

static void printState(Engine *engine) {
#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
	printSensors(&logger, false, engine);

	// todo: make SWO work
//	char *msg = "hello\r\n";
//	for(int i=0;i<strlen(msg);i++) {
//		ITM_SendChar(msg[i]);
//	}


	engine_configuration_s *engineConfiguration = engine->engineConfiguration;

	int rpm = getRpmE(engine);
	if (subscription[(int) RO_TOTAL_REVOLUTION_COUNTER])
		debugInt(&logger, "ckp_c", getCrankEventCounter());
	if (subscription[(int) RO_RUNNING_REVOLUTION_COUNTER])
		debugInt(&logger, "ckp_r", triggerCentral.triggerState.runningRevolutionCounter);

	if (subscription[(int) RO_RUNNING_TRIGGER_ERROR])
		debugInt(&logger, "trg_r_errors", triggerCentral.triggerState.runningTriggerErrorCounter);

	if (subscription[(int) RO_RUNNING_ORDERING_TRIGGER_ERROR])
		debugInt(&logger, "trg_r_order_errors", triggerCentral.triggerState.runningOrderingErrorCounter);

	if (subscription[(int) RO_WAVE_CHART_CURRENT_SIZE])
		debugInt(&logger, "wave_chart_current", 0);

//	debugInt(&logger, "idl", getIdleSwitch());

//	debugFloat(&logger, "table_spark", getAdvance(rpm, getMaf()), 2);

	float engineLoad = getEngineLoadT(PASS_ENGINE_PARAMETER);
	float baseFuel = getBaseFuel(rpm PASS_ENGINE_PARAMETER);
	debugFloat(&logger, "fuel_base", baseFuel, 2);
//	debugFloat(&logger, "fuel_iat", getIatCorrection(getIntakeAirTemperature()), 2);
//	debugFloat(&logger, "fuel_clt", getCltCorrection(getCoolantTemperature()), 2);
	debugFloat(&logger, "fuel_lag", getInjectorLag(getVBatt(engineConfiguration) PASS_ENGINE_PARAMETER), 2);
	debugFloat(&logger, "fuel", getFuelMs(rpm PASS_ENGINE_PARAMETER), 2);

	debugFloat(&logger, "timing", getAdvance(rpm, engineLoad PASS_ENGINE_PARAMETER), 2);

//		float map = getMap();

#endif /* EFI_SHAFT_POSITION_INPUT */
}

#define INITIAL_FULL_LOG TRUE
//#define INITIAL_FULL_LOG FALSE

volatile int needToReportStatus = FALSE;
static int prevCkpEventCounter = -1;

static LoggingWithStorage logger2("main event handler");

static void printStatus(void) {
	needToReportStatus = TRUE;
}

/**
 * Time when the firmware version was reported last time, in seconds
 * TODO: implement a request/response instead of just constantly sending this out
 */
static systime_t timeOfPreviousPrintVersion = (systime_t) -1;

#if EFI_PROD_CODE || defined(__DOXYGEN__)
static void printOutPin(const char *pinName, brain_pin_e hwPin) {
	appendPrintf(&logger, "outpin%s%s@%s%s", DELIMETER, pinName, hwPortname(hwPin), DELIMETER);
}
#endif /* EFI_PROD_CODE */

static void printInfo(Engine *engine, systime_t nowSeconds) {
	engine_configuration_s *engineConfiguration = engine->engineConfiguration;

	/**
	 * we report the version every 4 seconds - this way the console does not need to
	 * request it and we will display it pretty soon
	 */
	if (overflowDiff(nowSeconds, timeOfPreviousPrintVersion) < 4) {
		return;
	}
	timeOfPreviousPrintVersion = nowSeconds;
	appendPrintf(&logger, "rusEfiVersion%s%d@%s %s%s", DELIMETER, getRusEfiVersion(), VCS_VERSION,
			getConfigurationName(engineConfiguration->engineType),
			DELIMETER);
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	printOutPin(WC_CRANK1, boardConfiguration->triggerInputPins[0]);
	printOutPin(WC_CRANK2, boardConfiguration->triggerInputPins[1]);
#if EFI_WAVE_ANALYZER || defined(__DOXYGEN__)
	printOutPin(WA_CHANNEL_1, boardConfiguration->logicAnalyzerPins[0]);
	printOutPin(WA_CHANNEL_2, boardConfiguration->logicAnalyzerPins[1]);
#endif

	for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
		printOutPin(enginePins.coils[i].name, boardConfiguration->ignitionPins[i]);

		printOutPin(enginePins.injectors[i].name, boardConfiguration->injectionPins[i]);
	}
#endif

}

static systime_t timeOfPreviousReport = (systime_t) -1;

extern char errorMessageBuffer[200];

/**
 * @brief Sends all pending data to dev console
 */
void updateDevConsoleState(Engine *engine) {
	if (!isConsoleReady()) {
		return;
	}
// looks like this is not needed anymore
//	checkIfShouldHalt();
	printPending();

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	// todo: unify with simulator!
	if (hasFirmwareError()) {
		printMsg(&logger, "firmware error: %s", errorMessageBuffer);
		warningEnabled = FALSE;
		chThdSleepMilliseconds(200);
		return;
	}
#endif

#if (EFI_PROD_CODE && HAL_USE_ADC) || defined(__DOXYGEN__)
	pokeAdcInputs();
#endif

	if (!fullLog) {
		return;
	}

	systime_t nowSeconds = getTimeNowSeconds();
	printInfo(engine, nowSeconds);

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
	int currentCkpEventCounter = getCrankEventCounter();
	if (prevCkpEventCounter == currentCkpEventCounter && timeOfPreviousReport == nowSeconds) {
		return;
	}
	timeOfPreviousReport = nowSeconds;

	prevCkpEventCounter = currentCkpEventCounter;
#else
	chThdSleepMilliseconds(200);
#endif

	printState(engine);

#if EFI_WAVE_ANALYZER
	printWave(&logger);
#endif

	printLine(&logger);
}

#if EFI_PROD_CODE

/*
 * command example:
 * sfm 3500 400
 * that would be 'show fuel for rpm 3500 maf 4.0'
 */

static void showFuelInfo2(float rpm, float engineLoad, Engine *engine) {
	engine_configuration_s *engineConfiguration = engine->engineConfiguration;

	float baseFuelMs = getBaseTableFuel(engineConfiguration, (int) rpm, engineLoad);

	scheduleMsg(&logger, "SD magic fuel %f", sdMath(engineConfiguration, 100, 100, 14.7, convertCelsiusToKelvin(20)));

	scheduleMsg(&logger2, "algo=%s/pump=%s", getEngine_load_mode_e(engineConfiguration->algorithm),
			boolToString(enginePins.fuelPumpRelay.getLogicValue()));

	scheduleMsg(&logger2, "phase=%f correction=%f", getInjectionAngle(rpm),
			engineConfiguration->globalFuelCorrection);

#if EFI_ENGINE_CONTROL
	scheduleMsg(&logger, "base cranking fuel %f", engineConfiguration->cranking.baseFuel);
	scheduleMsg(&logger2, "cranking fuel: %f", getCrankingFuel(engine));

	if (engine->rpmCalculator.isRunning()) {
		float iatCorrection = getIatCorrection(getIntakeAirTemperature(engine) PASS_ENGINE_PARAMETER);
		float cltCorrection = getCltCorrection(getCoolantTemperature(engine) PASS_ENGINE_PARAMETER);
		float injectorLag = getInjectorLag(getVBatt(engineConfiguration) PASS_ENGINE_PARAMETER);
		scheduleMsg(&logger2, "rpm=%f engineLoad=%f", rpm, engineLoad);
		scheduleMsg(&logger2, "baseFuel=%f", baseFuelMs);

		scheduleMsg(&logger2, "iatCorrection=%f cltCorrection=%f injectorLag=%f", iatCorrection, cltCorrection,
				injectorLag);

		float value = getRunningFuel(baseFuelMs, (int) rpm PASS_ENGINE_PARAMETER);
		scheduleMsg(&logger2, "injection pulse width: %f", value);
	}
#endif
}

#if EFI_ENGINE_CONTROL
static void showFuelInfo(Engine *engine) {
	showFuelInfo2((float) getRpmE(engine), getEngineLoadT(PASS_ENGINE_PARAMETER), engine);
}
#endif

#endif /* EFI_PROD_CODE */

static THD_WORKING_AREA(lcdThreadStack, UTILITY_THREAD_STACK_SIZE);

/**
 * blinking thread to show that we are alive
 * that's a trivial task - a smaller stack should work
 */
static THD_WORKING_AREA(blinkingStack, 128);

static OutputPin communicationPin;
OutputPin checkEnginePin;
OutputPin warningPin;
OutputPin runningPin;
extern engine_pins_s enginePins;

static OutputPin *leds[] = { &warningPin, &runningPin, &enginePins.errorLedPin, &communicationPin, &checkEnginePin };

static void initStatisLeds() {
#if EFI_PROD_CODE
	outputPinRegister("communication status 1", &communicationPin,
	LED_COMMUNICATION_PORT, LED_COMMUNICATION_PIN);
#endif

#if EFI_WARNING_LED
	outputPinRegister("warning", &warningPin, LED_WARNING_PORT,
	LED_WARNING_PIN);
	outputPinRegister("is running status", &runningPin, LED_RUNNING_STATUS_PORT,
	LED_RUNNING_STATUS_PIN);
#endif /* EFI_WARNING_LED */
}

/**
 * This method would blink all the LEDs just to test them
 */
static void initialLedsBlink(void) {
	int size = sizeof(leds) / sizeof(leds[0]);
	for (int i = 0; i < size; i++)
		leds[i]->setValue(1);

	chThdSleepMilliseconds(100);

	for (int i = 0; i < size; i++)
		leds[i]->setValue(0);
}

#if EFI_PROD_CODE || defined(__DOXYGEN__)
/**
 * this thread has a lower-then-usual stack size so we cannot afford *print* methods here
 */
static void blinkingThread(void *arg) {
	(void) arg;
	chRegSetThreadName("communication blinking");

	initialLedsBlink();

	while (true) {
		int delay;

		if (getNeedToWriteConfiguration()) {
			delay = isConsoleReady() ? 200 : 66;
		} else {
			delay = isConsoleReady() ? 100 : 33;
		}

		communicationPin.setValue(0);
		warningPin.setValue(0);
		chThdSleepMilliseconds(delay);

		communicationPin.setValue(1);
#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
		if (isTriggerDecoderError() || isIgnitionTimingError())
			warningPin.setValue(1);
#endif
		chThdSleepMilliseconds(delay);

	}
}

#endif /* EFI_PROD_CODE */

static void lcdThread(Engine *engine) {
	chRegSetThreadName("lcd");
	while (true) {
#if EFI_HD44780_LCD
		updateHD44780lcd(engine);
#endif
		chThdSleepMilliseconds(engine->engineConfiguration->bc.lcdThreadPeriod);
	}
}

// stack for Tuner Studio thread
static THD_WORKING_AREA(tsThreadStack, UTILITY_THREAD_STACK_SIZE);

#if EFI_TUNER_STUDIO

extern Map3D1616 veMap;

void updateTunerStudioState(Engine *engine, TunerStudioOutputChannels *tsOutputChannels) {
#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)
	int rpm = getRpmE(engine);
#else
	int rpm = 0;
#endif

	engine_configuration_s *engineConfiguration = engine->engineConfiguration;

	float tps = getTPS(PASS_ENGINE_PARAMETER_F);
	float coolant = getCoolantTemperature(engine);
	float intake = getIntakeAirTemperature(engine);

	float engineLoad = getEngineLoadT(PASS_ENGINE_PARAMETER);
	float baseFuelMs = getBaseTableFuel(engineConfiguration, (int) rpm, engineLoad);

	// header
	tsOutputChannels->tsConfigVersion = TS_FILE_VERSION;

	// engine state
	tsOutputChannels->rpm = rpm;
	tsOutputChannels->coolant_temperature = coolant;
	tsOutputChannels->intakeAirTemperature = intake;
	tsOutputChannels->throttlePositon = tps;
	tsOutputChannels->massAirFlowVoltage = getMaf();
	tsOutputChannels->massAirFlowValue = getRealMaf();
	tsOutputChannels->veValue = veMap.getValue(getMap(), rpm);
	tsOutputChannels->airFuelRatio = getAfr();
	tsOutputChannels->v_batt = getVBatt(engineConfiguration);
	tsOutputChannels->tpsADC = getTPS10bitAdc(PASS_ENGINE_PARAMETER_F);
	tsOutputChannels->atmospherePressure = getBaroPressure();
	tsOutputChannels->manifold_air_pressure = getMap();
	tsOutputChannels->engineLoad = engineLoad;
	tsOutputChannels->rpmAcceleration = engine->rpmCalculator.getRpmAcceleration();
	tsOutputChannels->maxDelta = engine->accelEnrichment.maxDelta;
	tsOutputChannels->minDelta = engine->accelEnrichment.minDelta;

	tsOutputChannels->checkEngine = hasErrorCodes();
#if EFI_PROD_CODE
	for (int i = 0; i < EGT_CHANNEL_COUNT; i++)
		tsOutputChannels->egtValues.values[i] = getEgtValue(i);

	tsOutputChannels->needBurn = getNeedToWriteConfiguration();
	tsOutputChannels->hasSdCard = isSdCardAlive();
	tsOutputChannels->isFuelPumpOn = enginePins.fuelPumpRelay.getLogicValue();
	tsOutputChannels->isFanOn = enginePins.fanRelay.getLogicValue();
	tsOutputChannels->isO2HeaterOn = enginePins.o2heater.getLogicValue();
	tsOutputChannels->ignition_enabled = engineConfiguration->isIgnitionEnabled;
	tsOutputChannels->injection_enabled = engineConfiguration->isInjectionEnabled;
	tsOutputChannels->cylinder_cleanup_enabled = engineConfiguration->isCylinderCleanupEnabled;
	tsOutputChannels->cylinder_cleanup_activated = engine->isCylinderCleanupMode;
	tsOutputChannels->secondTriggerChannelEnabled = engineConfiguration->secondTriggerChannelEnabled;
	tsOutputChannels->vehicleSpeedKph = getVehicleSpeed();
	tsOutputChannels->isCltError = !isValidCoolantTemperature(getCoolantTemperature(engine));
	tsOutputChannels->isIatError = !isValidIntakeAirTemperature(getIntakeAirTemperature(engine));
#endif
	tsOutputChannels->clutchUpState = engine->clutchUpState;
	tsOutputChannels->clutchDownState = engine->clutchDownState;
	tsOutputChannels->tCharge = getTCharge(rpm, tps, coolant, intake);
	float timing = getAdvance(rpm, engineLoad PASS_ENGINE_PARAMETER);
	tsOutputChannels->inj_adv = timing > 360 ? timing - 720 : timing;
	tsOutputChannels->sparkDwell = getSparkDwellMsT(rpm PASS_ENGINE_PARAMETER);
	tsOutputChannels->baseFuel = baseFuelMs;
	tsOutputChannels->pulseWidthMs = getRunningFuel(baseFuelMs, rpm PASS_ENGINE_PARAMETER);
	tsOutputChannels->crankingFuelMs = getCrankingFuel(engine);
}

extern TunerStudioOutputChannels tsOutputChannels;
#endif /* EFI_TUNER_STUDIO */

static void tsStatusThread(Engine *engine) {
	chRegSetThreadName("tuner s");
	while (true) {
#if EFI_TUNER_STUDIO
		// sensor state for EFI Analytics Tuner Studio
		updateTunerStudioState(engine, &tsOutputChannels);
#endif /* EFI_TUNER_STUDIO */
		chThdSleepMilliseconds(boardConfiguration->tunerStudioThreadPeriod);
	}
}

static void subscribe(int outputOrdinal) {
	subscription[outputOrdinal] = true;
}

static void unsubscribe(int outputOrdinal) {
	subscription[outputOrdinal] = false;
}

void initStatusLoop(Engine *engine) {
	setFullLog(INITIAL_FULL_LOG);
	addConsoleActionI(FULL_LOGGING_KEY, setFullLog);
	addConsoleActionI("warn", setWarningEnabled);

#if EFI_PROD_CODE

#if EFI_ENGINE_CONTROL
	addConsoleActionFFP("fuelinfo2", (VoidFloatFloatVoidPtr) showFuelInfo2, engine);
	addConsoleActionP("fuelinfo", (VoidPtr) showFuelInfo, engine);
#endif

	subscription[(int) RO_TRG1_DUTY] = true;
	subscription[(int) RO_TRG2_DUTY] = true;
	subscription[(int) RO_TRG3_DUTY] = false;
	subscription[(int) RO_TRG4_DUTY] = false;
	subscription[(int) RO_TOTAL_REVOLUTION_COUNTER] = true;
	subscription[(int) RO_RUNNING_REVOLUTION_COUNTER] = false;

	addConsoleActionI("subscribe", subscribe);
	addConsoleActionI("unsubscribe", unsubscribe);

	addConsoleAction("status", printStatus);
#endif /* EFI_PROD_CODE */
}

void startStatusThreads(Engine *engine) {
	// todo: refactoring needed, this file should probably be split into pieces
	chThdCreateStatic(lcdThreadStack, sizeof(lcdThreadStack), NORMALPRIO, (tfunc_t) lcdThread, engine);
	chThdCreateStatic(tsThreadStack, sizeof(tsThreadStack), NORMALPRIO, (tfunc_t) tsStatusThread, engine);
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	initStatisLeds();
	chThdCreateStatic(blinkingStack, sizeof(blinkingStack), NORMALPRIO, (tfunc_t) blinkingThread, NULL);
#endif /* EFI_PROD_CODE */
}

void setFullLog(int value) {
	print("Setting full logging: %s\r\n", boolean2string(value));
	printMsg(&logger, "%s%d", FULL_LOGGING_KEY, value);
	fullLog = value;
}

bool getFullLog(void) {
	return fullLog;
}
