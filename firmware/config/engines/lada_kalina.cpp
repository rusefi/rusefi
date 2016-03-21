/*
 * @file lada_kalina.cpp
 *
 * set_engine_type 39
 *
 * @date  Jan 21, 2016
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "lada_kalina.h"
#include "custom_engine.h"
#include "fsio_impl.h"


EXTERN_ENGINE;

void setLadaKalina(DECLARE_ENGINE_PARAMETER_F) {
	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_F);

	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);


	// starter relay solenoid
	/**
	 * to test
	 * set_fsio_setting 0 5000
	 */
	engineConfiguration->bc.fsio_setting[0] = 500;
	// set_fsio_expression 1 "rpm 0 fsio_setting <"
	setFsioExt(0, GPIOE_3, "rpm 0 fsio_setting <", 150 PASS_ENGINE_PARAMETER);

}
