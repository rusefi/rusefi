/**
 * @file	rusEfiFunctionalTest.cpp
 *
 * @date Mar 1, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
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

EXTERN_ENGINE;
extern engine_configuration2_s * engineConfiguration2;

extern WaveChart waveChart;

uint32_t maxLockTime = 0;

// todo: move this field to trigger_central
bool isInsideTriggerHandler;

//persistent_config_container_s persistentState;
//static engine_configuration2_s ec2;

static LoggingWithStorage sharedLogger("simulator");

//engine_configuration_s * engineConfiguration = &persistentState.persistentConfiguration.engineConfiguration;
//board_configuration_s *boardConfiguration = &persistentState.persistentConfiguration.engineConfiguration.bc;
//engine_configuration2_s *engineConfiguration2 = &ec2;

void outputPinRegisterExt2(const char *msg, OutputPin *output, brain_pin_e brainPin, pin_output_mode_e *outputMode) {
}

//int isInjectionEnabled(void) {
//	return true;
//}

int getRemainingStack(Thread *otp) {
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

	initializeConsole(&sharedLogger);

	initFakeBoard();

	initStatusLoop(engine);
	initDataStructures(PASS_ENGINE_PARAMETER_F);

	engine->engineConfiguration2 = engineConfiguration2;

	// todo: reduce code duplication with initEngineContoller

	resetConfigurationExt(NULL, FORD_ESCORT_GT PASS_ENGINE_PARAMETER);
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
}

void printPendingMessages(void) {
	updateDevConsoleState(engine);
	waveChart.publishChartIfFull();
}

int isSerialOverTcpReady;

int maxNesting = 0;

bool isConsoleReady(void) {
	return isSerialOverTcpReady;
}

void applyNewConfiguration(void) {

}


bool hasFirmwareErrorFlag = false;

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
	printf("firmwareError [%s]\r\n", fmt);

	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);


	exit(-1);
}

SerialDriver * getConsoleChannel(void) {
	return (SerialDriver *)EFI_CONSOLE_UART_DEVICE;
}

void chDbgPanic3(const char *msg, const char * file, int line) {
	onFatalError(msg, file, line);
}
