/*
 * @file config/boards/kinetis/rusefi_hw_enums.h
 *
 * @date Jun 2, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 * @author andreika <prometheus.pcb@gmail.com>
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
	A16 = 18,
	A17 = 19,

	B0 = 20,
	B1 = 21,
	B2 = 22,
	B3 = 23,
	B4 = 24,
	B5 = 25,
	B6 = 26,
	B7 = 27,
	B8 = 28,
	B9 = 29,
	B10 = 30,
	B11 = 31,
	B12 = 32,
	B13 = 33,
	B14 = 34,
	B15 = 35,
	B16 = 36,
	B17 = 37,

	C0 = 38,
	C1 = 39,
	C2 = 40,
	C3 = 41,
	C4 = 42,
	C5 = 43,
	C6 = 44,
	C7 = 45,
	C8 = 46,
	C9 = 47,
	C10 = 48,
	C11 = 49,
	C12 = 50,
	C13 = 51,
	C14 = 52,
	C15 = 53,
	C16 = 54,
	C17 = 55,

	D0 = 56,
	D1 = 57,
	D2 = 58,
	D3 = 59,
	D4 = 60,
	D5 = 61,
	D6 = 62,
	D7 = 63,
	D8 = 64,
	D9 = 65,
	D10 = 66,
	D11 = 67,
	D12 = 68,
	D13 = 69,
	D14 = 70,
	D15 = 71,
	D16 = 72,
	D17 = 73,
	
	E0 = 74,
	E1 = 75,
	E2 = 76,
	E3 = 77,
	E4 = 78,
	E5 = 79,
	E6 = 80,
	E7 = 81,
	E8 = 82,
	E9 = 83,
	E10 = 84,
	E11 = 85,
	E12 = 86,
	E13 = 87,
	E14 = 88,
	E15 = 89,
	E16 = 90,
	E17 = 91,

	// TLE6240 pins go right after on chips
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
#define BRAIN_PIN_ONCHIP_LAST	Gpio::E17
#define BRAIN_PIN_ONCHIP_PINS	(BRAIN_PIN_ONCHIP_LAST - Gpio::A0 + 1)
#define BRAIN_PIN_LAST 			Gpio::TLE6240_PIN_16
#define BRAIN_PIN_TOTAL_PINS	(BRAIN_PIN_LAST - Gpio::A0 + 1)

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
