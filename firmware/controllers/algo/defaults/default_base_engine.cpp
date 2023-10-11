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

/* Cylinder to bank mapping */
void setLeftRightBanksNeedBetterName() {
    for (size_t i = 0; i < engineConfiguration->cylindersCount; i++) {
	    engineConfiguration->cylinderBankSelect[i] = i % 2;
    }
}

void setDefaultBaseEngine() {
	// Base Engine Settings
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->displacement = 2;
	engineConfiguration->firingOrder = FO_1_3_4_2;

    // todo: extract constant in instant_rpm_calculator.h?
    engineConfiguration->instantRpmRange = 90;

	engineConfiguration->compressionRatio = 9;

	engineConfiguration->turbochargerFilter = 0.01f;

	engineConfiguration->fuelAlgorithm = LM_SPEED_DENSITY;
	// let's have valid default while we still have the field
	engineConfiguration->debugMode = DBG_INSTANT_RPM;

	// Limits and Fallbacks
	engineConfiguration->rpmHardLimit = 7000;
	engineConfiguration->rpmHardLimitHyst = 50;
	engineConfiguration->cutFuelOnHardLimit = true;
	engineConfiguration->cutSparkOnHardLimit = false;

	engineConfiguration->ALSMinRPM = 400;
	engineConfiguration->ALSMaxRPM = 3200;
	engineConfiguration->ALSMaxDuration = 3.5;
	engineConfiguration->ALSMaxCLT = 105;
//	engineConfiguration->alsMinPps = 10;
	engineConfiguration->alsMinTimeBetween = 5;
	engineConfiguration->alsEtbPosition = 30;
	engineConfiguration->ALSMaxTPS = 5;

	// Trigger
	engineConfiguration->trigger.type = trigger_type_e::TT_TOOTHED_WHEEL_60_2;

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

	engineConfiguration->ignTestOnTime = 3;
	engineConfiguration->ignTestOffTime = 500;
	engineConfiguration->ignTestCount = 3;

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

void setPPSInputs(adc_channel_e pps1, adc_channel_e pps2) {
	engineConfiguration->throttlePedalPositionAdcChannel = pps1;
	engineConfiguration->throttlePedalPositionSecondAdcChannel = pps2;
}

void setTPS1Inputs(adc_channel_e tps1, adc_channel_e tps2) {
	engineConfiguration->tps1_1AdcChannel = tps1;
	engineConfiguration->tps1_2AdcChannel = tps2;
}

void setTPS1Calibration(uint16_t tpsMin, uint16_t tpsMax, uint16_t tps1SecondaryMin, uint16_t tps1SecondaryMax) {
	engineConfiguration->tpsMin = tpsMin;
	engineConfiguration->tpsMax = tpsMax;

	engineConfiguration->tps1SecondaryMin = tps1SecondaryMin;
	engineConfiguration->tps1SecondaryMax = tps1SecondaryMax;
}

void setPPSCalibration(float primaryUp, float primaryDown, float secondaryUp, float secondaryDown) {
	engineConfiguration->throttlePedalUpVoltage = primaryUp;
	engineConfiguration->throttlePedalWOTVoltage = primaryDown;
	engineConfiguration->throttlePedalSecondaryUpVoltage = secondaryUp;
	engineConfiguration->throttlePedalSecondaryWOTVoltage = secondaryDown;
}

void setEtbPID(float p, float i, float d) {
	engineConfiguration->etb.pFactor = p;
	engineConfiguration->etb.iFactor = i;
	engineConfiguration->etb.dFactor = d;
}
