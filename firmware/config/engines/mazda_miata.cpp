/**
 * @file	mazda_miata.cpp
 *
 * FORD_ESCORT_GT = 14
 * set engine_type 14
 * http://rusefi.com/wiki/index.php?title=Vehicle:Mazda_Protege_1993
 *
 * MRE_MIATA_94_MAP = 20
 * MIATA_1996 = 21
 * set engine_type 21
 *
 * @date Apr 11, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "mazda_miata.h"
#include "advance_map.h"
#include "custom_engine.h"


#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
static const uint8_t miataNA8_maf_advance_table[16][16] = { {/*0  engineLoad=1.200*//*0 800.0*/+4, /*1 1213.0*/+11, /*2 1626.0*/
		+23, /*3 2040.0*/+25, /*4 2453.0*/+25, /*5 2866.0*/+25, /*6 3280.0*/+29, /*7 3693.0*/
		+32, /*8 4106.0*/+35, /*9 4520.0*/+37, /*10 4933.0*/+36, /*11 5346.0*/+30, /*12 5760.0*/
		+29, /*13 6173.0*/+29, /*14 6586.0*/+27, /*15 7000.0*/+28 }, {/*1  engineLoad=1.413*//*0 800.0*/
		+4, /*1 1213.0*/+12, /*2 1626.0*/+23, /*3 2040.0*/+25, /*4 2453.0*/+25, /*5 2866.0*/+25, /*6 3280.0*/
		+29, /*7 3693.0*/+33, /*8 4106.0*/+34, /*9 4520.0*/+37, /*10 4933.0*/+35, /*11 5346.0*/
		+30, /*12 5760.0*/+28, /*13 6173.0*/+28, /*14 6586.0*/+27, /*15 7000.0*/+27 }, {/*2  engineLoad=1.626*//*0 800.0*/
		+4, /*1 1213.0*/+12, /*2 1626.0*/+23, /*3 2040.0*/+25, /*4 2453.0*/+25, /*5 2866.0*/+25, /*6 3280.0*/
		+29, /*7 3693.0*/+33, /*8 4106.0*/+34, /*9 4520.0*/+37, /*10 4933.0*/+35, /*11 5346.0*/
		+30, /*12 5760.0*/+29, /*13 6173.0*/+28, /*14 6586.0*/+27, /*15 7000.0*/+26 }, {/*3  engineLoad=1.839*//*0 800.0*/
		+4, /*1 1213.0*/+12, /*2 1626.0*/+23, /*3 2040.0*/+25, /*4 2453.0*/+25, /*5 2866.0*/+25, /*6 3280.0*/
		+29, /*7 3693.0*/+33, /*8 4106.0*/+34, /*9 4520.0*/+36, /*10 4933.0*/+36, /*11 5346.0*/
		+30, /*12 5760.0*/+28, /*13 6173.0*/+28, /*14 6586.0*/+27, /*15 7000.0*/+26 }, {/*4  engineLoad=2.053*//*0 800.0*/
		+4, /*1 1213.0*/+11, /*2 1626.0*/+23, /*3 2040.0*/+25, /*4 2453.0*/+25, /*5 2866.0*/+25, /*6 3280.0*/
		+29, /*7 3693.0*/+32, /*8 4106.0*/+34, /*9 4520.0*/+37, /*10 4933.0*/+35, /*11 5346.0*/
		+29, /*12 5760.0*/+28, /*13 6173.0*/+27, /*14 6586.0*/+28, /*15 7000.0*/+27 }, {/*5  engineLoad=2.266*//*0 800.0*/
		+4, /*1 1213.0*/+11, /*2 1626.0*/+23, /*3 2040.0*/+25, /*4 2453.0*/+25, /*5 2866.0*/+25, /*6 3280.0*/
		+29, /*7 3693.0*/+33, /*8 4106.0*/+34, /*9 4520.0*/+37, /*10 4933.0*/+35, /*11 5346.0*/
		+31, /*12 5760.0*/+28, /*13 6173.0*/+28, /*14 6586.0*/+27, /*15 7000.0*/+26 }, {/*6  engineLoad=2.479*//*0 800.0*/
		+4, /*1 1213.0*/+12, /*2 1626.0*/+23, /*3 2040.0*/+25, /*4 2453.0*/+25, /*5 2866.0*/+25, /*6 3280.0*/
		+29, /*7 3693.0*/+33, /*8 4106.0*/+34, /*9 4520.0*/+36, /*10 4933.0*/+36, /*11 5346.0*/
		+31, /*12 5760.0*/+28, /*13 6173.0*/+28, /*14 6586.0*/+27, /*15 7000.0*/+27 }, {/*7  engineLoad=2.693*//*0 800.0*/
		+4, /*1 1213.0*/+12, /*2 1626.0*/+23, /*3 2040.0*/+25, /*4 2453.0*/+25, /*5 2866.0*/+26, /*6 3280.0*/
		+30, /*7 3693.0*/+33, /*8 4106.0*/+34, /*9 4520.0*/+37, /*10 4933.0*/+36, /*11 5346.0*/
		+30, /*12 5760.0*/+28, /*13 6173.0*/+29, /*14 6586.0*/+27, /*15 7000.0*/+28 }, {/*8  engineLoad=2.906*//*0 800.0*/
		+4, /*1 1213.0*/+12, /*2 1626.0*/+23, /*3 2040.0*/+25, /*4 2453.0*/+25, /*5 2866.0*/+25, /*6 3280.0*/
		+29, /*7 3693.0*/+33, /*8 4106.0*/+34, /*9 4520.0*/+37, /*10 4933.0*/+36, /*11 5346.0*/
		+29, /*12 5760.0*/+28, /*13 6173.0*/+28, /*14 6586.0*/+26, /*15 7000.0*/+28 }, {/*9  engineLoad=3.119997*//*0 800.0*/
		+4, /*1 1213.0*/+11, /*2 1626.0*/+23, /*3 2040.0*/+25, /*4 2453.0*/+25, /*5 2866.0*/+26, /*6 3280.0*/
		+30, /*7 3693.0*/+33, /*8 4106.0*/+34, /*9 4520.0*/+38, /*10 4933.0*/+35, /*11 5346.0*/
		+29, /*12 5760.0*/+29, /*13 6173.0*/+28, /*14 6586.0*/+27, /*15 7000.0*/+29 }, {/*10 engineLoad=3.33333*//*0 800.0*/
		+4, /*1 1213.0*/+12, /*2 1626.0*/+23, /*3 2040.0*/+25, /*4 2453.0*/+25, /*5 2866.0*/+26, /*6 3280.0*/
		+29, /*7 3693.0*/+33, /*8 4106.0*/+34, /*9 4520.0*/+37, /*10 4933.0*/+36, /*11 5346.0*/
		+30, /*12 5760.0*/+29, /*13 6173.0*/+28, /*14 6586.0*/+27, /*15 7000.0*/+27 }, {/*11 engineLoad=3.546663*//*0 800.0*/
		+4, /*1 1213.0*/+12, /*2 1626.0*/+23, /*3 2040.0*/+25, /*4 2453.0*/+25, /*5 2866.0*/+25, /*6 3280.0*/
		+29, /*7 3693.0*/+32, /*8 4106.0*/+34, /*9 4520.0*/+38, /*10 4933.0*/+35, /*11 5346.0*/
		+31, /*12 5760.0*/+28, /*13 6173.0*/+28, /*14 6586.0*/+27, /*15 7000.0*/+27 }, {/*12 engineLoad=3.759996*//*0 800.0*/
		+4, /*1 1213.0*/+12, /*2 1626.0*/+23, /*3 2040.0*/+25, /*4 2453.0*/+25, /*5 2866.0*/+25, /*6 3280.0*/
		+30, /*7 3693.0*/+33, /*8 4106.0*/+34, /*9 4520.0*/+37, /*10 4933.0*/+35, /*11 5346.0*/
		+30, /*12 5760.0*/+28, /*13 6173.0*/+28, /*14 6586.0*/+27, /*15 7000.0*/+26 }, {/*13 engineLoad=3.973329*//*0 800.0*/
		+4, /*1 1213.0*/+11, /*2 1626.0*/+23, /*3 2040.0*/+25, /*4 2453.0*/+25, /*5 2866.0*/+25, /*6 3280.0*/
		+29, /*7 3693.0*/+33, /*8 4106.0*/+34, /*9 4520.0*/+37, /*10 4933.0*/+36, /*11 5346.0*/
		+30, /*12 5760.0*/+28, /*13 6173.0*/+28, /*14 6586.0*/+27, /*15 7000.0*/+27 }, {/*14 engineLoad=4.186662*//*0 800.0*/
		+4, /*1 1213.0*/+11, /*2 1626.0*/+23, /*3 2040.0*/+25, /*4 2453.0*/+25, /*5 2866.0*/+25, /*6 3280.0*/
		+29, /*7 3693.0*/+33, /*8 4106.0*/+34, /*9 4520.0*/+36, /*10 4933.0*/+35, /*11 5346.0*/
		+30, /*12 5760.0*/+28, /*13 6173.0*/+28, /*14 6586.0*/+27, /*15 7000.0*/+26 }, {/*15 engineLoad=4.399*//*0 800.0*/
		+4, /*1 1213.0*/+12, /*2 1626.0*/+24, /*3 2040.0*/+25, /*4 2453.0*/+25, /*5 2866.0*/+25, /*6 3280.0*/
		+29, /*7 3693.0*/+33, /*8 4106.0*/+36, /*9 4520.0*/+37, /*10 4933.0*/+36, /*11 5346.0*/
		+29, /*12 5760.0*/+28, /*13 6173.0*/+28, /*14 6586.0*/+27, /*15 7000.0*/+27 } };
