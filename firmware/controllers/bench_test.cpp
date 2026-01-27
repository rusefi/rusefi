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
#include "tunerstudio.h"
#include "tunerstudio_calibration_channel.h"
#include "long_term_fuel_trim.h"
#include "can_common.h"
#include "can_rx.h"
#include "value_lookup.h"
#include "can_msg_tx.h"
#include "gm_sbc.h" // setStepperHw

#include "fw_configuration.h"
#include "board_overrides.h"

static bool isRunningBench = false;
static OutputPin *outputOnTheBenchTest = nullptr;

bool isRunningBenchTest() {
	return isRunningBench;
}

const OutputPin *getOutputOnTheBenchTest() {
    return outputOnTheBenchTest;
}

#if !EFI_UNIT_TEST

#include "flash_main.h"
#include "bench_test.h"
#include "main_trigger_callback.h"
#include "periodic_thread_controller.h"
#include "electronic_throttle.h"
#include "electronic_throttle_impl.h"
#include "malfunction_central.h"
#include "trigger_emulator_algo.h"
#include "vvt.h"
#include "microsecond_timer.h"
#include "rusefi_wideband.h"

#if EFI_PROD_CODE
#include "rusefi.h"
#include "mpu_util.h"
#endif /* EFI_PROD_CODE */

#if (BOARD_TLE8888_COUNT > 0)
#include "gpio/tle8888.h"
#endif // BOARD_TLE8888_COUNT

#if EFI_FILE_LOGGING
#include "mmc_card.h"
#endif

static scheduling_s benchSchedStart;
static scheduling_s benchSchedEnd;

#if EFI_SIMULATOR
static int savedPinToggleCounter = 0;
static uint32_t savedDurationsInStateMs[2] = { 0, 0 };
#endif // EFI_SIMULATOR


#define BENCH_MSG "bench"

static void benchOn(OutputPin* output) {
	output->setValue(BENCH_MSG, true, /*isForce*/ true);
}

static void benchOff(OutputPin* output) {
#if EFI_PROD_CODE && (BOARD_EXT_GPIOCHIPS > 0)
	static char pin_error[64];

	brain_pin_diag_e diag = output->getDiag();
	if (diag == PIN_UNKNOWN) {
		efiPrintf("No Diag on this pin");
	} else {
		pinDiag2string(pin_error, sizeof(pin_error), diag);
		efiPrintf("Diag says %s", pin_error);
	}
#endif // EFI_PROD_CODE
	output->setValue(BENCH_MSG, false, /*isForce*/ true);
}

static void runBench(OutputPin *output, float onTimeMs, float offTimeMs, int count, bool swapOnOff) {
	int onTimeUs = MS2US(std::max(0.1f, onTimeMs));
	int offTimeUs = MS2US(std::max(0.1f, offTimeMs));

	if (onTimeUs > TOO_FAR_INTO_FUTURE_US) {
		firmwareError(ObdCode::CUSTOM_ERR_BENCH_PARAM, "onTime above limit %dus", TOO_FAR_INTO_FUTURE_US);
		return;
	}

	efiPrintf("Running bench: ON_TIME=%d us OFF_TIME=%d us Counter=%d", onTimeUs, offTimeUs, count);
	efiPrintf("output on %s", hwPortname(output->brainPin));

	isRunningBench = true;
	outputOnTheBenchTest = output;

	for (int i = 0; isRunningBench && i < count; i++) {
		engine->outputChannels.testBenchIter = i;
		efitick_t nowNt = getTimeNowNt();
		// start in a short time so the scheduler can precisely schedule the start event
		efitick_t startTime = nowNt + US2NT(50);
		efitick_t endTime = startTime + US2NT(onTimeUs);

		auto const bstartAction{ swapOnOff ? action_s::make<benchOff>(output) : action_s::make<benchOn>(output) };
		auto const bendAction{ swapOnOff ? action_s::make<benchOn>(output) : action_s::make<benchOff>(output) };

		// Schedule both events
		engine->scheduler.schedule("bstart", &benchSchedStart, startTime, bstartAction);
		engine->scheduler.schedule("bend", &benchSchedEnd, endTime, bendAction);

		// Wait one full cycle time for the event + delay to happen
		chThdSleepMicroseconds(onTimeUs + offTimeUs);
	}
	/* last */
	engine->outputChannels.testBenchIter++;

#if EFI_SIMULATOR
    // save the current counters and durations after the test while the pin is still controlled
	savedPinToggleCounter = output->pinToggleCounter;
	savedDurationsInStateMs[0] = output->durationsInStateMs[0];
	savedDurationsInStateMs[1] = output->durationsInStateMs[1];
#endif // EFI_SIMULATOR

	efiPrintf("Done!");
	outputOnTheBenchTest = nullptr;
	isRunningBench = false;
}

