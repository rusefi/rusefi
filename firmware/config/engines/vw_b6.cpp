/*
 * @file vw_b6.cpp
 *
 * @date Dec 26, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "vw_b6.h"
#include "custom_engine.h"
#include "table_helper.h"
#include "electronic_throttle_impl.h"

/**
 * set engine_type 62
 * VW_B6
 * has to be microRusEFI 0.5.2
 */
void setVwPassatB6(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
#if (BOARD_TLE8888_COUNT > 0)
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->vvtMode[0] = VVT_BOSCH_QUICK_START;
	engineConfiguration->map.sensor.type = MT_BOSCH_2_5;

	// Injectors flow 1214 cc/min at 100 bar pressure
	engineConfiguration->injector.flow = 1214;
	// Use high pressure sensor
	engineConfiguration->injectorPressureType = IPT_High;
	// Automatic compensation of injector flow based on rail pressure
	engineConfiguration->injectorCompensationMode = ICM_SensedRailPressure;
	// Reference rail pressure is 10 000 kPa = 100 bar
	engineConfiguration->fuelReferencePressure = 10000;
	//setting "flat" 0.2 ms injector's lag time
	setArrayValues(engineConfiguration->injector.battLagCorr, 0.2);
	
	strcpy(CONFIG(engineMake), ENGINE_MAKE_VAG);
	strcpy(CONFIG(engineCode), "BPY");


	engineConfiguration->verboseVVTDecoding = true;
	engineConfiguration->invertCamVVTSignal = true;
	engineConfiguration->vvtCamSensorUseRise = true;

	// EFI_ADC_7: "31 - AN volt 3" - PA7
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_7;

	// "26 - AN volt 2"
	engineConfiguration->highPressureFuel.hwChannel = EFI_ADC_6;
	/**
	 * PSS-140
	 */
	// todo: calibration
	engineConfiguration->highPressureFuel.v1 = 0.5; /* volts */;
	engineConfiguration->highPressureFuel.value1 = 0;
	engineConfiguration->highPressureFuel.v2 = 4.5; /* volts */;
	engineConfiguration->highPressureFuel.value2 = BAR2KPA(140);

	// "19 - AN volt 4"
	engineConfiguration->lowPressureFuel.hwChannel = EFI_ADC_12;
	engineConfiguration->lowPressureFuel.v1 = 0.5; /* volts */;
	engineConfiguration->lowPressureFuel.value1 = PSI2KPA(0);
	engineConfiguration->lowPressureFuel.v2 = 4.5; /* volts */;
	// todo: what's the proper calibration of this Bosch sensor? is it really 200psi?
	engineConfiguration->lowPressureFuel.value2 = PSI2KPA(200);

	CONFIG(isSdCardEnabled) = false;

	CONFIG(mc33816spiDevice) = SPI_DEVICE_3;
	setBoschHDEV_5_injectors(PASS_CONFIG_PARAMETER_SIGNATURE);
	// RED
	engineConfiguration->spi3mosiPin = GPIOC_12;
	// YELLOW
	engineConfiguration->spi3misoPin = GPIOC_11;
	// BROWN
	engineConfiguration->spi3sckPin = GPIOC_10;
	engineConfiguration->sdCardCsPin = GPIO_UNASSIGNED;
	CONFIG(is_enabled_spi_3) = true;


	// J8 orange
	CONFIG(mc33816_cs) = GPIOB_8;
	// J8 Grey
	CONFIG(mc33816_rstb) = GPIOA_15;
	// J8 Dark BLUE
	CONFIG(mc33816_driven) = GPIOB_9;
	// J9 violet
	CONFIG(mc33816_flag0) = GPIOC_13;

	// J10 Dark BLUE
	engineConfiguration->injectionPins[0] = GPIOE_6;
	// J11 green
	engineConfiguration->injectionPins[1] = GPIOE_5;
	// J18 grey
	engineConfiguration->injectionPins[2] = GPIOB_7;
	// J6 white
	engineConfiguration->injectionPins[3] = GPIOE_0;


	gppwm_channel *lowPressureFuelPumpControl = &engineConfiguration->gppwm[1];
	lowPressureFuelPumpControl->pwmFrequency = 20;
	lowPressureFuelPumpControl->loadAxis = GPPWM_FuelLoad;
	lowPressureFuelPumpControl->dutyIfError = 50;
	setTable(lowPressureFuelPumpControl->table, (uint8_t)50);
	// "42 - Injector 4", somehow GP4 did not work? not enough current? not happy with diode?
	lowPressureFuelPumpControl->pin = TLE8888_PIN_4;


	gppwm_channel *coolantControl = &engineConfiguration->gppwm[0];

	coolantControl->pwmFrequency = 25;
	coolantControl->loadAxis = GPPWM_FuelLoad;
	// Volkswage wants 10% for fan to be OFF, between pull-up and low side control we need to invert that value
	int value = 100 - 10;
	coolantControl->dutyIfError = value;
	setTable(coolantControl->table, (uint8_t)value);
	// for now I just want to stop radiator whine
	// todo: enable cooling!
	/*
    for (int load = 0; load < GPPWM_LOAD_COUNT; load++) {
		for (int r = 0; r < GPPWM_RPM_COUNT; r++) {
			engineConfiguration->gppwm[0].table[load][r] = value;
		}
	}
*/
	coolantControl->pin = TLE8888_PIN_5; // "3 - Lowside 2"
	// "7 - Lowside 1"
	// engineConfiguration->hpfpValvePin = TLE8888_PIN_6; // Disable for now

	setBoschVAGETB(PASS_CONFIG_PARAMETER_SIGNATURE);


	engineConfiguration->injector.flow = 300;

	engineConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;
	engineConfiguration->fanPin = GPIO_UNASSIGNED;

	CONFIG(useETBforIdleControl) = true;
	engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;
#endif /* BOARD_TLE8888_COUNT */
}
