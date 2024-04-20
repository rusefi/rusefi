#include "pch.h"

#include "gc_auto.h"

#if EFI_TCU
AutomaticGearController automaticGearController;

AutomaticGearController::AutomaticGearController() {
}

void AutomaticGearController::update() {
	auto tps = Sensor::get(SensorType::DriverThrottleIntent);
	auto vss = Sensor::get(SensorType::VehicleSpeed);

	if (getDesiredGear() == NEUTRAL) {
		setDesiredGear(GEAR_1);
	}

	if (tps.Valid && vss.Valid) {
		switch (getDesiredGear()) {
		case GEAR_1 :
			shift(vss.Value, tps.Value, &config->tcu_shiftSpeed12, GEAR_2);
			break;
		case GEAR_2 :
			shift(vss.Value, tps.Value, &config->tcu_shiftSpeed21, GEAR_1, true);
			shift(vss.Value, tps.Value, &config->tcu_shiftSpeed23, GEAR_3);
			break;
		case GEAR_3 :
			shift(vss.Value, tps.Value, &config->tcu_shiftSpeed32, GEAR_2, true);
			shift(vss.Value, tps.Value, &config->tcu_shiftSpeed34, GEAR_4);
			break;
		case GEAR_4 :
			shift(vss.Value, tps.Value, &config->tcu_shiftSpeed43, GEAR_3, true);
			break;
		default :
			break;
		}
	}
	
	GearControllerBase::update();
}

void AutomaticGearController::shift(float speed, float throttle, uint8_t (*curve)[TCU_TABLE_WIDTH], gear_e gear) {
	shift(speed, throttle, curve, gear, false);
}

void AutomaticGearController::shift(float speed, float throttle, uint8_t (*curve)[TCU_TABLE_WIDTH], gear_e gear, bool down) {
	int curveSpeed = interpolate2d(throttle, config->tcu_shiftTpsBins, *curve);

	if ((down && speed < curveSpeed) || (!down && speed > curveSpeed)) {
		setDesiredGear(gear);
	}
}

AutomaticGearController* getAutomaticGearController() {
	return &automaticGearController;
}
#endif // EFI_TCU