// todo: migrate to smarter getOutputOnTheBenchTest() approach?
static volatile bool isBenchTestPending = false;
static bool widebandUpdatePending = false;
static bool widebandUpdateFromFile = false;
static uint8_t widebandUpdateHwId = 0;
static float globalOnTimeMs;
static float globalOffTimeMs;
static int globalCount;
static OutputPin* pinX;
static bool swapOnOff = false;

static chibios_rt::CounterSemaphore benchSemaphore(0);

static void pinbench(float ontimeMs, float offtimeMs, int iterations,
	OutputPin* pinParam, bool p_swapOnOff = false)
{
	globalOnTimeMs = ontimeMs;
	globalOffTimeMs = offtimeMs;
#if EFI_SIMULATOR
	globalCount = maxI(2, iterations);
#else
	globalCount = iterations;
#endif // EFI_SIMULATOR
	pinX = pinParam;
	swapOnOff = p_swapOnOff;
	// let's signal bench thread to wake up
	isBenchTestPending = true;
	benchSemaphore.signal();
}

static void cancelBenchTest() {
	isRunningBench = false;
}

static void doRunFuelInjBench(size_t humanIndex, float onTimeMs, float offTimeMs, int count) {
	if (humanIndex < 1 || humanIndex > engineConfiguration->cylindersCount) {
		efiPrintf("Invalid index: %d", humanIndex);
		return;
	}
	pinbench(onTimeMs, offTimeMs, count,
		&enginePins.injectors[humanIndex - 1]);
}

static void doRunSparkBench(size_t humanIndex, float onTime, float offTime, int count) {
	if (humanIndex < 1 || humanIndex > engineConfiguration->cylindersCount) {
		efiPrintf("Invalid index: %d", humanIndex);
		return;
	}
	pinbench(onTime, offTime, count, &enginePins.coils[humanIndex - 1]);
}

static void doRunSolenoidBench(size_t humanIndex, float onTime, float offTime, int count) {
	if (humanIndex < 1 || humanIndex > TCU_SOLENOID_COUNT) {
		efiPrintf("Invalid index: %d", humanIndex);
		return;
	}
	pinbench(onTime, offTime, count, &enginePins.tcuSolenoids[humanIndex - 1]);
}

static void doRunBenchTestLuaOutput(size_t humanIndex, float onTimeMs, float offTimeMs, int count) {
	if (humanIndex < 1 || humanIndex > LUA_PWM_COUNT) {
		efiPrintf("Invalid index: %d", humanIndex);
		return;
	}
	pinbench(onTimeMs, offTimeMs, count,
		&enginePins.luaOutputPins[humanIndex - 1]);
}

/**
 * cylinder #2, 5ms ON, 1000ms OFF, repeat 3 times
 * fuelInjBenchExt 2 5 1000 3
 */
static void fuelInjBenchExt(float humanIndex, float onTimeMs, float offTimeMs, float count) {
	doRunFuelInjBench((int)humanIndex, onTimeMs, offTimeMs, (int)count);
}

/**
 * fuelbench 5 1000 2
 */
