
#include "pch.h"
#include "bench_test.h"
#include "board_id.h"
#include "can_bench_test.h"
#include "can_msg_tx.h"
#include "can_common.h"
#include "frequency_sensor.h"
#include "settings.h"

#define TRUNCATE_TO_BYTE(i) ((i) & 0xff)
// raw values are 0..5V, convert it to 8-bit (0..255)
#define RAW_TO_BYTE(v) TRUNCATE_TO_BYTE((int)(v * 255.0 / 5.0))

bool qcDirectPinControlMode = false;

#if EFI_CAN_SUPPORT

static void setPin(const CANRxFrame& frame, int value) {
		int outputIndex = frame.data8[2];
		if (outputIndex >= getBoardMetaOutputsCount())
			return;
		Gpio pin = getBoardMetaOutputs()[outputIndex];
#if EFI_GPIO_HARDWARE && EFI_PROD_CODE

        int hwIndex = brainPin_to_index(pin);
        if (engine->pinRepository.getBrainUsedPin(hwIndex) == nullptr) {
            // if pin is assigned we better configure it
            efiSetPadModeWithoutOwnershipAcquisition("QC_SET", pin, PAL_MODE_OUTPUT_PUSHPULL);
        }

		palWritePad(getHwPort("can_write", pin), getHwPin("can_write", pin), value);
		// todo: add smart chip support support
#endif // EFI_GPIO_HARDWARE && EFI_PROD_CODE
}

void sendEventCounters() {
#if EFI_SHAFT_POSITION_INPUT
	CanTxMessage msg(CanCategory::BENCH_TEST, (int)bench_test_packet_ids_e::EVENT_COUNTERS, 8, /*bus*/0, /*isExtended*/true);

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

void sendButtonCounters() {
	CanTxMessage msg(CanCategory::BENCH_TEST, (int)bench_test_packet_ids_e::BUTTON_COUNTERS, 8, /*bus*/0, /*isExtended*/true);
	msg[0] = TRUNCATE_TO_BYTE(engine->brakePedalSwitchedState.getCounter());
	msg[1] = TRUNCATE_TO_BYTE(engine->clutchUpSwitchedState.getCounter());
	msg[2] = TRUNCATE_TO_BYTE(engine->acButtonSwitchedState.getCounter());
	// todo: start button
}

void sendRawAnalogValues() {
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
	static_assert(efi::size(values_1) <= 8);
	static_assert(efi::size(values_2) <= 8);


	// send the first packet
	{
		CanTxMessage msg(CanCategory::BENCH_TEST, (int)bench_test_packet_ids_e::RAW_ANALOG_1, 8, /*bus*/0, /*isExtended*/true);
		for (int valueIdx = 0; valueIdx < efi::size(values_1); valueIdx++) {
			msg[valueIdx] = RAW_TO_BYTE(values_1[valueIdx]);
		}
	}
	{
		CanTxMessage msg(CanCategory::BENCH_TEST, (int)bench_test_packet_ids_e::RAW_ANALOG_2, 8, /*bus*/0, /*isExtended*/true);
		for (int valueIdx = 0; valueIdx < efi::size(values_2); valueIdx++) {
			msg[valueIdx] = RAW_TO_BYTE(values_2[valueIdx]);
		}
	}
}

static void sendOutBoardMeta() {
#if EFI_PROD_CODE
	CanTxMessage msg(CanCategory::BENCH_TEST, (int)bench_test_packet_ids_e::IO_META_INFO, 8, /*bus*/0, /*isExtended*/true);
	msg[0] = CAN_BENCH_HEADER;
	msg[1] = 0;
	msg[2] = getBoardMetaOutputsCount();
	msg[3] = getBoardMetaLowSideOutputsCount();
#endif // EFI_PROD_CODE
}

void sendBoardStatus() {
#if EFI_PROD_CODE
	CanTxMessage msg(CanCategory::BENCH_TEST, (int)bench_test_packet_ids_e::BOARD_STATUS, 8, /*bus*/0, /*isExtended*/true);

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
	sendOutBoardMeta();
#endif // EFI_PROD_CODE
}

#if 0
// bench test fuel pump pin #5603
static void sendPinStatePackets(int benchCommandIdx)

    OutputPin *pin = get pin by benchCommandIdx, at least for fuel pump and VVT
    // packet one ID XXX
    pack two bytes pinToggleCounter

    // packet two ID XXX+1
     four byes durationsInStateMs[0]
     four byes durationsInStateMs[1]

#endif

void processCanBenchTest(const CANRxFrame& frame) {
	if (CAN_EID(frame) != (int)bench_test_packet_ids_e::IO_CONTROL) {
		return;
	}
	if (frame.data8[0] != CAN_BENCH_HEADER) {
		return;
	}
	qcDirectPinControlMode = true;
	uint8_t command = frame.data8[1];
	if (command == (uint8_t)bench_test_io_control_e::CAN_BENCH_GET_COUNT) {
	    sendOutBoardMeta();
	} else if (command == (uint8_t)bench_test_io_control_e::CAN_BENCH_GET_SET) {
	    setPin(frame, 1);
	} else if (command == (uint8_t)bench_test_io_control_e::CAN_BENCH_GET_CLEAR) {
	    setPin(frame, 0);
	} else if (command == (uint8_t)bench_test_io_control_e::CAN_BENCH_SET_ENGINE_TYPE) {
		int eType = frame.data8[2];
		// todo: fix firmware for 'false' to be possible - i.e. more of properties should be applied on the fly
		setEngineType(eType, true);
#if EFI_PROD_CODE
		scheduleReboot();
#endif // EFI_PROD_CODE
	} else if (command == (uint8_t)bench_test_io_control_e::CAN_BENCH_EXECUTE_BENCH_TEST) {
		int benchCommandIdx = frame.data8[2];
		handleBenchCategory(benchCommandIdx);
// else if (command == (uint8_t)bench_test_io_control_e::CAN_TEST_QUERY_PIN_STATE) {
//      sendPinStatePackets(benchCommandIdx);
	}
}

#endif // EFI_CAN_SUPPORT
