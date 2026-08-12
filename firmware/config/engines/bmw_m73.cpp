/*
 * @file bmw_m73.cpp
 *
 * https://wiki.rusefi.com/BMW_e38_750
 *
 * https://rusefi.com/wiki/index.php?title=Hardware:OEM_connectors#134_pin
 * https://wiki.rusefi.com/HOWTO_electronic_throttle_body
 * Ignition module https://rusefi.com/forum/viewtopic.php?f=4&t=286
 *
 *
 * 1/2 plugs black
 * 2/2 plugs grey
 *
 *
 * ********* |    | OEM | rusEfi | function
 *
 * Plug #1 9 pin
 * ECU pin 4:  GND BRN/ORG
 * ECU pin 6:  GND BRN
 * ECU pin 7:  IN  RED         +12v hot at all times
 * ECU pin 8:  IN  RED/BLU     +12v from ECU relay
 *
 * Plug #2 24 pin
 * ECU pin 3:  CAN xxx/xxx     CAN low
 * ECU pin 4:  CAN xxx/xxx     CAN high
 * ECU pin 23: OUT BRN/BLK BLK ECU relay control, low-side
 *
 * Plug #3 52 pin
 * ECU pin 2:  OUT         WHT injector #4
 * ECU pin 6:  GND             ECU
 * ECU pin 15: OUT         BLK injector #2
 * ECU pin 20: IN          WHT hall effect camshaft sensor signal
 * ECU pin 21: GND BRN     BLK CLT sensor (only on first ECU)
 * ECU pin 22: IN  RED/BRN GRN CLT sensor (only on first ECU)
 * ECU pin 27: OUT         ORG injector #6
 * ECU pin 28: OUT         RED injector #5
 * ECU pin 32: IN          ORG VR positive crankshaft sensor - only 2x 5k per channel, R111 not installed, W1002 not installed
 * ECU pin 34: IN              IAT sensor (only on second ECU)
 * ECU pin 36: IN              Knock Sensor #2
 * ECU pin 37: GND             Knock Sensor GND
 * ECU pin 40: OUT BRN/BLK GRN injector #3
 * ECU pin 41: OUT BRN/WHT BLU injector #1
 * ECU pin 45: GND             crankshaft shield
 * ECU pin 46: IN  BLK     BLU VR negative crankshaft sensor
 * ECU pin 47: GND BRN     BLK IAT sensor Ground (only on second ECU)
 * ECU pin 49: IN              Knock Sensor #1
 * ECU pin 50: GND             Knock Sensor GND
 *
 * Plug #4 40 pin
 * ECU pin 6:  IN              start signal from ignition key
 * ECU pin 17: OUT BLK         engine speed output for gauge cluster
 * ECU pin 26: IN  GRN/BLK RED +12v hot in start & run
 * ECU pin 40: OUT YEL/BRN BRN starter enable
 *
 *
 * Plug #5 9 pin
 * ECU pin 3:  OUT BLK         coil signal, low-side
 * ECU pin 5:  GND BRN         ground
 * ECU pin 6:  OUT BLK         coil signal, low-side
 * ECU pin 9:  OUT BLK     RED coil signal, low-side
 *
 * Frankenso
 * set engine_type 40
 * Manhattan
 * set engine_type 24
 * Proteus
 * set engine_type 63
 *
 * https://raw.githubusercontent.com/wiki/rusefi/rusefi_documentation/oem_docs/VAG/Bosch_0280750009_pinout.jpg
 *
 * @date Nov 1, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "bmw_m73.h"
#include "custom_engine.h"

#if EFI_ELECTRONIC_THROTTLE_BODY
#include "electronic_throttle.h"
#endif // EFI_ELECTRONIC_THROTTLE_BODY

#if HW_PROTEUS
#include "proteus_meta.h"
#endif // HW_PROTEUS

void m73engine() {
	// 13641435991 injector
	engineConfiguration->injector.flow = 180; // cc/min, who knows if this number is real - no good source of info

	engineConfiguration->cylindersCount = 12;
	engineConfiguration->displacement = 5.4;
	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_BMW);
	strcpy(engineConfiguration->engineCode, "M73");
	engineConfiguration->firingOrder = FO_1_7_5_11_3_9_6_12_2_8_4_10;
	engineConfiguration->fuelAlgorithm = engine_load_mode_e::LM_ALPHA_N;
	engineConfiguration->canNbcType = CAN_BUS_NBC_NONE;

	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;

	engineConfiguration->globalTriggerAngleOffset = 90;
	setCrankOperationMode();
	engineConfiguration->trigger.type = trigger_type_e::TT_TOOTHED_WHEEL_60_2;

	// this large engine seems to crank at around only 150 RPM? And happily idle at 400RPM?
	engineConfiguration->cranking.rpm = 350;

	engineConfiguration->ignitionMode = IM_TWO_COILS;

	// set cranking_fuel x
	setTable(config->crankingCycleBaseFuel, 27);

	engineConfiguration->crankingTimingAngle = 15;
	setTable(config->veTable, 45);

	engineConfiguration->cylinderBore = 85.0;
}


// BMW_M73_F
void setBMW_M73_TwoCoilUnitTest() {
	engineConfiguration->camInputs[0] = Gpio::A0; // a random unused pin needed for happy HW CI
	// twoCoil configuration without unit tests ETB setup drama
	m73engine();
}

/**
 * set engine_type 63
 *
 * https://github.com/mck1117/proteus/blob/master/readme_pinout.md
 *
 * black#3 : orange   : injector #1
 * black#4 : blue     : injector #3
 * black#5 : white    : injector #5
 * black#6 : green    : injector #6
 * black#7 : orange   : injector #7
 * black#8 : blue     : injector #9
 * black#9 : white    : injector #11
 * black#15: blue     : injector #2
 * black#16: white    : injector #4
 * black#19: green    : injector #8
 * black#20:          : injector #10
 * black#21:          : injector #12
 *
 *
 * small#5 :          : VR1 pos
 * small#8 : blue     : ETB1-
 * small#13: blue     : VR1 neg
 * small#15: orange   : ETB1+
 * small#18: red      : ignition power / ECU power source
 * small#19: black    : GND
 * small#21: blue     : ETB2-
 * small#22: orange   : ETB2+
 * small#23: red      : ETB/high-side power from main relay
 *
 *
 *
 * white#9 : orange   : +5v
 * white#17: green    : PPS
 * white#18: red      : TPS#2
 * white#23: black    : Sensor Ground
 * white#24: red      : TPS#1
 *
 */