static void fuelInjBench(float onTimeMs, float offTimeMs, float count) {
	fuelInjBenchExt(1, onTimeMs, offTimeMs, count);
}

/**
 * sparkbench2 1 5 1000 2
 */
static void sparkBenchExt(float humanIndex, float onTime, float offTimeMs, float count) {
	doRunSparkBench((int)humanIndex,  onTime, offTimeMs, (int)count);
}

/**
 * sparkbench 5 400 2
 * 5 ms ON, 400 ms OFF, two times
 */
static void sparkBench(float onTime, float offTimeMs, float count) {
	sparkBenchExt(1, onTime, offTimeMs, count);
}

/**
 * solenoid #2, 1000ms ON, 1000ms OFF, repeat 3 times
 * tcusolbench 2 1000 1000 3
 */
static void tcuSolenoidBench(float humanIndex, float onTime, float offTimeMs, float count) {
	doRunSolenoidBench((int)humanIndex, onTime, offTimeMs, (int)count);
}

static void fanBenchExt(float onTimeMs) {
	pinbench(onTimeMs, 100.0, 1, &enginePins.fanRelay);
}

void fanBench() {
	fanBenchExt(BENCH_FAN_DURATION);
}

void fan2Bench() {
	pinbench(3000.0, 100.0, 1, &enginePins.fanRelay2);
}

/**
 * we are blinking for 16 seconds so that one can click the button and walk around to see the light blinking
 */
void milBench() {
	pinbench(500.0, 500.0, 16, &enginePins.checkEnginePin);
}

void starterRelayBench() {
	pinbench(BENCH_STARTER_DURATION, 100.0, 1, &enginePins.starterControl);
}

static void fuelPumpBenchExt(float durationMs) {
	pinbench(durationMs, 100.0, 1,
		&enginePins.fuelPumpRelay);
}

void acRelayBench() {
	pinbench(BENCH_AC_RELAY_DURATION, 100.0, 1, &enginePins.acRelay);
}

static void mainRelayBench() {
	pinbench(BENCH_MAIN_RELAY_DURATION, 100.0, 1, &enginePins.mainRelay, true);
}

static void hpfpValveBench() {
	pinbench(engineConfiguration->benchTestOnTime, engineConfiguration->benchTestOffTime, engineConfiguration->benchTestCount,
		&enginePins.hpfpValve);
}

void fuelPumpBench() {
	fuelPumpBenchExt(BENCH_FUEL_PUMP_DURATION);
}

static void vvtValveBench(int vvtIndex) {
#if EFI_VVT_PID
	pinbench(BENCH_VVT_DURATION, 100.0, 1, getVvtOutputPin(vvtIndex));
#endif // EFI_VVT_PID
}

static void requestWidebandUpdate(int hwIndex, bool fromFile)
{
	widebandUpdateHwId = hwIndex;
	widebandUpdateFromFile = fromFile;
	widebandUpdatePending = true;
	benchSemaphore.signal();
}

class BenchController : public ThreadController<4 * UTILITY_THREAD_STACK_SIZE> {
public:
	BenchController() : ThreadController("BenchTest", PRIO_BENCH_TEST) { }
private:
	void ThreadTask() override	{
		while (true) {
			benchSemaphore.wait();

		    assertStackVoid("Bench", ObdCode::STACK_USAGE_MISC, EXPECTED_REMAINING_STACK);

			if (isBenchTestPending) {
				isBenchTestPending = false;
				runBench(pinX, globalOnTimeMs, globalOffTimeMs, globalCount, swapOnOff);
			}

			if (widebandUpdatePending) {
	#if EFI_WIDEBAND_FIRMWARE_UPDATE && EFI_CAN_SUPPORT
				if (widebandUpdateFromFile) {
					#if EFI_PROD_CODE
						updateWidebandFirmwareFromFile(widebandUpdateHwId);
					#endif
				} else {
					updateWidebandFirmware(widebandUpdateHwId);
				}
	#endif
				widebandUpdatePending = false;
			}
		}
	}
};

