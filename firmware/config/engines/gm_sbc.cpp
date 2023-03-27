#include "pch.h"
#include "gm_sbc.h"
#include "defaults.h"

#if HW_HELLEN
#include "hellen_meta.h"
#endif // HW_HELLEN

void setStepperHw() {
	engineConfiguration->useStepperIdle = true;
	engineConfiguration->useHbridgesToDriveIdleStepper = true;
	engineConfiguration->stepper_dc_use_two_wires = false;
	engineConfiguration->stepperDcInvertedPins = false; // or true?

#if HW_HELLEN
    setPPSInputs(EFI_ADC_NONE, EFI_ADC_NONE);
	// using 8chan pinout for DC1: A26 (OUT_DC1+) and A27 (OUT_DC1-)
	engineConfiguration->stepperDcIo[0].controlPin = H144_GP4; // DC1_PWM
	engineConfiguration->stepperDcIo[0].directionPin1 = H144_GP3; // DC1_DIR
	engineConfiguration->stepperDcIo[0].directionPin2 = Gpio::Unassigned;
	engineConfiguration->stepperDcIo[0].disablePin = H144_GP5; // ETB_DIS
	// using 8chan pinout for DC2: A28 (OUT_DC2+) and A29 (OUT_DC2-)
	engineConfiguration->stepperDcIo[1].controlPin = H144_UART1_TX; // DC2_PWM
	engineConfiguration->stepperDcIo[1].directionPin1 = H_SPI3_CS; // DC2_DIR
	engineConfiguration->stepperDcIo[1].directionPin2 = Gpio::Unassigned;
// open question if we need custom logic to handle shared disable pin?	engineConfiguration->stepperDcIo[1].disablePin = H144_GP5; // ETB_DIS
#endif // HW_HELLEN
}

void setSbc() {
	engineConfiguration->specs.cylindersCount = 8;
	engineConfiguration->specs.firingOrder = FO_1_8_4_3_6_5_7_2;
	engineConfiguration->specs.displacement = 5.2;

	engineConfiguration->etbFunctions[0] = DC_None;
	engineConfiguration->etbFunctions[1] = DC_None;

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_GM);
	strcpy(engineConfiguration->engineCode, "SBC");

 	setStepperHw();

	// 8 cylinders one coil HEI trigger
}

void set8chanSbc() {
    setSbc();
}

void setProteusSbc() {
    setSbc();
}