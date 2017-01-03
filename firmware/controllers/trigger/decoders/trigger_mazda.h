/**
 * @file	trigger_mazda.h
 *
 * @date Feb 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef TRIGGER_MAZDA_H_
#define TRIGGER_MAZDA_H_

#include "trigger_structure.h"
#include "engine.h"

#define MIATA_NA_GAP 1.4930f

void initializeMazdaMiataNaShape(TriggerShape *s DECLARE_ENGINE_PARAMETER_S);
void initializeMazdaMiataNb1Shape(TriggerShape *s DECLARE_ENGINE_PARAMETER_S);
void initializeMazdaMiataNb2Crank(TriggerShape *s DECLARE_ENGINE_PARAMETER_S);
void initializeMazdaMiataVVtTestShape(TriggerShape *s DECLARE_ENGINE_PARAMETER_S);
void configureMazdaProtegeSOHC(TriggerShape *s DECLARE_ENGINE_PARAMETER_S);
void configureMazdaProtegeLx(TriggerShape *s DECLARE_ENGINE_PARAMETER_S);

#endif /* TRIGGER_MAZDA_H_ */
