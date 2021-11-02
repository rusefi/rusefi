#pragma once

#include "electronic_throttle.h"
#include "dc_motor.h"
#include "table_helper.h"
#include "pwm_generator_logic.h"
#include "airmass.h"
#include "injector_model.h"
#include "stepper.h"

#include "gmock/gmock.h"

class MockEtb : public IEtbController {
public:
	MockEtb();
	virtual ~MockEtb();

	// IEtbController mocks
	MOCK_METHOD(void, reset, (), (override));
	MOCK_METHOD(void, update, (), (override));
	MOCK_METHOD(bool, init, (etb_function_e function, DcMotor* motor, pid_s* pidParameters, const ValueProvider3D* pedalMap, bool initializeThrottles), (override));
	MOCK_METHOD(void, setIdlePosition, (percent_t pos), (override));
	MOCK_METHOD(void, setWastegatePosition, (percent_t pos), (override));
	MOCK_METHOD(void, autoCalibrateTps, (), (override));
	MOCK_METHOD(const pid_state_s*, getPidState, (), (const, override));

	// ClosedLoopController mocks
	MOCK_METHOD(expected<percent_t>, getSetpoint, (), (const, override));
	MOCK_METHOD(expected<percent_t>, observePlant, (), (const, override));
	MOCK_METHOD(expected<percent_t>, getOpenLoop, (percent_t setpoint), (const, override));
	MOCK_METHOD(expected<percent_t>, getClosedLoop, (percent_t setpoint, percent_t observation), (override));
	MOCK_METHOD(void, setOutput, (expected<percent_t> outputValue), (override));
};

class MockMotor : public DcMotor {
public:
	MockMotor();
	virtual ~MockMotor();

	MOCK_METHOD(bool, set, (float duty), (override));
	MOCK_METHOD(float, get, (), (const, override));
	MOCK_METHOD(void, enable, (), (override));
	MOCK_METHOD(void, disable, (), (override));
	MOCK_METHOD(bool, isOpenDirection, (), (const, override));
};

class MockVp3d : public ValueProvider3D {
public:
	MockVp3d();
	virtual ~MockVp3d();

	MOCK_METHOD(float, getValue, (float xColumn, float yRow), (const, override));
};

class MockPwm : public IPwm {
public:
	MockPwm();
	virtual ~MockPwm();

	MOCK_METHOD(void, setSimplePwmDutyCycle, (float dutyCycle), (override));
};

class MockOutputPin : public OutputPin {
public:
	MockOutputPin();
	virtual ~MockOutputPin();

	MOCK_METHOD(void, setValue, (int value), (override));
};

class MockExecutor : public TestExecutor {
public:
	MockExecutor();
	virtual ~MockExecutor();

	MOCK_METHOD(void, scheduleByTimestamp, (const char *msg, scheduling_s *scheduling, efitimeus_t timeUs, action_s action), (override));
	MOCK_METHOD(void, scheduleByTimestampNt, (const char *msg, scheduling_s *scheduling, efitime_t timeUs, action_s action), (override));
	MOCK_METHOD(void, scheduleForLater, (scheduling_s *scheduling, int delayUs, action_s action), (override));
	MOCK_METHOD(void, cancel, (scheduling_s*), (override));
};

class MockAirmass : public AirmassVeModelBase {
public:
	MockAirmass();
	virtual ~MockAirmass();

	MockVp3d veTable;

	MOCK_METHOD(AirmassResult, getAirmass, (int rpm), (override));
};

class MockInjectorModel2 : public IInjectorModel {
public:
	MockInjectorModel2();
	virtual ~MockInjectorModel2();

	MOCK_METHOD(void, prepare, (), (override));
	MOCK_METHOD(floatms_t, getInjectionDuration, (float fuelMassGram), (const, override));
	MOCK_METHOD(float, getFuelMassForDuration, (floatms_t duration), (const, override));
};

class MockStepperHardware : public StepperHw {
public:
	MockStepperHardware();
	virtual ~MockStepperHardware();

	MOCK_METHOD(bool, step, (bool positive), (override));
};
