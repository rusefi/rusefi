/**
 * @file	rusEfiFunctionalTest.cpp
 *
 * @date Mar 1, 2014
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "global.h"
#include "boards.h"
#include "rusEfiFunctionalTest.h"
#include "console_io.h"
#include "eficonsole.h"
#include "engine_configuration.h"
#include "rusefi_enums.h"
#include "pwm_generator_logic.h"
#include "trigger_central.h"
#include "datalogging.h"
#include "algo.h"
#include "rpm_calculator.h"
#include "engine_sniffer.h"
#include "status_loop.h"
#include "trigger_emulator_algo.h"
#include "main_trigger_callback.h"
#include "allsensors.h"
#include "sensor_chart.h"
#include "injector_central.h"
#include "engine.h"
#include "tunerstudio.h"
#include "trigger_emulator.h"
#include "engine_controller.h"
#include "map_averaging.h"

#define DEFAULT_SIM_RPM 1200
#define DEFAULT_SNIFFER_THR 2500

EXTERN_ENGINE;

extern WaveChart waveChart;

static LoggingWithStorage sharedLogger("simulator");

void outputPinRegisterExt2(const char *msg, OutputPin *output, brain_pin_e brainPin, pin_output_mode_e *outputMode) {
}

int getRemainingStack(thread_t *otp) {
	return 99999;
}

void idleDebug(const char *msg, percent_t value) {
}

float getMap(void) {
	return getRawMap();
}

void rusEfiFunctionalTest(void) {
	printToWin32Console("Running version:");
	printToWin32Console("TODO");

	initIntermediateLoggingBuffer();
	initErrorHandling();

	engine->setConfig(config);

	initializeConsole(&sharedLogger);

	initStatusLoop(engine);
	initDataStructures(PASS_ENGINE_PARAMETER_F);


	// todo: reduce code duplication with initEngineContoller

	resetConfigurationExt(NULL, DEFAULT_ENGINE_TYPE PASS_ENGINE_PARAMETER);
	prepareShapes(PASS_ENGINE_PARAMETER_F);

	initAlgo(&sharedLogger, engineConfiguration);
	commonInitEngineController(&sharedLogger);

	initRpmCalculator(&sharedLogger, engine);

#if EFI_SENSOR_CHART || defined(__DOXYGEN__)
	initSensorChart();
#endif /* EFI_SENSOR_CHART */

	initTriggerCentral(&sharedLogger, engine);

	initTriggerEmulator(&sharedLogger, engine);

#if EFI_MAP_AVERAGING || defined(__DOXYGEN__)
	initMapAveraging(&sharedLogger, engine);
#endif /* EFI_MAP_AVERAGING */

	initMainEventListener(&sharedLogger, engine);

	startStatusThreads(engine);

	initPeriodicEvents(PASS_ENGINE_PARAMETER_F);

	setTriggerEmulatorRPM(DEFAULT_SIM_RPM, engine);
	engineConfiguration->engineSnifferRpmThreshold = DEFAULT_SNIFFER_THR;
}

void printPendingMessages(void) {
	updateDevConsoleState(engine);
	waveChart.publishIfFull();
}

int isSerialOverTcpReady;

int maxNesting = 0;

bool isCommandLineConsoleReady(void) {
	return isSerialOverTcpReady;
}

void applyNewConfiguration(void) {
}

void onFatalError(const char *msg, const char * file, int line) {
	printf("onFatalError %s %s%d", msg, file, line);
	exit(-1);
}

void logMsg(const char *format, ...) {
//	FILE * fp;
//	fp = fopen ("simulator.log", "a");
//
//	va_list(args);
//	va_start(args, format);
//	vfprintf(fp, format, args);
//
//	fclose(fp);
}

SerialDriver * getConsoleChannel(void) {
	return (SerialDriver *)EFI_CONSOLE_UART_DEVICE;
}
