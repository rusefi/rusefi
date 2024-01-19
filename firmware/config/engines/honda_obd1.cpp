#include "pch.h"

#if HW_HELLEN_UAEFI
#include "hellen_meta.h"
#endif // HW_HELLEN_UAEFI

void setHondaObd1() {
	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_HONDA);

	engineConfiguration->trigger.type = trigger_type_e::TT_12_TOOTH_CRANK; // 12 crank same as 24 cam
	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;

	engineConfiguration->map.sensor.type = MT_DENSO183;
	engineConfiguration->ignitionMode = IM_ONE_COIL;

  // what's average displacement for Honda OBD1? :)
  engineConfiguration->displacement = 1.645;

	// Keihin 06164-P0A-A00
	engineConfiguration->injector.flow = 248;

#if HW_HELLEN_UAEFI
	engineConfiguration->camInputs[0] = Gpio::MM100_UART8_RX; // "C18 VR1+ Discrete"
	engineConfiguration->camInputs[1] = Gpio::Unassigned;
#endif // HW_HELLEN_UAEFI
}
