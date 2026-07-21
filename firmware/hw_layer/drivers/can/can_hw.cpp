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

static const CANConfig * findCanConfig(can_baudrate_e /*rate*/)
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

	void setDevice(CANDriver* device) {
		m_device = device;
	}

	void start(void) {
		if (m_device) {
			ThreadController::start();
		}
	}

	using ThreadController::stop;

	void ThreadTask() override {
		while (!chThdShouldTerminateX()) {
			// Block until we get a message
			msg_t result = canReceiveTimeout(m_device, CAN_ANY_MAILBOX, &m_buffer, CAN_RX_TIMEOUT);

			if (result != MSG_OK) {
				canHwRecover(m_index, m_device);
				continue;
			}

			// Process the message
			engine->outputChannels.canReadCounter++;

			processCanRxMessage(m_index, m_buffer, getTimeNowNt());
		}

		chThdExit((msg_t)0x0);
	}

private:
	const size_t m_index;
	CANRxFrame m_buffer;
	CANDriver* m_device;
};

CCM_OPTIONAL static CanRead canRead[EFI_CAN_BUS_COUNT] = { CanRead(0), CanRead(1)
#if (EFI_CAN_BUS_COUNT >= 3)
	, CanRead(2)
#endif
	};
static CanWrite canWrite CCM_OPTIONAL;

#if EFI_PROD_CODE
static CANDriver* getCanDevice(size_t index)
{
	switch (index) {
	case 0:
		return detectCanDevice(engineConfiguration->canRxPin, engineConfiguration->canTxPin);
	case 1:
		return detectCanDevice(engineConfiguration->can2RxPin, engineConfiguration->can2TxPin);
#if (EFI_CAN_BUS_COUNT >= 3)
	case 2:
		return detectCanDevice(engineConfiguration->can3RxPin, engineConfiguration->can3TxPin);
#endif
	}

	return nullptr;
}
#endif // EFI_PROD_CODE

static can_baudrate_e getDefaultCanBaudRate(size_t index) {
	switch (index) {
	case 0:
		return engineConfiguration->canBaudRate;
	case 1:
		return engineConfiguration->can2BaudRate;
#if (EFI_CAN_BUS_COUNT >= 3)
	case 2:
		return engineConfiguration->can3BaudRate;
#endif
	}

	return engineConfiguration->canBaudRate;
}

static bool getCanConfiguredListenOnly(size_t index) {
	switch (index) {
	case 0:
		return engineConfiguration->can1ListenMode;
	case 1:
		return engineConfiguration->can2ListenMode;
#if (EFI_CAN_BUS_COUNT >= 3)
	case 2:
		return engineConfiguration->can3ListenMode;
#endif
	}

	return engineConfiguration->can1ListenMode;
}

// Lua canSetListenMode override; zero-initialized = follow the can*ListenMode configuration
enum class ListenOverride : int8_t { FollowConfig = 0, ForceNormal, ForceListen };
static ListenOverride listenOverride[EFI_CAN_BUS_COUNT] = {};

static bool getCanListenOnly(size_t index) {
	if (index < EFI_CAN_BUS_COUNT && listenOverride[index] != ListenOverride::FollowConfig) {
		return listenOverride[index] == ListenOverride::ForceListen;
	}
	return getCanConfiguredListenOnly(index);
}

// Last rate actually applied, so a listen-mode toggle preserves a runtime baud change
static can_baudrate_e currentBaudRate[EFI_CAN_BUS_COUNT] = {};

int txErrorCount[EFI_CAN_BUS_COUNT] = {};

