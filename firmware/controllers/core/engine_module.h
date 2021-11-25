#pragma once

class EngineModule {
public:
	// Called when the engine_configuration_s part of the tune has changed.
	virtual void onConfigurationChange(engine_configuration_s const * /*previousConfig*/) { }

	// Called approx 20Hz
	virtual void onSlowCallback() { }

	// Called approx 200Hz
	virtual void onFastCallback() { }
};
