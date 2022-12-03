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
	{"turbochargerFilter", &engineConfiguration->turbochargerFilter},
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
	{"auxFrequencyFilter", &engineConfiguration->auxFrequencyFilter},
	{"etbDutyThreshold", &engineConfiguration->etbDutyThreshold},
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
	{"etbDutyShutdownThreshold", &engineConfiguration->etbDutyShutdownThreshold},
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
	int hash = djb2lowerCase(name);
	switch(hash) {
		case -920133784:
			return engineConfiguration->sensorSnifferRpmThreshold;
		case -776106289:
			return engineConfiguration->launchRpm;
		case 1176614802:
			return engineConfiguration->rpmHardLimit;
		case 46918724:
			return engineConfiguration->engineSnifferRpmThreshold;
		case -775882810:
			return engineConfiguration->multisparkMaxRpm;
		case 1569359006:
			return engineConfiguration->maxAcRpm;
		case 1569361190:
			return engineConfiguration->maxAcTps;
		case 1569342546:
			return engineConfiguration->maxAcClt;
		case 402092253:
			return engineConfiguration->multisparkMaxSparkingAngle;
		case 726613349:
			return engineConfiguration->multisparkMaxExtraSparkCount;
		case -617915487:
			return engineConfiguration->isForcedInduction;
		case -1284354759:
			return engineConfiguration->useFordRedundantTps;
		case 563123475:
			return engineConfiguration->isVerboseAuxPid1;
		case 1362186948:
			return engineConfiguration->overrideTriggerGaps;
		case -298185774:
			return engineConfiguration->enableFan1WithAc;
		case 993282195:
			return engineConfiguration->enableFan2WithAc;
		case -1132710384:
			return engineConfiguration->disableFan1WhenStopped;
		case -311454863:
			return engineConfiguration->disableFan2WhenStopped;
		case -1409016614:
			return engineConfiguration->enableTrailingSparks;
		case 119246268:
			return engineConfiguration->isCJ125Verbose;
		case 1627431605:
			return engineConfiguration->cj125isUaDivided;
		case -1780521945:
			return engineConfiguration->cj125isLsu49;
		case -1334689362:
			return engineConfiguration->etb_use_two_wires;
		case 1097520823:
			return engineConfiguration->isDoubleSolenoidIdle;
		case -1572546086:
			return engineConfiguration->useEeprom;
		case 291489190:
			return engineConfiguration->cj125isUrDivided;
		case -264572349:
			return engineConfiguration->useCicPidForIdle;
		case -1279270887:
			return engineConfiguration->useTLE8888_cranking_hack;
		case 1156367906:
			return engineConfiguration->kickStartCranking;
		case -418454384:
			return engineConfiguration->useSeparateIdleTablesForCrankingTaper;
		case -338157684:
			return engineConfiguration->launchControlEnabled;
		case -905762494:
			return engineConfiguration->rollingLaunchEnabled;
		case -933524784:
			return engineConfiguration->antiLagEnabled;
		case -54973711:
			return engineConfiguration->useRunningMathForCranking;
		case 1616873838:
			return engineConfiguration->displayLogicLevelsInEngineSniffer;
		case -647008743:
			return engineConfiguration->useTLE8888_stepper;
		case 1992465535:
			return engineConfiguration->enableMapEstimationTableFallback;
		case 671821024:
			return engineConfiguration->usescriptTableForCanSniffingFiltering;
		case -1073210355:
			return engineConfiguration->verboseCan;
		case -1528619572:
			return engineConfiguration->artificialTestMisfire;
		case -1571463185:
			return engineConfiguration->issue_294_31;
		case 513872736:
			return engineConfiguration->tpsMin;
		case 513872482:
			return engineConfiguration->tpsMax;
		case 625338761:
			return engineConfiguration->tpsErrorDetectionTooLow;
		case -838808073:
			return engineConfiguration->tpsErrorDetectionTooHigh;
		case -1717336209:
			return engineConfiguration->cranking.rpm;
		case 401321850:
			return engineConfiguration->etbRevLimitStart;
		case 399466905:
			return engineConfiguration->etbRevLimitRange;
		case 1226423210:
			return engineConfiguration->launchTimingRetard;
		case -1310663509:
			return engineConfiguration->hip9011PrescalerAndSDO;
		case -601272985:
			return engineConfiguration->specs.cylindersCount;
		case -1976636992:
			return engineConfiguration->boostControlMinRpm;
		case -1976634808:
			return engineConfiguration->boostControlMinTps;
		case -1976642929:
			return engineConfiguration->boostControlMinMap;
		case -2079960526:
			return engineConfiguration->gapTrackingLengthOverride;
		case 1408421157:
			return engineConfiguration->maxIdleVss;
		case -855687130:
			return engineConfiguration->minOilPressureAfterStart;
		case 296563315:
			return engineConfiguration->canSleepPeriodMs;
		case 171869155:
			return engineConfiguration->byFirmwareVersion;
		case -526236296:
			return engineConfiguration->HD44780width;
		case -777537519:
			return engineConfiguration->HD44780height;
		case 274318471:
			return engineConfiguration->trailingSparkAngle;
		case 1212570205:
			return engineConfiguration->trigger.customTotalToothCount;
		case 387956265:
			return engineConfiguration->trigger.customSkippedToothCount;
		case -1730107688:
			return engineConfiguration->failedMapFallback;
		case -1965075262:
			return engineConfiguration->boostControlSafeDutyCycle;
		case -568903712:
			return engineConfiguration->idle.solenoidFrequency;
		case 1989648303:
			return engineConfiguration->knockRetardAggression;
		case -938609466:
			return engineConfiguration->knockRetardReapplyRate;
		case -495855925:
			return engineConfiguration->vssFilterReciprocal;
		case -213773345:
			return engineConfiguration->vssGearRatio;
		case -1958312328:
			return engineConfiguration->vssToothCount;
		case -47286421:
			return engineConfiguration->triggerSimulatorFrequency;
		case 1129109559:
			return engineConfiguration->mc33_t_min_boost;
		case 1933500210:
			return engineConfiguration->acIdleExtraOffset;
		case -869432403:
			return engineConfiguration->finalGearRatio;
		case -2116746650:
			return engineConfiguration->tcuInputSpeedSensorTeeth;
		case 2097342691:
			return engineConfiguration->wastegatePositionMin;
		case 2097342437:
			return engineConfiguration->wastegatePositionMax;
		case -1256791716:
			return engineConfiguration->idlePositionMin;
		case -1256791970:
			return engineConfiguration->idlePositionMax;
		case 1467553180:
			return engineConfiguration->tunerStudioSerialSpeed;
		case -1465119085:
			return engineConfiguration->fordInjectorSmallPulseSlope;
		case -722238362:
			return engineConfiguration->is_enabled_spi_1;
		case -722238361:
			return engineConfiguration->is_enabled_spi_2;
		case -722238360:
			return engineConfiguration->is_enabled_spi_3;
		case -1603423011:
			return engineConfiguration->isSdCardEnabled;
		case 2043354390:
			return engineConfiguration->rusefiVerbose29b;
		case 630961411:
			return engineConfiguration->isEngineControlEnabled;
		case -2119647784:
			return engineConfiguration->isHip9011Enabled;
		case -956913549:
			return engineConfiguration->isVerboseAlternator;
		case -1055678778:
			return engineConfiguration->verboseQuad;
		case -920116109:
			return engineConfiguration->useStepperIdle;
		case -2068213709:
			return engineConfiguration->enabledStep1Limiter;
		case -1037977635:
			return engineConfiguration->useTpicAdvancedMode;
		case -538782875:
			return engineConfiguration->useLcdScreen;
		case -1251676000:
			return engineConfiguration->verboseTLE8888;
		case 1643181024:
			return engineConfiguration->enableVerboseCanTx;
		case -432676169:
			return engineConfiguration->etb1configured;
		case -28818575:
			return engineConfiguration->isCJ125Enabled;
		case 1804769272:
			return engineConfiguration->etb2configured;
		case -1236202550:
			return engineConfiguration->measureMapOnlyInOneCylinder;
		case -2104625325:
			return engineConfiguration->stepperForceParkingEveryRestart;
		case 2048390406:
			return engineConfiguration->isFasterEngineSpinUpEnabled;
		case 1570088896:
			return engineConfiguration->coastingFuelCutEnabled;
		case -2101952506:
			return engineConfiguration->useIacTableForCoasting;
		case 957244017:
			return engineConfiguration->useNoiselessTriggerDecoder;
		case 11736566:
			return engineConfiguration->useIdleTimingPidControl;
		case -2096650725:
			return engineConfiguration->disableEtbWhenEngineStopped;
		case -722238359:
			return engineConfiguration->is_enabled_spi_4;
		case -24313313:
			return engineConfiguration->pauseEtbControl;
		case 1836072195:
			return engineConfiguration->alignEngineSnifferAtTDC;
		case -1854539309:
			return engineConfiguration->useETBforIdleControl;
		case 1064399425:
			return engineConfiguration->idleIncrementalPidCic;
		case 259909218:
			return engineConfiguration->enableAemXSeries;
		case 6238478:
			return engineConfiguration->verboseCanBaseAddress;
		case 1182499943:
			return engineConfiguration->mc33_hvolt;
		case 596585178:
			return engineConfiguration->minimumBoostClosedLoopMap;
		case 1549866701:
			return engineConfiguration->vehicleWeight;
		case 1962548154:
			return engineConfiguration->idlePidRpmUpperLimit;
		case 119460051:
			return engineConfiguration->applyNonlinearBelowPulse;
		case 1945443830:
			return engineConfiguration->stoichRatioSecondary;
		case -786901709:
			return engineConfiguration->etbMaximumPosition;
		case -773841334:
			return engineConfiguration->sdCardLogFrequency;
		case -561715997:
			return engineConfiguration->mapMinBufferLength;
		case 1490213215:
			return engineConfiguration->idlePidDeactivationTpsThreshold;
		case -509113657:
			return engineConfiguration->stepperParkingExtraSteps;
		case -1312968487:
			return engineConfiguration->tps1SecondaryMin;
		case -1312968741:
			return engineConfiguration->tps1SecondaryMax;
		case -47392967:
			return engineConfiguration->antiLagRpmTreshold;
		case 644409862:
			return engineConfiguration->startCrankingDuration;
		case -1171325035:
			return engineConfiguration->clutchUpPinInverted;
		case -675074552:
			return engineConfiguration->clutchDownPinInverted;
		case 13806936:
			return engineConfiguration->useHbridgesToDriveIdleStepper;
		case -1074393096:
			return engineConfiguration->multisparkEnable;
		case -1583589783:
			return engineConfiguration->enableLaunchRetard;
		case -246028069:
			return engineConfiguration->unfinishedenableLaunchBoost;
		case 963548365:
			return engineConfiguration->unfinishedlaunchDisableBySpeed;
		case 245947066:
			return engineConfiguration->enableCanVss;
		case 1382521617:
			return engineConfiguration->enableInnovateLC2;
		case 2046602309:
			return engineConfiguration->showHumanReadableWarning;
		case 1838375282:
			return engineConfiguration->stftIgnoreErrorMagnitude;
		case -376859614:
			return engineConfiguration->vvtBooleanForVerySpecialCases;
		case -1724873939:
			return engineConfiguration->enableSoftwareKnock;
		case 1540420600:
			return engineConfiguration->verboseVVTDecoding;
		case 1081026348:
			return engineConfiguration->invertCamVVTSignal;
		case 950993889:
			return engineConfiguration->consumeObdSensors;
		case 1509355728:
			return engineConfiguration->knockBankCyl1;
		case 1509355729:
			return engineConfiguration->knockBankCyl2;
		case 1509355730:
			return engineConfiguration->knockBankCyl3;
		case 1509355731:
			return engineConfiguration->knockBankCyl4;
		case 1509355732:
			return engineConfiguration->knockBankCyl5;
		case 1509355733:
			return engineConfiguration->knockBankCyl6;
		case 1509355734:
			return engineConfiguration->knockBankCyl7;
		case 1509355735:
			return engineConfiguration->knockBankCyl8;
		case 1509355736:
			return engineConfiguration->knockBankCyl9;
		case -1730868480:
			return engineConfiguration->knockBankCyl10;
		case -1730868479:
			return engineConfiguration->knockBankCyl11;
		case -1730868478:
			return engineConfiguration->knockBankCyl12;
		case 1997663388:
			return engineConfiguration->tcuEnabled;
		case 1181153770:
			return engineConfiguration->canBroadcastUseChannelTwo;
		case 839486731:
			return engineConfiguration->useRawOutputToDriveIdleStepper;
		case -1056203297:
			return engineConfiguration->verboseCan2;
		case 1435801342:
			return engineConfiguration->boostPid.offset;
		case -945021094:
			return engineConfiguration->boostPid.periodMs;
		case 777954680:
			return engineConfiguration->boostPid.minValue;
		case -572500550:
			return engineConfiguration->boostPid.maxValue;
		case -1990709262:
			return engineConfiguration->boostPwmFrequency;
		case -1743747394:
			return engineConfiguration->launchSpeedThreshold;
		case 906908772:
			return engineConfiguration->launchTimingRpmRange;
		case 975838686:
			return engineConfiguration->launchFuelAdded;
		case -232786835:
			return engineConfiguration->launchBoostDuty;
		case -163078708:
			return engineConfiguration->hardCutRpmRange;
		case -1340834204:
			return engineConfiguration->launchTpsThreshold;
		case 1887120139:
			return engineConfiguration->stft.maxIdleRegionRpm;
		case 1673018859:
			return engineConfiguration->stft.maxOverrunLoad;
		case -1942479387:
			return engineConfiguration->stft.minPowerLoad;
		case -1157123369:
			return engineConfiguration->stft.deadband;
		case -81412933:
			return engineConfiguration->stft.minClt;
		case -81415311:
			return engineConfiguration->stft.minAfr;
		case -90543309:
			return engineConfiguration->stft.maxAfr;
		case 1028902582:
			return engineConfiguration->stft.startupDelay;
		case 18659930:
			return engineConfiguration->tps2SecondaryMin;
		case 18659676:
			return engineConfiguration->tps2SecondaryMax;
		case 825184089:
			return engineConfiguration->disablePrimaryUart;
		case 1611799720:
			return engineConfiguration->fuelClosedLoopCorrectionEnabled;
		case -546057276:
			return engineConfiguration->isVerboseIAC;
		case 885690524:
			return engineConfiguration->boardUseTachPullUp;
		case -1457484942:
			return engineConfiguration->boardUseTempPullUp;
		case -1985448683:
			return engineConfiguration->yesUnderstandLocking;
		case 582390322:
			return engineConfiguration->silentTriggerError;
		case -1816997974:
			return engineConfiguration->useLinearCltSensor;
		case -528008450:
			return engineConfiguration->canReadEnabled;
		case 2104472173:
			return engineConfiguration->canWriteEnabled;
		case 667504645:
			return engineConfiguration->useLinearIatSensor;
		case -1470758947:
			return engineConfiguration->boardUse2stepPullDown;
		case 99110078:
			return engineConfiguration->tachPulseDurationAsDutyCycle;
		case -1771449111:
			return engineConfiguration->isAlternatorControlEnabled;
		case -587700301:
			return engineConfiguration->invertPrimaryTriggerSignal;
		case -1944260329:
			return engineConfiguration->invertSecondaryTriggerSignal;
		case -794334344:
			return engineConfiguration->cutFuelOnHardLimit;
		case -1087585075:
			return engineConfiguration->cutSparkOnHardLimit;
		case -980843809:
			return engineConfiguration->launchFuelCutEnable;
		case -1829995308:
			return engineConfiguration->launchSparkCutEnable;
		case 1280316843:
			return engineConfiguration->boardUseCrankPullUp;
		case 1304425152:
			return engineConfiguration->boardUseCamPullDown;
		case -94892331:
			return engineConfiguration->boardUseCamVrPullUp;
		case 1868156805:
			return engineConfiguration->boardUseD2PullDown;
		case -472498042:
			return engineConfiguration->boardUseD3PullDown;
		case 1481814407:
			return engineConfiguration->boardUseD4PullDown;
		case -858840440:
			return engineConfiguration->boardUseD5PullDown;
		case -487204662:
			return engineConfiguration->verboseIsoTp;
		case 465982216:
			return engineConfiguration->engineSnifferFocusOnInputs;
		case -1884116782:
			return engineConfiguration->launchActivateInverted;
		case 38719415:
			return engineConfiguration->twoStroke;
		case 1538579992:
			return engineConfiguration->skippedWheelOnCam;
		case 1664668368:
			return engineConfiguration->etbNeutralPosition;
		case 1917301199:
			return engineConfiguration->isInjectionEnabled;
		case -1354884979:
			return engineConfiguration->isIgnitionEnabled;
		case -1491889938:
			return engineConfiguration->isCylinderCleanupEnabled;
		case -2018683234:
			return engineConfiguration->complexWallModel;
		case -1934896346:
			return engineConfiguration->alwaysInstantRpm;
		case 285726462:
			return engineConfiguration->isMapAveragingEnabled;
		case -1500666851:
			return engineConfiguration->overrideCrankingIacSetting;
		case 216872574:
			return engineConfiguration->useSeparateAdvanceForIdle;
		case -726159643:
			return engineConfiguration->isWaveAnalyzerEnabled;
		case -1997438905:
			return engineConfiguration->useSeparateVeForIdle;
		case -325018342:
			return engineConfiguration->verboseTriggerSynchDetails;
		case -795241238:
			return engineConfiguration->isManualSpinningMode;
		case -1248413509:
			return engineConfiguration->twoWireBatchInjection;
		case 454615641:
			return engineConfiguration->twoWireBatchIgnition;
		case 201676494:
			return engineConfiguration->useFixedBaroCorrFromMap;
		case 685453869:
			return engineConfiguration->useSeparateAdvanceForCranking;
		case 1060606307:
			return engineConfiguration->useAdvanceCorrectionsForCranking;
		case -563973567:
			return engineConfiguration->flexCranking;
		case -364626778:
			return engineConfiguration->useIacPidMultTable;
		case -1832431020:
			return engineConfiguration->isBoostControlEnabled;
		case -954321508:
			return engineConfiguration->launchSmoothRetard;
		case -1543452424:
			return engineConfiguration->isPhaseSyncRequiredForIgnition;
		case 939816194:
			return engineConfiguration->useCltBasedRpmLimit;
		case -389678699:
			return engineConfiguration->forceO2Heating;
		case 755512282:
			return engineConfiguration->invertVvtControlIntake;
		case -1331668704:
			return engineConfiguration->invertVvtControlExhaust;
		case 1486014321:
			return engineConfiguration->useBiQuadOnAuxSpeedSensors;
		case -153724425:
			return engineConfiguration->tempBooleanForVerySpecialLogic;
		case -1492464952:
			return engineConfiguration->engineChartSize;
		case 1683471850:
			return engineConfiguration->acIdleRpmBump;
		case 1322924702:
			return engineConfiguration->warningPeriod;
		case -1077032999:
			return engineConfiguration->idleStepperTotalSteps;
		case 1556302698:
			return engineConfiguration->mapAveragingSchedulingAtIndex;
		case 1983046327:
			return engineConfiguration->alternatorControl.offset;
		case -1995686573:
			return engineConfiguration->alternatorControl.periodMs;
		case -272710799:
			return engineConfiguration->alternatorControl.minValue;
		case -1623166029:
			return engineConfiguration->alternatorControl.maxValue;
		case -423426795:
			return engineConfiguration->etb.offset;
		case 1580101425:
			return engineConfiguration->etb.periodMs;
		case -991890097:
			return engineConfiguration->etb.minValue;
		case 1952621969:
			return engineConfiguration->etb.maxValue;
		case -174866119:
			return engineConfiguration->airTaperRpmRange;
		case -224184878:
			return engineConfiguration->tps2Min;
		case -224185132:
			return engineConfiguration->tps2Max;
		case -2142425790:
			return engineConfiguration->tachPulsePerRev;
		case 48235288:
			return engineConfiguration->multisparkSparkDuration;
		case 617337449:
			return engineConfiguration->multisparkDwell;
		case -103982236:
			return engineConfiguration->idleRpmPid.offset;
		case 1562875200:
			return engineConfiguration->idleRpmPid.periodMs;
		case -1009116322:
			return engineConfiguration->idleRpmPid.minValue;
		case 1935395744:
			return engineConfiguration->idleRpmPid.maxValue;
		case -664540020:
			return engineConfiguration->startUpFuelPumpDuration;
		case -528043591:
			return engineConfiguration->idlePidRpmDeadZone;
		case 671504531:
			return engineConfiguration->afterCrankingIACtaperDuration;
		case -2065814304:
			return engineConfiguration->iacByTpsTaper;
		case -1164331270:
			return engineConfiguration->tpsAccelLookback;
		case 329016035:
			return engineConfiguration->coastingFuelCutVssLow;
		case -2027523439:
			return engineConfiguration->coastingFuelCutVssHigh;
		case 1286770247:
			return engineConfiguration->noFuelTrimAfterDfcoTime;
		case -2114430012:
			return engineConfiguration->totalGearsCount;
		case -999870619:
			return engineConfiguration->uartConsoleSerialSpeed;
		case 781749924:
			return engineConfiguration->auxSerialSpeed;
		case 578933872:
			return engineConfiguration->mc33_hpfp_i_peak;
		case 578657622:
			return engineConfiguration->mc33_hpfp_i_hold;
		case 944657936:
			return engineConfiguration->mc33_hpfp_i_hold_off;
		case 663037299:
			return engineConfiguration->mc33_hpfp_max_hold;
		case 337194154:
			return engineConfiguration->stepperDcInvertedPins;
		case -631762165:
			return engineConfiguration->canOpenBLT;
		case -1514354211:
			return engineConfiguration->can2OpenBLT;
		case 444648859:
			return engineConfiguration->injectorFlowAsMassFlow;
		case -1655178001:
			return engineConfiguration->benchTestOffTime;
		case -204721234:
			return engineConfiguration->benchTestCount;
		case -1992436143:
			return engineConfiguration->benchTestOnTime;
		case -224532684:
			return engineConfiguration->crankingIACposition;
		case 1397079021:
			return engineConfiguration->fan1ExtraIdle;
		case 799798215:
			return engineConfiguration->alternatorPwmFrequency;
		case 1464880398:
			return engineConfiguration->fan2ExtraIdle;
		case 1506446058:
			return engineConfiguration->primingDelay;
		case -430252676:
			return engineConfiguration->knockSamplingDuration;
		case 2016887022:
			return engineConfiguration->etbFreq;
		case 794879319:
			return engineConfiguration->etbWastegatePid.offset;
		case 1170565107:
			return engineConfiguration->etbWastegatePid.periodMs;
		case -1401426415:
			return engineConfiguration->etbWastegatePid.minValue;
		case 1543085651:
			return engineConfiguration->etbWastegatePid.maxValue;
		case -1147263902:
			return engineConfiguration->stepperMinDutyCycle;
		case -1188944476:
			return engineConfiguration->stepperMaxDutyCycle;
		case 871106583:
			return engineConfiguration->fan2OnTemperature;
		case 1077467605:
			return engineConfiguration->fan2OffTemperature;
		case 1470846937:
			return engineConfiguration->vvtControlMinRpm;
		case -904771683:
			return engineConfiguration->launchFuelAdderPercent;
		case -86185846:
			return engineConfiguration->etbExpAverageLength;
		case 1272017572:
			return engineConfiguration->coastingFuelCutRpmHigh;
		case 168701078:
			return engineConfiguration->coastingFuelCutRpmLow;
		case -1494035092:
			return engineConfiguration->coastingFuelCutTps;
		case -1494053736:
			return engineConfiguration->coastingFuelCutClt;
		case 1414346958:
			return engineConfiguration->pidExtraForLowRpm;
		case -1494043213:
			return engineConfiguration->coastingFuelCutMap;
		case -1725192476:
			return engineConfiguration->etb_iTermMin;
		case -1725192730:
			return engineConfiguration->etb_iTermMax;
		case -2033984419:
			return engineConfiguration->idleTimingPid.offset;
		case 29505657:
			return engineConfiguration->idleTimingPid.periodMs;
		case 1752481431:
			return engineConfiguration->idleTimingPid.minValue;
		case 402026201:
			return engineConfiguration->idleTimingPid.maxValue;
		case 481957934:
			return engineConfiguration->etbRocExpAverageLength;
		case 1117566701:
			return engineConfiguration->tpsAccelFractionPeriod;
		case -1742418701:
			return engineConfiguration->idlerpmpid_iTermMin;
		case 1762426578:
			return engineConfiguration->stoichRatioPrimary;
		case -1742418955:
			return engineConfiguration->idlerpmpid_iTermMax;
		case 1455224910:
			return engineConfiguration->triggerCompCenterVolt;
		case -25671212:
			return engineConfiguration->triggerCompHystMin;
		case -25671466:
			return engineConfiguration->triggerCompHystMax;
		case -953183719:
			return engineConfiguration->triggerCompSensorSatRpm;
		case 1844327734:
			return engineConfiguration->idleRpmPid2.offset;
		case 1558588306:
			return engineConfiguration->idleRpmPid2.periodMs;
		case -1013403216:
			return engineConfiguration->idleRpmPid2.minValue;
		case 1931108850:
			return engineConfiguration->idleRpmPid2.maxValue;
		case -371707639:
			return engineConfiguration->mc33_i_boost;
		case 379679459:
			return engineConfiguration->mc33_i_peak;
		case 379403209:
			return engineConfiguration->mc33_i_hold;
		case -486240071:
			return engineConfiguration->mc33_t_max_boost;
		case -893360504:
			return engineConfiguration->mc33_t_peak_off;
		case -893354748:
			return engineConfiguration->mc33_t_peak_tot;
		case 1281862911:
			return engineConfiguration->mc33_t_bypass;
		case -2086522258:
			return engineConfiguration->mc33_t_hold_off;
		case -2086516502:
			return engineConfiguration->mc33_t_hold_tot;
		case -2070332548:
			return engineConfiguration->maxCamPhaseResolveRpm;
		case 1676700208:
			return engineConfiguration->dfcoDelay;
		case 483010008:
			return engineConfiguration->acDelay;
		case -1032938305:
			return engineConfiguration->fordInjectorSmallPulseBreakPoint;
		case -1256141959:
			return engineConfiguration->hpfpCamLobes;
		case 1145081382:
			return engineConfiguration->hpfpPeakPos;
		case 1494394654:
			return engineConfiguration->hpfpMinAngle;
		case 334494733:
			return engineConfiguration->hpfpPumpVolume;
		case -81483444:
			return engineConfiguration->hpfpActivationAngle;
		case -413489794:
			return engineConfiguration->issFilterReciprocal;
		case 436500704:
			return engineConfiguration->hpfpPidP;
		case 436500697:
			return engineConfiguration->hpfpPidI;
		case 1025426144:
			return engineConfiguration->hpfpTargetDecay;
		case 288250022:
			return engineConfiguration->vvtActivationDelayMs;
		case -243078627:
			return engineConfiguration->tuneHidingKey;
	}
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
	if (strEqualCaseInsensitive(name, "kickStartCranking"))
	{
		engineConfiguration->kickStartCranking = (int)value;
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
	if (strEqualCaseInsensitive(name, "fordInjectorSmallPulseSlope"))
	{
		engineConfiguration->fordInjectorSmallPulseSlope = (int)value;
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
	if (strEqualCaseInsensitive(name, "etb1configured"))
	{
		engineConfiguration->etb1configured = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "isCJ125Enabled"))
	{
		engineConfiguration->isCJ125Enabled = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etb2configured"))
	{
		engineConfiguration->etb2configured = (int)value;
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
	if (strEqualCaseInsensitive(name, "sdCardLogFrequency"))
	{
		engineConfiguration->sdCardLogFrequency = (int)value;
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
	if (strEqualCaseInsensitive(name, "vvtBooleanForVerySpecialCases"))
	{
		engineConfiguration->vvtBooleanForVerySpecialCases = (int)value;
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
	if (strEqualCaseInsensitive(name, "useBiQuadOnAuxSpeedSensors"))
	{
		engineConfiguration->useBiQuadOnAuxSpeedSensors = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "tempBooleanForVerySpecialLogic"))
	{
		engineConfiguration->tempBooleanForVerySpecialLogic = (int)value;
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
	if (strEqualCaseInsensitive(name, "injectorFlowAsMassFlow"))
	{
		engineConfiguration->injectorFlowAsMassFlow = (int)value;
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
	if (strEqualCaseInsensitive(name, "vvtControlMinRpm"))
	{
		engineConfiguration->vvtControlMinRpm = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "launchFuelAdderPercent"))
	{
		engineConfiguration->launchFuelAdderPercent = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "etbExpAverageLength"))
	{
		engineConfiguration->etbExpAverageLength = (int)value;
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
	if (strEqualCaseInsensitive(name, "etbRocExpAverageLength"))
	{
		engineConfiguration->etbRocExpAverageLength = (int)value;
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
	if (strEqualCaseInsensitive(name, "dfcoDelay"))
	{
		engineConfiguration->dfcoDelay = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "acDelay"))
	{
		engineConfiguration->acDelay = (int)value;
		return;
	}
	if (strEqualCaseInsensitive(name, "fordInjectorSmallPulseBreakPoint"))
	{
		engineConfiguration->fordInjectorSmallPulseBreakPoint = (int)value;
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
