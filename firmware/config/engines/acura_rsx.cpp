/**
 * @file	acura_rsx.cpp
 *
 * set_engine_type 27
 *
 * @date Jan 16, 2015
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "acura_rsx.h"

void setAcuraRSX(engine_configuration_s *engineConfiguration) {

	// http://injectordynamics.com/injectors/id1300-2/
	engineConfiguration->injectorFlow = 1300;

	engineConfiguration->cylindersCount = 4;
}

