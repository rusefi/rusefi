/**
 * @file	custom_engine.cpp
 *
 *
 * set engine_type 49
 * FRANKENSO_QA_ENGINE
 * See also DEFAULT_ENGINE_TYPE
 * Frankenso QA 12 cylinder engine
 *
 * @date Jan 18, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "custom_engine.h"
#include "mre_meta.h"
#include "proteus_meta.h"
#include "hellen_meta.h"
#include "odometer.h"
#include "defaults.h"
#if EFI_PROD_CODE
#include "drivers/gpio/mc33810.h"
#endif /* EFI_PROD_CODE */

#if EFI_ELECTRONIC_THROTTLE_BODY
#include "electronic_throttle.h"
#endif

#if EFI_PROD_CODE
#include "can_hw.h"
#include "scheduler.h"
#endif /* EFI_PROD_CODE */

void setDiscoveryPdm() {
}

#if defined(HW_NUCLEO_F767) || defined(HW_NUCLEO_H743) || defined(HW_FRANKENSO)

/**
 * set engine_type 59
 */
void setDiscovery33810Test() {
//	spi3mosiPin = Gpio::B5 grey
//	spi3misoPin = Gpio::B4; vio
//	spi3sckPin = Gpio::B3; blue
//  CS PC5 white
//  EN PA6 yellow

    engineConfiguration->map.sensor.hwChannel = EFI_ADC_NONE;
	engineConfiguration->clt.adcChannel = EFI_ADC_NONE;
	engineConfiguration->triggerSimulatorPins[0] = Gpio::Unassigned;
	engineConfiguration->triggerSimulatorPins[1] = Gpio::Unassigned;

    engineConfiguration->mc33810_cs[0] = Gpio::C5;

	engineConfiguration->injectionPins[0] = Gpio::MC33810_0_OUT_0;
	engineConfiguration->injectionPins[1] = Gpio::MC33810_0_OUT_1;
	engineConfiguration->injectionPins[2] = Gpio::Unassigned;
	engineConfiguration->injectionPins[3] = Gpio::Unassigned;

	engineConfiguration->ignitionPins[0] = Gpio::MC33810_0_GD_0;
	engineConfiguration->ignitionPins[1] = Gpio::MC33810_0_GD_1;
	engineConfiguration->ignitionPins[2] = Gpio::MC33810_0_GD_2;
	engineConfiguration->ignitionPins[3] = Gpio::MC33810_0_GD_3;

	engineConfiguration->cylindersCount = 2;
	engineConfiguration->firingOrder = FO_1_2;
}
#endif // HW_FRANKENSO

