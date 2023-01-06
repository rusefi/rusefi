/*
 * @file rusefi_hw_enums.h
 *
 * @date Jun 2, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

// todo: migrate/unify with pin_output_mode_e? rename? something is messy here
// this enum is currently only used for SPI pins
typedef enum __attribute__ ((__packed__)) {
// todo: here we have a rare example of stm32-specific enum, todo: make this not stm32 specific?
	PO_DEFAULT = 0,
	PO_OPENDRAIN = 4, // PAL_STM32_OTYPE_OPENDRAIN
	PO_PULLUP = 32, // PAL_STM32_PUDR_PULLUP
	PO_PULLDOWN = 64 // PAL_STM32_PUPDR_PULLDOWN
} pin_mode_e;

/**
 * Hardware pin. This enum is platform-specific.
 */
enum class Gpio : uint16_t {
	Unassigned = 0,
	// only used as return value of 'parseBrainPin' function do we really this this logic special value at all?!
	Invalid = 1,

	A0 = 2,
	A1 = 3,
	A2 = 4,
	A3 = 5,
	A4 = 6,
	A5 = 7,
	A6 = 8,
	A7 = 9,
	A8 = 10,
	A9 = 11,
	A10 = 12,
	A11 = 13,
	A12 = 14,
	A13 = 15,
	A14 = 16,
	A15 = 17,

	B0 = 18,
	B1 = 19,
	B2 = 20,
	B3 = 21,
	B4 = 22,
	B5 = 23,
	B6 = 24,
	B7 = 25,
	B8 = 26,
	B9 = 27,
	B10 = 28,
	B11 = 29,
	B12 = 30,
	B13 = 31,
	B14 = 32,
	B15 = 33,

	C0 = 34,
	C1 = 35,
	C2 = 36,
	C3 = 37,
	C4 = 38,
	C5 = 39,
	C6 = 40,
	C7 = 41,
	C8 = 42,
	C9 = 43,
	C10 = 44,
	C11 = 45,
	C12 = 46,
	C13 = 47,
	C14 = 48,
	C15 = 49,

	D0 = 50,
	D1 = 51,
	D2 = 52,
	D3 = 53,
	D4 = 54,
	D5 = 55,
	D6 = 56,
	D7 = 57,
	D8 = 58,
	D9 = 59,
	D10 = 60,
	D11 = 61,
	D12 = 62,
	D13 = 63,
	D14 = 64,
	D15 = 65,

	E0 = 66,
	E1 = 67,
	E2 = 68,
	E3 = 69,
	E4 = 70,
	E5 = 71,
	E6 = 72,
	E7 = 73,
	E8 = 74,
	E9 = 75,
	E10 = 76,
	E11 = 77,
	E12 = 78,
	E13 = 79,
	E14 = 80,
	E15 = 81,

	F0 = 82,
	F1 = 83,
	F2 = 84,
	F3 = 85,
	F4 = 86,
	F5 = 87,
	F6 = 88,
	F7 = 89,
	F8 = 90,
	F9 = 91,
	F10 = 92,
	F11 = 93,
	F12 = 94,
	F13 = 95,
	F14 = 96,
	F15 = 97,

	G0 = 98,
	G1 = 99,
	G2 = 100,
	G3 = 101,
	G4 = 102,
	G5 = 103,
	G6 = 104,
	G7 = 105,
	G8 = 106,
	G9 = 107,
	G10 = 108,
	G11 = 109,
	G12 = 110,
	G13 = 111,
	G14 = 112,
	G15 = 113,

	H0 = 114,
	H1 = 115,
	H2 = 116,
	H3 = 117,
	H4 = 118,
	H5 = 119,
	H6 = 120,
	H7 = 121,
	H8 = 122,
	H9 = 123,
	H10 = 124,
	H11 = 125,
	H12 = 126,
	H13 = 127,
	H14 = 128,
	H15 = 129,

