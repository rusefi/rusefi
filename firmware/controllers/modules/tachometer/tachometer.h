/*
 * @file tachometer.h
 *
 * @date Aug 18, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

void initTachometer();

class TachometerModule : public EngineModule {
public:
	// TODO: can/should this be slow callback instead?
	void onFastCallback() override;
	void onIgnitionStateChanged(bool ignitionOn) override;

private:
	float getRpm();

	bool m_doTachSweep = false;

	enum class TachState {
		Normal,
		RampUp,
		RampDown,
	};

	TachState m_state = TachState::Normal;
	Timer m_stateChangeTimer;
};
