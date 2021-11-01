/*
 * honda_k_dbc.cpp
 *
 * @date Oct 2, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "honda_k_dbc.h"

#if HW_PROTEUS & EFI_PROD_CODE
#include "proteus_meta.h"
#endif // HW_PROTEUS

/**
 * K24A4 engine
 * PROTEUS_HONDA_ELEMENT_2003
 */
void setProteusHondaElement2003(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;
	engineConfiguration->specs.displacement = 2.4;
//	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
//	engineConfiguration->trigger.customTotalToothCount = 12;
//	engineConfiguration->trigger.customSkippedToothCount = 0;

	engineConfiguration->trigger.type = TT_HONDA_K_4_1;
	engineConfiguration->globalTriggerAngleOffset = 570;

	engineConfiguration->vvtMode[0] = VVT_HONDA_K;
	engineConfiguration->vvtMode[1] = VVT_FIRST_HALF;
	engineConfiguration->vvtOffsets[0] = -41;

	engineConfiguration->map.sensor.type = MT_DENSO183;
	engineConfiguration->injector.flow = 270;
	engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;
	engineConfiguration->fuelReferencePressure = 350; // TODO: what is real value?!

	strcpy(CONFIG(engineMake), ENGINE_MAKE_HONDA);
	strcpy(CONFIG(engineCode), "K24");
	strcpy(CONFIG(vehicleName), "test");

	engineConfiguration->tpsMin = 100;
	engineConfiguration->tpsMax = 830;

	engineConfiguration->displayLogicLevelsInEngineSniffer = true;

	// set cranking_fuel 15
	engineConfiguration->cranking.baseFuel = 70;


#if HW_PROTEUS & EFI_PROD_CODE
//	engineConfiguration->triggerInputPins[0] = PROTEUS_DIGITAL_2; // crank
//	engineConfiguration->camInputs[0] = PROTEUS_DIGITAL_4; // intake
//	engineConfiguration->camInputs[1 * CAMS_PER_BANK] = PROTEUS_DIGITAL_1; // exhaust

	engineConfiguration->triggerInputPins[0] = PROTEUS_DIGITAL_1; // exhaust
	engineConfiguration->camInputs[0] = PROTEUS_DIGITAL_4; // intake

	engineConfiguration->injectionPins[0] = PROTEUS_LS_8;
	engineConfiguration->injectionPins[1] = PROTEUS_LS_7;
	engineConfiguration->injectionPins[2] = PROTEUS_LS_6;
	engineConfiguration->injectionPins[3] = PROTEUS_LS_5;

	engineConfiguration->vvtPins[0] = PROTEUS_HS_2;

	engineConfiguration->malfunctionIndicatorPin = PROTEUS_LS_10;
	engineConfiguration->idle.solenoidPin = PROTEUS_LS_15;

	engineConfiguration->iat.adcChannel = PROTEUS_IN_ANALOG_TEMP_1;
	engineConfiguration->clt.adcChannel = PROTEUS_IN_ANALOG_TEMP_2;
	engineConfiguration->tps1_1AdcChannel = PROTEUS_IN_ANALOG_VOLT_3;
	engineConfiguration->map.sensor.hwChannel = PROTEUS_IN_ANALOG_VOLT_6;
	CONFIG(fanPin) = GPIO_UNASSIGNED;


	CONFIG(mainRelayPin) = PROTEUS_LS_9;
	CONFIG(fuelPumpPin) = PROTEUS_LS_11;
//	CONFIG(fanPin) = PROTEUS_LS_15;

#endif // HW_PROTEUS
}

void setProteusHondaOBD2A(DECLARE_CONFIG_PARAMETER_SIGNATURE) {

}
