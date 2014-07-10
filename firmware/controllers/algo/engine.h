/**
 * @file	engine.h
 *
 * @date May 21, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */
#ifndef ENGINE_H_
#define ENGINE_H_

#include "main.h"
#include "engine_configuration.h"

class EngineState {
public:
	/**
	 * Access to these two fields is not synchronized in any way - that should work since float read/write are atomic.
	 */
	float iat;
	float clt;
};

class RpmCalculator;

class Engine {
public:
	RpmCalculator *rpmCalculator;
	engine_configuration_s *engineConfiguration;


	EngineState engineState;

	void updateSlowSensors();
};

#endif /* ENGINE_H_ */
