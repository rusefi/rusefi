/**
 * @file	mazda_miata.cpp
 *
 * FORD_ESCORT_GT = 14
 * set engine_type 14
 * http://rusefi.com/wiki/index.php?title=Vehicle:Mazda_Protege_1993
 *
 * MIATA_1990 = 19 (Frankenstein board)
 * MRE_MIATA_94_MAP = 20
 * MIATA_1996 = 21
 * set engine_type 21
 *
 * @date Apr 11, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "global.h"
#include "fsio_impl.h"
#include "mazda_miata.h"
#include "engine_math.h"
#include "advance_map.h"
#include "allsensors.h"
#include "custom_engine.h"


#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
static const ignition_table_t miataNA8_maf_advance_table = { {/*0  engineLoad=1.200*//*0 800.0*/+4.498, /*1 1213.0*/+11.905, /*2 1626.0*/
		+23.418, /*3 2040.0*/+25.357, /*4 2453.0*/+25.441, /*5 2866.0*/+25.468, /*6 3280.0*/+29.425, /*7 3693.0*/
		+32.713, /*8 4106.0*/+35.556, /*9 4520.0*/+37.594, /*10 4933.0*/+36.165, /*11 5346.0*/+30.578, /*12 5760.0*/
		+29.145, /*13 6173.0*/+29.065, /*14 6586.0*/+27.071, /*15 7000.0*/+28.282 }, {/*1  engineLoad=1.413*//*0 800.0*/
		+4.87, /*1 1213.0*/+12.138, /*2 1626.0*/+23.389, /*3 2040.0*/+25.501, /*4 2453.0*/+25.441, /*5 2866.0*/+25.468, /*6 3280.0*/
		+29.125, /*7 3693.0*/+33.074, /*8 4106.0*/+34.203, /*9 4520.0*/+37.769, /*10 4933.0*/+35.899, /*11 5346.0*/
		+30.519, /*12 5760.0*/+28.88, /*13 6173.0*/+28.74, /*14 6586.0*/+27.189, /*15 7000.0*/+27.826 }, {/*2  engineLoad=1.626*//*0 800.0*/
		+4.817, /*1 1213.0*/+12.262, /*2 1626.0*/+23.925, /*3 2040.0*/+25.501, /*4 2453.0*/+25.5, /*5 2866.0*/+25.468, /*6 3280.0*/
		+29.364, /*7 3693.0*/+33.489, /*8 4106.0*/+34.839, /*9 4520.0*/+37.545, /*10 4933.0*/+35.875, /*11 5346.0*/
		+30.353, /*12 5760.0*/+29.052, /*13 6173.0*/+28.37, /*14 6586.0*/+27.072, /*15 7000.0*/+26.828 }, {/*3  engineLoad=1.839*//*0 800.0*/
		+4.537, /*1 1213.0*/+12.421, /*2 1626.0*/+23.214, /*3 2040.0*/+25.394, /*4 2453.0*/+25.412, /*5 2866.0*/+25.485, /*6 3280.0*/
		+29.425, /*7 3693.0*/+33.427, /*8 4106.0*/+34.091, /*9 4520.0*/+36.887, /*10 4933.0*/+36.047, /*11 5346.0*/
		+30.079, /*12 5760.0*/+28.453, /*13 6173.0*/+28.074, /*14 6586.0*/+27.189, /*15 7000.0*/+26.641 }, {/*4  engineLoad=2.053*//*0 800.0*/
		+4.522, /*1 1213.0*/+11.76, /*2 1626.0*/+23.915, /*3 2040.0*/+25.415, /*4 2453.0*/+25.551, /*5 2866.0*/+25.14, /*6 3280.0*/
		+29.346, /*7 3693.0*/+32.917, /*8 4106.0*/+34.815, /*9 4520.0*/+37.211, /*10 4933.0*/+35.817, /*11 5346.0*/
		+29.694, /*12 5760.0*/+28.799, /*13 6173.0*/+27.818, /*14 6586.0*/+28.098, /*15 7000.0*/+27.662 }, {/*5  engineLoad=2.266*//*0 800.0*/
		+4.678, /*1 1213.0*/+11.912, /*2 1626.0*/+23.486, /*3 2040.0*/+25.379, /*4 2453.0*/+25.551, /*5 2866.0*/+25.527, /*6 3280.0*/
		+29.856, /*7 3693.0*/+33.511, /*8 4106.0*/+34.786, /*9 4520.0*/+37.963, /*10 4933.0*/+35.917, /*11 5346.0*/
		+31.073, /*12 5760.0*/+28.361, /*13 6173.0*/+28.468, /*14 6586.0*/+27.188, /*15 7000.0*/+26.729 }, {/*6  engineLoad=2.479*//*0 800.0*/
		+4.517, /*1 1213.0*/+12.029, /*2 1626.0*/+23.477, /*3 2040.0*/+25.455, /*4 2453.0*/+25.382, /*5 2866.0*/+25.898, /*6 3280.0*/
		+29.147, /*7 3693.0*/+33.578, /*8 4106.0*/+34.12, /*9 4520.0*/+36.279, /*10 4933.0*/+36.432, /*11 5346.0*/
		+31.362, /*12 5760.0*/+28.084, /*13 6173.0*/+28.463, /*14 6586.0*/+27.691, /*15 7000.0*/+27.83 }, {/*7  engineLoad=2.693*//*0 800.0*/
		+4.532, /*1 1213.0*/+12.262, /*2 1626.0*/+23.935, /*3 2040.0*/+25.489, /*4 2453.0*/+25.595, /*5 2866.0*/+26.816, /*6 3280.0*/
		+30.251, /*7 3693.0*/+33.533, /*8 4106.0*/+34.794, /*9 4520.0*/+37.882, /*10 4933.0*/+36.104, /*11 5346.0*/
		+30.079, /*12 5760.0*/+28.545, /*13 6173.0*/+29.304, /*14 6586.0*/+27.07, /*15 7000.0*/+28.324 }, {/*8  engineLoad=2.906*//*0 800.0*/
		+4.532, /*1 1213.0*/+12.036, /*2 1626.0*/+23.418, /*3 2040.0*/+25.513, /*4 2453.0*/+25.382, /*5 2866.0*/+25.357, /*6 3280.0*/
		+29.934, /*7 3693.0*/+33.467, /*8 4106.0*/+34.748, /*9 4520.0*/+37.288, /*10 4933.0*/+36.38, /*11 5346.0*/
		+29.516, /*12 5760.0*/+28.799, /*13 6173.0*/+28.407, /*14 6586.0*/+26.951, /*15 7000.0*/+28.203 }, {/*9  engineLoad=3.119997*//*0 800.0*/
		+4.532, /*1 1213.0*/+11.978, /*2 1626.0*/+23.73, /*3 2040.0*/+25.501, /*4 2453.0*/+25.624, /*5 2866.0*/+26.328, /*6 3280.0*/
		+30.015, /*7 3693.0*/+33.187, /*8 4106.0*/+34.881, /*9 4520.0*/+38.044, /*10 4933.0*/+35.81, /*11 5346.0*/
		+29.843, /*12 5760.0*/+29.306, /*13 6173.0*/+28.997, /*14 6586.0*/+27.109, /*15 7000.0*/+29.339 }, {/*10 engineLoad=3.33333*//*0 800.0*/
		+4.527, /*1 1213.0*/+12.131, /*2 1626.0*/+23.486, /*3 2040.0*/+25.43, /*4 2453.0*/+25.551, /*5 2866.0*/+26.276, /*6 3280.0*/
		+29.639, /*7 3693.0*/+33.005, /*8 4106.0*/+34.253, /*9 4520.0*/+37.788, /*10 4933.0*/+36.077, /*11 5346.0*/
		+30.188, /*12 5760.0*/+29.087, /*13 6173.0*/+28.481, /*14 6586.0*/+27.348, /*15 7000.0*/+27.777 }, {/*11 engineLoad=3.546663*//*0 800.0*/
		+4.889, /*1 1213.0*/+12.175, /*2 1626.0*/+23.271, /*3 2040.0*/+25.357, /*4 2453.0*/+25.551, /*5 2866.0*/+25.485, /*6 3280.0*/
		+29.899, /*7 3693.0*/+32.802, /*8 4106.0*/+34.786, /*9 4520.0*/+38.686, /*10 4933.0*/+35.722, /*11 5346.0*/
		+31.347, /*12 5760.0*/+28.891, /*13 6173.0*/+28.333, /*14 6586.0*/+27.149, /*15 7000.0*/+27.236 }, {/*12 engineLoad=3.759996*//*0 800.0*/
		+4.537, /*1 1213.0*/+12.073, /*2 1626.0*/+23.896, /*3 2040.0*/+25.525, /*4 2453.0*/+25.595, /*5 2866.0*/+25.451, /*6 3280.0*/
		+30.428, /*7 3693.0*/+33.714, /*8 4106.0*/+34.08, /*9 4520.0*/+37.526, /*10 4933.0*/+35.817, /*11 5346.0*/
		+30.733, /*12 5760.0*/+28.718, /*13 6173.0*/+28.518, /*14 6586.0*/+27.518, /*15 7000.0*/+26.561 }, {/*13 engineLoad=3.973329*//*0 800.0*/
		+4.86, /*1 1213.0*/+11.883, /*2 1626.0*/+23.428, /*3 2040.0*/+25.489, /*4 2453.0*/+25.536, /*5 2866.0*/+25.613, /*6 3280.0*/
		+29.895, /*7 3693.0*/+33.648, /*8 4106.0*/+34.758, /*9 4520.0*/+37.988, /*10 4933.0*/+36.047, /*11 5346.0*/
		+30.225, /*12 5760.0*/+28.698, /*13 6173.0*/+28.487, /*14 6586.0*/+27.111, /*15 7000.0*/+27.708 }, {/*14 engineLoad=4.186662*//*0 800.0*/
		+4.683, /*1 1213.0*/+11.898, /*2 1626.0*/+23.506, /*3 2040.0*/+25.562, /*4 2453.0*/+25.61, /*5 2866.0*/+25.519, /*6 3280.0*/
		+29.95, /*7 3693.0*/+33.582, /*8 4106.0*/+34.548, /*9 4520.0*/+36.201, /*10 4933.0*/+35.788, /*11 5346.0*/
		+30.053, /*12 5760.0*/+28.292, /*13 6173.0*/+28.259, /*14 6586.0*/+27.269, /*15 7000.0*/+26.863 }, {/*15 engineLoad=4.399*//*0 800.0*/
		+4.85, /*1 1213.0*/+12.24, /*2 1626.0*/+24.091, /*3 2040.0*/+25.394, /*4 2453.0*/+25.323, /*5 2866.0*/+25.544, /*6 3280.0*/
		+29.915, /*7 3693.0*/+33.104, /*8 4106.0*/+36.016, /*9 4520.0*/+37.933, /*10 4933.0*/+36.254, /*11 5346.0*/
		+29.712, /*12 5760.0*/+28.651, /*13 6173.0*/+28.045, /*14 6586.0*/+27.228, /*15 7000.0*/+27.784 } };
