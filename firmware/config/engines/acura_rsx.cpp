/**
 * @file	acura_rsx.cpp
 *
 * set engine_type 27
 *
 * @date Jan 16, 2015
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "acura_rsx.h"

EXTERN_CONFIG;

void setAcuraRSX(DECLARE_CONFIG_PARAMETER_SIGNATURE) {

	// http://injectordynamics.com/injectors/id1300-2/
	engineConfiguration->injector.flow = 1300;

	engineConfiguration->specs.cylindersCount = 4;
}