#endif

static void commonMiataNa() {
	engineConfiguration->trigger.type = trigger_type_e::TT_MAZDA_MIATA_NA;
	engineConfiguration->engineChartSize = 100;

	engineConfiguration->triggerInputPins[0] = Gpio::C6; // 2G YEL/BLU
	engineConfiguration->triggerInputPins[1] = Gpio::A5; // 2E White CKP

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;

	engineConfiguration->idle.solenoidFrequency = 160;

	// Frankenstein: high side #1 is PE8
	// Frankenstein: high side #2 is PE10
	// Frankenstein: high side #3 is PE12
	// Frankenstein: high side #4 is PE14
	// Frankenstein: high side #5 is PC9
	// Frankenstein: high side #6 is PC7

	engineConfiguration->ignitionPins[0] = Gpio::E12; // Frankenstein: high side #3
	engineConfiguration->ignitionPins[1] = Gpio::E14; // Frankenstein: high side #4
	engineConfiguration->ignitionPins[2] = Gpio::Unassigned;
	engineConfiguration->ignitionPins[3] = Gpio::Unassigned;

	engineConfiguration->cranking.baseFuel = 24;

	setCommonNTCSensor(&engineConfiguration->clt, 2700);
	setCommonNTCSensor(&engineConfiguration->iat, 2700);
}

