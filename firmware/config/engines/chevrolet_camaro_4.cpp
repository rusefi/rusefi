/*
 * @file chevrolet_camaro_4.cpp
 *
 * CAMARO_4
 * set engine_type 35
 *
 * @date Oct 16, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "chevrolet_camaro_4.h"
#include "custom_engine.h"

void setCamaro4() {
	setFrankensoConfiguration();


	setAlgorithm(LM_SPEED_DENSITY);

	engineConfiguration->displacement = 5.7;
	engineConfiguration->cylindersCount = 8;
	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_GM);

	engineConfiguration->firingOrder = FO_1_8_7_2_6_5_4_3;
	engineConfiguration->triggerInputPins[0] = Gpio::A5;
	engineConfiguration->triggerInputPins[1] = Gpio::C6;

	engineConfiguration->injectionMode = IM_BATCH;

	// set ignition_mode 2
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;

	setCrankOperationMode();
	engineConfiguration->trigger.type = trigger_type_e::TT_GM_24x;

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_0; // PA0

	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;


	engineConfiguration->tps1_1AdcChannel = EFI_ADC_2;

	// todo: move this into trigger definition
	// set global_trigger_offset_angle 50
	engineConfiguration->globalTriggerAngleOffset = 50;

	engineConfiguration->iat.adcChannel = EFI_ADC_11;

	engineConfiguration->clt.adcChannel = EFI_ADC_12;
	engineConfiguration->clt.config.bias_resistor = 2700;

	engineConfiguration->vbattAdcChannel = EFI_ADC_14;

	engineConfiguration->afr.hwChannel = EFI_ADC_13;


	engineConfiguration->idle.solenoidPin = Gpio::Unassigned;
	engineConfiguration->fuelPumpPin = Gpio::Unassigned;

	engineConfiguration->injectionPins[0] = Gpio::E6;
	engineConfiguration->injectionPins[1] = Gpio::E5;
	engineConfiguration->injectionPins[2] = Gpio::D7;
	engineConfiguration->injectionPins[3] = Gpio::C13;
	engineConfiguration->injectionPins[4] = Gpio::E3;
	engineConfiguration->injectionPins[5] = Gpio::E4;
	engineConfiguration->injectionPins[6] = Gpio::D3;
	engineConfiguration->injectionPins[7] = Gpio::E2;


	engineConfiguration->ignitionPins[0] = Gpio::C9;
	engineConfiguration->ignitionPins[1] = Gpio::C7;
	engineConfiguration->ignitionPins[2] = Gpio::E14;
	engineConfiguration->ignitionPins[3] = Gpio::E12;
	engineConfiguration->ignitionPins[4] = Gpio::E10;
	engineConfiguration->ignitionPins[5] = Gpio::E8;
	engineConfiguration->ignitionPins[6] = Gpio::D9;
	engineConfiguration->ignitionPins[7] = Gpio::D8;

	engineConfiguration->fuelPumpPin = Gpio::B8;
	engineConfiguration->fanPin = Gpio::Unassigned;
	engineConfiguration->mainRelayPin = Gpio::D5;
}