#endif

EXTERN_ENGINE;

static void commonMiataNa(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->trigger.type = TT_MAZDA_MIATA_NA;
	engineConfiguration->engineChartSize = 100;

	engineConfiguration->triggerInputPins[0] = GPIOC_6; // 2G YEL/BLU
	engineConfiguration->triggerInputPins[1] = GPIOA_5; // 2E White CKP

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;

	engineConfiguration->idle.solenoidFrequency = 160;

	// Frankenstein: high side #1 is PE8
	// Frankenstein: high side #2 is PE10
	// Frankenstein: high side #3 is PE12
	// Frankenstein: high side #4 is PE14
	// Frankenstein: high side #5 is PC9
	// Frankenstein: high side #6 is PC7

	engineConfiguration->ignitionPins[0] = GPIOE_12; // Frankenstein: high side #3
	engineConfiguration->ignitionPins[1] = GPIOE_14; // Frankenstein: high side #4
	engineConfiguration->ignitionPins[2] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPinMode = OM_DEFAULT;

	engineConfiguration->cranking.baseFuel = 24;

	engineConfiguration->triggerSimulatorPinModes[0] = OM_OPENDRAIN;
	engineConfiguration->triggerSimulatorPinModes[1] = OM_OPENDRAIN;

	setCommonNTCSensor(&engineConfiguration->clt, 2700);
	setCommonNTCSensor(&engineConfiguration->iat, 2700);
}

