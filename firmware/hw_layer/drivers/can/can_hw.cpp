/**
 * @file	can_hw.cpp
 * @brief	CAN bus low level code
 *
 * todo: this file should be split into two - one for CAN transport level ONLY and
 * another one with actual messages
 *
 * @see can_verbose.cpp for higher level logic
 * @see obd2.cpp for OBD-II messages via CAN
 *
 * @date Dec 11, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if EFI_CAN_SUPPORT

#include "can.h"
#include "can_hw.h"
#include "can_msg_tx.h"
#include "string.h"
#include "mpu_util.h"

static bool isCanEnabled = false;

class CanRead final : protected ThreadController<UTILITY_THREAD_STACK_SIZE> {
public:
	CanRead(CanBusIndex index)
		: ThreadController("CAN RX", PRIO_CAN_RX)
		, m_index(index)
	{
	}

	void tryStart(CANDriver *device) {
		m_device = device;

		if (device) {
			ThreadController::start();
		}
	}

	void ThreadTask() override {
		while (true) {
			// Block until we get a message
			msg_t result = canReceiveTimeout(m_device, CAN_ANY_MAILBOX, &m_buffer, TIME_INFINITE);

			if (result != MSG_OK) {
				continue;
			}

			// Process the message
			engine->outputChannels.canReadCounter++;

			processCanRxMessage(m_index, m_buffer, getTimeNowNt());
		}
	}

private:
	const CanBusIndex m_index;
	CANRxFrame m_buffer;
	CANDriver* m_device;
};

CCM_OPTIONAL static CanRead canRead1(CanBusIndex::Bus0);
CCM_OPTIONAL static CanRead canRead2(CanBusIndex::Bus1);
static CanWrite canWrite CCM_OPTIONAL;

static void canInfo() {
	if (!isCanEnabled) {
		efiPrintf("CAN is not enabled, please enable & restart");
		return;
	}

	efiPrintf("CAN1 TX %s %s", hwPortname(engineConfiguration->canTxPin), getCan_baudrate_e(engineConfiguration->canBaudRate));
	efiPrintf("CAN1 RX %s", hwPortname(engineConfiguration->canRxPin));

	efiPrintf("CAN2 TX %s %s", hwPortname(engineConfiguration->can2TxPin), getCan_baudrate_e(engineConfiguration->can2BaudRate));
	efiPrintf("CAN2 RX %s", hwPortname(engineConfiguration->can2RxPin));

	efiPrintf("type=%d canReadEnabled=%s canWriteEnabled=%s period=%d", engineConfiguration->canNbcType,
			boolToString(engineConfiguration->canReadEnabled), boolToString(engineConfiguration->canWriteEnabled),
			engineConfiguration->canSleepPeriodMs);

	efiPrintf("CAN rx_cnt=%d/tx_ok=%d/tx_not_ok=%d",
			engine->outputChannels.canReadCounter,
			engine->outputChannels.canWriteOk,
			engine->outputChannels.canWriteNotOk);
}

void setCanType(int type) {
	engineConfiguration->canNbcType = (can_nbc_e)type;
	canInfo();
}

#if EFI_TUNER_STUDIO
void postCanState() {
	if (!isCanEnabled) {
		engine->outputChannels.canReadCounter = -1;
		engine->outputChannels.canWriteOk = -1;
		engine->outputChannels.canWriteNotOk = -1;
	}
}
#endif /* EFI_TUNER_STUDIO */

void stopCanPins() {
	efiSetPadUnusedIfConfigurationChanged(canTxPin);
	efiSetPadUnusedIfConfigurationChanged(canRxPin);
	efiSetPadUnusedIfConfigurationChanged(can2TxPin);
	efiSetPadUnusedIfConfigurationChanged(can2RxPin);
}

// at the moment we support only very limited runtime configuration change, still not supporting online CAN toggle
void startCanPins() {
	// nothing to do if we aren't enabled...
	if (!isCanEnabled) {
		return;
	}

	// Validate pins
	if (!isValidCanTxPin(engineConfiguration->canTxPin)) {
		if (!isBrainPinValid(engineConfiguration->canTxPin)) {
			// todo: smarter online change of settings, kill isCanEnabled with fire
			return;
		}
		firmwareError(ObdCode::CUSTOM_OBD_70, "invalid CAN TX %s", hwPortname(engineConfiguration->canTxPin));
		return;
	}

	if (!isValidCanRxPin(engineConfiguration->canRxPin)) {
		if (!isBrainPinValid(engineConfiguration->canRxPin)) {
			// todo: smarter online change of settings, kill isCanEnabled with fire
			return;
		}
		firmwareError(ObdCode::CUSTOM_OBD_70, "invalid CAN RX %s", hwPortname(engineConfiguration->canRxPin));
		return;
	}

#if EFI_PROD_CODE
	efiSetPadModeIfConfigurationChanged("CAN TX", canTxPin, PAL_MODE_ALTERNATE(EFI_CAN_TX_AF));
	efiSetPadModeIfConfigurationChanged("CAN RX", canRxPin, PAL_MODE_ALTERNATE(EFI_CAN_RX_AF));

	efiSetPadModeIfConfigurationChanged("CAN2 TX", can2TxPin, PAL_MODE_ALTERNATE(EFI_CAN_TX_AF));
	efiSetPadModeIfConfigurationChanged("CAN2 RX", can2RxPin, PAL_MODE_ALTERNATE(EFI_CAN_RX_AF));
#endif // EFI_PROD_CODE
}

void initCan() {
	addConsoleAction("caninfo", canInfo);

	isCanEnabled = false;

	// No CAN features enabled, nothing more to do.
	if (!engineConfiguration->canWriteEnabled && !engineConfiguration->canReadEnabled) {
		return;
	}

	// Determine physical CAN peripherals based on selected pins
	auto device1 = detectCanDevice(engineConfiguration->canRxPin, engineConfiguration->canTxPin);
	auto device2 = detectCanDevice(engineConfiguration->can2RxPin, engineConfiguration->can2TxPin);

	// If both devices are null, a firmware error was already thrown by detectCanDevice, but we shouldn't continue
	if (!device1 && !device2) {
		return;
	}

	// Devices can't be the same!
	if (device1 == device2) {
		firmwareError(ObdCode::OBD_PCM_Processor_Fault, "CAN pins must be set to different devices");
		return;
	}

	// Generate configs based on baud rate
	auto config1 = findCanConfig(engineConfiguration->canBaudRate);
	auto config2 = findCanConfig(engineConfiguration->can2BaudRate);

	// Initialize peripherals
	if (device1) {
		canStart(device1, config1);
	}

	if (device2) {
		canStart(device2, config2);
	}

	// Plumb CAN devices to tx system
	CanTxMessage::setDevice(device1, device2);

	// fire up threads, as necessary
	if (engineConfiguration->canWriteEnabled) {
		canWrite.start();
	}

	if (engineConfiguration->canReadEnabled) {
		canRead1.tryStart(device1);
		canRead2.tryStart(device2);
	}

	isCanEnabled = true;
}

bool getIsCanEnabled(void) {
	return isCanEnabled;
}

#endif /* EFI_CAN_SUPPORT */
