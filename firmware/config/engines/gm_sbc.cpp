#include "pch.h"
#include "gm_sbc.h"
#include "defaults.h"

#ifdef HW_HELLEN
#include "hellen_meta.h"
#endif // HW_HELLEN

#ifdef HW_HELLEN_8CHAN
#include "connectors/generated_board_pin_names.h"
#endif // HW_HELLEN_8CHAN

#include "proteus_meta.h"

void setStepperHw() {
	engineConfiguration->useStepperIdle = true;
	engineConfiguration->useHbridgesToDriveIdleStepper = true;
	engineConfiguration->stepper_dc_use_two_wires = false;
	engineConfiguration->stepperDcInvertedPins = false;

  for (size_t i = 0;i<ETB_COUNT;i++) {
	  engineConfiguration->etbFunctions[i] = DC_None;
	}

  setPPSInputs(EFI_ADC_NONE, EFI_ADC_NONE);

#ifdef HW_HELLEN_8CHAN
  void set8chanDefaultETBPins();
  set8chanDefaultETBPins();
#endif // HW_HELLEN_8CHAN

#ifdef HW_HELLEN_UAEFI
void setUaefiDefaultETBPins();
  setUaefiDefaultETBPins();
#endif // HW_HELLEN_UAEFI

#ifdef HW_PROTEUS
	// coil #1 = proteus pin 15 (DC2 positive) to BL pin "A", pin B to pin 8 (DC2 negative)
	// PWM pin
	engineConfiguration->stepperDcIo[0].controlPin = Gpio::D13;
	// DIR pin
	engineConfiguration->stepperDcIo[0].directionPin1 = Gpio::D9;
	// Disable pin
	engineConfiguration->stepperDcIo[0].disablePin = Gpio::D8;
	// Unused
	engineConfiguration->stepperDcIo[0].directionPin2 = Gpio::Unassigned;

	// coil #2 - proteus pin 7 (DC1 positive) to AH pin "D", pin C to pin 6 (DC1 negative)
	// PWM pin
	engineConfiguration->stepperDcIo[1].controlPin = Gpio::D12;
	// DIR pin
	engineConfiguration->stepperDcIo[1].directionPin1 = Gpio::D10;
	// Disable pin
	engineConfiguration->stepperDcIo[1].disablePin = Gpio::D11;
	// Unused
	engineConfiguration->stepperDcIo[1].directionPin2 = Gpio::Unassigned;

#endif // HW_PROTEUS
}

static void gmRailSensor() {
	engineConfiguration->highPressureFuel.v1 = 0.5; /* volts */;
	engineConfiguration->highPressureFuel.value1 = 0;
	engineConfiguration->highPressureFuel.v2 = 4.5; /* volts */;
	// fun fact: twice the range of VAG B6?
	engineConfiguration->highPressureFuel.value2 = 25300;
}

void setGmSbc() {
	engineConfiguration->cylindersCount = 8;
  // that's original firing order before gen 3
	engineConfiguration->firingOrder = FO_1_8_4_3_6_5_7_2;
	engineConfiguration->displacement = 5.2;

    // see engine sniffer at https://github.com/rusefi/rusefi/issues/5353
    // if injectors are wired in groups of four best to use channels 1 and 4
    engineConfiguration->crankingInjectionMode = IM_BATCH;
    engineConfiguration->injectionMode = IM_BATCH;

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_GM);
	strcpy(engineConfiguration->engineCode, "SBC");
	// white wire "HEI E" plug pin D
	engineConfiguration->ignitionMode = IM_ONE_COIL;
	engineConfiguration->globalTriggerAngleOffset = 24;

    gppwm_channel *ignOverride = &engineConfiguration->gppwm[0];
   	ignOverride->pwmFrequency = 0;
   	strcpy(engineConfiguration->gpPwmNote[0], "ign ovrrd B");
   	setTable(ignOverride->table, 100);

#ifdef HW_HELLEN_UAEFI121
    // coil control plug pin A

    // tan wire with a black trace - "HEI B", plug pin B (we use yellow)
    ignOverride->pin = Gpio::MM100_IGN2; // 12a Coil 2
    // tach plug pin C
    engineConfiguration->triggerInputPins[0] = Gpio::MM100_IN_D2; // 35a HALL2 red wire
	  engineConfiguration->camInputs[0] = Gpio::Unassigned;
	  engineConfiguration->camInputs[1] = Gpio::Unassigned;
	  engineConfiguration->ignitionPins[1] = Gpio::Unassigned;
#endif // HW_HELLEN_UAEFI121

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

    setGmCltSensor(&engineConfiguration->clt);
