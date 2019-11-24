/*
 * @file aux_valves.h
 *
 * @date Nov 25, 2017
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#pragma once

#include "engine.h"

/*
class AuxActor {
public:
	int phaseIndex;
	int valveIndex;
	angle_t extra;

	AngleBasedEvent open;
	AngleBasedEvent close;
};
*/

void initAuxValves(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);
void updateAuxValves(DECLARE_ENGINE_PARAMETER_SIGNATURE);
//void plainPinTurnOn(AuxActor *current);
void plainPinTurnOn(NamedOutputPin *output);
void plainPinTurnOff(NamedOutputPin *output);

