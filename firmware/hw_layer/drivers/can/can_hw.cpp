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
#include "thread_priority.h"

static int canReadCounter = 0;
int canWriteOk = 0;
int canWriteNotOk = 0;
static bool isCanEnabled = false;

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

static const CANConfig *canConfig = &canConfig500;

class CanRead final : public ThreadController<UTILITY_THREAD_STACK_SIZE> {
public:
	CanRead()
		: ThreadController("CAN RX", PRIO_CAN_RX)
	{
	}

	void ThreadTask() override {
		CANDriver* device = detectCanDevice(CONFIG(canRxPin), CONFIG(canTxPin));

		if (!device) {
			warning(CUSTOM_ERR_CAN_CONFIGURATION, "CAN configuration issue");
			return;
		}

		while (true) {
			// Block until we get a message
			msg_t result = canReceiveTimeout(device, CAN_ANY_MAILBOX, &m_buffer, TIME_INFINITE);

			if (result != MSG_OK) {
				continue;
			}

			// Process the message
			canReadCounter++;

			processCanRxMessage(m_buffer, getTimeNowNt());
		}
	}

private:
	CANRxFrame m_buffer;
};

static CanRead canRead CCM_OPTIONAL;
static CanWrite canWrite CCM_OPTIONAL;

static void canInfo(void) {
	if (!isCanEnabled) {
		efiPrintf("CAN is not enabled, please enable & restart");
		return;
	}

	efiPrintf("CAN TX %s", hwPortname(CONFIG(canTxPin)));
	efiPrintf("CAN RX %s", hwPortname(CONFIG(canRxPin)));
	efiPrintf("type=%d canReadEnabled=%s canWriteEnabled=%s period=%d", engineConfiguration->canNbcType,
			boolToString(engineConfiguration->canReadEnabled), boolToString(engineConfiguration->canWriteEnabled),
			engineConfiguration->canSleepPeriodMs);

	efiPrintf("CAN rx_cnt=%d/tx_ok=%d/tx_not_ok=%d", canReadCounter, canWriteOk, canWriteNotOk);
}

void setCanType(int type) {
	engineConfiguration->canNbcType = (can_nbc_e)type;
	canInfo();
}

#if EFI_TUNER_STUDIO
void postCanState(TunerStudioOutputChannels *tsOutputChannels) {
	tsOutputChannels->debugIntField1 = isCanEnabled ? canReadCounter : -1;
	tsOutputChannels->debugIntField2 = isCanEnabled ? canWriteOk : -1;
	tsOutputChannels->debugIntField3 = isCanEnabled ? canWriteNotOk : -1;
}
#endif /* EFI_TUNER_STUDIO */

void enableFrankensoCan(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	CONFIG(canTxPin) = GPIOB_6;
	CONFIG(canRxPin) = GPIOB_12;
	engineConfiguration->canReadEnabled = false;
}

// todo: we usually use 'activeConfiguration' for 'stopPin' why this unusual code here?
// this is related to #1375
static brain_pin_e currentTxPin = GPIO_UNASSIGNED;
static brain_pin_e currentRxPin = GPIO_UNASSIGNED;

void stopCanPins(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	efiSetPadUnused(currentTxPin);
	efiSetPadUnused(currentRxPin);
}

void startCanPins(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// Store pins so we can disable later
	currentTxPin = CONFIG(canTxPin);
	currentRxPin = CONFIG(canRxPin);

	efiSetPadMode("CAN TX", currentTxPin, PAL_MODE_ALTERNATE(EFI_CAN_TX_AF));
	efiSetPadMode("CAN RX", currentRxPin, PAL_MODE_ALTERNATE(EFI_CAN_RX_AF));
}

void initCan(void) {
	addConsoleAction("caninfo", canInfo);

	isCanEnabled = 
		(isBrainPinValid(CONFIG(canTxPin))) && // both pins are set...
		(isBrainPinValid(CONFIG(canRxPin))) &&
		(CONFIG(canWriteEnabled) || CONFIG(canReadEnabled)) ; // ...and either read or write is enabled

	// nothing to do if we aren't enabled...
	if (!isCanEnabled) {
		return;
	}

	// Validate pins
	if (!isValidCanTxPin(CONFIG(canTxPin))) {
		firmwareError(CUSTOM_OBD_70, "invalid CAN TX %s", hwPortname(CONFIG(canTxPin)));
		return;
	}

	if (!isValidCanRxPin(CONFIG(canRxPin))) {
		firmwareError(CUSTOM_OBD_70, "invalid CAN RX %s", hwPortname(CONFIG(canRxPin)));
		return;
	}

	switch (CONFIG(canBaudRate)) {
	case B100KBPS:
		canConfig = &canConfig100;
		break;
	case B250KBPS:
		canConfig = &canConfig250;
		break;
	case B500KBPS:
		canConfig = &canConfig500;
		break;
	case B1MBPS:
		canConfig = &canConfig1000;
		break;
	default:
		break;
	}

	startCanPins();

	// Initialize hardware
#if STM32_CAN_USE_CAN2
	// CAN1 is required for CAN2
	canStart(&CAND1, canConfig);
	canStart(&CAND2, canConfig);
#else
	canStart(&CAND1, canConfig);
#endif /* STM32_CAN_USE_CAN2 */

	// Plumb CAN device to tx system
	CanTxMessage::setDevice(detectCanDevice(
		CONFIG(canRxPin),
		CONFIG(canTxPin)
	));

	// fire up threads, as necessary
	if (CONFIG(canWriteEnabled)) {
		canWrite.Start();
	}

	if (CONFIG(canReadEnabled)) {
		canRead.Start();
	}
}

#endif /* EFI_CAN_SUPPORT */
