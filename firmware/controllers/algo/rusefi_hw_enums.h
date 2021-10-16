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

	/* Used by 176-pin STM32 MCUs */
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
} brain_pin_e;

/* Please keep updating these defines */
#define BRAIN_PIN_ONCHIP_LAST	GPIOI_15
#define BRAIN_PIN_ONCHIP_PINS	(BRAIN_PIN_ONCHIP_LAST - GPIOA_0 + 1)
#define BRAIN_PIN_LAST 			TLE8888_PIN_WAKE
#define BRAIN_PIN_TOTAL_PINS	(BRAIN_PIN_LAST - GPIOA_0 + 1)

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

	EFI_ADC_LAST_CHANNEL = 17,	// Please keep this in sync with the last valid channel index!

	EFI_ADC_ERROR = 50,
} adc_channel_e;
