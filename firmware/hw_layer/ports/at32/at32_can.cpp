/**
 * @file	at32_can.cpp
 * @brief	Low level common AT32 code
 *
 * @date Nov 19, 2023
 * @author Andrey Gusakov, (c) 2023
 */

#include "pch.h"

#if EFI_CAN_SUPPORT

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

#endif /* EFI_CAN_SUPPORT */