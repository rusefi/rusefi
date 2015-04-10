/*
 * @file gm_2_2.cpp
 *
 * set_engine_type 30
 *
 * @date Apr 9, 2015
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "global.h"
#include "gm_2_2.h"

EXTERN_ENGINE;

void setGm2_2(DECLARE_ENGINE_PARAMETER_F) {
	board_configuration_s * boardConfiguration = &engineConfiguration->bc;

	boardConfiguration->triggerInputPins[0] = GPIOC_6;
	boardConfiguration->triggerInputPins[1] = GPIOA_8;

}

