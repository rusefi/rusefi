/**
 * @file    bench_test.cpp
 * @brief	Utility methods related to bench testing.
 *
 *
 * @date Sep 8, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
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

// todo: rename this file
#include "global.h"

#if EFI_ENGINE_CONTROL
#if !EFI_UNIT_TEST

#include "os_access.h"
#include "flash_main.h"
#include "bench_test.h"
#include "io_pins.h"
#include "main_trigger_callback.h"
#include "engine_configuration.h"
#include "pin_repository.h"
#include "efi_gpio.h"
#include "settings.h"
#include "idle_thread.h"
#include "periodic_thread_controller.h"
#include "tps.h"
#include "electronic_throttle.h"
#include "cj125.h"
#include "malfunction_central.h"
#include "tunerstudio_outputs.h"

#if EFI_PROD_CODE
#include "rusefi.h"
#include "mpu_util.h"
#endif /* EFI_PROD_CODE */

#if (BOARD_TLE8888_COUNT > 0)
#include "gpio/tle8888.h"
#endif



EXTERN_ENGINE;

static Logging * logger;
static bool isRunningBench = false;

bool isRunningBenchTest(void) {
	return isRunningBench;
}

static void runBench(brain_pin_e brainPin, OutputPin *output, float delayMs, float onTimeMs, float offTimeMs,
		int count) {
    int delaySt = delayMs < 1 ? 1 : TIME_MS2I(delayMs);
	int onTimeSt = onTimeMs < 1 ? 1 : TIME_MS2I(onTimeMs);
	int offTimeSt = offTimeMs < 1 ? 1 : TIME_MS2I(offTimeMs);
	if (delaySt < 0) {
		scheduleMsg(logger, "Invalid delay %.2f", delayMs);
		return;
	}
	if (onTimeSt <= 0) {
		scheduleMsg(logger, "Invalid onTime %.2f", onTimeMs);
		return;
	}
	if (offTimeSt <= 0) {
		scheduleMsg(logger, "Invalid offTime %.2f", offTimeMs);
		return;
	}
	scheduleMsg(logger, "Running bench: ON_TIME=%.2f ms OFF_TIME=%.2fms Counter=%d", onTimeMs, offTimeMs, count);
	scheduleMsg(logger, "output on %s", hwPortname(brainPin));

	if (delaySt != 0) {
		chThdSleep(delaySt);
	}

	isRunningBench = true;
	for (int i = 0; i < count; i++) {
		output->setValue(true);
		chThdSleep(onTimeSt);
		output->setValue(false);
		chThdSleep(offTimeSt);
	}
	scheduleMsg(logger, "Done!");
	isRunningBench = false;
}

static volatile bool isBenchTestPending = false;
static float onTime;
static float offTime;
static float delayMs;
static int count;
static brain_pin_e brainPin;
static OutputPin* pinX;

static void pinbench(const char *delayStr, const char *onTimeStr, const char *offTimeStr, const char *countStr,
		OutputPin* pinParam, brain_pin_e brainPinParam) {
	delayMs = atoff(delayStr);
	onTime = atoff(onTimeStr);
	offTime = atoff(offTimeStr);
	count = atoi(countStr);

	brainPin = brainPinParam;
	pinX = pinParam;
	isBenchTestPending = true; // let's signal bench thread to wake up
}

static void doRunFuel(int humanIndex, const char *delayStr, const char * onTimeStr, const char *offTimeStr,
		const char *countStr) {
	if (humanIndex < 1 || humanIndex > engineConfiguration->specs.cylindersCount) {
		scheduleMsg(logger, "Invalid index: %d", humanIndex);
		return;
	}
	brain_pin_e b = CONFIG(injectionPins)[humanIndex - 1];
	pinbench(delayStr, onTimeStr, offTimeStr, countStr, &enginePins.injectors[humanIndex - 1], b);
}

