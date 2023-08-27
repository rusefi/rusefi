// This file contains just CAN bit timing configs for various MCUs and bitrates

#include "pch.h"

#if EFI_PROD_CODE

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
static const CANConfig canConfig50 = {
	.mcr = CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
	.btr = CAN_BTR_50
};

static const CANConfig canConfig83 = {
	.mcr = CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
	.btr = CAN_BTR_83
};

static const CANConfig canConfig100 = {
	.mcr = CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
	.btr = CAN_BTR_100
};

static const CANConfig canConfig125 = {
	.mcr = CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
	.btr = CAN_BTR_125
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

#else // not EFI_PROD_CODE
// Nothing to actually set for the simulator's CAN config.
// It's impossible to set CAN bitrate from userspace, so we can't set it.
static const CANConfig canConfig50;
static const CANConfig canConfig83;
static const CANConfig canConfig100;
static const CANConfig canConfig125;
static const CANConfig canConfig250;
static const CANConfig canConfig500;
static const CANConfig canConfig1000;
#endif

const CANConfig* findCanConfig(can_baudrate_e rate) {
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
