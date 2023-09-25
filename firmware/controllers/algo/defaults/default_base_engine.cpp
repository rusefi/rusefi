#include "pch.h"

#include "defaults.h"
#include "vr_pwm.h"
#include "kline.h"
#if HW_PROTEUS
#include "proteus_meta.h"
#endif // HW_PROTEUS

static void setDefaultAlternatorParameters() {
	engineConfiguration->alternatorOffAboveTps = 120;

	engineConfiguration->targetVBatt = 14;

	engineConfiguration->alternatorControl.offset = 0;
	engineConfiguration->alternatorControl.pFactor = 30;
	engineConfiguration->alternatorControl.periodMs = 100;
}

void setGDIFueling() {
	// Use high pressure sensor
	engineConfiguration->injectorPressureType = IPT_High;
	// Automatic compensation of injector flow based on rail pressure
	engineConfiguration->injectorCompensationMode = ICM_SensedRailPressure;
	// Reference rail pressure is 10 000 kPa = 100 bar
	engineConfiguration->fuelReferencePressure = 10000;
	//setting "flat" 0.2 ms injector's lag time
	setArrayValues(engineConfiguration->injector.battLagCorr, 0.2);

	setTable(config->injectionPhase, -200.0f);
	engineConfiguration->injectionTimingMode = InjectionTimingMode::Center;
}

/* Cylinder to bank mapping */
void setLeftRightBanksNeedBetterName() {
    for (size_t i = 0; i < engineConfiguration->cylindersCount; i++) {
	    engineConfiguration->cylinderBankSelect[i] = i % 2;
    }
}

static void setDefaultHPFP() {
#if ! EFI_UNIT_TEST
    // unit tests rely on 'hpfpCamLobes' for isGdiEngine() and we need not-GDI by default for unit tests
	engineConfiguration->hpfpCamLobes = 3;
#endif

// todo: would be nice for unit tests to be happy about these defaults
#if EFI_PROD_CODE
	engineConfiguration->hpfpPumpVolume = 0.290;
#endif
	engineConfiguration->hpfpMinAngle = 10;
	engineConfiguration->hpfpActivationAngle = 30;
	engineConfiguration->hpfpTargetDecay = 2000;
	engineConfiguration->hpfpPidP = 0.01;
	engineConfiguration->hpfpPidI = 0.0003;
	engineConfiguration->hpfpPeakPos = 10;
}

void setDefaultBaseEngine() {
	// Base Engine Settings
	engineConfiguration->displacement = 2;
	setInline4();

	engineConfiguration->compressionRatio = 9;
	engineConfiguration->vssFilterReciprocal = VSS_FILTER_MIN;
	engineConfiguration->boardUseCanTerminator = true;

    setDefaultHPFP();

	engineConfiguration->turbochargerFilter = 0.01f;

	engineConfiguration->fuelAlgorithm = LM_SPEED_DENSITY;
	// let's have valid default while we still have the field
	engineConfiguration->debugMode = DBG_INSTANT_RPM;

	engineConfiguration->boostCutPressure = 300;
	engineConfiguration->boostCutPressureHyst = 20;

	engineConfiguration->primingDelay = 0.5;
	engineConfiguration->vvtControlMinRpm = 500.0;

    engineConfiguration->camDecoder2jzPosition = 95;
    engineConfiguration->camDecoder2jzPrecision = 25;

	// Limits and Fallbacks
	engineConfiguration->rpmHardLimit = 7000;
	engineConfiguration->rpmHardLimitHyst = 50;
	engineConfiguration->cutFuelOnHardLimit = true;
	engineConfiguration->cutSparkOnHardLimit = true;

//	engineConfiguration->totalGearsCount = 5;
//	engineConfiguration->gearRatio[0] = 3.35f;
//	engineConfiguration->gearRatio[1] = 1.99f;
//	engineConfiguration->gearRatio[2] = 1.33f;
//	engineConfiguration->gearRatio[3] = 1.00f;
//	engineConfiguration->gearRatio[4] = 0.72f;

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

	engineConfiguration->idleStepperReactionTime = 3;
	engineConfiguration->idleStepperTotalSteps = 200;
	engineConfiguration->stepperForceParkingEveryRestart = true;
	engineConfiguration->iacByTpsTaper = 2;

	// Advanced Trigger

	// Battery and alternator
	engineConfiguration->vbattDividerCoeff = ((float) (15 + 65)) / 15;

#if EFI_ALTERNATOR_CONTROL
	setDefaultAlternatorParameters();
#endif /* EFI_ALTERNATOR_CONTROL */

	// Fuel pump
	engineConfiguration->startUpFuelPumpDuration = 4;

	engineConfiguration->kLineBaudRate = KLINE_BAUD_RATE;

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

void setPPSInputs(adc_channel_e pps1, adc_channel_e pps2) {
	engineConfiguration->throttlePedalPositionAdcChannel = pps1;
	engineConfiguration->throttlePedalPositionSecondAdcChannel = pps2;
}

void setTPS1Inputs(adc_channel_e tps1, adc_channel_e tps2) {
	engineConfiguration->tps1_1AdcChannel = tps1;
	engineConfiguration->tps1_2AdcChannel = tps2;
}

void setTPS1Calibration(uint16_t tpsMin, uint16_t tpsMax) {
	engineConfiguration->tpsMin = tpsMin;
	engineConfiguration->tpsMax = tpsMax;
}

void setTPS1Calibration(uint16_t tpsMin, uint16_t tpsMax, uint16_t tps1SecondaryMin, uint16_t tps1SecondaryMax) {
    setTPS1Calibration(tpsMin, tpsMax);


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

void setInline4() {
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;
}

void setProteusEtbIO() {
#if HW_PROTEUS && EFI_PROD_CODE
	engineConfiguration->tps1_2AdcChannel = PROTEUS_IN_TPS1_2;
	setPPSInputs(PROTEUS_IN_PPS, PROTEUS_IN_PPS2);
#endif // HW_PROTEUS
}
