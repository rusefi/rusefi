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
#include "trigger_emulator_algo.h"
#include "microsecond_timer.h"

#if EFI_WIDEBAND_FIRMWARE_UPDATE
#include "rusefi_wideband.h"
#endif // EFI_WIDEBAND_FIRMWARE_UPDATE

#if EFI_PROD_CODE
#include "rusefi.h"
#include "mpu_util.h"
#endif /* EFI_PROD_CODE */

#if (BOARD_TLE8888_COUNT > 0)
#include "gpio/tle8888.h"
#endif // BOARD_TLE8888_COUNT

static bool isRunningBench = false;

bool isRunningBenchTest(void) {
	return isRunningBench;
}

static scheduling_s benchSchedStart;
static scheduling_s benchSchedEnd;

void benchOn(OutputPin* output) {
	output->setValue(true);
}

void benchOff(OutputPin* output) {
	output->setValue(false);
}

static void runBench(brain_pin_e brainPin, OutputPin *output, float delayMs, float onTimeMs, float offTimeMs,
		int count) {
	int delayUs = MS2US(maxF(0.1, delayMs));
	int onTimeUs = MS2US(maxF(0.1, onTimeMs));
	int offTimeUs = MS2US(maxF(0.1, offTimeMs));

	if (onTimeUs > TOO_FAR_INTO_FUTURE_US) {
		firmwareError(CUSTOM_ERR_6703, "onTime above limit %dus", TOO_FAR_INTO_FUTURE_US);
		return;
	}

	efiPrintf("Running bench: ON_TIME=%d us OFF_TIME=%d us Counter=%d", onTimeUs, offTimeUs, count);
	efiPrintf("output on %s", hwPortname(brainPin));

	chThdSleepMicroseconds(delayUs);

	isRunningBench = true;

	for (int i = 0; i < count; i++) {
		efitick_t nowNt = getTimeNowNt();
		// start in a short time so the scheduler can precisely schedule the start event
		efitick_t startTime = nowNt + US2NT(50);
		efitick_t endTime = startTime + US2NT(onTimeUs);

		// Schedule both events
		engine->executor.scheduleByTimestampNt("bstart", &benchSchedStart, startTime, {benchOn, output});
		engine->executor.scheduleByTimestampNt("bend", &benchSchedEnd, endTime, {benchOff, output});

		// Wait one full cycle time for the event + delay to happen
		chThdSleepMicroseconds(onTimeUs + offTimeUs);
	}

	efiPrintf("Done!");
	isRunningBench = false;
}

static volatile bool isBenchTestPending = false;
static bool widebandUpdatePending = false;
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

static void doRunFuel(size_t humanIndex, const char *delayStr, const char * onTimeStr, const char *offTimeStr,
		const char *countStr) {
	if (humanIndex < 1 || humanIndex > engineConfiguration->specs.cylindersCount) {
		efiPrintf("Invalid index: %d", humanIndex);
		return;
	}
	brain_pin_e b = CONFIG(injectionPins)[humanIndex - 1];
	pinbench(delayStr, onTimeStr, offTimeStr, countStr, &enginePins.injectors[humanIndex - 1], b);
}

static void doTestSolenoid(int humanIndex, const char *delayStr, const char * onTimeStr, const char *offTimeStr,
		const char *countStr) {
	if (humanIndex < 1 || humanIndex > TCU_SOLENOID_COUNT) {
		efiPrintf("Invalid index: %d", humanIndex);
		return;
	}
	brain_pin_e b = CONFIG(tcu_solenoid)[humanIndex - 1];
	pinbench(delayStr, onTimeStr, offTimeStr, countStr, &enginePins.tcuSolenoids[humanIndex - 1], b);
}

static void doBenchTestFsio(int humanIndex, const char *delayStr, const char * onTimeStr, const char *offTimeStr,
		const char *countStr) {
	if (humanIndex < 1 || humanIndex > FSIO_COMMAND_COUNT) {
		efiPrintf("Invalid index: %d", humanIndex);
		return;
	}
	brain_pin_e b = CONFIG(fsioOutputPins)[humanIndex - 1];
	pinbench(delayStr, onTimeStr, offTimeStr, countStr, &enginePins.fsioOutputs[humanIndex - 1], b);
}

/**
 * delay 100, cylinder #2, 5ms ON, 1000ms OFF, repeat 3 times
 * fuelbench2 100 2 5 1000 3
 */
static void fuelbench2(const char *delayStr, const char *indexStr, const char * onTimeStr, const char *offTimeStr,
		const char *countStr) {
	int index = atoi(indexStr);
	doRunFuel(index, delayStr, onTimeStr, offTimeStr, countStr);
}

/**
 * delay 100, solenoid #2, 1000ms ON, 1000ms OFF, repeat 3 times
 * tcusolbench 100 2 1000 1000 3
 */
