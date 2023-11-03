/*
 * @file mre_meta.h
 *
 * @date Oct 25, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#pragma once

#define MRE_INJ_1 Gpio::TLE8888_PIN_1
#define MRE_INJ_2 Gpio::TLE8888_PIN_2
#define MRE_INJ_3 Gpio::TLE8888_PIN_3
#define MRE_INJ_4 Gpio::TLE8888_PIN_4

// OpenDrain: 4.5A
#define MRE_LS_1 Gpio::TLE8888_PIN_6

// "3 - Lowside 2"
#define MRE_LS_2 Gpio::TLE8888_PIN_5

// clt = "18 - AN temp 1"
// R27 pull-up
#define MRE_IN_ANALOG_TEMP_1 EFI_ADC_0
#define MRE_IN_CLT MRE_IN_ANALOG_TEMP_1

// iat = "23 - AN temp 2"
// R25 pull-up
#define MRE_IN_ANALOG_TEMP_2 EFI_ADC_1
#define MRE_IN_IAT MRE_IN_ANALOG_TEMP_2

// map = "27 - AN volt 1"
#define MRE_IN_ANALOG_VOLT_1 EFI_ADC_10
#define MRE_IN_MAP MRE_IN_ANALOG_VOLT_1

// tps = "20 - AN volt 5" PC3
#define MRE_IN_ANALOG_VOLT_5 EFI_ADC_13
#define MRE_IN_TPS MRE_IN_ANALOG_VOLT_5

// 26 - AN volt 2
#define MRE_IN_ANALOG_VOLT_2 EFI_ADC_6

// 31 - AN volt 3
#define MRE_IN_ANALOG_VOLT_3 EFI_ADC_7

// 32 - AN volt 6
#define MRE_IN_ANALOG_VOLT_6 EFI_ADC_14

// 36 - AN volt 8
#define MRE_IN_ANALOG_VOLT_8 EFI_ADC_8

// 40 - AN volt 9
#define MRE_IN_ANALOG_VOLT_9 EFI_ADC_9

// 28 - AN volt 10
#define MRE_IN_ANALOG_VOLT_10 EFI_ADC_4
