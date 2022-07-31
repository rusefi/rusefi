#pragma once

// TODO: this name is now probably wrong, since it checks injectors/ignition too
struct SensorChecker : public EngineModule {
public:
	void onSlowCallback() override;
	void onIgnitionStateChanged(bool ignitionOn) override;

private:
	bool m_ignitionIsOn = false;
	Timer m_timeSinceIgnOff;
};
