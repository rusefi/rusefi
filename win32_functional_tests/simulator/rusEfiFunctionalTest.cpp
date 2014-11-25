/**
 * @file	rusEfiFunctionalTest.cpp
 *
 * @date Mar 1, 2014
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#include "global.h"
#include "boards.h"
#include "rusEfiFunctionalTest.h"
#include "console_io.h"
#include "eficonsole.h"
#include "engine_configuration.h"
#include "ec2.h"
#include "rusefi_enums.h"
#include "pwm_generator_logic.h"
#include "trigger_central.h"
#include "datalogging.h"
#include "algo.h"
#include "rpm_calculator.h"
#include "wave_chart.h"
#include "status_loop.h"
#include "trigger_emulator_algo.h"
#include "main_trigger_callback.h"
#include "allsensors.h"
#include "analog_chart.h"
#include "injector_central.h"
#include "engine.h"
#include "tunerstudio.h"
#include "trigger_emulator.h"

static Engine _engine;
Engine *engine = &_engine;

extern WaveChart waveChart;

uint32_t maxLockTime = 0;

persistent_config_container_s persistentState;
static engine_configuration2_s ec2;

engine_configuration_s * engineConfiguration = &persistentState.persistentConfiguration.engineConfiguration;
board_configuration_s *boardConfiguration = &persistentState.persistentConfiguration.engineConfiguration.bc;
engine_configuration2_s *engineConfiguration2 = &ec2;

void setOutputPinValue(io_pin_e pin, int logicValue) {
}

int isInjectionEnabled(void) {
	return TRUE;
}

int getRemainingStack(Thread *otp) {
	return 99999;
}

void idleDebug(const char *msg, int value) {
}

float getMap(void) {
	return getRawMap();
}

void rusEfiFunctionalTest(void) {
	initializeConsole();

	initFakeBoard();

	initStatusLoop(engine);
	initDataStructures(PASS_ENGINE_PARAMETER_F);

	engine->engineConfiguration = engineConfiguration;
	engine->engineConfiguration2 = engineConfiguration2;


	resetConfigurationExt(NULL, FORD_ASPIRE_1996, engine);
	prepareShapes(engine);

	initThermistors(engine);
	initAlgo(engineConfiguration);
	initRpmCalculator(engine);

	initAnalogChart();

	initTriggerEmulator(engine);

	initMainEventListener(engine, engineConfiguration2);

	initTriggerCentral(engine);

	startStatusThreads(engine);
	startTunerStudioConnectivity();
}

void printPendingMessages(void) {
	updateDevConsoleState(engine);
	waveChart.publishChartIfFull();
}

int isSerialOverTcpReady;

bool isConsoleReady(void) {
	return isSerialOverTcpReady;
}

bool hasFirmwareErrorFlag = false;

void onFatalError(const char *msg, const char * file, int line) {
	printf("onFatalError %s %s%d", msg, file, line);
	exit(-1);
}

static time_t timeOfPreviousWarning = -10;

// todo: re-use primary firmware implementation?
int warning(obd_code_e code, const char *fmt, ...) {
	int now = currentTimeMillis() / 1000;
	if (absI(now - timeOfPreviousWarning) < 10)
		return TRUE; // we just had another warning, let's not spam
	timeOfPreviousWarning = now;
	printf("Warning: %s\r\n", fmt);
	return FALSE;
}

void firmwareError(const char *fmt, ...) {
	printf("firmwareError [%s]", fmt);
	exit(-1);
}

SerialDriver * getConsoleChannel(void) {
	return (SerialDriver *)EFI_CONSOLE_UART_DEVICE;
}

void chDbgPanic3(const char *msg, const char * file, int line) {
	onFatalError(msg, file, line);
}