#endif // HW_PROTEUS
	engineConfiguration->mainRelayPin = Gpio::Unassigned; // vehicle controls main relay

    // for instance IWP069
	engineConfiguration->injector.flow = 482.5;

 	setStepperHw();

	// 8 cylinders one coil HEI trigger
	engineConfiguration->trigger.type = trigger_type_e::TT_TOOTHED_WHEEL;
	engineConfiguration->trigger.customTotalToothCount = 8;
	engineConfiguration->trigger.customSkippedToothCount = 0;
	engineConfiguration->skippedWheelOnCam = true;
	// Proteus Digital 1 is Tach input "HEI R", plug pin C

	engineConfiguration->map.sensor.type = MT_GM_1_BAR;
}

static void setGmGdi() {
  engineConfiguration->camSyncOnSecondCrankRevolution = true;
  engineConfiguration->globalTriggerAngleOffset = 90;
  engineConfiguration->vvtMode[0] = VVT_BOSCH_QUICK_START;
  engineConfiguration->lowPressureFuel.hwChannel = EFI_ADC_NONE;
  gmRailSensor();

  // is this oddly high or OKis?
	engineConfiguration->mc33_hpfp_i_peak = 10;
	engineConfiguration->mc33_hpfp_i_hold = 7;

#ifdef HW_HELLEN_4K_GDI
  engineConfiguration->invertCamVVTSignal = true;
  engineConfiguration->invertExhaustCamVVTSignal = true;

  engineConfiguration->triggerInputPins[0] = Gpio::H144_IN_SENS2; // Digital Input 3
  engineConfiguration->camInputs[0] = Gpio::H144_IN_SENS3; // Digital Input 4
  engineConfiguration->camInputs[1] = Gpio::Unassigned;
  engineConfiguration->sentInputPins[0] = Gpio::H144_IN_AUX2_DIGITAL;
  setGmCltSensor(&engineConfiguration->clt);

  // engineConfiguration->starterControlPin = high side :()
#endif // HW_HELLEN_4K_GDI

#ifdef HW_HELLEN_8CHAN
  engineConfiguration->sentInputPins[0] = Gpio::MM176_IN_D3;
  engineConfiguration->starterControlPin = Gpio::MM176_IGN5; // 14C

  engineConfiguration->map.sensor.hwChannel = MM176_IN_CRANK_ANALOG; // 1A Aux Analog 1. Important to use analog without muxing!
  engineConfiguration->fuelPumpPin = Gpio::MM176_OUT_IO2; // 6C - High Side 2 orange/brown
  engineConfiguration->hpfpValvePin = Gpio::MM176_GP17; // 19D Injector 9

	engineConfiguration->camInputs[2] = Gpio::Unassigned;
	engineConfiguration->camInputs[3] = Gpio::Unassigned;
	config->boardUseCrankPullUp = true;

	engineConfiguration->starterControlPin = Gpio::PIN_5C; // 5C - High Side 1, ignition output is not enough at least on HHR
	setGmCltSensor(&engineConfiguration->clt);
#endif // HW_HELLEN_8CHAN

  setPPSCalibration(1, 4.25, 0.5, 2.14);

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_GM);
	engineConfiguration->etbFunctions[1] = DC_None;
	setGDIFueling();
}

static void setGmEcotec3() {
  setGmGdi();
  engineConfiguration->sentEtbType = SentEtbType::GM_TYPE_1;
  setTPS1Inputs(EFI_ADC_NONE, EFI_ADC_NONE);
  engineConfiguration->EtbSentInput = SentInput::INPUT1;
}

static void set4CylGmEcotec3() {
  setGmEcotec3();
  engineConfiguration->hpfpCamLobes = 4;
  setHpfpLobeProfileAngle(engineConfiguration->hpfpCamLobes);
  engineConfiguration->vvtMode[1] = VVT_BOSCH_QUICK_START;
	setInline4();
}

void setGmLcv() {
  set4CylGmEcotec3();
  engineConfiguration->displacement = 2.5;
	engineConfiguration->map.sensor.type = MT_GM_1_BAR;
  strcpy(engineConfiguration->engineCode, "LCV");
}

void setGmLnf() {
  setGmGdi();
  engineConfiguration->displacement = 2.0;
  strcpy(engineConfiguration->engineCode, "LNF");
  setTPS1Calibration(880, 129, 118, 870);

  engineConfiguration->hpfpPeakPos = 0;

  // todo: engineConfiguration->hpfpCam = HPFP_CAM_IN1;
#ifdef HW_HELLEN_8CHAN
  engineConfiguration->vvtMode[1] = VVT_BOSCH_QUICK_START;
  engineConfiguration->invertCamVVTSignal = true;
	engineConfiguration->invertPrimaryTriggerSignal = true;
  engineConfiguration->highPressureFuel.hwChannel = MM176_IN_MAP1_ANALOG; // 2A Aux Analog 4

	engineConfiguration->vvtPins[0] = Gpio::MM176_OUT_PWM1;
	engineConfiguration->vvtPins[1] = Gpio::MM176_OUT_PWM2;
#endif // HW_HELLEN_8CHAN
}

