/**
 * @file	citroenBerlingoTU3JP.cpp
 *
 * CITROEN_TU3JP
 * set engine_type 15
 * Board pin out for Frankenstein
 *
 * @date Apr 15, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "citroenBerlingoTU3JP.h"
#include "custom_engine.h"

#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
static const uint8_t default_tps_advance_table[16][16] = {
{/*0  engineLoad=0.00*/   /*0 800.0*/+15, /*1 1213.0*/+16, /*2 1626.0*/+17, /*3 2040.0*/+19, /*4 2453.0*/+20, /*5 2866.0*/+21, /*6 3280.0*/+23, /*7 3693.0*/+24, /*8 4106.0*/+25, /*9 4520.0*/+27, /*10 4933.0*/+28, /*11 5346.0*/+29, /*12 5760.0*/+31, /*13 6173.0*/+32, /*14 6586.0*/+33, /*15 7000.0*/+35},
{/*1  engineLoad=6.66*/	  /*0 800.0*/+14, /*1 1213.0*/+16, /*2 1626.0*/+17, /*3 2040.0*/+18, /*4 2453.0*/+20, /*5 2866.0*/+21, /*6 3280.0*/+22, /*7 3693.0*/+24, /*8 4106.0*/+25, /*9 4520.0*/+26, /*10 4933.0*/+28, /*11 5346.0*/+29, /*12 5760.0*/+30, /*13 6173.0*/+32, /*14 6586.0*/+33, /*15 7000.0*/+34},
{/*2  engineLoad=13.33*/  /*0 800.0*/+14, /*1 1213.0*/+15, /*2 1626.0*/+17, /*3 2040.0*/+18, /*4 2453.0*/+19, /*5 2866.0*/+21, /*6 3280.0*/+22, /*7 3693.0*/+23, /*8 4106.0*/+25, /*9 4520.0*/+26, /*10 4933.0*/+27, /*11 5346.0*/+29, /*12 5760.0*/+30, /*13 6173.0*/+31, /*14 6586.0*/+33, /*15 7000.0*/+34},
{/*3  engineLoad=20.00*/  /*0 800.0*/+14, /*1 1213.0*/+15, /*2 1626.0*/+17, /*3 2040.0*/+18, /*4 2453.0*/+19, /*5 2866.0*/+20, /*6 3280.0*/+22, /*7 3693.0*/+23, /*8 4106.0*/+24, /*9 4520.0*/+26, /*10 4933.0*/+27, /*11 5346.0*/+28, /*12 5760.0*/+30, /*13 6173.0*/+31, /*14 6586.0*/+32, /*15 7000.0*/+34},
{/*4  engineLoad=26.66*/  /*0 800.0*/+14, /*1 1213.0*/+15, /*2 1626.0*/+16, /*3 2040.0*/+18, /*4 2453.0*/+19, /*5 2866.0*/+20, /*6 3280.0*/+21, /*7 3693.0*/+23, /*8 4106.0*/+24, /*9 4520.0*/+25, /*10 4933.0*/+27, /*11 5346.0*/+28, /*12 5760.0*/+29, /*13 6173.0*/+31, /*14 6586.0*/+32, /*15 7000.0*/+33},
{/*5  engineLoad=33.33*/  /*0 800.0*/+14, /*1 1213.0*/+15, /*2 1626.0*/+16, /*3 2040.0*/+17, /*4 2453.0*/+19, /*5 2866.0*/+20, /*6 3280.0*/+21, /*7 3693.0*/+23, /*8 4106.0*/+24, /*9 4520.0*/+25, /*10 4933.0*/+26, /*11 5346.0*/+28, /*12 5760.0*/+29, /*13 6173.0*/+30, /*14 6586.0*/+32, /*15 7000.0*/+33},
{/*6  engineLoad=40.00*/  /*0 800.0*/+13, /*1 1213.0*/+15, /*2 1626.0*/+16, /*3 2040.0*/+17, /*4 2453.0*/+18, /*5 2866.0*/+20, /*6 3280.0*/+21, /*7 3693.0*/+22, /*8 4106.0*/+24, /*9 4520.0*/+25, /*10 4933.0*/+26, /*11 5346.0*/+27, /*12 5760.0*/+29, /*13 6173.0*/+30, /*14 6586.0*/+31, /*15 7000.0*/+33},
{/*7  engineLoad=46.66*/  /*0 800.0*/+13, /*1 1213.0*/+14, /*2 1626.0*/+16, /*3 2040.0*/+17, /*4 2453.0*/+18, /*5 2866.0*/+19, /*6 3280.0*/+21, /*7 3693.0*/+22, /*8 4106.0*/+23, /*9 4520.0*/+25, /*10 4933.0*/+26, /*11 5346.0*/+27, /*12 5760.0*/+28, /*13 6173.0*/+30, /*14 6586.0*/+31, /*15 7000.0*/+32},
{/*8  engineLoad=53.33*/  /*0 800.0*/+13, /*1 1213.0*/+14, /*2 1626.0*/+15, /*3 2040.0*/+17, /*4 2453.0*/+18, /*5 2866.0*/+19, /*6 3280.0*/+20, /*7 3693.0*/+22, /*8 4106.0*/+23, /*9 4520.0*/+24, /*10 4933.0*/+26, /*11 5346.0*/+27, /*12 5760.0*/+28, /*13 6173.0*/+29, /*14 6586.0*/+31, /*15 7000.0*/+32},
{/*9  engineLoad=60.00*/  /*0 800.0*/+13, /*1 1213.0*/+14, /*2 1626.0*/+15, /*3 2040.0*/+16, /*4 2453.0*/+18, /*5 2866.0*/+19, /*6 3280.0*/+20, /*7 3693.0*/+21, /*8 4106.0*/+23, /*9 4520.0*/+24, /*10 4933.0*/+25, /*11 5346.0*/+26, /*12 5760.0*/+28, /*13 6173.0*/+29, /*14 6586.0*/+30, /*15 7000.0*/+32},
{/*10 engineLoad=66.66*/  /*0 800.0*/+13, /*1 1213.0*/+14, /*2 1626.0*/+15, /*3 2040.0*/+16, /*4 2453.0*/+17, /*5 2866.0*/+19, /*6 3280.0*/+20, /*7 3693.0*/+21, /*8 4106.0*/+22, /*9 4520.0*/+24, /*10 4933.0*/+25, /*11 5346.0*/+26, /*12 5760.0*/+27, /*13 6173.0*/+29, /*14 6586.0*/+30, /*15 7000.0*/+31},
{/*11 engineLoad=73.33*/  /*0 800.0*/+12, /*1 1213.0*/+14, /*2 1626.0*/+15, /*3 2040.0*/+16, /*4 2453.0*/+17, /*5 2866.0*/+18, /*6 3280.0*/+20, /*7 3693.0*/+21, /*8 4106.0*/+22, /*9 4520.0*/+23, /*10 4933.0*/+25, /*11 5346.0*/+26, /*12 5760.0*/+27, /*13 6173.0*/+28, /*14 6586.0*/+30, /*15 7000.0*/+31},
{/*12 engineLoad=80.00*/  /*0 800.0*/+12, /*1 1213.0*/+13, /*2 1626.0*/+15, /*3 2040.0*/+16, /*4 2453.0*/+17, /*5 2866.0*/+18, /*6 3280.0*/+19, /*7 3693.0*/+21, /*8 4106.0*/+22, /*9 4520.0*/+23, /*10 4933.0*/+24, /*11 5346.0*/+26, /*12 5760.0*/+27, /*13 6173.0*/+28, /*14 6586.0*/+29, /*15 7000.0*/+31},
{/*13 engineLoad=86.66*/  /*0 800.0*/+12, /*1 1213.0*/+13, /*2 1626.0*/+14, /*3 2040.0*/+16, /*4 2453.0*/+17, /*5 2866.0*/+18, /*6 3280.0*/+19, /*7 3693.0*/+20, /*8 4106.0*/+22, /*9 4520.0*/+23, /*10 4933.0*/+24, /*11 5346.0*/+25, /*12 5760.0*/+27, /*13 6173.0*/+28, /*14 6586.0*/+29, /*15 7000.0*/+30},
{/*14 engineLoad=93.33*/  /*0 800.0*/+12, /*1 1213.0*/+13, /*2 1626.0*/+14, /*3 2040.0*/+15, /*4 2453.0*/+17, /*5 2866.0*/+18, /*6 3280.0*/+19, /*7 3693.0*/+20, /*8 4106.0*/+21, /*9 4520.0*/+23, /*10 4933.0*/+24, /*11 5346.0*/+25, /*12 5760.0*/+26, /*13 6173.0*/+27, /*14 6586.0*/+29, /*15 7000.0*/+30},
{/*15 engineLoad=100.00*/ /*0 800.0*/+12, /*1 1213.0*/+13, /*2 1626.0*/+14, /*3 2040.0*/+15, /*4 2453.0*/+16, /*5 2866.0*/+18, /*6 3280.0*/+19, /*7 3693.0*/+20, /*8 4106.0*/+21, /*9 4520.0*/+22, /*10 4933.0*/+24, /*11 5346.0*/+25, /*12 5760.0*/+26, /*13 6173.0*/+27, /*14 6586.0*/+28, /*15 7000.0*/+30}
};
#endif

