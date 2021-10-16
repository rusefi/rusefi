/*
 * @file aux_valves.h
 *
 * @date Nov 25, 2017
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

void initAuxValves(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void recalculateAuxValveTiming(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void auxPlainPinTurnOn(AuxActor *current);
