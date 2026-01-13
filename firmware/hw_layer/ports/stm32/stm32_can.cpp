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

// CAN_BTR_SJW(n), where n = SJW - 1
// CAN_BTR_BRP(n), where n = prescaler - 1
// CAN_BTR_TS1(n), where n = Seg 1 - 1
// CAN_BTR_TS2(n), where n = Seg 2 - 1

#ifdef STM32F4XX
// Clock 42 MHz
#define CAN_BTR_33  (CAN_BTR_SJW(0) | CAN_BTR_BRP(139)| CAN_BTR_TS1(6)  | CAN_BTR_TS2(0)) // sampling point at 88.9%
// These have an 85.7% sample point
#define CAN_BTR_50  (CAN_BTR_SJW(0) | CAN_BTR_BRP(59) | CAN_BTR_TS1(10) | CAN_BTR_TS2(1))
#define CAN_BTR_83  (CAN_BTR_SJW(0) | CAN_BTR_BRP(35) | CAN_BTR_TS1(10) | CAN_BTR_TS2(1))
#define CAN_BTR_100 (CAN_BTR_SJW(0) | CAN_BTR_BRP(29) | CAN_BTR_TS1(10) | CAN_BTR_TS2(1))
#define CAN_BTR_125 (CAN_BTR_SJW(0) | CAN_BTR_BRP(23) | CAN_BTR_TS1(10) | CAN_BTR_TS2(1))
#define CAN_BTR_250 (CAN_BTR_SJW(0) | CAN_BTR_BRP(11) | CAN_BTR_TS1(10) | CAN_BTR_TS2(1))
#define CAN_BTR_500 (CAN_BTR_SJW(0) | CAN_BTR_BRP(5)  | CAN_BTR_TS1(10) | CAN_BTR_TS2(1))
#define CAN_BTR_666 (CAN_BTR_SJW(0) | CAN_BTR_BRP(2)  | CAN_BTR_TS1(15) | CAN_BTR_TS2(3)) // sampling point 81.0%
#define CAN_BTR_1k0 (CAN_BTR_SJW(0) | CAN_BTR_BRP(2)  | CAN_BTR_TS1(10) | CAN_BTR_TS2(1))

#elif defined(STM32F7XX)
// Clock 54 MHz
// These have an 88.9% sample point
#define CAN_BTR_33  (CAN_BTR_SJW(0) | CAN_BTR_BRP(179)| CAN_BTR_TS1(6)  | CAN_BTR_TS2(0))
#define CAN_BTR_50  (CAN_BTR_SJW(0) | CAN_BTR_BRP(59) | CAN_BTR_TS1(14) | CAN_BTR_TS2(1))
#define CAN_BTR_83  (CAN_BTR_SJW(0) | CAN_BTR_BRP(35) | CAN_BTR_TS1(14) | CAN_BTR_TS2(1))
#define CAN_BTR_100 (CAN_BTR_SJW(0) | CAN_BTR_BRP(29) | CAN_BTR_TS1(14) | CAN_BTR_TS2(1))
#define CAN_BTR_125 (CAN_BTR_SJW(0) | CAN_BTR_BRP(23) | CAN_BTR_TS1(14) | CAN_BTR_TS2(1))
#define CAN_BTR_250 (CAN_BTR_SJW(0) | CAN_BTR_BRP(11) | CAN_BTR_TS1(14) | CAN_BTR_TS2(1))
#define CAN_BTR_500 (CAN_BTR_SJW(0) | CAN_BTR_BRP(5)  | CAN_BTR_TS1(14) | CAN_BTR_TS2(1))
#define CAN_BTR_666 (CAN_BTR_SJW(0) | CAN_BTR_BRP(8)  | CAN_BTR_TS1(6)  | CAN_BTR_TS2(0))
#define CAN_BTR_1k0 (CAN_BTR_SJW(0) | CAN_BTR_BRP(2)  | CAN_BTR_TS1(14) | CAN_BTR_TS2(1))

#elif defined(STM32H7XX)
// FDCAN driver has different bit timing registers (yes, different format)
// for the arbitration and data phases

static_assert(STM32_FDCANCLK == 80'000'000, "CANFD baudrates calculated for 80MHz clock!");

constexpr uint32_t CAN_NBTP_H7(uint32_t sjw, uint32_t prescaler, uint32_t seg1, uint32_t seg2) {
    return ((sjw-1)<<24) | ((prescaler-1)<<16) | ((seg1-1)<<8) | (seg2-1);
}
constexpr uint32_t CAN_DBTP_H7(uint32_t sjw, uint32_t prescaler, uint32_t seg1, uint32_t seg2) {
    return ((prescaler-1) << 16) | ((seg1-1) << 8) | ((seg2-1)<<4) | (sjw-1);
}

