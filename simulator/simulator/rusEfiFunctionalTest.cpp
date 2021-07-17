/**
 * @file	rusEfiFunctionalTest.cpp
 *
 * @date Mar 1, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "boards.h"
#include "rusEfiFunctionalTest.h"
#include "console_io.h"
#include "eficonsole.h"
#include "rusefi_enums.h"
#include "trigger_central.h"
#include "datalogging.h"
#include "rpm_calculator.h"
#include "engine_sniffer.h"
#include "status_loop.h"
#include "trigger_emulator_algo.h"
#include "main_trigger_callback.h"
#include "allsensors.h"
#include "sensor_chart.h"
#include "bench_test.h"
#include "tunerstudio.h"
#include "engine_controller.h"
#include "map_averaging.h"
#include "memstreams.h"
#include <chprintf.h>
#include "rusefi_lua.h"

#define DEFAULT_SIM_RPM 1200
#define DEFAULT_SNIFFER_THR 2500

extern WaveChart waveChart;

int getRemainingStack(thread_t *otp) {
	return 99999;
}

float getMap(void) {
	return getRawMap();
}

static void assertString(const char*actual, const char *expected) {
	if (strcmp(actual, expected) != 0) {
		firmwareError(OBD_PCM_Processor_Fault, "chprintf test: got %s while %s", actual, expected);
	}
}

static void runChprintfTest() {
	static MemoryStream ts;
	static char testBuffer[200];
	msObjectInit(&ts, (uint8_t *) testBuffer, sizeof(testBuffer), 0);


// it's a very, very long and mostly forgotten story how this became our %.2f precision format
	ts.eos = 0; // reset
	chprintf((BaseSequentialStream*)&ts, "%.2f/%.4f/%.4f", 0.239f, 239.932, 0.1234);
	ts.buffer[ts.eos] = 0;

	assertString(testBuffer, "0.23/239.9320/0.1234");


	{
		LoggingWithStorage testLogging("test");
		testLogging.appendFloat(1.23, 5);
		testLogging.appendFloat(1.234, 2);
		assertString(testLogging.buffer, "1.230001.23");

	}

	{
		LoggingWithStorage testLogging("test");
		testLogging.appendFloat(-1.23, 5);
		assertString(testLogging.buffer, "-1.23000");
	}
	{
		LoggingWithStorage testLogging("test");
		testLogging.appendPrintf( "a%.2fb%fc", -1.2, -3.4);
		assertString(testLogging.buffer, "a-1.20b-3.400000095c");
	}

}

void rusEfiFunctionalTest(void) {
	printToConsole("Running rusEfi simulator version:");
	static char versionBuffer[20];
	itoa10(versionBuffer, (int)getRusEfiVersion());
	printToConsole(versionBuffer);

	engine->setConfig();

	initializeConsole();

	initStatusLoop();
	initDataStructures(PASS_ENGINE_PARAMETER_SIGNATURE);


	// todo: reduce code duplication with initEngineContoller

	resetConfigurationExt(MINIMAL_PINS PASS_ENGINE_PARAMETER_SUFFIX);
	enableTriggerStimulator();

	commonInitEngineController();

	initTriggerCentral();
	initTriggerEmulator(PASS_ENGINE_PARAMETER_SIGNATURE);

	startStatusThreads();

	startLoggingProcessor();

	runChprintfTest();

	initPeriodicEvents(PASS_ENGINE_PARAMETER_SIGNATURE);

	setTriggerEmulatorRPM(DEFAULT_SIM_RPM PASS_ENGINE_PARAMETER_SUFFIX);
	engineConfiguration->engineSnifferRpmThreshold = DEFAULT_SNIFFER_THR;

	startSerialChannels();

	startLua();
}

void printPendingMessages(void) {
	updateDevConsoleState();
	waveChart.publishIfFull();
}

int isSerialOverTcpReady;

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
