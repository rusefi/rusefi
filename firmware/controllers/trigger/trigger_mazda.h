/**
 * @file	trigger_mazda.h
 *
 * @date Feb 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef TRIGGER_MAZDA_H_
#define TRIGGER_MAZDA_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "trigger_structure.h"
#include "engine_configuration.h"
#include "ec2.h"


void initializeMazdaMiataNbShape(trigger_shape_s *s);
void configureMazdaProtegeLx(engine_configuration_s *engineConfiguration,
		engine_configuration2_s *engineConfiguration2);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TRIGGER_MAZDA_H_ */
