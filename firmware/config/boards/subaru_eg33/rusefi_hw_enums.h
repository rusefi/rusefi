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

	TLE6240_PIN_1 = 146,
	TLE6240_PIN_2 = 147,
	TLE6240_PIN_3 = 148,
	TLE6240_PIN_4 = 149,
	TLE6240_PIN_5 = 150,
	TLE6240_PIN_6 = 151,
	TLE6240_PIN_7 = 152,
	TLE6240_PIN_8 = 153,
	TLE6240_PIN_9 = 154,
	TLE6240_PIN_10 = 155,
	TLE6240_PIN_11 = 156,
	TLE6240_PIN_12 = 157,
	TLE6240_PIN_13 = 158,
	TLE6240_PIN_14 = 159,
	TLE6240_PIN_15 = 160,
	TLE6240_PIN_16 = 161,

	MC33972_PIN_1 = 162,
	MC33972_PIN_2 = 163,
	MC33972_PIN_3 = 164,
	MC33972_PIN_4 = 165,
	MC33972_PIN_5 = 166,
	MC33972_PIN_6 = 167,
	MC33972_PIN_7 = 168,
	MC33972_PIN_8 = 169,
	MC33972_PIN_9 = 170,
	MC33972_PIN_10 = 171,
	MC33972_PIN_11 = 172,
	MC33972_PIN_12 = 173,
	MC33972_PIN_13 = 174,
	MC33972_PIN_14 = 175,
	MC33972_PIN_15 = 176,
	MC33972_PIN_16 = 177,
	MC33972_PIN_17 = 178,
	MC33972_PIN_18 = 179,
	MC33972_PIN_19 = 180,
	MC33972_PIN_20 = 181,
	MC33972_PIN_21 = 182,
	MC33972_PIN_22 = 183,

	MC33810_0_OUT_0 = 184,
	MC33810_0_OUT_1 = 185,
	MC33810_0_OUT_2 = 186,
	MC33810_0_OUT_3 = 187,
	MC33810_0_GD_0  = 188,
	MC33810_0_GD_1  = 189,
	MC33810_0_GD_2  = 190,
	MC33810_0_GD_3  = 191,

	MC33810_1_OUT_0 = 192,
	MC33810_1_OUT_1 = 193,
	MC33810_1_OUT_2 = 194,
	MC33810_1_OUT_3 = 195,
	MC33810_1_GD_0  = 196,
	MC33810_1_GD_1  = 197,
	MC33810_1_GD_2  = 198,
	MC33810_1_GD_3  = 199,

	CAN_PIN_0 = 250,
	CAN_PIN_1 = 251,
	CAN_PIN_2 = 252,
	CAN_PIN_3 = 253,
	CAN_PIN_4 = 254,
	CAN_PIN_5 = 255,
	CAN_PIN_6 = 256,
	CAN_PIN_7 = 257,
};

/* Plase keep updating these defines */
#define BRAIN_PIN_ONCHIP_LAST	Gpio::I15
#define BRAIN_PIN_ONCHIP_PINS	(BRAIN_PIN_ONCHIP_LAST - Gpio::A0 + 1)
#define BRAIN_PIN_LAST 			Gpio::MC33810_1_GD_3
#define BRAIN_PIN_TOTAL_PINS	(BRAIN_PIN_LAST - Gpio::A0 + 1)
/* Plase keep updating these defines */

typedef enum __attribute__ ((__packed__)) {
	EFI_ADC_NONE = 0,

	EFI_ADC_0 = 1,
	EFI_ADC_1 = 2,
	EFI_ADC_2 = 3,
	EFI_ADC_3 = 4,
	EFI_ADC_4 = 5,
	EFI_ADC_5 = 6,
	EFI_ADC_6 = 7,
	EFI_ADC_7 = 8,
	EFI_ADC_8 = 9,
	EFI_ADC_9 = 10,
	EFI_ADC_10 = 11,
	EFI_ADC_11 = 12,
	EFI_ADC_12 = 13,
	EFI_ADC_13 = 14,
	EFI_ADC_14 = 15,
	EFI_ADC_15 = 16,

	EFI_ADC3_4 = 17,			// PF6
	EFI_ADC3_5 = 18,			// PF7
	EFI_ADC3_6 = 19,			// PF8
	EFI_ADC3_7 = 20,			// PF9
	EFI_ADC3_8 = 21,			// PF10
	EFI_ADC3_9 = 22,			// PF3
	EFI_ADC3_14 = 23,			// PF4
	EFI_ADC3_15 = 24,			// PF5

	EFI_ADC_LAST_CHANNEL = 25,	// Please keep this in sync with the last valid channel index!

	EFI_ADC_ERROR = 50,
} adc_channel_e;
