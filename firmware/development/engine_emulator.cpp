/**
 * @file	engine_emulator.cpp
 * @brief	Entry point for all the emulation and analysis code
 *
 * @date Mar 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"
#include "engine_emulator.h"

#include "advance_map.h"
#include "fuel_math.h"

#include "status_loop.h"
#include "wave_analyzer.h"
#include "pin_repository.h"
#include "pwm_generator_logic.h"

#include "poten.h"
#include "trigger_emulator.h"

extern bool hasFirmwareErrorFlag;

static THD_WORKING_AREA(eeThreadStack, UTILITY_THREAD_STACK_SIZE);

#define DIAG_PIN GPIOD_0

void setDiag(int value) {
	print("Setting diag: %d\r\n", value);
// todo: convert to new api	palWritePad(DIAG_PORT, DIAG_PIN, value);
}

#define PERIOD 3000

EXTERN_ENGINE;

static void emulate(void) {
	print("Emulating...\r\n");
	setDiag(1);
	chThdSleep(1);
	setFullLog(1);

	for (int i = 400; i <= 1300; i++) {
		if (i % 50 != 0)
			continue;
		setTriggerEmulatorRPM(i PASS_ENGINE_PARAMETER_SUFFIX);
		chThdSleepMilliseconds(PERIOD);
	}

	setTriggerEmulatorRPM(0 PASS_ENGINE_PARAMETER_SUFFIX);

	setFullLog(0);
	setDiag(0);
	chThdSleep(1);
	print("Emulation DONE!\r\n");
}

static int flag = FALSE;

static msg_t eeThread(void *arg) {
	(void)arg;
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

//static void printAdvance(int rpm, int maf100) {
//	float advance = getAdvance(rpm, maf100 / 100.0);
//	print("advance for %d rpm %d maf100: %.2f\r\n", rpm, maf100, advance);
//}

static void initECUstimulator(Engine *engine) {
	efiSetPadMode("TEN", DIAG_PIN, PAL_MODE_OUTPUT_PUSHPULL);

	addConsoleActionI("diag", setDiag);
	addConsoleAction("emu", startEmulator);
//	addConsoleActionII("ad", printAdvance);

	setDiag(1);

	chThdCreateStatic(eeThreadStack, sizeof(eeThreadStack), NORMALPRIO, (tfunc_t) eeThread, engine);
}

void initEngineEmulator(Logging *sharedLogger, Engine *engine) {
	if (hasFirmwareError())
		return;

#if EFI_POTENTIOMETER
	initPotentiometers(sharedLogger, &engine->engineConfiguration->bc);
#endif /* EFI_POTENTIOMETER */

	//initECUstimulator();
	initTriggerEmulator(sharedLogger PASS_ENGINE_PARAMETER_SUFFIX);
}
