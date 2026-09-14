#include "pch.h"
#include "gpio/gpio_ext.h"
#include "mock-pwm.h"

class HardwarePwmCharacterization : public testing::Test {
protected:
	void SetUp() override {
		resetStm32PwmForUnitTest();
		resetGpioPwmForUnitTest();
	}

	void TearDown() override {
		resetStm32PwmForUnitTest();
		resetGpioPwmForUnitTest();
	}
};

TEST_F(HardwarePwmCharacterization, SharedTimerFrequencyChangeLeavesSiblingDutyStale) {
	PWMDriver timer;
	auto a = initStm32PwmForUnitTest(timer, 0, 1000, 0.5f);
	auto b = initStm32PwmForUnitTest(timer, 1, 1000, 0.5f);
	ASSERT_NE(nullptr, a);
	ASSERT_NE(nullptr, b);

	// Starting B may reset the shared timer: establish both outputs explicitly.
	a->setDuty(0.5f);
	b->setDuty(0.5f);
	ASSERT_EQ(2000u, timer.period);
	ASSERT_EQ(1000u, timer.compare[0]);
	ASSERT_EQ(1000u, timer.compare[1]);

	ASSERT_TRUE(a->setFrequency(100));
	// Even re-applying both consumers' duties does not repair B's stale period.
	a->setDuty(0.5f);
	b->setDuty(0.5f);
	ASSERT_EQ(20000u, timer.period);
	EXPECT_EQ(10000u, timer.compare[0]);
	// BUG: B should still be 50% (10000 ticks), but actually becomes 5%.
	EXPECT_EQ(1000u, timer.compare[1]);
	EXPECT_FLOAT_EQ(0.05f, static_cast<float>(timer.compare[1]) / timer.period);
}

namespace {
class RecordingPwmChip : public GpioChip {
public:
	int init() override { return 0; }
	int setPadPWM(size_t pin, float frequency, float duty) override {
		EXPECT_EQ(0u, pin);
		lastFrequency = frequency;
		lastDuty = duty;
		return 0;
	}

	float lastFrequency = 0;
	float lastDuty = 0;
};
}

TEST_F(HardwarePwmCharacterization, ExternalFrequencyUpdateKeepsInitialFrequency) {
	RecordingPwmChip chip;
	ASSERT_EQ(static_cast<int>(Gpio::HBRIDGE_1_OUT),
		gpiochip_register(Gpio::HBRIDGE_1_OUT, "H-bridge", chip, 1));
	ASSERT_EQ(1, gpiochips_init());
	auto pwm = gpiochip_tryInitPwm("lua", Gpio::HBRIDGE_1_OUT, 100, 0.5f);
	ASSERT_NE(nullptr, pwm);
	ASSERT_FLOAT_EQ(100, chip.lastFrequency);
	ASSERT_FLOAT_EQ(0.5f, chip.lastDuty);

	// SimplePwm::setFrequency delegates here for a hardware-backed Lua output.
	const bool accepted = pwm->setFrequency(300);
	// BUG: the wrapper should accept/forward 300 Hz to this accepting chip.
	EXPECT_FALSE(accepted);
	EXPECT_FLOAT_EQ(100, chip.lastFrequency);

	// Updating duty also reuses the original cached frequency.
	pwm->setDuty(0.25f);
	EXPECT_FLOAT_EQ(0.25f, chip.lastDuty);
	// BUG: should remain at the newly requested 300 Hz.
	EXPECT_FLOAT_EQ(100, chip.lastFrequency);
}
