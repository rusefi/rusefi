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

#if EFI_PROD_CODE

extern const CANConfig *findCanConfig(can_baudrate_e rate);

#else // not EFI_PROD_CODE
// Nothing to actually set for the simulator's CAN config.
// It's impossible to set CAN bitrate from userspace, so we can't set it.
static const CANConfig canConfig_dummy;

static const CANConfig * findCanConfig(can_baudrate_e rate)
{
	return &canConfig_dummy;
}

#endif

class CanRead final : protected ThreadController<UTILITY_THREAD_STACK_SIZE> {
public:
	CanRead(size_t index)
		: ThreadController("CAN RX", PRIO_CAN_RX)
		, m_index(index)
	{
	}

	void start(CANDriver* device) {
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
	const size_t m_index;
	CANRxFrame m_buffer;
	CANDriver* m_device;
};

CCM_OPTIONAL static CanRead canRead1(0);
CCM_OPTIONAL static CanRead canRead2(1);
#if (EFI_CAN_BUS_COUNT >= 3)
CCM_OPTIONAL static CanRead canRead3(2);
#endif
static CanWrite canWrite CCM_OPTIONAL;

#if EFI_PROD_CODE
static CANDriver* getCanDevice(size_t index)
{
	switch (index) {
	case 0:
		return detectCanDevice(engineConfiguration->canRxPin, engineConfiguration->canTxPin);
	case 1:
		return detectCanDevice(engineConfiguration->can2RxPin, engineConfiguration->can2TxPin);
	case 2:
		return detectCanDevice(engineConfiguration->can3RxPin, engineConfiguration->can3TxPin);
	}

	return nullptr;
}
#endif

static void canInfo() {
	if (!isCanEnabled) {
		efiPrintf("CAN is not enabled, please enable & restart");
		return;
	}

	efiPrintf("CAN1 TX %s %s", hwPortname(engineConfiguration->canTxPin), getCan_baudrate_e(engineConfiguration->canBaudRate));
	efiPrintf("CAN1 RX %s", hwPortname(engineConfiguration->canRxPin));
	canHwInfo(getCanDevice(0));

	efiPrintf("CAN2 TX %s %s", hwPortname(engineConfiguration->can2TxPin), getCan_baudrate_e(engineConfiguration->can2BaudRate));
	efiPrintf("CAN2 RX %s", hwPortname(engineConfiguration->can2RxPin));
	canHwInfo(getCanDevice(1));

#if (EFI_CAN_BUS_COUNT >= 3)
	efiPrintf("CAN3 TX %s %s", hwPortname(engineConfiguration->can3TxPin), getCan_baudrate_e(engineConfiguration->can3BaudRate));
	efiPrintf("CAN3 RX %s", hwPortname(engineConfiguration->can3RxPin));
	canHwInfo(getCanDevice(2));
#endif

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

void stopCanPins() {
	efiSetPadUnusedIfConfigurationChanged(canTxPin);
	efiSetPadUnusedIfConfigurationChanged(canRxPin);
	efiSetPadUnusedIfConfigurationChanged(can2TxPin);
	efiSetPadUnusedIfConfigurationChanged(can2RxPin);
#if (EFI_CAN_BUS_COUNT >= 3)
	efiSetPadUnusedIfConfigurationChanged(can3TxPin);
	efiSetPadUnusedIfConfigurationChanged(can3RxPin);
#endif
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

#if (EFI_CAN_BUS_COUNT >= 3)
	efiSetPadModeIfConfigurationChanged("CAN3 TX", can3TxPin, PAL_MODE_ALTERNATE(EFI_CAN3_TX_AF));
	efiSetPadModeIfConfigurationChanged("CAN3 RX", can3RxPin, PAL_MODE_ALTERNATE(EFI_CAN3_RX_AF));
#endif // EFI_CAN_BUS_COUNT >= 3
#endif // EFI_PROD_CODE
}

// Move to port CAN helpers file
static void applyListenOnly(CANConfig* canConfig, bool isListenOnly) {
#if defined(STM32F4XX) || defined(STM32F7XX)
	if (isListenOnly) {
		canConfig->btr |= CAN_BTR_SILM;
	}
#elif defined(STM32H7XX)
	// TODO: move to ChibiOS stm32_fdcan.h
	#define FDCAN_CONFIG_CCCR_MON	(1u << 5)
	if (isListenOnly) {
		canConfig->CCCR |= FDCAN_CONFIG_CCCR_MON;
	}
#endif
}

void initCan() {
	addConsoleAction("caninfo", canInfo);

	isCanEnabled = false;

	// No CAN features enabled, nothing more to do.
	if (!engineConfiguration->canWriteEnabled && !engineConfiguration->canReadEnabled) {
		return;
	}

	// Determine physical CAN peripherals based on selected pins
	auto device1 = getCanDevice(0);
	auto device2 = getCanDevice(1);
#if (EFI_CAN_BUS_COUNT >= 3)
	auto device3 = getCanDevice(2);
#endif

	// If all devices are null, a firmware error was already thrown by detectCanDevice, but we shouldn't continue
	if (!device1 && !device2) {
#if (EFI_CAN_BUS_COUNT >= 3)
		if (!device3)
#endif
				return;
	}

	// Devices can't be the same!
	if (((device1 == device2) && device1) ||
#if (EFI_CAN_BUS_COUNT >= 3)
		((device2 == device3) && device2) ||
		((device3 == device1) && device3) ||
#endif
		0) {
		criticalError("CAN pins must be set to different devices");
		return;
	}

	// Initialize peripherals
	if (device1) {
	    // Config based on baud rate
	    // Pointer to this local canConfig is stored inside CANDriver
	    // even it is used only during canStart this is wierd
	    CANConfig canConfig;
	    memcpy(&canConfig, findCanConfig(engineConfiguration->canBaudRate), sizeof(canConfig));
	    applyListenOnly(&canConfig, engineConfiguration->can1ListenMode);
		canStart(device1, &canConfig);

		// Plumb CAN devices to tx system
		CanTxMessage::setDevice(0, device1);
	}

	if (device2) {
	    CANConfig canConfig;
	    memcpy(&canConfig, findCanConfig(engineConfiguration->can2BaudRate), sizeof(canConfig));
	    applyListenOnly(&canConfig, engineConfiguration->can2ListenMode);
		canStart(device2, &canConfig);

		// Plumb CAN devices to tx system
		CanTxMessage::setDevice(1, device2);
	}

#if (EFI_CAN_BUS_COUNT >= 3)
	if (device3) {
	    CANConfig canConfig;
	    memcpy(&canConfig, findCanConfig(engineConfiguration->can3BaudRate), sizeof(canConfig));
	    applyListenOnly(&canConfig, engineConfiguration->can3ListenMode);
		canStart(device3, &canConfig);

		// Plumb CAN devices to tx system
		CanTxMessage::setDevice(2, device3);
	}
#endif

	// fire up threads, as necessary
	if (engineConfiguration->canWriteEnabled) {
		canWrite.start();
	}

	if (engineConfiguration->canReadEnabled) {
		canRead1.start(device1);
		canRead2.start(device2);
#if (EFI_CAN_BUS_COUNT >= 3)
		canRead3.start(device3);
#endif
	}

	isCanEnabled = true;
}

bool getIsCanEnabled(void) {
	return isCanEnabled;
}

#endif /* EFI_CAN_SUPPORT */
