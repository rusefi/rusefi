/**
 * @file engine_module.h
 */

#pragma once

#include "engine_configuration.h"

class EngineModule {
public:
	// Called exactly once during boot, before configuration is loaded
	virtual void initNoConfiguration() { }

  virtual void setDefaultConfiguration() { }

	// Called when 'Burn' is invoked
	virtual void onConfigurationChange(engine_configuration_s const * /*previousConfig*/) { }

	// Called approx 20Hz
	virtual void onSlowCallback() { }

	// Called approx 200Hz
	virtual void onFastCallback() { }

	// Called whenever the ignition switch state changes
	virtual void onIgnitionStateChanged(bool /*ignitionOn*/) { }

	// Queried to determine whether this module needs a delayed shutoff, defaults to false
	virtual bool needsDelayedShutoff() { return false; }

	// Called on every successfully decoded tooth of the primary trigger
	virtual void onEnginePhase(float /*rpm*/,
								efitick_t /*edgeTimestamp*/,
								angle_t /*currentPhase*/,
								angle_t /*nextPhase*/)
								{ }

	// adds-remove fuel from final calculation, do not implement any close loop logic here! (ie only for VVL/Nitrous/etc)
	virtual float getFuelCoefficient(){ return 1.0f; }

	// adds-remove timing from final calculation, do not implement any close loop logic here! (ie only for VVL/Nitrous/etc)
	virtual float getTimingModifier() { return 0.0f;}
};