static BenchController instance;

static void auxOutBench(int index) {
    // todo!
    UNUSED(index);
}

#if EFI_HD_ACR
static void hdAcrBench(int index) {
    OutputPin*  pin = index == 0 ? &enginePins.harleyAcr : &enginePins.harleyAcr2;
    pinbench(BENCH_AC_RELAY_DURATION, 100.0, 1, pin);
}
#endif // EFI_HD_ACR

int luaCommandCounters[LUA_BUTTON_COUNT] = {};

void handleBenchCategory(uint16_t index) {
	switch(index) {
	case BENCH_VVT0_VALVE:
	    vvtValveBench(0);
		return;
	case BENCH_VVT1_VALVE:
	    vvtValveBench(1);
		return;
	case BENCH_VVT2_VALVE:
	    vvtValveBench(2);
		return;
	case BENCH_VVT3_VALVE:
	    vvtValveBench(3);
		return;
	case BENCH_AUXOUT0:
	    auxOutBench(0);
		return;
	case BENCH_AUXOUT1:
	    auxOutBench(1);
		return;
	case BENCH_AUXOUT2:
	    auxOutBench(2);
		return;
	case BENCH_AUXOUT3:
	    auxOutBench(3);
		return;
	case BENCH_AUXOUT4:
	    auxOutBench(4);
		return;
	case BENCH_AUXOUT5:
	    auxOutBench(5);
		return;
	case BENCH_AUXOUT6:
	    auxOutBench(6);
		return;
	case BENCH_AUXOUT7:
	    auxOutBench(7);
		return;
	case LUA_COMMAND_1:
		luaCommandCounters[0]++;
		return;
	case LUA_COMMAND_2:
		luaCommandCounters[1]++;
		return;
	case LUA_COMMAND_3:
		luaCommandCounters[2]++;
		return;
	case LUA_COMMAND_4:
		luaCommandCounters[3]++;
		return;
#if EFI_LTFT_CONTROL
	case LTFT_RESET:
		resetLongTermFuelTrim();
		return;
	case LTFT_APPLY_TO_VE:
		applyLongTermFuelTrimToVe();
		return;
	case LTFT_DEV_POKE:
		devPokeLongTermFuelTrim();
		return;
#endif // EFI_LTFT_CONTROL
#if EFI_HD_ACR
	case HD_ACR:
		hdAcrBench(0);
		return;
	case HD_ACR2:
		hdAcrBench(1);
		return;
#endif // EFI_HD_ACR
	case BENCH_HPFP_VALVE:
		hpfpValveBench();
		return;
	case BENCH_FUEL_PUMP:
		// cmd_test_fuel_pump
		fuelPumpBench();
		return;
	case BENCH_MAIN_RELAY:
		mainRelayBench();
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
	default:
		criticalError("Unexpected bench function %d", index);
	}
}

int getSavedBenchTestPinStates(uint32_t durationsInStateMs[2]) {
#if EFI_SIMULATOR
	durationsInStateMs[0] = savedDurationsInStateMs[0];
	durationsInStateMs[1] = savedDurationsInStateMs[1];
	return savedPinToggleCounter;
#else
    UNUSED(durationsInStateMs);
	return 0;
#endif // EFI_SIMULATOR
}