// todo: should this be part of more default configurations?
void setFrankensoConfiguration() {
#ifdef HW_FRANKENSO
	engineConfiguration->trigger.type = trigger_type_e::TT_ONE_PLUS_ONE;

	commonFrankensoAnalogInputs();

	/**
	 * Frankenso analog #1 PC2 ADC12 CLT
	 * Frankenso analog #2 PC1 ADC11 IAT
	 * Frankenso analog #3 PA0 ADC0 MAP
	 * Frankenso analog #4 PC3 ADC13 WBO / O2
	 * Frankenso analog #5 PA2 ADC2 TPS
	 * Frankenso analog #6 PA1 ADC1
	 * Frankenso analog #7 PA4 ADC4
	 * Frankenso analog #8 PA3 ADC3
	 * Frankenso analog #9 PA7 ADC7
	 * Frankenso analog #10 PA6 ADC6
	 * Frankenso analog #11 PC5 ADC15
	 * Frankenso analog #12 PC4 ADC14 VBatt
	 */
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_2; // PA2

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_0;

	engineConfiguration->clt.adcChannel = EFI_ADC_12;
	engineConfiguration->iat.adcChannel = EFI_ADC_11;
	engineConfiguration->afr.hwChannel = EFI_ADC_13;

  // Frankenso hardware
  engineConfiguration->clt.config.bias_resistor = 2700;
  engineConfiguration->iat.config.bias_resistor = 2700;

	/**
	 * http://rusefi.com/wiki/index.php?title=Manual:Hardware_Frankenso_board
	 */
	// Frankenso low out #1: PE6
	// Frankenso low out #2: PE5
	// Frankenso low out #3: PD7 Main Relay
	// Frankenso low out #4: PC13 Idle valve solenoid
	// Frankenso low out #5: PE3
	// Frankenso low out #6: PE4 fuel pump relay
	// Frankenso low out #7: PE1 (do not use with discovery!)
	// Frankenso low out #8: PE2 injector #2
	// Frankenso low out #9: PB9 injector #1
	// Frankenso low out #10: PE0 (do not use with discovery!)
	// Frankenso low out #11: PB8 injector #3
	// Frankenso low out #12: PB7 injector #4

	engineConfiguration->fuelPumpPin = Gpio::E4;
	engineConfiguration->mainRelayPin = Gpio::D7;
	engineConfiguration->idle.solenoidPin = Gpio::C13;

	engineConfiguration->fanPin = Gpio::E5;

	engineConfiguration->injectionPins[0] = Gpio::B9; // #1
	engineConfiguration->injectionPins[1] = Gpio::E2; // #2
	engineConfiguration->injectionPins[2] = Gpio::B8; // #3
#ifndef EFI_INJECTOR_PIN3
	engineConfiguration->injectionPins[3] = Gpio::B7; // #4
#else /* EFI_INJECTOR_PIN3 */
	engineConfiguration->injectionPins[3] = EFI_INJECTOR_PIN3; // #4
#endif /* EFI_INJECTOR_PIN3 */

	setAlgorithm(engine_load_mode_e::LM_SPEED_DENSITY);

	engineConfiguration->injectionPins[4] = Gpio::Unassigned;
	engineConfiguration->injectionPins[5] = Gpio::Unassigned;
	engineConfiguration->injectionPins[6] = Gpio::Unassigned;
	engineConfiguration->injectionPins[7] = Gpio::Unassigned;
	engineConfiguration->injectionPins[8] = Gpio::Unassigned;
	engineConfiguration->injectionPins[9] = Gpio::Unassigned;
	engineConfiguration->injectionPins[10] = Gpio::Unassigned;
	engineConfiguration->injectionPins[11] = Gpio::Unassigned;

	engineConfiguration->ignitionPins[0] = Gpio::E14;
	engineConfiguration->ignitionPins[1] = Gpio::C7;
	engineConfiguration->ignitionPins[2] = Gpio::C9;
	// set_ignition_pin 4 PE10
	engineConfiguration->ignitionPins[3] = Gpio::E10;

	// todo: 8.2 or 10k?
	engineConfiguration->vbattDividerCoeff = ((float) (10 + 33)) / 10 * 2;
#endif // HW_FRANKENSO
}

