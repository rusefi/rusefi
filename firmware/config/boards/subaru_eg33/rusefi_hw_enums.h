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
typedef enum __attribute__ ((__packed__)) {
	GPIO_UNASSIGNED = 0,
	GPIO_INVALID = 1,

	GPIOA_0 = 2,
	GPIOA_1 = 3,
	GPIOA_2 = 4,
	GPIOA_3 = 5,
	GPIOA_4 = 6,
	GPIOA_5 = 7,
	GPIOA_6 = 8,
	GPIOA_7 = 9,
	GPIOA_8 = 10,
	GPIOA_9 = 11,
	GPIOA_10 = 12,
	GPIOA_11 = 13,
	GPIOA_12 = 14,
	GPIOA_13 = 15,
	GPIOA_14 = 16,
	GPIOA_15 = 17,

	GPIOB_0 = 18,
	GPIOB_1 = 19,
	GPIOB_2 = 20,
	GPIOB_3 = 21,
	GPIOB_4 = 22,
	GPIOB_5 = 23,
	GPIOB_6 = 24,
	GPIOB_7 = 25,
	GPIOB_8 = 26,
	GPIOB_9 = 27,
	GPIOB_10 = 28,
	GPIOB_11 = 29,
	GPIOB_12 = 30,
	GPIOB_13 = 31,
	GPIOB_14 = 32,
	GPIOB_15 = 33,

	GPIOC_0 = 34,
	GPIOC_1 = 35,
	GPIOC_2 = 36,
	GPIOC_3 = 37,
	GPIOC_4 = 38,
	GPIOC_5 = 39,
	GPIOC_6 = 40,
	GPIOC_7 = 41,
	GPIOC_8 = 42,
	GPIOC_9 = 43,
	GPIOC_10 = 44,
	GPIOC_11 = 45,
	GPIOC_12 = 46,
	GPIOC_13 = 47,
	GPIOC_14 = 48,
	GPIOC_15 = 49,

	GPIOD_0 = 50,
	GPIOD_1 = 51,
	GPIOD_2 = 52,
	GPIOD_3 = 53,
	GPIOD_4 = 54,
	GPIOD_5 = 55,
	GPIOD_6 = 56,
	GPIOD_7 = 57,
	GPIOD_8 = 58,
	GPIOD_9 = 59,
	GPIOD_10 = 60,
	GPIOD_11 = 61,
	GPIOD_12 = 62,
	GPIOD_13 = 63,
	GPIOD_14 = 64,
	GPIOD_15 = 65,

	GPIOE_0 = 66,
	GPIOE_1 = 67,
	GPIOE_2 = 68,
	GPIOE_3 = 69,
	GPIOE_4 = 70,
	GPIOE_5 = 71,
	GPIOE_6 = 72,
	GPIOE_7 = 73,
	GPIOE_8 = 74,
	GPIOE_9 = 75,
	GPIOE_10 = 76,
	GPIOE_11 = 77,
	GPIOE_12 = 78,
	GPIOE_13 = 79,
	GPIOE_14 = 80,
	GPIOE_15 = 81,

	GPIOF_0 = 82,
	GPIOF_1 = 83,
	GPIOF_2 = 84,
	GPIOF_3 = 85,
	GPIOF_4 = 86,
	GPIOF_5 = 87,
	GPIOF_6 = 88,
	GPIOF_7 = 89,
	GPIOF_8 = 90,
	GPIOF_9 = 91,
	GPIOF_10 = 92,
	GPIOF_11 = 93,
	GPIOF_12 = 94,
	GPIOF_13 = 95,
	GPIOF_14 = 96,
	GPIOF_15 = 97,

	GPIOG_0 = 98,
	GPIOG_1 = 99,
	GPIOG_2 = 100,
	GPIOG_3 = 101,
	GPIOG_4 = 102,
	GPIOG_5 = 103,
	GPIOG_6 = 104,
	GPIOG_7 = 105,
	GPIOG_8 = 106,
	GPIOG_9 = 107,
	GPIOG_10 = 108,
	GPIOG_11 = 109,
	GPIOG_12 = 110,
	GPIOG_13 = 111,
	GPIOG_14 = 112,
	GPIOG_15 = 113,

	GPIOH_0 = 114,
	GPIOH_1 = 115,
	GPIOH_2 = 116,
	GPIOH_3 = 117,
	GPIOH_4 = 118,
	GPIOH_5 = 119,
	GPIOH_6 = 120,
	GPIOH_7 = 121,
	GPIOH_8 = 122,
	GPIOH_9 = 123,
	GPIOH_10 = 124,
	GPIOH_11 = 125,
	GPIOH_12 = 126,
	GPIOH_13 = 127,
	GPIOH_14 = 128,
	GPIOH_15 = 129,

	GPIOI_0 = 130,
	GPIOI_1 = 131,
	GPIOI_2 = 132,
	GPIOI_3 = 133,
	GPIOI_4 = 134,
	GPIOI_5 = 135,
	GPIOI_6 = 136,
	GPIOI_7 = 137,
	GPIOI_8 = 138,
	GPIOI_9 = 139,
	GPIOI_10 = 140,
	GPIOI_11 = 141,
	GPIOI_12 = 142,
	GPIOI_13 = 143,
	GPIOI_14 = 144,
	GPIOI_15 = 145,

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
} brain_pin_e;

/* Plase keep updating these defines */
#define BRAIN_PIN_ONCHIP_LAST	GPIOI_15
#define BRAIN_PIN_ONCHIP_PINS	(BRAIN_PIN_ONCHIP_LAST - GPIOA_0 + 1)
#define BRAIN_PIN_LAST 			MC33810_1_GD_3
#define BRAIN_PIN_TOTAL_PINS	(BRAIN_PIN_LAST - GPIOA_0 + 1)
/* Plase keep updating these defines */

/* diagnostic for brain pins
 * can be combination of few bits
 * defined as bit mask */
typedef enum __attribute__ ((__packed__))
{
	PIN_OK = 0,
	PIN_OPEN = 0x01,
	PIN_SHORT_TO_GND = 0x02,
	PIN_SHORT_TO_BAT = 0x04,
	PIN_OVERLOAD =	0x08,
	PIN_DRIVER_OVERTEMP = 0x10,
	PIN_INVALID = 0x80
} brain_pin_diag_e;

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
