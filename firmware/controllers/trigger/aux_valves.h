/*
 * aux_valves.h
 *
 * @date Nov 25, 2017
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef CONTROLLERS_TRIGGER_AUX_VALVES_H_
#define CONTROLLERS_TRIGGER_AUX_VALVES_H_

#include "engine.h"

void initAuxValves(Logging *sharedLogger);
void updateAuxValves(DECLARE_ENGINE_PARAMETER_SIGNATURE);

#endif /* CONTROLLERS_TRIGGER_AUX_VALVES_H_ */
