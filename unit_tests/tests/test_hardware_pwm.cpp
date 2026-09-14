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
	// Subsequent writes by either consumer must use the shared timer period.
	a->setDuty(0.5f);
	b->setDuty(0.5f);
	ASSERT_EQ(20000u, timer.period);
	EXPECT_EQ(10000u, timer.compare[0]);
	// PR #10217: B previously became 5% despite requesting 50%.
	EXPECT_EQ(10000u, timer.compare[1]);
	EXPECT_FLOAT_EQ(0.5f, static_cast<float>(timer.compare[1]) / timer.period);
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
	// PR #10217: the wrapper previously rejected this update.
	EXPECT_TRUE(accepted);
	EXPECT_FLOAT_EQ(300, chip.lastFrequency);

	// Updating duty must reuse the newly accepted frequency.
	pwm->setDuty(0.25f);
	EXPECT_FLOAT_EQ(0.25f, chip.lastDuty);
	EXPECT_FLOAT_EQ(300, chip.lastFrequency);
}

TEST_F(HardwarePwmCharacterization, FrequencyOnlyChangePreservesAllTimerDuties) {
	PWMDriver timer;
	PWMDriver otherTimer;
	auto a = initStm32PwmForUnitTest(timer, 0, 1000, 0.25f);
	auto b = initStm32PwmForUnitTest(timer, 1, 1000, 0.75f);
	auto off = initStm32PwmForUnitTest(timer, 2, 1000, 0);
	auto full = initStm32PwmForUnitTest(timer, 3, 1000, 1);
	auto other = initStm32PwmForUnitTest(otherTimer, 0, 1000, 0.5f);
	ASSERT_NE(nullptr, a);
	ASSERT_NE(nullptr, b);
	ASSERT_NE(nullptr, off);
	ASSERT_NE(nullptr, full);
	ASSERT_NE(nullptr, other);
	a->setDuty(0.25f);
	b->setDuty(0.75f);
	off->setDuty(0);
	full->setDuty(1);
	ASSERT_EQ(2000u, timer.period);
	ASSERT_EQ(500u, timer.compare[0]);
	ASSERT_EQ(1500u, timer.compare[1]);
	ASSERT_EQ(0u, timer.compare[2]);
	ASSERT_EQ(2000u, timer.compare[3]);

	ASSERT_TRUE(a->setFrequency(100));
	EXPECT_EQ(20000u, timer.period);
	EXPECT_EQ(5000u, timer.compare[0]);
	EXPECT_EQ(15000u, timer.compare[1]);
	EXPECT_EQ(0u, timer.compare[2]);
	EXPECT_EQ(20000u, timer.compare[3]);
	EXPECT_EQ(2000u, otherTimer.period);
	EXPECT_EQ(1000u, otherTimer.compare[0]);
}

namespace {
class LimitedPwmChip : public RecordingPwmChip {
public:
	int setPadPWM(size_t pin, float frequency, float duty) override {
		if (frequency > 300) {
			return -1;
		}
		return RecordingPwmChip::setPadPWM(pin, frequency, duty);
	}
};
}

TEST_F(HardwarePwmCharacterization, ExternalFrequencyChangePreservesDutyAndRejectedRequestState) {
	LimitedPwmChip chip;
	ASSERT_EQ(static_cast<int>(Gpio::HBRIDGE_1_OUT),
		gpiochip_register(Gpio::HBRIDGE_1_OUT, "H-bridge", chip, 1));
	ASSERT_EQ(1, gpiochips_init());
	auto pwm = gpiochip_tryInitPwm("lua", Gpio::HBRIDGE_1_OUT, 100, 0.5f);
	ASSERT_NE(nullptr, pwm);
	pwm->setDuty(0.75f);
	// 300 Hz succeeds while preserving the most recently set duty.
	EXPECT_TRUE(pwm->setFrequency(300));
	EXPECT_FLOAT_EQ(300, chip.lastFrequency);
	EXPECT_FLOAT_EQ(0.75f, chip.lastDuty);

	EXPECT_FALSE(pwm->setFrequency(400));
	// A refused update must retain the last accepted frequency and duty.
	EXPECT_FLOAT_EQ(300, chip.lastFrequency);
	EXPECT_FLOAT_EQ(0.75f, chip.lastDuty);
	pwm->setDuty(0.25f);
	EXPECT_FLOAT_EQ(300, chip.lastFrequency);
	EXPECT_FLOAT_EQ(0.25f, chip.lastDuty);
}
