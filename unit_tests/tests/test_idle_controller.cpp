/*
 * @file test_idle_controller.cpp
 *
 * @date Oct 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "engine_test_helper.h"
#include "advance_map.h"
#include "tps.h"
#include "pid.h"
#include "fsio_impl.h"
#include "idle_thread.h"
#include "allsensors.h"
#include "engine_controller.h"

extern IdleController idleControllerInstance;
extern int timeNowUs;

#define Q(x) #x
#define QUOTE(x) Q(x)

TEST(idle, fsioPidParameters) {
	WITH_ENGINE_TEST_HELPER(MIATA_NA6_MAP);

	// todo finish this unit test!
	engineConfiguration->useFSIO12ForIdleOffset = true;
	setFsioExpression(QUOTE(MAGIC_OFFSET_FOR_IDLE_OFFSET), "ac_on_switch cfg_idleRpmPid_offset cfg_idleRpmPid2_offset if" PASS_ENGINE_PARAMETER_SUFFIX);

	engineConfiguration->useFSIO13ForIdleMinValue = true;
	setFsioExpression(QUOTE(MAGIC_OFFSET_FOR_IDLE_MIN_VALUE), "ac_on_switch cfg_idleRpmPid_minValue cfg_idleRpmPid2_minValue if" PASS_ENGINE_PARAMETER_SUFFIX);

	eth.engine.periodicSlowCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	ASSERT_EQ(1, hasAcToggle(PASS_ENGINE_PARAMETER_SIGNATURE));
//	ASSERT_EQ(0, getAcToggle(PASS_ENGINE_PARAMETER_SIGNATURE));

//	setMockVoltage(engineConfiguration->acSwitchAdc, 5 PASS_ENGINE_PARAMETER_SUFFIX);
//	ASSERT_EQ(1, getAcToggle(PASS_ENGINE_PARAMETER_SIGNATURE));

//	timeNowUs = MS2US(700);

//	idleControllerInstance.PeriodicTask();

//	ASSERT_EQ(0, engine->acSwitchLastChangeTime);
//	ASSERT_EQ(1, engine->acSwitchState);

}

TEST(idle, pid) {
	print("******************************************* testPidController\r\n");
	pid_s pidS;
	pidS.pFactor = 50;
	pidS.iFactor = 0.5;
	pidS.dFactor = 0;
	pidS.offset = 0;
	pidS.minValue = 10;
	pidS.maxValue = 90;
	pidS.periodMs = 1;

	Pid pid(&pidS);

	ASSERT_EQ( 90,  pid.getOutput(14, 12, 0.1)) << "getValue#90";


	ASSERT_EQ( 10,  pid.getOutput(14, 16, 0.1)) << "getValue#10";
	ASSERT_EQ(10, pid.getOutput(14, 16, 1));

	pid.updateFactors(29, 0, 0);
	ASSERT_EQ(10, pid.getOutput(14, 16, 1));
//	ASSERT_EQ(68, pid.getIntegration());

	ASSERT_EQ(10, pid.getOutput(14, 16, 1));
//	ASSERT_EQ(0, pid.getIntegration());

	ASSERT_EQ(10, pid.getOutput(14, 16, 1));
//	ASSERT_EQ(68, pid.getIntegration());



	pidS.pFactor = 1;
	pidS.iFactor = 0;
	pidS.dFactor = 0;
	pidS.offset = 0;
	pidS.minValue = 0;
	pidS.maxValue = 100;
	pidS.periodMs = 1;

	pid.reset();

	ASSERT_EQ( 50,  pid.getOutput(/*target*/50, /*input*/0)) << "target=50, input=0";
	ASSERT_EQ( 0,  pid.iTerm) << "target=50, input=0 iTerm";

	ASSERT_EQ( 0,  pid.getOutput(/*target*/50, /*input*/70)) << "target=50, input=70";
	ASSERT_EQ( 0,  pid.iTerm) << "target=50, input=70 iTerm";

	ASSERT_EQ( 0,  pid.getOutput(/*target*/50, /*input*/70)) << "target=50, input=70 #2";
	ASSERT_EQ( 0,  pid.iTerm) << "target=50, input=70 iTerm #2";

	ASSERT_EQ( 0,  pid.getOutput(/*target*/50, /*input*/50)) << "target=50, input=50";
	ASSERT_EQ( 0,  pid.iTerm) << "target=50, input=50 iTerm";

}

TEST(idle, timingPid) {
	print("******************************************* testTimingPidController\r\n");

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	// basic engine setup
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);

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

	// setup target rpm curve (we need only 1 value when CLT sensor is disabled)
	const int idleRpmTarget = 700;
	engineConfiguration->cltIdleRpm[0] = idleRpmTarget;
	
	// setup other settings
	engineConfiguration->idleTimingPid = pidS;
	eth.engine.fsioState.fsioTimingAdjustment = 0;
	eth.engine.fsioState.fsioIdleTargetRPMAdjustment = 0;
	eth.engine.engineState.cltTimingCorrection = 0;

	// configure TPS
	engineConfiguration->tpsMin = 0;
	engineConfiguration->tpsMax = 100;
	engineConfiguration->bc.idlePidDeactivationTpsThreshold = 10;
	setMockTpsAdc(0 PASS_ENGINE_PARAMETER_SUFFIX);

	// disable temperature sensors
	eth.engine.sensors.clt = NAN;
	eth.engine.sensors.iat = NAN;

	// all corrections disabled, should be 0
	engineConfiguration->bc.useIdleTimingPidControl = false;
	angle_t corr = getAdvanceCorrections(idleRpmTarget PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ(0, corr) << "getAdvanceCorrections#1";
	
	// basic IDLE PID correction test
	engineConfiguration->bc.useIdleTimingPidControl = true;
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
	setMockTpsAdc(engineConfiguration->bc.idlePidDeactivationTpsThreshold + 1 PASS_ENGINE_PARAMETER_SUFFIX);
	corr = getAdvanceCorrections(idleRpmTarget PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ(0, corr) << "getAdvanceCorrections#6";

	// check if PID correction is interpolated for transient idle-running TPS positions
	setMockTpsAdc(engineConfiguration->bc.idlePidDeactivationTpsThreshold / 2 PASS_ENGINE_PARAMETER_SUFFIX);
	corr = getAdvanceCorrections(baseTestRpm PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_FLOAT_EQ(-5.0f, corr) << "getAdvanceCorrections#7";

}
