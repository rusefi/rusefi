/*
 * @file test_idle_controller.cpp
 *
 * @date Oct 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine_test_helper.h"
#include "advance_map.h"
#include "tps.h"
#include "pid.h"
#include "fsio_impl.h"
#include "idle_thread.h"
#include "allsensors.h"
#include "engine_controller.h"
#include "electronic_throttle.h"
#include "sensor.h"

extern IdleController idleControllerInstance;
extern int timeNowUs;

TEST(idle, fsioPidParameters) {
	WITH_ENGINE_TEST_HELPER(MIATA_NA6_MAP);

	engineConfiguration->idleRpmPid.offset = 40;
	engineConfiguration->acIdleExtraOffset = 10;

	engineConfiguration->idleRpmPid.minValue = 30;
	engineConfiguration->acIdleExtraMin = 30;

	engineConfiguration->useFSIO12ForIdleOffset = true;
	applyFsioExpression(QUOTE(MAGIC_OFFSET_FOR_IDLE_OFFSET), "ac_on_switch 0 cfg_acIdleExtraOffset if" PASS_ENGINE_PARAMETER_SUFFIX);

	engineConfiguration->useFSIO13ForIdleMinValue = true;
	applyFsioExpression(QUOTE(MAGIC_OFFSET_FOR_IDLE_MIN_VALUE), "ac_on_switch 0 cfg_acIdleExtraMin if" PASS_ENGINE_PARAMETER_SUFFIX);

	ASSERT_EQ(1, hasAcToggle(PASS_ENGINE_PARAMETER_SIGNATURE));
	setMockState(engineConfiguration->acSwitch, true);
	timeNowUs += MS2US(15);
	ASSERT_TRUE(getAcToggle(PASS_ENGINE_PARAMETER_SIGNATURE));

	eth.engine.periodicSlowCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ(40, getIdlePidOffset(PASS_ENGINE_PARAMETER_SIGNATURE));
	ASSERT_EQ(30, getIdlePidMinValue(PASS_ENGINE_PARAMETER_SIGNATURE));

	setMockState(engineConfiguration->acSwitch, false);
	timeNowUs += MS2US(15);
	ASSERT_FALSE(getAcToggle(PASS_ENGINE_PARAMETER_SIGNATURE));

	eth.engine.periodicSlowCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ(50, getIdlePidOffset(PASS_ENGINE_PARAMETER_SIGNATURE));
	ASSERT_EQ(60, getIdlePidMinValue(PASS_ENGINE_PARAMETER_SIGNATURE));


	// todo finish this unit test!
//	timeNowUs = MS2US(700);
	idleControllerInstance.update();
//	ASSERT_EQ(0, engine->acSwitchLastChangeTime);
//	ASSERT_EQ(1, engine->acSwitchState);
}

// see also util.pid test
TEST(idle, timingPid) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	// set PID settings
	pid_s pidS;
	pidS.pFactor = 0.1;
	pidS.iFactor = 0;
	pidS.dFactor = 0;
	pidS.offset = 0;
	pidS.minValue = -20;
	pidS.maxValue = +20;
	pidS.periodMs = 1;

	// setup TimingPid settings
	engineConfiguration->idleTimingPidDeadZone = 10;
	engineConfiguration->idleTimingPidWorkZone = 100;
	engineConfiguration->idlePidFalloffDeltaRpm = 30;

	// setup target rpm curve
	const int idleRpmTarget = 700;
	setArrayValues<float>(engineConfiguration->cltIdleRpm, idleRpmTarget);
	
	// setup other settings
	engineConfiguration->idleTimingPid = pidS;
	eth.engine.fsioState.fsioTimingAdjustment = 0;
	eth.engine.fsioState.fsioIdleTargetRPMAdjustment = 0;
	eth.engine.engineState.cltTimingCorrection = 0;

	// configure TPS
	engineConfiguration->idlePidDeactivationTpsThreshold = 10;
	Sensor::setMockValue(SensorType::Tps1, 0);

	// all corrections disabled, should be 0
	engineConfiguration->useIdleTimingPidControl = false;
	angle_t corr = getAdvanceCorrections(idleRpmTarget PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ(0, corr) << "getAdvanceCorrections#1";
	
	// basic IDLE PID correction test
	engineConfiguration->useIdleTimingPidControl = true;
	int baseTestRpm = idleRpmTarget + engineConfiguration->idleTimingPidWorkZone;
	corr = getAdvanceCorrections(baseTestRpm PASS_ENGINE_PARAMETER_SUFFIX);
	// (delta_rpm=-100) * (p-factor=0.1) = -10 degrees
	ASSERT_EQ(-10, corr) << "getAdvanceCorrections#2";

	// check if rpm is too close to the target
	corr = getAdvanceCorrections((idleRpmTarget + engineConfiguration->idleTimingPidDeadZone) PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ(0, corr) << "getAdvanceCorrections#3";

	// check if rpm is too high (just outside the workzone and even falloff) so we disable the PID correction
	int tooHighRpm = idleRpmTarget + engineConfiguration->idleTimingPidWorkZone + engineConfiguration->idlePidFalloffDeltaRpm;
	corr = getAdvanceCorrections(tooHighRpm PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ(0, corr) << "getAdvanceCorrections#4";

	// check if rpm is within the falloff zone
	int falloffRpm = idleRpmTarget + engineConfiguration->idleTimingPidWorkZone + (engineConfiguration->idlePidFalloffDeltaRpm / 2);
	corr = getAdvanceCorrections(falloffRpm PASS_ENGINE_PARAMETER_SUFFIX);
	// -(100+30/2) * 0.1 / 2 = -5.75
	ASSERT_FLOAT_EQ(-5.75f, corr) << "getAdvanceCorrections#5";

	// check if PID correction is disabled in running mode (tps > threshold):
	Sensor::setMockValue(SensorType::Tps1, engineConfiguration->idlePidDeactivationTpsThreshold + 1);
	corr = getAdvanceCorrections(idleRpmTarget PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ(0, corr) << "getAdvanceCorrections#6";

	// check if PID correction is interpolated for transient idle-running TPS positions
	Sensor::setMockValue(SensorType::Tps1, engineConfiguration->idlePidDeactivationTpsThreshold / 2);
	corr = getAdvanceCorrections(baseTestRpm PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_FLOAT_EQ(-5.0f, corr) << "getAdvanceCorrections#7";

}
