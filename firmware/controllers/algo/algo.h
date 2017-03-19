/*
 * @file	algo.h
 *
 * @date Mar 2, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef ALGO_H_
#define ALGO_H_

#include "main.h"
#include "engine_configuration.h"
void initDataStructures(DECLARE_ENGINE_PARAMETER_F);
void initAlgo(Logging *sharedLogger, const engine_configuration_s *engineConfiguration);

#endif /* ALGO_H_ */