// ETB_BENCH_ENGINE
// set engine_type 58
void setEtbTestConfiguration() {
	// VAG test ETB
	engineConfiguration->tpsMin = 54;
	// by the way this ETB has default position of ADC=74 which is about 4%
	engineConfiguration->tpsMax = 540;

	// yes, 30K - that's a test configuration
	engineConfiguration->rpmHardLimit = 30000;

	setCrankOperationMode();
	engineConfiguration->trigger.type = trigger_type_e::TT_TOOTHED_WHEEL_60_2;


	engineConfiguration->ignitionPins[0] = Gpio::Unassigned;
	engineConfiguration->ignitionPins[1] = Gpio::Unassigned;
	engineConfiguration->ignitionPins[2] = Gpio::Unassigned;
	engineConfiguration->ignitionPins[3] = Gpio::Unassigned;
	/**
	 * remember that some H-bridges require 5v control lines, not just 3v logic outputs we have on stm32
	 */
	engineConfiguration->etbIo[0].directionPin1 = Gpio::C7; // Frankenso high-side in order to get 5v control
	engineConfiguration->etbIo[0].directionPin2 = Gpio::C9;
	engineConfiguration->etbIo[0].controlPin = Gpio::E14;

#if EFI_ELECTRONIC_THROTTLE_BODY
	setBoschVNH2SP30Curve();
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_2; // PA2
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_9; // PB1

	// turning off other PWMs to simplify debugging
	engineConfiguration->triggerSimulatorRpm = 0;
	engineConfiguration->stepperEnablePin = Gpio::Unassigned;
	engineConfiguration->idle.stepperStepPin = Gpio::Unassigned;
	engineConfiguration->idle.stepperDirectionPin = Gpio::Unassigned;
	engineConfiguration->useStepperIdle = true;

	// no analog dividers - all sensors with 3v supply, naked discovery bench setup
	engineConfiguration->analogInputDividerCoefficient = 1;

	// see also setDefaultEtbBiasCurve
}

#if defined(HW_FRANKENSO) && EFI_PROD_CODE && HAL_USE_EEPROM

// todo: page_size + 2
// todo:  CC_SECTION(".nocache")
static uint8_t write_buf[EE_PAGE_SIZE + 10];


#define EEPROM_WRITE_TIME_MS    10          /* time to write one page in ms. Consult datasheet! */

/**
 * https://www.onsemi.com/pdf/datasheet/cat24c32-d.pdf
 * CAT24C32
 */
static const I2CEepromFileConfig i2cee = {
		.barrier_low = 0,
		.barrier_hi = EE_SIZE - 1,
		.size = EE_SIZE,
		.pagesize = EE_PAGE_SIZE,
		.write_time = TIME_MS2I(EEPROM_WRITE_TIME_MS),
    .i2cp = &EE_U2CD,
    .addr = 0x50,
	.write_buf = write_buf
};

extern EepromDevice eepdev_24xx;
static I2CEepromFileStream ifile;

/**
 * set engine_type 61
 */
void setEepromTestConfiguration() {
    engineConfiguration->useEeprom = true;
	engineConfiguration->ignitionPins[2] = Gpio::Unassigned;
	// dirty hack
	brain_pin_markUnused(Gpio::C9);
    efiSetPadMode("I2C", Gpio::A8, PAL_MODE_ALTERNATE(4));
    efiSetPadMode("I2C", Gpio::C9, PAL_MODE_ALTERNATE(4));


    	addConsoleActionI("ee_read",
    		[](int value) {
    			if (ifile.vmt != eepdev_24xx.efsvmt) {
    				EepromFileOpen((EepromFileStream *)&ifile, (EepromFileConfig *)&i2cee, &eepdev_24xx);
    			}

    			ifile.vmt->setposition(&ifile, 0);
//    			chFileStreamSeek(&ifile, 0);
    			int v2;
    			streamRead(&ifile, (uint8_t *)&v2, 4);
    			efiPrintf("EE has %d", v2);

    			v2 += 3;
    			ifile.vmt->setposition(&ifile, 0);
    			streamWrite(&ifile, (uint8_t *)&v2, 4);


    		});
}
#endif //HW_FRANKENSO

// F407 discovery
void setL9779TestConfiguration() {
	// enable_spi 3
	engineConfiguration->is_enabled_spi_3 = true;
	// Wire up spi3
	// green
	engineConfiguration->spi3mosiPin = Gpio::B5;
	// blue
	engineConfiguration->spi3misoPin = Gpio::B4;
	// white
	engineConfiguration->spi3sckPin = Gpio::B3;

	engineConfiguration->l9779spiDevice = SPI_DEVICE_3;
	// orange
	engineConfiguration->l9779_cs = Gpio::D5;
}

#if HW_PROTEUS
/*
 * set engine_type 96
 */

