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

#include "pch.h"

#if EFI_ENGINE_CONTROL
#if !EFI_UNIT_TEST

#include "flash_main.h"
#include "bench_test.h"
#include "main_trigger_callback.h"
#include "periodic_thread_controller.h"
#include "electronic_throttle.h"
#include "malfunction_central.h"
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

bool isRunningBenchTest() {
	return isRunningBench;
}

static void benchOn(OutputPin* output) {
	output->setValue(true);
}

static void benchOff(OutputPin* output) {
#if EFI_PROD_CODE && (BOARD_EXT_GPIOCHIPS > 0)
	static char pin_error[64];

	brain_pin_diag_e diag = output->getDiag();
	if (diag == PIN_INVALID) {
		efiPrintf("No Diag on this pin");
	} else {
		pinDiag2string(pin_error, sizeof(pin_error), diag);
		efiPrintf("Diag says %s", pin_error);
	}
#endif // EFI_PROD_CODE
	output->setValue(false);
}

struct BenchParams {
	OutputPin* Pin;
	float OnTimeMs;
	float OffTimeMs;
	size_t Count = 1;
};

static void runBench(BenchParams& params) {
	int onTimeUs = MS2US(maxF(0.1, params.OnTimeMs));
	int offTimeUs = MS2US(maxF(0.1, params.OffTimeMs));

	if (onTimeUs > TOO_FAR_INTO_FUTURE_US) {
		firmwareError(ObdCode::CUSTOM_ERR_BENCH_PARAM, "onTime above limit %dus", TOO_FAR_INTO_FUTURE_US);
		return;
	}

	efiPrintf("Running bench: ON_TIME=%d us OFF_TIME=%d us Counter=%d", onTimeUs, offTimeUs, params.Count);
	efiPrintf("output on %s", hwPortname(params.Pin->brainPin));

	isRunningBench = true;

	for (size_t i = 0; isRunningBench && i < params.Count; i++) {
		engine->outputChannels.testBenchIter = i;
		efitick_t nowNt = getTimeNowNt();
		// start in a short time so the scheduler can precisely schedule the start event
		efitick_t startTime = nowNt + US2NT(50);
		efitick_t endTime = startTime + US2NT(onTimeUs);

		// Schedule both events
		engine->executor.scheduleByTimestampNt("bstart", nullptr, startTime, {benchOn, params.Pin});
		engine->executor.scheduleByTimestampNt("bend", nullptr, endTime, {benchOff, params.Pin});

		// Wait one full cycle time for the event + delay to happen
		chThdSleepMicroseconds(onTimeUs + offTimeUs);
	}
	/* last */
	engine->outputChannels.testBenchIter++;

	efiPrintf("Done!");
	isRunningBench = false;
}

static bool isBenchTestPending = false;
static bool widebandUpdatePending = false;

static BenchParams benchParams;

static chibios_rt::CounterSemaphore benchSemaphore(0);

static void pinbench(float ontime, float offtime, size_t iterations, OutputPin& pin)
{
	benchParams = { &pin, ontime, offtime, iterations };

	// let's signal bench thread to wake up
	isBenchTestPending = true;
	benchSemaphore.signal();
}

static void cancelBenchTest() {
	isRunningBench = false;
}

/*==========================================================================*/

static void doRunFuelInjBench(size_t humanIndex, float onTime, float offTime, int count) {
	if (humanIndex < 1 || humanIndex > engineConfiguration->cylindersCount) {
		efiPrintf("Invalid index: %d", humanIndex);
		return;
	}
	pinbench(onTime, offTime, count, enginePins.injectors[humanIndex - 1]);
}

static void doRunSparkBench(size_t humanIndex, float onTime, float offTime, int count) {
	if (humanIndex < 1 || humanIndex > engineConfiguration->cylindersCount) {
		efiPrintf("Invalid index: %d", humanIndex);
		return;
	}
	pinbench(onTime, offTime, count, enginePins.coils[humanIndex - 1]);
}

