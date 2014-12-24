/**
 * @file ec2.h
 *
 * this is a mess because some code is still in C and some is
 * already in C++. trigger structure is C++
 * TODO: rename? merge? Do something with this file
 *
 * @date Apr 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef EC2_H_
#define EC2_H_

#include "global.h"
#include "engine_configuration.h"
#include "event_registry.h"
#include "trigger_structure.h"
#include "table_helper.h"

class FuelSchedule {
public:
	FuelSchedule();
	void clear();
	ActuatorEventList events;

	void addFuelEvents(injection_mode_e mode DECLARE_ENGINE_PARAMETER_S);
	void registerInjectionEvent(
			io_pin_e pin, float angle, bool_t isSimultanious DECLARE_ENGINE_PARAMETER_S);

	uint8_t hasEvents[PWM_PHASE_MAX_COUNT];

};

/**
 * this part of the structure is separate just because so far
 * these fields are not integrated with Tuner Studio. Step by step :)
 */
class engine_configuration2_s {
	// todo: move these fields into Engine class, eliminate this class
public:
	engine_configuration2_s();

	// todo: this should go, too
	engine_configuration_s *engineConfiguration;

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
	FuelSchedule crankingInjectionEvents;
	FuelSchedule injectionEvents;
#endif

	float fsioLastValue[LE_COMMAND_COUNT];

	float sparkAtable[DWELL_CURVE_SIZE];
	float sparkBtable[DWELL_CURVE_SIZE];

	Table2D sparkTable;
	void precalc();

	/**
	 * We are alternating two event lists in order to avoid a potential issue around revolution boundary
	 * when an event is scheduled within the next revolution.
	 */
	IgnitionEventList ignitionEvents[2];

	uint64_t stopEngineRequestTimeNt;
};

void initializeIgnitionActions(float advance, float dwellAngle,
		IgnitionEventList *list DECLARE_ENGINE_PARAMETER_S);

void setDefaultNonPersistentConfiguration(engine_configuration2_s *engineConfiguration2);
void printConfiguration(engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2);

#endif /* EC2_H_ */