	/* Used by 176-pin STM32 MCUs */
	I0 = 130,
	I1 = 131,
	I2 = 132,
	I3 = 133,
	I4 = 134,
	I5 = 135,
	I6 = 136,
	I7 = 137,
	I8 = 138,
	I9 = 139,
	I10 = 140,
	I11 = 141,
	I12 = 142,
	I13 = 143,
	I14 = 144,
	I15 = 145,

	/* MC33972 pins go right after on_chip pins */
	MC33972_PIN_1 = 146,
	MC33972_PIN_21 = 166,
	MC33972_PIN_22 = 167,

	TLE8888_PIN_1 = 168,
	TLE8888_PIN_2 = 169,
	TLE8888_PIN_3 = 170,
	TLE8888_PIN_4 = 171,
	TLE8888_PIN_5 = 172,
	TLE8888_PIN_6 = 173,
	TLE8888_PIN_7 = 174,
	TLE8888_PIN_8 = 175,
	TLE8888_PIN_9 = 176,
	TLE8888_PIN_10 = 177,
	TLE8888_PIN_11 = 178,
	TLE8888_PIN_12 = 179,
	TLE8888_PIN_13 = 180,
	TLE8888_PIN_14 = 181,
	TLE8888_PIN_15 = 182,
	TLE8888_PIN_16 = 183,
	TLE8888_PIN_17 = 184,
	TLE8888_PIN_18 = 185,
	TLE8888_PIN_19 = 186,
	TLE8888_PIN_20 = 187,
	TLE8888_PIN_21 = 188,
	TLE8888_PIN_22 = 189,
	TLE8888_PIN_23 = 190,
	TLE8888_PIN_24 = 191,
	TLE8888_PIN_25 = 192,
	TLE8888_PIN_26 = 193,
	TLE8888_PIN_27 = 194,
	TLE8888_PIN_28 = 195,
	TLE8888_PIN_MR = 196,
	TLE8888_PIN_KEY = 197,
	TLE8888_PIN_WAKE = 198,

	/* TLE6240 pins */
	TLE6240_PIN_1 = 199,
	TLE6240_PIN_2 = 200,
	TLE6240_PIN_3 = 201,
	TLE6240_PIN_4 = 202,
	TLE6240_PIN_5 = 203,
	TLE6240_PIN_6 = 204,
	TLE6240_PIN_7 = 205,
	TLE6240_PIN_8 = 206,
	TLE6240_PIN_9 = 207,
	TLE6240_PIN_10 = 208,
	TLE6240_PIN_11 = 209,
	TLE6240_PIN_12 = 210,
	TLE6240_PIN_13 = 211,
	TLE6240_PIN_14 = 212,
	TLE6240_PIN_15 = 213,
	TLE6240_PIN_16 = 214,

	/* L9779 */
	L9779_IGN_1  = 215,
	L9779_IGN_2  = 216,
	L9779_IGN_3  = 217,
	L9779_IGN_4  = 218,
	L9779_OUT_1  = 219,
	L9779_OUT_2  = 220,
	L9779_OUT_3  = 221,
	L9779_OUT_4  = 222,
	L9779_OUT_5  = 223,
	L9779_OUT_6  = 224,
	L9779_OUT_7  = 225,
	L9779_OUT_8  = 226,
	L9779_OUT_9  = 227,
	L9779_OUT_10 = 228,
	L9779_OUT_11 = 229,
	L9779_OUT_12 = 230,
	L9779_OUT_13 = 231,
	L9779_OUT_14 = 232,
	L9779_OUT_15 = 233,
	L9779_OUT_16 = 234,
	L9779_OUT_17 = 235,
	L9779_OUT_18 = 236,
	L9779_OUT_19 = 237,
	L9779_OUT_20 = 238,
	L9779_OUT_A  = 239,
	L9779_OUT_B  = 240,
	L9779_OUT_C  = 241,
	L9779_OUT_D  = 242,
	L9779_OUT_25 = 243,
	L9779_OUT_26 = 244,
	L9779_OUT_27 = 245,
	L9779_OUT_28 = 246,
	L9779_OUT_MRD = 247,
	L9779_PIN_KEY = 248,