// 87.5% sample point
#define CAN_NBTP_33 0x06950c01
#define CAN_DBTP_33 0x00950D03   // TODO: validate!

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

#define CAN_NBTP_500 CAN_NBTP_H7(7, 10, 13, 2)
#define CAN_DBTP_500 CAN_DBTP_H7(4, 10, 13, 2)
static_assert(CAN_NBTP_500 == 0x06090C01);
static_assert(CAN_DBTP_500 == 0x00090C13);

#define CAN_NBTP_1k0 CAN_NBTP_H7(7, 5, 13, 2)
#define CAN_DBTP_1k0 CAN_DBTP_H7(4, 5, 13, 2)
static_assert(CAN_NBTP_1k0 == 0x06040C01);
static_assert(CAN_DBTP_1k0 == 0x00040C13);

// 86.7% sample point
#define CAN_NBTP_666 CAN_NBTP_H7(7, 8, 12, 2)
#define CAN_DBTP_666 CAN_DBTP_H7(4, 8, 12, 2)

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

static const CANConfig canConfig33 = {
   .mcr = STM32FxMCR,
   .btr = CAN_BTR_33
};

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

static const CANConfig canConfig666 = {
   .mcr = STM32FxMCR,
   .btr = CAN_BTR_666
};

static const CANConfig canConfig1000 = {
    .mcr = STM32FxMCR,
   .btr = CAN_BTR_1k0
};

#elif defined(STM32H7XX)
static const CANConfig canConfig33 = {
   .op_mode = OPMODE_CAN,
   .NBTP = CAN_NBTP_33,
   .DBTP = CAN_DBTP_33,
   .TDCR = 0,
   .CCCR = 0,
   .TEST = 0,
   .RXGFC = 0,
};

static const CANConfig canConfig50 = {
   .op_mode = OPMODE_CAN,
   .NBTP = CAN_NBTP_50,
   .DBTP = CAN_DBTP_50,
   .TDCR = 0,
   .CCCR = 0,
   .TEST = 0,
   .RXGFC = 0,
};

static const CANConfig canConfig83 = {
   .op_mode = OPMODE_CAN,
   .NBTP = CAN_NBTP_83,
   .DBTP = CAN_DBTP_83,
   .TDCR = 0,
   .CCCR = 0,
   .TEST = 0,
   .RXGFC = 0,
};

static const CANConfig canConfig100 = {
   .op_mode = OPMODE_CAN,
   .NBTP = CAN_NBTP_100,
   .DBTP = CAN_DBTP_100,
   .TDCR = 0,
   .CCCR = 0,
   .TEST = 0,
   .RXGFC = 0,
};

static const CANConfig canConfig125 = {
   .op_mode = OPMODE_CAN,
   .NBTP = CAN_NBTP_125,
   .DBTP = CAN_DBTP_125,
   .TDCR = 0,
   .CCCR = 0,
   .TEST = 0,
   .RXGFC = 0,
};

static const CANConfig canConfig250 = {
   .op_mode = OPMODE_CAN,
   .NBTP = CAN_NBTP_250,
   .DBTP = CAN_DBTP_250,
   .TDCR = 0,
   .CCCR = 0,
   .TEST = 0,
   .RXGFC = 0,
};

static const CANConfig canConfig500 = {
   .op_mode = OPMODE_CAN,
   .NBTP = CAN_NBTP_500,
   .DBTP = CAN_DBTP_500,
   .TDCR = 0,
   .CCCR = 0,
   .TEST = 0,
   .RXGFC = 0,
};

static const CANConfig canConfig666 = {
   .op_mode = OPMODE_CAN,
   .NBTP = CAN_NBTP_666,
   .DBTP = CAN_DBTP_666,
   .TDCR = 0,
   .CCCR = 0,
   .TEST = 0,
   .RXGFC = 0,
};