void proteusDcWastegateTest() {
	engineConfiguration->isBoostControlEnabled = true;
	engineConfiguration->etbFunctions[0] = DC_Wastegate;
	engineConfiguration->etbFunctions[1] = DC_None;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_NONE;

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_10;
	setTPS1Calibration(98, 926, 891, 69);

	engineConfiguration->wastegatePositionSensor = EFI_ADC_6;
	engineConfiguration->wastegatePositionClosedVoltage = 0.7;
	engineConfiguration->wastegatePositionOpenedVoltage = 4.0;

	setLuaScript(R"(

mapSensor = Sensor.new("map")
mapSensor : setTimeout(3000)

function onTick()
	local tps = getSensor("TPS1")
	tps = (tps == nil and 0 or tps)
	mapSensor : set(tps)
end

    )");
}

#endif // HW_PROTEUS

static void setBasicNotECUmode() {
    engineConfiguration->trigger.type = trigger_type_e::TT_HALF_MOON;

	// todo: shall we disable map averaging?

	engineConfiguration->wwaeTau = 0.0;
	engineConfiguration->wwaeBeta = 0.0;

	engineConfiguration->fanPin = Gpio::Unassigned;
	engineConfiguration->triggerInputPins[0] = Gpio::Unassigned;

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_NONE;
	engineConfiguration->tps2_1AdcChannel = EFI_ADC_NONE;
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_NONE;
	engineConfiguration->throttlePedalPositionSecondAdcChannel = EFI_ADC_NONE;
	engineConfiguration->vehicleSpeedSensorInputPin = Gpio::Unassigned;
	engineConfiguration->clt.adcChannel = EFI_ADC_NONE;
	engineConfiguration->iat.adcChannel = EFI_ADC_NONE;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_NONE;
}

void setBodyControlUnit() {
	for (int i = 0; i < MAX_CYLINDER_COUNT;i++) {
		engineConfiguration->ignitionPins[i] = Gpio::Unassigned;
		engineConfiguration->injectionPins[i] = Gpio::Unassigned;
	}
  setBasicNotECUmode();
}

void mreSecondaryCan() {
	setBodyControlUnit();

	engineConfiguration->auxAnalogInputs[0] = MRE_IN_TPS;
	engineConfiguration->auxAnalogInputs[1] = MRE_IN_MAP;
	engineConfiguration->auxAnalogInputs[2] = MRE_IN_CLT;
	engineConfiguration->auxAnalogInputs[3] = MRE_IN_IAT;
	// engineConfiguration->auxAnalogInputs[0] =


	// EFI_ADC_14: "32 - AN volt 6"
//	engineConfiguration->afr.hwChannel = EFI_ADC_14;


	setLuaScript( R"(
txPayload = {}
function onTick()
  auxV = getAuxAnalog(0)
  print('Hello analog ' .. auxV )
  -- first byte: integer part, would be autoboxed to int
  txPayload[1] = auxV
  -- second byte: fractional part, would be autoboxed to int, overflow would be ignored
  txPayload[2] = auxV * 256;
  auxV = getAuxAnalog(1)
  print('Hello analog ' .. auxV )
  txPayload[3] = auxV
  txPayload[4] = auxV * 256;
  auxV = getAuxAnalog(2)
  print('Hello analog ' .. auxV )
  txPayload[5] = auxV
  txPayload[6] = auxV * 256;
  txCan(1, 0x600, 1, txPayload)
end
)");

}

void mreBCM() {
    mreSecondaryCan();
    // maybe time to kill this feature is pretty soon?
	engineConfiguration->consumeObdSensors = true;
}

