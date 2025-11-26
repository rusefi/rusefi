/*
 * file can_bench_test.cpp
 * see also https://wiki.rusefi.com/CAN BENCH_TEST_BASE_ADDRESS 0x770000
 *
 * primary recipient is https://github.com/rusefi/rusefi-hardware/tree/main/digital-inputs/firmware
 *
 * todo: shall we not broadcast by default but wait until stim firmware wakes us up?
 */

#include "pch.h"
#include "bench_test.h"
#include "board_id.h"
#include "can_bench_test.h"
#include "can_msg_tx.h"
#include "can_common.h"
#include "frequency_sensor.h"
#include "settings.h"
#include "gpio/gpio_ext.h"

#ifdef HW_HELLEN
#include "hellen_meta.h"
#endif // HW_HELLEN

extern PinRepository pinRepository;

// todo: WHAT?! document why do we manually truncate higher bits?
#define TRUNCATE_TO_BYTE(i) ((i) & 0xff)
// raw values are 0..5V, convert it to 8-bit (0..255)
#define RAW_TO_BYTE(v) TRUNCATE_TO_BYTE((int)(v * 255.0 / 5.0))

#if EFI_PROD_CODE
/**
 * QC direct output control API is used by https://github.com/rusefi/stim test device
 * quite different from bench testing user functionality: QC direct should never be engaged on a real vehicle
 * Once QC direct control mode is activated the only way out is to reboot the unit!
 */
static bool qcDirectPinControlMode = false;
#endif

/*board public API*/bool isHwQcMode() {
#if EFI_PROD_CODE
  return qcDirectPinControlMode;
#else
  return false;
#endif // EFI_PROD_CODE
}

void setHwQcMode() {
#if EFI_PROD_CODE
  qcDirectPinControlMode = true;
#if HW_HELLEN
        if (!getHellenBoardEnabled()) {
            hellenEnableEn("HW QC");
        }
#endif // HW_HELLEN
#endif // EFI_PROD_CODE
}

#if EFI_CAN_SUPPORT

static void directWritePad(Gpio pin, int value, const char *msg = "") {
  if (!isBrainPinValid(pin)) {
    criticalError("QC of invalid pin %d %s", (int)pin, msg);
    return;
  }

#if EFI_GPIO_HARDWARE && EFI_PROD_CODE
	if (brain_pin_is_onchip(pin)) {
	  palWritePad(getHwPort("can_write", pin), getHwPin("can_write", pin), value);
	} else {
#if (BOARD_EXT_GPIOCHIPS > 0)
  	gpiochips_writePad(pin, value);
#endif
	}
#endif // EFI_GPIO_HARDWARE && EFI_PROD_CODE
}

static void qcSetEtbState(uint8_t dcIndex, uint8_t direction) {
	setHwQcMode();
	const dc_io *io = &engineConfiguration->etbIo[dcIndex];
	Gpio controlPin = io->controlPin;
  directWritePad(controlPin, 1, "DC control");
	if (engineConfiguration->etb_use_two_wires) {
	  // TLE7209 and L6205
	  // let's force proper pin mode to work around potentially uninitialized subsystem
	  efiSetPadModeWithoutOwnershipAcquisition("QC_ETB_1", io->directionPin1, PAL_MODE_OUTPUT_PUSHPULL);
	  efiSetPadModeWithoutOwnershipAcquisition("QC_ETB_2", io->directionPin2, PAL_MODE_OUTPUT_PUSHPULL);

	  directWritePad(io->directionPin1, direction, "DC dir1");
	  directWritePad(io->directionPin2, !direction, "DC dir2");
	} else {
	  // TLE9201 and VNH2SP30
	  efiSetPadModeWithoutOwnershipAcquisition("QC_ETB", controlPin, PAL_MODE_OUTPUT_PUSHPULL);
	  directWritePad(io->directionPin1, direction, "DC dir");
	  directWritePad(io->disablePin, 0, "DC dis"); // disable pin is inverted - here we ENABLE. direct pin access due to qcDirectPinControlMode
  }
}

static void setPin(const CANRxFrame& frame, int value) {
		size_t outputIndex = frame.data8[2];
		if (outputIndex >= getBoardMetaOutputsCount()) {
		  criticalError("QC pin index %d out of range", outputIndex);
			return;
	  }
#if EFI_GPIO_HARDWARE && EFI_PROD_CODE
	  Gpio* boardOutputs = getBoardMetaOutputs();
	  criticalAssertVoid(boardOutputs != nullptr, "outputs not defined");
		Gpio pin = boardOutputs[outputIndex];

        int hwIndex = brainPin_to_index(pin);
        if (pinRepository.getBrainUsedPin(hwIndex) == nullptr) {
            // if pin is assigned we better configure it
            efiSetPadModeWithoutOwnershipAcquisition("QC_SET", pin, PAL_MODE_OUTPUT_PUSHPULL);
        }

        directWritePad(pin, value);
#endif // EFI_GPIO_HARDWARE && EFI_PROD_CODE
}