static const CANConfig canConfig1000 = {
   .op_mode = OPMODE_CAN,
   .NBTP = CAN_NBTP_1k0,
   .DBTP = CAN_DBTP_1k0,
   .TDCR = 0,
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

#if STM32_CAN_USE_CAN3
// this is about STM32F413
static bool isValidCan3RxPin(brain_pin_e pin) {
	return pin == Gpio::A8 || pin == Gpio::B3;
}

static bool isValidCan3TxPin(brain_pin_e pin) {
	return pin == Gpio::A15 || pin == Gpio::B4;
}
#elif STM32_CAN_USE_FDCAN3
// STM32H723
// See different AF for PD12/PD13 vs PF6/PF7 and PG9/PG10
static bool isValidCan3RxPin(brain_pin_e pin) {
	return pin == Gpio::D12 /* || pin == Gpio::F6 || pin == Gpio::G10 */;
}

static bool isValidCan3TxPin(brain_pin_e pin) {
	return pin == Gpio::D13 /* || pin == Gpio::F7 || pin == Gpio::G9 */;
}
#else
static __attribute__((unused)) bool isValidCan3RxPin(brain_pin_e) {
	return false;
}

static __attribute__((unused)) bool isValidCan3TxPin(brain_pin_e) {
	return false;
}
#endif

bool isValidCanTxPin(brain_pin_e pin) {
	// Note: different AF for CAN3 and CANFD3, so check for CAN1/CANFD1 and CAN2/CANFD2 only
	// see startCanPins()
	return isValidCan1TxPin(pin) || isValidCan2TxPin(pin) /* || isValidCan3TxPin(pin) */;
}

bool isValidCanRxPin(brain_pin_e pin) {
	return isValidCan1RxPin(pin) || isValidCan2RxPin(pin) /* || isValidCan3RxPin(pin) */;
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
#if STM32_CAN_USE_CAN3 || STM32_CAN_USE_FDCAN3
   if (isValidCan3RxPin(pinRx) && isValidCan3TxPin(pinTx))
      return &CAND3;
#endif
   criticalError("invalid CAN pins tx %s and rx %s", hwPortname(pinTx), hwPortname(pinRx));
   return nullptr;
}

const CANConfig * findCanConfig(can_baudrate_e rate) {
   switch (rate) {
   case B33KBPS:
      return &canConfig33;
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
   case B666KBPS:
      return &canConfig666;
   case B1MBPS:
      return &canConfig1000;
   case B500KBPS:
   default:
      return &canConfig500;
   }
}

void canHwInfo(CANDriver* cand) {
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
#elif STM32_CAN_USE_FDCAN1 || STM32_CAN_USE_FDCAN2 || STM32_CAN_USE_FDCAN3
	if (cand->fdcan == NULL) {
		efiPrintf("No device assigned!");
	}

	uint32_t cccr = cand->fdcan->CCCR;
	efiPrintf("CCCR: %s %s %s",
		(cccr & FDCAN_CCCR_MON) ? "Bus monitoring mode" : "",
		(cccr & FDCAN_CCCR_ASM) ? "ASM restricted operation mode" : "",
		(cccr & FDCAN_CCCR_INIT) ? "INIT" : "");

	uint32_t ecr = cand->fdcan->ECR;
	efiPrintf("CAN error logging counter %ld",
		(ecr & FDCAN_ECR_CEL_Msk) >> FDCAN_ECR_CEL_Pos);
	if (ecr & FDCAN_ECR_RP) {
		efiPrintf("Receive error passive 128+");
	} else {
		efiPrintf("Receive error counter %ld",
			(ecr & FDCAN_ECR_REC_Msk) >> FDCAN_ECR_REC_Pos);
	}
	efiPrintf("Transmit error counter %ld",
		(ecr & FDCAN_ECR_TEC_Msk) >> FDCAN_ECR_TEC_Pos);

	uint32_t psr = cand->fdcan->PSR;
	efiPrintf("Flags: %s %s %s %s",
		(psr & FDCAN_PSR_PXE) ? "PXE" : "",
		(psr & FDCAN_PSR_BO) ? "Bus_Off" : "",
		(psr & FDCAN_PSR_EW) ? "EW" : "",
		(psr & FDCAN_PSR_EP) ? "Error passive" : "");

	static const char *lec_names[8] = {
		"No error",
		"Stuff error",
		"Form error",
		"AckError",
		"Bit1Error",
		"Bit0Error",
		"CRCError",
		"NoChange"
	};

	efiPrintf("Last error %s", lec_names[psr & 0x7]);
#endif
}

void canHwRecover(const size_t busIndex, CANDriver *cand)
{
#if STM32_CAN_USE_FDCAN1 || STM32_CAN_USE_FDCAN2 || STM32_CAN_USE_FDCAN3
	// equal to TIMEOUT_INIT_MS
	#define TIMEOUT_RECOVER_MS	250U

	// mostly copy-paste of static bool fdcan_active_mode(CANDriver *canp) from ChibiOS hal_can_lld.c
	if (cand->fdcan->CCCR & FDCAN_CCCR_INIT) {
		systime_t start, end;

		/* Going in active mode then waiting for it to happen.*/
		cand->fdcan->CCCR &= ~FDCAN_CCCR_INIT;
		start = osalOsGetSystemTimeX();
		end = osalTimeAddX(start, TIME_MS2I(TIMEOUT_RECOVER_MS));
		while ((cand->fdcan->CCCR & FDCAN_CCCR_INIT) != 0U) {
			if (!osalTimeIsInRangeX(osalOsGetSystemTimeX(), start, end)) {
				return;
			}
			osalThreadSleep(1);
		}
	}
#endif
}

#endif /* EFI_CAN_SUPPORT */
