#pragma once

#include "exhaust_cutout_generated.h"
#include <rusefi/timer.h>
#include "efi_output.h"
#include "pwm_generator_logic.h"

enum class ExhaustCutoutState : uint8_t {
	CLOSED = 0,
	OPENING = 1,
	OPEN = 2,
	CLOSING = 3,
};

enum class ExhaustCutoutTestPhase : uint8_t {
	IDLE = 0,
	KEY_ON_OPEN_1,
	KEY_ON_CLOSE_1,
	KEY_ON_OPEN_2,
	KEY_ON_CLOSE_2,
	ENGINE_ON_OPEN,
	ENGINE_ON_CLOSE,
};

class ExhaustCutoutController : public exhaust_cutout_s, public EngineModule {
public:
	void onSlowCallback() override;
	void onConfigurationChange(engine_configuration_s const* previousConfig) override;

	void initPins();

private:
	bool getInputHigh() const;
	bool evaluateAutoTrigger();
	void setState(ExhaustCutoutState newState);
	void updateLed();
	void deEnergize();
	bool runTestSequence();

	OutputPin m_outputPin;    // Digital output, single PWM output, or H-Bridge IN1
	OutputPin m_hbridgePin2;  // H-Bridge IN2
	OutputPin m_ledPin;
	SimplePwm m_pwmOutput{"cutout"};     // used in PWM mode or H-Bridge IN1
	SimplePwm m_hbridgePwm{"cutout-in2"}; // H-Bridge IN2 PWM

	Timer m_tpsHoldTimer;
	Timer m_closeDelayTimer;
	Timer m_moveTimer;
	Timer m_ledTimer;
	Timer m_testStepTimer;

	ExhaustCutoutState m_state = ExhaustCutoutState::CLOSED;

	ExhaustCutoutTestPhase m_testPhase = ExhaustCutoutTestPhase::IDLE;
	bool m_bootTestDone = false;
	bool m_keyOnTestPending = false;
	bool m_engineOnTestArmed = false;
	bool m_lastEngineRunning = false;
};

void initExhaustCutout();
