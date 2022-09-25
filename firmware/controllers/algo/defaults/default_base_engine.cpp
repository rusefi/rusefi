#include "pch.h"

#include "defaults.h"
#include "vr_pwm.h"

static void setDefaultAlternatorParameters() {
	engineConfiguration->alternatorOffAboveTps = 120;

	engineConfiguration->targetVBatt = 14;

	engineConfiguration->alternatorControl.offset = 0;
	engineConfiguration->alternatorControl.pFactor = 30;
	engineConfiguration->alternatorControl.periodMs = 100;
}

void setDefaultBaseEngine() {
	// Base Engine Settings
	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.displacement = 2;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	engineConfiguration->compressionRatio = 9;

	engineConfiguration->turbochargerFilter = 0.01f;

	engineConfiguration->fuelAlgorithm = LM_SPEED_DENSITY;
	// let's have valid default while we still have the field
	engineConfiguration->debugMode = DBG_INSTANT_RPM;

	// Limits and Fallbacks
	engineConfiguration->rpmHardLimit = 7000;
	engineConfiguration->cutFuelOnHardLimit = true;
	engineConfiguration->cutSparkOnHardLimit = true;

	engineConfiguration->failedMapFallback = 60;
	engineConfiguration->enableMapEstimationTableFallback = false;

	// Trigger
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;

	engineConfiguration->globalTriggerAngleOffset = 0;

	// Default this to on - if you want to diagnose, turn it off.
	engineConfiguration->silentTriggerError = true;

	// Advanced Trigger

	// Battery and alternator
	engineConfiguration->vbattDividerCoeff = ((float) (15 + 65)) / 15;

#if EFI_ALTERNATOR_CONTROL
	setDefaultAlternatorParameters();
#endif /* EFI_ALTERNATOR_CONTROL */

	// Fuel pump
	engineConfiguration->startUpFuelPumpDuration = 4;

	engineConfiguration->benchTestOnTime = 4;
	engineConfiguration->benchTestOffTime = 500;
	engineConfiguration->benchTestCount = 3;

	// Fans
	engineConfiguration->fanOnTemperature = 95;
	engineConfiguration->fanOffTemperature = 91;
	engineConfiguration->fan2OnTemperature = 95;
	engineConfiguration->fan2OffTemperature = 91;

	// Tachometer
	// 50% duty cycle is the default for tach signal
	engineConfiguration->tachPulseDurationAsDutyCycle = true;
	engineConfiguration->tachPulseDuractionMs = 0.5;
	engineConfiguration->tachPulsePerRev = 1;

	engineConfiguration->etbMinimumPosition = 1;
	engineConfiguration->etbMaximumPosition = 100;

	engineConfiguration->tcuInputSpeedSensorTeeth = 1;
	engineConfiguration->issFilterReciprocal = 2;

	// Check engine light
#if EFI_PROD_CODE
	engineConfiguration->warningPeriod = 10;
#else
	engineConfiguration->warningPeriod = 0;
#endif /* EFI_PROD_CODE */

	setDefaultVrThresholds();

}