static void doRunSolenoidBench(size_t humanIndex, float onTime, float offTime, int count) {
	if (humanIndex < 1 || humanIndex > TCU_SOLENOID_COUNT) {
		efiPrintf("Invalid index: %d", humanIndex);
		return;
	}
	pinbench(onTime, offTime, count, enginePins.tcuSolenoids[humanIndex - 1]);
}

static void doRunBenchTestLuaOutput(size_t humanIndex, float onTime, float offTime, int count) {
	if (humanIndex < 1 || humanIndex > LUA_PWM_COUNT) {
		efiPrintf("Invalid index: %d", humanIndex);
		return;
	}
	pinbench(onTime, offTime, count, enginePins.luaOutputPins[humanIndex - 1]);
}

/**
 * cylinder #2, 5ms ON, 1000ms OFF, repeat 3 times
 * fuelInjBenchExt 2 5 1000 3
 */
static void fuelInjBenchExt(float humanIndex, float onTime, float offTime, float count) {
	doRunFuelInjBench((int)humanIndex, onTime, offTime, (int)count);
}

/**
 * fuelbench 5 1000 2
 */
static void fuelInjBench(float onTime, float offTime, float count) {
	fuelInjBenchExt(1, onTime, offTime, count);
}

/**
 * sparkbench2 1 5 1000 2
 */
static void sparkBenchExt(float humanIndex, float onTime, float offTime, float count) {
	doRunSparkBench((int)humanIndex, onTime, offTime, (int)count);
}

/**
 * sparkbench 5 400 2
 * 5 ms ON, 400 ms OFF, two times
 */
static void sparkBench(float onTime, float offTime, float count) {
	sparkBenchExt(1, onTime, offTime, count);
}

/**
 * solenoid #2, 1000ms ON, 1000ms OFF, repeat 3 times
 * tcusolbench 2 1000 1000 3
 */
static void tcuSolenoidBench(float humanIndex, float onTime, float offTime, float count) {
	doRunSolenoidBench((int)humanIndex, onTime, offTime, (int)count);
}

/**
 * channel #1, 5ms ON, 1000ms OFF, repeat 3 times
 * fsiobench2 1 5 1000 3
 */
static void luaOutBench2(float humanIndex, float onTime, float offTime, float count) {
	doRunBenchTestLuaOutput((int)humanIndex, onTime, offTime, (int)count);
}

static void fanBenchExt(float onTime) {
	pinbench(onTime, 100.0, 1.0, enginePins.fanRelay);
}

void fanBench(void) {
	fanBenchExt(3000.0);
}

void fan2Bench(void) {
	pinbench(3000.0, 100.0, 1.0, enginePins.fanRelay2);
}

/**
 * we are blinking for 16 seconds so that one can click the button and walk around to see the light blinking
 */
void milBench(void) {
	pinbench(500.0, 500.0, 16, enginePins.checkEnginePin);
}

void starterRelayBench(void) {
	pinbench(6000.0, 100.0, 1, enginePins.starterControl);
}

static void fuelPumpBenchExt(float durationMs) {
	pinbench(durationMs, 100.0, 1.0, enginePins.fuelPumpRelay);
}

void acRelayBench(void) {
	pinbench(1000.0, 100.0, 1, enginePins.acRelay);
}

static void mainRelayBench(void) {
	// main relay is usually "ON" via FSIO thus bench testing that one is pretty unusual
	engine->mainRelayBenchStartNt = getTimeNowNt();
}

static void hpfpValveBench(void) {
	pinbench(20.0, engineConfiguration->benchTestOffTime, engineConfiguration->benchTestCount, enginePins.hpfpValve);
}

void fuelPumpBench(void) {
	fuelPumpBenchExt(3000.0);
}

class BenchController : public ThreadController<UTILITY_THREAD_STACK_SIZE> {
public:
	BenchController() : ThreadController("BenchTest", PRIO_BENCH_TEST) { }
private:
	void ThreadTask() override	{
		while (true) {
			benchSemaphore.wait();

			if (isBenchTestPending) {
				isBenchTestPending = false;
				runBench(benchParams);
			}

			if (widebandUpdatePending) {
	#if EFI_WIDEBAND_FIRMWARE_UPDATE && EFI_CAN_SUPPORT
				updateWidebandFirmware();
	#endif
				widebandUpdatePending = false;
			}
		}
	}
};

