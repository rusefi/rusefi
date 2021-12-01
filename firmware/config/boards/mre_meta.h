/*
 * @file mre_meta.h
 *
 * @date Oct 25, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#pragma once

// clt = "18 - AN temp 1"
#define MRE_IN_ANALOG_TEMP_1 EFI_ADC_0
#define MRE_IN_CLT MRE_IN_ANALOG_TEMP_1

// iat = "23 - AN temp 2"
#define MRE_IN_ANALOG_TEMP_2 EFI_ADC_1
#define MRE_IN_IAT MRE_IN_ANALOG_TEMP_2

// map = "27 - AN volt 1"
#define MRE_IN_ANALOG_VOLT_1 EFI_ADC_10
#define MRE_IN_MAP MRE_IN_ANALOG_VOLT_1

// tps = "20 - AN volt 5" PC3
#define MRE_IN_ANALOG_VOLT_5 EFI_ADC_13
#define MRE_IN_TPS MRE_IN_ANALOG_VOLT_5
