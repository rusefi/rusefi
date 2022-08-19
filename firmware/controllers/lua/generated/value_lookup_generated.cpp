#include "pch.h"
#include "value_lookup.h"
static plain_get_float_s getF_plain[] = {
	{"injector.flow", &engineConfiguration->injector.flow},
	{"cranking.baseFuel", &engineConfiguration->cranking.baseFuel},
	{"ignitionDwellForCrankingMs", &engineConfiguration->ignitionDwellForCrankingMs},
	{"map.sensor.lowValue", &engineConfiguration->map.sensor.lowValue},
	{"map.sensor.highValue", &engineConfiguration->map.sensor.highValue},
	{"clt.config.tempC_1", &engineConfiguration->clt.config.tempC_1},
	{"clt.config.tempC_2", &engineConfiguration->clt.config.tempC_2},
	{"clt.config.tempC_3", &engineConfiguration->clt.config.tempC_3},
	{"clt.config.resistance_1", &engineConfiguration->clt.config.resistance_1},
	{"clt.config.resistance_2", &engineConfiguration->clt.config.resistance_2},
	{"clt.config.resistance_3", &engineConfiguration->clt.config.resistance_3},
	{"clt.config.bias_resistor", &engineConfiguration->clt.config.bias_resistor},
	{"iat.config.tempC_1", &engineConfiguration->iat.config.tempC_1},
	{"iat.config.tempC_2", &engineConfiguration->iat.config.tempC_2},
	{"iat.config.tempC_3", &engineConfiguration->iat.config.tempC_3},
	{"iat.config.resistance_1", &engineConfiguration->iat.config.resistance_1},
	{"iat.config.resistance_2", &engineConfiguration->iat.config.resistance_2},
	{"iat.config.resistance_3", &engineConfiguration->iat.config.resistance_3},
	{"iat.config.bias_resistor", &engineConfiguration->iat.config.bias_resistor},
	{"knockBandCustom", &engineConfiguration->knockBandCustom},
	{"specs.displacement", &engineConfiguration->specs.displacement},
	{"cylinderBore", &engineConfiguration->cylinderBore},
	{"crankingTimingAngle", &engineConfiguration->crankingTimingAngle},
	{"fixedModeTiming", &engineConfiguration->fixedModeTiming},
	{"globalTriggerAngleOffset", &engineConfiguration->globalTriggerAngleOffset},
	{"analogInputDividerCoefficient", &engineConfiguration->analogInputDividerCoefficient},
	{"vbattDividerCoeff", &engineConfiguration->vbattDividerCoeff},
	{"fanOnTemperature", &engineConfiguration->fanOnTemperature},
	{"fanOffTemperature", &engineConfiguration->fanOffTemperature},
	{"driveWheelRevPerKm", &engineConfiguration->driveWheelRevPerKm},
	{"idle_derivativeFilterLoss", &engineConfiguration->idle_derivativeFilterLoss},
	{"airByRpmTaper", &engineConfiguration->airByRpmTaper},
	{"globalFuelCorrection", &engineConfiguration->globalFuelCorrection},
	{"adcVcc", &engineConfiguration->adcVcc},
	{"mapCamDetectionAnglePosition", &engineConfiguration->mapCamDetectionAnglePosition},
	{"afr.v1", &engineConfiguration->afr.v1},
	{"afr.value1", &engineConfiguration->afr.value1},
	{"afr.v2", &engineConfiguration->afr.v2},
	{"afr.value2", &engineConfiguration->afr.value2},
	{"baroSensor.lowValue", &engineConfiguration->baroSensor.lowValue},
	{"baroSensor.highValue", &engineConfiguration->baroSensor.highValue},
	{"manIdlePosition", &engineConfiguration->manIdlePosition},
	{"idle_antiwindupFreq", &engineConfiguration->idle_antiwindupFreq},
	{"compressionRatio", &engineConfiguration->compressionRatio},
	{"boostPid.pFactor", &engineConfiguration->boostPid.pFactor},
	{"boostPid.iFactor", &engineConfiguration->boostPid.iFactor},
	{"boostPid.dFactor", &engineConfiguration->boostPid.dFactor},
	{"launchActivateDelay", &engineConfiguration->launchActivateDelay},
	{"turboSpeedSensorMultiplier", &engineConfiguration->turboSpeedSensorMultiplier},
	{"knockDetectionWindowStart", &engineConfiguration->knockDetectionWindowStart},
	{"knockDetectionWindowEnd", &engineConfiguration->knockDetectionWindowEnd},
	{"idleStepperReactionTime", &engineConfiguration->idleStepperReactionTime},
	{"noAccelAfterHardLimitPeriodSecs", &engineConfiguration->noAccelAfterHardLimitPeriodSecs},
	{"tachPulseDuractionMs", &engineConfiguration->tachPulseDuractionMs},
	{"wwaeTau", &engineConfiguration->wwaeTau},
	{"alternatorControl.pFactor", &engineConfiguration->alternatorControl.pFactor},
	{"alternatorControl.iFactor", &engineConfiguration->alternatorControl.iFactor},
	{"alternatorControl.dFactor", &engineConfiguration->alternatorControl.dFactor},
	{"etb.pFactor", &engineConfiguration->etb.pFactor},
	{"etb.iFactor", &engineConfiguration->etb.iFactor},
	{"etb.dFactor", &engineConfiguration->etb.dFactor},
	{"mapErrorDetectionTooLow", &engineConfiguration->mapErrorDetectionTooLow},
	{"mapErrorDetectionTooHigh", &engineConfiguration->mapErrorDetectionTooHigh},
	{"idleRpmPid.pFactor", &engineConfiguration->idleRpmPid.pFactor},
	{"idleRpmPid.iFactor", &engineConfiguration->idleRpmPid.iFactor},
	{"idleRpmPid.dFactor", &engineConfiguration->idleRpmPid.dFactor},
	{"wwaeBeta", &engineConfiguration->wwaeBeta},
	{"throttlePedalUpVoltage", &engineConfiguration->throttlePedalUpVoltage},
	{"throttlePedalWOTVoltage", &engineConfiguration->throttlePedalWOTVoltage},
	{"targetVBatt", &engineConfiguration->targetVBatt},
	{"alternatorOffAboveTps", &engineConfiguration->alternatorOffAboveTps},
	{"tpsAccelEnrichmentThreshold", &engineConfiguration->tpsAccelEnrichmentThreshold},
	{"tpsDecelEnleanmentThreshold", &engineConfiguration->tpsDecelEnleanmentThreshold},
	{"tpsDecelEnleanmentMultiplier", &engineConfiguration->tpsDecelEnleanmentMultiplier},
	{"slowAdcAlpha", &engineConfiguration->slowAdcAlpha},
	{"throttlePedalSecondaryUpVoltage", &engineConfiguration->throttlePedalSecondaryUpVoltage},
	{"throttlePedalSecondaryWOTVoltage", &engineConfiguration->throttlePedalSecondaryWOTVoltage},
	{"boostCutPressure", &engineConfiguration->boostCutPressure},
	{"fixedTiming", &engineConfiguration->fixedTiming},
	{"mapLowValueVoltage", &engineConfiguration->mapLowValueVoltage},
	{"mapHighValueVoltage", &engineConfiguration->mapHighValueVoltage},
	{"egoValueShift", &engineConfiguration->egoValueShift},
	{"tChargeMinRpmMinTps", &engineConfiguration->tChargeMinRpmMinTps},
	{"tChargeMinRpmMaxTps", &engineConfiguration->tChargeMinRpmMaxTps},
	{"tChargeMaxRpmMinTps", &engineConfiguration->tChargeMaxRpmMinTps},
	{"tChargeMaxRpmMaxTps", &engineConfiguration->tChargeMaxRpmMaxTps},
	{"fuelReferencePressure", &engineConfiguration->fuelReferencePressure},
	{"postCrankingFactor", &engineConfiguration->postCrankingFactor},
	{"postCrankingDurationSec", &engineConfiguration->postCrankingDurationSec},
	{"auxTempSensor1.config.tempC_1", &engineConfiguration->auxTempSensor1.config.tempC_1},
	{"auxTempSensor1.config.tempC_2", &engineConfiguration->auxTempSensor1.config.tempC_2},
	{"auxTempSensor1.config.tempC_3", &engineConfiguration->auxTempSensor1.config.tempC_3},
	{"auxTempSensor1.config.resistance_1", &engineConfiguration->auxTempSensor1.config.resistance_1},
	{"auxTempSensor1.config.resistance_2", &engineConfiguration->auxTempSensor1.config.resistance_2},
	{"auxTempSensor1.config.resistance_3", &engineConfiguration->auxTempSensor1.config.resistance_3},
	{"auxTempSensor1.config.bias_resistor", &engineConfiguration->auxTempSensor1.config.bias_resistor},
	{"auxTempSensor2.config.tempC_1", &engineConfiguration->auxTempSensor2.config.tempC_1},
	{"auxTempSensor2.config.tempC_2", &engineConfiguration->auxTempSensor2.config.tempC_2},
	{"auxTempSensor2.config.tempC_3", &engineConfiguration->auxTempSensor2.config.tempC_3},
	{"auxTempSensor2.config.resistance_1", &engineConfiguration->auxTempSensor2.config.resistance_1},
	{"auxTempSensor2.config.resistance_2", &engineConfiguration->auxTempSensor2.config.resistance_2},
	{"auxTempSensor2.config.resistance_3", &engineConfiguration->auxTempSensor2.config.resistance_3},
	{"auxTempSensor2.config.bias_resistor", &engineConfiguration->auxTempSensor2.config.bias_resistor},
	{"etbWastegatePid.pFactor", &engineConfiguration->etbWastegatePid.pFactor},
	{"etbWastegatePid.iFactor", &engineConfiguration->etbWastegatePid.iFactor},
	{"etbWastegatePid.dFactor", &engineConfiguration->etbWastegatePid.dFactor},
	{"idlePidActivationTime", &engineConfiguration->idlePidActivationTime},
	{"oilPressure.v1", &engineConfiguration->oilPressure.v1},
	{"oilPressure.value1", &engineConfiguration->oilPressure.value1},
	{"oilPressure.v2", &engineConfiguration->oilPressure.v2},
	{"oilPressure.value2", &engineConfiguration->oilPressure.value2},
	{"highPressureFuel.v1", &engineConfiguration->highPressureFuel.v1},
	{"highPressureFuel.value1", &engineConfiguration->highPressureFuel.value1},
	{"highPressureFuel.v2", &engineConfiguration->highPressureFuel.v2},
	{"highPressureFuel.value2", &engineConfiguration->highPressureFuel.value2},
	{"lowPressureFuel.v1", &engineConfiguration->lowPressureFuel.v1},
	{"lowPressureFuel.value1", &engineConfiguration->lowPressureFuel.value1},
	{"lowPressureFuel.v2", &engineConfiguration->lowPressureFuel.v2},
	{"lowPressureFuel.value2", &engineConfiguration->lowPressureFuel.value2},
	{"tChargeAirCoefMin", &engineConfiguration->tChargeAirCoefMin},
	{"tChargeAirCoefMax", &engineConfiguration->tChargeAirCoefMax},
	{"tChargeAirFlowMax", &engineConfiguration->tChargeAirFlowMax},
	{"tChargeAirIncrLimit", &engineConfiguration->tChargeAirIncrLimit},
	{"tChargeAirDecrLimit", &engineConfiguration->tChargeAirDecrLimit},
	{"hip9011Gain", &engineConfiguration->hip9011Gain},
	{"idleTimingPid.pFactor", &engineConfiguration->idleTimingPid.pFactor},
	{"idleTimingPid.iFactor", &engineConfiguration->idleTimingPid.iFactor},
	{"idleTimingPid.dFactor", &engineConfiguration->idleTimingPid.dFactor},
	{"tpsAccelFractionDivisor", &engineConfiguration->tpsAccelFractionDivisor},
	{"etbIdleThrottleRange", &engineConfiguration->etbIdleThrottleRange},
	{"idleRpmPid2.pFactor", &engineConfiguration->idleRpmPid2.pFactor},
	{"idleRpmPid2.iFactor", &engineConfiguration->idleRpmPid2.iFactor},
	{"idleRpmPid2.dFactor", &engineConfiguration->idleRpmPid2.dFactor},
	{"auxLinear1.v1", &engineConfiguration->auxLinear1.v1},
	{"auxLinear1.value1", &engineConfiguration->auxLinear1.value1},
	{"auxLinear1.v2", &engineConfiguration->auxLinear1.v2},
	{"auxLinear1.value2", &engineConfiguration->auxLinear1.value2},
	{"auxLinear2.v1", &engineConfiguration->auxLinear2.v1},
	{"auxLinear2.value1", &engineConfiguration->auxLinear2.value1},
	{"auxLinear2.v2", &engineConfiguration->auxLinear2.v2},
	{"auxLinear2.value2", &engineConfiguration->auxLinear2.value2},
	{"etbMinimumPosition", &engineConfiguration->etbMinimumPosition},
};

