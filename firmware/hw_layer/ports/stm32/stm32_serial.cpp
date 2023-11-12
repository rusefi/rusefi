/**
 * @file	stm32_common.cpp
 * @brief	Low level common STM32 code
 *
 * @date Mar 28, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if EFI_AUX_SERIAL

static bool isValidUART6TxPin(brain_pin_e pin) {
	return pin == Gpio::C6 || pin == Gpio::G14;
}

static bool isValidUART6RxPin(brain_pin_e pin) {
	return pin == Gpio::C7 || pin == Gpio::G9;
}

bool isValidSerialTxPin(brain_pin_e pin) {
   return isValidUART6TxPin(pin);
}

bool isValidSerialRxPin(brain_pin_e pin) {
   return isValidUART6RxPin(pin);
}

#endif /*EFI_AUX_SERIAL*/