static void handleCommandX14(uint16_t index) {
	switch (index) {
	case TS_SET_STEPPER_IDLE:
	  setStepperHw();
	  onApplyPreset();
		return;
	case TS_GRAB_PEDAL_UP:
		grabPedalIsUp();
		return;
	case TS_GRAB_PEDAL_WOT:
		grabPedalIsWideOpen();
		return;
	case TS_GRAB_TPS_CLOSED:
		grapTps1PrimaryIsClosed();
		return;
	case TS_GRAB_TPS_OPEN:
		grapTps1PrimaryIsOpen();
		return;
	case TS_RESET_TLE8888:
		#if (BOARD_TLE8888_COUNT > 0)
			tle8888_req_init();
		#endif
		return;
	case TS_TCU_UPSHIFT_REQUEST:
	case TS_TCU_DOWNSHIFT_REQUEST:
	  // do nothing, we are catching with Lua
	  // this is temporary uaDASH API
		return;
	case TS_RESET_MC33810:
		#if EFI_PROD_CODE && (BOARD_MC33810_COUNT > 0)
			mc33810_req_init();
		#endif
		return;
#if EFI_SHAFT_POSITION_INPUT
	case TS_START_STOP_ENGINE:
	  // this is different from starter relay bench test!
	  startStopButtonToggle();
		return;
#endif // EFI_SHAFT_POSITION_INPUT
	case TS_WRITE_FLASH:
		// cmd_write_config
		#if EFI_CONFIGURATION_STORAGE
			writeToFlashNow();
		#endif /* EFI_CONFIGURATION_STORAGE */
		return;
	case TS_TRIGGER_STIMULATOR_ENABLE:
		#if EFI_EMULATE_POSITION_SENSORS == TRUE
			enableTriggerStimulator();
		#endif /* EFI_EMULATE_POSITION_SENSORS == TRUE */
		return;
	case TS_TRIGGER_STIMULATOR_DISABLE:
		#if EFI_EMULATE_POSITION_SENSORS == TRUE
			disableTriggerStimulator();
		#endif /* EFI_EMULATE_POSITION_SENSORS == TRUE */
		return;
	case TS_EXTERNAL_TRIGGER_STIMULATOR_ENABLE:
		#if EFI_EMULATE_POSITION_SENSORS == TRUE
			enableExternalTriggerStimulator();
		#endif /* EFI_EMULATE_POSITION_SENSORS == TRUE */
		return;
/*
    case TS_ETB_RESET:
		#if EFI_ELECTRONIC_THROTTLE_BODY == TRUE
		#if EFI_PROD_CODE
			etbPidReset();
		#endif
		#endif // EFI_ELECTRONIC_THROTTLE_BODY
		return;
*/
#if EFI_ELECTRONIC_THROTTLE_BODY
	case TS_ETB_AUTOCAL_0:
			etbAutocal(DC_Throttle1);
		return;
	case TS_ETB_AUTOCAL_1:
			etbAutocal(DC_Throttle2);
		return;
	case TS_ETB_AUTOCAL_0_FAST:
			etbAutocal(DC_Throttle1, false);
		return;
	case TS_ETB_AUTOCAL_1_FAST:
			etbAutocal(DC_Throttle2, false);
		return;
	case TS_ETB_START_AUTOTUNE:
			engine->etbAutoTune = true;
		return;
	case TS_ETB_STOP_AUTOTUNE:
			engine->etbAutoTune = false;
			#if EFI_TUNER_STUDIO
				tsCalibrationSetIdle();
			#endif // EFI_TUNER_STUDIO
		return;
	case TS_ETB_DISABLE_JAM_DETECT:
		engine->etbIgnoreJamProtection = true;
		return;
	case TS_EWG_AUTOCAL_0:
			etbAutocal(DC_Wastegate);
		return;
	case TS_EWG_AUTOCAL_0_FAST:
			etbAutocal(DC_Wastegate, false);
		return;
#endif // EFI_ELECTRONIC_THROTTLE_BODY
	case TS_WIDEBAND_UPDATE:
	case TS_WIDEBAND_UPDATE_FILE:
		// broadcast, for old WBO FWs
		requestWidebandUpdate(0xff, index == TS_WIDEBAND_UPDATE_FILE);
		return;
	case TS_ESTIMATE_TORQUE_TABLE:
		return;
	case COMMAND_X14_UNUSED_15:
		return;

#if EFI_PROD_CODE && EFI_FILE_LOGGING
	case TS_SD_MOUNT_PC:
		sdCardRequestMode(SD_MODE_PC);
		return;
	case TS_SD_MOUNT_ECU:
		sdCardRequestMode(SD_MODE_ECU);
		return;
	case TS_SD_UNMOUNT:
		sdCardRequestMode(SD_MODE_UNMOUNT);
		return;
	case TS_SD_FORMAT:
		sdCardRequestMode(SD_MODE_FORMAT);
		return;
	case TS_SD_DELETE_REPORTS:
		sdCardRemoveReportFiles();
		return;
#endif // EFI_FILE_LOGGING

	default:
		criticalError("Unexpected bench x14 %d", index);
	}
}