//static const float rpmSteps[16] = {400, 800, 1200, 1600, 2000, 2400, 2800, 3200, 3600, 4000, 4400, 4800, 5200, 5600, 6000, 6400};
//static const float mapSteps[16] = {25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100};

void setCitroenBerlingoTU3JPConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setDefaultFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);

	engineConfiguration->engineType = CITROEN_TU3JP;

	/**
	 * Base engine setting
	 */
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->globalTriggerAngleOffset = 114;
	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.displacement = 1.360;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->injectionMode = IM_BATCH;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->rpmHardLimit = 5000;
	engineConfiguration->cranking.rpm = 600;

//	memcpy(config->ve2RpmBins, rpmSteps, sizeof(rpmSteps));

	/**
	 * Algorithm Alpha-N setting
	 */
	setAlgorithm(LM_ALPHA_N PASS_CONFIG_PARAMETER_SUFFIX);
	setTimingRpmBin(800, 7000 PASS_CONFIG_PARAMETER_SUFFIX);

	/**
	 * Outputs
	 */

	// Frankenstein lo-side output #1: PC14 Igniter 1-4
	// Frankenstein lo-side output #2: PC15 Igniter 2-3
	// Frankenstein lo-side output #3: PE6  Injector 1-4
	// Frankenstein lo-side output #4: PC13 Injector 2-3
	// Frankenstein lo-side output #5: PE4
	// Frankenstein lo-side output #6: PE5
	// Frankenstein lo-side output #7: PE2
	// Frankenstein lo-side output #8: PE3	Tachometer
	// Frankenstein lo-side output #9: PE0	Fan
	// Frankenstein lo-side output #10: PE1	MIL
	// Frankenstein lo-side output #11: PB8	Main relay
	// Frankenstein lo-side output #12: PB9	Fuel pump

	engineConfiguration->ignitionPins[0] = GPIOC_14;
	engineConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[2] = GPIOC_15;
	engineConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;

	engineConfiguration->injector.flow = 137; //SIEMENS DEKA VAZ20734
	engineConfiguration->injectionPins[0] = GPIOE_6;
	engineConfiguration->injectionPins[1] = GPIOC_13;
	engineConfiguration->injectionPins[2] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[3] = GPIO_UNASSIGNED;

	engineConfiguration->fanPin = GPIOE_0;
	engineConfiguration->fanPinMode = OM_DEFAULT;
	engineConfiguration->fanOffTemperature = 95;
	engineConfiguration->fanOnTemperature = 99;

	engineConfiguration->malfunctionIndicatorPin = GPIOE_1;
	engineConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;

	engineConfiguration->mainRelayPin = GPIOB_8;

	engineConfiguration->fuelPumpPin = GPIOB_9;
	engineConfiguration->fuelPumpPinMode = OM_DEFAULT;

	setFrankenso_01_LCD(engineConfiguration);