plain_get_float_s * findFloat(const char *name) {
	plain_get_float_s *currentF = &getF_plain[0];
	while (currentF < getF_plain + efi::size(getF_plain)) {
		if (strEqualCaseInsensitive(name, currentF->token)) {
			return currentF;
		}
		currentF++;
	}
	return nullptr;
}
float getConfigValueByName(const char *name) {
	{
		plain_get_float_s * known = findFloat(name);
		if (known != nullptr) {
			return *(float*)hackEngineConfigurationPointer(known->value);
		}
	}
	if (strEqualCaseInsensitive(name, "sensorSnifferRpmThreshold"))
		return engineConfiguration->sensorSnifferRpmThreshold;
	if (strEqualCaseInsensitive(name, "launchRpm"))
		return engineConfiguration->launchRpm;
	if (strEqualCaseInsensitive(name, "rpmHardLimit"))
		return engineConfiguration->rpmHardLimit;
	if (strEqualCaseInsensitive(name, "engineSnifferRpmThreshold"))
		return engineConfiguration->engineSnifferRpmThreshold;
	if (strEqualCaseInsensitive(name, "multisparkMaxRpm"))
		return engineConfiguration->multisparkMaxRpm;
	if (strEqualCaseInsensitive(name, "maxAcRpm"))
		return engineConfiguration->maxAcRpm;
	if (strEqualCaseInsensitive(name, "maxAcTps"))
		return engineConfiguration->maxAcTps;
	if (strEqualCaseInsensitive(name, "maxAcClt"))
		return engineConfiguration->maxAcClt;
	if (strEqualCaseInsensitive(name, "multisparkMaxSparkingAngle"))
		return engineConfiguration->multisparkMaxSparkingAngle;
	if (strEqualCaseInsensitive(name, "multisparkMaxExtraSparkCount"))
		return engineConfiguration->multisparkMaxExtraSparkCount;
	if (strEqualCaseInsensitive(name, "isForcedInduction"))
		return engineConfiguration->isForcedInduction;
	if (strEqualCaseInsensitive(name, "useFordRedundantTps"))
		return engineConfiguration->useFordRedundantTps;
	if (strEqualCaseInsensitive(name, "isVerboseAuxPid1"))
		return engineConfiguration->isVerboseAuxPid1;
	if (strEqualCaseInsensitive(name, "overrideTriggerGaps"))
		return engineConfiguration->overrideTriggerGaps;
	if (strEqualCaseInsensitive(name, "enableFan1WithAc"))
		return engineConfiguration->enableFan1WithAc;
	if (strEqualCaseInsensitive(name, "enableFan2WithAc"))
		return engineConfiguration->enableFan2WithAc;
	if (strEqualCaseInsensitive(name, "disableFan1WhenStopped"))
		return engineConfiguration->disableFan1WhenStopped;
	if (strEqualCaseInsensitive(name, "disableFan2WhenStopped"))
		return engineConfiguration->disableFan2WhenStopped;
	if (strEqualCaseInsensitive(name, "enableTrailingSparks"))
		return engineConfiguration->enableTrailingSparks;
	if (strEqualCaseInsensitive(name, "isCJ125Verbose"))
		return engineConfiguration->isCJ125Verbose;
	if (strEqualCaseInsensitive(name, "cj125isUaDivided"))
		return engineConfiguration->cj125isUaDivided;
	if (strEqualCaseInsensitive(name, "cj125isLsu49"))
		return engineConfiguration->cj125isLsu49;
	if (strEqualCaseInsensitive(name, "etb_use_two_wires"))
		return engineConfiguration->etb_use_two_wires;
	if (strEqualCaseInsensitive(name, "isDoubleSolenoidIdle"))
		return engineConfiguration->isDoubleSolenoidIdle;
	if (strEqualCaseInsensitive(name, "useEeprom"))
		return engineConfiguration->useEeprom;
	if (strEqualCaseInsensitive(name, "cj125isUrDivided"))
		return engineConfiguration->cj125isUrDivided;
	if (strEqualCaseInsensitive(name, "useCicPidForIdle"))
		return engineConfiguration->useCicPidForIdle;
	if (strEqualCaseInsensitive(name, "useTLE8888_cranking_hack"))
		return engineConfiguration->useTLE8888_cranking_hack;
	if (strEqualCaseInsensitive(name, "useInstantRpmForIdle"))
		return engineConfiguration->useInstantRpmForIdle;
	if (strEqualCaseInsensitive(name, "useSeparateIdleTablesForCrankingTaper"))
		return engineConfiguration->useSeparateIdleTablesForCrankingTaper;
	if (strEqualCaseInsensitive(name, "launchControlEnabled"))
		return engineConfiguration->launchControlEnabled;
	if (strEqualCaseInsensitive(name, "rollingLaunchEnabled"))
		return engineConfiguration->rollingLaunchEnabled;
	if (strEqualCaseInsensitive(name, "antiLagEnabled"))
		return engineConfiguration->antiLagEnabled;
	if (strEqualCaseInsensitive(name, "useRunningMathForCranking"))
		return engineConfiguration->useRunningMathForCranking;
	if (strEqualCaseInsensitive(name, "displayLogicLevelsInEngineSniffer"))
		return engineConfiguration->displayLogicLevelsInEngineSniffer;
	if (strEqualCaseInsensitive(name, "useTLE8888_stepper"))
		return engineConfiguration->useTLE8888_stepper;
	if (strEqualCaseInsensitive(name, "enableMapEstimationTableFallback"))
		return engineConfiguration->enableMapEstimationTableFallback;
	if (strEqualCaseInsensitive(name, "usescriptTableForCanSniffingFiltering"))
		return engineConfiguration->usescriptTableForCanSniffingFiltering;
	if (strEqualCaseInsensitive(name, "verboseCan"))
		return engineConfiguration->verboseCan;
	if (strEqualCaseInsensitive(name, "artificialTestMisfire"))
		return engineConfiguration->artificialTestMisfire;
	if (strEqualCaseInsensitive(name, "issue_294_31"))
		return engineConfiguration->issue_294_31;
	if (strEqualCaseInsensitive(name, "tpsMin"))
		return engineConfiguration->tpsMin;
	if (strEqualCaseInsensitive(name, "tpsMax"))
		return engineConfiguration->tpsMax;
	if (strEqualCaseInsensitive(name, "tpsErrorDetectionTooLow"))
		return engineConfiguration->tpsErrorDetectionTooLow;
	if (strEqualCaseInsensitive(name, "tpsErrorDetectionTooHigh"))
		return engineConfiguration->tpsErrorDetectionTooHigh;
	if (strEqualCaseInsensitive(name, "cranking.rpm"))
		return engineConfiguration->cranking.rpm;
	if (strEqualCaseInsensitive(name, "etbRevLimitStart"))
		return engineConfiguration->etbRevLimitStart;
	if (strEqualCaseInsensitive(name, "etbRevLimitRange"))
		return engineConfiguration->etbRevLimitRange;
	if (strEqualCaseInsensitive(name, "launchTimingRetard"))
		return engineConfiguration->launchTimingRetard;
	if (strEqualCaseInsensitive(name, "hip9011PrescalerAndSDO"))
		return engineConfiguration->hip9011PrescalerAndSDO;
	if (strEqualCaseInsensitive(name, "specs.cylindersCount"))
		return engineConfiguration->specs.cylindersCount;
	if (strEqualCaseInsensitive(name, "boostControlMinRpm"))
		return engineConfiguration->boostControlMinRpm;
	if (strEqualCaseInsensitive(name, "boostControlMinTps"))
		return engineConfiguration->boostControlMinTps;
	if (strEqualCaseInsensitive(name, "boostControlMinMap"))
		return engineConfiguration->boostControlMinMap;
	if (strEqualCaseInsensitive(name, "gapTrackingLengthOverride"))
		return engineConfiguration->gapTrackingLengthOverride;
	if (strEqualCaseInsensitive(name, "maxIdleVss"))
		return engineConfiguration->maxIdleVss;
	if (strEqualCaseInsensitive(name, "minOilPressureAfterStart"))
		return engineConfiguration->minOilPressureAfterStart;
	if (strEqualCaseInsensitive(name, "canSleepPeriodMs"))
		return engineConfiguration->canSleepPeriodMs;
	if (strEqualCaseInsensitive(name, "byFirmwareVersion"))
		return engineConfiguration->byFirmwareVersion;
	if (strEqualCaseInsensitive(name, "HD44780width"))
		return engineConfiguration->HD44780width;
	if (strEqualCaseInsensitive(name, "HD44780height"))
		return engineConfiguration->HD44780height;
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
	if (strEqualCaseInsensitive(name, "idle.solenoidFrequency"))
		return engineConfiguration->idle.solenoidFrequency;
	if (strEqualCaseInsensitive(name, "knockRetardAggression"))
		return engineConfiguration->knockRetardAggression;
	if (strEqualCaseInsensitive(name, "knockRetardReapplyRate"))
		return engineConfiguration->knockRetardReapplyRate;
	if (strEqualCaseInsensitive(name, "knockRetardMaximum"))
		return engineConfiguration->knockRetardMaximum;
	if (strEqualCaseInsensitive(name, "vssFilterReciprocal"))
		return engineConfiguration->vssFilterReciprocal;
	if (strEqualCaseInsensitive(name, "vssGearRatio"))
		return engineConfiguration->vssGearRatio;
	if (strEqualCaseInsensitive(name, "vssToothCount"))
		return engineConfiguration->vssToothCount;
	if (strEqualCaseInsensitive(name, "triggerSimulatorFrequency"))
		return engineConfiguration->triggerSimulatorFrequency;
	if (strEqualCaseInsensitive(name, "mc33_t_min_boost"))
		return engineConfiguration->mc33_t_min_boost;
	if (strEqualCaseInsensitive(name, "acIdleExtraOffset"))
		return engineConfiguration->acIdleExtraOffset;
	if (strEqualCaseInsensitive(name, "finalGearRatio"))
		return engineConfiguration->finalGearRatio;
	if (strEqualCaseInsensitive(name, "tcuInputSpeedSensorTeeth"))
		return engineConfiguration->tcuInputSpeedSensorTeeth;
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
	if (strEqualCaseInsensitive(name, "is_enabled_spi_1"))
		return engineConfiguration->is_enabled_spi_1;
	if (strEqualCaseInsensitive(name, "is_enabled_spi_2"))
		return engineConfiguration->is_enabled_spi_2;
	if (strEqualCaseInsensitive(name, "is_enabled_spi_3"))
		return engineConfiguration->is_enabled_spi_3;
	if (strEqualCaseInsensitive(name, "isSdCardEnabled"))
		return engineConfiguration->isSdCardEnabled;
	if (strEqualCaseInsensitive(name, "rusefiVerbose29b"))
		return engineConfiguration->rusefiVerbose29b;
	if (strEqualCaseInsensitive(name, "isEngineControlEnabled"))
		return engineConfiguration->isEngineControlEnabled;
	if (strEqualCaseInsensitive(name, "isHip9011Enabled"))
		return engineConfiguration->isHip9011Enabled;
	if (strEqualCaseInsensitive(name, "isVerboseAlternator"))
		return engineConfiguration->isVerboseAlternator;
	if (strEqualCaseInsensitive(name, "verboseQuad"))
		return engineConfiguration->verboseQuad;
	if (strEqualCaseInsensitive(name, "useStepperIdle"))
		return engineConfiguration->useStepperIdle;
	if (strEqualCaseInsensitive(name, "enabledStep1Limiter"))
		return engineConfiguration->enabledStep1Limiter;
	if (strEqualCaseInsensitive(name, "useTpicAdvancedMode"))
		return engineConfiguration->useTpicAdvancedMode;
	if (strEqualCaseInsensitive(name, "useLcdScreen"))
		return engineConfiguration->useLcdScreen;
	if (strEqualCaseInsensitive(name, "verboseTLE8888"))
		return engineConfiguration->verboseTLE8888;
	if (strEqualCaseInsensitive(name, "enableVerboseCanTx"))
		return engineConfiguration->enableVerboseCanTx;
	if (strEqualCaseInsensitive(name, "onOffAlternatorLogic"))
		return engineConfiguration->onOffAlternatorLogic;
	if (strEqualCaseInsensitive(name, "isCJ125Enabled"))
		return engineConfiguration->isCJ125Enabled;
	if (strEqualCaseInsensitive(name, "vvtCamSensorUseRise"))
		return engineConfiguration->vvtCamSensorUseRise;
	if (strEqualCaseInsensitive(name, "measureMapOnlyInOneCylinder"))
		return engineConfiguration->measureMapOnlyInOneCylinder;
	if (strEqualCaseInsensitive(name, "stepperForceParkingEveryRestart"))
		return engineConfiguration->stepperForceParkingEveryRestart;
	if (strEqualCaseInsensitive(name, "isFasterEngineSpinUpEnabled"))
		return engineConfiguration->isFasterEngineSpinUpEnabled;
	if (strEqualCaseInsensitive(name, "coastingFuelCutEnabled"))
		return engineConfiguration->coastingFuelCutEnabled;
	if (strEqualCaseInsensitive(name, "useIacTableForCoasting"))
		return engineConfiguration->useIacTableForCoasting;
	if (strEqualCaseInsensitive(name, "useNoiselessTriggerDecoder"))
		return engineConfiguration->useNoiselessTriggerDecoder;
	if (strEqualCaseInsensitive(name, "useIdleTimingPidControl"))
		return engineConfiguration->useIdleTimingPidControl;
	if (strEqualCaseInsensitive(name, "disableEtbWhenEngineStopped"))
		return engineConfiguration->disableEtbWhenEngineStopped;
	if (strEqualCaseInsensitive(name, "is_enabled_spi_4"))
		return engineConfiguration->is_enabled_spi_4;
	if (strEqualCaseInsensitive(name, "pauseEtbControl"))
		return engineConfiguration->pauseEtbControl;
	if (strEqualCaseInsensitive(name, "alignEngineSnifferAtTDC"))
		return engineConfiguration->alignEngineSnifferAtTDC;
	if (strEqualCaseInsensitive(name, "useETBforIdleControl"))
		return engineConfiguration->useETBforIdleControl;
	if (strEqualCaseInsensitive(name, "idleIncrementalPidCic"))
		return engineConfiguration->idleIncrementalPidCic;
	if (strEqualCaseInsensitive(name, "enableAemXSeries"))
		return engineConfiguration->enableAemXSeries;
	if (strEqualCaseInsensitive(name, "verboseCanBaseAddress"))
		return engineConfiguration->verboseCanBaseAddress;
	if (strEqualCaseInsensitive(name, "mc33_hvolt"))
		return engineConfiguration->mc33_hvolt;
	if (strEqualCaseInsensitive(name, "minimumBoostClosedLoopMap"))
		return engineConfiguration->minimumBoostClosedLoopMap;
	if (strEqualCaseInsensitive(name, "vehicleWeight"))
		return engineConfiguration->vehicleWeight;
	if (strEqualCaseInsensitive(name, "idlePidRpmUpperLimit"))
		return engineConfiguration->idlePidRpmUpperLimit;
	if (strEqualCaseInsensitive(name, "applyNonlinearBelowPulse"))
		return engineConfiguration->applyNonlinearBelowPulse;
	if (strEqualCaseInsensitive(name, "stoichRatioSecondary"))
		return engineConfiguration->stoichRatioSecondary;
	if (strEqualCaseInsensitive(name, "etbMaximumPosition"))
		return engineConfiguration->etbMaximumPosition;
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
	if (strEqualCaseInsensitive(name, "clutchUpPinInverted"))
		return engineConfiguration->clutchUpPinInverted;
	if (strEqualCaseInsensitive(name, "clutchDownPinInverted"))
		return engineConfiguration->clutchDownPinInverted;
	if (strEqualCaseInsensitive(name, "useHbridgesToDriveIdleStepper"))
		return engineConfiguration->useHbridgesToDriveIdleStepper;
	if (strEqualCaseInsensitive(name, "multisparkEnable"))
		return engineConfiguration->multisparkEnable;
	if (strEqualCaseInsensitive(name, "enableLaunchRetard"))
		return engineConfiguration->enableLaunchRetard;
	if (strEqualCaseInsensitive(name, "unfinishedenableLaunchBoost"))
		return engineConfiguration->unfinishedenableLaunchBoost;
	if (strEqualCaseInsensitive(name, "unfinishedlaunchDisableBySpeed"))
		return engineConfiguration->unfinishedlaunchDisableBySpeed;
	if (strEqualCaseInsensitive(name, "enableCanVss"))
		return engineConfiguration->enableCanVss;
	if (strEqualCaseInsensitive(name, "enableInnovateLC2"))
		return engineConfiguration->enableInnovateLC2;
	if (strEqualCaseInsensitive(name, "showHumanReadableWarning"))
		return engineConfiguration->showHumanReadableWarning;
	if (strEqualCaseInsensitive(name, "stftIgnoreErrorMagnitude"))
		return engineConfiguration->stftIgnoreErrorMagnitude;
	if (strEqualCaseInsensitive(name, "tempBooleanForVerySpecialCases"))
		return engineConfiguration->tempBooleanForVerySpecialCases;
	if (strEqualCaseInsensitive(name, "enableSoftwareKnock"))
		return engineConfiguration->enableSoftwareKnock;
	if (strEqualCaseInsensitive(name, "verboseVVTDecoding"))
		return engineConfiguration->verboseVVTDecoding;
	if (strEqualCaseInsensitive(name, "invertCamVVTSignal"))
		return engineConfiguration->invertCamVVTSignal;
	if (strEqualCaseInsensitive(name, "consumeObdSensors"))
		return engineConfiguration->consumeObdSensors;
	if (strEqualCaseInsensitive(name, "knockBankCyl1"))
		return engineConfiguration->knockBankCyl1;
	if (strEqualCaseInsensitive(name, "knockBankCyl2"))
		return engineConfiguration->knockBankCyl2;
	if (strEqualCaseInsensitive(name, "knockBankCyl3"))
		return engineConfiguration->knockBankCyl3;
	if (strEqualCaseInsensitive(name, "knockBankCyl4"))
		return engineConfiguration->knockBankCyl4;
	if (strEqualCaseInsensitive(name, "knockBankCyl5"))
		return engineConfiguration->knockBankCyl5;
	if (strEqualCaseInsensitive(name, "knockBankCyl6"))
		return engineConfiguration->knockBankCyl6;
	if (strEqualCaseInsensitive(name, "knockBankCyl7"))
		return engineConfiguration->knockBankCyl7;
	if (strEqualCaseInsensitive(name, "knockBankCyl8"))
		return engineConfiguration->knockBankCyl8;
	if (strEqualCaseInsensitive(name, "knockBankCyl9"))
		return engineConfiguration->knockBankCyl9;
	if (strEqualCaseInsensitive(name, "knockBankCyl10"))
		return engineConfiguration->knockBankCyl10;
	if (strEqualCaseInsensitive(name, "knockBankCyl11"))
		return engineConfiguration->knockBankCyl11;
	if (strEqualCaseInsensitive(name, "knockBankCyl12"))
		return engineConfiguration->knockBankCyl12;
	if (strEqualCaseInsensitive(name, "tcuEnabled"))
		return engineConfiguration->tcuEnabled;
	if (strEqualCaseInsensitive(name, "canBroadcastUseChannelTwo"))
		return engineConfiguration->canBroadcastUseChannelTwo;
	if (strEqualCaseInsensitive(name, "useRawOutputToDriveIdleStepper"))
		return engineConfiguration->useRawOutputToDriveIdleStepper;
	if (strEqualCaseInsensitive(name, "verboseCan2"))
		return engineConfiguration->verboseCan2;
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
	if (strEqualCaseInsensitive(name, "launchTpsThreshold"))
		return engineConfiguration->launchTpsThreshold;
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
	if (strEqualCaseInsensitive(name, "tps2SecondaryMin"))
		return engineConfiguration->tps2SecondaryMin;
	if (strEqualCaseInsensitive(name, "tps2SecondaryMax"))
		return engineConfiguration->tps2SecondaryMax;
	if (strEqualCaseInsensitive(name, "disablePrimaryUart"))
		return engineConfiguration->disablePrimaryUart;
	if (strEqualCaseInsensitive(name, "fuelClosedLoopCorrectionEnabled"))
		return engineConfiguration->fuelClosedLoopCorrectionEnabled;
	if (strEqualCaseInsensitive(name, "isVerboseIAC"))
		return engineConfiguration->isVerboseIAC;
	if (strEqualCaseInsensitive(name, "boardUseTachPullUp"))
		return engineConfiguration->boardUseTachPullUp;
	if (strEqualCaseInsensitive(name, "boardUseTempPullUp"))
		return engineConfiguration->boardUseTempPullUp;
	if (strEqualCaseInsensitive(name, "yesUnderstandLocking"))
		return engineConfiguration->yesUnderstandLocking;
	if (strEqualCaseInsensitive(name, "silentTriggerError"))
		return engineConfiguration->silentTriggerError;
	if (strEqualCaseInsensitive(name, "useLinearCltSensor"))
		return engineConfiguration->useLinearCltSensor;
	if (strEqualCaseInsensitive(name, "canReadEnabled"))
		return engineConfiguration->canReadEnabled;
	if (strEqualCaseInsensitive(name, "canWriteEnabled"))
		return engineConfiguration->canWriteEnabled;
	if (strEqualCaseInsensitive(name, "useLinearIatSensor"))
		return engineConfiguration->useLinearIatSensor;
	if (strEqualCaseInsensitive(name, "boardUse2stepPullDown"))
		return engineConfiguration->boardUse2stepPullDown;
	if (strEqualCaseInsensitive(name, "tachPulseDurationAsDutyCycle"))
		return engineConfiguration->tachPulseDurationAsDutyCycle;
	if (strEqualCaseInsensitive(name, "isAlternatorControlEnabled"))
		return engineConfiguration->isAlternatorControlEnabled;
	if (strEqualCaseInsensitive(name, "invertPrimaryTriggerSignal"))
		return engineConfiguration->invertPrimaryTriggerSignal;
	if (strEqualCaseInsensitive(name, "invertSecondaryTriggerSignal"))
		return engineConfiguration->invertSecondaryTriggerSignal;
	if (strEqualCaseInsensitive(name, "cutFuelOnHardLimit"))
		return engineConfiguration->cutFuelOnHardLimit;
	if (strEqualCaseInsensitive(name, "cutSparkOnHardLimit"))
		return engineConfiguration->cutSparkOnHardLimit;
	if (strEqualCaseInsensitive(name, "launchFuelCutEnable"))
		return engineConfiguration->launchFuelCutEnable;
	if (strEqualCaseInsensitive(name, "launchSparkCutEnable"))
		return engineConfiguration->launchSparkCutEnable;
	if (strEqualCaseInsensitive(name, "boardUseCrankPullUp"))
		return engineConfiguration->boardUseCrankPullUp;
	if (strEqualCaseInsensitive(name, "boardUseCamPullDown"))
		return engineConfiguration->boardUseCamPullDown;
	if (strEqualCaseInsensitive(name, "boardUseCamVrPullUp"))
		return engineConfiguration->boardUseCamVrPullUp;
	if (strEqualCaseInsensitive(name, "boardUseD2PullDown"))
		return engineConfiguration->boardUseD2PullDown;
	if (strEqualCaseInsensitive(name, "boardUseD3PullDown"))
		return engineConfiguration->boardUseD3PullDown;
	if (strEqualCaseInsensitive(name, "boardUseD4PullDown"))
		return engineConfiguration->boardUseD4PullDown;
	if (strEqualCaseInsensitive(name, "boardUseD5PullDown"))
		return engineConfiguration->boardUseD5PullDown;
	if (strEqualCaseInsensitive(name, "verboseIsoTp"))
		return engineConfiguration->verboseIsoTp;
	if (strEqualCaseInsensitive(name, "engineSnifferFocusOnInputs"))
		return engineConfiguration->engineSnifferFocusOnInputs;
	if (strEqualCaseInsensitive(name, "launchActivateInverted"))
		return engineConfiguration->launchActivateInverted;
	if (strEqualCaseInsensitive(name, "twoStroke"))
		return engineConfiguration->twoStroke;
	if (strEqualCaseInsensitive(name, "skippedWheelOnCam"))
		return engineConfiguration->skippedWheelOnCam;
	if (strEqualCaseInsensitive(name, "etbNeutralPosition"))
		return engineConfiguration->etbNeutralPosition;
	if (strEqualCaseInsensitive(name, "isInjectionEnabled"))
		return engineConfiguration->isInjectionEnabled;
	if (strEqualCaseInsensitive(name, "isIgnitionEnabled"))
		return engineConfiguration->isIgnitionEnabled;
	if (strEqualCaseInsensitive(name, "isCylinderCleanupEnabled"))
		return engineConfiguration->isCylinderCleanupEnabled;
	if (strEqualCaseInsensitive(name, "complexWallModel"))
		return engineConfiguration->complexWallModel;
	if (strEqualCaseInsensitive(name, "alwaysInstantRpm"))
		return engineConfiguration->alwaysInstantRpm;
	if (strEqualCaseInsensitive(name, "isMapAveragingEnabled"))
		return engineConfiguration->isMapAveragingEnabled;
	if (strEqualCaseInsensitive(name, "overrideCrankingIacSetting"))
		return engineConfiguration->overrideCrankingIacSetting;
	if (strEqualCaseInsensitive(name, "useSeparateAdvanceForIdle"))
		return engineConfiguration->useSeparateAdvanceForIdle;
	if (strEqualCaseInsensitive(name, "isWaveAnalyzerEnabled"))
		return engineConfiguration->isWaveAnalyzerEnabled;
	if (strEqualCaseInsensitive(name, "useSeparateVeForIdle"))
		return engineConfiguration->useSeparateVeForIdle;
	if (strEqualCaseInsensitive(name, "verboseTriggerSynchDetails"))
		return engineConfiguration->verboseTriggerSynchDetails;
	if (strEqualCaseInsensitive(name, "isManualSpinningMode"))
		return engineConfiguration->isManualSpinningMode;
	if (strEqualCaseInsensitive(name, "twoWireBatchInjection"))
		return engineConfiguration->twoWireBatchInjection;
	if (strEqualCaseInsensitive(name, "useOnlyRisingEdgeForTrigger"))
		return engineConfiguration->useOnlyRisingEdgeForTrigger;
	if (strEqualCaseInsensitive(name, "twoWireBatchIgnition"))
		return engineConfiguration->twoWireBatchIgnition;
	if (strEqualCaseInsensitive(name, "useFixedBaroCorrFromMap"))
		return engineConfiguration->useFixedBaroCorrFromMap;
	if (strEqualCaseInsensitive(name, "useSeparateAdvanceForCranking"))
		return engineConfiguration->useSeparateAdvanceForCranking;
	if (strEqualCaseInsensitive(name, "useAdvanceCorrectionsForCranking"))
		return engineConfiguration->useAdvanceCorrectionsForCranking;
	if (strEqualCaseInsensitive(name, "flexCranking"))
		return engineConfiguration->flexCranking;
	if (strEqualCaseInsensitive(name, "useIacPidMultTable"))
		return engineConfiguration->useIacPidMultTable;
	if (strEqualCaseInsensitive(name, "isBoostControlEnabled"))
		return engineConfiguration->isBoostControlEnabled;
	if (strEqualCaseInsensitive(name, "launchSmoothRetard"))
		return engineConfiguration->launchSmoothRetard;
	if (strEqualCaseInsensitive(name, "isPhaseSyncRequiredForIgnition"))
		return engineConfiguration->isPhaseSyncRequiredForIgnition;
	if (strEqualCaseInsensitive(name, "useCltBasedRpmLimit"))
		return engineConfiguration->useCltBasedRpmLimit;
	if (strEqualCaseInsensitive(name, "forceO2Heating"))
		return engineConfiguration->forceO2Heating;
	if (strEqualCaseInsensitive(name, "invertVvtControlIntake"))
		return engineConfiguration->invertVvtControlIntake;
	if (strEqualCaseInsensitive(name, "invertVvtControlExhaust"))
		return engineConfiguration->invertVvtControlExhaust;
	if (strEqualCaseInsensitive(name, "engineChartSize"))
		return engineConfiguration->engineChartSize;
	if (strEqualCaseInsensitive(name, "acIdleRpmBump"))
		return engineConfiguration->acIdleRpmBump;
	if (strEqualCaseInsensitive(name, "warningPeriod"))
		return engineConfiguration->warningPeriod;
	if (strEqualCaseInsensitive(name, "idleStepperTotalSteps"))
		return engineConfiguration->idleStepperTotalSteps;
	if (strEqualCaseInsensitive(name, "mapAveragingSchedulingAtIndex"))
		return engineConfiguration->mapAveragingSchedulingAtIndex;
	if (strEqualCaseInsensitive(name, "alternatorControl.offset"))
		return engineConfiguration->alternatorControl.offset;
	if (strEqualCaseInsensitive(name, "alternatorControl.periodMs"))
		return engineConfiguration->alternatorControl.periodMs;
	if (strEqualCaseInsensitive(name, "alternatorControl.minValue"))
		return engineConfiguration->alternatorControl.minValue;
	if (strEqualCaseInsensitive(name, "alternatorControl.maxValue"))
		return engineConfiguration->alternatorControl.maxValue;
	if (strEqualCaseInsensitive(name, "etb.offset"))
		return engineConfiguration->etb.offset;
	if (strEqualCaseInsensitive(name, "etb.periodMs"))
		return engineConfiguration->etb.periodMs;
	if (strEqualCaseInsensitive(name, "etb.minValue"))
		return engineConfiguration->etb.minValue;
	if (strEqualCaseInsensitive(name, "etb.maxValue"))
		return engineConfiguration->etb.maxValue;
	if (strEqualCaseInsensitive(name, "airTaperRpmRange"))
		return engineConfiguration->airTaperRpmRange;
	if (strEqualCaseInsensitive(name, "tps2Min"))
		return engineConfiguration->tps2Min;
	if (strEqualCaseInsensitive(name, "tps2Max"))
		return engineConfiguration->tps2Max;
	if (strEqualCaseInsensitive(name, "tachPulsePerRev"))
		return engineConfiguration->tachPulsePerRev;
	if (strEqualCaseInsensitive(name, "multisparkSparkDuration"))
		return engineConfiguration->multisparkSparkDuration;
	if (strEqualCaseInsensitive(name, "multisparkDwell"))
		return engineConfiguration->multisparkDwell;
	if (strEqualCaseInsensitive(name, "idleRpmPid.offset"))
		return engineConfiguration->idleRpmPid.offset;
	if (strEqualCaseInsensitive(name, "idleRpmPid.periodMs"))
		return engineConfiguration->idleRpmPid.periodMs;
	if (strEqualCaseInsensitive(name, "idleRpmPid.minValue"))
		return engineConfiguration->idleRpmPid.minValue;
	if (strEqualCaseInsensitive(name, "idleRpmPid.maxValue"))
		return engineConfiguration->idleRpmPid.maxValue;
	if (strEqualCaseInsensitive(name, "startUpFuelPumpDuration"))
		return engineConfiguration->startUpFuelPumpDuration;
	if (strEqualCaseInsensitive(name, "idlePidRpmDeadZone"))
		return engineConfiguration->idlePidRpmDeadZone;
	if (strEqualCaseInsensitive(name, "afterCrankingIACtaperDuration"))
		return engineConfiguration->afterCrankingIACtaperDuration;
	if (strEqualCaseInsensitive(name, "iacByTpsTaper"))
		return engineConfiguration->iacByTpsTaper;
	if (strEqualCaseInsensitive(name, "tpsAccelLookback"))
		return engineConfiguration->tpsAccelLookback;
	if (strEqualCaseInsensitive(name, "coastingFuelCutVssLow"))
		return engineConfiguration->coastingFuelCutVssLow;
	if (strEqualCaseInsensitive(name, "coastingFuelCutVssHigh"))
		return engineConfiguration->coastingFuelCutVssHigh;
	if (strEqualCaseInsensitive(name, "noFuelTrimAfterDfcoTime"))
		return engineConfiguration->noFuelTrimAfterDfcoTime;
	if (strEqualCaseInsensitive(name, "totalGearsCount"))
		return engineConfiguration->totalGearsCount;
	if (strEqualCaseInsensitive(name, "uartConsoleSerialSpeed"))
		return engineConfiguration->uartConsoleSerialSpeed;
	if (strEqualCaseInsensitive(name, "auxSerialSpeed"))
		return engineConfiguration->auxSerialSpeed;
	if (strEqualCaseInsensitive(name, "mc33_hpfp_i_peak"))
		return engineConfiguration->mc33_hpfp_i_peak;
	if (strEqualCaseInsensitive(name, "mc33_hpfp_i_hold"))
		return engineConfiguration->mc33_hpfp_i_hold;
	if (strEqualCaseInsensitive(name, "mc33_hpfp_i_hold_off"))
		return engineConfiguration->mc33_hpfp_i_hold_off;
	if (strEqualCaseInsensitive(name, "mc33_hpfp_max_hold"))
		return engineConfiguration->mc33_hpfp_max_hold;
	if (strEqualCaseInsensitive(name, "stepperDcInvertedPins"))
		return engineConfiguration->stepperDcInvertedPins;
	if (strEqualCaseInsensitive(name, "canOpenBLT"))
		return engineConfiguration->canOpenBLT;
	if (strEqualCaseInsensitive(name, "can2OpenBLT"))
		return engineConfiguration->can2OpenBLT;
	if (strEqualCaseInsensitive(name, "benchTestOffTime"))
		return engineConfiguration->benchTestOffTime;
	if (strEqualCaseInsensitive(name, "benchTestCount"))
		return engineConfiguration->benchTestCount;
	if (strEqualCaseInsensitive(name, "benchTestOnTime"))
		return engineConfiguration->benchTestOnTime;
	if (strEqualCaseInsensitive(name, "crankingIACposition"))
		return engineConfiguration->crankingIACposition;
	if (strEqualCaseInsensitive(name, "fan1ExtraIdle"))
		return engineConfiguration->fan1ExtraIdle;
	if (strEqualCaseInsensitive(name, "alternatorPwmFrequency"))
		return engineConfiguration->alternatorPwmFrequency;
	if (strEqualCaseInsensitive(name, "fan2ExtraIdle"))
		return engineConfiguration->fan2ExtraIdle;
	if (strEqualCaseInsensitive(name, "primingDelay"))
		return engineConfiguration->primingDelay;
	if (strEqualCaseInsensitive(name, "knockSamplingDuration"))
		return engineConfiguration->knockSamplingDuration;
	if (strEqualCaseInsensitive(name, "etbFreq"))
		return engineConfiguration->etbFreq;
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
	if (strEqualCaseInsensitive(name, "etb_iTermMin"))
		return engineConfiguration->etb_iTermMin;
	if (strEqualCaseInsensitive(name, "etb_iTermMax"))
		return engineConfiguration->etb_iTermMax;
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
	if (strEqualCaseInsensitive(name, "idlerpmpid_iTermMin"))
		return engineConfiguration->idlerpmpid_iTermMin;
	if (strEqualCaseInsensitive(name, "stoichRatioPrimary"))
		return engineConfiguration->stoichRatioPrimary;
	if (strEqualCaseInsensitive(name, "idlerpmpid_iTermMax"))
		return engineConfiguration->idlerpmpid_iTermMax;
	if (strEqualCaseInsensitive(name, "triggerCompCenterVolt"))
		return engineConfiguration->triggerCompCenterVolt;
	if (strEqualCaseInsensitive(name, "triggerCompHystMin"))
		return engineConfiguration->triggerCompHystMin;
	if (strEqualCaseInsensitive(name, "triggerCompHystMax"))
		return engineConfiguration->triggerCompHystMax;
	if (strEqualCaseInsensitive(name, "triggerCompSensorSatRpm"))
		return engineConfiguration->triggerCompSensorSatRpm;
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
	if (strEqualCaseInsensitive(name, "maxCamPhaseResolveRpm"))
		return engineConfiguration->maxCamPhaseResolveRpm;
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
	if (strEqualCaseInsensitive(name, "issFilterReciprocal"))
		return engineConfiguration->issFilterReciprocal;
	if (strEqualCaseInsensitive(name, "hpfpPidP"))
		return engineConfiguration->hpfpPidP;
	if (strEqualCaseInsensitive(name, "hpfpPidI"))
		return engineConfiguration->hpfpPidI;
	if (strEqualCaseInsensitive(name, "hpfpTargetDecay"))
		return engineConfiguration->hpfpTargetDecay;
	if (strEqualCaseInsensitive(name, "vvtActivationDelayMs"))
		return engineConfiguration->vvtActivationDelayMs;
	if (strEqualCaseInsensitive(name, "tuneHidingKey"))
		return engineConfiguration->tuneHidingKey;
	return EFI_ERROR_CODE;
}
void setConfigValueByName(const char *name, float value) {
	{
		plain_get_float_s * known = findFloat(name);
		if (known != nullptr) {
			*(float*)hackEngineConfigurationPointer(known->value) = value;
		}
	}

	if (strEqualCaseInsensitive(name, "sensorSnifferRpmThreshold"))
	{
		engineConfiguration->sensorSnifferRpmThreshold = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "launchRpm"))
	{
		engineConfiguration->launchRpm = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "rpmHardLimit"))
	{
		engineConfiguration->rpmHardLimit = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "engineSnifferRpmThreshold"))
	{
		engineConfiguration->engineSnifferRpmThreshold = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "multisparkMaxRpm"))
	{
		engineConfiguration->multisparkMaxRpm = (int)value;
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
	if (strEqualCaseInsensitive(name, "isForcedInduction"))
	{
		engineConfiguration->isForcedInduction = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useFordRedundantTps"))
	{
		engineConfiguration->useFordRedundantTps = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "isVerboseAuxPid1"))
	{
		engineConfiguration->isVerboseAuxPid1 = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "overrideTriggerGaps"))
	{
		engineConfiguration->overrideTriggerGaps = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "enableFan1WithAc"))
	{
		engineConfiguration->enableFan1WithAc = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "enableFan2WithAc"))
	{
		engineConfiguration->enableFan2WithAc = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "disableFan1WhenStopped"))
	{
		engineConfiguration->disableFan1WhenStopped = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "disableFan2WhenStopped"))
	{
		engineConfiguration->disableFan2WhenStopped = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "enableTrailingSparks"))
	{
		engineConfiguration->enableTrailingSparks = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "isCJ125Verbose"))
	{
		engineConfiguration->isCJ125Verbose = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "cj125isUaDivided"))
	{
		engineConfiguration->cj125isUaDivided = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "cj125isLsu49"))
	{
		engineConfiguration->cj125isLsu49 = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etb_use_two_wires"))
	{
		engineConfiguration->etb_use_two_wires = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "isDoubleSolenoidIdle"))
	{
		engineConfiguration->isDoubleSolenoidIdle = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useEeprom"))
	{
		engineConfiguration->useEeprom = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "cj125isUrDivided"))
	{
		engineConfiguration->cj125isUrDivided = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useCicPidForIdle"))
	{
		engineConfiguration->useCicPidForIdle = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useTLE8888_cranking_hack"))
	{
		engineConfiguration->useTLE8888_cranking_hack = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useInstantRpmForIdle"))
	{
		engineConfiguration->useInstantRpmForIdle = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useSeparateIdleTablesForCrankingTaper"))
	{
		engineConfiguration->useSeparateIdleTablesForCrankingTaper = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "launchControlEnabled"))
	{
		engineConfiguration->launchControlEnabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "rollingLaunchEnabled"))
	{
		engineConfiguration->rollingLaunchEnabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "antiLagEnabled"))
	{
		engineConfiguration->antiLagEnabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useRunningMathForCranking"))
	{
		engineConfiguration->useRunningMathForCranking = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "displayLogicLevelsInEngineSniffer"))
	{
		engineConfiguration->displayLogicLevelsInEngineSniffer = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useTLE8888_stepper"))
	{
		engineConfiguration->useTLE8888_stepper = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "enableMapEstimationTableFallback"))
	{
		engineConfiguration->enableMapEstimationTableFallback = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "usescriptTableForCanSniffingFiltering"))
	{
		engineConfiguration->usescriptTableForCanSniffingFiltering = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "verboseCan"))
	{
		engineConfiguration->verboseCan = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "artificialTestMisfire"))
	{
		engineConfiguration->artificialTestMisfire = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "issue_294_31"))
	{
		engineConfiguration->issue_294_31 = (int)value;
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
	if (strEqualCaseInsensitive(name, "cranking.rpm"))
	{
		engineConfiguration->cranking.rpm = (int)value;
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
	if (strEqualCaseInsensitive(name, "specs.cylindersCount"))
	{
		engineConfiguration->specs.cylindersCount = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boostControlMinRpm"))
	{
		engineConfiguration->boostControlMinRpm = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boostControlMinTps"))
	{
		engineConfiguration->boostControlMinTps = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boostControlMinMap"))
	{
		engineConfiguration->boostControlMinMap = (int)value;
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
	if (strEqualCaseInsensitive(name, "idle.solenoidFrequency"))
	{
		engineConfiguration->idle.solenoidFrequency = (int)value;
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
	if (strEqualCaseInsensitive(name, "vssFilterReciprocal"))
	{
		engineConfiguration->vssFilterReciprocal = (int)value;
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
	if (strEqualCaseInsensitive(name, "triggerSimulatorFrequency"))
	{
		engineConfiguration->triggerSimulatorFrequency = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mc33_t_min_boost"))
	{
		engineConfiguration->mc33_t_min_boost = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "acIdleExtraOffset"))
	{
		engineConfiguration->acIdleExtraOffset = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "finalGearRatio"))
	{
		engineConfiguration->finalGearRatio = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tcuInputSpeedSensorTeeth"))
	{
		engineConfiguration->tcuInputSpeedSensorTeeth = (int)value;
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
	if (strEqualCaseInsensitive(name, "is_enabled_spi_1"))
	{
		engineConfiguration->is_enabled_spi_1 = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "is_enabled_spi_2"))
	{
		engineConfiguration->is_enabled_spi_2 = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "is_enabled_spi_3"))
	{
		engineConfiguration->is_enabled_spi_3 = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "isSdCardEnabled"))
	{
		engineConfiguration->isSdCardEnabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "rusefiVerbose29b"))
	{
		engineConfiguration->rusefiVerbose29b = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "isEngineControlEnabled"))
	{
		engineConfiguration->isEngineControlEnabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "isHip9011Enabled"))
	{
		engineConfiguration->isHip9011Enabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "isVerboseAlternator"))
	{
		engineConfiguration->isVerboseAlternator = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "verboseQuad"))
	{
		engineConfiguration->verboseQuad = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useStepperIdle"))
	{
		engineConfiguration->useStepperIdle = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "enabledStep1Limiter"))
	{
		engineConfiguration->enabledStep1Limiter = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useTpicAdvancedMode"))
	{
		engineConfiguration->useTpicAdvancedMode = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useLcdScreen"))
	{
		engineConfiguration->useLcdScreen = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "verboseTLE8888"))
	{
		engineConfiguration->verboseTLE8888 = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "enableVerboseCanTx"))
	{
		engineConfiguration->enableVerboseCanTx = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "onOffAlternatorLogic"))
	{
		engineConfiguration->onOffAlternatorLogic = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "isCJ125Enabled"))
	{
		engineConfiguration->isCJ125Enabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "vvtCamSensorUseRise"))
	{
		engineConfiguration->vvtCamSensorUseRise = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "measureMapOnlyInOneCylinder"))
	{
		engineConfiguration->measureMapOnlyInOneCylinder = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "stepperForceParkingEveryRestart"))
	{
		engineConfiguration->stepperForceParkingEveryRestart = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "isFasterEngineSpinUpEnabled"))
	{
		engineConfiguration->isFasterEngineSpinUpEnabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "coastingFuelCutEnabled"))
	{
		engineConfiguration->coastingFuelCutEnabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useIacTableForCoasting"))
	{
		engineConfiguration->useIacTableForCoasting = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useNoiselessTriggerDecoder"))
	{
		engineConfiguration->useNoiselessTriggerDecoder = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useIdleTimingPidControl"))
	{
		engineConfiguration->useIdleTimingPidControl = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "disableEtbWhenEngineStopped"))
	{
		engineConfiguration->disableEtbWhenEngineStopped = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "is_enabled_spi_4"))
	{
		engineConfiguration->is_enabled_spi_4 = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "pauseEtbControl"))
	{
		engineConfiguration->pauseEtbControl = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "alignEngineSnifferAtTDC"))
	{
		engineConfiguration->alignEngineSnifferAtTDC = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useETBforIdleControl"))
	{
		engineConfiguration->useETBforIdleControl = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idleIncrementalPidCic"))
	{
		engineConfiguration->idleIncrementalPidCic = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "enableAemXSeries"))
	{
		engineConfiguration->enableAemXSeries = (int)value;
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
	if (strEqualCaseInsensitive(name, "vehicleWeight"))
	{
		engineConfiguration->vehicleWeight = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "idlePidRpmUpperLimit"))
	{
		engineConfiguration->idlePidRpmUpperLimit = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "applyNonlinearBelowPulse"))
	{
		engineConfiguration->applyNonlinearBelowPulse = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "stoichRatioSecondary"))
	{
		engineConfiguration->stoichRatioSecondary = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etbMaximumPosition"))
	{
		engineConfiguration->etbMaximumPosition = (int)value;
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
	if (strEqualCaseInsensitive(name, "clutchUpPinInverted"))
	{
		engineConfiguration->clutchUpPinInverted = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "clutchDownPinInverted"))
	{
		engineConfiguration->clutchDownPinInverted = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useHbridgesToDriveIdleStepper"))
	{
		engineConfiguration->useHbridgesToDriveIdleStepper = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "multisparkEnable"))
	{
		engineConfiguration->multisparkEnable = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "enableLaunchRetard"))
	{
		engineConfiguration->enableLaunchRetard = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "unfinishedenableLaunchBoost"))
	{
		engineConfiguration->unfinishedenableLaunchBoost = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "unfinishedlaunchDisableBySpeed"))
	{
		engineConfiguration->unfinishedlaunchDisableBySpeed = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "enableCanVss"))
	{
		engineConfiguration->enableCanVss = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "enableInnovateLC2"))
	{
		engineConfiguration->enableInnovateLC2 = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "showHumanReadableWarning"))
	{
		engineConfiguration->showHumanReadableWarning = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "stftIgnoreErrorMagnitude"))
	{
		engineConfiguration->stftIgnoreErrorMagnitude = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tempBooleanForVerySpecialCases"))
	{
		engineConfiguration->tempBooleanForVerySpecialCases = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "enableSoftwareKnock"))
	{
		engineConfiguration->enableSoftwareKnock = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "verboseVVTDecoding"))
	{
		engineConfiguration->verboseVVTDecoding = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "invertCamVVTSignal"))
	{
		engineConfiguration->invertCamVVTSignal = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "consumeObdSensors"))
	{
		engineConfiguration->consumeObdSensors = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "knockBankCyl1"))
	{
		engineConfiguration->knockBankCyl1 = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "knockBankCyl2"))
	{
		engineConfiguration->knockBankCyl2 = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "knockBankCyl3"))
	{
		engineConfiguration->knockBankCyl3 = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "knockBankCyl4"))
	{
		engineConfiguration->knockBankCyl4 = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "knockBankCyl5"))
	{
		engineConfiguration->knockBankCyl5 = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "knockBankCyl6"))
	{
		engineConfiguration->knockBankCyl6 = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "knockBankCyl7"))
	{
		engineConfiguration->knockBankCyl7 = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "knockBankCyl8"))
	{
		engineConfiguration->knockBankCyl8 = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "knockBankCyl9"))
	{
		engineConfiguration->knockBankCyl9 = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "knockBankCyl10"))
	{
		engineConfiguration->knockBankCyl10 = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "knockBankCyl11"))
	{
		engineConfiguration->knockBankCyl11 = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "knockBankCyl12"))
	{
		engineConfiguration->knockBankCyl12 = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tcuEnabled"))
	{
		engineConfiguration->tcuEnabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "canBroadcastUseChannelTwo"))
	{
		engineConfiguration->canBroadcastUseChannelTwo = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useRawOutputToDriveIdleStepper"))
	{
		engineConfiguration->useRawOutputToDriveIdleStepper = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "verboseCan2"))
	{
		engineConfiguration->verboseCan2 = (int)value;
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
	if (strEqualCaseInsensitive(name, "launchTpsThreshold"))
	{
		engineConfiguration->launchTpsThreshold = (int)value;
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
	if (strEqualCaseInsensitive(name, "disablePrimaryUart"))
	{
		engineConfiguration->disablePrimaryUart = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "fuelClosedLoopCorrectionEnabled"))
	{
		engineConfiguration->fuelClosedLoopCorrectionEnabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "isVerboseIAC"))
	{
		engineConfiguration->isVerboseIAC = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boardUseTachPullUp"))
	{
		engineConfiguration->boardUseTachPullUp = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boardUseTempPullUp"))
	{
		engineConfiguration->boardUseTempPullUp = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "yesUnderstandLocking"))
	{
		engineConfiguration->yesUnderstandLocking = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "silentTriggerError"))
	{
		engineConfiguration->silentTriggerError = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useLinearCltSensor"))
	{
		engineConfiguration->useLinearCltSensor = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "canReadEnabled"))
	{
		engineConfiguration->canReadEnabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "canWriteEnabled"))
	{
		engineConfiguration->canWriteEnabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useLinearIatSensor"))
	{
		engineConfiguration->useLinearIatSensor = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boardUse2stepPullDown"))
	{
		engineConfiguration->boardUse2stepPullDown = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tachPulseDurationAsDutyCycle"))
	{
		engineConfiguration->tachPulseDurationAsDutyCycle = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "isAlternatorControlEnabled"))
	{
		engineConfiguration->isAlternatorControlEnabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "invertPrimaryTriggerSignal"))
	{
		engineConfiguration->invertPrimaryTriggerSignal = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "invertSecondaryTriggerSignal"))
	{
		engineConfiguration->invertSecondaryTriggerSignal = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "cutFuelOnHardLimit"))
	{
		engineConfiguration->cutFuelOnHardLimit = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "cutSparkOnHardLimit"))
	{
		engineConfiguration->cutSparkOnHardLimit = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "launchFuelCutEnable"))
	{
		engineConfiguration->launchFuelCutEnable = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "launchSparkCutEnable"))
	{
		engineConfiguration->launchSparkCutEnable = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boardUseCrankPullUp"))
	{
		engineConfiguration->boardUseCrankPullUp = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boardUseCamPullDown"))
	{
		engineConfiguration->boardUseCamPullDown = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boardUseCamVrPullUp"))
	{
		engineConfiguration->boardUseCamVrPullUp = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boardUseD2PullDown"))
	{
		engineConfiguration->boardUseD2PullDown = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boardUseD3PullDown"))
	{
		engineConfiguration->boardUseD3PullDown = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boardUseD4PullDown"))
	{
		engineConfiguration->boardUseD4PullDown = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "boardUseD5PullDown"))
	{
		engineConfiguration->boardUseD5PullDown = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "verboseIsoTp"))
	{
		engineConfiguration->verboseIsoTp = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "engineSnifferFocusOnInputs"))
	{
		engineConfiguration->engineSnifferFocusOnInputs = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "launchActivateInverted"))
	{
		engineConfiguration->launchActivateInverted = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "twoStroke"))
	{
		engineConfiguration->twoStroke = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "skippedWheelOnCam"))
	{
		engineConfiguration->skippedWheelOnCam = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etbNeutralPosition"))
	{
		engineConfiguration->etbNeutralPosition = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "isInjectionEnabled"))
	{
		engineConfiguration->isInjectionEnabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "isIgnitionEnabled"))
	{
		engineConfiguration->isIgnitionEnabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "isCylinderCleanupEnabled"))
	{
		engineConfiguration->isCylinderCleanupEnabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "complexWallModel"))
	{
		engineConfiguration->complexWallModel = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "alwaysInstantRpm"))
	{
		engineConfiguration->alwaysInstantRpm = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "isMapAveragingEnabled"))
	{
		engineConfiguration->isMapAveragingEnabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "overrideCrankingIacSetting"))
	{
		engineConfiguration->overrideCrankingIacSetting = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useSeparateAdvanceForIdle"))
	{
		engineConfiguration->useSeparateAdvanceForIdle = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "isWaveAnalyzerEnabled"))
	{
		engineConfiguration->isWaveAnalyzerEnabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useSeparateVeForIdle"))
	{
		engineConfiguration->useSeparateVeForIdle = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "verboseTriggerSynchDetails"))
	{
		engineConfiguration->verboseTriggerSynchDetails = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "isManualSpinningMode"))
	{
		engineConfiguration->isManualSpinningMode = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "twoWireBatchInjection"))
	{
		engineConfiguration->twoWireBatchInjection = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useOnlyRisingEdgeForTrigger"))
	{
		engineConfiguration->useOnlyRisingEdgeForTrigger = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "twoWireBatchIgnition"))
	{
		engineConfiguration->twoWireBatchIgnition = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useFixedBaroCorrFromMap"))
	{
		engineConfiguration->useFixedBaroCorrFromMap = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useSeparateAdvanceForCranking"))
	{
		engineConfiguration->useSeparateAdvanceForCranking = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useAdvanceCorrectionsForCranking"))
	{
		engineConfiguration->useAdvanceCorrectionsForCranking = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "flexCranking"))
	{
		engineConfiguration->flexCranking = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useIacPidMultTable"))
	{
		engineConfiguration->useIacPidMultTable = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "isBoostControlEnabled"))
	{
		engineConfiguration->isBoostControlEnabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "launchSmoothRetard"))
	{
		engineConfiguration->launchSmoothRetard = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "isPhaseSyncRequiredForIgnition"))
	{
		engineConfiguration->isPhaseSyncRequiredForIgnition = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "useCltBasedRpmLimit"))
	{
		engineConfiguration->useCltBasedRpmLimit = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "forceO2Heating"))
	{
		engineConfiguration->forceO2Heating = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "invertVvtControlIntake"))
	{
		engineConfiguration->invertVvtControlIntake = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "invertVvtControlExhaust"))
	{
		engineConfiguration->invertVvtControlExhaust = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "engineChartSize"))
	{
		engineConfiguration->engineChartSize = (int)value;
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
	if (strEqualCaseInsensitive(name, "idleStepperTotalSteps"))
	{
		engineConfiguration->idleStepperTotalSteps = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mapAveragingSchedulingAtIndex"))
	{
		engineConfiguration->mapAveragingSchedulingAtIndex = (int)value;
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
	if (strEqualCaseInsensitive(name, "airTaperRpmRange"))
	{
		engineConfiguration->airTaperRpmRange = (int)value;
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
	if (strEqualCaseInsensitive(name, "tpsAccelLookback"))
	{
		engineConfiguration->tpsAccelLookback = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "coastingFuelCutVssLow"))
	{
		engineConfiguration->coastingFuelCutVssLow = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "coastingFuelCutVssHigh"))
	{
		engineConfiguration->coastingFuelCutVssHigh = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "noFuelTrimAfterDfcoTime"))
	{
		engineConfiguration->noFuelTrimAfterDfcoTime = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "totalGearsCount"))
	{
		engineConfiguration->totalGearsCount = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "uartConsoleSerialSpeed"))
	{
		engineConfiguration->uartConsoleSerialSpeed = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "auxSerialSpeed"))
	{
		engineConfiguration->auxSerialSpeed = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mc33_hpfp_i_peak"))
	{
		engineConfiguration->mc33_hpfp_i_peak = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mc33_hpfp_i_hold"))
	{
		engineConfiguration->mc33_hpfp_i_hold = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mc33_hpfp_i_hold_off"))
	{
		engineConfiguration->mc33_hpfp_i_hold_off = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "mc33_hpfp_max_hold"))
	{
		engineConfiguration->mc33_hpfp_max_hold = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "stepperDcInvertedPins"))
	{
		engineConfiguration->stepperDcInvertedPins = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "canOpenBLT"))
	{
		engineConfiguration->canOpenBLT = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "can2OpenBLT"))
	{
		engineConfiguration->can2OpenBLT = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "benchTestOffTime"))
	{
		engineConfiguration->benchTestOffTime = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "benchTestCount"))
	{
		engineConfiguration->benchTestCount = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "benchTestOnTime"))
	{
		engineConfiguration->benchTestOnTime = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "crankingIACposition"))
	{
		engineConfiguration->crankingIACposition = (int)value;
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
	if (strEqualCaseInsensitive(name, "maxCamPhaseResolveRpm"))
	{
		engineConfiguration->maxCamPhaseResolveRpm = (int)value;
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
	if (strEqualCaseInsensitive(name, "issFilterReciprocal"))
	{
		engineConfiguration->issFilterReciprocal = (int)value;
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
	if (strEqualCaseInsensitive(name, "vvtActivationDelayMs"))
	{
		engineConfiguration->vvtActivationDelayMs = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tuneHidingKey"))
	{
		engineConfiguration->tuneHidingKey = (int)value;
		return;
	}
}
