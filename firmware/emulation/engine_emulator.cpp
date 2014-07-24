/**
 * @file	engine_emulator.cpp
 * @brief	Entry point for all the emulation and analysis code
 *
 * @date Mar 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "engine_emulator.h"

extern "C" {

#include "status_loop.h"
#include "advance_map.h"
#include "wave_analyzer.h"
#include "fuel_math.h"
#include "pin_repository.h"
#include "poten.h"
}
#include "trigger_emulator.h"


static THD_WORKING_AREA(eeThreadStack, UTILITY_THREAD_STACK_SIZE);

#define DIAG_PORT GPIOD
#define DIAG_PIN 0

void setDiag(int value) {
	print("Setting diag: %d\r\n", value);
	palWritePad(DIAG_PORT, DIAG_PIN, value);
}

#define PERIOD 3000

void emulate(void) {
	print("Emulating...\r\n");
	setDiag(1);
	chThdSleep(1);
	setFullLog(1);

	for (int i = 400; i <= 1300; i++) {
		if (i % 50 != 0)
			continue;
		setTriggerEmulatorRPM(i);
		chThdSleepMilliseconds(PERIOD);
	}

	setTriggerEmulatorRPM(0);

	setFullLog(0);
	setDiag(0);
	chThdSleep(1);
	print("Emulation DONE!\r\n");
}

static int flag = FALSE;

static msg_t eeThread(void *arg) {
	chRegSetThreadName("Engine");

	while (TRUE) {
		while (!flag)
			chThdSleepMilliseconds(200);
		flag = FALSE;
		emulate();
	}
#if defined __GNUC__
	return (msg_t)NULL;
#endif
}

void startEmulator(void) {
	flag = TRUE;
}

static void printAdvance(int rpm, int maf100) {
	float advance = getAdvance(rpm, maf100 / 100.0);
	print("advance for %d rpm %d maf100: %f\r\n", rpm, maf100, advance);
}

static void initECUstimulator(void) {
	mySetPadMode("TEN", DIAG_PORT, DIAG_PIN,
	PAL_MODE_OUTPUT_PUSHPULL);

	addConsoleActionI("diag", setDiag);
	addConsoleAction("emu", startEmulator);
	addConsoleActionII("ad", printAdvance);

	setDiag(1);

	chThdCreateStatic(eeThreadStack, sizeof(eeThreadStack), NORMALPRIO, (tfunc_t) eeThread, NULL);
}

void initEngineEmulator(board_configuration_s *boardConfiguration) {
	if (hasFirmwareError())
		return;

#if EFI_POTENTIOMETER
	initPotentiometers(boardConfiguration);
#endif /* EFI_POTENTIOMETER */

	//initECUstimulator();
	initTriggerEmulator();
}