// 2.0T
void setGmLtg() {
  set4CylGmEcotec3();
  engineConfiguration->displacement = 2.0;
#ifdef HW_HELLEN_8CHAN
  engineConfiguration->oilPressure.hwChannel = MM176_IN_MAP1_ANALOG; // 2A Aux Analog 4
  engineConfiguration->invertCamVVTSignal = true;

  engineConfiguration->triggerInputPins[0] = Gpio::MM176_IN_D4; // 9A - Hall Input 4

  engineConfiguration->luaOutputPins[0] = Gpio::MM176_OUT_IO1; // 7C - High Side 3 - unusual starter
  engineConfiguration->luaOutputPins[1] = Gpio::MM176_OUT_IO4, // 9C - Low Side 4 - unusual starter
#endif // HW_HELLEN_8CHAN
  strcpy(engineConfiguration->engineCode, "LTG");
}

void setGmSbcGen5() {
  setGmEcotec3();
  engineConfiguration->hpfpCamLobes = 3;
  engineConfiguration->displacement = 5.3;
	engineConfiguration->cylindersCount = 8;
	engineConfiguration->firingOrder = FO_1_8_7_2_6_5_4_3;
	engineConfiguration->map.sensor.type = MT_GM_1_BAR;

  // engineConfiguration->hpfpPumpVolume = 0.623; // LT4
	engineConfiguration->hpfpPumpVolume = 0.49; // cc per stroke

  // LT 17.7 g/sec at 100bar which 1440
  // todo: real flow!
	engineConfiguration->injector.flow = 990; // 12.6g/sec

#ifdef HW_HELLEN_8CHAN
  engineConfiguration->mc33_hpfp_i_peak = 15;
  engineConfiguration->mc33_hpfp_i_hold = 7;

	engineConfiguration->injectionPins[4] = Gpio::MM176_INJ5;
	engineConfiguration->injectionPins[5] = Gpio::MM176_INJ6;
	engineConfiguration->injectionPins[6] = Gpio::MM176_INJ7;
	engineConfiguration->injectionPins[7] = Gpio::MM176_INJ8;

	engineConfiguration->ignitionPins[4] = Gpio::MM176_IGN5;
	engineConfiguration->ignitionPins[5] = Gpio::MM176_IGN6;
	engineConfiguration->ignitionPins[6] = Gpio::MM176_IGN7;
	engineConfiguration->ignitionPins[7] = Gpio::MM176_IGN8;

	engineConfiguration->starterControlPin = Gpio::MM176_SPI3_SCK; // 29C - Ignition 9

  engineConfiguration->invertCamVVTSignal = true;
	engineConfiguration->invertPrimaryTriggerSignal = true;
  engineConfiguration->vvtMode[1] = VVT_INACTIVE;
  engineConfiguration->camInputs[1] = Gpio::Unassigned;

  // early models before SENT
  // todo: early gen5 LS engines have redundant high pressure sensors #7364
	engineConfiguration->highPressureFuel.v1 = 0.57; /* volts */;
	engineConfiguration->highPressureFuel.value1 = 500;
	engineConfiguration->highPressureFuel.v2 = 2.25; /* volts */;
	engineConfiguration->highPressureFuel.value2 = 10900;

	engineConfiguration->lowPressureFuel.v1 = 2.9; /* volts */;
	engineConfiguration->lowPressureFuel.value1 = 435;
	engineConfiguration->lowPressureFuel.v2 = 3.7; /* volts */;
	engineConfiguration->lowPressureFuel.value2 = 576;

	engineConfiguration->oilPressure.v1 = 0.6;
	engineConfiguration->oilPressure.value1 = 0; // 100 kPa absolute
	engineConfiguration->oilPressure.v2 = 2.6f;
	engineConfiguration->oilPressure.value2 = 420; // 520 kPa absolute

  engineConfiguration->oilPressure.hwChannel = MM176_IN_MAP1_ANALOG; // 2A Aux Analog 4
  engineConfiguration->fuelLevelSensor = MM176_IN_O2S_ANALOG; // 3A - Aux Analog 2
  engineConfiguration->lowPressureFuel.hwChannel = MM176_IN_AUX2_ANALOG; // 	4A - Aux Analog 7
  engineConfiguration->acPressure.hwChannel = MM176_IN_MAP2_ANALOG;// 10A - Aux Analog 5
  engineConfiguration->flexSensorPin = Gpio::MM176_IN_SENS3;
#endif // HW_HELLEN_8CHAN
}
