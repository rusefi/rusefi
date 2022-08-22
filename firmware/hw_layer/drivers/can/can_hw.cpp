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

// Values below calculated with http://www.bittiming.can-wiki.info/
// Pick ST micro bxCAN
// Clock rate of 42mhz for f4, 54mhz for f7, 80mhz for h7
#ifdef STM32F4XX
// These have an 85.7% sample point
#define CAN_BTR_100 (CAN_BTR_SJW(0) | CAN_BTR_BRP(29) | CAN_BTR_TS1(10) | CAN_BTR_TS2(1))
#define CAN_BTR_250 (CAN_BTR_SJW(0) | CAN_BTR_BRP(11) | CAN_BTR_TS1(10) | CAN_BTR_TS2(1))
#define CAN_BTR_500 (CAN_BTR_SJW(0) | CAN_BTR_BRP(5)  | CAN_BTR_TS1(10) | CAN_BTR_TS2(1))
#define CAN_BTR_1k0 (CAN_BTR_SJW(0) | CAN_BTR_BRP(2)  | CAN_BTR_TS1(10) | CAN_BTR_TS2(1))
#elif defined(STM32F7XX)
// These have an 88.9% sample point
#define CAN_BTR_100 (CAN_BTR_SJW(0) | CAN_BTR_BRP(30) | CAN_BTR_TS1(15) | CAN_BTR_TS2(2))
#define CAN_BTR_250 (CAN_BTR_SJW(0) | CAN_BTR_BRP(11) | CAN_BTR_TS1(14) | CAN_BTR_TS2(1))
#define CAN_BTR_500 (CAN_BTR_SJW(0) | CAN_BTR_BRP(5)  | CAN_BTR_TS1(14) | CAN_BTR_TS2(1))
#define CAN_BTR_1k0 (CAN_BTR_SJW(0) | CAN_BTR_BRP(2)  | CAN_BTR_TS1(14) | CAN_BTR_TS2(1))
#elif defined(STM32H7XX)
// These have an 87.5% sample point
// FDCAN driver has different bit timing registers (yes, different format)
// for the arbitration and data phases
#define CAN_NBTP_100 0x00310C01
#define CAN_DBTP_100 0x00310C13

#define CAN_NBTP_250 0x00130C01
#define CAN_DBTP_250 0x00130C13

#define CAN_NBTP_500 0x00090C01
#define CAN_DBTP_500 0x00090C13

#define CAN_NBTP_1k0 0x00040C01
#define CAN_DBTP_1k0 0x00040C13
#else
#error Please define CAN BTR settings for your MCU!
#endif

/*
 * 500KBaud
 * automatic wakeup
 * automatic recover from abort mode
 * See section 22.7.7 on the STM32 reference manual.
 * 
 * 29 bit would be CAN_TI0R_EXID (?) but we do not mention it here
 * CAN_TI0R_STID "Standard Identifier or Extended Identifier"? not mentioned as well
 */
#if defined(STM32F4XX) || defined(STM32F7XX)
static const CANConfig canConfig100 = {
	.mcr = CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
	.btr = CAN_BTR_100
};

static const CANConfig canConfig250 = {
	.mcr = CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
	.btr = CAN_BTR_250
};

static const CANConfig canConfig500 = {
	.mcr = CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
	.btr = CAN_BTR_500
};

static const CANConfig canConfig1000 = {
CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
CAN_BTR_1k0 };
#elif defined(STM32H7XX)
static const CANConfig canConfig100 = {
	.NBTP = CAN_NBTP_100,
	.DBTP = CAN_DBTP_100,
	.CCCR = 0,
	.TEST = 0,
	.RXGFC = 0,
};

static const CANConfig canConfig250 = {
	.NBTP = CAN_NBTP_250,
	.DBTP = CAN_DBTP_250,
	.CCCR = 0,
	.TEST = 0,
	.RXGFC = 0,
};

static const CANConfig canConfig500 = {
	.NBTP = CAN_NBTP_500,
	.DBTP = CAN_DBTP_500,
	.CCCR = 0,
	.TEST = 0,
	.RXGFC = 0,
};

static const CANConfig canConfig1000 = {
	.NBTP = CAN_NBTP_1k0,
	.DBTP = CAN_DBTP_1k0,
	.CCCR = 0,
	.TEST = 0,
	.RXGFC = 0,
};
#endif

#else // not EFI_PROD_CODE
// Nothing to actually set for the simulator's CAN config.
// It's impossible to set CAN bitrate from userspace, so we can't set it.
static const CANConfig canConfig100;
static const CANConfig canConfig250;
static const CANConfig canConfig500;
static const CANConfig canConfig1000;
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

void enableFrankensoCan() {
	engineConfiguration->canTxPin = Gpio::B6;
	engineConfiguration->canRxPin = Gpio::B12;
	engineConfiguration->canReadEnabled = false;
}

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
		if (engineConfiguration->canTxPin == Gpio::Unassigned) {
			// todo: smarter online change of settings, kill isCanEnabled with fire
			return;
		}
		firmwareError(CUSTOM_OBD_70, "invalid CAN TX %s", hwPortname(engineConfiguration->canTxPin));
		return;
	}

	if (!isValidCanRxPin(engineConfiguration->canRxPin)) {
		if (engineConfiguration->canRxPin == Gpio::Unassigned) {
			// todo: smarter online change of settings, kill isCanEnabled with fire
			return;
		}
		firmwareError(CUSTOM_OBD_70, "invalid CAN RX %s", hwPortname(engineConfiguration->canRxPin));
		return;
	}

#if EFI_PROD_CODE
	efiSetPadModeIfConfigurationChanged("CAN TX", canTxPin, PAL_MODE_ALTERNATE(EFI_CAN_TX_AF));
	efiSetPadModeIfConfigurationChanged("CAN RX", canRxPin, PAL_MODE_ALTERNATE(EFI_CAN_RX_AF));

	efiSetPadModeIfConfigurationChanged("CAN2 TX", can2TxPin, PAL_MODE_ALTERNATE(EFI_CAN_TX_AF));
	efiSetPadModeIfConfigurationChanged("CAN2 RX", can2RxPin, PAL_MODE_ALTERNATE(EFI_CAN_RX_AF));
#endif // EFI_PROD_CODE
}

static const CANConfig * findConfig(can_baudrate_e rate) {
	switch (rate) {
	case B100KBPS:
		return &canConfig100;
		break;
	case B250KBPS:
		return &canConfig250;
		break;
	case B1MBPS:
		return &canConfig1000;
		break;
	case B500KBPS:
	default:
		return &canConfig500;
	}
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
		firmwareError(OBD_PCM_Processor_Fault, "CAN pins must be set to different devices");
		return;
	}

	// Generate configs based on baud rate
	auto config1 = findConfig(engineConfiguration->canBaudRate);
	auto config2 = findConfig(engineConfiguration->can2BaudRate);

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
		canRead1.start(device1);
		canRead2.start(device2);
	}

	isCanEnabled = true;
}

bool getIsCanEnabled(void) {
	return isCanEnabled;
}

#endif /* EFI_CAN_SUPPORT */

void assertHwCanBusIndex(const size_t busIndex) {
    // 'size_t' is an unsigned type so we are never below zero here
	if (busIndex > 1) {
  		firmwareError(CUSTOM_OBD_70, "Invalid HW CAN bus index %d", busIndex);
  	}
}
