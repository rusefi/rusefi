/**
 * @file	electronic_throttle.h
 *
 * @date Dec 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "closed_loop_controller.h"
#include "rusefi_types.h"
#include "engine_configuration.h"

void initElectronicThrottle();
void doInitElectronicThrottle();

void setEtbIdlePosition(percent_t pos);
void setEtbWastegatePosition(percent_t pos);
void setEtbLuaAdjustment(percent_t adjustment);
void setHitachiEtbCalibration();

void setToyota89281_33010_pedal_position_sensor();
void setBoschVAGETB();

void setDefaultEtbBiasCurve();
void setDefaultEtbParameters();
void setBoschVNH2SP30Curve();
void setThrottleDutyCycle(percent_t level);
void onConfigurationChangeElectronicThrottleCallback(engine_configuration_s *previousConfiguration);
void unregisterEtbPins();
void setProteusHitachiEtbDefaults();

void etbAutocal(size_t throttleIndex);

class DcMotor;
struct pid_s;
class ValueProvider3D;
struct pid_state_s;

class IEtbController : public ClosedLoopController<percent_t, percent_t>  {
public:
	// Initialize the throttle.
	// returns true if the throttle was initialized, false otherwise.
	virtual bool init(dc_function_e function, DcMotor *motor, pid_s *pidParameters, const ValueProvider3D* pedalMap, bool initializeThrottles = true) = 0;
	virtual void reset() = 0;
	virtual void setIdlePosition(percent_t pos) = 0;
	virtual void setWastegatePosition(percent_t pos) = 0;
	virtual void update() = 0;
	virtual void autoCalibrateTps() = 0;
	virtual bool isEtbMode() const = 0;

	virtual const pid_state_s& getPidState() const = 0;

	virtual void setLuaAdjustment(percent_t adjustment) = 0;
};
