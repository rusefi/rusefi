/*
 * @file mazda_miata_1_6.cpp
 *
 * set_engine_type 41
 *
 * @date Apr 16, 2016
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "engine_template.h"
#include "custom_engine.h"
#include "fsio_impl.h"

EXTERN_ENGINE;

void setMiataNA_1_6_Configuration(DECLARE_ENGINE_PARAMETER_F) {
	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_F);

	engineConfiguration->trigger.type = TT_MAZDA_MIATA_NA;

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_THEN_3_THEN_4_THEN2;

	engineConfiguration->vbattDividerCoeff = 9.75;// ((float) (8.2 + 33)) / 8.2 * 2;

	boardConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;

	// warning light
	/**
	 * to test
	 * set_fsio_setting 0 1800
	 * set_fsio_setting 1 95
	 * set_fsio_setting 3 14
	 *
	 * set_fsio_expression 1 "rpm 0 fsio_setting > coolant 1 fsio_setting > | vbatt 2 fsio_setting < |"
	 * eval "rpm 0 fsio_setting > coolant 1 fsio_setting > | vbatt 2 fsio_setting < |"
	 */
	engineConfiguration->bc.fsio_setting[0] = 6200; // RPM threshold
	engineConfiguration->bc.fsio_setting[1] = 90; // CLT threshold
	engineConfiguration->bc.fsio_setting[2] = 13.5; // voltage threshold

	setFsio(0, GPIOC_13, "rpm 0 fsio_setting > coolant 1 fsio_setting > | vbatt 2 fsio_setting < |" PASS_ENGINE_PARAMETER);

}


