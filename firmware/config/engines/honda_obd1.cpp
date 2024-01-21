#include "pch.h"
#include "defaults.h"

#if HW_HELLEN_UAEFI
#include "hellen_meta.h"
#endif // HW_HELLEN_UAEFI

void setHondaObd1() {
	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_HONDA);

	engineConfiguration->trigger.type = trigger_type_e::TT_12_TOOTH_CRANK; // 12 crank same as 24 cam
	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;

	engineConfiguration->map.sensor.type = MT_DENSO183;
	engineConfiguration->ignitionMode = IM_ONE_COIL;

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

#if HW_HELLEN_UAEFI
  // Honda ICM is weird it fires spark on RAISING edge and has internal pull-up
  // thus we use a low side output to drive OEM ignition
  engineConfiguration->ignitionPins[0] = Gpio::MM100_IGN7; // low side!

	engineConfiguration->camInputs[0] = Gpio::MM100_UART8_RX; // "C18 VR1+ Discrete"
	engineConfiguration->camInputs[1] = Gpio::Unassigned;

	engineConfiguration->tps1_2AdcChannel = EFI_ADC_NONE;
	setPPSInputs(EFI_ADC_NONE, EFI_ADC_NONE);
#endif // HW_HELLEN_UAEFI
}
