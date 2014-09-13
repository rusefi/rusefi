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

Engine engine;

extern WaveChart waveChart;

persistent_config_container_s persistentState;
static engine_configuration2_s ec2;

engine_configuration_s * engineConfiguration = &persistentState.persistentConfiguration.engineConfiguration;
board_configuration_s *boardConfiguration = &persistentState.persistentConfiguration.engineConfiguration.bc;
engine_configuration2_s *engineConfiguration2 = &ec2;

static configuration_s cfg = {engineConfiguration, engineConfiguration2};

configuration_s * configuration = &cfg;

void setOutputPinValue(io_pin_e pin, int logicValue) {
}

int isInjectionEnabled(void) {
	return TRUE;
}

// todo: move this to "idle_controller.h"

extern "C" {
void idleDebug(char *msg, int value);
}

int getRemainingStack(Thread *otp) {
	return 99999;
}

void idleDebug(char *msg, int value) {
}

float getMap(void) {
	return getRawMap();
}

static TriggerEmulatorHelper helper;

static void triggerEmulatorCallback(PwmConfig *state, int stateIndex) {
	helper.handleEmulatorCallback(state, stateIndex);
}

void rusEfiFunctionalTest(void) {
	initializeConsole();

	initFakeBoard();

	initStatusLoop();
	initDataStructures(engineConfiguration);

	engine.engineConfiguration = engineConfiguration;


	resetConfigurationExt(NULL, FORD_ASPIRE_1996, engineConfiguration, engineConfiguration2);

	initThermistors();
	initAlgo(engineConfiguration);
	initRpmCalculator();

	initAnalogChart();

	initTriggerEmulatorLogic(triggerEmulatorCallback);

	initMainEventListener(&engine, engineConfiguration2);

	initTriggerCentral(&engine);

	startStatusThreads();
	startTunerStudioConnectivity();

}

void printPendingMessages(void) {
	updateDevConsoleState();
	waveChart.publishChartIfFull();
}

static size_t wt_writes(void *ip, const uint8_t *bp, size_t n) {
	printToWin32Console((char*)bp);
	return CONSOLE_PORT->vmt->write(CONSOLE_PORT, bp, n);
}

static size_t wt_reads(void *ip, uint8_t *bp, size_t n) {
	return CONSOLE_PORT->vmt->read(CONSOLE_PORT, bp, n);
}

static msg_t wt_putt(void *instance, uint8_t b, systime_t time) {
	return CONSOLE_PORT->vmt->putt(CONSOLE_PORT, b, time);
}

static msg_t wt_gett(void *instance, systime_t time) {
	return CONSOLE_PORT->vmt->gett(CONSOLE_PORT, time);
}

static size_t wt_writet(void *instance, const uint8_t *bp,
                size_t n, systime_t time) {
	return CONSOLE_PORT->vmt->writet(CONSOLE_PORT, bp, n, time);
}

static size_t wt_readt(void *instance, uint8_t *bp, size_t n, systime_t time) {
	return CONSOLE_PORT->vmt->readt(CONSOLE_PORT, bp, n, time);
}

static char putMessageBuffer[2];

static msg_t wt_put(void *ip, uint8_t b) {
	putMessageBuffer[0] = b;
	putMessageBuffer[1] = 0;
	printToWin32Console((char*)putMessageBuffer);
//	cputs("wt_put");
	return CONSOLE_PORT->vmt->put(CONSOLE_PORT, b);
}

static msg_t wt_get(void *ip) {
//	cputs("wt_get");
	//return 0;
	return CONSOLE_PORT->vmt->get(CONSOLE_PORT);
}

static const struct Win32TestStreamVMT vmt = { wt_writes, wt_reads, wt_put, wt_get, wt_putt, wt_gett, wt_writet, wt_readt };

void initTestStream(TestStream *ts) {
	ts->vmt = &vmt;
}

int isSerialOverTcpReady;

bool isConsoleReady(void) {
	return isSerialOverTcpReady;
}

bool_t hasFirmwareError(void) {
	return FALSE;
}

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

uint64_t getTimeNowUs(void) {
	return chTimeNow() * (1000000 / CH_FREQUENCY);
}

efitimems_t currentTimeMillis(void) {
	return getTimeNowUs() / 1000;
}

int getTimeNowSeconds(void) {
	return chTimeNow() / CH_FREQUENCY;
}

int getRusEfiVersion(void) {
	return 239;
}
