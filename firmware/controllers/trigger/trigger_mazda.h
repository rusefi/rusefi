/**
 * @file	trigger_mazda.h
 *
 * @date Feb 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef TRIGGER_MAZDA_H_
#define TRIGGER_MAZDA_H_

#include "trigger_structure.h"
#include "engine.h"

#define MIATA_NA_GAP 1.4930f

void initializeMazdaMiataNaShape(trigger_shape_s *s);
void initializeMazdaMiataNbShape(trigger_shape_s *s);
void configureMazdaProtegeLx(trigger_shape_s *s);

#endif /* TRIGGER_MAZDA_H_ */
