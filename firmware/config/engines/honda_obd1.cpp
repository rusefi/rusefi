#include "pch.h"
#include "defaults.h"
#include "honda_obd1_canned_tables.cpp"

#ifdef HW_HELLEN_UAEFI
#include "hellen_meta.h"
#endif // HW_HELLEN_UAEFI

void setHondaObd1() {
	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_HONDA);

	engineConfiguration->trigger.type = trigger_type_e::TT_12_TOOTH_CRANK; // 12 crank same as 24 cam
	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;

	engineConfiguration->map.sensor.type = MT_DENSO183;
	engineConfiguration->ignitionMode = IM_ONE_COIL;

    engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;
    engineConfiguration->fuelReferencePressure = 294;

    engineConfiguration->tpsMin = 124;
    engineConfiguration->tpsMax = 919;
    engineConfiguration->fan1ExtraIdle = 10;
    engineConfiguration->injector.flow = 240;
    engineConfiguration->mapErrorDetectionTooHigh = 250;

	// we expect end users to use timing light to validate specific vehicle!
	engineConfiguration->globalTriggerAngleOffset = 80;

  // what's average displacement for Honda OBD1? :)
  engineConfiguration->displacement = 1.645;

	// Keihin 06164-P0A-A00
	engineConfiguration->injector.flow = 248;

	// Disable ETBs
	engineConfiguration->etbFunctions[0] = dc_function_e::DC_None;
	engineConfiguration->etbFunctions[1] = dc_function_e::DC_None;

  // ECU does not control main relay
  engineConfiguration->mainRelayPin = Gpio::Unassigned;

#ifdef HW_HELLEN_UAEFI
#if (IGN_LOAD_COUNT == 16) && (IGN_RPM_COUNT == 16) && (FUEL_LOAD_COUNT == 16) && (FUEL_RPM_COUNT == 16)
  cannedignitionTable();
#endif
#endif // HW_HELLEN_UAEFI

	strcpy(engineConfiguration->gpPwmNote[0], "VTC");
	gppwm_channel *vtcControl = &engineConfiguration->gppwm[0];
	vtcControl->loadAxis = GPPWM_Map;
	vtcControl->pwmFrequency = 0;
#ifdef HW_HELLEN_UAEFI
  // Honda ICM is weird it fires spark on RAISING edge and has internal pull-up
  // thus we use a low side output to drive OEM ignition
  engineConfiguration->ignitionPins[0] = Gpio::MM100_IGN7; // low side!

 	engineConfiguration->idle.solenoidPin = Gpio::MM100_INJ8;
 	engineConfiguration->malfunctionIndicatorPin = Gpio::MM100_INJ7;
	engineConfiguration->acRelayPin = Gpio::MM100_OUT_PWM1;

	engineConfiguration->camInputs[0] = Gpio::MM100_UART8_RX; // "C18 VR1+ Discrete"
	engineConfiguration->camInputs[1] = Gpio::Unassigned;

	engineConfiguration->tps1_2AdcChannel = EFI_ADC_NONE;
	setPPSInputs(EFI_ADC_NONE, EFI_ADC_NONE);
  vtcControl->pin = Gpio::MM100_IGN6;
#endif // HW_HELLEN_UAEFI
}
