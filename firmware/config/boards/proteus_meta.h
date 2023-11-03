/*
 * proteus_meta.h
 *
 * @date Oct 2, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#pragma once

#define PROTEUS_LS_1 Gpio::D7
// pin 15/black35
#define PROTEUS_LS_2 Gpio::G9

// pin 4/black35
#define PROTEUS_LS_3 Gpio::G10

// pin 16/black35
#define PROTEUS_LS_4 Gpio::G11

// pin 5/black35
#define PROTEUS_LS_5 Gpio::G12

// pin 6/black35
#define PROTEUS_LS_6 Gpio::G13

// pin 7/black35
#define PROTEUS_LS_7 Gpio::G14
#define PROTEUS_LS_8 Gpio::B4
#define PROTEUS_LS_9 Gpio::B5

// pin 20/black35
#define PROTEUS_LS_10 Gpio::B6
#define PROTEUS_LS_11 Gpio::B7
#define PROTEUS_LS_12 Gpio::B8

// pin 10/black35
#define PROTEUS_LS_13 Gpio::B9
#define PROTEUS_LS_14 Gpio::E0
// pin 12/black35
#define PROTEUS_LS_15 Gpio::E1
// pin 23/black35
#define PROTEUS_LS_16 Gpio::E2


// pin 2/black35
#define PROTEUS_HS_1 Gpio::A9

// pin 1/black35
#define PROTEUS_HS_2 Gpio::A8

#define PROTEUS_HS_3 Gpio::D15

#define PROTEUS_HS_4 Gpio::D14

// pin 35/black35
#define PROTEUS_IGN_1 Gpio::D4

// pin 34/black35
#define PROTEUS_IGN_2 Gpio::D3

// pin 22/black35
#define PROTEUS_IGN_3 Gpio::C9
#define PROTEUS_IGN_4 Gpio::C8
#define PROTEUS_IGN_5 Gpio::C7
#define PROTEUS_IGN_6 Gpio::G8
#define PROTEUS_IGN_7 Gpio::G7
#define PROTEUS_IGN_8 Gpio::G6
#define PROTEUS_IGN_9 Gpio::G5
#define PROTEUS_IGN_10 Gpio::G4
#define PROTEUS_IGN_11 Gpio::G3
#define PROTEUS_IGN_12 Gpio::G2

// Analog Temp 1 = PC4
#define PROTEUS_IN_ANALOG_TEMP_1 EFI_ADC_14

// IAT = Analog Temp 2 = PC5
#define PROTEUS_IN_ANALOG_TEMP_2 EFI_ADC_15
#define PROTEUS_IN_IAT PROTEUS_IN_ANALOG_TEMP_2

// CLT = Analog Temp 3 = PB0
#define PROTEUS_IN_ANALOG_TEMP_3 EFI_ADC_8
#define PROTEUS_IN_CLT PROTEUS_IN_ANALOG_TEMP_3

// Analog Temp 4 = PB1
#define PROTEUS_IN_ANALOG_TEMP_4 EFI_ADC_9

// MAP = Analog volt 1 = PC0
#define PROTEUS_IN_ANALOG_VOLT_1 EFI_ADC_10
#define PROTEUS_IN_MAP PROTEUS_IN_ANALOG_VOLT_1

// TPS = Analog volt 2 = PC1
#define PROTEUS_IN_ANALOG_VOLT_2 EFI_ADC_11
#define PROTEUS_IN_TPS PROTEUS_IN_ANALOG_VOLT_2

// PC2
#define PROTEUS_IN_ANALOG_VOLT_3 EFI_ADC_12
#define PROTEUS_IN_TPS1_2 PROTEUS_IN_ANALOG_VOLT_3

// PC3
#define PROTEUS_IN_ANALOG_VOLT_4 EFI_ADC_13
#define PROTEUS_IN_TPS2_1 PROTEUS_IN_ANALOG_VOLT_4

// PA0
#define PROTEUS_IN_ANALOG_VOLT_5 EFI_ADC_0

// PA1
#define PROTEUS_IN_ANALOG_VOLT_6 EFI_ADC_1
// we are not in sync with yaml?! #define PROTEUS_IN_PPS PROTEUS_IN_ANALOG_VOLT_6

// PA2
#define PROTEUS_IN_ANALOG_VOLT_7 EFI_ADC_2
#define PROTEUS_IN_PPS2 PROTEUS_IN_ANALOG_VOLT_7

// PA3
#define PROTEUS_IN_ANALOG_VOLT_8 EFI_ADC_3

// PA4
#define PROTEUS_IN_ANALOG_VOLT_9 EFI_ADC_4


// PA5
#define PROTEUS_IN_ANALOG_VOLT_10 EFI_ADC_5

// PA6
#define PROTEUS_IN_ANALOG_VOLT_11 EFI_ADC_6

#define PROTEUS_VR_1 Gpio::E7
#define PROTEUS_VR_2 Gpio::E8

#define PROTEUS_DIGITAL_1 Gpio::C6
#define PROTEUS_DIGITAL_2 Gpio::E11
#define PROTEUS_DIGITAL_3 Gpio::E12
#define PROTEUS_DIGITAL_4 Gpio::E14
#define PROTEUS_DIGITAL_5 Gpio::E13
#define PROTEUS_DIGITAL_6 Gpio::E15
