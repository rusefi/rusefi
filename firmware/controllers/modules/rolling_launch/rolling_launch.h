/*
 * @file rolling_launch.h
 *
 * Rolling Launch Control: a driver-held button that, while the car is moving, holds the engine
 * at a captured RPM target (via a graded spark-skip request) so the turbo can be spooled, then
 * releases on button-up at the next slow callback with a flat timing ramp-out so boost is eased in, not slammed.
 *
 * This is NOT the off-throttle antilag_system (ALS). It is a launch-control variant: on-throttle,
 * holds an RPM ceiling captured at the moment the button is pressed.
 */

#pragma once

#include "rolling_launch_state_generated.h"
#include <rusefi/timer.h>

class RollingLaunchControl : public rolling_launch_state_s, public EngineModule {
public:
	using interface_t = RollingLaunchControl;

	void setDefaultConfiguration() override;
	void onSlowCallback() override;
	void onEngineStop() override;
	void onConfigurationChange(engine_configuration_s const* previousConfig) override;

	// Spark-cut ratio summed into the shared hardSparkLimiter to hold the captured RPM ceiling.
	float getSparkSkipRatio() const;
	// Fuel multiplier (1.0 when inactive) adding the boost-building enrichment while held.
	float getFuelCoefficient() const;
	// Ignition timing pulled (deg): flat retard while held, linear decay to zero during ramp-out.
	float getTimingRetard() const;

private:
	void reset();
	Timer m_rampTimer;
	bool m_prevArmed = false;
	float m_rampStartRetard = 0;
};