void setBoschHDEV_5_injectors() {
#if HPFP_LOBE_PROFILE_SIZE == 16
static const float hardCodedHpfpLobeProfileQuantityBins[16] = {0.0, 1.0, 4.5, 9.5,
16.5, 25.0, 34.5, 45.0 ,
55.0, 65.5, 75.0, 83.5,
90.5, 95.5, 99.0, 100.0};
	copyArray(config->hpfpLobeProfileQuantityBins, hardCodedHpfpLobeProfileQuantityBins);
#endif // HPFP_LOBE_PROFILE_SIZE
	setHpfpLobeProfileAngle(3);
	setLinearCurve(config->hpfpDeadtimeVoltsBins, 8, 16, 0.5);

	setRpmTableBin(config->hpfpCompensationRpmBins);
	setLinearCurve(config->hpfpCompensationLoadBins, 0.005, 0.120, 0.001);

	// This is the configuration for bosch HDEV 5 injectors
	// all times in microseconds/us
	engineConfiguration->mc33_hvolt = 65;
	engineConfiguration->mc33_i_boost = 13000;
	engineConfiguration->mc33_i_peak = 9400;
	engineConfiguration->mc33_i_hold = 3700;
	engineConfiguration->mc33_t_min_boost = 100;
	engineConfiguration->mc33_t_max_boost = 400;
	engineConfiguration->mc33_t_peak_off = 10;
	engineConfiguration->mc33_t_peak_tot = 700;
	engineConfiguration->mc33_t_bypass = 10;
	engineConfiguration->mc33_t_hold_off = 60;
	engineConfiguration->mc33_t_hold_tot = 10000;

	engineConfiguration->mc33_hpfp_i_peak = 5; // A not mA like above
	engineConfiguration->mc33_hpfp_i_hold = 3;
	engineConfiguration->mc33_hpfp_i_hold_off = 10; // us
	engineConfiguration->mc33_hpfp_max_hold = 10; // this value in ms not us

}

/**
 * set engine_type 107
 */
void setRotary() {
	engineConfiguration->cylindersCount = 2;
	engineConfiguration->firingOrder = FO_1_2;

	engineConfiguration->trigger.type = trigger_type_e::TT_36_2_2_2;
	engineConfiguration->twoStroke = true;

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_MAZDA);
	strcpy(engineConfiguration->engineCode, "13B");
	strcpy(engineConfiguration->vehicleName, "test");

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->injectionPins[2] = Gpio::Unassigned; // injector in default pinout
	engineConfiguration->injectionPins[3] = Gpio::Unassigned;

	engineConfiguration->enableTrailingSparks = true;
	engineConfiguration->trailingCoilPins[0] = Gpio::C9;
	engineConfiguration->trailingCoilPins[1] = Gpio::E10;
}

/**
 * set engine_type 103
 */
void setTest33816EngineConfiguration() {

	// grey
	// default spi3mosiPin PB5
	// white
	// default spi3misoPin PB4
	// violet
	// default spi3sckPin  PB3


	engineConfiguration->triggerSimulatorPins[0] = Gpio::Unassigned;
	engineConfiguration->triggerSimulatorPins[1] = Gpio::Unassigned;

	engineConfiguration->injectionPins[0] = Gpio::B9; // #1
	engineConfiguration->injectionPins[1] = Gpio::E2; // #2
	engineConfiguration->injectionPins[2] = Gpio::B8; // #3
	engineConfiguration->injectionPins[3] = Gpio::B7; // #4


	// blue
	engineConfiguration->mc33816_cs = Gpio::D7;
	// green
	engineConfiguration->mc33816_rstb = Gpio::D4;
	engineConfiguration->sdCardCsPin = Gpio::Unassigned;
	// yellow
	engineConfiguration->mc33816_driven = Gpio::D6;

	engineConfiguration->mc33816_flag0 = Gpio::D3;

	// enable_spi 3
	engineConfiguration->is_enabled_spi_3 = true;
	// Wire up spi3
	engineConfiguration->spi3mosiPin = Gpio::B5;
	engineConfiguration->spi3misoPin = Gpio::B4;
	engineConfiguration->spi3sckPin = Gpio::B3;

	engineConfiguration->isSdCardEnabled = false;

	engineConfiguration->mc33816spiDevice = SPI_DEVICE_3;
}

