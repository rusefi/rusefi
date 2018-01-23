/**
 * @file	mazda_miata.cpp
 *
 * FORD_ESCORT_GT = 14
 * set engine_type 14
 * http://rusefi.com/wiki/index.php?title=Vehicle:Mazda_Protege_1993
 *
 * MIATA_1990 = 19 (Frankenstein board)
 * MIATA_1994_DEVIATOR = 20
 * MIATA_1996 = 21
 * MIATA_1994_SPAGS = 24
 * set engine_type 24
 *
 * @date Apr 11, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "fsio_impl.h"
#include "mazda_miata.h"
#include "engine_math.h"
#include "advance_map.h"
#include "allsensors.h"
#include "custom_engine.h"

static const fuel_table_t miata_maf_fuel_table = { {/*0 engineLoad=1.2*//*0 800.0*/1.53, /*1 1213.0*/0.92, /*2 1626.0*/
		0.74, /*3 2040.0*/0.69, /*4 2453.0*/0.69, /*5 2866.0*/0.67, /*6 3280.0*/0.67, /*7 3693.0*/0.67, /*8 4106.0*/
		0.67, /*9 4520.0*/1.02, /*10 4933.0*/0.98, /*11 5346.0*/0.98, /*12 5760.0*/0.92, /*13 6173.0*/0.89, /*14 6586.0*/
		0.82, /*15 7000.0*/0.87 }, {/*1 engineLoad=1.413333*//*0 800.0*/2.98, /*1 1213.0*/2.07, /*2 1626.0*/1.74, /*3 2040.0*/
		1.55, /*4 2453.0*/1.43, /*5 2866.0*/1.18, /*6 3280.0*/0.0, /*7 3693.0*/0.0, /*8 4106.0*/0.0, /*9 4520.0*/0.0, /*10 4933.0*/
		0.0, /*11 5346.0*/0.0, /*12 5760.0*/0.0, /*13 6173.0*/0.0, /*14 6586.0*/0.0, /*15 7000.0*/0.0 }, {/*2 engineLoad=1.626666*//*0 800.0*/
		4.9, /*1 1213.0*/3.45, /*2 1626.0*/2.76, /*3 2040.0*/2.35, /*4 2453.0*/2.08, /*5 2866.0*/1.84, /*6 3280.0*/0.0, /*7 3693.0*/
		0.0, /*8 4106.0*/0.0, /*9 4520.0*/0.0, /*10 4933.0*/0.0, /*11 5346.0*/0.0, /*12 5760.0*/0.0, /*13 6173.0*/0.0, /*14 6586.0*/
		0.0, /*15 7000.0*/0.0 }, {/*3 engineLoad=1.839999*//*0 800.0*/6.94, /*1 1213.0*/4.81, /*2 1626.0*/3.79, /*3 2040.0*/
		3.14, /*4 2453.0*/2.72, /*5 2866.0*/2.42, /*6 3280.0*/1.93, /*7 3693.0*/1.8, /*8 4106.0*/0.0, /*9 4520.0*/0.0, /*10 4933.0*/
		0.0, /*11 5346.0*/0.0, /*12 5760.0*/0.0, /*13 6173.0*/0.0, /*14 6586.0*/0.0, /*15 7000.0*/0.0 }, {/*4 engineLoad=2.053332*//*0 800.0*/
		11.94, /*1 1213.0*/6.67, /*2 1626.0*/5.15, /*3 2040.0*/4.23, /*4 2453.0*/3.63, /*5 2866.0*/3.19, /*6 3280.0*/
		2.83, /*7 3693.0*/2.59, /*8 4106.0*/2.39, /*9 4520.0*/2.24, /*10 4933.0*/0.0, /*11 5346.0*/0.0, /*12 5760.0*/
		0.0, /*13 6173.0*/0.0, /*14 6586.0*/0.0, /*15 7000.0*/0.0 }, {/*5 engineLoad=2.266665*//*0 800.0*/12.63, /*1 1213.0*/
		10.51, /*2 1626.0*/6.91, /*3 2040.0*/5.63, /*4 2453.0*/4.8, /*5 2866.0*/4.21, /*6 3280.0*/3.75, /*7 3693.0*/3.4, /*8 4106.0*/
		3.09, /*9 4520.0*/2.87, /*10 4933.0*/2.8, /*11 5346.0*/2.68, /*12 5760.0*/2.52, /*13 6173.0*/2.38, /*14 6586.0*/
		2.29, /*15 7000.0*/0.0 }, {/*6 engineLoad=2.479998*//*0 800.0*/12.11, /*1 1213.0*/12.5, /*2 1626.0*/10.04, /*3 2040.0*/
		7.24, /*4 2453.0*/6.11, /*5 2866.0*/5.32, /*6 3280.0*/4.73, /*7 3693.0*/4.28, /*8 4106.0*/3.89, /*9 4520.0*/
		3.61, /*10 4933.0*/3.54, /*11 5346.0*/3.32, /*12 5760.0*/3.13, /*13 6173.0*/2.92, /*14 6586.0*/2.82, /*15 7000.0*/
		0.0 }, {/*7 engineLoad=2.693331*//*0 800.0*/12.09, /*1 1213.0*/12.19, /*2 1626.0*/12.43, /*3 2040.0*/10.69, /*4 2453.0*/
		7.92, /*5 2866.0*/6.87, /*6 3280.0*/6.09, /*7 3693.0*/5.49, /*8 4106.0*/4.97, /*9 4520.0*/4.59, /*10 4933.0*/
		4.44, /*11 5346.0*/4.21, /*12 5760.0*/3.97, /*13 6173.0*/3.7, /*14 6586.0*/3.55, /*15 7000.0*/0.0 }, {/*8 engineLoad=2.906664*//*0 800.0*/
		12.09, /*1 1213.0*/12.18, /*2 1626.0*/12.34, /*3 2040.0*/12.48, /*4 2453.0*/11.36, /*5 2866.0*/9.48, /*6 3280.0*/
		7.68, /*7 3693.0*/6.89, /*8 4106.0*/6.23, /*9 4520.0*/5.76, /*10 4933.0*/5.65, /*11 5346.0*/5.32, /*12 5760.0*/
		4.97, /*13 6173.0*/4.6, /*14 6586.0*/4.4, /*15 7000.0*/0.0 }, {/*9 engineLoad=3.119997*//*0 800.0*/12.08, /*1 1213.0*/
		12.17, /*2 1626.0*/12.34, /*3 2040.0*/12.41, /*4 2453.0*/12.76, /*5 2866.0*/12.47, /*6 3280.0*/11.02, /*7 3693.0*/
		9.67, /*8 4106.0*/8.29, /*9 4520.0*/7.23, /*10 4933.0*/7.0, /*11 5346.0*/6.82, /*12 5760.0*/6.44, /*13 6173.0*/
		6.06, /*14 6586.0*/5.76, /*15 7000.0*/0.0 }, {/*10 engineLoad=3.33333*//*0 800.0*/12.08, /*1 1213.0*/12.18, /*2 1626.0*/
		12.34, /*3 2040.0*/12.43, /*4 2453.0*/12.7, /*5 2866.0*/12.93, /*6 3280.0*/12.78, /*7 3693.0*/12.4, /*8 4106.0*/
		11.4, /*9 4520.0*/10.16, /*10 4933.0*/9.44, /*11 5346.0*/9.01, /*12 5760.0*/8.36, /*13 6173.0*/7.83, /*14 6586.0*/
		7.45, /*15 7000.0*/0.0 }, {/*11 engineLoad=3.546663*//*0 800.0*/12.09, /*1 1213.0*/12.17, /*2 1626.0*/12.34, /*3 2040.0*/
		12.43, /*4 2453.0*/12.7, /*5 2866.0*/12.89, /*6 3280.0*/12.72, /*7 3693.0*/12.55, /*8 4106.0*/12.98, /*9 4520.0*/
		12.96, /*10 4933.0*/12.3, /*11 5346.0*/11.59, /*12 5760.0*/10.9, /*13 6173.0*/10.07, /*14 6586.0*/9.59, /*15 7000.0*/
		0.0 }, {/*12 engineLoad=3.759996*//*0 800.0*/12.08, /*1 1213.0*/12.18, /*2 1626.0*/12.34, /*3 2040.0*/12.43, /*4 2453.0*/
		12.7, /*5 2866.0*/12.93, /*6 3280.0*/12.72, /*7 3693.0*/12.63, /*8 4106.0*/12.87, /*9 4520.0*/13.56, /*10 4933.0*/
		14.76, /*11 5346.0*/14.62, /*12 5760.0*/14.14, /*13 6173.0*/13.39, /*14 6586.0*/12.19, /*15 7000.0*/0.0 }, {/*13 engineLoad=3.973329*//*0 800.0*/
		12.09, /*1 1213.0*/12.17, /*2 1626.0*/12.34, /*3 2040.0*/12.43, /*4 2453.0*/12.71, /*5 2866.0*/12.92, /*6 3280.0*/
		12.74, /*7 3693.0*/12.57, /*8 4106.0*/12.88, /*9 4520.0*/13.82, /*10 4933.0*/15.15, /*11 5346.0*/15.57, /*12 5760.0*/
		16.22, /*13 6173.0*/16.05, /*14 6586.0*/15.59, /*15 7000.0*/0.0 }, {/*14 engineLoad=4.186662*//*0 800.0*/12.08, /*1 1213.0*/
		12.18, /*2 1626.0*/12.34, /*3 2040.0*/12.4, /*4 2453.0*/12.71, /*5 2866.0*/12.92, /*6 3280.0*/12.71, /*7 3693.0*/
		12.53, /*8 4106.0*/12.97, /*9 4520.0*/13.55, /*10 4933.0*/14.87, /*11 5346.0*/15.67, /*12 5760.0*/16.16, /*13 6173.0*/
		16.2, /*14 6586.0*/15.89, /*15 7000.0*/0.0 }, {/*15 engineLoad=4.399995*//*0 800.0*/12.08, /*1 1213.0*/12.17, /*2 1626.0*/
		12.34, /*3 2040.0*/12.43, /*4 2453.0*/12.74, /*5 2866.0*/12.91, /*6 3280.0*/12.71, /*7 3693.0*/12.54, /*8 4106.0*/
		12.97, /*9 4520.0*/13.53, /*10 4933.0*/14.87, /*11 5346.0*/15.48, /*12 5760.0*/16.1, /*13 6173.0*/16.18, /*14 6586.0*/
		15.93, /*15 7000.0*/0.0 } };

