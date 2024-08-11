#include "pch.h"
#include "gm_sbc.h"
#include "defaults.h"

#ifdef HW_HELLEN
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
  setPPSInputs(EFI_ADC_NONE, EFI_ADC_NONE);

#ifdef HW_HELLEN_8CHAN
	// using 8chan pinout for DC1: A26 (OUT_DC1+ AH pin "D") and A27 (OUT_DC1- AL pin "C")
	engineConfiguration->stepperDcIo[0].controlPin = Gpio::H144_GP_IO4; // DC1_PWM
	engineConfiguration->stepperDcIo[0].directionPin1 = Gpio::H144_GP_IO3; // DC1_DIR
	engineConfiguration->stepperDcIo[0].directionPin2 = Gpio::Unassigned;
	engineConfiguration->stepperDcIo[0].disablePin = Gpio::H144_GP_IO5; // ETB_DIS
	// using 8chan pinout for DC2: A28 (OUT_DC2+ BL pin "A") and A29 (OUT_DC2- BH pin "B")
	engineConfiguration->stepperDcIo[1].controlPin = H144_UART8_TX; // DC2_PWM
	engineConfiguration->stepperDcIo[1].directionPin1 = H_SPI3_CS; // DC2_DIR
	engineConfiguration->stepperDcIo[1].directionPin2 = Gpio::Unassigned;
#endif // HW_HELLEN

#ifdef HW_HELLEN_UAEFI
  // TODO: all the copy-pasting here begs the question: "shall we rename etbIo to hBridgeIo and reuse for stepper"?
	engineConfiguration->stepperDcIo[0].controlPin = Gpio::MM100_OUT_PWM3;
	engineConfiguration->stepperDcIo[0].directionPin1 = Gpio::MM100_OUT_PWM4;
	engineConfiguration->stepperDcIo[0].directionPin2 = Gpio::Unassigned;
	engineConfiguration->stepperDcIo[0].disablePin = Gpio::MM100_SPI2_MISO;
	engineConfiguration->stepperDcIo[1].controlPin = Gpio::MM100_OUT_PWM5;
	engineConfiguration->stepperDcIo[1].directionPin1 = Gpio::MM100_SPI2_MOSI;
	engineConfiguration->stepperDcIo[1].directionPin2 = Gpio::MM100_USB1ID;
#endif // HW_HELLEN_UAEFI

#ifdef HW_PROTEUS
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

void setGmSbc() {
	engineConfiguration->cylindersCount = 8;
	engineConfiguration->firingOrder = FO_1_8_4_3_6_5_7_2;
	engineConfiguration->displacement = 5.2;

    // see engine sniffer at https://github.com/rusefi/rusefi/issues/5353
    // if injectors are wired in groups of four best to use channels 1 and 4
    engineConfiguration->crankingInjectionMode = IM_BATCH;
    engineConfiguration->injectionMode = IM_BATCH;

	engineConfiguration->etbFunctions[0] = DC_None;
	engineConfiguration->etbFunctions[1] = DC_None;

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_GM);
	strcpy(engineConfiguration->engineCode, "SBC");
	// white wire "HEI E" plug pin D
	engineConfiguration->ignitionMode = IM_ONE_COIL;
	engineConfiguration->globalTriggerAngleOffset = 24;

    gppwm_channel *ignOverride = &engineConfiguration->gppwm[0];
   	ignOverride->pwmFrequency = 0;
   	strcpy(engineConfiguration->gpPwmNote[0], "ign ovrrd B");
   	setTable(ignOverride->table, 100);

#if HW_PROTEUS
    // tan wire with a black trace - "HEI B", plug pin B
    ignOverride->pin = Gpio::PROTEUS_IGN_12;
	engineConfiguration->fanPin = Gpio::Unassigned;
	// reminder about D101
   	engineConfiguration->injectionPins[0] = Gpio::PROTEUS_LS_14; // #1
   	// reminder about D104
   	engineConfiguration->injectionPins[3] = Gpio::PROTEUS_LS_15; // #4

    // wow high side relay control on MEFI1 ELECTRONIC-FUEL-INJECTION-MEFI-1-2-5.7L-350CID-8.2L-502CID.pdf page 46
    engineConfiguration->fuelPumpPin = Gpio::PROTEUS_HS_1;
    // low side on MEFI3 ELECTRONIC-FUEL-INJECTION-MEFI-3-5.7L-350CID-8.2L-502CID.pdf page 487

    setGmCltSensor(&engineConfiguration->clt, PROTEUS_DEFAULT_AT_PULLUP);
#endif // HW_PROTEUS
	engineConfiguration->mainRelayPin = Gpio::Unassigned; // vehicle controls main relay

 	setStepperHw();

	// 8 cylinders one coil HEI trigger
	engineConfiguration->trigger.type = trigger_type_e::TT_TOOTHED_WHEEL;
	engineConfiguration->trigger.customTotalToothCount = 8;
	engineConfiguration->trigger.customSkippedToothCount = 0;
	engineConfiguration->skippedWheelOnCam = true;
	// Proteus Digital 1 is Tach input "HEI R", plug pin C

	engineConfiguration->map.sensor.type = MT_GM_1_BAR;
}