void common079721_2351(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setDefaultFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);

	engineConfiguration->engineChartSize = 300;

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	engineConfiguration->fuelPumpPin = GPIO_UNASSIGNED; // fuel pump is not controlled by ECU on this engine

	// set cranking_injection_mode 0
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
 * Frankenstein board
 */
void setMiata1990(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	common079721_2351(PASS_CONFIG_PARAMETER_SIGNATURE);

	commonMiataNa(PASS_CONFIG_PARAMETER_SIGNATURE);

	// Frankenstein: low side - out #1: PC14
	// Frankenstein: low side - out #2: PC15
	// Frankenstein: low side - out #3: PE6
	// Frankenstein: low side - out #4: PC13
	// Frankenstein: low side - out #5: PE4
	// Frankenstein: low side - out #6: PE5
	// Frankenstein: low side - out #7: PE2
	// Frankenstein: low side - out #8: PE3
	// Frankenstein: low side - out #9: PE0
	// Frankenstein: low side - out #10: PE1
	// Frankenstein: low side - out #11: PB8
	// Frankenstein: low side - out #12: PB9

	engineConfiguration->injectionPins[0] = GPIOB_9; // Frankenstein: low side - out #12
	engineConfiguration->injectionPins[1] = GPIOB_8; // Frankenstein: low side - out #11
	engineConfiguration->injectionPins[2] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[3] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[5] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPinMode = OM_DEFAULT;

// todo: idleValvePin
}