extern bool rebootForPresetPending;

static void applyPreset(int index) {
  setEngineType(index);
#if EFI_TUNER_STUDIO
	onApplyPreset();
#endif // EFI_TUNER_STUDIO
}

// placeholder to force custom_board_ts_command migration
void boardTsAction(uint16_t index) { UNUSED(index); }

#if EFI_CAN_SUPPORT
/**
 * for example to bench test injector 1
 * 0x77000C 0x66 0x00 ?? ?? ?? ??
 * 0x77000C 0x66 0x00 0x14 0x00 0x09 0x00 start/stop engine
 *
 * See also more complicated ISO-TP CANBus wrapper of complete TS protocol
 */
static void processCanUserControl(const CANRxFrame& frame) {
	// reserved data8[1]
	uint16_t subsystem = getTwoBytesLsb(frame, 2);
	uint16_t index = getTwoBytesLsb(frame, 4);
	executeTSCommand(subsystem, index);
}

   union FloatIntBytes {
          float f;
          int i;
          uint8_t bytes[sizeof(float)];
      };

static void processCanSetCalibration(const CANRxFrame& frame) {
	// todo allow changes of scalar settings via CANbus
	UNUSED(frame);
}
/**
 * CANbus protocol to query scalar calibrations using hash keys
 *
 * see fields_api.txt for well-known fields
 * see generated_fields_api_header.h for corresponding hashes
 */
static void processCanRequestCalibration(const CANRxFrame& frame) {
#if EFI_LUA_LOOKUP
  int hash = getFourBytesLsb(frame, 2);
  efiPrintf("processCanRequestCalibration=%x", hash);
  FloatIntBytes fb;
  fb.f = getConfigValueByHash(hash);

	CanTxMessage msg(CanCategory::BENCH_TEST, (int)bench_test_packet_ids_e::ECU_GET_CALIBRATION, 8, /*bus*/0, /*isExtended*/true);
  for (size_t i = 0;i<sizeof(float);i++) {
    msg[4 + i] = fb.bytes[i];
  }

  fb.i = hash;
  // first half of the packed is copy of that hash value so that recipients know what they are processing
  for (size_t i = 0;i<sizeof(float);i++) {
    msg[i] = fb.bytes[i];
  }
#endif // EFI_LUA_LOOKUP
}

void processCanEcuControl(const CANRxFrame& frame) {
	if (frame.data8[0] != (int)bench_test_magic_numbers_e::BENCH_HEADER) {
		return;
	}

  int eid = CAN_EID(frame);
  if (eid == (int)bench_test_packet_ids_e::ECU_SET_CALIBRATION) {
    processCanSetCalibration(frame);
  } else if (eid == (int)bench_test_packet_ids_e::ECU_REQ_CALIBRATION) {
    processCanRequestCalibration(frame);
  } else if (eid == (int)bench_test_packet_ids_e::ECU_CAN_BUS_USER_CONTROL) {
    processCanUserControl(frame);
  }
}

#endif // EFI_CAN_SUPPORT