/**
 * delay 100, cylinder #2, 5ms ON, 1000ms OFF, repeat 2 times
 * fuelbench2 100 2 5 1000 2
 */
static void fuelbench2(const char *delayStr, const char *indexStr, const char * onTimeStr, const char *offTimeStr,
		const char *countStr) {
	int index = atoi(indexStr);
	doRunFuel(index, delayStr, onTimeStr, offTimeStr, countStr);
}

static void fanBenchExt(const char *durationMs) {
	pinbench("0", durationMs, "100", "1", &enginePins.fanRelay, CONFIG(fanPin));
}

void fanBench(void) {
	fanBenchExt("3000");
}

/**
 * we are blinking for 16 seconds so that one can click the button and walk around to see the light blinking
 */
void milBench(void) {
	pinbench("0", "500", "500", "16", &enginePins.checkEnginePin, CONFIG(malfunctionIndicatorPin));
}

void starterRelayBench(void) {
	pinbench("0", "6000", "100", "1", &enginePins.starterControl, CONFIG(starterControlPin));
}

void fuelPumpBenchExt(const char *durationMs) {
	pinbench("0", durationMs, "100", "1", &enginePins.fuelPumpRelay, CONFIG(fuelPumpPin));
}

void acRelayBench(void) {
	pinbench("0", "1000", "100", "1", &enginePins.acRelay, CONFIG(acRelayPin));
}

void fuelPumpBench(void) {
	fuelPumpBenchExt("3000");
}

// fuelbench 5 1000 2
static void fuelbench(const char * onTimeStr, const char *offTimeStr, const char *countStr) {
	fuelbench2("0", "1", onTimeStr, offTimeStr, countStr);
}

static void doRunSpark(int humanIndex, const char *delayStr, const char * onTimeStr, const char *offTimeStr,
		const char *countStr) {
	if (humanIndex < 1 || humanIndex > engineConfiguration->specs.cylindersCount) {
		scheduleMsg(logger, "Invalid index: %d", humanIndex);
		return;
	}
	brain_pin_e b = CONFIG(ignitionPins)[humanIndex - 1];
	pinbench(delayStr, onTimeStr, offTimeStr, countStr, &enginePins.coils[humanIndex - 1], b);
}

/**
 * sparkbench2 0 1 5 1000 2
 */
static void sparkbench2(const char *delayStr, const char *indexStr, const char * onTimeStr, const char *offTimeStr,
		const char *countStr) {
	int index = atoi(indexStr);
	doRunSpark(index, delayStr, onTimeStr, offTimeStr, countStr);
}

/**
 * sparkbench 5 400 2
 * 5 ms ON, 400 ms OFF, two times
 */
static void sparkbench(const char * onTimeStr, const char *offTimeStr, const char *countStr) {
	sparkbench2("0", "1", onTimeStr, offTimeStr, countStr);
}


void dizzyBench(void) {
	pinbench("300", "5", "400", "3", &enginePins.dizzyOutput, engineConfiguration->dizzySparkOutputPin);
}

class BenchController : public PeriodicController<UTILITY_THREAD_STACK_SIZE> {
public:
	BenchController() : PeriodicController("BenchThread") { }
private:
	void PeriodicTask(efitick_t nowNt) override	{
		UNUSED(nowNt);
		setPeriod(50 /* ms */);

		validateStack("Bench", STACK_USAGE_BENCH, 128);

		// naive inter-thread communication - waiting for a flag
		if (isBenchTestPending) {
			isBenchTestPending = false;
			runBench(brainPin, pinX, delayMs, onTime, offTime, count);
		}
	}
};

static BenchController instance;