#if HW_PROTEUS
void setEngineBMW_M73_Proteus() {
	m73engine();

	// 12 injectors defined in boards/proteus/board_configuration.cpp


	strcpy(engineConfiguration->vehicleName, "Using Proteus");

	engineConfiguration->triggerInputPins[0] = PROTEUS_VR_1;

	engineConfiguration->camInputs[0] = PROTEUS_DIGITAL_2;

	// no idea why https://wiki.rusefi.com/HOWTO-M73-v12-on-Proteus uses non default CLT pin
//	engineConfiguration->clt.adcChannel = PROTEUS_IN_ANALOG_TEMP_4;
  // newer https://wiki.rusefi.com/HOWTO-custom-harness-lazyharnezz-M73 uses default pin


	engineConfiguration->starterControlPin = Gpio::PROTEUS_LS_14;
	engineConfiguration->startStopButtonPin = PROTEUS_DIGITAL_3;
	engineConfiguration->startStopButtonMode = PI_PULLUP;
	engineConfiguration->fuelPumpPin = Gpio::PROTEUS_LS_16;
	engineConfiguration->fanPin = Gpio::PROTEUS_LS_15;
	engineConfiguration->mainRelayPin = Gpio::PROTEUS_LS_13;

    engineConfiguration->map.sensor.hwChannel = PROTEUS_IN_ANALOG_VOLT_8; // M73 adapter board
    engineConfiguration->map.sensor.type = MT_MPX4250A;

	// tps and pps
#if EFI_ELECTRONIC_THROTTLE_BODY
	setProteusHitachiEtbDefaults();
#endif // EFI_ELECTRONIC_THROTTLE_BODY
 	setPPSCalibration(0.73, 4.0, 0.34, 1.86);
}
#endif // HW_PROTEUS