void proteusLuaDemo() {
#if HW_PROTEUS
	engineConfiguration->tpsMin = 889;
	engineConfiguration->tpsMax = 67;

	engineConfiguration->tps1SecondaryMin = 105;
	engineConfiguration->tps1SecondaryMax = 933;

	strcpy(engineConfiguration->scriptCurveName[2 - 1], "rateofchange");

	strcpy(engineConfiguration->scriptCurveName[3 - 1], "bias");

	/**
	 * for this demo I use ETB just a sample object to control with PID. No reasonable person should consider actually using
	 * Lua for actual intake ETB control while driving around the racing track - hard-coded ETB control is way smarter!
	 */
	static const float defaultBiasBins[] = {
		0, 1, 2, 4, 7, 98, 99, 100
	};
	static const float defaultBiasValues[] = {
		-20, -18, -17, 0, 20, 21, 22, 25
	};

	engineConfiguration->luaOutputPins[0] = Gpio::D12;
	engineConfiguration->luaOutputPins[1] = Gpio::D10;
	engineConfiguration->luaOutputPins[2] = Gpio::D11;

	setLinearCurve(config->scriptCurve2Bins, 0, 8000, 1);
	setLinearCurve(config->scriptCurve2, 0, 100, 1);

	copyArray(config->scriptCurve3Bins, defaultBiasBins);
	copyArray(config->scriptCurve3, defaultBiasValues);

	engineConfiguration->auxAnalogInputs[0] = PROTEUS_IN_ANALOG_VOLT_10;
	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;


	// ETB direction #1 PD10
	engineConfiguration->etbIo[0].directionPin1 = Gpio::Unassigned;
	// ETB control PD12
	engineConfiguration->etbIo[0].controlPin = Gpio::Unassigned;
	// ETB disable PD11
	engineConfiguration->etbIo[0].disablePin = Gpio::Unassigned;

/**
controlIndex = 0
directionIndex = 1

  print('pid output ' .. output)
  print('')



  local duty = (bias + output) / 100

--  isPositive = duty > 0;
--  pwmValue = isPositive and duty or -duty
--  setPwmDuty(controlIndex, pwmValue)

--  dirValue = isPositive and 1 or 0;
--  setPwmDuty(directionIndex, dirValue)

--  print('pwm ' .. pwmValue .. ' dir ' .. dirValue)

 *
 */

	auto script = R"(

startPwm(0, 800, 0.1)
-- direction
startPwm(1, 80, 1.0)
-- disable
startPwm(2, 80, 0.0)

pid = Pid.new(2, 0, 0, -100, 100)

biasCurveIndex = findCurveIndex("bias")

voltageFromCan = nil
canRxAdd(0x600)

function onCanRx(bus, id, dlc, data)
  print('got CAN id=' .. id .. ' dlc='  .. dlc)
  voltageFromCan =   data[2] / 256.0 + data[1]
end

function onTick()
  local targetVoltage = getAuxAnalog(0)

--  local target = interpolate(1, 0, 3.5, 100, targetVoltage)
  local target = interpolate(1, 0, 3.5, 100, voltageFromCan)
-- clamp 0 to 100
  target = math.max(0, target)
  target = math.min(100, target)

  print('Decoded target: ' .. target)

  local tps = getSensor("TPS1")
  tps = (tps == nil and 'invalid TPS' or tps)
  print('Tps ' .. tps)

  local output = pid:get(target, tps)

  local bias = curve(biasCurveIndex, target)
  print('bias ' .. bias)

  local duty = (bias + output) / 100
  isPositive = duty > 0;
  pwmValue = isPositive and duty or -duty
  setPwmDuty(0, pwmValue)

  dirValue = isPositive and 1 or 0;
  setPwmDuty(1, dirValue)

  print('pwm ' .. pwmValue .. ' dir ' .. dirValue)
  print('')
end
				)";
	setLuaScript(script);