static void tcusolbench(const char *delayStr, const char *indexStr, const char * onTimeStr, const char *offTimeStr,
		const char *countStr) {
	int index = atoi(indexStr);
	doTestSolenoid(index, delayStr, onTimeStr, offTimeStr, countStr);
}

/**
 * delay 100, channel #1, 5ms ON, 1000ms OFF, repeat 3 times
 * fsiobench2 100 1 5 1000 3
 */
static void fsioBench2(const char *delayStr, const char *indexStr, const char * onTimeStr, const char *offTimeStr,
		const char *countStr) {
	int index = atoi(indexStr);
	doBenchTestFsio(index, delayStr, onTimeStr, offTimeStr, countStr);
}

static void fanBenchExt(const char *durationMs) {
	pinbench("0", durationMs, "100", "1", &enginePins.fanRelay, CONFIG(fanPin));
}

void fanBench(void) {
	fanBenchExt("3000");
}

void fan2Bench(void) {
	pinbench("0", "3000", "100", "1", &enginePins.fanRelay2, CONFIG(fan2Pin));
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

void mainRelayBench(void) {
	// main relay is usually "ON" via FSIO thus bench testing that one is pretty unusual
	engine->mainRelayBenchStartNt = getTimeNowNt();
}

void hpfpValveBench(void) {
	pinbench(/*delay*/"1000", /* onTime */"20", /*oftime*/"500", "3", &enginePins.hpfpValve, CONFIG(hpfpValvePin));
}

void fuelPumpBench(void) {
	fuelPumpBenchExt("3000");
}

// fuelbench 5 1000 2
static void fuelbench(const char * onTimeStr, const char *offTimeStr, const char *countStr) {
	fuelbench2("0", "1", onTimeStr, offTimeStr, countStr);
}

static void doRunSpark(size_t humanIndex, const char *delayStr, const char * onTimeStr, const char *offTimeStr,
		const char *countStr) {
	if (humanIndex < 1 || humanIndex > engineConfiguration->specs.cylindersCount) {
		efiPrintf("Invalid index: %d", humanIndex);
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

		if (widebandUpdatePending) {
#if EFI_WIDEBAND_FIRMWARE_UPDATE && EFI_CAN_SUPPORT
			updateWidebandFirmware();
#endif
			widebandUpdatePending = false;
		}
	}
};

static BenchController instance;

static void handleBenchCategory(uint16_t index) {
	switch(index) {
	case CMD_TS_BENCH_MAIN_RELAY:
		mainRelayBench();
		return;
	case CMD_TS_BENCH_HPFP_VALVE:
		hpfpValveBench();
		return;
	case CMD_TS_BENCH_FUEL_PUMP:
		// cmd_test_fuel_pump
		fuelPumpBench();
		return;
	case CMD_TS_BENCH_STARTER_ENABLE_RELAY:
		starterRelayBench();
		return;
	case CMD_TS_BENCH_CHECK_ENGINE_LIGHT:
		// cmd_test_check_engine_light
		milBench();
		return;
	case CMD_TS_BENCH_AC_COMPRESSOR_RELAY:
		acRelayBench();
		return;
	case CMD_TS_BENCH_FAN_RELAY:
		fanBench();
		return;
	case CMD_TS_BENCH_AC_FAN_RELAY:
		fan2Bench();
		return;
	default:
		firmwareError(OBD_PCM_Processor_Fault, "Unexpected bench function %d", index);
	}
}

static void handleCommandX14(uint16_t index) {
	switch (index) {
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
		tle8888_req_init();
#endif
		return;
	case 0xA:
		// cmd_write_config
#if EFI_INTERNAL_FLASH
		writeToFlashNow();
#endif /* EFI_INTERNAL_FLASH */
		return;
#if EFI_EMULATE_POSITION_SENSORS
	case 0xD:
		enableTriggerStimulator();
		return;
	case 0xF:
		disableTriggerStimulator();
		return;
	case 0x13:
		enableExternalTriggerStimulator();
		return;
#endif // EFI_EMULATE_POSITION_SENSORS
#if EFI_ELECTRONIC_THROTTLE_BODY
	case 0xE:
		etbAutocal(0);
		return;
	case 0x11:
		etbAutocal(1);
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
	case 0x12:
		widebandUpdatePending = true;
		return;
	case 0x14:
#ifdef STM32F7
		void sys_dual_bank(void);
		/**
		 * yes, this would instantly cause a hard fault as a random sequence of bytes is decoded as instructions
		 * and that's the intended behavious - the point is to set flash properly and to re-flash once in proper configuration
		 */
		sys_dual_bank();
		rebootNow();
#else
		firmwareError(OBD_PCM_Processor_Fault, "Unexpected dbank command", index);
#endif
		return;
	default:
		firmwareError(OBD_PCM_Processor_Fault, "Unexpected bench x14 %d", index);
	}
}

extern bool rebootForPresetPending;

static void fatalErrorForPresetApply() {
	rebootForPresetPending = true;
	firmwareError(OBD_PCM_Processor_Fault,
		"\n\nTo complete preset apply:\n"
		"   1. Close TunerStudio\n"
		"   2. Power cycle ECU\n"
		"   3. Open TunerStudio and reconnect\n\n");
}

void executeTSCommand(uint16_t subsystem, uint16_t index) {
	efiPrintf("IO test subsystem=%d index=%d", subsystem, index);

	bool running = !ENGINE(rpmCalculator).isStopped();

	switch (subsystem) {
	case 0x11:
		clearWarnings();
		break;

	case CMD_TS_IGNITION_CATEGORY:
		if (!running) {
			doRunSpark(index, "300", "4", "400", "3");
		}
		break;

	case CMD_TS_INJECTOR_CATEGORY:
		if (!running) {
			doRunFuel(index, "300", "4", "400", "3");
		}
		break;

	case CMD_TS_SOLENOID_CATEGORY:
		if (!running) {
			doTestSolenoid(index, "300", "1000", "1000", "3");
		}
		break;

	case CMD_TS_FSIO_CATEGORY:
		if (!running) {
			doBenchTestFsio(index, "300", "4", "400", "3");
		}
		break;

	case CMD_TS_X14:
		handleCommandX14(index);
		break;
#ifdef EFI_WIDEBAND_FIRMWARE_UPDATE
	case 0x15:
		setWidebandOffset(index);
		break;
#endif // EFI_WIDEBAND_FIRMWARE_UPDATE
	case CMD_TS_BENCH_CATEGORY:
		handleBenchCategory(index);
		break;

	case CMD_TS_X17:
		// cmd_test_idle_valve
#if EFI_IDLE_CONTROL
		startIdleBench();
#endif /* EFI_IDLE_CONTROL */
		break;

	case 0x18:
#if EFI_CJ125 && HAL_USE_SPI
		cjStartCalibration();
#endif /* EFI_CJ125 */
		break;

	case 0x20:
		if (index == 0x3456) {
			// call to pit
			setCallFromPitStop(30000);
		}
		break;

	case 0x30:
		fatalErrorForPresetApply();
		setEngineType(index);
		break;

	case CMD_TS_X31:
		fatalErrorForPresetApply();
		setEngineType(DEFAULT_ENGINE_TYPE);
		break;

	case 0x79:
		scheduleStopEngine();
		break;

	case 0xba:
#if EFI_PROD_CODE
		jump_to_bootloader();
#endif /* EFI_PROD_CODE */
		break;

	case 0xbb:
#if EFI_PROD_CODE
		rebootNow();
#endif /* EFI_PROD_CODE */
		break;
	default:
		firmwareError(OBD_PCM_Processor_Fault, "Unexpected bench subsystem %d %d", subsystem, index);
	}
}

void initBenchTest() {
	addConsoleAction("fuelpumpbench", fuelPumpBench);
	addConsoleAction(CMD_AC_RELAY_BENCH, acRelayBench);
	addConsoleActionS("fuelpumpbench2", fuelPumpBenchExt);
	addConsoleAction(CMD_FAN_BENCH, fanBench);
	addConsoleAction(CMD_FAN2_BENCH, fan2Bench);
	addConsoleAction("mainrelaybench", mainRelayBench);
	addConsoleActionS("fanbench2", fanBenchExt);

#if EFI_WIDEBAND_FIRMWARE_UPDATE
	addConsoleAction("update_wideband", []() { widebandUpdatePending = true; });
	addConsoleActionI("set_wideband_index", [](int index) { setWidebandOffset(index); });
#endif // EFI_WIDEBAND_FIRMWARE_UPDATE

	addConsoleAction(CMD_STARTER_BENCH, starterRelayBench);
	addConsoleAction(CMD_MIL_BENCH, milBench);
	addConsoleActionSSS(CMD_FUEL_BENCH, fuelbench);
	addConsoleActionSSS(CMD_SPARK_BENCH, sparkbench);
	addConsoleAction(CMD_HPFP_BENCH, hpfpValveBench);

	addConsoleActionSSSSS("fuelbench2", fuelbench2);
	addConsoleActionSSSSS("tcusolbench", tcusolbench);
	addConsoleActionSSSSS("fsiobench2", fsioBench2);
	addConsoleActionSSSSS("sparkbench2", sparkbench2);
	instance.setPeriod(200 /*ms*/);
	instance.Start();
}

#endif /* EFI_UNIT_TEST */
#endif