//	engineConfiguration->o2heaterPin = GPIOC_13;
//	engineConfiguration->logicAnalyzerPins[1] = GPIO_UNASSIGNED;

	/**
	 * Inputs
	 */

	// See https://docs.google.com/spreadsheet/ccc?key=0Arl1FeMZcfisdEdGdUlHdWh6cVBoSzFIbkxqa1QtZ3c
	// Frankenstein analog input #1: PA1		adc1	MAP
	// Frankenstein analog input #2: PA3		adc3	TPS
	// Frankenstein analog input #3: PC3		adc13	IAT
	// Frankenstein analog input #4: PC1		adc11	CLT
	// Frankenstein analog input #5: PA0		adc0	vBatt
	// Frankenstein analog input #6: PC2		adc12	WBO
	// Frankenstein analog input #7: PA4		adc4
	// Frankenstein analog input #8: PA2		adc2
	// Frankenstein analog input #9: PA6		adc6
	// Frankenstein analog input #10: PA7		adc7
	// Frankenstein analog input #11: PC4		adc14
	// Frankenstein analog input #12: PC5|PA8	adc15	Speed Sensor

	/**
	 * MAP <BOSCH 0 261 230 057>
	 */
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_1;
	engineConfiguration->map.sensor.type = MT_CUSTOM;
	engineConfiguration->map.sensor.lowValue = 10;
	engineConfiguration->map.sensor.highValue = 105;
	engineConfiguration->mapHighValueVoltage = 5;
	/**
	 * TPS <MAGNETI MARELLI>
	 */
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_3;
	engineConfiguration->tpsMax = 102; // convert 12to10 bit (ADC/4)
	engineConfiguration->tpsMin = 825; // convert 12to10 bit (ADC/4)
	/**
	 * IAT <OEM ECU>
	 */
	engineConfiguration->iat.adcChannel = EFI_ADC_13;
	engineConfiguration->iat.config = {-20.0, 23.0, 92.0, 15600.0, 2250.0, 240.0, 2660};
	/**
	* CLT <LADA Samara>
	*/
	engineConfiguration->clt.adcChannel = EFI_ADC_11;
	engineConfiguration->clt.config = {-20.0, 25.0, 100.0, 28680.0, 2796.0, 177.0, 2660};
	/**
	 * vBatt
	 */
	engineConfiguration->vbattAdcChannel = EFI_ADC_0;
	engineConfiguration->vbattDividerCoeff = ((float) (2.6 + 10.1)) / 2.6 * 2;
	/**
	* WBO Innovate LC-1
	*/
	engineConfiguration->afr.hwChannel = EFI_ADC_12;
	/**
	* Speed Sensor
	*/
	//engineConfiguration->vehicleSpeedSensorInputPin = GPIOA_8;
	/**
	* Other
	*/
//	engineConfiguration->mafAdcChannel = GPIO_UNASSIGNED;

#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
	copyTable(config->ignitionTable, default_tps_advance_table);
#endif
}