#endif
}

void detectBoardType() {
#if HW_HELLEN && EFI_PROD_CODE
	detectHellenBoardType();
#endif //HW_HELLEN EFI_PROD_CODE
	// todo: add board ID detection?
	// see hellen128 which has/had alternative i2c board id?
}

// set engine_type 15
void fuelBenchMode() {
    engineConfiguration->cranking.rpm = 12000;
#if EFI_ENGINE_CONTROL
    setFlatInjectorLag(0);
#endif // EFI_ENGINE_CONTROL
	setTable(config->postCrankingFactor, 1.0f);
	setArrayValues(config->crankingFuelCoef, 1.0f);
	setTable(config->crankingCycleBaseFuel, 1.0f);
	setBasicNotECUmode();
}

#if HW_PROTEUS
// PROTEUS_STIM_QC
// set engine_type 73
void proteusStimQc() {
    engineConfiguration->trigger.type = trigger_type_e::TT_ONE_PLUS_ONE;
	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;
	engineConfiguration->vvtMode[1] = VVT_SINGLE_TOOTH;

   	engineConfiguration->triggerInputPins[0] = PROTEUS_DIGITAL_1;
   	engineConfiguration->triggerInputPins[1] = PROTEUS_DIGITAL_2;
   	engineConfiguration->camInputs[0] = PROTEUS_DIGITAL_3;
   	engineConfiguration->camInputs[1] = PROTEUS_DIGITAL_4;
   	engineConfiguration->vehicleSpeedSensorInputPin = PROTEUS_DIGITAL_5;
   	engineConfiguration->brakePedalPin = PROTEUS_DIGITAL_6;

	setProteusEtbIO();
	// EFI_ADC_13: "Analog Volt 4"
   	engineConfiguration->tps2_1AdcChannel = PROTEUS_IN_TPS2_1;
   	// EFI_ADC_0: "Analog Volt 5"
   	engineConfiguration->tps2_2AdcChannel = PROTEUS_IN_ANALOG_VOLT_5;
   	engineConfiguration->oilPressure.hwChannel = PROTEUS_IN_ANALOG_VOLT_6;
   	// pps2 volt 7

    // pps1 volt 9
    // afr volt 10
    engineConfiguration->oilTempSensor.adcChannel = PROTEUS_IN_ANALOG_VOLT_11;
	setCommonNTCSensor(&engineConfiguration->oilTempSensor, 2700);


	engineConfiguration->auxLinear1.hwChannel = PROTEUS_IN_ANALOG_TEMP_1;
	engineConfiguration->auxLinear2.hwChannel = PROTEUS_IN_ANALOG_TEMP_4;

//   	engineConfiguration->fan2Pin = Gpio::PROTEUS_LS_9;
//   	engineConfiguration->malfunctionIndicatorPin = Gpio::PROTEUS_LS_13;
//   	engineConfiguration->tachOutputPin = Gpio::PROTEUS_LS_14;
//
//   	engineConfiguration->vvtPins[0] = Gpio::PROTEUS_LS_15;
//   	engineConfiguration->vvtPins[1] = Gpio::PROTEUS_LS_16;
}
#endif // HW_PROTEUS

// set engine_type 93
void testEngine6451() {
#ifdef HW_FRANKENSO
  setFrankensoConfiguration();
#endif
  engineConfiguration->trigger.type = trigger_type_e::TT_NARROW_SINGLE_TOOTH;

	setWholeTimingTable(30);
	setTable(config->ignitionIatCorrTable, 0);
	engineConfiguration->cylindersCount = 6;
	engineConfiguration->firingOrder = FO_1_5_3_6_2_4;
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->triggerSimulatorRpm = 4800;
}

