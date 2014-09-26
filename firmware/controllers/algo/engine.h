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
#include "ec2.h"

class EngineState {
public:
	/**
	 * Access to these two fields is not synchronized in any way - that should work since float read/write are atomic.
	 */
	float iat;
	float clt;

	/**
	 * By the way:
	 * 'cranking' means engine is not stopped and the rpm are below crankingRpm
	 * 'running' means RPM are above crankingRpm
	 * 'spinning' means the engine is not stopped
	 */
	bool isSpinning;

};

class RpmCalculator;

class Engine {
public:
	RpmCalculator *rpmCalculator;
	engine_configuration_s *engineConfiguration;
	engine_configuration2_s *engineConfiguration2;


	EngineState engineState;

	void updateSlowSensors();
};

#endif /* ENGINE_H_ */