	CAN_PIN_0 = 249,
	CAN_PIN_1 = 250,
	CAN_PIN_2 = 251,
	CAN_PIN_3 = 252,
	CAN_PIN_4 = 253,
	CAN_PIN_5 = 254,
	CAN_PIN_6 = 255,
	CAN_PIN_7 = 256,

	PROTECTED_PIN_0  = 257,
	PROTECTED_PIN_1  = 258,
	PROTECTED_PIN_2  = 259,
	PROTECTED_PIN_3  = 260,
	PROTECTED_PIN_4  = 261,
	PROTECTED_PIN_5  = 262,
	PROTECTED_PIN_6  = 263,
	PROTECTED_PIN_7  = 264,
	PROTECTED_PIN_8  = 265,
	PROTECTED_PIN_9  = 266,
	PROTECTED_PIN_10 = 267,
	PROTECTED_PIN_11 = 268,
	PROTECTED_PIN_12 = 269,
	PROTECTED_PIN_13 = 270,
	PROTECTED_PIN_14 = 271,
	PROTECTED_PIN_15 = 272,
};

/* Please keep updating these defines */
#define BRAIN_PIN_ONCHIP_LAST	Gpio::I15
#define BRAIN_PIN_ONCHIP_PINS	(BRAIN_PIN_ONCHIP_LAST - Gpio::A0 + 1)
#define BRAIN_PIN_LAST 			Gpio::PROTECTED_PIN_15
#define BRAIN_PIN_TOTAL_PINS	(BRAIN_PIN_LAST - Gpio::A0 + 1)

/**
 * Frankenso analog #1 PC2 ADC12
 * Frankenso analog #2 PC1 ADC11
 * Frankenso analog #3 PA0 ADC0
 * Frankenso analog #4 PC3 ADC13
 * Frankenso analog #5 PA2 ADC2
 * Frankenso analog #6 PA1 ADC1
 * Frankenso analog #7 PA4 ADC4
 * Frankenso analog #8 PA3 ADC3
 * Frankenso analog #9 PA7 ADC7
 * Frankenso analog #10 PA6 ADC6
 * Frankenso analog #11 PC5 ADC15
 * Frankenso analog #12 PC4 ADC14
 */

typedef enum __attribute__ ((__packed__)) {
	EFI_ADC_NONE = 0,

	EFI_ADC_0 = 1, // PA0
	EFI_ADC_1 = 2, // PA1
	EFI_ADC_2 = 3, // PA2
	EFI_ADC_3 = 4, // PA3
	EFI_ADC_4 = 5, // PA4
	EFI_ADC_5 = 6, // PA5
	EFI_ADC_6 = 7, // PA6
	EFI_ADC_7 = 8, // PA7
	EFI_ADC_8 = 9, // PB0
	EFI_ADC_9 = 10, // PB1
	EFI_ADC_10 = 11, // PC0
	EFI_ADC_11 = 12, // PC1
	EFI_ADC_12 = 13, // PC2
	EFI_ADC_13 = 14, // PC3
	EFI_ADC_14 = 15, // PC4
	EFI_ADC_15 = 16, // PC5
	EFI_ADC_16 = 17,
	EFI_ADC_17 = 18,
	EFI_ADC_18 = 19,
	EFI_ADC_19 = 20,
	EFI_ADC_20 = 21,
	EFI_ADC_21 = 22,
	EFI_ADC_22 = 23,
	EFI_ADC_23 = 24,
	EFI_ADC_24 = 25,
	EFI_ADC_25 = 26,
	EFI_ADC_26 = 27,
	EFI_ADC_27 = 28,
	EFI_ADC_28 = 29,
	EFI_ADC_29 = 30,
	EFI_ADC_30 = 31,
	EFI_ADC_31 = 32,

	EFI_ADC_LAST_CHANNEL = 33,	// Please keep this in sync with the last valid channel index!

	EFI_ADC_ERROR = 50,
} adc_channel_e;
