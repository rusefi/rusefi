/*
 * @file toyota_jzs147.cpp
 *
 * toyota aristo
 * https://en.wikipedia.org/wiki/Lexus_GS
 * http://rusefi.com/forum/viewtopic.php?f=15&t=969
 *
 * http://wilbo666.pbworks.com/w/page/37134472/2JZ-GTE%20JZS147%20Aristo%20Engine%20Wiring
 *
 * NON VVTi
 * set engine_type 38
 *
 * VVTi
 * set engine_type 44
 *
 * @date Dec 30, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "toyota_jzs147.h"
#include "custom_engine.h"
#include "mazda_miata_vvt.h"

static void common2jz() {
	setFrankensoConfiguration(); // default pinout

	engineConfiguration->displacement = 3.0;
	engineConfiguration->cylindersCount = 6;
	engineConfiguration->firingOrder = FO_1_5_3_6_2_4;
	// set ignition_mode 1
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;

	engineConfiguration->ignitionPins[0] = Gpio::E14;
	engineConfiguration->ignitionPins[1] = Gpio::C7;
	engineConfiguration->ignitionPins[2] = Gpio::C9;
	engineConfiguration->ignitionPins[3] = Gpio::E10;
	engineConfiguration->ignitionPins[4] = Gpio::E8;
	engineConfiguration->ignitionPins[5] = Gpio::E12;


	engineConfiguration->injectionPins[0] = Gpio::B9; // #1
	engineConfiguration->injectionPins[1] = Gpio::E2; // #2
	engineConfiguration->injectionPins[2] = Gpio::B8; // #3
	engineConfiguration->injectionPins[3] = Gpio::B7; // #4
	engineConfiguration->injectionPins[4] = Gpio::E3; // #5
	engineConfiguration->injectionPins[5] = Gpio::E4; // #6

	engineConfiguration->fuelPumpPin = Gpio::Unassigned;

	// chartsize 450
	engineConfiguration->engineChartSize = 450;

	engineConfiguration->map.sensor.type = MT_CUSTOM;

	engineConfiguration->injector.flow = 430;

	/**
	 * http://thesafetyaversionsociety.com/wordpress/wp-content/uploads/2010/08/Troubleshooting-2JZ-GTE.pdf
	 * pages 44&46
	 */
	engineConfiguration->clt.config = {-20, 40, 120.0, 15000, 1200, 200.0, 2700};
	engineConfiguration->iat.config = {-20, 40, 120.0, 15000, 1200, 200.0, 2700};

}

/**
 * TOYOTA_2JZ_GTE_VVTi
 * set engine_type 44
 */
void setToyota_2jz_vics() {
	common2jz();

	setCrankOperationMode();
	engineConfiguration->trigger.type = trigger_type_e::TT_TOOTHED_WHEEL_36_2;

	engineConfiguration->triggerInputPins[0] = Gpio::A5; // crank sensor
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned; // cam sensor will he handled by custom vtti code

	engineConfiguration->camInputs[0] = Gpio::C6;
	engineConfiguration->vvtMode[0] = VVT_TOYOTA_3_TOOTH;

	// set global_trigger_offset_angle 155
	engineConfiguration->globalTriggerAngleOffset = 155; // todo

	engineConfiguration->ignitionMode = IM_WASTED_SPARK; // just for now
	engineConfiguration->injectionMode = IM_BATCH; // just for now

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_TOYOTA);
	strcpy(engineConfiguration->engineCode, "2JZ");
	strcpy(engineConfiguration->vehicleName, "VVT example");

	// todo: these magic values would be hardcoded once we find out proper magic values
	//	engineConfiguration->scriptSetting[4] = 175 - 45;
	//	engineConfiguration->scriptSetting[5] = 175 + 45;

	engineConfiguration->vvtPins[0] = Gpio::E3; // VVT solenoid control
}
