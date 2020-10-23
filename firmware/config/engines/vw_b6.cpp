/*
 * @file vw_b6.cpp
 *
 * @date Dec 26, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine.h"
#include "vw_b6.h"
#include "custom_engine.h"
#include "table_helper.h"

EXTERN_CONFIG;

/**
 * set engine_type 62
 * VW_B6
 * has to be microRusEFI 0.5.2
 */
void setVwPassatB6(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
#if (BOARD_TLE8888_COUNT > 0)
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->vvtMode = VVT_BOSCH_QUICK_START;
	engineConfiguration->map.sensor.type = MT_BOSCH_2_5;

	strcpy(CONFIG(engineMake), ENGINE_MAKE_VAG);
	strcpy(CONFIG(engineCode), "BPY");


	engineConfiguration->verboseVVTDecoding = true;
	engineConfiguration->invertCamVVTSignal = true;
	engineConfiguration->vvtCamSensorUseRise = true;

	// EFI_ADC_7: "31 - AN volt 3" - PA7
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_7;

	// "26 - AN volt 2"
	engineConfiguration->highPressureFuel.hwChannel = EFI_ADC_6;
	// "19 - AN volt 4"
	engineConfiguration->lowPressureFuel.hwChannel = EFI_ADC_12;

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
	// TLE8888_PIN_24: "43 - GP Out 4"
	lowPressureFuelPumpControl->pin = TLE8888_PIN_24;


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


	engineConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;
	engineConfiguration->fanPin = GPIO_UNASSIGNED;

	CONFIG(useETBforIdleControl) = true;
	engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;
#endif /* BOARD_TLE8888_COUNT */
}
