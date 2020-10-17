/*
 * @file vw_b6.cpp
 *
 * @date Dec 26, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine.h"
#include "vw_b6.h"
#include "custom_engine.h"

EXTERN_CONFIG;

/**
 * set engine_type 62
 * VW_B6
 * has to be microRusEFI 0.5.2
 */
void setVwPassatB6(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->vvtMode = VVT_BOSCH_QUICK_START;

	strcpy(CONFIG(engineMake), ENGINE_MAKE_VAG);
	strcpy(CONFIG(engineCode), "BPY");


	engineConfiguration->verboseVVTDecoding = true;
	engineConfiguration->invertCamVVTSignal = true;
	engineConfiguration->vvtCamSensorUseRise = true;

	// EFI_ADC_7: "31 - AN volt 3" - PA7
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_7;

	// "26 - AN volt 2"
	engineConfiguration->high_fuel_pressure_sensor_1 = EFI_ADC_6;
	// "19 - AN volt 4"
	engineConfiguration->low_fuel_pressure_sensor = EFI_ADC_12;

	CONFIG(isSdCardEnabled) = false;

	CONFIG(mc33816spiDevice) = SPI_DEVICE_3;
	setBoschHDEV_5_injectors(PASS_CONFIG_PARAMETER_SIGNATURE);
	engineConfiguration->spi3mosiPin = GPIOC_12;
	engineConfiguration->spi3misoPin = GPIOC_11;
	engineConfiguration->spi3sckPin = GPIOC_10;
	engineConfiguration->sdCardCsPin = GPIO_UNASSIGNED;
	CONFIG(is_enabled_spi_3) = true;


	// blue wire
	CONFIG(mc33816_cs) = GPIOB_9;
	// J18 green wire
	CONFIG(mc33816_rstb) = GPIOB_7;
	// J9 yellow wire
	CONFIG(mc33816_driven) = GPIOC_13;
	// J6 white wire
	CONFIG(mc33816_flag0) = GPIOE_9;

	// J10
	engineConfiguration->injectionPins[0] = GPIOE_6;
	// J11
	engineConfiguration->injectionPins[1] = GPIOE_5;
}