static BenchController instance;

static void handleBenchCategory(uint16_t index) {
	switch(index) {
	case BENCH_MAIN_RELAY:
		mainRelayBench();
		return;
	case BENCH_HPFP_VALVE:
		hpfpValveBench();
		return;
	case BENCH_STARTER_ENABLE_RELAY:
		starterRelayBench();
		return;
	case BENCH_CHECK_ENGINE_LIGHT:
		// cmd_test_check_engine_light
		milBench();
		return;
	case BENCH_AC_COMPRESSOR_RELAY:
		acRelayBench();
		return;
	case BENCH_FAN_RELAY:
		fanBench();
		return;
	case BENCH_IDLE_VALVE:
		// cmd_test_idle_valve
#if EFI_IDLE_CONTROL
		startIdleBench();
#endif /* EFI_IDLE_CONTROL */
		return;
	case BENCH_FAN_RELAY_2:
		fan2Bench();
		return;
	case BENCH_CANCEL:
		cancelBenchTest();
		return;
	case BENCH_FUEL_PUMP_ON:
		engine->module<FuelPumpController>()->forcePumpState(true);
		return;
	case BENCH_FUEL_PUMP_OFF:
		engine->module<FuelPumpController>()->forcePumpState(false);
		return;
	default:
		firmwareError(ObdCode::OBD_PCM_Processor_Fault, "Unexpected bench function %d", index);
	}
}

static void handleCommandX14(uint16_t index) {
	switch (index) {
	case TS_GRAB_TPS_CLOSED:
		grabTPSIsClosed();
		return;
	case TS_GRAB_TPS_WOT:
		grabTPSIsWideOpen();
		return;
	// case 4: tps2_closed
	// case 5: tps2_wot
	case TS_GRAB_PEDAL_UP:
		grabPedalIsUp();
		return;
	case TS_GRAB_PEDAL_WOT:
		grabPedalIsWideOpen();
		return;
	case TS_RESET_TLE8888:
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
		engine->outputChannels.calibrationMode = (uint8_t)TsCalMode::None;
#endif // EFI_TUNER_STUDIO
		return;
#endif
	case 0x12:
		widebandUpdatePending = true;
		benchSemaphore.signal();
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
		firmwareError(ObdCode::OBD_PCM_Processor_Fault, "Unexpected dbank command", index);
#endif
		return;
	case 0x15:
#if EFI_PROD_CODE
		extern bool burnWithoutFlash;
		burnWithoutFlash = true;
#endif // EFI_PROD_CODE
		return;
	default:
		firmwareError(ObdCode::OBD_PCM_Processor_Fault, "Unexpected bench x14 %d", index);
	}
}

extern bool rebootForPresetPending;

static void fatalErrorForPresetApply() {
	rebootForPresetPending = true;
	firmwareError(ObdCode::OBD_PCM_Processor_Fault,
		"\n\nTo complete preset apply:\n"
		"   1. Close TunerStudio\n"
		"   2. Power cycle ECU\n"
		"   3. Open TunerStudio and reconnect\n\n");
}