static const ignition_table_t miata_maf_advance_table = { {/*0  engineLoad=1.200*//*0 800.0*/+4.498, /*1 1213.0*/+11.905, /*2 1626.0*/
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

static void setDefaultCrankingFuel(engine_configuration_s *engineConfiguration) {
	// todo: set cranking parameters method based on injectors and displacement?

	// set cranking_fuel 5
	engineConfiguration->cranking.baseFuel = 5;
}

EXTERN_ENGINE;

static void commonMiataNa(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engineConfiguration->trigger.type = TT_MAZDA_MIATA_NA;
	engineConfiguration->engineChartSize = 100;

	boardConfiguration->triggerInputPins[0] = GPIOC_6; // 2G YEL/BLU
	boardConfiguration->triggerInputPins[1] = GPIOA_5; // 2E White CKP

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	setFuelLoadBin(1.2, 4.4 PASS_ENGINE_PARAMETER_SUFFIX);
	setFuelRpmBin(800, 7000 PASS_ENGINE_PARAMETER_SUFFIX);

	boardConfiguration->idle.solenoidFrequency = 160;

	// Frankenstein: high side #1 is PE8
	// Frankenstein: high side #2 is PE10
	// Frankenstein: high side #3 is PE12
	// Frankenstein: high side #4 is PE14
	// Frankenstein: high side #5 is PC9
	// Frankenstein: high side #6 is PC7

	boardConfiguration->ignitionPins[0] = GPIOE_12; // Frankenstein: high side #3
	boardConfiguration->ignitionPins[1] = GPIOE_14; // Frankenstein: high side #4
	boardConfiguration->ignitionPins[2] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPinMode = OM_DEFAULT;

	setDefaultCrankingFuel(engineConfiguration);

	boardConfiguration->triggerSimulatorPinModes[0] = OM_OPENDRAIN;
	boardConfiguration->triggerSimulatorPinModes[1] = OM_OPENDRAIN;

	setCommonNTCSensor(&engineConfiguration->clt);
	engineConfiguration->clt.config.bias_resistor = 2700;
	setCommonNTCSensor(&engineConfiguration->iat);
	engineConfiguration->iat.config.bias_resistor = 2700;

}

void common079721_2351(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration) {

	engineConfiguration->engineChartSize = 300;

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	boardConfiguration->fuelPumpPin = GPIO_UNASSIGNED; // fuel pump is not controlled by ECU on this engine

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
	engineConfiguration->tpsAdcChannel = EFI_ADC_3;
	engineConfiguration->clt.adcChannel = EFI_ADC_11;

}

/**
 * Frankenstein board
 */
void setMiata1990(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	common079721_2351(engineConfiguration, boardConfiguration);

	commonMiataNa(PASS_ENGINE_PARAMETER_SIGNATURE);

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

	boardConfiguration->injectionPins[0] = GPIOB_9; // Frankenstein: low side - out #12
	boardConfiguration->injectionPins[1] = GPIOB_8; // Frankenstein: low side - out #11
	boardConfiguration->injectionPins[2] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[3] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[5] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPinMode = OM_DEFAULT;

// todo: idleValvePin
}


static void setMiata1994_common(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	commonMiataNa(PASS_ENGINE_PARAMETER_SIGNATURE);
	engineConfiguration->specs.displacement = 1.839;

	// set cranking_timing_angle 0
	engineConfiguration->crankingTimingAngle = 0;

	engineConfiguration->crankingChargeAngle = 70;

	copyFuelTable(miata_maf_fuel_table, config->fuelTable);

	copyTimingTable(miata_maf_advance_table, config->ignitionTable);

//	boardConfiguration->triggerSimulatorPins[0] = GPIOD_2; // 2G - YEL/BLU
//	boardConfiguration->triggerSimulatorPins[1] = GPIOB_3; // 2E - WHT - four times
//	boardConfiguration->triggerSimulatorPinModes[0] = OM_OPENDRAIN;
//	boardConfiguration->triggerSimulatorPinModes[1] = OM_OPENDRAIN;
//
//	boardConfiguration->triggerInputPins[0] = GPIO_UNASSIGNED;
//	boardConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
//
//	boardConfiguration->is_enabled_spi_1 = false;
//	boardConfiguration->is_enabled_spi_2 = false;
//	boardConfiguration->is_enabled_spi_3 = false;

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
	boardConfiguration->fanPin = GPIOE_6;

	boardConfiguration->o2heaterPin = GPIO_UNASSIGNED;

	boardConfiguration->fuelPumpPin = GPIOE_4;

	boardConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[5] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPinMode = OM_DEFAULT;

	boardConfiguration->idle.solenoidPin = GPIOB_9;

	boardConfiguration->ignitionPins[0] = GPIOE_14; // Frankenso high side - pin 1G
	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[2] = GPIOC_7; // Frankenso high side - pin 1H
	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPinMode = OM_DEFAULT;

	setFrankenso_01_LCD(boardConfiguration);

	commonFrankensoAnalogInputs(engineConfiguration);

	engineConfiguration->tpsAdcChannel = EFI_ADC_2;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;
	engineConfiguration->mafAdcChannel = EFI_ADC_0;
	engineConfiguration->clt.adcChannel = EFI_ADC_12;
	engineConfiguration->iat.adcChannel = EFI_ADC_11;
// end of 1994 commond
}

/**
 * Frankenso board
 * set engine_type 20
 */
void setMiata1994_d(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setMiata1994_common(PASS_ENGINE_PARAMETER_SIGNATURE);
	engineConfiguration->vbattDividerCoeff = ((float) (8.2 + 33)) / 8.2 * 2;
	/**
	 * This board was avoiding PE0 & PE1 mosfets altogether
	 */
	boardConfiguration->injectionPins[0] = GPIOD_7; // avoiding PE1
	boardConfiguration->injectionPins[1] = GPIOE_2;
	boardConfiguration->injectionPins[2] = GPIOB_8;
	boardConfiguration->injectionPins[3] = GPIOB_7;

	// todo: add the diode? change idle valve logic?
	boardConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;
}

void setMiata1994_s(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setMiata1994_common(PASS_ENGINE_PARAMETER_SIGNATURE);
	engineConfiguration->vbattDividerCoeff = ((float) (10.0 + 33)) / 10 * 2;

	boardConfiguration->triggerSimulatorPins[2] = GPIO_UNASSIGNED;

	engineConfiguration->acSwitchAdc = EFI_ADC_1; // PA1, W50 on Frankenso

	engineConfiguration->afr.hwChannel = EFI_ADC_3;
	setEgoSensor(ES_Innovate_MTX_L PASS_ENGINE_PARAMETER_SUFFIX);

	/**
	 * This board has PE0<>PD5 & PE1<>PD3 rewired in order to avoid Discovery issue
	 */
	boardConfiguration->injectionPins[0] = GPIOD_3; // avoiding PE1
	boardConfiguration->injectionPins[1] = GPIOE_2; // injector #2
	boardConfiguration->injectionPins[2] = GPIOB_8; // injector #3
	boardConfiguration->injectionPins[3] = GPIOB_7; // injector #4

	//	setFsio(engineConfiguration, 0, GPIOD_11, "coolant 80 >");
	boardConfiguration->idle.solenoidFrequency = 500;

	engineConfiguration->acCutoffLowRpm = 400;
	engineConfiguration->acCutoffHighRpm = 4500;
	engineConfiguration->acIdleRpmBump = 200;

	//engineConfiguration->idleMode != IM_AUTO;
	setTargetRpmCurve(800 PASS_ENGINE_PARAMETER_SUFFIX);


	engineConfiguration->tpsMax = 86;
	engineConfiguration->tpsMin = 596;

	boardConfiguration->malfunctionIndicatorPin = GPIOE_5;
	boardConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;

	engineConfiguration->fuelAlgorithm = LM_REAL_MAF;
	setMazdaMiataNAMaf(config);
	engineConfiguration->injector.flow = 230;
}

/**
 * Tom tomiata, Frankenstein board
 */
void setMiata1996(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	commonMiataNa(PASS_ENGINE_PARAMETER_SIGNATURE);
	engineConfiguration->specs.displacement = 1.839;

	copyFuelTable(miata_maf_fuel_table, config->fuelTable);
	copyTimingTable(miata_maf_advance_table, config->ignitionTable);

	// upside down
	boardConfiguration->triggerInputPins[0] = GPIOA_5;
	boardConfiguration->triggerInputPins[1] = GPIOC_6;

	boardConfiguration->fuelPumpPin = GPIOE_4;
	boardConfiguration->idle.solenoidPin = GPIOE_5;

	engineConfiguration->mafAdcChannel = EFI_ADC_1;
	engineConfiguration->clt.adcChannel = EFI_ADC_11;
	engineConfiguration->tpsAdcChannel = EFI_ADC_13;

	boardConfiguration->ignitionPins[0] = GPIOE_12; // Frankenstein: high side #3
	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[2] = GPIOE_14; // Frankenstein: high side #4
	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPinMode = OM_DEFAULT;

	// harness is sequential but we have a limited board
	engineConfiguration->crankingInjectionMode = IM_BATCH;
	engineConfiguration->injectionMode = IM_BATCH;

	boardConfiguration->injectionPins[0] = GPIOB_9; // Frankenstein: low side - out #12
	boardConfiguration->injectionPins[1] = GPIOB_8; // Frankenstein: low side - out #11
	boardConfiguration->injectionPins[2] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[3] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[5] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPinMode = OM_DEFAULT;
}