std::optional<setup_custom_board_ts_command_override_type> custom_board_ts_command;

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
			doRunSparkBench(index, engineConfiguration->benchTestOnTime,
				engineConfiguration->benchTestOffTime, engineConfiguration->benchTestCount);
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
#if EFI_CAN_SUPPORT
	case TS_WIDEBAND:
		setWidebandOffset(0xff, index);
		break;
	case TS_WIDEBAND_SET_IDX_BY_ID:
		{
			uint8_t hwIndex = index >> 8;
			uint8_t canIndex = index & 0xff;

			// Hack until we fix canReWidebandHwIndex and set "Broadcast" to 0xff
			// TODO:
			hwIndex = hwIndex < 8 ? hwIndex : 0xff;
			setWidebandOffset(hwIndex, canIndex);
		}
		break;
	case TS_WIDEBAND_SET_SENS_BY_ID:
		{
			uint8_t hwIndex = index >> 8;
			uint8_t sensType = index & 0xff;

			// Hack until we fix canReWidebandHwIndex and set "Broadcast" to 0xff
			// TODO:
			hwIndex = hwIndex < 8 ? hwIndex : 0xff;
			setWidebandSensorType(hwIndex, sensType);
		}
		break;
	case TS_WIDEBAND_PING_BY_ID:
		pingWideband(index >> 8);
		break;

	case TS_WIDEBAND_FLASH_BY_ID:
	case TS_WIDEBAND_FLASH_BY_ID_FILE:
		{
			uint8_t hwIndex = index >> 8;

			// Hack until we fix canReWidebandHwIndex and set "Broadcast" to 0xff
			// TODO:
			widebandUpdateHwId = hwIndex < 8 ? hwIndex : 0xff;
			requestWidebandUpdate(widebandUpdateHwId, subsystem == TS_WIDEBAND_FLASH_BY_ID_FILE);
		}
		break;

	case TS_WIDEBAND_RESTART:
		restartWideband();
		break;

#endif // EFI_CAN_SUPPORT
	case TS_BENCH_CATEGORY:
		handleBenchCategory(index);
		break;

	case TS_SET_ENGINE_TYPE:
		applyPreset(index);
		break;

  case TS_BOARD_ACTION:
      // TODO: use call_board_override
	  if (custom_board_ts_command.has_value()) {
		  custom_board_ts_command.value()(subsystem, index);
	  }
		break;

	case TS_SET_DEFAULT_ENGINE:
		applyPreset((int)DEFAULT_ENGINE_TYPE);
		break;

	case TS_STOP_ENGINE:
		doScheduleStopEngine(StopRequestedReason::TsCommand);
		break;

	case 0xba:
#if EFI_PROD_CODE && EFI_DFU_JUMP
		jump_to_bootloader();
#endif /* EFI_DFU_JUMP */
		break;

	case REBOOT_COMMAND:
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
		criticalError("Unexpected bench subsystem %d %d", subsystem, index);
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

#if EFI_CAN_SUPPORT
#if EFI_WIDEBAND_FIRMWARE_UPDATE
	addConsoleActionI("update_wideband", [](int hwIndex) { requestWidebandUpdate(hwIndex, false); });
#endif // EFI_WIDEBAND_FIRMWARE_UPDATE
	addConsoleActionII("set_wideband_index", [](int hwIndex, int index) { setWidebandOffset(hwIndex, index); });
#endif // EFI_CAN_SUPPORT

	addConsoleAction(CMD_STARTER_BENCH, starterRelayBench);
	addConsoleAction(CMD_MIL_BENCH, milBench);
	addConsoleAction(CMD_HPFP_BENCH, hpfpValveBench);

#if EFI_CAN_SUPPORT
  addConsoleActionI("ping_wideband", [](int index) {
    pingWideband(index);
  });
#endif // EFI_CAN_SUPPORT

#if EFI_LUA
  // this commands facilitates TS Lua Button scripts development
  addConsoleActionI("lua_button", [](int index) {
    if (index < 0 || index > LUA_BUTTON_COUNT)
      return;
    luaCommandCounters[index - 1]++;
  });
  addConsoleActionFFFF("luabench2", [](float humanIndex, float onTime, float offTimeMs, float count) {
	  doRunBenchTestLuaOutput((int)humanIndex, onTime, offTimeMs, (int)count);
  });
#endif // EFI_LUA
	instance.start();
	onConfigurationChangeBenchTest();
}

#endif /* EFI_UNIT_TEST */
