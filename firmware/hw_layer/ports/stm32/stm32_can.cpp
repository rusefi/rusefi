/**
 * @file	stm32_common_can.cpp
 * @brief	Low level common STM32 code
 *
 * @date Mar 28, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if EFI_CAN_SUPPORT


// Values below calculated with http://www.bittiming.can-wiki.info/
// Pick ST micro bxCAN
// Clock rate of 42mhz for f4, 54mhz for f7, 80mhz for h7
#ifdef STM32F4XX
// These have an 85.7% sample point
#define CAN_BTR_50  (CAN_BTR_SJW(0) | CAN_BTR_BRP(59) | CAN_BTR_TS1(10) | CAN_BTR_TS2(1))
#define CAN_BTR_83  (CAN_BTR_SJW(0) | CAN_BTR_BRP(35) | CAN_BTR_TS1(10) | CAN_BTR_TS2(1))
#define CAN_BTR_100 (CAN_BTR_SJW(0) | CAN_BTR_BRP(29) | CAN_BTR_TS1(10) | CAN_BTR_TS2(1))
#define CAN_BTR_125 (CAN_BTR_SJW(0) | CAN_BTR_BRP(23) | CAN_BTR_TS1(10) | CAN_BTR_TS2(1))
#define CAN_BTR_250 (CAN_BTR_SJW(0) | CAN_BTR_BRP(11) | CAN_BTR_TS1(10) | CAN_BTR_TS2(1))
#define CAN_BTR_500 (CAN_BTR_SJW(0) | CAN_BTR_BRP(5)  | CAN_BTR_TS1(10) | CAN_BTR_TS2(1))
#define CAN_BTR_1k0 (CAN_BTR_SJW(0) | CAN_BTR_BRP(2)  | CAN_BTR_TS1(10) | CAN_BTR_TS2(1))
#elif defined(STM32F7XX)
// These have an 88.9% sample point
#define CAN_BTR_50  (CAN_BTR_SJW(0) | CAN_BTR_BRP(59) | CAN_BTR_TS1(14) | CAN_BTR_TS2(1))
#define CAN_BTR_83  (CAN_BTR_SJW(0) | CAN_BTR_BRP(35) | CAN_BTR_TS1(14) | CAN_BTR_TS2(1))
#define CAN_BTR_100 (CAN_BTR_SJW(0) | CAN_BTR_BRP(29) | CAN_BTR_TS1(14) | CAN_BTR_TS2(1))
#define CAN_BTR_125 (CAN_BTR_SJW(0) | CAN_BTR_BRP(23) | CAN_BTR_TS1(14) | CAN_BTR_TS2(1))
#define CAN_BTR_250 (CAN_BTR_SJW(0) | CAN_BTR_BRP(11) | CAN_BTR_TS1(14) | CAN_BTR_TS2(1))
#define CAN_BTR_500 (CAN_BTR_SJW(0) | CAN_BTR_BRP(5)  | CAN_BTR_TS1(14) | CAN_BTR_TS2(1))
#define CAN_BTR_1k0 (CAN_BTR_SJW(0) | CAN_BTR_BRP(2)  | CAN_BTR_TS1(14) | CAN_BTR_TS2(1))
#elif defined(STM32H7XX)
// FDCAN driver has different bit timing registers (yes, different format)
// for the arbitration and data phases

// 66% sample point, not ideal but best we can do without changing CAN clock
#define CAN_NBTP_50 0x061F1F10
#define CAN_DBTP_50 0x001F2003

// 86.7% sample point
#define CAN_NBTP_83 0x061F1803
#define CAN_DBTP_83 0x001F1833

// 88.0% sample point
#define CAN_NBTP_100 0x061F1402
#define CAN_DBTP_100 0x001F1423

// 85.0% sample point
#define CAN_NBTP_125 0x061F0F02
#define CAN_DBTP_125 0x001F0F23

// These have an 87.5% sample point
#define CAN_NBTP_250 0x06130C01
#define CAN_DBTP_250 0x00130C13

#define CAN_NBTP_500 0x06090C01
#define CAN_DBTP_500 0x00090C13

#define CAN_NBTP_1k0 0x06040C01
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

#elif defined(STM32H7XX)
static const CANConfig canConfig50 = {
   .NBTP = CAN_NBTP_50,
   .DBTP = CAN_DBTP_50,
   .CCCR = 0,
   .TEST = 0,
   .RXGFC = 0,
};

static const CANConfig canConfig83 = {
   .NBTP = CAN_NBTP_83,
   .DBTP = CAN_DBTP_83,
   .CCCR = 0,
   .TEST = 0,
   .RXGFC = 0,
};

static const CANConfig canConfig100 = {
   .NBTP = CAN_NBTP_100,
   .DBTP = CAN_DBTP_100,
   .CCCR = 0,
   .TEST = 0,
   .RXGFC = 0,
};

static const CANConfig canConfig125 = {
   .NBTP = CAN_NBTP_125,
   .DBTP = CAN_DBTP_125,
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

static bool isValidCan1RxPin(brain_pin_e pin) {
	return pin == Gpio::A11 || pin == Gpio::B8 || pin == Gpio::D0;
}

static bool isValidCan1TxPin(brain_pin_e pin) {
	return pin == Gpio::A12 || pin == Gpio::B9 || pin == Gpio::D1;
}

static bool isValidCan2RxPin(brain_pin_e pin) {
	return pin == Gpio::B5 || pin == Gpio::B12;
}

static bool isValidCan2TxPin(brain_pin_e pin) {
	return pin == Gpio::B6 || pin == Gpio::B13;
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
   if (cand == NULL)
      return;

   if (cand->state != CAN_READY) {
      efiPrintf("Interface is not ready");
      return;
   }

#if STM32_CAN_USE_CAN1 || STM32_CAN_USE_CAN2
   if (cand->can == NULL) {
      efiPrintf("No device assigned!");
   }

   uint32_t esr = cand->can->ESR;
   efiPrintf("Receive error counter %ld", (esr >> 24) & 0xff);
   efiPrintf("Transmit error counter %ld", (esr >> 16) & 0xff);
   efiPrintf("Last error %ld", (esr >> 4) & 0x7);
   efiPrintf("Flags: %s %s %s",
      (esr & 0x4) ? "BOFF" : "",
      (esr & 0x2) ? "EPVF" : "",
      (esr & 0x1) ? "EWGF" : "");
#endif
}

#endif /* EFI_CAN_SUPPORT */