static void handleCommandX14(uint16_t index) {
	switch (index) {
	case 1:
		// cmd_test_fuel_pump
		fuelPumpBench();
		return;
	case 2:
		grabTPSIsClosed();
		return;
	case 3:
		grabTPSIsWideOpen();
		return;
	// case 4: tps2_closed
	// case 5: tps2_wot
	case 6:
		grabPedalIsUp();
		return;
	case 7:
		grabPedalIsWideOpen();
		return;
	case 8:
#if (BOARD_TLE8888_COUNT > 0)
		requestTLE8888initialization();
#endif
		return;
	case 9:
		acRelayBench();
		return;
	case 0xA:
		// cmd_write_config
#if EFI_PROD_CODE
		writeToFlashNow();
#endif /* EFI_PROD_CODE */
		return;
	case 0xB:
		starterRelayBench();
		return;
	case 0xD:
		engine->directSelfStimulation = true;
		return;
#if EFI_ELECTRONIC_THROTTLE_BODY
	case 0xE:
		etbAutocal(0);
		return;
	case 0xC:
		engine->etbAutoTune = true;
		return;
	case 0x10:
		engine->etbAutoTune = false;
#if EFI_TUNER_STUDIO
		tsOutputChannels.calibrationMode = TsCalMode::None;
#endif // EFI_TUNER_STUDIO
		return;
#endif
	case 0xF:
		engine->directSelfStimulation = false;
		return;
	}
}

// todo: this is probably a wrong place for this method now
void executeTSCommand(uint16_t subsystem, uint16_t index) {
	scheduleMsg(logger, "IO test subsystem=%d index=%d", subsystem, index);

	bool running = !ENGINE(rpmCalculator).isStopped();

    if (subsystem == 0x11) {
        clearWarnings();
	} else if (subsystem == 0x12 && !running) {
		doRunSpark(index, "300", "4", "400", "3");
	} else if (subsystem == 0x13 && !running) {
		doRunFuel(index, "300", "4", "400", "3");
	} else if (subsystem == 0x14) {
		handleCommandX14(index);
	} else if (subsystem == 0x15) {
		fanBench();
	} else if (subsystem == 0x16) {
		// cmd_test_check_engine_light
		milBench();
	} else if (subsystem == 0x17) {
		// cmd_test_idle_valve
#if EFI_IDLE_CONTROL
		startIdleBench();
#endif /* EFI_IDLE_CONTROL */
	} else if (subsystem == 0x18) {
#if EFI_CJ125 && HAL_USE_SPI
		cjStartCalibration();
#endif /* EFI_CJ125 */
	} else if (subsystem == 0x20 && index == 0x3456) {
		// call to pit
		setCallFromPitStop(30000);
	} else if (subsystem == 0x21) {
	} else if (subsystem == 0x30) {
		setEngineType(index);
	} else if (subsystem == 0x31) {
		setEngineType(DEFAULT_ENGINE_TYPE);
	} else if (subsystem == 0x79) {
		scheduleStopEngine();
	} else if (subsystem == 0xba) {
#if EFI_PROD_CODE
		jump_to_bootloader();
#endif /* EFI_PROD_CODE */
	} else if (subsystem == 0xbb) {
#if EFI_PROD_CODE
		rebootNow();
#endif /* EFI_PROD_CODE */
	}
}

void initBenchTest(Logging *sharedLogger) {
	logger = sharedLogger;

	addConsoleAction("fuelpumpbench", fuelPumpBench);
	addConsoleAction("acrelaybench", acRelayBench);
	addConsoleActionS("fuelpumpbench2", fuelPumpBenchExt);
	addConsoleAction("fanbench", fanBench);
	addConsoleActionS("fanbench2", fanBenchExt);
	addConsoleAction("dizzybench", dizzyBench); // this is useful for tach output testing

	addConsoleAction("starterbench", starterRelayBench);
	addConsoleAction("milbench", milBench);
	addConsoleActionSSS("fuelbench", fuelbench);
	addConsoleActionSSS("sparkbench", sparkbench);

	addConsoleActionSSSSS("fuelbench2", fuelbench2);
	addConsoleActionSSSSS("sparkbench2", sparkbench2);
	instance.setPeriod(200 /*ms*/);
	instance.Start();
}

#endif /* EFI_UNIT_TEST */
#endif