static void setMiata1994_common(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	commonMiataNa(PASS_CONFIG_PARAMETER_SIGNATURE);
	engineConfiguration->specs.displacement = 1.839;

	// set cranking_timing_angle 0
	engineConfiguration->crankingTimingAngle = 0;

	engineConfiguration->crankingChargeAngle = 70;

#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
	MEMCPY(config->ignitionTable, miataNA8_maf_advance_table);
#endif

//	engineConfiguration->triggerSimulatorPins[0] = GPIOD_2; // 2G - YEL/BLU
//	engineConfiguration->triggerSimulatorPins[1] = GPIOB_3; // 2E - WHT - four times
//	engineConfiguration->triggerSimulatorPinModes[0] = OM_OPENDRAIN;
//	engineConfiguration->triggerSimulatorPinModes[1] = OM_OPENDRAIN;
//
//	engineConfiguration->triggerInputPins[0] = GPIO_UNASSIGNED;
//	engineConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
//
//	engineConfiguration->is_enabled_spi_1 = false;
//	engineConfiguration->is_enabled_spi_2 = false;
//	engineConfiguration->is_enabled_spi_3 = false;

	/**
	 * Outputs
	 */
	// Frankenso low out #: PE6
	// Frankenso low out #: PE5
	// Frankenso low out #:
	// Frankenso low out #:
	// Frankenso low out #5: PE3
	// Frankenso low out #6: PE4
	// Frankenso low out #7: PE1 (do not use with discovery!)
	// Frankenso low out #8:
	// Frankenso low out #9: PB9
	// Frankenso low out #10: PE0 (do not use with discovery!)
	// Frankenso low out #11: PB8
	// Frankenso low out #12: PB7
	engineConfiguration->fanPin = GPIOE_6;

	engineConfiguration->o2heaterPin = GPIO_UNASSIGNED;

	engineConfiguration->fuelPumpPin = GPIOE_4;

	engineConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[5] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPinMode = OM_DEFAULT;

	engineConfiguration->idle.solenoidPin = GPIOB_9;

	engineConfiguration->ignitionPins[0] = GPIOE_14; // Frankenso high side - pin 1G
	engineConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[2] = GPIOC_7; // Frankenso high side - pin 1H
	engineConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPinMode = OM_DEFAULT;

	setFrankenso_01_LCD(engineConfiguration);

	commonFrankensoAnalogInputs(engineConfiguration);

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_2; // PA2
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;
	engineConfiguration->mafAdcChannel = EFI_ADC_0;
	engineConfiguration->clt.adcChannel = EFI_ADC_12;
	engineConfiguration->iat.adcChannel = EFI_ADC_11;
// end of 1994 commond
}

/**
 * Tom tomiata, Frankenstein board
 */
void setMiata1996(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	commonMiataNa(PASS_CONFIG_PARAMETER_SIGNATURE);
	engineConfiguration->specs.displacement = 1.839;

#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
	MEMCPY(config->ignitionTable, miataNA8_maf_advance_table);
#endif

	// upside down
	engineConfiguration->triggerInputPins[0] = GPIOA_5;
	engineConfiguration->triggerInputPins[1] = GPIOC_6;

	engineConfiguration->fuelPumpPin = GPIOE_4;
	engineConfiguration->idle.solenoidPin = GPIOE_5;

	engineConfiguration->mafAdcChannel = EFI_ADC_1;
	engineConfiguration->clt.adcChannel = EFI_ADC_11;
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_13;

	engineConfiguration->ignitionPins[0] = GPIOE_12; // Frankenstein: high side #3
	engineConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[2] = GPIOE_14; // Frankenstein: high side #4
	engineConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPinMode = OM_DEFAULT;

	// harness is sequential but we have a limited board
	engineConfiguration->crankingInjectionMode = IM_BATCH;
	engineConfiguration->injectionMode = IM_BATCH;

	engineConfiguration->injectionPins[0] = GPIOB_9; // Frankenstein: low side - out #12
	engineConfiguration->injectionPins[1] = GPIOB_8; // Frankenstein: low side - out #11
	engineConfiguration->injectionPins[2] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[3] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[5] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPinMode = OM_DEFAULT;
}
