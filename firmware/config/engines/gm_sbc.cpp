#include "pch.h"
#include "gm_sbc.h"
#include "defaults.h"

#if HW_HELLEN
#include "hellen_meta.h"
#endif // HW_HELLEN

#include "proteus_meta.h"

void setStepperHw() {
	engineConfiguration->useStepperIdle = true;
	engineConfiguration->useHbridgesToDriveIdleStepper = true;
	engineConfiguration->stepper_dc_use_two_wires = false;
	engineConfiguration->stepperDcInvertedPins = false; // or true?

    // for instance IWP069
	engineConfiguration->injector.flow = 482.5;

#if HW_HELLEN
    setPPSInputs(EFI_ADC_NONE, EFI_ADC_NONE);
	// using 8chan pinout for DC1: A26 (OUT_DC1+ AH pin "D") and A27 (OUT_DC1- AL pin "C")
	engineConfiguration->stepperDcIo[0].controlPin = H144_GP4; // DC1_PWM
	engineConfiguration->stepperDcIo[0].directionPin1 = H144_GP3; // DC1_DIR
	engineConfiguration->stepperDcIo[0].directionPin2 = Gpio::Unassigned;
	engineConfiguration->stepperDcIo[0].disablePin = H144_GP5; // ETB_DIS
	// using 8chan pinout for DC2: A28 (OUT_DC2+ BL pin "A") and A29 (OUT_DC2- BH pin "B")
	engineConfiguration->stepperDcIo[1].controlPin = H144_UART1_TX; // DC2_PWM
	engineConfiguration->stepperDcIo[1].directionPin1 = H_SPI3_CS; // DC2_DIR
	engineConfiguration->stepperDcIo[1].directionPin2 = Gpio::Unassigned;
// open question if we need custom logic to handle shared disable pin?	engineConfiguration->stepperDcIo[1].disablePin = H144_GP5; // ETB_DIS
#endif // HW_HELLEN

#if HW_PROTEUS
	// coil #1 - proteus pin 7 to AH pin "D"
	// PWM pin
	engineConfiguration->stepperDcIo[0].controlPin = Gpio::D12;
	// DIR pin
	engineConfiguration->stepperDcIo[0].directionPin1 = Gpio::D10;
	// Disable pin
	engineConfiguration->stepperDcIo[0].disablePin = Gpio::D11;
	// Unused
	engineConfiguration->stepperDcIo[0].directionPin2 = Gpio::Unassigned;

	// coil #2 = proteus pin 15 to BL pin "A"
	// PWM pin
	engineConfiguration->stepperDcIo[1].controlPin = Gpio::D13;
	// DIR pin
	engineConfiguration->stepperDcIo[1].directionPin1 = Gpio::D9;
	// Disable pin
	engineConfiguration->stepperDcIo[1].disablePin = Gpio::D8;
	// Unused
	engineConfiguration->stepperDcIo[1].directionPin2 = Gpio::Unassigned;
#endif // HW_PROTEUS
}

void setSbc() {
	engineConfiguration->cylindersCount = 8;
	engineConfiguration->firingOrder = FO_1_8_4_3_6_5_7_2;
	engineConfiguration->displacement = 5.2;

    engineConfiguration->crankingInjectionMode = IM_BATCH;
    engineConfiguration->injectionMode = IM_BATCH;

	engineConfiguration->etbFunctions[0] = DC_None;
	engineConfiguration->etbFunctions[1] = DC_None;

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_GM);
	strcpy(engineConfiguration->engineCode, "SBC");

    gppwm_channel *ignOverride = &engineConfiguration->gppwm[0];
   	ignOverride->pwmFrequency = 0;
   	strcpy(engineConfiguration->gpPwmNote[0], "ign ovrrd B");
   	setTable(ignOverride->table, 100);

#if HW_PROTEUS
    ignOverride->pin = PROTEUS_IGN_12;
#endif // HW_PROTEUS

 	setStepperHw();

	// 8 cylinders one coil HEI trigger
	engineConfiguration->trigger.type = trigger_type_e::TT_TOOTHED_WHEEL;
	engineConfiguration->trigger.customTotalToothCount = 8;
	engineConfiguration->trigger.customSkippedToothCount = 0;
	engineConfiguration->skippedWheelOnCam = true;

	engineConfiguration->map.sensor.type = MT_GM_1_BAR;
}

void set8chanSbc() {
    setSbc();
}

void setProteusSbc() {
    setSbc();
}