void sendQcBenchEventCounters(size_t bus) {
#if EFI_SHAFT_POSITION_INPUT
	CanTxMessage msg(CanCategory::BENCH_TEST, (int)bench_test_packet_ids_e::EVENT_COUNTERS, 8, bus, /*isExtended*/true);

	int primaryFall = engine->triggerCentral.getHwEventCounter((int)SHAFT_PRIMARY_FALLING);
	int primaryRise = engine->triggerCentral.getHwEventCounter((int)SHAFT_PRIMARY_RISING);
	int secondaryFall = engine->triggerCentral.getHwEventCounter((int)SHAFT_SECONDARY_FALLING);
	int secondaryRise = engine->triggerCentral.getHwEventCounter((int)SHAFT_SECONDARY_RISING);

	msg[0] = TRUNCATE_TO_BYTE(primaryRise + primaryFall);
	msg[1] = TRUNCATE_TO_BYTE(secondaryRise + secondaryFall);

	for (int camIdx = 0; camIdx < 4; camIdx++) {
		int vvtRise = 0, vvtFall = 0;
		if (camIdx < CAM_INPUTS_COUNT) {
			vvtRise = engine->triggerCentral.vvtEventRiseCounter[camIdx];
			vvtFall = engine->triggerCentral.vvtEventFallCounter[camIdx];
		}

		msg[2 + camIdx] = TRUNCATE_TO_BYTE(vvtRise + vvtFall);
	}

	extern FrequencySensor vehicleSpeedSensor;
	msg[6] = TRUNCATE_TO_BYTE(vehicleSpeedSensor.eventCounter);
#endif // EFI_SHAFT_POSITION_INPUT
}

void sendQcBenchButtonCounters() {
	CanTxMessage msg(CanCategory::BENCH_TEST, (int)bench_test_packet_ids_e::BUTTON_COUNTERS, 8, /*bus*/0, /*isExtended*/true);
	msg[0] = TRUNCATE_TO_BYTE(engine->brakePedalSwitchedState.getCounter());
	msg[1] = TRUNCATE_TO_BYTE(engine->clutchUpSwitchedState.getCounter());
	msg[2] = TRUNCATE_TO_BYTE(engine->acButtonSwitchedState.getCounter());
	// todo: start button
}

void sendQcBenchAuxDigitalCounters() {
	CanTxMessage msg(CanCategory::BENCH_TEST, (int)bench_test_packet_ids_e::AUX_DIGITAL_COUNTERS, 8, /*bus*/0, /*isExtended*/true);
  for (int i =0;i<LUA_DIGITAL_INPUT_COUNT;i++) {
	  msg[i] = TRUNCATE_TO_BYTE(engine->luaDigitalInputState[i].state.getCounter());
  }
}

