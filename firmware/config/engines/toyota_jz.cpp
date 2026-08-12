/*
 * @file toyota_jz.cpp
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

#include "toyota_jz.h"
#include "custom_engine.h"
#include "mazda_miata_vvt.h"

static void common2jz() {
	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_TOYOTA);
	strcpy(engineConfiguration->engineCode, "2JZ");
	strcpy(engineConfiguration->vehicleName, "VVT example");

	engineConfiguration->displacement = 3.0;
	engineConfiguration->cylindersCount = 6;
	engineConfiguration->firingOrder = FO_1_5_3_6_2_4;
	// set ignition_mode 1
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;

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

	engineConfiguration->vvtMode[0] = VVT_TOYOTA_3_TOOTH;

	// set global_trigger_offset_angle 155
	engineConfiguration->globalTriggerAngleOffset = 155; // todo

	engineConfiguration->ignitionMode = IM_WASTED_SPARK; // just for now
  engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->twoWireBatchIgnition = true;

	// todo: these magic values would be hardcoded once we find out proper magic values!
	//	engineConfiguration->scriptSetting[4] = 175 - 45;
	//	engineConfiguration->scriptSetting[5] = 175 + 45;

}
