#include "pch.h"
#include "value_lookup.h"
float getConfigValueByName(const char *name) {
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
		case 1542550121:
			return engineConfiguration->injector.flow;
		case -617915487:
			return engineConfiguration->isForcedInduction;
		case -1284354759:
			return engineConfiguration->useFordRedundantTps;
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
		case -1334689362:
			return engineConfiguration->etb_use_two_wires;
		case 1097520823:
			return engineConfiguration->isDoubleSolenoidIdle;
		case -1572546086:
			return engineConfiguration->useEeprom;
		case -1279270887:
			return engineConfiguration->useTLE8888_cranking_hack;
		case 1156367906:
			return engineConfiguration->kickStartCranking;
		case -418454384:
			return engineConfiguration->useSeparateIdleTablesForCrankingTaper;
		case -338157684:
			return engineConfiguration->launchControlEnabled;
		case 240251894:
			return engineConfiguration->doNotFilterTriggerEdgeNoise;
		case -933524784:
			return engineConfiguration->antiLagEnabled;
		case -54973711:
			return engineConfiguration->useRunningMathForCranking;
		case -647008743:
			return engineConfiguration->useTLE8888_stepper;
		case 671821024:
			return engineConfiguration->usescriptTableForCanSniffingFiltering;
		case -1073210355:
			return engineConfiguration->verboseCan;
		case -1528619572:
			return engineConfiguration->artificialTestMisfire;
		case -1284359115:
			return engineConfiguration->useFordRedundantPps;
		case 1925843479:
			return engineConfiguration->cltSensorPulldown;
		case 1570551154:
			return engineConfiguration->iatSensorPulldown;
		case 445194564:
			return engineConfiguration->allowIdenticalPps;
		case 513872736:
			return engineConfiguration->tpsMin;
		case 513872482:
			return engineConfiguration->tpsMax;
		case 625338761:
			return engineConfiguration->tpsErrorDetectionTooLow;
		case -838808073:
			return engineConfiguration->tpsErrorDetectionTooHigh;
		case -781044761:
			return engineConfiguration->cranking.baseFuel;
		case -1717336209:
			return engineConfiguration->cranking.rpm;
		case -589710926:
			return engineConfiguration->ignitionDwellForCrankingMs;
		case 401321850:
			return engineConfiguration->etbRevLimitStart;
		case 399466905:
			return engineConfiguration->etbRevLimitRange;
		case -1307181880:
			return engineConfiguration->map.sensor.lowValue;
		case -1434278346:
			return engineConfiguration->map.sensor.highValue;
		case -1832527325:
			return engineConfiguration->clt.config.tempC_1;
		case -1832527324:
			return engineConfiguration->clt.config.tempC_2;
		case -1832527323:
			return engineConfiguration->clt.config.tempC_3;
		case -5328869:
			return engineConfiguration->clt.config.resistance_1;
		case -5328868:
			return engineConfiguration->clt.config.resistance_2;
		case -5328867:
			return engineConfiguration->clt.config.resistance_3;
		case -653172717:
			return engineConfiguration->clt.config.bias_resistor;
		case -672272162:
			return engineConfiguration->iat.config.tempC_1;
		case -672272161:
			return engineConfiguration->iat.config.tempC_2;
		case -672272160:
			return engineConfiguration->iat.config.tempC_3;
		case -1014418506:
			return engineConfiguration->iat.config.resistance_1;
		case -1014418505:
			return engineConfiguration->iat.config.resistance_2;
		case -1014418504:
			return engineConfiguration->iat.config.resistance_3;
		case 406607630:
			return engineConfiguration->iat.config.bias_resistor;
		case 1226423210:
			return engineConfiguration->launchTimingRetard;
		case -1738843157:
			return engineConfiguration->knockBandCustom;
		case -1227821282:
			return engineConfiguration->displacement;
		case 2122875976:
			return engineConfiguration->triggerSimulatorRpm;
		case -1578655365:
			return engineConfiguration->cylindersCount;
		case -204721234:
			return engineConfiguration->benchTestCount;
		case -1992436143:
			return engineConfiguration->benchTestOnTime;
		case 1425664391:
			return engineConfiguration->cylinderBore;
		case 1523832034:
			return engineConfiguration->ALSMaxTPS;
		case -1976636992:
			return engineConfiguration->boostControlMinRpm;
		case -1976634808:
			return engineConfiguration->boostControlMinTps;
		case -1976642929:
			return engineConfiguration->boostControlMinMap;
		case 2071041633:
			return engineConfiguration->crankingTimingAngle;
		case -2079960526:
			return engineConfiguration->gapTrackingLengthOverride;
		case 1408421157:
			return engineConfiguration->maxIdleVss;
		case -855687130:
			return engineConfiguration->minOilPressureAfterStart;
		case -1965056926:
			return engineConfiguration->fixedModeTiming;
		case -295413160:
			return engineConfiguration->globalTriggerAngleOffset;
		case 1779689293:
			return engineConfiguration->analogInputDividerCoefficient;
		case -1311217456:
			return engineConfiguration->vbattDividerCoeff;
		case -853358747:
			return engineConfiguration->fanOnTemperature;
		case 4686563:
			return engineConfiguration->fanOffTemperature;
		case 991724096:
			return engineConfiguration->driveWheelRevPerKm;
		case 296563315:
			return engineConfiguration->canSleepPeriodMs;
		case 171869155:
			return engineConfiguration->byFirmwareVersion;
		case -1269130308:
			return engineConfiguration->idle_derivativeFilterLoss;
		case 274318471:
			return engineConfiguration->trailingSparkAngle;
		case 1212570205:
			return engineConfiguration->trigger.customTotalToothCount;
		case 387956265:
			return engineConfiguration->trigger.customSkippedToothCount;
		case 2070727975:
			return engineConfiguration->airByRpmTaper;
		case -1965075262:
			return engineConfiguration->boostControlSafeDutyCycle;
		case -1373423899:
			return engineConfiguration->acrRevolutions;
		case 1371979578:
			return engineConfiguration->globalFuelCorrection;
		case -244496183:
			return engineConfiguration->adcVcc;
		case 1958105647:
			return engineConfiguration->mapCamDetectionAnglePosition;
		case -241663117:
			return engineConfiguration->afr.v1;
		case 1273287930:
			return engineConfiguration->afr.value1;
		case -241663116:
			return engineConfiguration->afr.v2;
		case 1273287931:
			return engineConfiguration->afr.value2;
		case 1474689568:
			return engineConfiguration->baroSensor.lowValue;
		case 173166222:
			return engineConfiguration->baroSensor.highValue;
		case -568903712:
			return engineConfiguration->idle.solenoidFrequency;
		case 1401746228:
			return engineConfiguration->manIdlePosition;
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
		case -524949197:
			return engineConfiguration->idle_antiwindupFreq;
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
		case 1467553180:
			return engineConfiguration->tunerStudioSerialSpeed;
		case 355839382:
			return engineConfiguration->compressionRatio;
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
		case -956913549:
			return engineConfiguration->isVerboseAlternator;
		case -920116109:
			return engineConfiguration->useStepperIdle;
		case -2068213709:
			return engineConfiguration->enabledStep1Limiter;
		case -1251676000:
			return engineConfiguration->verboseTLE8888;
		case 1643181024:
			return engineConfiguration->enableVerboseCanTx;
		case -432676169:
			return engineConfiguration->etb1configured;
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
		case 245947066:
			return engineConfiguration->enableCanVss;
		case 1382521617:
			return engineConfiguration->enableInnovateLC2;
		case 2046602309:
			return engineConfiguration->showHumanReadableWarning;
		case 1838375282:
			return engineConfiguration->stftIgnoreErrorMagnitude;
		case -1724873939:
			return engineConfiguration->enableSoftwareKnock;
		case 1540420600:
			return engineConfiguration->verboseVVTDecoding;
		case 1081026348:
			return engineConfiguration->invertCamVVTSignal;
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
		case 1421783686:
			return engineConfiguration->boostPid.pFactor;
		case 971442495:
			return engineConfiguration->boostPid.iFactor;
		case -1190930054:
			return engineConfiguration->boostPid.dFactor;
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
		case 983814707:
			return engineConfiguration->turbochargerFilter;
		case -1340834204:
			return engineConfiguration->launchTpsThreshold;
		case -1153051360:
			return engineConfiguration->launchActivateDelay;
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
		case 1445062086:
			return engineConfiguration->widebandOnSecondBus;
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
		case 240697653:
			return engineConfiguration->neverInstantRpm;
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
		case -105464622:
			return engineConfiguration->sdTriggerLog;
		case -1027820105:
			return engineConfiguration->ALSActivateInverted;
		case -1492464952:
			return engineConfiguration->engineChartSize;
		case -1056007485:
			return engineConfiguration->turboSpeedSensorMultiplier;
		case 1683471850:
			return engineConfiguration->acIdleRpmBump;
		case 1322924702:
			return engineConfiguration->warningPeriod;
		case -339821344:
			return engineConfiguration->knockDetectionWindowStart;
		case -1562133783:
			return engineConfiguration->knockDetectionWindowEnd;
		case 1145888170:
			return engineConfiguration->idleStepperReactionTime;
		case -1077032999:
			return engineConfiguration->idleStepperTotalSteps;
		case -66405381:
			return engineConfiguration->noAccelAfterHardLimitPeriodSecs;
		case 1556302698:
			return engineConfiguration->mapAveragingSchedulingAtIndex;
		case 58931511:
			return engineConfiguration->tachPulseDuractionMs;
		case -390306941:
			return engineConfiguration->wwaeTau;
		case -1993968289:
			return engineConfiguration->alternatorControl.pFactor;
		case 1850657816:
			return engineConfiguration->alternatorControl.iFactor;
		case -311714733:
			return engineConfiguration->alternatorControl.dFactor;
		case 1983046327:
			return engineConfiguration->alternatorControl.offset;
		case -1995686573:
			return engineConfiguration->alternatorControl.periodMs;
		case -272710799:
			return engineConfiguration->alternatorControl.minValue;
		case -1623166029:
			return engineConfiguration->alternatorControl.maxValue;
		case 196797309:
			return engineConfiguration->etb.pFactor;
		case -253543882:
			return engineConfiguration->etb.iFactor;
		case 1879050865:
			return engineConfiguration->etb.dFactor;
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
		case 9388880:
			return engineConfiguration->mapErrorDetectionTooLow;
		case 309682334:
			return engineConfiguration->mapErrorDetectionTooHigh;
		case 48235288:
			return engineConfiguration->multisparkSparkDuration;
		case 617337449:
			return engineConfiguration->multisparkDwell;
		case -2146434132:
			return engineConfiguration->idleRpmPid.pFactor;
		case 1698191973:
			return engineConfiguration->idleRpmPid.iFactor;
		case -464180576:
			return engineConfiguration->idleRpmPid.dFactor;
		case -103982236:
			return engineConfiguration->idleRpmPid.offset;
		case 1562875200:
			return engineConfiguration->idleRpmPid.periodMs;
		case -1009116322:
			return engineConfiguration->idleRpmPid.minValue;
		case 1935395744:
			return engineConfiguration->idleRpmPid.maxValue;
		case 4130389:
			return engineConfiguration->wwaeBeta;
		case -2130836456:
			return engineConfiguration->throttlePedalUpVoltage;
		case 2099527693:
			return engineConfiguration->throttlePedalWOTVoltage;
		case -664540020:
			return engineConfiguration->startUpFuelPumpDuration;
		case -528043591:
			return engineConfiguration->idlePidRpmDeadZone;
		case 1160967565:
			return engineConfiguration->targetVBatt;
		case -1166743424:
			return engineConfiguration->alternatorOffAboveTps;
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
		case 1313479950:
			return engineConfiguration->tpsAccelEnrichmentThreshold;
		case -2114430012:
			return engineConfiguration->totalGearsCount;
		case 1397079021:
			return engineConfiguration->fan1ExtraIdle;
		case -999870619:
			return engineConfiguration->uartConsoleSerialSpeed;
		case -965453331:
			return engineConfiguration->tpsDecelEnleanmentThreshold;
		case 1164012327:
			return engineConfiguration->tpsDecelEnleanmentMultiplier;
		case 781749924:
			return engineConfiguration->auxSerialSpeed;
		case -1564912640:
			return engineConfiguration->throttlePedalSecondaryUpVoltage;
		case -699822859:
			return engineConfiguration->throttlePedalSecondaryWOTVoltage;
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
		case -1992945039:
			return engineConfiguration->boostCutPressure;
		case -1359134883:
			return engineConfiguration->fixedTiming;
		case -137390524:
			return engineConfiguration->mapLowValueVoltage;
		case -1084262094:
			return engineConfiguration->mapHighValueVoltage;
		case 280323355:
			return engineConfiguration->egoValueShift;
		case -224532684:
			return engineConfiguration->crankingIACposition;
		case -557658095:
			return engineConfiguration->tChargeMinRpmMinTps;
		case -566786093:
			return engineConfiguration->tChargeMinRpmMaxTps;
		case -599338669:
			return engineConfiguration->tChargeMaxRpmMinTps;
		case -608466667:
			return engineConfiguration->tChargeMaxRpmMaxTps;
		case 799798215:
			return engineConfiguration->alternatorPwmFrequency;
		case 1464880398:
			return engineConfiguration->fan2ExtraIdle;
		case 1506446058:
			return engineConfiguration->primingDelay;
		case -395416807:
			return engineConfiguration->fuelReferencePressure;
		case -1297058729:
			return engineConfiguration->postCrankingFactor;
		case 1183263609:
			return engineConfiguration->postCrankingDurationSec;
		case -1104834801:
			return engineConfiguration->auxTempSensor1.config.tempC_1;
		case -1104834800:
			return engineConfiguration->auxTempSensor1.config.tempC_2;
		case -1104834799:
			return engineConfiguration->auxTempSensor1.config.tempC_3;
		case 629168263:
			return engineConfiguration->auxTempSensor1.config.resistance_1;
		case 629168264:
			return engineConfiguration->auxTempSensor1.config.resistance_2;
		case 629168265:
			return engineConfiguration->auxTempSensor1.config.resistance_3;
		case -1189603841:
			return engineConfiguration->auxTempSensor1.config.bias_resistor;
		case -900025104:
			return engineConfiguration->auxTempSensor2.config.tempC_1;
		case -900025103:
			return engineConfiguration->auxTempSensor2.config.tempC_2;
		case -900025102:
			return engineConfiguration->auxTempSensor2.config.tempC_3;
		case 1988973320:
			return engineConfiguration->auxTempSensor2.config.resistance_1;
		case 1988973321:
			return engineConfiguration->auxTempSensor2.config.resistance_2;
		case 1988973322:
			return engineConfiguration->auxTempSensor2.config.resistance_3;
		case 734290080:
			return engineConfiguration->auxTempSensor2.config.bias_resistor;
		case -430252676:
			return engineConfiguration->knockSamplingDuration;
		case 2016887022:
			return engineConfiguration->etbFreq;
		case 1746193407:
			return engineConfiguration->etbWastegatePid.pFactor;
		case 1295852216:
			return engineConfiguration->etbWastegatePid.iFactor;
		case -866520333:
			return engineConfiguration->etbWastegatePid.dFactor;
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
		case -1562952959:
			return engineConfiguration->idlePidActivationTime;
		case -613392361:
			return engineConfiguration->oilPressure.v1;
		case -505257058:
			return engineConfiguration->oilPressure.value1;
		case -613392360:
			return engineConfiguration->oilPressure.v2;
		case -505257057:
			return engineConfiguration->oilPressure.value2;
		case 871106583:
			return engineConfiguration->fan2OnTemperature;
		case 1077467605:
			return engineConfiguration->fan2OffTemperature;
		case 383307083:
			return engineConfiguration->auxFrequencyFilter;
		case 1470846937:
			return engineConfiguration->vvtControlMinRpm;
		case -904771683:
			return engineConfiguration->launchFuelAdderPercent;
		case 1057701663:
			return engineConfiguration->etbJamTimeout;
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
		case 2032707103:
			return engineConfiguration->highPressureFuel.v1;
		case -1898025562:
			return engineConfiguration->highPressureFuel.value1;
		case 2032707104:
			return engineConfiguration->highPressureFuel.v2;
		case -1898025561:
			return engineConfiguration->highPressureFuel.value2;
		case -474791503:
			return engineConfiguration->lowPressureFuel.v1;
		case 764445240:
			return engineConfiguration->lowPressureFuel.value1;
		case -474791502:
			return engineConfiguration->lowPressureFuel.v2;
		case 764445241:
			return engineConfiguration->lowPressureFuel.value2;
		case 2115625312:
			return engineConfiguration->tChargeAirCoefMin;
		case 2115625058:
			return engineConfiguration->tChargeAirCoefMax;
		case 1590125629:
			return engineConfiguration->tChargeAirFlowMax;
		case -1128797750:
			return engineConfiguration->tChargeAirIncrLimit;
		case 670644444:
			return engineConfiguration->tChargeAirDecrLimit;
		case -1725192476:
			return engineConfiguration->etb_iTermMin;
		case -1725192730:
			return engineConfiguration->etb_iTermMax;
		case -1411996731:
			return engineConfiguration->idleTimingPid.pFactor;
		case -1862337922:
			return engineConfiguration->idleTimingPid.iFactor;
		case 270256825:
			return engineConfiguration->idleTimingPid.dFactor;
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
		case 68539114:
			return engineConfiguration->tpsAccelFractionDivisor;
		case -1742418701:
			return engineConfiguration->idlerpmpid_iTermMin;
		case 1762426578:
			return engineConfiguration->stoichRatioPrimary;
		case -1742418955:
			return engineConfiguration->idlerpmpid_iTermMax;
		case -54532767:
			return engineConfiguration->etbIdleThrottleRange;
		case 1455224910:
			return engineConfiguration->triggerCompCenterVolt;
		case -25671212:
			return engineConfiguration->triggerCompHystMin;
		case -25671466:
			return engineConfiguration->triggerCompHystMax;
		case -953183719:
			return engineConfiguration->triggerCompSensorSatRpm;
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
		case -929987018:
			return engineConfiguration->etbJamIntegratorLimit;
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
		case 1574876352:
			return engineConfiguration->acrDisablePhase;
		case -1415669996:
			return engineConfiguration->auxLinear1.v1;
		case -65154789:
			return engineConfiguration->auxLinear1.value1;
		case -1415669995:
			return engineConfiguration->auxLinear1.v2;
		case -65154788:
			return engineConfiguration->auxLinear1.value2;
		case -1415634059:
			return engineConfiguration->auxLinear2.v1;
		case -396384772:
			return engineConfiguration->auxLinear2.value1;
		case -1415634058:
			return engineConfiguration->auxLinear2.v2;
		case -396384771:
			return engineConfiguration->auxLinear2.value2;
		case -1855700175:
			return engineConfiguration->etbMinimumPosition;
		case -243078627:
			return engineConfiguration->tuneHidingKey;
		case 1532957848:
			return engineConfiguration->ALSMinRPM;
		case 1523829850:
			return engineConfiguration->ALSMaxRPM;
		case 1830002033:
			return engineConfiguration->ALSMaxDuration;
		case 1532941388:
			return engineConfiguration->ALSMinCLT;
		case 1523813390:
			return engineConfiguration->ALSMaxCLT;
		case -1118241790:
			return engineConfiguration->alsMinTimeBetween;
		case -731677323:
			return engineConfiguration->alsEtbPosition;
		case -908268760:
			return engineConfiguration->acRelayAlternatorDutyAdder;
		case -748456478:
			return engineConfiguration->instantRpmRange;
		case -2021435668:
			return engineConfiguration->ALSIdleAdd;
		case 1232469673:
			return engineConfiguration->ALSEtbAdd;
		case -1404414917:
			return engineConfiguration->ALSSkipRatio;
		case 612659807:
			return engineConfiguration->ALSMaxDriverThrottleIntent;
		case -1744146782:
			return engineConfiguration->tpsSecondaryMaximum;
		case -727657058:
			return engineConfiguration->ppsSecondaryMaximum;
		case 1756514810:
			return engineConfiguration->rpmHardLimitHyst;
	}
	return EFI_ERROR_CODE;
}
void setConfigValueByName(const char *name, float value) {
	int hash = djb2lowerCase(name);
	switch(hash) {
		case -920133784:
	{
		engineConfiguration->sensorSnifferRpmThreshold = (int)value;
		return;
	}
		case -776106289:
	{
		engineConfiguration->launchRpm = (int)value;
		return;
	}
		case 1176614802:
	{
		engineConfiguration->rpmHardLimit = (int)value;
		return;
	}
		case 46918724:
	{
		engineConfiguration->engineSnifferRpmThreshold = (int)value;
		return;
	}
		case -775882810:
	{
		engineConfiguration->multisparkMaxRpm = (int)value;
		return;
	}
		case 1569359006:
	{
		engineConfiguration->maxAcRpm = (int)value;
		return;
	}
		case 1569361190:
	{
		engineConfiguration->maxAcTps = (int)value;
		return;
	}
		case 1569342546:
	{
		engineConfiguration->maxAcClt = (int)value;
		return;
	}
		case 402092253:
	{
		engineConfiguration->multisparkMaxSparkingAngle = (int)value;
		return;
	}
		case 726613349:
	{
		engineConfiguration->multisparkMaxExtraSparkCount = (int)value;
		return;
	}
		case 1542550121:
	{
		engineConfiguration->injector.flow = value;
		return;
	}
		case -617915487:
	{
		engineConfiguration->isForcedInduction = (int)value;
		return;
	}
		case -1284354759:
	{
		engineConfiguration->useFordRedundantTps = (int)value;
		return;
	}
		case 1362186948:
	{
		engineConfiguration->overrideTriggerGaps = (int)value;
		return;
	}
		case -298185774:
	{
		engineConfiguration->enableFan1WithAc = (int)value;
		return;
	}
		case 993282195:
	{
		engineConfiguration->enableFan2WithAc = (int)value;
		return;
	}
		case -1132710384:
	{
		engineConfiguration->disableFan1WhenStopped = (int)value;
		return;
	}
		case -311454863:
	{
		engineConfiguration->disableFan2WhenStopped = (int)value;
		return;
	}
		case -1409016614:
	{
		engineConfiguration->enableTrailingSparks = (int)value;
		return;
	}
		case -1334689362:
	{
		engineConfiguration->etb_use_two_wires = (int)value;
		return;
	}
		case 1097520823:
	{
		engineConfiguration->isDoubleSolenoidIdle = (int)value;
		return;
	}
		case -1572546086:
	{
		engineConfiguration->useEeprom = (int)value;
		return;
	}
		case -1279270887:
	{
		engineConfiguration->useTLE8888_cranking_hack = (int)value;
		return;
	}
		case 1156367906:
	{
		engineConfiguration->kickStartCranking = (int)value;
		return;
	}
		case -418454384:
	{
		engineConfiguration->useSeparateIdleTablesForCrankingTaper = (int)value;
		return;
	}
		case -338157684:
	{
		engineConfiguration->launchControlEnabled = (int)value;
		return;
	}
		case 240251894:
	{
		engineConfiguration->doNotFilterTriggerEdgeNoise = (int)value;
		return;
	}
		case -933524784:
	{
		engineConfiguration->antiLagEnabled = (int)value;
		return;
	}
		case -54973711:
	{
		engineConfiguration->useRunningMathForCranking = (int)value;
		return;
	}
		case -647008743:
	{
		engineConfiguration->useTLE8888_stepper = (int)value;
		return;
	}
		case 671821024:
	{
		engineConfiguration->usescriptTableForCanSniffingFiltering = (int)value;
		return;
	}
		case -1073210355:
	{
		engineConfiguration->verboseCan = (int)value;
		return;
	}
		case -1528619572:
	{
		engineConfiguration->artificialTestMisfire = (int)value;
		return;
	}
		case -1284359115:
	{
		engineConfiguration->useFordRedundantPps = (int)value;
		return;
	}
		case 1925843479:
	{
		engineConfiguration->cltSensorPulldown = (int)value;
		return;
	}
		case 1570551154:
	{
		engineConfiguration->iatSensorPulldown = (int)value;
		return;
	}
		case 445194564:
	{
		engineConfiguration->allowIdenticalPps = (int)value;
		return;
	}
		case 513872736:
	{
		engineConfiguration->tpsMin = (int)value;
		return;
	}
		case 513872482:
	{
		engineConfiguration->tpsMax = (int)value;
		return;
	}
		case 625338761:
	{
		engineConfiguration->tpsErrorDetectionTooLow = (int)value;
		return;
	}
		case -838808073:
	{
		engineConfiguration->tpsErrorDetectionTooHigh = (int)value;
		return;
	}
		case -781044761:
	{
		engineConfiguration->cranking.baseFuel = value;
		return;
	}
		case -1717336209:
	{
		engineConfiguration->cranking.rpm = (int)value;
		return;
	}
		case -589710926:
	{
		engineConfiguration->ignitionDwellForCrankingMs = value;
		return;
	}
		case 401321850:
	{
		engineConfiguration->etbRevLimitStart = (int)value;
		return;
	}
		case 399466905:
	{
		engineConfiguration->etbRevLimitRange = (int)value;
		return;
	}
		case -1307181880:
	{
		engineConfiguration->map.sensor.lowValue = value;
		return;
	}
		case -1434278346:
	{
		engineConfiguration->map.sensor.highValue = value;
		return;
	}
		case -1832527325:
	{
		engineConfiguration->clt.config.tempC_1 = (int)value;
		return;
	}
		case -1832527324:
	{
		engineConfiguration->clt.config.tempC_2 = (int)value;
		return;
	}
		case -1832527323:
	{
		engineConfiguration->clt.config.tempC_3 = (int)value;
		return;
	}
		case -5328869:
	{
		engineConfiguration->clt.config.resistance_1 = (int)value;
		return;
	}
		case -5328868:
	{
		engineConfiguration->clt.config.resistance_2 = (int)value;
		return;
	}
		case -5328867:
	{
		engineConfiguration->clt.config.resistance_3 = (int)value;
		return;
	}
		case -653172717:
	{
		engineConfiguration->clt.config.bias_resistor = (int)value;
		return;
	}
		case -672272162:
	{
		engineConfiguration->iat.config.tempC_1 = (int)value;
		return;
	}
		case -672272161:
	{
		engineConfiguration->iat.config.tempC_2 = (int)value;
		return;
	}
		case -672272160:
	{
		engineConfiguration->iat.config.tempC_3 = (int)value;
		return;
	}
		case -1014418506:
	{
		engineConfiguration->iat.config.resistance_1 = (int)value;
		return;
	}
		case -1014418505:
	{
		engineConfiguration->iat.config.resistance_2 = (int)value;
		return;
	}
		case -1014418504:
	{
		engineConfiguration->iat.config.resistance_3 = (int)value;
		return;
	}
		case 406607630:
	{
		engineConfiguration->iat.config.bias_resistor = (int)value;
		return;
	}
		case 1226423210:
	{
		engineConfiguration->launchTimingRetard = (int)value;
		return;
	}
		case -1738843157:
	{
		engineConfiguration->knockBandCustom = value;
		return;
	}
		case -1227821282:
	{
		engineConfiguration->displacement = (int)value;
		return;
	}
		case 2122875976:
	{
		engineConfiguration->triggerSimulatorRpm = (int)value;
		return;
	}
		case -1578655365:
	{
		engineConfiguration->cylindersCount = (int)value;
		return;
	}
		case -204721234:
	{
		engineConfiguration->benchTestCount = (int)value;
		return;
	}
		case -1992436143:
	{
		engineConfiguration->benchTestOnTime = (int)value;
		return;
	}
		case 1425664391:
	{
		engineConfiguration->cylinderBore = value;
		return;
	}
		case 1523832034:
	{
		engineConfiguration->ALSMaxTPS = (int)value;
		return;
	}
		case -1976636992:
	{
		engineConfiguration->boostControlMinRpm = (int)value;
		return;
	}
		case -1976634808:
	{
		engineConfiguration->boostControlMinTps = (int)value;
		return;
	}
		case -1976642929:
	{
		engineConfiguration->boostControlMinMap = (int)value;
		return;
	}
		case 2071041633:
	{
		engineConfiguration->crankingTimingAngle = value;
		return;
	}
		case -2079960526:
	{
		engineConfiguration->gapTrackingLengthOverride = (int)value;
		return;
	}
		case 1408421157:
	{
		engineConfiguration->maxIdleVss = (int)value;
		return;
	}
		case -855687130:
	{
		engineConfiguration->minOilPressureAfterStart = (int)value;
		return;
	}
		case -1965056926:
	{
		engineConfiguration->fixedModeTiming = value;
		return;
	}
		case -295413160:
	{
		engineConfiguration->globalTriggerAngleOffset = value;
		return;
	}
		case 1779689293:
	{
		engineConfiguration->analogInputDividerCoefficient = value;
		return;
	}
		case -1311217456:
	{
		engineConfiguration->vbattDividerCoeff = value;
		return;
	}
		case -853358747:
	{
		engineConfiguration->fanOnTemperature = (int)value;
		return;
	}
		case 4686563:
	{
		engineConfiguration->fanOffTemperature = (int)value;
		return;
	}
		case 991724096:
	{
		engineConfiguration->driveWheelRevPerKm = value;
		return;
	}
		case 296563315:
	{
		engineConfiguration->canSleepPeriodMs = (int)value;
		return;
	}
		case 171869155:
	{
		engineConfiguration->byFirmwareVersion = (int)value;
		return;
	}
		case -1269130308:
	{
		engineConfiguration->idle_derivativeFilterLoss = value;
		return;
	}
		case 274318471:
	{
		engineConfiguration->trailingSparkAngle = (int)value;
		return;
	}
		case 1212570205:
	{
		engineConfiguration->trigger.customTotalToothCount = (int)value;
		return;
	}
		case 387956265:
	{
		engineConfiguration->trigger.customSkippedToothCount = (int)value;
		return;
	}
		case 2070727975:
	{
		engineConfiguration->airByRpmTaper = value;
		return;
	}
		case -1965075262:
	{
		engineConfiguration->boostControlSafeDutyCycle = (int)value;
		return;
	}
		case -1373423899:
	{
		engineConfiguration->acrRevolutions = (int)value;
		return;
	}
		case 1371979578:
	{
		engineConfiguration->globalFuelCorrection = value;
		return;
	}
		case -244496183:
	{
		engineConfiguration->adcVcc = value;
		return;
	}
		case 1958105647:
	{
		engineConfiguration->mapCamDetectionAnglePosition = value;
		return;
	}
		case -241663117:
	{
		engineConfiguration->afr.v1 = (int)value;
		return;
	}
		case 1273287930:
	{
		engineConfiguration->afr.value1 = (int)value;
		return;
	}
		case -241663116:
	{
		engineConfiguration->afr.v2 = (int)value;
		return;
	}
		case 1273287931:
	{
		engineConfiguration->afr.value2 = (int)value;
		return;
	}
		case 1474689568:
	{
		engineConfiguration->baroSensor.lowValue = value;
		return;
	}
		case 173166222:
	{
		engineConfiguration->baroSensor.highValue = value;
		return;
	}
		case -568903712:
	{
		engineConfiguration->idle.solenoidFrequency = (int)value;
		return;
	}
		case 1401746228:
	{
		engineConfiguration->manIdlePosition = value;
		return;
	}
		case 1989648303:
	{
		engineConfiguration->knockRetardAggression = (int)value;
		return;
	}
		case -938609466:
	{
		engineConfiguration->knockRetardReapplyRate = (int)value;
		return;
	}
		case -495855925:
	{
		engineConfiguration->vssFilterReciprocal = (int)value;
		return;
	}
		case -213773345:
	{
		engineConfiguration->vssGearRatio = (int)value;
		return;
	}
		case -1958312328:
	{
		engineConfiguration->vssToothCount = (int)value;
		return;
	}
		case -524949197:
	{
		engineConfiguration->idle_antiwindupFreq = value;
		return;
	}
		case 1129109559:
	{
		engineConfiguration->mc33_t_min_boost = (int)value;
		return;
	}
		case 1933500210:
	{
		engineConfiguration->acIdleExtraOffset = (int)value;
		return;
	}
		case -869432403:
	{
		engineConfiguration->finalGearRatio = (int)value;
		return;
	}
		case -2116746650:
	{
		engineConfiguration->tcuInputSpeedSensorTeeth = (int)value;
		return;
	}
		case 2097342691:
	{
		engineConfiguration->wastegatePositionMin = (int)value;
		return;
	}
		case 2097342437:
	{
		engineConfiguration->wastegatePositionMax = (int)value;
		return;
	}
		case 1467553180:
	{
		engineConfiguration->tunerStudioSerialSpeed = (int)value;
		return;
	}
		case 355839382:
	{
		engineConfiguration->compressionRatio = value;
		return;
	}
		case -1465119085:
	{
		engineConfiguration->fordInjectorSmallPulseSlope = (int)value;
		return;
	}
		case -722238362:
	{
		engineConfiguration->is_enabled_spi_1 = (int)value;
		return;
	}
		case -722238361:
	{
		engineConfiguration->is_enabled_spi_2 = (int)value;
		return;
	}
		case -722238360:
	{
		engineConfiguration->is_enabled_spi_3 = (int)value;
		return;
	}
		case -1603423011:
	{
		engineConfiguration->isSdCardEnabled = (int)value;
		return;
	}
		case 2043354390:
	{
		engineConfiguration->rusefiVerbose29b = (int)value;
		return;
	}
		case -956913549:
	{
		engineConfiguration->isVerboseAlternator = (int)value;
		return;
	}
		case -920116109:
	{
		engineConfiguration->useStepperIdle = (int)value;
		return;
	}
		case -2068213709:
	{
		engineConfiguration->enabledStep1Limiter = (int)value;
		return;
	}
		case -1251676000:
	{
		engineConfiguration->verboseTLE8888 = (int)value;
		return;
	}
		case 1643181024:
	{
		engineConfiguration->enableVerboseCanTx = (int)value;
		return;
	}
		case -432676169:
	{
		engineConfiguration->etb1configured = (int)value;
		return;
	}
		case 1804769272:
	{
		engineConfiguration->etb2configured = (int)value;
		return;
	}
		case -1236202550:
	{
		engineConfiguration->measureMapOnlyInOneCylinder = (int)value;
		return;
	}
		case -2104625325:
	{
		engineConfiguration->stepperForceParkingEveryRestart = (int)value;
		return;
	}
		case 2048390406:
	{
		engineConfiguration->isFasterEngineSpinUpEnabled = (int)value;
		return;
	}
		case 1570088896:
	{
		engineConfiguration->coastingFuelCutEnabled = (int)value;
		return;
	}
		case -2101952506:
	{
		engineConfiguration->useIacTableForCoasting = (int)value;
		return;
	}
		case 11736566:
	{
		engineConfiguration->useIdleTimingPidControl = (int)value;
		return;
	}
		case -2096650725:
	{
		engineConfiguration->disableEtbWhenEngineStopped = (int)value;
		return;
	}
		case -722238359:
	{
		engineConfiguration->is_enabled_spi_4 = (int)value;
		return;
	}
		case -24313313:
	{
		engineConfiguration->pauseEtbControl = (int)value;
		return;
	}
		case 1836072195:
	{
		engineConfiguration->alignEngineSnifferAtTDC = (int)value;
		return;
	}
		case 259909218:
	{
		engineConfiguration->enableAemXSeries = (int)value;
		return;
	}
		case 6238478:
	{
		engineConfiguration->verboseCanBaseAddress = (int)value;
		return;
	}
		case 1182499943:
	{
		engineConfiguration->mc33_hvolt = (int)value;
		return;
	}
		case 596585178:
	{
		engineConfiguration->minimumBoostClosedLoopMap = (int)value;
		return;
	}
		case 1549866701:
	{
		engineConfiguration->vehicleWeight = (int)value;
		return;
	}
		case 1962548154:
	{
		engineConfiguration->idlePidRpmUpperLimit = (int)value;
		return;
	}
		case 119460051:
	{
		engineConfiguration->applyNonlinearBelowPulse = (int)value;
		return;
	}
		case 1945443830:
	{
		engineConfiguration->stoichRatioSecondary = (int)value;
		return;
	}
		case -786901709:
	{
		engineConfiguration->etbMaximumPosition = (int)value;
		return;
	}
		case -773841334:
	{
		engineConfiguration->sdCardLogFrequency = (int)value;
		return;
	}
		case -561715997:
	{
		engineConfiguration->mapMinBufferLength = (int)value;
		return;
	}
		case 1490213215:
	{
		engineConfiguration->idlePidDeactivationTpsThreshold = (int)value;
		return;
	}
		case -509113657:
	{
		engineConfiguration->stepperParkingExtraSteps = (int)value;
		return;
	}
		case -1312968487:
	{
		engineConfiguration->tps1SecondaryMin = (int)value;
		return;
	}
		case -1312968741:
	{
		engineConfiguration->tps1SecondaryMax = (int)value;
		return;
	}
		case -47392967:
	{
		engineConfiguration->antiLagRpmTreshold = (int)value;
		return;
	}
		case 644409862:
	{
		engineConfiguration->startCrankingDuration = (int)value;
		return;
	}
		case -1171325035:
	{
		engineConfiguration->clutchUpPinInverted = (int)value;
		return;
	}
		case -675074552:
	{
		engineConfiguration->clutchDownPinInverted = (int)value;
		return;
	}
		case 13806936:
	{
		engineConfiguration->useHbridgesToDriveIdleStepper = (int)value;
		return;
	}
		case -1074393096:
	{
		engineConfiguration->multisparkEnable = (int)value;
		return;
	}
		case -1583589783:
	{
		engineConfiguration->enableLaunchRetard = (int)value;
		return;
	}
		case 245947066:
	{
		engineConfiguration->enableCanVss = (int)value;
		return;
	}
		case 1382521617:
	{
		engineConfiguration->enableInnovateLC2 = (int)value;
		return;
	}
		case 2046602309:
	{
		engineConfiguration->showHumanReadableWarning = (int)value;
		return;
	}
		case 1838375282:
	{
		engineConfiguration->stftIgnoreErrorMagnitude = (int)value;
		return;
	}
		case -1724873939:
	{
		engineConfiguration->enableSoftwareKnock = (int)value;
		return;
	}
		case 1540420600:
	{
		engineConfiguration->verboseVVTDecoding = (int)value;
		return;
	}
		case 1081026348:
	{
		engineConfiguration->invertCamVVTSignal = (int)value;
		return;
	}
		case 1509355728:
	{
		engineConfiguration->knockBankCyl1 = (int)value;
		return;
	}
		case 1509355729:
	{
		engineConfiguration->knockBankCyl2 = (int)value;
		return;
	}
		case 1509355730:
	{
		engineConfiguration->knockBankCyl3 = (int)value;
		return;
	}
		case 1509355731:
	{
		engineConfiguration->knockBankCyl4 = (int)value;
		return;
	}
		case 1509355732:
	{
		engineConfiguration->knockBankCyl5 = (int)value;
		return;
	}
		case 1509355733:
	{
		engineConfiguration->knockBankCyl6 = (int)value;
		return;
	}
		case 1509355734:
	{
		engineConfiguration->knockBankCyl7 = (int)value;
		return;
	}
		case 1509355735:
	{
		engineConfiguration->knockBankCyl8 = (int)value;
		return;
	}
		case 1509355736:
	{
		engineConfiguration->knockBankCyl9 = (int)value;
		return;
	}
		case -1730868480:
	{
		engineConfiguration->knockBankCyl10 = (int)value;
		return;
	}
		case -1730868479:
	{
		engineConfiguration->knockBankCyl11 = (int)value;
		return;
	}
		case -1730868478:
	{
		engineConfiguration->knockBankCyl12 = (int)value;
		return;
	}
		case 1997663388:
	{
		engineConfiguration->tcuEnabled = (int)value;
		return;
	}
		case 1181153770:
	{
		engineConfiguration->canBroadcastUseChannelTwo = (int)value;
		return;
	}
		case 839486731:
	{
		engineConfiguration->useRawOutputToDriveIdleStepper = (int)value;
		return;
	}
		case -1056203297:
	{
		engineConfiguration->verboseCan2 = (int)value;
		return;
	}
		case 1421783686:
	{
		engineConfiguration->boostPid.pFactor = value;
		return;
	}
		case 971442495:
	{
		engineConfiguration->boostPid.iFactor = value;
		return;
	}
		case -1190930054:
	{
		engineConfiguration->boostPid.dFactor = value;
		return;
	}
		case 1435801342:
	{
		engineConfiguration->boostPid.offset = (int)value;
		return;
	}
		case -945021094:
	{
		engineConfiguration->boostPid.periodMs = (int)value;
		return;
	}
		case 777954680:
	{
		engineConfiguration->boostPid.minValue = (int)value;
		return;
	}
		case -572500550:
	{
		engineConfiguration->boostPid.maxValue = (int)value;
		return;
	}
		case -1990709262:
	{
		engineConfiguration->boostPwmFrequency = (int)value;
		return;
	}
		case -1743747394:
	{
		engineConfiguration->launchSpeedThreshold = (int)value;
		return;
	}
		case 906908772:
	{
		engineConfiguration->launchTimingRpmRange = (int)value;
		return;
	}
		case 975838686:
	{
		engineConfiguration->launchFuelAdded = (int)value;
		return;
	}
		case -232786835:
	{
		engineConfiguration->launchBoostDuty = (int)value;
		return;
	}
		case -163078708:
	{
		engineConfiguration->hardCutRpmRange = (int)value;
		return;
	}
		case 983814707:
	{
		engineConfiguration->turbochargerFilter = value;
		return;
	}
		case -1340834204:
	{
		engineConfiguration->launchTpsThreshold = (int)value;
		return;
	}
		case -1153051360:
	{
		engineConfiguration->launchActivateDelay = value;
		return;
	}
		case 1887120139:
	{
		engineConfiguration->stft.maxIdleRegionRpm = (int)value;
		return;
	}
		case 1673018859:
	{
		engineConfiguration->stft.maxOverrunLoad = (int)value;
		return;
	}
		case -1942479387:
	{
		engineConfiguration->stft.minPowerLoad = (int)value;
		return;
	}
		case -1157123369:
	{
		engineConfiguration->stft.deadband = (int)value;
		return;
	}
		case -81412933:
	{
		engineConfiguration->stft.minClt = (int)value;
		return;
	}
		case -81415311:
	{
		engineConfiguration->stft.minAfr = (int)value;
		return;
	}
		case -90543309:
	{
		engineConfiguration->stft.maxAfr = (int)value;
		return;
	}
		case 1028902582:
	{
		engineConfiguration->stft.startupDelay = (int)value;
		return;
	}
		case 18659930:
	{
		engineConfiguration->tps2SecondaryMin = (int)value;
		return;
	}
		case 18659676:
	{
		engineConfiguration->tps2SecondaryMax = (int)value;
		return;
	}
		case 1445062086:
	{
		engineConfiguration->widebandOnSecondBus = (int)value;
		return;
	}
		case 1611799720:
	{
		engineConfiguration->fuelClosedLoopCorrectionEnabled = (int)value;
		return;
	}
		case -546057276:
	{
		engineConfiguration->isVerboseIAC = (int)value;
		return;
	}
		case 885690524:
	{
		engineConfiguration->boardUseTachPullUp = (int)value;
		return;
	}
		case -1457484942:
	{
		engineConfiguration->boardUseTempPullUp = (int)value;
		return;
	}
		case -1985448683:
	{
		engineConfiguration->yesUnderstandLocking = (int)value;
		return;
	}
		case 582390322:
	{
		engineConfiguration->silentTriggerError = (int)value;
		return;
	}
		case -1816997974:
	{
		engineConfiguration->useLinearCltSensor = (int)value;
		return;
	}
		case -528008450:
	{
		engineConfiguration->canReadEnabled = (int)value;
		return;
	}
		case 2104472173:
	{
		engineConfiguration->canWriteEnabled = (int)value;
		return;
	}
		case 667504645:
	{
		engineConfiguration->useLinearIatSensor = (int)value;
		return;
	}
		case -1470758947:
	{
		engineConfiguration->boardUse2stepPullDown = (int)value;
		return;
	}
		case 99110078:
	{
		engineConfiguration->tachPulseDurationAsDutyCycle = (int)value;
		return;
	}
		case -1771449111:
	{
		engineConfiguration->isAlternatorControlEnabled = (int)value;
		return;
	}
		case -587700301:
	{
		engineConfiguration->invertPrimaryTriggerSignal = (int)value;
		return;
	}
		case -1944260329:
	{
		engineConfiguration->invertSecondaryTriggerSignal = (int)value;
		return;
	}
		case -794334344:
	{
		engineConfiguration->cutFuelOnHardLimit = (int)value;
		return;
	}
		case -1087585075:
	{
		engineConfiguration->cutSparkOnHardLimit = (int)value;
		return;
	}
		case -980843809:
	{
		engineConfiguration->launchFuelCutEnable = (int)value;
		return;
	}
		case -1829995308:
	{
		engineConfiguration->launchSparkCutEnable = (int)value;
		return;
	}
		case 1280316843:
	{
		engineConfiguration->boardUseCrankPullUp = (int)value;
		return;
	}
		case 1304425152:
	{
		engineConfiguration->boardUseCamPullDown = (int)value;
		return;
	}
		case -94892331:
	{
		engineConfiguration->boardUseCamVrPullUp = (int)value;
		return;
	}
		case 1868156805:
	{
		engineConfiguration->boardUseD2PullDown = (int)value;
		return;
	}
		case -472498042:
	{
		engineConfiguration->boardUseD3PullDown = (int)value;
		return;
	}
		case 1481814407:
	{
		engineConfiguration->boardUseD4PullDown = (int)value;
		return;
	}
		case -858840440:
	{
		engineConfiguration->boardUseD5PullDown = (int)value;
		return;
	}
		case -487204662:
	{
		engineConfiguration->verboseIsoTp = (int)value;
		return;
	}
		case 465982216:
	{
		engineConfiguration->engineSnifferFocusOnInputs = (int)value;
		return;
	}
		case -1884116782:
	{
		engineConfiguration->launchActivateInverted = (int)value;
		return;
	}
		case 38719415:
	{
		engineConfiguration->twoStroke = (int)value;
		return;
	}
		case 1538579992:
	{
		engineConfiguration->skippedWheelOnCam = (int)value;
		return;
	}
		case 1664668368:
	{
		engineConfiguration->etbNeutralPosition = (int)value;
		return;
	}
		case 1917301199:
	{
		engineConfiguration->isInjectionEnabled = (int)value;
		return;
	}
		case -1354884979:
	{
		engineConfiguration->isIgnitionEnabled = (int)value;
		return;
	}
		case -1491889938:
	{
		engineConfiguration->isCylinderCleanupEnabled = (int)value;
		return;
	}
		case -2018683234:
	{
		engineConfiguration->complexWallModel = (int)value;
		return;
	}
		case -1934896346:
	{
		engineConfiguration->alwaysInstantRpm = (int)value;
		return;
	}
		case 285726462:
	{
		engineConfiguration->isMapAveragingEnabled = (int)value;
		return;
	}
		case -1500666851:
	{
		engineConfiguration->overrideCrankingIacSetting = (int)value;
		return;
	}
		case 216872574:
	{
		engineConfiguration->useSeparateAdvanceForIdle = (int)value;
		return;
	}
		case -726159643:
	{
		engineConfiguration->isWaveAnalyzerEnabled = (int)value;
		return;
	}
		case -1997438905:
	{
		engineConfiguration->useSeparateVeForIdle = (int)value;
		return;
	}
		case -325018342:
	{
		engineConfiguration->verboseTriggerSynchDetails = (int)value;
		return;
	}
		case -795241238:
	{
		engineConfiguration->isManualSpinningMode = (int)value;
		return;
	}
		case -1248413509:
	{
		engineConfiguration->twoWireBatchInjection = (int)value;
		return;
	}
		case 240697653:
	{
		engineConfiguration->neverInstantRpm = (int)value;
		return;
	}
		case 201676494:
	{
		engineConfiguration->useFixedBaroCorrFromMap = (int)value;
		return;
	}
		case 685453869:
	{
		engineConfiguration->useSeparateAdvanceForCranking = (int)value;
		return;
	}
		case 1060606307:
	{
		engineConfiguration->useAdvanceCorrectionsForCranking = (int)value;
		return;
	}
		case -563973567:
	{
		engineConfiguration->flexCranking = (int)value;
		return;
	}
		case -364626778:
	{
		engineConfiguration->useIacPidMultTable = (int)value;
		return;
	}
		case -1832431020:
	{
		engineConfiguration->isBoostControlEnabled = (int)value;
		return;
	}
		case -954321508:
	{
		engineConfiguration->launchSmoothRetard = (int)value;
		return;
	}
		case -1543452424:
	{
		engineConfiguration->isPhaseSyncRequiredForIgnition = (int)value;
		return;
	}
		case 939816194:
	{
		engineConfiguration->useCltBasedRpmLimit = (int)value;
		return;
	}
		case -389678699:
	{
		engineConfiguration->forceO2Heating = (int)value;
		return;
	}
		case 755512282:
	{
		engineConfiguration->invertVvtControlIntake = (int)value;
		return;
	}
		case -1331668704:
	{
		engineConfiguration->invertVvtControlExhaust = (int)value;
		return;
	}
		case 1486014321:
	{
		engineConfiguration->useBiQuadOnAuxSpeedSensors = (int)value;
		return;
	}
		case -105464622:
	{
		engineConfiguration->sdTriggerLog = (int)value;
		return;
	}
		case -1027820105:
	{
		engineConfiguration->ALSActivateInverted = (int)value;
		return;
	}
		case -1492464952:
	{
		engineConfiguration->engineChartSize = (int)value;
		return;
	}
		case -1056007485:
	{
		engineConfiguration->turboSpeedSensorMultiplier = value;
		return;
	}
		case 1683471850:
	{
		engineConfiguration->acIdleRpmBump = (int)value;
		return;
	}
		case 1322924702:
	{
		engineConfiguration->warningPeriod = (int)value;
		return;
	}
		case -339821344:
	{
		engineConfiguration->knockDetectionWindowStart = value;
		return;
	}
		case -1562133783:
	{
		engineConfiguration->knockDetectionWindowEnd = value;
		return;
	}
		case 1145888170:
	{
		engineConfiguration->idleStepperReactionTime = value;
		return;
	}
		case -1077032999:
	{
		engineConfiguration->idleStepperTotalSteps = (int)value;
		return;
	}
		case -66405381:
	{
		engineConfiguration->noAccelAfterHardLimitPeriodSecs = value;
		return;
	}
		case 1556302698:
	{
		engineConfiguration->mapAveragingSchedulingAtIndex = (int)value;
		return;
	}
		case 58931511:
	{
		engineConfiguration->tachPulseDuractionMs = value;
		return;
	}
		case -390306941:
	{
		engineConfiguration->wwaeTau = value;
		return;
	}
		case -1993968289:
	{
		engineConfiguration->alternatorControl.pFactor = value;
		return;
	}
		case 1850657816:
	{
		engineConfiguration->alternatorControl.iFactor = value;
		return;
	}
		case -311714733:
	{
		engineConfiguration->alternatorControl.dFactor = value;
		return;
	}
		case 1983046327:
	{
		engineConfiguration->alternatorControl.offset = (int)value;
		return;
	}
		case -1995686573:
	{
		engineConfiguration->alternatorControl.periodMs = (int)value;
		return;
	}
		case -272710799:
	{
		engineConfiguration->alternatorControl.minValue = (int)value;
		return;
	}
		case -1623166029:
	{
		engineConfiguration->alternatorControl.maxValue = (int)value;
		return;
	}
		case 196797309:
	{
		engineConfiguration->etb.pFactor = value;
		return;
	}
		case -253543882:
	{
		engineConfiguration->etb.iFactor = value;
		return;
	}
		case 1879050865:
	{
		engineConfiguration->etb.dFactor = value;
		return;
	}
		case -423426795:
	{
		engineConfiguration->etb.offset = (int)value;
		return;
	}
		case 1580101425:
	{
		engineConfiguration->etb.periodMs = (int)value;
		return;
	}
		case -991890097:
	{
		engineConfiguration->etb.minValue = (int)value;
		return;
	}
		case 1952621969:
	{
		engineConfiguration->etb.maxValue = (int)value;
		return;
	}
		case -174866119:
	{
		engineConfiguration->airTaperRpmRange = (int)value;
		return;
	}
		case -224184878:
	{
		engineConfiguration->tps2Min = (int)value;
		return;
	}
		case -224185132:
	{
		engineConfiguration->tps2Max = (int)value;
		return;
	}
		case -2142425790:
	{
		engineConfiguration->tachPulsePerRev = (int)value;
		return;
	}
		case 9388880:
	{
		engineConfiguration->mapErrorDetectionTooLow = value;
		return;
	}
		case 309682334:
	{
		engineConfiguration->mapErrorDetectionTooHigh = value;
		return;
	}
		case 48235288:
	{
		engineConfiguration->multisparkSparkDuration = (int)value;
		return;
	}
		case 617337449:
	{
		engineConfiguration->multisparkDwell = (int)value;
		return;
	}
		case -2146434132:
	{
		engineConfiguration->idleRpmPid.pFactor = value;
		return;
	}
		case 1698191973:
	{
		engineConfiguration->idleRpmPid.iFactor = value;
		return;
	}
		case -464180576:
	{
		engineConfiguration->idleRpmPid.dFactor = value;
		return;
	}
		case -103982236:
	{
		engineConfiguration->idleRpmPid.offset = (int)value;
		return;
	}
		case 1562875200:
	{
		engineConfiguration->idleRpmPid.periodMs = (int)value;
		return;
	}
		case -1009116322:
	{
		engineConfiguration->idleRpmPid.minValue = (int)value;
		return;
	}
		case 1935395744:
	{
		engineConfiguration->idleRpmPid.maxValue = (int)value;
		return;
	}
		case 4130389:
	{
		engineConfiguration->wwaeBeta = value;
		return;
	}
		case -2130836456:
	{
		engineConfiguration->throttlePedalUpVoltage = value;
		return;
	}
		case 2099527693:
	{
		engineConfiguration->throttlePedalWOTVoltage = value;
		return;
	}
		case -664540020:
	{
		engineConfiguration->startUpFuelPumpDuration = (int)value;
		return;
	}
		case -528043591:
	{
		engineConfiguration->idlePidRpmDeadZone = (int)value;
		return;
	}
		case 1160967565:
	{
		engineConfiguration->targetVBatt = value;
		return;
	}
		case -1166743424:
	{
		engineConfiguration->alternatorOffAboveTps = value;
		return;
	}
		case 671504531:
	{
		engineConfiguration->afterCrankingIACtaperDuration = (int)value;
		return;
	}
		case -2065814304:
	{
		engineConfiguration->iacByTpsTaper = (int)value;
		return;
	}
		case -1164331270:
	{
		engineConfiguration->tpsAccelLookback = (int)value;
		return;
	}
		case 329016035:
	{
		engineConfiguration->coastingFuelCutVssLow = (int)value;
		return;
	}
		case -2027523439:
	{
		engineConfiguration->coastingFuelCutVssHigh = (int)value;
		return;
	}
		case 1286770247:
	{
		engineConfiguration->noFuelTrimAfterDfcoTime = (int)value;
		return;
	}
		case 1313479950:
	{
		engineConfiguration->tpsAccelEnrichmentThreshold = value;
		return;
	}
		case -2114430012:
	{
		engineConfiguration->totalGearsCount = (int)value;
		return;
	}
		case 1397079021:
	{
		engineConfiguration->fan1ExtraIdle = (int)value;
		return;
	}
		case -999870619:
	{
		engineConfiguration->uartConsoleSerialSpeed = (int)value;
		return;
	}
		case -965453331:
	{
		engineConfiguration->tpsDecelEnleanmentThreshold = value;
		return;
	}
		case 1164012327:
	{
		engineConfiguration->tpsDecelEnleanmentMultiplier = value;
		return;
	}
		case 781749924:
	{
		engineConfiguration->auxSerialSpeed = (int)value;
		return;
	}
		case -1564912640:
	{
		engineConfiguration->throttlePedalSecondaryUpVoltage = value;
		return;
	}
		case -699822859:
	{
		engineConfiguration->throttlePedalSecondaryWOTVoltage = value;
		return;
	}
		case 578933872:
	{
		engineConfiguration->mc33_hpfp_i_peak = (int)value;
		return;
	}
		case 578657622:
	{
		engineConfiguration->mc33_hpfp_i_hold = (int)value;
		return;
	}
		case 944657936:
	{
		engineConfiguration->mc33_hpfp_i_hold_off = (int)value;
		return;
	}
		case 663037299:
	{
		engineConfiguration->mc33_hpfp_max_hold = (int)value;
		return;
	}
		case 337194154:
	{
		engineConfiguration->stepperDcInvertedPins = (int)value;
		return;
	}
		case -631762165:
	{
		engineConfiguration->canOpenBLT = (int)value;
		return;
	}
		case -1514354211:
	{
		engineConfiguration->can2OpenBLT = (int)value;
		return;
	}
		case 444648859:
	{
		engineConfiguration->injectorFlowAsMassFlow = (int)value;
		return;
	}
		case -1655178001:
	{
		engineConfiguration->benchTestOffTime = (int)value;
		return;
	}
		case -1992945039:
	{
		engineConfiguration->boostCutPressure = value;
		return;
	}
		case -1359134883:
	{
		engineConfiguration->fixedTiming = value;
		return;
	}
		case -137390524:
	{
		engineConfiguration->mapLowValueVoltage = value;
		return;
	}
		case -1084262094:
	{
		engineConfiguration->mapHighValueVoltage = value;
		return;
	}
		case 280323355:
	{
		engineConfiguration->egoValueShift = value;
		return;
	}
		case -224532684:
	{
		engineConfiguration->crankingIACposition = (int)value;
		return;
	}
		case -557658095:
	{
		engineConfiguration->tChargeMinRpmMinTps = value;
		return;
	}
		case -566786093:
	{
		engineConfiguration->tChargeMinRpmMaxTps = value;
		return;
	}
		case -599338669:
	{
		engineConfiguration->tChargeMaxRpmMinTps = value;
		return;
	}
		case -608466667:
	{
		engineConfiguration->tChargeMaxRpmMaxTps = value;
		return;
	}
		case 799798215:
	{
		engineConfiguration->alternatorPwmFrequency = (int)value;
		return;
	}
		case 1464880398:
	{
		engineConfiguration->fan2ExtraIdle = (int)value;
		return;
	}
		case 1506446058:
	{
		engineConfiguration->primingDelay = (int)value;
		return;
	}
		case -395416807:
	{
		engineConfiguration->fuelReferencePressure = value;
		return;
	}
		case -1297058729:
	{
		engineConfiguration->postCrankingFactor = value;
		return;
	}
		case 1183263609:
	{
		engineConfiguration->postCrankingDurationSec = value;
		return;
	}
		case -1104834801:
	{
		engineConfiguration->auxTempSensor1.config.tempC_1 = (int)value;
		return;
	}
		case -1104834800:
	{
		engineConfiguration->auxTempSensor1.config.tempC_2 = (int)value;
		return;
	}
		case -1104834799:
	{
		engineConfiguration->auxTempSensor1.config.tempC_3 = (int)value;
		return;
	}
		case 629168263:
	{
		engineConfiguration->auxTempSensor1.config.resistance_1 = (int)value;
		return;
	}
		case 629168264:
	{
		engineConfiguration->auxTempSensor1.config.resistance_2 = (int)value;
		return;
	}
		case 629168265:
	{
		engineConfiguration->auxTempSensor1.config.resistance_3 = (int)value;
		return;
	}
		case -1189603841:
	{
		engineConfiguration->auxTempSensor1.config.bias_resistor = (int)value;
		return;
	}
		case -900025104:
	{
		engineConfiguration->auxTempSensor2.config.tempC_1 = (int)value;
		return;
	}
		case -900025103:
	{
		engineConfiguration->auxTempSensor2.config.tempC_2 = (int)value;
		return;
	}
		case -900025102:
	{
		engineConfiguration->auxTempSensor2.config.tempC_3 = (int)value;
		return;
	}
		case 1988973320:
	{
		engineConfiguration->auxTempSensor2.config.resistance_1 = (int)value;
		return;
	}
		case 1988973321:
	{
		engineConfiguration->auxTempSensor2.config.resistance_2 = (int)value;
		return;
	}
		case 1988973322:
	{
		engineConfiguration->auxTempSensor2.config.resistance_3 = (int)value;
		return;
	}
		case 734290080:
	{
		engineConfiguration->auxTempSensor2.config.bias_resistor = (int)value;
		return;
	}
		case -430252676:
	{
		engineConfiguration->knockSamplingDuration = (int)value;
		return;
	}
		case 2016887022:
	{
		engineConfiguration->etbFreq = (int)value;
		return;
	}
		case 1746193407:
	{
		engineConfiguration->etbWastegatePid.pFactor = value;
		return;
	}
		case 1295852216:
	{
		engineConfiguration->etbWastegatePid.iFactor = value;
		return;
	}
		case -866520333:
	{
		engineConfiguration->etbWastegatePid.dFactor = value;
		return;
	}
		case 794879319:
	{
		engineConfiguration->etbWastegatePid.offset = (int)value;
		return;
	}
		case 1170565107:
	{
		engineConfiguration->etbWastegatePid.periodMs = (int)value;
		return;
	}
		case -1401426415:
	{
		engineConfiguration->etbWastegatePid.minValue = (int)value;
		return;
	}
		case 1543085651:
	{
		engineConfiguration->etbWastegatePid.maxValue = (int)value;
		return;
	}
		case -1147263902:
	{
		engineConfiguration->stepperMinDutyCycle = (int)value;
		return;
	}
		case -1188944476:
	{
		engineConfiguration->stepperMaxDutyCycle = (int)value;
		return;
	}
		case -1562952959:
	{
		engineConfiguration->idlePidActivationTime = value;
		return;
	}
		case -613392361:
	{
		engineConfiguration->oilPressure.v1 = value;
		return;
	}
		case -505257058:
	{
		engineConfiguration->oilPressure.value1 = value;
		return;
	}
		case -613392360:
	{
		engineConfiguration->oilPressure.v2 = value;
		return;
	}
		case -505257057:
	{
		engineConfiguration->oilPressure.value2 = value;
		return;
	}
		case 871106583:
	{
		engineConfiguration->fan2OnTemperature = (int)value;
		return;
	}
		case 1077467605:
	{
		engineConfiguration->fan2OffTemperature = (int)value;
		return;
	}
		case 383307083:
	{
		engineConfiguration->auxFrequencyFilter = value;
		return;
	}
		case 1470846937:
	{
		engineConfiguration->vvtControlMinRpm = (int)value;
		return;
	}
		case -904771683:
	{
		engineConfiguration->launchFuelAdderPercent = (int)value;
		return;
	}
		case 1057701663:
	{
		engineConfiguration->etbJamTimeout = (int)value;
		return;
	}
		case -86185846:
	{
		engineConfiguration->etbExpAverageLength = (int)value;
		return;
	}
		case 1272017572:
	{
		engineConfiguration->coastingFuelCutRpmHigh = (int)value;
		return;
	}
		case 168701078:
	{
		engineConfiguration->coastingFuelCutRpmLow = (int)value;
		return;
	}
		case -1494035092:
	{
		engineConfiguration->coastingFuelCutTps = (int)value;
		return;
	}
		case -1494053736:
	{
		engineConfiguration->coastingFuelCutClt = (int)value;
		return;
	}
		case 1414346958:
	{
		engineConfiguration->pidExtraForLowRpm = (int)value;
		return;
	}
		case -1494043213:
	{
		engineConfiguration->coastingFuelCutMap = (int)value;
		return;
	}
		case 2032707103:
	{
		engineConfiguration->highPressureFuel.v1 = value;
		return;
	}
		case -1898025562:
	{
		engineConfiguration->highPressureFuel.value1 = value;
		return;
	}
		case 2032707104:
	{
		engineConfiguration->highPressureFuel.v2 = value;
		return;
	}
		case -1898025561:
	{
		engineConfiguration->highPressureFuel.value2 = value;
		return;
	}
		case -474791503:
	{
		engineConfiguration->lowPressureFuel.v1 = value;
		return;
	}
		case 764445240:
	{
		engineConfiguration->lowPressureFuel.value1 = value;
		return;
	}
		case -474791502:
	{
		engineConfiguration->lowPressureFuel.v2 = value;
		return;
	}
		case 764445241:
	{
		engineConfiguration->lowPressureFuel.value2 = value;
		return;
	}
		case 2115625312:
	{
		engineConfiguration->tChargeAirCoefMin = value;
		return;
	}
		case 2115625058:
	{
		engineConfiguration->tChargeAirCoefMax = value;
		return;
	}
		case 1590125629:
	{
		engineConfiguration->tChargeAirFlowMax = value;
		return;
	}
		case -1128797750:
	{
		engineConfiguration->tChargeAirIncrLimit = value;
		return;
	}
		case 670644444:
	{
		engineConfiguration->tChargeAirDecrLimit = value;
		return;
	}
		case -1725192476:
	{
		engineConfiguration->etb_iTermMin = (int)value;
		return;
	}
		case -1725192730:
	{
		engineConfiguration->etb_iTermMax = (int)value;
		return;
	}
		case -1411996731:
	{
		engineConfiguration->idleTimingPid.pFactor = value;
		return;
	}
		case -1862337922:
	{
		engineConfiguration->idleTimingPid.iFactor = value;
		return;
	}
		case 270256825:
	{
		engineConfiguration->idleTimingPid.dFactor = value;
		return;
	}
		case -2033984419:
	{
		engineConfiguration->idleTimingPid.offset = (int)value;
		return;
	}
		case 29505657:
	{
		engineConfiguration->idleTimingPid.periodMs = (int)value;
		return;
	}
		case 1752481431:
	{
		engineConfiguration->idleTimingPid.minValue = (int)value;
		return;
	}
		case 402026201:
	{
		engineConfiguration->idleTimingPid.maxValue = (int)value;
		return;
	}
		case 481957934:
	{
		engineConfiguration->etbRocExpAverageLength = (int)value;
		return;
	}
		case 1117566701:
	{
		engineConfiguration->tpsAccelFractionPeriod = (int)value;
		return;
	}
		case 68539114:
	{
		engineConfiguration->tpsAccelFractionDivisor = value;
		return;
	}
		case -1742418701:
	{
		engineConfiguration->idlerpmpid_iTermMin = (int)value;
		return;
	}
		case 1762426578:
	{
		engineConfiguration->stoichRatioPrimary = (int)value;
		return;
	}
		case -1742418955:
	{
		engineConfiguration->idlerpmpid_iTermMax = (int)value;
		return;
	}
		case -54532767:
	{
		engineConfiguration->etbIdleThrottleRange = value;
		return;
	}
		case 1455224910:
	{
		engineConfiguration->triggerCompCenterVolt = (int)value;
		return;
	}
		case -25671212:
	{
		engineConfiguration->triggerCompHystMin = (int)value;
		return;
	}
		case -25671466:
	{
		engineConfiguration->triggerCompHystMax = (int)value;
		return;
	}
		case -953183719:
	{
		engineConfiguration->triggerCompSensorSatRpm = (int)value;
		return;
	}
		case -371707639:
	{
		engineConfiguration->mc33_i_boost = (int)value;
		return;
	}
		case 379679459:
	{
		engineConfiguration->mc33_i_peak = (int)value;
		return;
	}
		case 379403209:
	{
		engineConfiguration->mc33_i_hold = (int)value;
		return;
	}
		case -486240071:
	{
		engineConfiguration->mc33_t_max_boost = (int)value;
		return;
	}
		case -893360504:
	{
		engineConfiguration->mc33_t_peak_off = (int)value;
		return;
	}
		case -893354748:
	{
		engineConfiguration->mc33_t_peak_tot = (int)value;
		return;
	}
		case 1281862911:
	{
		engineConfiguration->mc33_t_bypass = (int)value;
		return;
	}
		case -2086522258:
	{
		engineConfiguration->mc33_t_hold_off = (int)value;
		return;
	}
		case -2086516502:
	{
		engineConfiguration->mc33_t_hold_tot = (int)value;
		return;
	}
		case -2070332548:
	{
		engineConfiguration->maxCamPhaseResolveRpm = (int)value;
		return;
	}
		case 1676700208:
	{
		engineConfiguration->dfcoDelay = (int)value;
		return;
	}
		case 483010008:
	{
		engineConfiguration->acDelay = (int)value;
		return;
	}
		case -1032938305:
	{
		engineConfiguration->fordInjectorSmallPulseBreakPoint = (int)value;
		return;
	}
		case -929987018:
	{
		engineConfiguration->etbJamIntegratorLimit = (int)value;
		return;
	}
		case -1256141959:
	{
		engineConfiguration->hpfpCamLobes = (int)value;
		return;
	}
		case 1145081382:
	{
		engineConfiguration->hpfpPeakPos = (int)value;
		return;
	}
		case 1494394654:
	{
		engineConfiguration->hpfpMinAngle = (int)value;
		return;
	}
		case 334494733:
	{
		engineConfiguration->hpfpPumpVolume = (int)value;
		return;
	}
		case -81483444:
	{
		engineConfiguration->hpfpActivationAngle = (int)value;
		return;
	}
		case -413489794:
	{
		engineConfiguration->issFilterReciprocal = (int)value;
		return;
	}
		case 436500704:
	{
		engineConfiguration->hpfpPidP = (int)value;
		return;
	}
		case 436500697:
	{
		engineConfiguration->hpfpPidI = (int)value;
		return;
	}
		case 1025426144:
	{
		engineConfiguration->hpfpTargetDecay = (int)value;
		return;
	}
		case 288250022:
	{
		engineConfiguration->vvtActivationDelayMs = (int)value;
		return;
	}
		case 1574876352:
	{
		engineConfiguration->acrDisablePhase = (int)value;
		return;
	}
		case -1415669996:
	{
		engineConfiguration->auxLinear1.v1 = value;
		return;
	}
		case -65154789:
	{
		engineConfiguration->auxLinear1.value1 = value;
		return;
	}
		case -1415669995:
	{
		engineConfiguration->auxLinear1.v2 = value;
		return;
	}
		case -65154788:
	{
		engineConfiguration->auxLinear1.value2 = value;
		return;
	}
		case -1415634059:
	{
		engineConfiguration->auxLinear2.v1 = value;
		return;
	}
		case -396384772:
	{
		engineConfiguration->auxLinear2.value1 = value;
		return;
	}
		case -1415634058:
	{
		engineConfiguration->auxLinear2.v2 = value;
		return;
	}
		case -396384771:
	{
		engineConfiguration->auxLinear2.value2 = value;
		return;
	}
		case -1855700175:
	{
		engineConfiguration->etbMinimumPosition = value;
		return;
	}
		case -243078627:
	{
		engineConfiguration->tuneHidingKey = (int)value;
		return;
	}
		case 1532957848:
	{
		engineConfiguration->ALSMinRPM = (int)value;
		return;
	}
		case 1523829850:
	{
		engineConfiguration->ALSMaxRPM = (int)value;
		return;
	}
		case 1830002033:
	{
		engineConfiguration->ALSMaxDuration = (int)value;
		return;
	}
		case 1532941388:
	{
		engineConfiguration->ALSMinCLT = (int)value;
		return;
	}
		case 1523813390:
	{
		engineConfiguration->ALSMaxCLT = (int)value;
		return;
	}
		case -1118241790:
	{
		engineConfiguration->alsMinTimeBetween = (int)value;
		return;
	}
		case -731677323:
	{
		engineConfiguration->alsEtbPosition = (int)value;
		return;
	}
		case -908268760:
	{
		engineConfiguration->acRelayAlternatorDutyAdder = (int)value;
		return;
	}
		case -748456478:
	{
		engineConfiguration->instantRpmRange = (int)value;
		return;
	}
		case -2021435668:
	{
		engineConfiguration->ALSIdleAdd = (int)value;
		return;
	}
		case 1232469673:
	{
		engineConfiguration->ALSEtbAdd = (int)value;
		return;
	}
		case -1404414917:
	{
		engineConfiguration->ALSSkipRatio = (int)value;
		return;
	}
		case 612659807:
	{
		engineConfiguration->ALSMaxDriverThrottleIntent = (int)value;
		return;
	}
		case -1744146782:
	{
		engineConfiguration->tpsSecondaryMaximum = (int)value;
		return;
	}
		case -727657058:
	{
		engineConfiguration->ppsSecondaryMaximum = (int)value;
		return;
	}
		case 1756514810:
	{
		engineConfiguration->rpmHardLimitHyst = (int)value;
		return;
	}
	}
}
