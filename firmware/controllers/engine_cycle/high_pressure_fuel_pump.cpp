/*
 * @file high_pressure_fuel_pump.cpp
 *
 *
 * todo: there is some similarity with aux_valves.cpp and even map_averaging.cpp maybe reduce code duplication?
 *
 * @date Nov 5, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pin_repository.h"
#include "high_pressure_fuel_pump.h"
#include "spark_logic.h"
#include "map.h"

#if EFI_HPFP

#define LOBE_COUNT 3

class HpfpActor {
public:
	angle_t extra;
	int phaseIndex;

	AngleBasedEvent open;
	AngleBasedEvent close;
	DECLARE_ENGINE_PTR;
};

static HpfpActor actors[LOBE_COUNT];

static void plainPinTurnOff(RegisteredNamedOutputPin *output) {
	output->setLow();
}

void hpfpPlainPinTurnOn(HpfpActor *current);

static void scheduleNextCycle(HpfpActor *actor) {
#if EFI_UNIT_TEST
	Engine *engine = actor->engine;
	EXPAND_Engine;
#endif /* EFI_UNIT_TEST */

	scheduleOrQueue(&actor->open,
	TRIGGER_EVENT_UNDEFINED, getTimeNowNt(), actor->extra + CONFIG(tempHpfpStart),
		{hpfpPlainPinTurnOn, actor }
	PASS_ENGINE_PARAMETER_SUFFIX
	);

	scheduleOrQueue(&actor->close,
	TRIGGER_EVENT_UNDEFINED, getTimeNowNt(),
			actor->extra + CONFIG(tempHpfpStart) + CONFIG(tempHpfpDuration),
			{ plainPinTurnOff, &enginePins.hpfpValve }
	PASS_ENGINE_PARAMETER_SUFFIX
			);
}

void hpfpPlainPinTurnOn(HpfpActor *current) {
	RegisteredNamedOutputPin *output = &enginePins.hpfpValve;
	int highPressureKpa = Sensor::get(SensorType::FuelPressureHigh).Value;
	// very basic control strategy
	if (highPressureKpa < BAR2KPA(50)) {
		output->setHigh();
	}
	scheduleNextCycle(current);
}

void initHPFP(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (!isBrainPinValid(engineConfiguration->hpfpValvePin)) {
		return;
	}

	for (int i = 0; i < LOBE_COUNT; i++) {
		HpfpActor *actor = &actors[i];
		INJECT_ENGINE_REFERENCE(actor);

		actor->extra = 720 / LOBE_COUNT * i;
		scheduleNextCycle(actor);
	}
}

#endif // EFI_HPFP
