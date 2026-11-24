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
		// Legacy Generic4 and 4L6X transmission controllers have four gears.
		// Only the simple solenoid controller may select 5th/6th.
		const int configuredGears = engineConfiguration->totalGearsCount;
		// Use only valid configured forward gears, capped by the physical
		// controller's supported gear count. In particular, Generic4 and GM4L6X
		// must not select fifth/sixth even if a six-speed curve is configured.
		const int modeLimit = engineConfiguration->transmissionControllerMode == TransmissionControllerMode::SimpleTransmissionController ? 6 : 4;
		const int maxGears = configuredGears < modeLimit ? configuredGears : modeLimit;
		switch (getDesiredGear()) {
		case GEAR_1 :
			if (maxGears >= 2) shift(vss.Value, tps.Value, &config->tcu_shiftSpeed12, GEAR_2);
			break;
		case GEAR_2 :
			shift(vss.Value, tps.Value, &config->tcu_shiftSpeed21, GEAR_1, true);
			if (getDesiredGear() == GEAR_2 && maxGears >= 3) shift(vss.Value, tps.Value, &config->tcu_shiftSpeed23, GEAR_3);
			break;
		case GEAR_3 :
			shift(vss.Value, tps.Value, &config->tcu_shiftSpeed32, GEAR_2, true);
			if (getDesiredGear() == GEAR_3 && maxGears >= 4) shift(vss.Value, tps.Value, &config->tcu_shiftSpeed34, GEAR_4);
			break;
		case GEAR_4 :
			shift(vss.Value, tps.Value, &config->tcu_shiftSpeed43, GEAR_3, true);
			if (getDesiredGear() == GEAR_4 && maxGears >= 5) shift(vss.Value, tps.Value, &config->tcu_shiftSpeed45, GEAR_5);
			break;
		case GEAR_5 :
			shift(vss.Value, tps.Value, &config->tcu_shiftSpeed54, GEAR_4, true);
			if (getDesiredGear() == GEAR_5 && maxGears >= 6) shift(vss.Value, tps.Value, &config->tcu_shiftSpeed56, GEAR_6);
			break;
		case GEAR_6 :
			shift(vss.Value, tps.Value, &config->tcu_shiftSpeed65, GEAR_5, true);
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
	int curveSpeed = static_cast<int>(interpolate2d(throttle, config->tcu_shiftTpsBins, *curve));

	// Keep new, unconfigured curves from triggering a shift.
	if (curveSpeed <= 0) return;
	if ((down && speed < curveSpeed) || (!down && speed > curveSpeed)) {
		setDesiredGear(gear);
	}
}

AutomaticGearController* getAutomaticGearController() {
	return &automaticGearController;
}
#endif // EFI_TCU