void executeTSCommand(uint16_t subsystem, uint16_t index) {
	efiPrintf("IO test subsystem=%d index=%d", subsystem, index);

	bool running = !engine->rpmCalculator.isStopped();

	switch (subsystem) {
	case TS_CLEAR_WARNINGS:
		clearWarnings();
		break;

	case TS_DEBUG_MODE:
		engineConfiguration->debugMode = (debug_mode_e)index;
		break;

	case TS_IGNITION_CATEGORY:
		if (!running) {
			doRunSparkBench(index, engineConfiguration->ignTestOnTime,
				engineConfiguration->ignTestOffTime, engineConfiguration->ignTestCount);
		}
		break;

	case TS_INJECTOR_CATEGORY:
		if (!running) {
			doRunFuelInjBench(index, engineConfiguration->benchTestOnTime,
				engineConfiguration->benchTestOffTime, engineConfiguration->benchTestCount);
		}
		break;

	case TS_SOLENOID_CATEGORY:
		if (!running) {
			doRunSolenoidBench(index, 1000.0,
				1000.0, engineConfiguration->benchTestCount);
		}
		break;

	case TS_LUA_OUTPUT_CATEGORY:
		if (!running) {
			doRunBenchTestLuaOutput(index, 4.0,
				engineConfiguration->benchTestOffTime, engineConfiguration->benchTestCount);
		}
		break;

	case TS_X14:
		handleCommandX14(index);
		break;
#if defined(EFI_WIDEBAND_FIRMWARE_UPDATE) && EFI_CAN_SUPPORT
	case TS_WIDEBAND:
		setWidebandOffset(index);
		break;
#endif // EFI_WIDEBAND_FIRMWARE_UPDATE && EFI_CAN_SUPPORT
	case TS_BENCH_CATEGORY:
		handleBenchCategory(index);
		break;

	case TS_SET_ENGINE_TYPE:
		fatalErrorForPresetApply();
		setEngineType(index);
		break;

	case TS_SET_DEFAULT_ENGINE:
		fatalErrorForPresetApply();
		setEngineType(engine_type_e::DEFAULT_ENGINE_TYPE);
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

#if EFI_USE_OPENBLT
	case 0xbc:
		/* Jump to OpenBLT if present */
		jump_to_openblt();
		break;
#endif

	default:
		firmwareError(ObdCode::OBD_PCM_Processor_Fault, "Unexpected bench subsystem %d %d", subsystem, index);
	}
}

void onConfigurationChangeBenchTest() {
	// default values if configuration was not specified
	if (engineConfiguration->benchTestOnTime == 0) {
		engineConfiguration->benchTestOnTime = 4; 
	}

	if (engineConfiguration->benchTestOffTime < 5) {
		engineConfiguration->benchTestOffTime = 500;
	}

	if (engineConfiguration->benchTestCount < 1) {
		engineConfiguration->benchTestCount = 3;
	}
}

void initBenchTest() {
	addConsoleAction("fuelpumpbench", fuelPumpBench);
	addConsoleActionF("fuelpumpbench2", fuelPumpBenchExt);

	addConsoleActionFFF(CMD_FUEL_BENCH, fuelInjBench);
	addConsoleActionFFFF("fuelbench2", fuelInjBenchExt);

	addConsoleActionFFF(CMD_SPARK_BENCH, sparkBench);
	addConsoleActionFFFF("sparkbench2", sparkBenchExt);

	addConsoleActionFFFF("tcusolbench", tcuSolenoidBench);

	addConsoleAction(CMD_AC_RELAY_BENCH, acRelayBench);

	addConsoleAction(CMD_FAN_BENCH, fanBench);
	addConsoleAction(CMD_FAN2_BENCH, fan2Bench);
	addConsoleActionF("fanbench2", fanBenchExt);

	addConsoleAction("mainrelaybench", mainRelayBench);

#if EFI_WIDEBAND_FIRMWARE_UPDATE && EFI_CAN_SUPPORT
	addConsoleAction("update_wideband", []() { widebandUpdatePending = true; });
	addConsoleActionI("set_wideband_index", [](int index) { setWidebandOffset(index); });
#endif // EFI_WIDEBAND_FIRMWARE_UPDATE && EFI_CAN_SUPPORT

	addConsoleAction(CMD_STARTER_BENCH, starterRelayBench);
	addConsoleAction(CMD_MIL_BENCH, milBench);
	addConsoleAction(CMD_HPFP_BENCH, hpfpValveBench);

	addConsoleActionFFFF("luabench2", luaOutBench2);
	instance.start();
	onConfigurationChangeBenchTest();
}

#endif /* EFI_UNIT_TEST */
#endif