static void canInfo() {
	if (!isCanEnabled) {
		efiPrintf("CAN is not enabled, please enable & restart");
		return;
	}

	efiPrintf("CAN1 TX %s %s err=%d listenOnly=%s", hwPortname(engineConfiguration->canTxPin), getCan_baudrate_e(engineConfiguration->canBaudRate), txErrorCount[0], boolToString(getCanListenOnly(0)));
	efiPrintf("CAN1 RX %s", hwPortname(engineConfiguration->canRxPin));
	canHwInfo(getCanDevice(0));

	efiPrintf("CAN2 TX %s %s err=%d listenOnly=%s", hwPortname(engineConfiguration->can2TxPin), getCan_baudrate_e(engineConfiguration->can2BaudRate), txErrorCount[1], boolToString(getCanListenOnly(1)));
	efiPrintf("CAN2 RX %s", hwPortname(engineConfiguration->can2RxPin));
	canHwInfo(getCanDevice(1));

#if (EFI_CAN_BUS_COUNT >= 3)
	efiPrintf("CAN3 TX %s %s err=%d listenOnly=%s", hwPortname(engineConfiguration->can3TxPin), getCan_baudrate_e(engineConfiguration->can3BaudRate), txErrorCount[2], boolToString(getCanListenOnly(2)));
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
#else
  UNUSED(canConfig);
  UNUSED(isListenOnly);
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
	CANDriver *device[EFI_CAN_BUS_COUNT];
	bool anyCan = false;
	for (size_t index = 0; index < EFI_CAN_BUS_COUNT; index++) {
		device[index] = getCanDevice(index);

		// Check for same devie select
		for (size_t j = 0; j < index; j++) {
			if ((device[index] != nullptr) && (device[index] == device[j])) {
				criticalError("CAN%d and CAN%d pins must be set to different devices", index + 1, j + 1);
				return;
			}
		}
		anyCan |= (device[index] != nullptr);
	}

	// If all devices are null, a firmware error was already thrown by detectCanDevice, but we shouldn't continue
	if (!anyCan) {
		return;
	}

	// Initialize peripherals
	for (size_t index = 0; index < EFI_CAN_BUS_COUNT; index++) {
		if (device[index]) {
			// Config based on baud rate
			// Pointer to this local canConfig is stored inside CANDriver
			// even it is used only during canStart this is wierd
			CANConfig canConfig;
			currentBaudRate[index] = getDefaultCanBaudRate(index);
			memcpy(&canConfig, findCanConfig(currentBaudRate[index]), sizeof(canConfig));
			applyListenOnly(&canConfig, getCanListenOnly(index));
			canStart(device[index], &canConfig);

			// Plumb CAN devices to tx system
			CanTxMessage::setDevice(index, device[index]);
		}
	}

	// fire up threads, as necessary
	if (engineConfiguration->canWriteEnabled) {
		canWrite.start();
	}

	if (engineConfiguration->canReadEnabled) {
		for (size_t index = 0; index < EFI_CAN_BUS_COUNT; index++) {
			canRead[index].setDevice(device[index]);
			canRead[index].start();
		}
	}

	isCanEnabled = true;
}

bool getIsCanEnabled(void) {
	return isCanEnabled;
}

// Stop, reconfigure and restart one CAN peripheral; shared by setCanBaud and setCanListenMode
static int restartCanBus(size_t index, can_baudrate_e rate) {
	auto device = getCanDevice(index);
	if (device == nullptr)
		return -2;

	// Stop listener
	canRead[index].stop();

	// Remove CAN device from tx system
	CanTxMessage::removeDevice(index);

	// Actually stop HW
	canStop(device);

	// Give some time for HW to stop
	chThdSleepMilliseconds(1);

	// Get config for new baudrate
	CANConfig canConfig;
	memcpy(&canConfig, findCanConfig(rate), sizeof(canConfig));
	applyListenOnly(&canConfig, getCanListenOnly(index));

	// Start HW
	canStart(device, &canConfig);

	// Plumb CAN devices to tx system
	CanTxMessage::setDevice(index, device);

	// Start listener
	if (engineConfiguration->canReadEnabled) {
		canRead[index].start();
	}

	currentBaudRate[index] = rate;

	return 0;
}

int setCanBaud(size_t index, can_baudrate_e rate) {
	if (index >= EFI_CAN_BUS_COUNT)
		return -1;

	return restartCanBus(index, rate);
}

int setCanListenMode(size_t index, bool listenOnlyEnabled) {
	if (index >= EFI_CAN_BUS_COUNT)
		return -1;

	listenOverride[index] = listenOnlyEnabled ? ListenOverride::ForceListen : ListenOverride::ForceNormal;

	if (!isCanEnabled) {
		// remembered; applied when CAN starts
		return 0;
	}

	return restartCanBus(index, currentBaudRate[index]);
}

int setCanBaud(size_t index, int baudrate) {
	can_baudrate_e rate;

	switch (baudrate) {
	case 33000:
		rate = B33KBPS;
		break;
	case 50000:
		rate = B50KBPS;
		break;
	case 83000:
	case 83333:
		rate = B83KBPS;
		break;
	case 100000:
		rate = B100KBPS;
		break;
	case 125000:
		rate = B125KBPS;
		break;
	case 250000:
		rate = B250KBPS;
		break;
	case 666000:
	case 666666:
		rate = B666KBPS;
		break;
	case 1000000:
		rate = B1MBPS;
		break;
	case 500000:
		rate = B500KBPS;
		break;
	default:
		return -3;
	}

	return setCanBaud(index, rate);
}

#endif /* EFI_CAN_SUPPORT */