void sendQcBenchRawAnalogValues(size_t bus) {
	const float values_1[] = {
		Sensor::getRaw(SensorType::Tps1Primary),
		Sensor::getRaw(SensorType::Tps1Secondary),
		Sensor::getRaw(SensorType::AcceleratorPedalPrimary),
		Sensor::getRaw(SensorType::AcceleratorPedalSecondary),
		Sensor::getRaw(SensorType::MapSlow),
		Sensor::getRaw(SensorType::Clt),
		Sensor::getRaw(SensorType::Iat),
		Sensor::getRaw(SensorType::BatteryVoltage),
	};

	const float values_2[] = {
		Sensor::getRaw(SensorType::Tps2Primary),
		Sensor::getRaw(SensorType::Tps2Secondary),
		Sensor::getRaw(SensorType::AuxLinear1),
		Sensor::getRaw(SensorType::AuxLinear2),
		Sensor::getRaw(SensorType::OilPressure),
		Sensor::getRaw(SensorType::FuelPressureLow),
		Sensor::getRaw(SensorType::FuelPressureHigh),
		Sensor::getRaw(SensorType::AuxTemp1),
	};
	const float lua_values_1[] = {
		Sensor::getRaw(SensorType::AuxAnalog1),
		Sensor::getRaw(SensorType::AuxAnalog2),
		Sensor::getRaw(SensorType::AuxAnalog3),
		Sensor::getRaw(SensorType::AuxAnalog4),
		Sensor::getRaw(SensorType::AuxAnalog5),
		Sensor::getRaw(SensorType::AuxAnalog6),
		Sensor::getRaw(SensorType::AuxAnalog7),
		Sensor::getRaw(SensorType::AuxAnalog8),
	};
	static_assert(efi::size(values_1) <= 8);
	static_assert(efi::size(values_2) <= 8);
	static_assert(efi::size(lua_values_1) <= 8);


	// send the first packet
	{
		CanTxMessage msg(CanCategory::BENCH_TEST, (int)bench_test_packet_ids_e::RAW_ANALOG_1, 8, bus, /*isExtended*/true);
		for (size_t valueIdx = 0; valueIdx < efi::size(values_1); valueIdx++) {
			msg[valueIdx] = RAW_TO_BYTE(values_1[valueIdx]);
		}
	}
	{
		CanTxMessage msg(CanCategory::BENCH_TEST, (int)bench_test_packet_ids_e::RAW_ANALOG_2, 8, bus, /*isExtended*/true);
		for (size_t valueIdx = 0; valueIdx < efi::size(values_2); valueIdx++) {
			msg[valueIdx] = RAW_TO_BYTE(values_2[valueIdx]);
		}
	}
	// todo: time to extract method already?
	{
		CanTxMessage msg(CanCategory::BENCH_TEST, (int)bench_test_packet_ids_e::RAW_LUA_ANALOG_1, 8, bus, /*isExtended*/true);
		for (size_t valueIdx = 0; valueIdx < efi::size(lua_values_1); valueIdx++) {
			msg[valueIdx] = RAW_TO_BYTE(lua_values_1[valueIdx]);
		}
	}
}

static void sendOutBoardMeta(size_t bus) {
#if EFI_PROD_CODE
	CanTxMessage msg(CanCategory::BENCH_TEST, (int)bench_test_packet_ids_e::IO_META_INFO, 8, bus, /*isExtended*/true);
	msg[0] = (int)bench_test_magic_numbers_e::BENCH_HEADER;
	msg[1] = 0;
	msg[2] = getBoardMetaOutputsCount();
	msg[3] = getBoardMetaLowSideOutputsCount();
	msg[4] = getBoardMetaDcOutputsCount();
#endif // EFI_PROD_CODE
}

void sendQcBenchBoardStatus(size_t bus) {
#if EFI_PROD_CODE
	CanTxMessage msg(CanCategory::BENCH_TEST, (int)bench_test_packet_ids_e::BOARD_STATUS, 8, bus, /*isExtended*/true);

	int boardId = getBoardId();
	msg[0] = TRUNCATE_TO_BYTE(boardId >> 8);
	msg[1] = TRUNCATE_TO_BYTE(boardId);

	int numSecondsSinceReset = getTimeNowS();
	msg[2] = TRUNCATE_TO_BYTE(numSecondsSinceReset >> 16);
	msg[3] = TRUNCATE_TO_BYTE(numSecondsSinceReset >> 8);
	msg[4] = TRUNCATE_TO_BYTE(numSecondsSinceReset);

    int engineType = (int) engineConfiguration->engineType;
	msg[5] = engineType >> 8;
	msg[6] = engineType;
	sendOutBoardMeta(bus);
#endif // EFI_PROD_CODE
}

#if EFI_PROD_CODE
static void sendManualPinTest(int id) {
	CanTxMessage msg(CanCategory::BENCH_TEST, (int)bench_test_packet_ids_e::MANUAL_PIN_TEST, 8, /*bus*/0, /*isExtended*/true);
	msg[0] = id;
}
#endif // EFI_PROD_CODE

static void sendPinStatePackets(int pinToggleCounter, uint32_t durationsInStateMs[2]) {
	CanTxMessage msg(CanCategory::BENCH_TEST, (int)bench_test_packet_ids_e::PIN_STATE, 8, /*bus*/0, /*isExtended*/true);
	msg[0] = TRUNCATE_TO_BYTE(pinToggleCounter >> 8);
	msg[1] = TRUNCATE_TO_BYTE(pinToggleCounter);

	for (int i = 0, mIdx = 2; i < 2; i++) {
		msg[mIdx++] = TRUNCATE_TO_BYTE(durationsInStateMs[i] >> 16);
		msg[mIdx++] = TRUNCATE_TO_BYTE(durationsInStateMs[i] >> 8);
		msg[mIdx++] = TRUNCATE_TO_BYTE(durationsInStateMs[i]);
	}
}

