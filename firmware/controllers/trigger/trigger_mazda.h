/**
 * @file	trigger_mazda.h
 *
 * @date Feb 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef TRIGGER_MAZDA_H_
#define TRIGGER_MAZDA_H_

#include "trigger_structure.h"
#include "engine_configuration.h"
#include "ec2.h"

void initializeMazdaMiataNaShape(trigger_shape_s *s);
void initializeMazdaMiataNbShape(trigger_shape_s *s);
void configureMazdaProtegeLx(trigger_shape_s *s);

#endif /* TRIGGER_MAZDA_H_ */