void common079721_2351() {

	engineConfiguration->engineChartSize = 300;

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;

	engineConfiguration->fuelPumpPin = Gpio::Unassigned; // fuel pump is not controlled by ECU on this engine

	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	// set injection_mode 2
	engineConfiguration->injectionMode = IM_BATCH;

	// Frankenstein analog input #1: adc1
	// Frankenstein analog input #2: adc3
	// Frankenstein analog input #3: adc13
	// Frankenstein analog input #4: adc11
	// todo: see https://docs.google.com/spreadsheet/ccc?key=0Arl1FeMZcfisdEdGdUlHdWh6cVBoSzFIbkxqa1QtZ3c
	// Frankenstein analog input #5: adc
	// Frankenstein analog input #6: adc
	// Frankenstein analog input #7: adc
	// Frankenstein analog input #8: adc
	// Frankenstein analog input #9: adc
	// Frankenstein analog input #10: adc
	// Frankenstein analog input #11: adc
	// Frankenstein analog input #12: adc
	engineConfiguration->mafAdcChannel = EFI_ADC_1;
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_3;
	engineConfiguration->clt.adcChannel = EFI_ADC_11;

}

/**
 * Tom tomiata, Frankenstein board
 */
void setFrankensteinMiata1996() {
	commonMiataNa();
	engineConfiguration->displacement = 1.839;

#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
	copyTable(config->ignitionTable, miataNA8_maf_advance_table);
#endif

	// upside down
	engineConfiguration->triggerInputPins[0] = Gpio::A5;
	engineConfiguration->triggerInputPins[1] = Gpio::C6;

	engineConfiguration->fuelPumpPin = Gpio::E4;
	engineConfiguration->idle.solenoidPin = Gpio::E5;

	engineConfiguration->mafAdcChannel = EFI_ADC_1;
	engineConfiguration->clt.adcChannel = EFI_ADC_11;
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_13;

	engineConfiguration->ignitionPins[0] = Gpio::E12; // Frankenstein: high side #3
	engineConfiguration->ignitionPins[1] = Gpio::Unassigned;
	engineConfiguration->ignitionPins[2] = Gpio::E14; // Frankenstein: high side #4
	engineConfiguration->ignitionPins[3] = Gpio::Unassigned;

	// harness is sequential but we have a limited board
	engineConfiguration->crankingInjectionMode = IM_BATCH;
	engineConfiguration->injectionMode = IM_BATCH;

	engineConfiguration->injectionPins[0] = Gpio::B9; // Frankenstein: low side - out #12
	engineConfiguration->injectionPins[1] = Gpio::B8; // Frankenstein: low side - out #11
	engineConfiguration->injectionPins[2] = Gpio::Unassigned;
	engineConfiguration->injectionPins[3] = Gpio::Unassigned;
	engineConfiguration->injectionPins[4] = Gpio::Unassigned;
	engineConfiguration->injectionPins[5] = Gpio::Unassigned;
}
