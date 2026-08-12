/**
 * @file	at32_can.cpp
 * @brief	Low level common AT32 code
 *
 * @date Nov 19, 2023
 * @author Andrey Gusakov, (c) 2023
 */

#include "pch.h"

#if EFI_CAN_SUPPORT

// Values below calculated with http://www.bittiming.can-wiki.info/
// Pick ST micro bxCAN

#if (STM32_PCLK1 == 144000000)
// Clock rate of 144mhz for AT32F435
// These have an 85.7% sample point
// CAN_BTR_BRP - Baud Rate Prescaler
// CAN_BTR_SJW - Resynchronization Jump Width
// CAN_BTR_TS1 - Time Segment 1
// CAN_BTR_TS2 - Time Segment 2
#define CAN_BTR_50  (CAN_BTR_BRP(160 - 1) | CAN_BTR_SJW(18 - 1) | CAN_BTR_TS1(15 - 1) | CAN_BTR_TS2(2 - 1))
#define CAN_BTR_83  (CAN_BTR_BRP(96  - 1) | CAN_BTR_SJW(18 - 1) | CAN_BTR_TS1(15 - 1) | CAN_BTR_TS2(2 - 1)) /* not accurate */
#define CAN_BTR_100 (CAN_BTR_BRP(80  - 1) | CAN_BTR_SJW(18 - 1) | CAN_BTR_TS1(15 - 1) | CAN_BTR_TS2(2 - 1))
#define CAN_BTR_125 (CAN_BTR_BRP(64  - 1) | CAN_BTR_SJW(18 - 1) | CAN_BTR_TS1(15 - 1) | CAN_BTR_TS2(2 - 1))
#define CAN_BTR_250 (CAN_BTR_BRP(32  - 1) | CAN_BTR_SJW(18 - 1) | CAN_BTR_TS1(15 - 1) | CAN_BTR_TS2(2 - 1))
#define CAN_BTR_500 (CAN_BTR_BRP(16  - 1) | CAN_BTR_SJW(18 - 1) | CAN_BTR_TS1(15 - 1) | CAN_BTR_TS2(2 - 1))
#define CAN_BTR_1k0 (CAN_BTR_BRP(8   - 1) | CAN_BTR_SJW(18 - 1) | CAN_BTR_TS1(15 - 1) | CAN_BTR_TS2(2 - 1))
#endif

#define STM32FxMCR (CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP)

static const CANConfig canConfig50 = {
   .mcr = STM32FxMCR,
   .btr = CAN_BTR_50
};

static const CANConfig canConfig83 = {
   .mcr = STM32FxMCR,
   .btr = CAN_BTR_83
};

static const CANConfig canConfig100 = {
   .mcr = STM32FxMCR,
   .btr = CAN_BTR_100
};

static const CANConfig canConfig125 = {
   .mcr = STM32FxMCR,
   .btr = CAN_BTR_125
};

static const CANConfig canConfig250 = {
   .mcr = STM32FxMCR,
   .btr = CAN_BTR_250
};

static const CANConfig canConfig500 = {
   .mcr = STM32FxMCR,
   .btr = CAN_BTR_500
};

static const CANConfig canConfig1000 = {
    .mcr = STM32FxMCR,
   .btr = CAN_BTR_1k0
};

static bool isValidCan1RxPin(brain_pin_e pin) {
	return pin == Gpio::A11 || pin == Gpio::B8 || pin == Gpio::D0 || pin == Gpio::G0;
}

static bool isValidCan1TxPin(brain_pin_e pin) {
	return pin == Gpio::A12 || pin == Gpio::B9 || pin == Gpio::D1 || pin == Gpio::G1;
}

static bool isValidCan2RxPin(brain_pin_e pin) {
	return pin == Gpio::B5 || pin == Gpio::B12 || pin == Gpio::G11;
}

static bool isValidCan2TxPin(brain_pin_e pin) {
	return pin == Gpio::B6 || pin == Gpio::B13 || pin == Gpio::G12;
}

bool isValidCanTxPin(brain_pin_e pin) {
   return isValidCan1TxPin(pin) || isValidCan2TxPin(pin);
}

bool isValidCanRxPin(brain_pin_e pin) {
   return isValidCan1RxPin(pin) || isValidCan2RxPin(pin);
}

CANDriver* detectCanDevice(brain_pin_e pinRx, brain_pin_e pinTx) {
	if (pinRx == Gpio::Unassigned || pinTx == Gpio::Unassigned) {
		return nullptr;
	}
#if STM32_CAN_USE_CAN1 || STM32_CAN_USE_FDCAN1
   if (isValidCan1RxPin(pinRx) && isValidCan1TxPin(pinTx))
      return &CAND1;
#endif
#if STM32_CAN_USE_CAN2 || STM32_CAN_USE_FDCAN2
   if (isValidCan2RxPin(pinRx) && isValidCan2TxPin(pinTx))
      return &CAND2;
#endif
   criticalError("invalid CAN pins tx %s and rx %s", hwPortname(pinTx), hwPortname(pinRx));
   return nullptr;
}

const CANConfig * findCanConfig(can_baudrate_e rate) {
   switch (rate) {
   case B50KBPS:
      return &canConfig50;
   case B83KBPS:
      return &canConfig83;
   case B100KBPS:
      return &canConfig100;
   case B125KBPS:
      return &canConfig125;
   case B250KBPS:
      return &canConfig250;
   case B1MBPS:
      return &canConfig1000;
   case B500KBPS:
   default:
      return &canConfig500;
   }
}

void canHwInfo(CANDriver* cand)
{
   /* TODO: */
   (void)cand;
}

void canHwRecover(const size_t, CANDriver *)
{
	/* TODO: */
}

#endif /* EFI_CAN_SUPPORT */