// bench test fuel pump pin #5603
static void sendPinStatePackets(bench_mode_e benchModePinIdx) {
    OutputPin *pin = enginePins.getOutputPinForBenchMode(benchModePinIdx);
    if (pin == nullptr)
    	return;
#if EFI_SIMULATOR
	sendPinStatePackets(pin->pinToggleCounter, pin->durationsInStateMs);
#endif // EFI_SIMULATOR
}

static void sendSavedBenchStatePackets() {
	uint32_t savedDurationsInStateMs[2];
	int savedPinToggleCounter = getSavedBenchTestPinStates(savedDurationsInStateMs);
	sendPinStatePackets(savedPinToggleCounter, savedDurationsInStateMs);
}

static void resetPinStats(bench_mode_e benchModePinIdx) {
    OutputPin *pin = enginePins.getOutputPinForBenchMode(benchModePinIdx);

    if (pin == nullptr)
    	return;

#if EFI_SIMULATOR
	pin->resetToggleStats();
#endif // EFI_SIMULATOR
}

void processCanQcBenchTest(const CANRxFrame& frame) {
	if (CAN_EID(frame) != (int)bench_test_packet_ids_e::HW_QC_IO_CONTROL) {
		return;
	}
	if (frame.data8[0] != (int)bench_test_magic_numbers_e::BENCH_HEADER) {
		return;
	}
  setHwQcMode();
	bench_test_io_control_e command = (bench_test_io_control_e)frame.data8[1];
	if (command == bench_test_io_control_e::CAN_BENCH_GET_COUNT) {
	    sendOutBoardMeta(0);
	} else if (command == bench_test_io_control_e::CAN_QC_OUTPUT_CONTROL_SET) {
	  // see also "bench_setpin" console command
	    setPin(frame, 1);
	} else if (command == bench_test_io_control_e::CAN_QC_OUTPUT_CONTROL_CLEAR) {
	    setPin(frame, 0);
	} else if (command == bench_test_io_control_e::CAN_QC_ETB) {
		uint8_t dcIndex = frame.data8[2];
		uint8_t direction = frame.data8[3];
		qcSetEtbState(dcIndex, direction);
	} else if (command == bench_test_io_control_e::CAN_BENCH_SET_ENGINE_TYPE) {
		int eType = frame.data8[2];
		// todo: fix firmware for 'false' to be possible - i.e. more of properties should be applied on the fly
		setEngineType(eType, true);
#if EFI_PROD_CODE
		scheduleReboot();
#endif // EFI_PROD_CODE
} else if (command == bench_test_io_control_e::CAN_BENCH_START_PIN_TEST) {
		bench_mode_e benchModePinIdx = (bench_mode_e)frame.data8[2];
		// ignore previous pin state and stats
		resetPinStats(benchModePinIdx);
	} else if (command == bench_test_io_control_e::CAN_BENCH_END_PIN_TEST) {
		sendSavedBenchStatePackets();
	} else if (command == bench_test_io_control_e::CAN_BENCH_EXECUTE_BENCH_TEST) {
		int benchCommandIdx = frame.data8[2];
		handleBenchCategory(benchCommandIdx);
	} else if (command == bench_test_io_control_e::CAN_BENCH_QUERY_PIN_STATE) {
		bench_mode_e benchModePinIdx = (bench_mode_e)frame.data8[2];
		sendPinStatePackets(benchModePinIdx);
	}
}
#endif // EFI_CAN_SUPPORT

void initQcBenchControls() {
#if EFI_CAN_SUPPORT && EFI_PROD_CODE
    addConsoleActionII("qc_etb", [](int index, int direction) {
        qcSetEtbState(index, direction);
    });

    addConsoleActionI("qc_output", [](int index) {
	    Gpio* boardOutputs = getBoardMetaOutputs();
	    criticalAssertVoid(boardOutputs != nullptr, "outputs not defined");
      Gpio pin = boardOutputs[index];
	    efiSetPadModeWithoutOwnershipAcquisition("qc_output", pin, PAL_MODE_OUTPUT_PUSHPULL);

	    int physicalValue = palReadPad(getHwPort("read", pin), getHwPin("read", pin));
	    efiPrintf("original pin %s value %d", hwPortname(pin), physicalValue);
	    palWritePad(getHwPort("write", pin), getHwPin("write", pin), 1 - physicalValue);
      sendManualPinTest(index);
    });
#endif // EFI_PROD_CODE
}
