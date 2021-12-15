#include "pch.h"
float getConfigValueByName(const char *name) {
	if (strEqualCaseInsensitive(name, "engineSnifferRpmThreshold"))
		return engineConfiguration->engineSnifferRpmThreshold;
	if (strEqualCaseInsensitive(name, "injector.flow"))
		return engineConfiguration->injector.flow;
	if (strEqualCaseInsensitive(name, "tpsMin"))
		return engineConfiguration->tpsMin;
	if (strEqualCaseInsensitive(name, "tpsMax"))
		return engineConfiguration->tpsMax;
	if (strEqualCaseInsensitive(name, "tpsErrorDetectionTooLow"))
		return engineConfiguration->tpsErrorDetectionTooLow;
	if (strEqualCaseInsensitive(name, "tpsErrorDetectionTooHigh"))
		return engineConfiguration->tpsErrorDetectionTooHigh;
	if (strEqualCaseInsensitive(name, "cranking.baseFuel"))
		return engineConfiguration->cranking.baseFuel;
	if (strEqualCaseInsensitive(name, "cranking.rpm"))
		return engineConfiguration->cranking.rpm;
	if (strEqualCaseInsensitive(name, "primingSquirtDurationMs"))
		return engineConfiguration->primingSquirtDurationMs;
	if (strEqualCaseInsensitive(name, "ignitionDwellForCrankingMs"))
		return engineConfiguration->ignitionDwellForCrankingMs;
	if (strEqualCaseInsensitive(name, "etbRevLimitStart"))
		return engineConfiguration->etbRevLimitStart;
	if (strEqualCaseInsensitive(name, "etbRevLimitRange"))
		return engineConfiguration->etbRevLimitRange;
	if (strEqualCaseInsensitive(name, "map.sensor.lowValue"))
		return engineConfiguration->map.sensor.lowValue;
	if (strEqualCaseInsensitive(name, "map.sensor.highValue"))
		return engineConfiguration->map.sensor.highValue;
	if (strEqualCaseInsensitive(name, "clt.config.tempC_1"))
		return engineConfiguration->clt.config.tempC_1;
	if (strEqualCaseInsensitive(name, "clt.config.tempC_2"))
		return engineConfiguration->clt.config.tempC_2;
	if (strEqualCaseInsensitive(name, "clt.config.tempC_3"))
		return engineConfiguration->clt.config.tempC_3;
	if (strEqualCaseInsensitive(name, "clt.config.resistance_1"))
		return engineConfiguration->clt.config.resistance_1;
	if (strEqualCaseInsensitive(name, "clt.config.resistance_2"))
		return engineConfiguration->clt.config.resistance_2;
	if (strEqualCaseInsensitive(name, "clt.config.resistance_3"))
		return engineConfiguration->clt.config.resistance_3;
	if (strEqualCaseInsensitive(name, "clt.config.bias_resistor"))
		return engineConfiguration->clt.config.bias_resistor;
	if (strEqualCaseInsensitive(name, "iat.config.tempC_1"))
		return engineConfiguration->iat.config.tempC_1;
	if (strEqualCaseInsensitive(name, "iat.config.tempC_2"))
		return engineConfiguration->iat.config.tempC_2;
	if (strEqualCaseInsensitive(name, "iat.config.tempC_3"))
		return engineConfiguration->iat.config.tempC_3;
	if (strEqualCaseInsensitive(name, "iat.config.resistance_1"))
		return engineConfiguration->iat.config.resistance_1;
	if (strEqualCaseInsensitive(name, "iat.config.resistance_2"))
		return engineConfiguration->iat.config.resistance_2;
	if (strEqualCaseInsensitive(name, "iat.config.resistance_3"))
		return engineConfiguration->iat.config.resistance_3;
	if (strEqualCaseInsensitive(name, "iat.config.bias_resistor"))
		return engineConfiguration->iat.config.bias_resistor;
	if (strEqualCaseInsensitive(name, "launchRpm"))
		return engineConfiguration->launchRpm;
	if (strEqualCaseInsensitive(name, "launchTimingRetard"))
		return engineConfiguration->launchTimingRetard;
	if (strEqualCaseInsensitive(name, "hip9011PrescalerAndSDO"))
		return engineConfiguration->hip9011PrescalerAndSDO;
	if (strEqualCaseInsensitive(name, "knockBandCustom"))
		return engineConfiguration->knockBandCustom;
	if (strEqualCaseInsensitive(name, "specs.displacement"))
		return engineConfiguration->specs.displacement;
	if (strEqualCaseInsensitive(name, "specs.cylindersCount"))
		return engineConfiguration->specs.cylindersCount;
	if (strEqualCaseInsensitive(name, "cylinderBore"))
		return engineConfiguration->cylinderBore;
	if (strEqualCaseInsensitive(name, "sensorSnifferRpmThreshold"))
		return engineConfiguration->sensorSnifferRpmThreshold;
	if (strEqualCaseInsensitive(name, "rpmHardLimit"))
		return engineConfiguration->rpmHardLimit;
	if (strEqualCaseInsensitive(name, "extraInjectionOffset"))
		return engineConfiguration->extraInjectionOffset;
	if (strEqualCaseInsensitive(name, "crankingTimingAngle"))
		return engineConfiguration->crankingTimingAngle;
	if (strEqualCaseInsensitive(name, "gapTrackingLengthOverride"))
		return engineConfiguration->gapTrackingLengthOverride;
	if (strEqualCaseInsensitive(name, "maxIdleVss"))
		return engineConfiguration->maxIdleVss;
	if (strEqualCaseInsensitive(name, "minOilPressureAfterStart"))
		return engineConfiguration->minOilPressureAfterStart;
	if (strEqualCaseInsensitive(name, "fixedModeTiming"))
		return engineConfiguration->fixedModeTiming;
	if (strEqualCaseInsensitive(name, "globalTriggerAngleOffset"))
		return engineConfiguration->globalTriggerAngleOffset;
	if (strEqualCaseInsensitive(name, "analogInputDividerCoefficient"))
		return engineConfiguration->analogInputDividerCoefficient;
	if (strEqualCaseInsensitive(name, "vbattDividerCoeff"))
		return engineConfiguration->vbattDividerCoeff;
	if (strEqualCaseInsensitive(name, "fanOnTemperature"))
		return engineConfiguration->fanOnTemperature;
	if (strEqualCaseInsensitive(name, "fanOffTemperature"))
		return engineConfiguration->fanOffTemperature;
	if (strEqualCaseInsensitive(name, "driveWheelRevPerKm"))
		return engineConfiguration->driveWheelRevPerKm;
	if (strEqualCaseInsensitive(name, "canSleepPeriodMs"))
		return engineConfiguration->canSleepPeriodMs;
	if (strEqualCaseInsensitive(name, "byFirmwareVersion"))
		return engineConfiguration->byFirmwareVersion;
	if (strEqualCaseInsensitive(name, "HD44780width"))
		return engineConfiguration->HD44780width;
	if (strEqualCaseInsensitive(name, "HD44780height"))
		return engineConfiguration->HD44780height;
	if (strEqualCaseInsensitive(name, "idle_derivativeFilterLoss"))
		return engineConfiguration->idle_derivativeFilterLoss;
	if (strEqualCaseInsensitive(name, "trailingSparkAngle"))
		return engineConfiguration->trailingSparkAngle;
	if (strEqualCaseInsensitive(name, "trigger.customTotalToothCount"))
		return engineConfiguration->trigger.customTotalToothCount;
	if (strEqualCaseInsensitive(name, "trigger.customSkippedToothCount"))
		return engineConfiguration->trigger.customSkippedToothCount;
	if (strEqualCaseInsensitive(name, "failedMapFallback"))
		return engineConfiguration->failedMapFallback;
	if (strEqualCaseInsensitive(name, "boostControlSafeDutyCycle"))
		return engineConfiguration->boostControlSafeDutyCycle;
	if (strEqualCaseInsensitive(name, "globalFuelCorrection"))
		return engineConfiguration->globalFuelCorrection;
	if (strEqualCaseInsensitive(name, "adcVcc"))
		return engineConfiguration->adcVcc;
	if (strEqualCaseInsensitive(name, "mapCamDetectionAnglePosition"))
		return engineConfiguration->mapCamDetectionAnglePosition;
	if (strEqualCaseInsensitive(name, "afr.v1"))
		return engineConfiguration->afr.v1;
	if (strEqualCaseInsensitive(name, "afr.value1"))
		return engineConfiguration->afr.value1;
	if (strEqualCaseInsensitive(name, "afr.v2"))
		return engineConfiguration->afr.v2;
	if (strEqualCaseInsensitive(name, "afr.value2"))
		return engineConfiguration->afr.value2;
	if (strEqualCaseInsensitive(name, "baroSensor.lowValue"))
		return engineConfiguration->baroSensor.lowValue;
	if (strEqualCaseInsensitive(name, "baroSensor.highValue"))
		return engineConfiguration->baroSensor.highValue;
	if (strEqualCaseInsensitive(name, "idle.solenoidFrequency"))
		return engineConfiguration->idle.solenoidFrequency;
	if (strEqualCaseInsensitive(name, "manIdlePosition"))
		return engineConfiguration->manIdlePosition;
	if (strEqualCaseInsensitive(name, "knockRetardAggression"))
		return engineConfiguration->knockRetardAggression;
	if (strEqualCaseInsensitive(name, "knockRetardReapplyRate"))
		return engineConfiguration->knockRetardReapplyRate;
	if (strEqualCaseInsensitive(name, "knockRetardMaximum"))
		return engineConfiguration->knockRetardMaximum;
	if (strEqualCaseInsensitive(name, "mapCamDetectionThreshold"))
		return engineConfiguration->mapCamDetectionThreshold;
	if (strEqualCaseInsensitive(name, "vssGearRatio"))
		return engineConfiguration->vssGearRatio;
	if (strEqualCaseInsensitive(name, "vssToothCount"))
		return engineConfiguration->vssToothCount;
	if (strEqualCaseInsensitive(name, "mapCamAveragingLength"))
		return engineConfiguration->mapCamAveragingLength;
	if (strEqualCaseInsensitive(name, "triggerSimulatorFrequency"))
		return engineConfiguration->triggerSimulatorFrequency;
	if (strEqualCaseInsensitive(name, "idle_antiwindupFreq"))
		return engineConfiguration->idle_antiwindupFreq;
	if (strEqualCaseInsensitive(name, "acIdleExtraOffset"))
		return engineConfiguration->acIdleExtraOffset;
	if (strEqualCaseInsensitive(name, "can2SleepPeriodMs"))
		return engineConfiguration->can2SleepPeriodMs;
	if (strEqualCaseInsensitive(name, "wastegatePositionMin"))
		return engineConfiguration->wastegatePositionMin;
	if (strEqualCaseInsensitive(name, "wastegatePositionMax"))
		return engineConfiguration->wastegatePositionMax;
	if (strEqualCaseInsensitive(name, "idlePositionMin"))
		return engineConfiguration->idlePositionMin;
	if (strEqualCaseInsensitive(name, "idlePositionMax"))
		return engineConfiguration->idlePositionMax;
	if (strEqualCaseInsensitive(name, "tunerStudioSerialSpeed"))
		return engineConfiguration->tunerStudioSerialSpeed;
	if (strEqualCaseInsensitive(name, "compressionRatio"))
		return engineConfiguration->compressionRatio;
	if (strEqualCaseInsensitive(name, "verboseCanBaseAddress"))
		return engineConfiguration->verboseCanBaseAddress;
	if (strEqualCaseInsensitive(name, "mc33_hvolt"))
		return engineConfiguration->mc33_hvolt;
	if (strEqualCaseInsensitive(name, "minimumBoostClosedLoopMap"))
		return engineConfiguration->minimumBoostClosedLoopMap;
	if (strEqualCaseInsensitive(name, "mapCamSkipFactor"))
		return engineConfiguration->mapCamSkipFactor;
	if (strEqualCaseInsensitive(name, "sdCardPeriodMs"))
		return engineConfiguration->sdCardPeriodMs;
	if (strEqualCaseInsensitive(name, "mapMinBufferLength"))
		return engineConfiguration->mapMinBufferLength;
	if (strEqualCaseInsensitive(name, "idlePidDeactivationTpsThreshold"))
		return engineConfiguration->idlePidDeactivationTpsThreshold;
	if (strEqualCaseInsensitive(name, "stepperParkingExtraSteps"))
		return engineConfiguration->stepperParkingExtraSteps;
	if (strEqualCaseInsensitive(name, "tps1SecondaryMin"))
		return engineConfiguration->tps1SecondaryMin;
	if (strEqualCaseInsensitive(name, "tps1SecondaryMax"))
		return engineConfiguration->tps1SecondaryMax;
	if (strEqualCaseInsensitive(name, "antiLagRpmTreshold"))
		return engineConfiguration->antiLagRpmTreshold;
	if (strEqualCaseInsensitive(name, "startCrankingDuration"))
		return engineConfiguration->startCrankingDuration;
	if (strEqualCaseInsensitive(name, "multisparkMaxRpm"))
		return engineConfiguration->multisparkMaxRpm;
	if (strEqualCaseInsensitive(name, "multisparkMaxSparkingAngle"))
		return engineConfiguration->multisparkMaxSparkingAngle;
	if (strEqualCaseInsensitive(name, "multisparkMaxExtraSparkCount"))
		return engineConfiguration->multisparkMaxExtraSparkCount;
	if (strEqualCaseInsensitive(name, "boostPid.pFactor"))
		return engineConfiguration->boostPid.pFactor;
	if (strEqualCaseInsensitive(name, "boostPid.iFactor"))
		return engineConfiguration->boostPid.iFactor;
	if (strEqualCaseInsensitive(name, "boostPid.dFactor"))
		return engineConfiguration->boostPid.dFactor;
	if (strEqualCaseInsensitive(name, "boostPid.offset"))
		return engineConfiguration->boostPid.offset;
	if (strEqualCaseInsensitive(name, "boostPid.periodMs"))
		return engineConfiguration->boostPid.periodMs;
	if (strEqualCaseInsensitive(name, "boostPid.minValue"))
		return engineConfiguration->boostPid.minValue;
	if (strEqualCaseInsensitive(name, "boostPid.maxValue"))
		return engineConfiguration->boostPid.maxValue;
	if (strEqualCaseInsensitive(name, "boostPwmFrequency"))
		return engineConfiguration->boostPwmFrequency;
	if (strEqualCaseInsensitive(name, "launchSpeedThreshold"))
		return engineConfiguration->launchSpeedThreshold;
	if (strEqualCaseInsensitive(name, "launchTimingRpmRange"))
		return engineConfiguration->launchTimingRpmRange;
	if (strEqualCaseInsensitive(name, "launchFuelAdded"))
		return engineConfiguration->launchFuelAdded;
	if (strEqualCaseInsensitive(name, "launchBoostDuty"))
		return engineConfiguration->launchBoostDuty;
	if (strEqualCaseInsensitive(name, "hardCutRpmRange"))
		return engineConfiguration->hardCutRpmRange;
	if (strEqualCaseInsensitive(name, "launchAdvanceRpmRange"))
		return engineConfiguration->launchAdvanceRpmRange;
	if (strEqualCaseInsensitive(name, "launchTpsTreshold"))
		return engineConfiguration->launchTpsTreshold;
	if (strEqualCaseInsensitive(name, "launchActivateDelay"))
		return engineConfiguration->launchActivateDelay;
	if (strEqualCaseInsensitive(name, "stft.maxIdleRegionRpm"))
		return engineConfiguration->stft.maxIdleRegionRpm;
	if (strEqualCaseInsensitive(name, "stft.maxOverrunLoad"))
		return engineConfiguration->stft.maxOverrunLoad;
	if (strEqualCaseInsensitive(name, "stft.minPowerLoad"))
		return engineConfiguration->stft.minPowerLoad;
	if (strEqualCaseInsensitive(name, "stft.deadband"))
		return engineConfiguration->stft.deadband;
	if (strEqualCaseInsensitive(name, "stft.minClt"))
		return engineConfiguration->stft.minClt;
	if (strEqualCaseInsensitive(name, "stft.minAfr"))
		return engineConfiguration->stft.minAfr;
	if (strEqualCaseInsensitive(name, "stft.maxAfr"))
		return engineConfiguration->stft.maxAfr;
	if (strEqualCaseInsensitive(name, "stft.startupDelay"))
		return engineConfiguration->stft.startupDelay;
	if (strEqualCaseInsensitive(name, "vehicleWeight"))
		return engineConfiguration->vehicleWeight;
	if (strEqualCaseInsensitive(name, "tps2SecondaryMin"))
		return engineConfiguration->tps2SecondaryMin;
	if (strEqualCaseInsensitive(name, "tps2SecondaryMax"))
		return engineConfiguration->tps2SecondaryMax;
	if (strEqualCaseInsensitive(name, "etbNeutralPosition"))
		return engineConfiguration->etbNeutralPosition;
	if (strEqualCaseInsensitive(name, "engineChartSize"))
		return engineConfiguration->engineChartSize;
	if (strEqualCaseInsensitive(name, "idlePidRpmUpperLimit"))
		return engineConfiguration->idlePidRpmUpperLimit;
	if (strEqualCaseInsensitive(name, "turboSpeedSensorMultiplier"))
		return engineConfiguration->turboSpeedSensorMultiplier;
	if (strEqualCaseInsensitive(name, "acIdleRpmBump"))
		return engineConfiguration->acIdleRpmBump;
	if (strEqualCaseInsensitive(name, "warningPeriod"))
		return engineConfiguration->warningPeriod;
	if (strEqualCaseInsensitive(name, "knockDetectionWindowStart"))
		return engineConfiguration->knockDetectionWindowStart;
	if (strEqualCaseInsensitive(name, "knockDetectionWindowEnd"))
		return engineConfiguration->knockDetectionWindowEnd;
	if (strEqualCaseInsensitive(name, "idleStepperReactionTime"))
		return engineConfiguration->idleStepperReactionTime;
	if (strEqualCaseInsensitive(name, "idleStepperTotalSteps"))
		return engineConfiguration->idleStepperTotalSteps;
	if (strEqualCaseInsensitive(name, "noAccelAfterHardLimitPeriodSecs"))
		return engineConfiguration->noAccelAfterHardLimitPeriodSecs;
	if (strEqualCaseInsensitive(name, "mapAveragingSchedulingAtIndex"))
		return engineConfiguration->mapAveragingSchedulingAtIndex;
	if (strEqualCaseInsensitive(name, "tachPulseDuractionMs"))
		return engineConfiguration->tachPulseDuractionMs;
	if (strEqualCaseInsensitive(name, "maxAcRpm"))
		return engineConfiguration->maxAcRpm;
	if (strEqualCaseInsensitive(name, "maxAcTps"))
		return engineConfiguration->maxAcTps;
	if (strEqualCaseInsensitive(name, "maxAcClt"))
		return engineConfiguration->maxAcClt;
	if (strEqualCaseInsensitive(name, "wwaeTau"))
		return engineConfiguration->wwaeTau;
	if (strEqualCaseInsensitive(name, "alternatorControl.pFactor"))
		return engineConfiguration->alternatorControl.pFactor;
	if (strEqualCaseInsensitive(name, "alternatorControl.iFactor"))
		return engineConfiguration->alternatorControl.iFactor;
	if (strEqualCaseInsensitive(name, "alternatorControl.dFactor"))
		return engineConfiguration->alternatorControl.dFactor;
	if (strEqualCaseInsensitive(name, "alternatorControl.offset"))
		return engineConfiguration->alternatorControl.offset;
	if (strEqualCaseInsensitive(name, "alternatorControl.periodMs"))
		return engineConfiguration->alternatorControl.periodMs;
	if (strEqualCaseInsensitive(name, "alternatorControl.minValue"))
		return engineConfiguration->alternatorControl.minValue;
	if (strEqualCaseInsensitive(name, "alternatorControl.maxValue"))
		return engineConfiguration->alternatorControl.maxValue;
	if (strEqualCaseInsensitive(name, "etb.pFactor"))
		return engineConfiguration->etb.pFactor;
	if (strEqualCaseInsensitive(name, "etb.iFactor"))
		return engineConfiguration->etb.iFactor;
	if (strEqualCaseInsensitive(name, "etb.dFactor"))
		return engineConfiguration->etb.dFactor;
	if (strEqualCaseInsensitive(name, "etb.offset"))
		return engineConfiguration->etb.offset;
	if (strEqualCaseInsensitive(name, "etb.periodMs"))
		return engineConfiguration->etb.periodMs;
	if (strEqualCaseInsensitive(name, "etb.minValue"))
		return engineConfiguration->etb.minValue;
	if (strEqualCaseInsensitive(name, "etb.maxValue"))
		return engineConfiguration->etb.maxValue;
	if (strEqualCaseInsensitive(name, "tps2Min"))
		return engineConfiguration->tps2Min;
	if (strEqualCaseInsensitive(name, "tps2Max"))
		return engineConfiguration->tps2Max;
	if (strEqualCaseInsensitive(name, "tachPulsePerRev"))
		return engineConfiguration->tachPulsePerRev;
	if (strEqualCaseInsensitive(name, "mapErrorDetectionTooLow"))
		return engineConfiguration->mapErrorDetectionTooLow;
	if (strEqualCaseInsensitive(name, "mapErrorDetectionTooHigh"))
		return engineConfiguration->mapErrorDetectionTooHigh;
	if (strEqualCaseInsensitive(name, "multisparkSparkDuration"))
		return engineConfiguration->multisparkSparkDuration;
	if (strEqualCaseInsensitive(name, "multisparkDwell"))
		return engineConfiguration->multisparkDwell;
	if (strEqualCaseInsensitive(name, "idleRpmPid.pFactor"))
		return engineConfiguration->idleRpmPid.pFactor;
	if (strEqualCaseInsensitive(name, "idleRpmPid.iFactor"))
		return engineConfiguration->idleRpmPid.iFactor;
	if (strEqualCaseInsensitive(name, "idleRpmPid.dFactor"))
		return engineConfiguration->idleRpmPid.dFactor;
	if (strEqualCaseInsensitive(name, "idleRpmPid.offset"))
		return engineConfiguration->idleRpmPid.offset;
	if (strEqualCaseInsensitive(name, "idleRpmPid.periodMs"))
		return engineConfiguration->idleRpmPid.periodMs;
	if (strEqualCaseInsensitive(name, "idleRpmPid.minValue"))
		return engineConfiguration->idleRpmPid.minValue;
	if (strEqualCaseInsensitive(name, "idleRpmPid.maxValue"))
		return engineConfiguration->idleRpmPid.maxValue;
	if (strEqualCaseInsensitive(name, "wwaeBeta"))
		return engineConfiguration->wwaeBeta;
	if (strEqualCaseInsensitive(name, "throttlePedalUpVoltage"))
		return engineConfiguration->throttlePedalUpVoltage;
	if (strEqualCaseInsensitive(name, "throttlePedalWOTVoltage"))
		return engineConfiguration->throttlePedalWOTVoltage;
	if (strEqualCaseInsensitive(name, "startUpFuelPumpDuration"))
		return engineConfiguration->startUpFuelPumpDuration;
	if (strEqualCaseInsensitive(name, "idlePidRpmDeadZone"))
		return engineConfiguration->idlePidRpmDeadZone;
	if (strEqualCaseInsensitive(name, "targetVBatt"))
		return engineConfiguration->targetVBatt;
	if (strEqualCaseInsensitive(name, "alternatorOffAboveTps"))
		return engineConfiguration->alternatorOffAboveTps;
	if (strEqualCaseInsensitive(name, "afterCrankingIACtaperDuration"))
		return engineConfiguration->afterCrankingIACtaperDuration;
	if (strEqualCaseInsensitive(name, "iacByTpsTaper"))
		return engineConfiguration->iacByTpsTaper;
	if (strEqualCaseInsensitive(name, "tpsAccelLength"))
		return engineConfiguration->tpsAccelLength;
	if (strEqualCaseInsensitive(name, "tpsAccelEnrichmentThreshold"))
		return engineConfiguration->tpsAccelEnrichmentThreshold;
	if (strEqualCaseInsensitive(name, "engineLoadAccelLength"))
		return engineConfiguration->engineLoadAccelLength;
	if (strEqualCaseInsensitive(name, "uartConsoleSerialSpeed"))
		return engineConfiguration->uartConsoleSerialSpeed;
	if (strEqualCaseInsensitive(name, "tpsDecelEnleanmentThreshold"))
		return engineConfiguration->tpsDecelEnleanmentThreshold;
	if (strEqualCaseInsensitive(name, "tpsDecelEnleanmentMultiplier"))
		return engineConfiguration->tpsDecelEnleanmentMultiplier;
	if (strEqualCaseInsensitive(name, "slowAdcAlpha"))
		return engineConfiguration->slowAdcAlpha;
	if (strEqualCaseInsensitive(name, "auxSerialSpeed"))
		return engineConfiguration->auxSerialSpeed;
	if (strEqualCaseInsensitive(name, "throttlePedalSecondaryUpVoltage"))
		return engineConfiguration->throttlePedalSecondaryUpVoltage;
	if (strEqualCaseInsensitive(name, "throttlePedalSecondaryWOTVoltage"))
		return engineConfiguration->throttlePedalSecondaryWOTVoltage;
	if (strEqualCaseInsensitive(name, "verboseCan2BaseAddress"))
		return engineConfiguration->verboseCan2BaseAddress;
	if (strEqualCaseInsensitive(name, "boostCutPressure"))
		return engineConfiguration->boostCutPressure;
	if (strEqualCaseInsensitive(name, "fixedTiming"))
		return engineConfiguration->fixedTiming;
	if (strEqualCaseInsensitive(name, "mapLowValueVoltage"))
		return engineConfiguration->mapLowValueVoltage;
	if (strEqualCaseInsensitive(name, "mapHighValueVoltage"))
		return engineConfiguration->mapHighValueVoltage;
	if (strEqualCaseInsensitive(name, "egoValueShift"))
		return engineConfiguration->egoValueShift;
	if (strEqualCaseInsensitive(name, "crankingIACposition"))
		return engineConfiguration->crankingIACposition;
	if (strEqualCaseInsensitive(name, "tChargeMinRpmMinTps"))
		return engineConfiguration->tChargeMinRpmMinTps;
	if (strEqualCaseInsensitive(name, "tChargeMinRpmMaxTps"))
		return engineConfiguration->tChargeMinRpmMaxTps;
	if (strEqualCaseInsensitive(name, "tChargeMaxRpmMinTps"))
		return engineConfiguration->tChargeMaxRpmMinTps;
	if (strEqualCaseInsensitive(name, "tChargeMaxRpmMaxTps"))
		return engineConfiguration->tChargeMaxRpmMaxTps;
	if (strEqualCaseInsensitive(name, "fan1ExtraIdle"))
		return engineConfiguration->fan1ExtraIdle;
	if (strEqualCaseInsensitive(name, "alternatorPwmFrequency"))
		return engineConfiguration->alternatorPwmFrequency;
	if (strEqualCaseInsensitive(name, "fan2ExtraIdle"))
		return engineConfiguration->fan2ExtraIdle;
	if (strEqualCaseInsensitive(name, "primingDelay"))
		return engineConfiguration->primingDelay;
	if (strEqualCaseInsensitive(name, "fuelReferencePressure"))
		return engineConfiguration->fuelReferencePressure;
	if (strEqualCaseInsensitive(name, "postCrankingFactor"))
		return engineConfiguration->postCrankingFactor;
	if (strEqualCaseInsensitive(name, "postCrankingDurationSec"))
		return engineConfiguration->postCrankingDurationSec;
	if (strEqualCaseInsensitive(name, "auxTempSensor1.config.tempC_1"))
		return engineConfiguration->auxTempSensor1.config.tempC_1;
	if (strEqualCaseInsensitive(name, "auxTempSensor1.config.tempC_2"))
		return engineConfiguration->auxTempSensor1.config.tempC_2;
	if (strEqualCaseInsensitive(name, "auxTempSensor1.config.tempC_3"))
		return engineConfiguration->auxTempSensor1.config.tempC_3;
	if (strEqualCaseInsensitive(name, "auxTempSensor1.config.resistance_1"))
		return engineConfiguration->auxTempSensor1.config.resistance_1;
	if (strEqualCaseInsensitive(name, "auxTempSensor1.config.resistance_2"))
		return engineConfiguration->auxTempSensor1.config.resistance_2;
	if (strEqualCaseInsensitive(name, "auxTempSensor1.config.resistance_3"))
		return engineConfiguration->auxTempSensor1.config.resistance_3;
	if (strEqualCaseInsensitive(name, "auxTempSensor1.config.bias_resistor"))
		return engineConfiguration->auxTempSensor1.config.bias_resistor;
	if (strEqualCaseInsensitive(name, "auxTempSensor2.config.tempC_1"))
		return engineConfiguration->auxTempSensor2.config.tempC_1;
	if (strEqualCaseInsensitive(name, "auxTempSensor2.config.tempC_2"))
		return engineConfiguration->auxTempSensor2.config.tempC_2;
	if (strEqualCaseInsensitive(name, "auxTempSensor2.config.tempC_3"))
		return engineConfiguration->auxTempSensor2.config.tempC_3;
	if (strEqualCaseInsensitive(name, "auxTempSensor2.config.resistance_1"))
		return engineConfiguration->auxTempSensor2.config.resistance_1;
	if (strEqualCaseInsensitive(name, "auxTempSensor2.config.resistance_2"))
		return engineConfiguration->auxTempSensor2.config.resistance_2;
	if (strEqualCaseInsensitive(name, "auxTempSensor2.config.resistance_3"))
		return engineConfiguration->auxTempSensor2.config.resistance_3;
	if (strEqualCaseInsensitive(name, "auxTempSensor2.config.bias_resistor"))
		return engineConfiguration->auxTempSensor2.config.bias_resistor;
	if (strEqualCaseInsensitive(name, "applyNonlinearBelowPulse"))
		return engineConfiguration->applyNonlinearBelowPulse;
	if (strEqualCaseInsensitive(name, "knockSamplingDuration"))
		return engineConfiguration->knockSamplingDuration;
	if (strEqualCaseInsensitive(name, "etbFreq"))
		return engineConfiguration->etbFreq;
	if (strEqualCaseInsensitive(name, "etbWastegatePid.pFactor"))
		return engineConfiguration->etbWastegatePid.pFactor;
	if (strEqualCaseInsensitive(name, "etbWastegatePid.iFactor"))
		return engineConfiguration->etbWastegatePid.iFactor;
	if (strEqualCaseInsensitive(name, "etbWastegatePid.dFactor"))
		return engineConfiguration->etbWastegatePid.dFactor;
	if (strEqualCaseInsensitive(name, "etbWastegatePid.offset"))
		return engineConfiguration->etbWastegatePid.offset;
	if (strEqualCaseInsensitive(name, "etbWastegatePid.periodMs"))
		return engineConfiguration->etbWastegatePid.periodMs;
	if (strEqualCaseInsensitive(name, "etbWastegatePid.minValue"))
		return engineConfiguration->etbWastegatePid.minValue;
	if (strEqualCaseInsensitive(name, "etbWastegatePid.maxValue"))
		return engineConfiguration->etbWastegatePid.maxValue;
	if (strEqualCaseInsensitive(name, "stepperMinDutyCycle"))
		return engineConfiguration->stepperMinDutyCycle;
	if (strEqualCaseInsensitive(name, "stepperMaxDutyCycle"))
		return engineConfiguration->stepperMaxDutyCycle;
	if (strEqualCaseInsensitive(name, "idlePidActivationTime"))
		return engineConfiguration->idlePidActivationTime;
	if (strEqualCaseInsensitive(name, "oilPressure.v1"))
		return engineConfiguration->oilPressure.v1;
	if (strEqualCaseInsensitive(name, "oilPressure.value1"))
		return engineConfiguration->oilPressure.value1;
	if (strEqualCaseInsensitive(name, "oilPressure.v2"))
		return engineConfiguration->oilPressure.v2;
	if (strEqualCaseInsensitive(name, "oilPressure.value2"))
		return engineConfiguration->oilPressure.value2;
	if (strEqualCaseInsensitive(name, "fan2OnTemperature"))
		return engineConfiguration->fan2OnTemperature;
	if (strEqualCaseInsensitive(name, "fan2OffTemperature"))
		return engineConfiguration->fan2OffTemperature;
	if (strEqualCaseInsensitive(name, "coastingFuelCutRpmHigh"))
		return engineConfiguration->coastingFuelCutRpmHigh;
	if (strEqualCaseInsensitive(name, "coastingFuelCutRpmLow"))
		return engineConfiguration->coastingFuelCutRpmLow;
	if (strEqualCaseInsensitive(name, "coastingFuelCutTps"))
		return engineConfiguration->coastingFuelCutTps;
	if (strEqualCaseInsensitive(name, "coastingFuelCutClt"))
		return engineConfiguration->coastingFuelCutClt;
	if (strEqualCaseInsensitive(name, "pidExtraForLowRpm"))
		return engineConfiguration->pidExtraForLowRpm;
	if (strEqualCaseInsensitive(name, "coastingFuelCutMap"))
		return engineConfiguration->coastingFuelCutMap;
	if (strEqualCaseInsensitive(name, "highPressureFuel.v1"))
		return engineConfiguration->highPressureFuel.v1;
	if (strEqualCaseInsensitive(name, "highPressureFuel.value1"))
		return engineConfiguration->highPressureFuel.value1;
	if (strEqualCaseInsensitive(name, "highPressureFuel.v2"))
		return engineConfiguration->highPressureFuel.v2;
	if (strEqualCaseInsensitive(name, "highPressureFuel.value2"))
		return engineConfiguration->highPressureFuel.value2;
	if (strEqualCaseInsensitive(name, "lowPressureFuel.v1"))
		return engineConfiguration->lowPressureFuel.v1;
	if (strEqualCaseInsensitive(name, "lowPressureFuel.value1"))
		return engineConfiguration->lowPressureFuel.value1;
	if (strEqualCaseInsensitive(name, "lowPressureFuel.v2"))
		return engineConfiguration->lowPressureFuel.v2;
	if (strEqualCaseInsensitive(name, "lowPressureFuel.value2"))
		return engineConfiguration->lowPressureFuel.value2;
	if (strEqualCaseInsensitive(name, "tChargeAirCoefMin"))
		return engineConfiguration->tChargeAirCoefMin;
	if (strEqualCaseInsensitive(name, "tChargeAirCoefMax"))
		return engineConfiguration->tChargeAirCoefMax;
	if (strEqualCaseInsensitive(name, "tChargeAirFlowMax"))
		return engineConfiguration->tChargeAirFlowMax;
	if (strEqualCaseInsensitive(name, "tChargeAirIncrLimit"))
		return engineConfiguration->tChargeAirIncrLimit;
	if (strEqualCaseInsensitive(name, "tChargeAirDecrLimit"))
		return engineConfiguration->tChargeAirDecrLimit;
	if (strEqualCaseInsensitive(name, "hip9011Gain"))
		return engineConfiguration->hip9011Gain;
	if (strEqualCaseInsensitive(name, "etb_iTermMin"))
		return engineConfiguration->etb_iTermMin;
	if (strEqualCaseInsensitive(name, "etb_iTermMax"))
		return engineConfiguration->etb_iTermMax;
	if (strEqualCaseInsensitive(name, "etbMaximumPosition"))
		return engineConfiguration->etbMaximumPosition;
	if (strEqualCaseInsensitive(name, "idleTimingPid.pFactor"))
		return engineConfiguration->idleTimingPid.pFactor;
	if (strEqualCaseInsensitive(name, "idleTimingPid.iFactor"))
		return engineConfiguration->idleTimingPid.iFactor;
	if (strEqualCaseInsensitive(name, "idleTimingPid.dFactor"))
		return engineConfiguration->idleTimingPid.dFactor;
	if (strEqualCaseInsensitive(name, "idleTimingPid.offset"))
		return engineConfiguration->idleTimingPid.offset;
	if (strEqualCaseInsensitive(name, "idleTimingPid.periodMs"))
		return engineConfiguration->idleTimingPid.periodMs;
	if (strEqualCaseInsensitive(name, "idleTimingPid.minValue"))
		return engineConfiguration->idleTimingPid.minValue;
	if (strEqualCaseInsensitive(name, "idleTimingPid.maxValue"))
		return engineConfiguration->idleTimingPid.maxValue;
	if (strEqualCaseInsensitive(name, "idleTimingPidDeadZone"))
		return engineConfiguration->idleTimingPidDeadZone;
	if (strEqualCaseInsensitive(name, "tpsAccelFractionPeriod"))
		return engineConfiguration->tpsAccelFractionPeriod;
	if (strEqualCaseInsensitive(name, "tpsAccelFractionDivisor"))
		return engineConfiguration->tpsAccelFractionDivisor;
	if (strEqualCaseInsensitive(name, "idlerpmpid_iTermMin"))
		return engineConfiguration->idlerpmpid_iTermMin;
	if (strEqualCaseInsensitive(name, "stoichRatioPrimary"))
		return engineConfiguration->stoichRatioPrimary;
	if (strEqualCaseInsensitive(name, "idlerpmpid_iTermMax"))
		return engineConfiguration->idlerpmpid_iTermMax;
	if (strEqualCaseInsensitive(name, "stoichRatioSecondary"))
		return engineConfiguration->stoichRatioSecondary;
	if (strEqualCaseInsensitive(name, "etbIdleThrottleRange"))
		return engineConfiguration->etbIdleThrottleRange;
	if (strEqualCaseInsensitive(name, "triggerCompCenterVolt"))
		return engineConfiguration->triggerCompCenterVolt;
	if (strEqualCaseInsensitive(name, "triggerCompHystMin"))
		return engineConfiguration->triggerCompHystMin;
	if (strEqualCaseInsensitive(name, "triggerCompHystMax"))
		return engineConfiguration->triggerCompHystMax;
	if (strEqualCaseInsensitive(name, "triggerCompSensorSatRpm"))
		return engineConfiguration->triggerCompSensorSatRpm;
	if (strEqualCaseInsensitive(name, "idleRpmPid2.pFactor"))
		return engineConfiguration->idleRpmPid2.pFactor;
	if (strEqualCaseInsensitive(name, "idleRpmPid2.iFactor"))
		return engineConfiguration->idleRpmPid2.iFactor;
	if (strEqualCaseInsensitive(name, "idleRpmPid2.dFactor"))
		return engineConfiguration->idleRpmPid2.dFactor;
	if (strEqualCaseInsensitive(name, "idleRpmPid2.offset"))
		return engineConfiguration->idleRpmPid2.offset;
	if (strEqualCaseInsensitive(name, "idleRpmPid2.periodMs"))
		return engineConfiguration->idleRpmPid2.periodMs;
	if (strEqualCaseInsensitive(name, "idleRpmPid2.minValue"))
		return engineConfiguration->idleRpmPid2.minValue;
	if (strEqualCaseInsensitive(name, "idleRpmPid2.maxValue"))
		return engineConfiguration->idleRpmPid2.maxValue;
	if (strEqualCaseInsensitive(name, "mc33_i_boost"))
		return engineConfiguration->mc33_i_boost;
	if (strEqualCaseInsensitive(name, "mc33_i_peak"))
		return engineConfiguration->mc33_i_peak;
	if (strEqualCaseInsensitive(name, "mc33_i_hold"))
		return engineConfiguration->mc33_i_hold;
	if (strEqualCaseInsensitive(name, "mc33_t_max_boost"))
		return engineConfiguration->mc33_t_max_boost;
	if (strEqualCaseInsensitive(name, "mc33_t_peak_off"))
		return engineConfiguration->mc33_t_peak_off;
	if (strEqualCaseInsensitive(name, "mc33_t_peak_tot"))
		return engineConfiguration->mc33_t_peak_tot;
	if (strEqualCaseInsensitive(name, "mc33_t_bypass"))
		return engineConfiguration->mc33_t_bypass;
	if (strEqualCaseInsensitive(name, "mc33_t_hold_off"))
		return engineConfiguration->mc33_t_hold_off;
	if (strEqualCaseInsensitive(name, "mc33_t_hold_tot"))
		return engineConfiguration->mc33_t_hold_tot;
	if (strEqualCaseInsensitive(name, "hpfpCamLobes"))
		return engineConfiguration->hpfpCamLobes;
	if (strEqualCaseInsensitive(name, "hpfpPeakPos"))
		return engineConfiguration->hpfpPeakPos;
	if (strEqualCaseInsensitive(name, "hpfpMinAngle"))
		return engineConfiguration->hpfpMinAngle;
	if (strEqualCaseInsensitive(name, "hpfpPumpVolume"))
		return engineConfiguration->hpfpPumpVolume;
	if (strEqualCaseInsensitive(name, "hpfpActivationAngle"))
		return engineConfiguration->hpfpActivationAngle;
	if (strEqualCaseInsensitive(name, "hpfpPidP"))
		return engineConfiguration->hpfpPidP;
	if (strEqualCaseInsensitive(name, "hpfpPidI"))
		return engineConfiguration->hpfpPidI;
	if (strEqualCaseInsensitive(name, "hpfpTargetDecay"))
		return engineConfiguration->hpfpTargetDecay;
	if (strEqualCaseInsensitive(name, "vssFilterReciprocal"))
		return config->vssFilterReciprocal;
	return EFI_ERROR_CODE;
}
void setConfigValueByName(const char *name, float value) {
	if (strEqualCaseInsensitive(name, "engineSnifferRpmThreshold"))
	{
		engineConfiguration->engineSnifferRpmThreshold = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "injector.flow"))
	{
		engineConfiguration->injector.flow = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tpsMin"))
	{
		engineConfiguration->tpsMin = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tpsMax"))
	{
		engineConfiguration->tpsMax = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tpsErrorDetectionTooLow"))
	{
		engineConfiguration->tpsErrorDetectionTooLow = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tpsErrorDetectionTooHigh"))
	{
		engineConfiguration->tpsErrorDetectionTooHigh = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "cranking.baseFuel"))
	{
		engineConfiguration->cranking.baseFuel = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "cranking.rpm"))
	{
		engineConfiguration->cranking.rpm = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "primingSquirtDurationMs"))
	{
		engineConfiguration->primingSquirtDurationMs = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "ignitionDwellForCrankingMs"))
	{
		engineConfiguration->ignitionDwellForCrankingMs = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etbRevLimitStart"))
	{
		engineConfiguration->etbRevLimitStart = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etbRevLimitRange"))
	{
		engineConfiguration->etbRevLimitRange = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "map.sensor.lowValue"))
	{
		engineConfiguration->map.sensor.lowValue = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "map.sensor.highValue"))
	{
		engineConfiguration->map.sensor.highValue = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "clt.config.tempC_1"))
	{
		engineConfiguration->clt.config.tempC_1 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "clt.config.tempC_2"))
	{
		engineConfiguration->clt.config.tempC_2 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "clt.config.tempC_3"))
	{
		engineConfiguration->clt.config.tempC_3 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "clt.config.resistance_1"))
	{
		engineConfiguration->clt.config.resistance_1 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "clt.config.resistance_2"))
	{
		engineConfiguration->clt.config.resistance_2 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "clt.config.resistance_3"))
	{
		engineConfiguration->clt.config.resistance_3 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "clt.config.bias_resistor"))
	{
		engineConfiguration->clt.config.bias_resistor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "iat.config.tempC_1"))
	{
		engineConfiguration->iat.config.tempC_1 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "iat.config.tempC_2"))
	{
		engineConfiguration->iat.config.tempC_2 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "iat.config.tempC_3"))
	{
		engineConfiguration->iat.config.tempC_3 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "iat.config.resistance_1"))
	{
		engineConfiguration->iat.config.resistance_1 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "iat.config.resistance_2"))
	{
		engineConfiguration->iat.config.resistance_2 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "iat.config.resistance_3"))
	{
		engineConfiguration->iat.config.resistance_3 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "iat.config.bias_resistor"))
	{
		engineConfiguration->iat.config.bias_resistor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "launchRpm"))
	{
		engineConfiguration->launchRpm = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "launchTimingRetard"))
	{
		engineConfiguration->launchTimingRetard = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "hip9011PrescalerAndSDO"))
	{
		engineConfiguration->hip9011PrescalerAndSDO = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "knockBandCustom"))
	{
		engineConfiguration->knockBandCustom = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "specs.displacement"))
	{
		engineConfiguration->specs.displacement = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "specs.cylindersCount"))
	{
		engineConfiguration->specs.cylindersCount = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "cylinderBore"))
	{
		engineConfiguration->cylinderBore = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "sensorSnifferRpmThreshold"))
	{
		engineConfiguration->sensorSnifferRpmThreshold = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "rpmHardLimit"))
	{
		engineConfiguration->rpmHardLimit = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "extraInjectionOffset"))
	{
		engineConfiguration->extraInjectionOffset = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "crankingTimingAngle"))
	{
		engineConfiguration->crankingTimingAngle = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "gapTrackingLengthOverride"))
	{
		engineConfiguration->gapTrackingLengthOverride = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "maxIdleVss"))
	{
		engineConfiguration->maxIdleVss = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "minOilPressureAfterStart"))
	{
		engineConfiguration->minOilPressureAfterStart = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "fixedModeTiming"))
	{
		engineConfiguration->fixedModeTiming = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "globalTriggerAngleOffset"))
	{
		engineConfiguration->globalTriggerAngleOffset = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "analogInputDividerCoefficient"))
	{
		engineConfiguration->analogInputDividerCoefficient = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "vbattDividerCoeff"))
	{
		engineConfiguration->vbattDividerCoeff = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "fanOnTemperature"))
	{
		engineConfiguration->fanOnTemperature = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "fanOffTemperature"))
	{
		engineConfiguration->fanOffTemperature = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "driveWheelRevPerKm"))
	{
		engineConfiguration->driveWheelRevPerKm = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "canSleepPeriodMs"))
	{
		engineConfiguration->canSleepPeriodMs = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "byFirmwareVersion"))
	{
		engineConfiguration->byFirmwareVersion = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "HD44780width"))
	{
		engineConfiguration->HD44780width = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "HD44780height"))
	{
		engineConfiguration->HD44780height = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idle_derivativeFilterLoss"))
	{
		engineConfiguration->idle_derivativeFilterLoss = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "trailingSparkAngle"))
	{
		engineConfiguration->trailingSparkAngle = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "trigger.customTotalToothCount"))
	{
		engineConfiguration->trigger.customTotalToothCount = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "trigger.customSkippedToothCount"))
	{
		engineConfiguration->trigger.customSkippedToothCount = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "failedMapFallback"))
	{
		engineConfiguration->failedMapFallback = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boostControlSafeDutyCycle"))
	{
		engineConfiguration->boostControlSafeDutyCycle = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "globalFuelCorrection"))
	{
		engineConfiguration->globalFuelCorrection = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "adcVcc"))
	{
		engineConfiguration->adcVcc = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mapCamDetectionAnglePosition"))
	{
		engineConfiguration->mapCamDetectionAnglePosition = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "afr.v1"))
	{
		engineConfiguration->afr.v1 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "afr.value1"))
	{
		engineConfiguration->afr.value1 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "afr.v2"))
	{
		engineConfiguration->afr.v2 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "afr.value2"))
	{
		engineConfiguration->afr.value2 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "baroSensor.lowValue"))
	{
		engineConfiguration->baroSensor.lowValue = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "baroSensor.highValue"))
	{
		engineConfiguration->baroSensor.highValue = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idle.solenoidFrequency"))
	{
		engineConfiguration->idle.solenoidFrequency = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "manIdlePosition"))
	{
		engineConfiguration->manIdlePosition = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "knockRetardAggression"))
	{
		engineConfiguration->knockRetardAggression = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "knockRetardReapplyRate"))
	{
		engineConfiguration->knockRetardReapplyRate = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "knockRetardMaximum"))
	{
		engineConfiguration->knockRetardMaximum = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mapCamDetectionThreshold"))
	{
		engineConfiguration->mapCamDetectionThreshold = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "vssGearRatio"))
	{
		engineConfiguration->vssGearRatio = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "vssToothCount"))
	{
		engineConfiguration->vssToothCount = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mapCamAveragingLength"))
	{
		engineConfiguration->mapCamAveragingLength = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "triggerSimulatorFrequency"))
	{
		engineConfiguration->triggerSimulatorFrequency = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idle_antiwindupFreq"))
	{
		engineConfiguration->idle_antiwindupFreq = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "acIdleExtraOffset"))
	{
		engineConfiguration->acIdleExtraOffset = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "can2SleepPeriodMs"))
	{
		engineConfiguration->can2SleepPeriodMs = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "wastegatePositionMin"))
	{
		engineConfiguration->wastegatePositionMin = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "wastegatePositionMax"))
	{
		engineConfiguration->wastegatePositionMax = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idlePositionMin"))
	{
		engineConfiguration->idlePositionMin = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idlePositionMax"))
	{
		engineConfiguration->idlePositionMax = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tunerStudioSerialSpeed"))
	{
		engineConfiguration->tunerStudioSerialSpeed = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "compressionRatio"))
	{
		engineConfiguration->compressionRatio = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "verboseCanBaseAddress"))
	{
		engineConfiguration->verboseCanBaseAddress = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mc33_hvolt"))
	{
		engineConfiguration->mc33_hvolt = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "minimumBoostClosedLoopMap"))
	{
		engineConfiguration->minimumBoostClosedLoopMap = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mapCamSkipFactor"))
	{
		engineConfiguration->mapCamSkipFactor = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "sdCardPeriodMs"))
	{
		engineConfiguration->sdCardPeriodMs = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mapMinBufferLength"))
	{
		engineConfiguration->mapMinBufferLength = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idlePidDeactivationTpsThreshold"))
	{
		engineConfiguration->idlePidDeactivationTpsThreshold = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "stepperParkingExtraSteps"))
	{
		engineConfiguration->stepperParkingExtraSteps = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tps1SecondaryMin"))
	{
		engineConfiguration->tps1SecondaryMin = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tps1SecondaryMax"))
	{
		engineConfiguration->tps1SecondaryMax = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "antiLagRpmTreshold"))
	{
		engineConfiguration->antiLagRpmTreshold = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "startCrankingDuration"))
	{
		engineConfiguration->startCrankingDuration = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "multisparkMaxRpm"))
	{
		engineConfiguration->multisparkMaxRpm = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "multisparkMaxSparkingAngle"))
	{
		engineConfiguration->multisparkMaxSparkingAngle = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "multisparkMaxExtraSparkCount"))
	{
		engineConfiguration->multisparkMaxExtraSparkCount = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boostPid.pFactor"))
	{
		engineConfiguration->boostPid.pFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boostPid.iFactor"))
	{
		engineConfiguration->boostPid.iFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boostPid.dFactor"))
	{
		engineConfiguration->boostPid.dFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boostPid.offset"))
	{
		engineConfiguration->boostPid.offset = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boostPid.periodMs"))
	{
		engineConfiguration->boostPid.periodMs = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boostPid.minValue"))
	{
		engineConfiguration->boostPid.minValue = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boostPid.maxValue"))
	{
		engineConfiguration->boostPid.maxValue = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boostPwmFrequency"))
	{
		engineConfiguration->boostPwmFrequency = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "launchSpeedThreshold"))
	{
		engineConfiguration->launchSpeedThreshold = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "launchTimingRpmRange"))
	{
		engineConfiguration->launchTimingRpmRange = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "launchFuelAdded"))
	{
		engineConfiguration->launchFuelAdded = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "launchBoostDuty"))
	{
		engineConfiguration->launchBoostDuty = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "hardCutRpmRange"))
	{
		engineConfiguration->hardCutRpmRange = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "launchAdvanceRpmRange"))
	{
		engineConfiguration->launchAdvanceRpmRange = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "launchTpsTreshold"))
	{
		engineConfiguration->launchTpsTreshold = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "launchActivateDelay"))
	{
		engineConfiguration->launchActivateDelay = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "stft.maxIdleRegionRpm"))
	{
		engineConfiguration->stft.maxIdleRegionRpm = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "stft.maxOverrunLoad"))
	{
		engineConfiguration->stft.maxOverrunLoad = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "stft.minPowerLoad"))
	{
		engineConfiguration->stft.minPowerLoad = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "stft.deadband"))
	{
		engineConfiguration->stft.deadband = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "stft.minClt"))
	{
		engineConfiguration->stft.minClt = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "stft.minAfr"))
	{
		engineConfiguration->stft.minAfr = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "stft.maxAfr"))
	{
		engineConfiguration->stft.maxAfr = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "stft.startupDelay"))
	{
		engineConfiguration->stft.startupDelay = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "vehicleWeight"))
	{
		engineConfiguration->vehicleWeight = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tps2SecondaryMin"))
	{
		engineConfiguration->tps2SecondaryMin = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tps2SecondaryMax"))
	{
		engineConfiguration->tps2SecondaryMax = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etbNeutralPosition"))
	{
		engineConfiguration->etbNeutralPosition = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "engineChartSize"))
	{
		engineConfiguration->engineChartSize = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idlePidRpmUpperLimit"))
	{
		engineConfiguration->idlePidRpmUpperLimit = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "turboSpeedSensorMultiplier"))
	{
		engineConfiguration->turboSpeedSensorMultiplier = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "acIdleRpmBump"))
	{
		engineConfiguration->acIdleRpmBump = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "warningPeriod"))
	{
		engineConfiguration->warningPeriod = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "knockDetectionWindowStart"))
	{
		engineConfiguration->knockDetectionWindowStart = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "knockDetectionWindowEnd"))
	{
		engineConfiguration->knockDetectionWindowEnd = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleStepperReactionTime"))
	{
		engineConfiguration->idleStepperReactionTime = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleStepperTotalSteps"))
	{
		engineConfiguration->idleStepperTotalSteps = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "noAccelAfterHardLimitPeriodSecs"))
	{
		engineConfiguration->noAccelAfterHardLimitPeriodSecs = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mapAveragingSchedulingAtIndex"))
	{
		engineConfiguration->mapAveragingSchedulingAtIndex = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tachPulseDuractionMs"))
	{
		engineConfiguration->tachPulseDuractionMs = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "maxAcRpm"))
	{
		engineConfiguration->maxAcRpm = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "maxAcTps"))
	{
		engineConfiguration->maxAcTps = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "maxAcClt"))
	{
		engineConfiguration->maxAcClt = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "wwaeTau"))
	{
		engineConfiguration->wwaeTau = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "alternatorControl.pFactor"))
	{
		engineConfiguration->alternatorControl.pFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "alternatorControl.iFactor"))
	{
		engineConfiguration->alternatorControl.iFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "alternatorControl.dFactor"))
	{
		engineConfiguration->alternatorControl.dFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "alternatorControl.offset"))
	{
		engineConfiguration->alternatorControl.offset = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "alternatorControl.periodMs"))
	{
		engineConfiguration->alternatorControl.periodMs = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "alternatorControl.minValue"))
	{
		engineConfiguration->alternatorControl.minValue = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "alternatorControl.maxValue"))
	{
		engineConfiguration->alternatorControl.maxValue = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etb.pFactor"))
	{
		engineConfiguration->etb.pFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etb.iFactor"))
	{
		engineConfiguration->etb.iFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etb.dFactor"))
	{
		engineConfiguration->etb.dFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etb.offset"))
	{
		engineConfiguration->etb.offset = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etb.periodMs"))
	{
		engineConfiguration->etb.periodMs = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etb.minValue"))
	{
		engineConfiguration->etb.minValue = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etb.maxValue"))
	{
		engineConfiguration->etb.maxValue = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tps2Min"))
	{
		engineConfiguration->tps2Min = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tps2Max"))
	{
		engineConfiguration->tps2Max = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tachPulsePerRev"))
	{
		engineConfiguration->tachPulsePerRev = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mapErrorDetectionTooLow"))
	{
		engineConfiguration->mapErrorDetectionTooLow = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mapErrorDetectionTooHigh"))
	{
		engineConfiguration->mapErrorDetectionTooHigh = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "multisparkSparkDuration"))
	{
		engineConfiguration->multisparkSparkDuration = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "multisparkDwell"))
	{
		engineConfiguration->multisparkDwell = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleRpmPid.pFactor"))
	{
		engineConfiguration->idleRpmPid.pFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleRpmPid.iFactor"))
	{
		engineConfiguration->idleRpmPid.iFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleRpmPid.dFactor"))
	{
		engineConfiguration->idleRpmPid.dFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleRpmPid.offset"))
	{
		engineConfiguration->idleRpmPid.offset = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleRpmPid.periodMs"))
	{
		engineConfiguration->idleRpmPid.periodMs = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleRpmPid.minValue"))
	{
		engineConfiguration->idleRpmPid.minValue = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleRpmPid.maxValue"))
	{
		engineConfiguration->idleRpmPid.maxValue = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "wwaeBeta"))
	{
		engineConfiguration->wwaeBeta = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "throttlePedalUpVoltage"))
	{
		engineConfiguration->throttlePedalUpVoltage = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "throttlePedalWOTVoltage"))
	{
		engineConfiguration->throttlePedalWOTVoltage = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "startUpFuelPumpDuration"))
	{
		engineConfiguration->startUpFuelPumpDuration = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idlePidRpmDeadZone"))
	{
		engineConfiguration->idlePidRpmDeadZone = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "targetVBatt"))
	{
		engineConfiguration->targetVBatt = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "alternatorOffAboveTps"))
	{
		engineConfiguration->alternatorOffAboveTps = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "afterCrankingIACtaperDuration"))
	{
		engineConfiguration->afterCrankingIACtaperDuration = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "iacByTpsTaper"))
	{
		engineConfiguration->iacByTpsTaper = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tpsAccelLength"))
	{
		engineConfiguration->tpsAccelLength = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tpsAccelEnrichmentThreshold"))
	{
		engineConfiguration->tpsAccelEnrichmentThreshold = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "engineLoadAccelLength"))
	{
		engineConfiguration->engineLoadAccelLength = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "uartConsoleSerialSpeed"))
	{
		engineConfiguration->uartConsoleSerialSpeed = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tpsDecelEnleanmentThreshold"))
	{
		engineConfiguration->tpsDecelEnleanmentThreshold = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tpsDecelEnleanmentMultiplier"))
	{
		engineConfiguration->tpsDecelEnleanmentMultiplier = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "slowAdcAlpha"))
	{
		engineConfiguration->slowAdcAlpha = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "auxSerialSpeed"))
	{
		engineConfiguration->auxSerialSpeed = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "throttlePedalSecondaryUpVoltage"))
	{
		engineConfiguration->throttlePedalSecondaryUpVoltage = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "throttlePedalSecondaryWOTVoltage"))
	{
		engineConfiguration->throttlePedalSecondaryWOTVoltage = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "verboseCan2BaseAddress"))
	{
		engineConfiguration->verboseCan2BaseAddress = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boostCutPressure"))
	{
		engineConfiguration->boostCutPressure = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "fixedTiming"))
	{
		engineConfiguration->fixedTiming = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mapLowValueVoltage"))
	{
		engineConfiguration->mapLowValueVoltage = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mapHighValueVoltage"))
	{
		engineConfiguration->mapHighValueVoltage = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "egoValueShift"))
	{
		engineConfiguration->egoValueShift = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "crankingIACposition"))
	{
		engineConfiguration->crankingIACposition = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tChargeMinRpmMinTps"))
	{
		engineConfiguration->tChargeMinRpmMinTps = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tChargeMinRpmMaxTps"))
	{
		engineConfiguration->tChargeMinRpmMaxTps = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tChargeMaxRpmMinTps"))
	{
		engineConfiguration->tChargeMaxRpmMinTps = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tChargeMaxRpmMaxTps"))
	{
		engineConfiguration->tChargeMaxRpmMaxTps = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "fan1ExtraIdle"))
	{
		engineConfiguration->fan1ExtraIdle = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "alternatorPwmFrequency"))
	{
		engineConfiguration->alternatorPwmFrequency = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "fan2ExtraIdle"))
	{
		engineConfiguration->fan2ExtraIdle = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "primingDelay"))
	{
		engineConfiguration->primingDelay = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "fuelReferencePressure"))
	{
		engineConfiguration->fuelReferencePressure = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "postCrankingFactor"))
	{
		engineConfiguration->postCrankingFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "postCrankingDurationSec"))
	{
		engineConfiguration->postCrankingDurationSec = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "auxTempSensor1.config.tempC_1"))
	{
		engineConfiguration->auxTempSensor1.config.tempC_1 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "auxTempSensor1.config.tempC_2"))
	{
		engineConfiguration->auxTempSensor1.config.tempC_2 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "auxTempSensor1.config.tempC_3"))
	{
		engineConfiguration->auxTempSensor1.config.tempC_3 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "auxTempSensor1.config.resistance_1"))
	{
		engineConfiguration->auxTempSensor1.config.resistance_1 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "auxTempSensor1.config.resistance_2"))
	{
		engineConfiguration->auxTempSensor1.config.resistance_2 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "auxTempSensor1.config.resistance_3"))
	{
		engineConfiguration->auxTempSensor1.config.resistance_3 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "auxTempSensor1.config.bias_resistor"))
	{
		engineConfiguration->auxTempSensor1.config.bias_resistor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "auxTempSensor2.config.tempC_1"))
	{
		engineConfiguration->auxTempSensor2.config.tempC_1 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "auxTempSensor2.config.tempC_2"))
	{
		engineConfiguration->auxTempSensor2.config.tempC_2 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "auxTempSensor2.config.tempC_3"))
	{
		engineConfiguration->auxTempSensor2.config.tempC_3 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "auxTempSensor2.config.resistance_1"))
	{
		engineConfiguration->auxTempSensor2.config.resistance_1 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "auxTempSensor2.config.resistance_2"))
	{
		engineConfiguration->auxTempSensor2.config.resistance_2 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "auxTempSensor2.config.resistance_3"))
	{
		engineConfiguration->auxTempSensor2.config.resistance_3 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "auxTempSensor2.config.bias_resistor"))
	{
		engineConfiguration->auxTempSensor2.config.bias_resistor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "applyNonlinearBelowPulse"))
	{
		engineConfiguration->applyNonlinearBelowPulse = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "knockSamplingDuration"))
	{
		engineConfiguration->knockSamplingDuration = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etbFreq"))
	{
		engineConfiguration->etbFreq = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etbWastegatePid.pFactor"))
	{
		engineConfiguration->etbWastegatePid.pFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etbWastegatePid.iFactor"))
	{
		engineConfiguration->etbWastegatePid.iFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etbWastegatePid.dFactor"))
	{
		engineConfiguration->etbWastegatePid.dFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etbWastegatePid.offset"))
	{
		engineConfiguration->etbWastegatePid.offset = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etbWastegatePid.periodMs"))
	{
		engineConfiguration->etbWastegatePid.periodMs = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etbWastegatePid.minValue"))
	{
		engineConfiguration->etbWastegatePid.minValue = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etbWastegatePid.maxValue"))
	{
		engineConfiguration->etbWastegatePid.maxValue = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "stepperMinDutyCycle"))
	{
		engineConfiguration->stepperMinDutyCycle = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "stepperMaxDutyCycle"))
	{
		engineConfiguration->stepperMaxDutyCycle = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idlePidActivationTime"))
	{
		engineConfiguration->idlePidActivationTime = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "oilPressure.v1"))
	{
		engineConfiguration->oilPressure.v1 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "oilPressure.value1"))
	{
		engineConfiguration->oilPressure.value1 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "oilPressure.v2"))
	{
		engineConfiguration->oilPressure.v2 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "oilPressure.value2"))
	{
		engineConfiguration->oilPressure.value2 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "fan2OnTemperature"))
	{
		engineConfiguration->fan2OnTemperature = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "fan2OffTemperature"))
	{
		engineConfiguration->fan2OffTemperature = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "coastingFuelCutRpmHigh"))
	{
		engineConfiguration->coastingFuelCutRpmHigh = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "coastingFuelCutRpmLow"))
	{
		engineConfiguration->coastingFuelCutRpmLow = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "coastingFuelCutTps"))
	{
		engineConfiguration->coastingFuelCutTps = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "coastingFuelCutClt"))
	{
		engineConfiguration->coastingFuelCutClt = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "pidExtraForLowRpm"))
	{
		engineConfiguration->pidExtraForLowRpm = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "coastingFuelCutMap"))
	{
		engineConfiguration->coastingFuelCutMap = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "highPressureFuel.v1"))
	{
		engineConfiguration->highPressureFuel.v1 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "highPressureFuel.value1"))
	{
		engineConfiguration->highPressureFuel.value1 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "highPressureFuel.v2"))
	{
		engineConfiguration->highPressureFuel.v2 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "highPressureFuel.value2"))
	{
		engineConfiguration->highPressureFuel.value2 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "lowPressureFuel.v1"))
	{
		engineConfiguration->lowPressureFuel.v1 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "lowPressureFuel.value1"))
	{
		engineConfiguration->lowPressureFuel.value1 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "lowPressureFuel.v2"))
	{
		engineConfiguration->lowPressureFuel.v2 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "lowPressureFuel.value2"))
	{
		engineConfiguration->lowPressureFuel.value2 = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tChargeAirCoefMin"))
	{
		engineConfiguration->tChargeAirCoefMin = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tChargeAirCoefMax"))
	{
		engineConfiguration->tChargeAirCoefMax = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tChargeAirFlowMax"))
	{
		engineConfiguration->tChargeAirFlowMax = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tChargeAirIncrLimit"))
	{
		engineConfiguration->tChargeAirIncrLimit = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tChargeAirDecrLimit"))
	{
		engineConfiguration->tChargeAirDecrLimit = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "hip9011Gain"))
	{
		engineConfiguration->hip9011Gain = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etb_iTermMin"))
	{
		engineConfiguration->etb_iTermMin = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etb_iTermMax"))
	{
		engineConfiguration->etb_iTermMax = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etbMaximumPosition"))
	{
		engineConfiguration->etbMaximumPosition = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleTimingPid.pFactor"))
	{
		engineConfiguration->idleTimingPid.pFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleTimingPid.iFactor"))
	{
		engineConfiguration->idleTimingPid.iFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleTimingPid.dFactor"))
	{
		engineConfiguration->idleTimingPid.dFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleTimingPid.offset"))
	{
		engineConfiguration->idleTimingPid.offset = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleTimingPid.periodMs"))
	{
		engineConfiguration->idleTimingPid.periodMs = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleTimingPid.minValue"))
	{
		engineConfiguration->idleTimingPid.minValue = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleTimingPid.maxValue"))
	{
		engineConfiguration->idleTimingPid.maxValue = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleTimingPidDeadZone"))
	{
		engineConfiguration->idleTimingPidDeadZone = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tpsAccelFractionPeriod"))
	{
		engineConfiguration->tpsAccelFractionPeriod = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tpsAccelFractionDivisor"))
	{
		engineConfiguration->tpsAccelFractionDivisor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idlerpmpid_iTermMin"))
	{
		engineConfiguration->idlerpmpid_iTermMin = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "stoichRatioPrimary"))
	{
		engineConfiguration->stoichRatioPrimary = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idlerpmpid_iTermMax"))
	{
		engineConfiguration->idlerpmpid_iTermMax = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "stoichRatioSecondary"))
	{
		engineConfiguration->stoichRatioSecondary = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etbIdleThrottleRange"))
	{
		engineConfiguration->etbIdleThrottleRange = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "triggerCompCenterVolt"))
	{
		engineConfiguration->triggerCompCenterVolt = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "triggerCompHystMin"))
	{
		engineConfiguration->triggerCompHystMin = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "triggerCompHystMax"))
	{
		engineConfiguration->triggerCompHystMax = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "triggerCompSensorSatRpm"))
	{
		engineConfiguration->triggerCompSensorSatRpm = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleRpmPid2.pFactor"))
	{
		engineConfiguration->idleRpmPid2.pFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleRpmPid2.iFactor"))
	{
		engineConfiguration->idleRpmPid2.iFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleRpmPid2.dFactor"))
	{
		engineConfiguration->idleRpmPid2.dFactor = value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleRpmPid2.offset"))
	{
		engineConfiguration->idleRpmPid2.offset = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleRpmPid2.periodMs"))
	{
		engineConfiguration->idleRpmPid2.periodMs = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleRpmPid2.minValue"))
	{
		engineConfiguration->idleRpmPid2.minValue = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleRpmPid2.maxValue"))
	{
		engineConfiguration->idleRpmPid2.maxValue = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mc33_i_boost"))
	{
		engineConfiguration->mc33_i_boost = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mc33_i_peak"))
	{
		engineConfiguration->mc33_i_peak = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mc33_i_hold"))
	{
		engineConfiguration->mc33_i_hold = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mc33_t_max_boost"))
	{
		engineConfiguration->mc33_t_max_boost = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mc33_t_peak_off"))
	{
		engineConfiguration->mc33_t_peak_off = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mc33_t_peak_tot"))
	{
		engineConfiguration->mc33_t_peak_tot = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mc33_t_bypass"))
	{
		engineConfiguration->mc33_t_bypass = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mc33_t_hold_off"))
	{
		engineConfiguration->mc33_t_hold_off = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mc33_t_hold_tot"))
	{
		engineConfiguration->mc33_t_hold_tot = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "hpfpCamLobes"))
	{
		engineConfiguration->hpfpCamLobes = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "hpfpPeakPos"))
	{
		engineConfiguration->hpfpPeakPos = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "hpfpMinAngle"))
	{
		engineConfiguration->hpfpMinAngle = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "hpfpPumpVolume"))
	{
		engineConfiguration->hpfpPumpVolume = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "hpfpActivationAngle"))
	{
		engineConfiguration->hpfpActivationAngle = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "hpfpPidP"))
	{
		engineConfiguration->hpfpPidP = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "hpfpPidI"))
	{
		engineConfiguration->hpfpPidI = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "hpfpTargetDecay"))
	{
		engineConfiguration->hpfpTargetDecay = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "vssFilterReciprocal"))
	{
		config->vssFilterReciprocal = value;
		return;
	}
}
