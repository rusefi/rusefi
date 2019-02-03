/**
 * @file	trigger_mazda.h
 *
 * @date Feb 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef TRIGGER_MAZDA_H_
#define TRIGGER_MAZDA_H_

#include "trigger_structure.h"

#define MIATA_NA_GAP 1.4930f

void initializeMazdaMiataNaShape(TriggerShape *s);
void initializeMazdaMiataNb1Shape(TriggerShape *s);
void initializeMazdaMiataNb2Crank(TriggerShape *s);
void initializeMazdaMiataVVtTestShape(TriggerShape *s);
void configureMazdaProtegeSOHC(TriggerShape *s);
void configureMazdaProtegeLx(TriggerShape *s);
void initialize_Mazda_Engine_z5_Shape(TriggerShape *s);

#endif /* TRIGGER_MAZDA_H_ */
