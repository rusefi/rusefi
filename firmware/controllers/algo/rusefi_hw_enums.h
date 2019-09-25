/*
 * @file rusefi_hw_enums.h
 *
 * @date Jun 2, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#ifndef RUSEFI_HW_ENUMS_H_
#define RUSEFI_HW_ENUMS_H_

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

	/* TLE6240 pins go right after on chips */
	//#define TLE6240_PIN(n)		((brain_pin_e)((int)BRAIN_PIN_LAST_ONCHIP + 1 + (n)))
	TLE6240_PIN_1 = 130,
	TLE6240_PIN_2 = 131,
	TLE6240_PIN_3 = 132,
	TLE6240_PIN_4 = 133,
	TLE6240_PIN_5 = 134,
	TLE6240_PIN_6 = 135,
	TLE6240_PIN_7 = 136,
	TLE6240_PIN_8 = 137,
	TLE6240_PIN_9 = 138,
	TLE6240_PIN_10 = 139,
	TLE6240_PIN_11 = 140,
	TLE6240_PIN_12 = 141,
	TLE6240_PIN_13 = 142,
	TLE6240_PIN_14 = 143,
	TLE6240_PIN_15 = 144,
	TLE6240_PIN_16 = 145,

	/* MC33972 pins go right after TLE6240 */
	//#define MC33972_PIN(n)		((brain_pin_e)((int)BRAIN_PIN_LAST_ONCHIP + 1 + 16 + (n)))
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
} brain_pin_e;

/* Plase keep updating this define */
#define BRAIN_PIN_LAST_ONCHIP	GPIOH_15

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
	EFI_ADC_0 = 0, // PA0
	EFI_ADC_1 = 1, // PA1
	EFI_ADC_2 = 2, // PA2
	EFI_ADC_3 = 3, // PA3
	EFI_ADC_4 = 4, // PA4
	EFI_ADC_5 = 5, // PA5
	EFI_ADC_6 = 6, // PA6
	EFI_ADC_7 = 7, // PA7
	EFI_ADC_8 = 8, // PB0
	EFI_ADC_9 = 9, // PB1
	EFI_ADC_10 = 10, // PC0
	EFI_ADC_11 = 11, // PC1
	EFI_ADC_12 = 12, // PC2
	EFI_ADC_13 = 13, // PC3
	EFI_ADC_14 = 14, // PC4
	EFI_ADC_15 = 15, // PC5

	// todo: bad choice of value since now we have ADC_CHANNEL_SENSOR and could end up with 17 and 18 also
	EFI_ADC_NONE = 16,
	EFI_ADC_ERROR = 17,
#if EFI_UNIT_TEST
	/**
	 * these values are unfortunately visible to BoardReader
	 * and TunerStudio would need these ordinals to fit into field size
	 */
    TEST_MAF_CHANNEL = 18,
#endif /* EFI_UNIT_TEST */
} adc_channel_e;

#define INCOMPATIBLE_CONFIG_CHANGE EFI_ADC_0

#endif /* RUSEFI_HW_ENUMS_H_ */
