# m111-alex-comparing-against-current-m111-alex-default

// canned tune https://rusefi.com/online/view.php?msq=1622

```
    // default 0.098
    engineConfiguration->tChargeAirCoefMin = 0.4;
    // default 0.902
    engineConfiguration->tChargeAirCoefMax = 0.889999;
    // default 153.6
    engineConfiguration->tChargeAirFlowMax = 220;
    // default "Sequential"
    engineConfiguration->injectionMode = IM_BATCH;
    // default "Individual Coils"
    engineConfiguration->ignitionMode = IM_WASTED_SPARK;
    // default 0.0
    engineConfiguration->fixedTiming = 10;
    // default 2.295
    engineConfiguration->displacement = 2.2;
    // default 469.0
    engineConfiguration->globalTriggerAngleOffset = 72;
    // default 40.0
    engineConfiguration->camDecoder2jzPrecision = 25;
    // default 450.0
    engineConfiguration->vvtOffsets[0] = 0;
    // default 196.0
    engineConfiguration->injector.flow = 222;
    // default 300.0
    engineConfiguration->fuelReferencePressure = 0;
    // default 0.0
    engineConfiguration->cylinderBankSelect[4] = 1;
    // default 0.0
    engineConfiguration->cylinderBankSelect[5] = 1;
    // default 0.0
    engineConfiguration->cylinderBankSelect[6] = 1;
    // default 0.0
    engineConfiguration->cylinderBankSelect[7] = 1;
    // default 0.0
    engineConfiguration->cylinderBankSelect[8] = 1;
    // default 0.0
    engineConfiguration->cylinderBankSelect[9] = 1;
    // default 0.0
    engineConfiguration->cylinderBankSelect[10] = 1;
    // default 0.0
    engineConfiguration->cylinderBankSelect[11] = 1;
    // default 4.0
    engineConfiguration->benchTestOnTime = 5;
    // default 891.0
    engineConfiguration->tpsMin = 896;
    // default 69.0
    engineConfiguration->tpsMax = 39;
    // default 98.0
    engineConfiguration->tps1SecondaryMin = 99;
    // default 926.0
    engineConfiguration->tps1SecondaryMax = 911;
    // default 1000.0
    engineConfiguration->tps2SecondaryMin = 0;
    // default 0.0
    engineConfiguration->tps2SecondaryMax = 1000;
    // default 0.38
    engineConfiguration->throttlePedalUpVoltage = 0.326213;
    // default 4.77
    engineConfiguration->throttlePedalWOTVoltage = 4.58342;
    // default 4.64
    engineConfiguration->throttlePedalSecondaryUpVoltage = 0.137012;
    // default 2.47
    engineConfiguration->throttlePedalSecondaryWOTVoltage = 2.082364;
    // default 65.0
    engineConfiguration->mc33_hvolt = 0;
    // default 13000.0
    engineConfiguration->mc33_i_boost = 0;
    // default 400.0
    engineConfiguration->mc33_t_max_boost = 0;
    // default 9400.0
    engineConfiguration->mc33_i_peak = 0;
    // default 700.0
    engineConfiguration->mc33_t_peak_tot = 0;
    // default 10.0
    engineConfiguration->mc33_t_peak_off = 0;
    // default 10.0
    engineConfiguration->mc33_t_bypass = 0;
    // default 3700.0
    engineConfiguration->mc33_i_hold = 0;
    // default 60.0
    engineConfiguration->mc33_t_hold_off = 0;
    // default 10000.0
    engineConfiguration->mc33_t_hold_tot = 0;
    // default "Normal"
    engineConfiguration->clutchDownPinInverted = false;
    // default "Normal"
    engineConfiguration->clutchUpPinInverted = false;
    // default "Normal"
    engineConfiguration->flexSensorInverted = false;
    // default 1.0
    engineConfiguration->turboSpeedSensorMultiplier = 0;
    // default 0.3
    engineConfiguration->mapLowValueVoltage = 0.2;
    // default 4.65
    engineConfiguration->mapHighValueVoltage = 4.950012;
    // default 410.0
    engineConfiguration->mapErrorDetectionTooHigh = 250;
    // default 1.0
    engineConfiguration->mapMinBufferLength = 2;
    // default 0.0
    engineConfiguration->baroSensor.lowValue = 38.5;
    // default 500.0
    engineConfiguration->baroSensor.highValue = 250;
    // default "false"
    engineConfiguration->enableAemXSeries = true;
    // default 300.0
    engineConfiguration->idle.solenoidFrequency = 200;
    // default 3.0
    engineConfiguration->idleStepperReactionTime = 10;
    // default 200.0
    engineConfiguration->idleStepperTotalSteps = 150;
    // default "true"
    engineConfiguration->stepperForceParkingEveryRestart = false;
    // default 15.0
    engineConfiguration->etbIdleThrottleRange = 10;
    // default 0.05
    engineConfiguration->idleRpmPid.pFactor = 0.002;
    // default 0.002
    engineConfiguration->idleRpmPid.iFactor = 0.004;
    // default -20.0
    engineConfiguration->idleRpmPid.minValue = -10;
    // default 20.0
    engineConfiguration->idleRpmPid.maxValue = 15;
    // default -20.0
    engineConfiguration->idlerpmpid_iTermMin = -100;
    // default 20.0
    engineConfiguration->idlerpmpid_iTermMax = 100;
    // default 0.0
    engineConfiguration->pidExtraForLowRpm = 45;
    // default "false"
    engineConfiguration->useIacPidMultTable = true;
    // default 50.0
    engineConfiguration->manIdlePosition = 32;
    // default 15.0
    engineConfiguration->acIdleExtraOffset = 2;
    // default 2.0
    engineConfiguration->fan1ExtraIdle = 0;
    // default 2.0
    engineConfiguration->fan2ExtraIdle = 0;
    // default 2.0
    engineConfiguration->iacByTpsTaper = 0;
    // default 5.0
    engineConfiguration->idlePidDeactivationTpsThreshold = 4;
    // default 300.0
    engineConfiguration->idlePidRpmUpperLimit = 450;
    // default 50.0
    engineConfiguration->idlePidRpmDeadZone = 10;
    // default "Open Loop"
    engineConfiguration->idleMode = IM_AUTO;
    // default "false"
    engineConfiguration->useIdleTimingPidControl = true;
    // default -10.0
    engineConfiguration->idleTimingPid.minValue = -15;
    // default 92.0
    engineConfiguration->fanOnTemperature = 95;
    // default 88.0
    engineConfiguration->fanOffTemperature = 91;
    // default 0.5
    engineConfiguration->acDelay = 0;
    // default 4.0
    engineConfiguration->startUpFuelPumpDuration = 1;
    // default 3.0
    engineConfiguration->startCrankingDuration = 5;
    // default 0.0
    engineConfiguration->knockBandCustom = 12.80005;
    // default 20.0
    engineConfiguration->knockRetardAggression = 5;
    // default 3.0
    engineConfiguration->knockRetardReapplyRate = 0.1;
    // default 1000.0
    engineConfiguration->stft.maxIdleRegionRpm = 900;
    // default 60.0
    engineConfiguration->stft.startupDelay = 90;
    // default 60.0
    engineConfiguration->stft.minClt = 70;
    // default 12.0
    engineConfiguration->stft.minAfr = 10;
    // default 6000.0
    engineConfiguration->vvtActivationDelayMs = 0;
    // default 500.0
    engineConfiguration->vvtControlMinRpm = 550;
    // default 33.0
    engineConfiguration->auxPid[0].offset = 0;
    // default 2.0
    engineConfiguration->auxPid[0].pFactor = 0;
    // default 0.005
    engineConfiguration->auxPid[0].iFactor = 0;
    // default 10.0
    engineConfiguration->auxPid[1].minValue = 0;
    // default 90.0
    engineConfiguration->auxPid[1].maxValue = 0;
    // default 0.0
    engineConfiguration->alternatorControl.minValue = 10;
    // default 3.0
    engineConfiguration->vssFilterReciprocal = 10;
    // default 1000.0
    engineConfiguration->driveWheelRevPerKm = 500;
    // default 1.0
    engineConfiguration->finalGearRatio = 0;
    // default 38400.0
    engineConfiguration->tunerStudioSerialSpeed = 115200;
    // default "500kbps"
    engineConfiguration->can2BaudRate = B100KBPS;
    // default 0.0
    engineConfiguration->auxSerialSpeed = 115200;
    // default "MAP"
    engineConfiguration->debugMode = DBG_STEPPER_IDLE_CONTROL;
    // default 7000.0
    engineConfiguration->rpmHardLimit = 6200;
    // default 300.0
    engineConfiguration->boostCutPressure = 0;
    // default 200.0
    engineConfiguration->rpmSoftLimitWindowSize = 0;
    // default 4.0
    engineConfiguration->rpmSoftLimitTimingRetard = 0;
    // default 0.0
    engineConfiguration->etbRevLimitStart = 6100;
    // default 250.0
    engineConfiguration->etbRevLimitRange = 200;
    // default "Sequential"
    engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
    // default 27.0
    engineConfiguration->cranking.baseFuel = 36;
    // default 200.0
    engineConfiguration->afterCrankingIACtaperDuration = 250;
    // default "false"
    engineConfiguration->overrideCrankingIacSetting = true;
    // default 6.0
    engineConfiguration->crankingTimingAngle = 3;
    // default 6.0
    engineConfiguration->ignitionDwellForCrankingMs = 5;
    // default 0.5
    engineConfiguration->primingDelay = 0;
    // default 0.0
    engineConfiguration->tpsAccelLookback = 0.3;
    // default 40.0
    engineConfiguration->tpsAccelEnrichmentThreshold = 5;
    // default 0.0
    engineConfiguration->tpsDecelEnleanmentThreshold = 12;
    // default 0.0
    engineConfiguration->tpsAccelFractionPeriod = 3;
    // default 1.0
    engineConfiguration->tpsAccelFractionDivisor = 0.3;
    // default 0.3
    engineConfiguration->wwaeTau = 0.25;
    // default 0.3
    engineConfiguration->wwaeBeta = 0.35;
    // default 0.0
    engineConfiguration->scriptSetting[0] = 60;
    // default 2000.0
    engineConfiguration->boostControlMinRpm = 0;
    // default 30.0
    engineConfiguration->boostControlMinTps = 0;
    // default 110.0
    engineConfiguration->boostControlMinMap = 0;
    // default "Throttle 2"
    engineConfiguration->etbFunctions[1] = DC_None;
    // default 5.12
    engineConfiguration->etb.pFactor = 6.2437;
    // default 47.0
    engineConfiguration->etb.iFactor = 82.52869;
    // default 0.088
    engineConfiguration->etb.dFactor = 0.0761765;
    // default -100.0
    engineConfiguration->etb.minValue = -20;
    // default -30.0
    engineConfiguration->etb_iTermMin = -25;
    // default 800.0
    engineConfiguration->etbFreq = 2000;
    // default 100.0
    engineConfiguration->etbMaximumPosition = 97;
    // default "false"
    engineConfiguration->launchControlEnabled = true;
    // default 30.0
    engineConfiguration->launchSpeedThreshold = 15;
    // default 3000.0
    engineConfiguration->launchRpm = 3500;
    // default "false"
    engineConfiguration->enableLaunchRetard = true;
    // default 0.0
    engineConfiguration->launchTimingRetard = 10;
    // default "false"
    engineConfiguration->launchFuelCutEnable = true;
    // default "Switch Input"
    engineConfiguration->antiLagActivationMode = ALWAYS_ON_ANTILAG;
    // default 60.0
    engineConfiguration->coastingFuelCutClt = 30;
    // default 1500.0
    engineConfiguration->coastingFuelCutRpmHigh = 2000;
    // default 2.0
    engineConfiguration->coastingFuelCutTps = 5;
    // default "true"
    engineConfiguration->watchOutForLinearTime = false;
    // default 250.0
    engineConfiguration->gppwm[0].pwmFrequency = 10;
    // default 0.0
    engineConfiguration->gppwm[0].dutyIfError = 10;
    // default "Zero"
    engineConfiguration->gppwm[0].loadAxis = GPPWM_Clt;
    // default 250.0
    engineConfiguration->gppwm[1].pwmFrequency = 125;
    // default "Zero"
    engineConfiguration->gppwm[1].loadAxis = GPPWM_Map;
    // default 250.0
    engineConfiguration->gppwm[2].pwmFrequency = 5;
    // default "Zero"
    engineConfiguration->gppwm[2].loadAxis = GPPWM_Map;
    // default "Zero"
    engineConfiguration->gppwm[3].loadAxis = GPPWM_Tps;
    // default 3.0
    engineConfiguration->hpfpCamLobes = 0;
    // default 10.0
    engineConfiguration->hpfpPeakPos = 0;
    // default 0.29
    engineConfiguration->hpfpPumpVolume = 0;
    // default 10.0
    engineConfiguration->hpfpMinAngle = 0;
    // default 30.0
    engineConfiguration->hpfpActivationAngle = 0;
    // default 2000.0
    engineConfiguration->hpfpTargetDecay = 0;
    // default 0.01
    engineConfiguration->hpfpPidP = 0;
    // default 3.0E-4
    engineConfiguration->hpfpPidI = 0;


	cannedknockNoiseRpmBins();
	cannedknockBaseNoise();
	cannedscriptCurve5Bins();
	cannedscriptCurve6Bins();
	cannedsparkDwellValues();
	canneddwellVoltageCorrVoltBins();
	canneddwellVoltageCorrValues();
	cannedprimeBins();
	cannedprimeValues();
	cannedmap_samplingAngleBins();
	cannedmap_samplingWindowBins();
	cannedinjector_battLagCorrBins();
	cannedinjector_battLagCorr();
	cannedvrThreshold1_rpmBins();
	cannedvrThreshold1_values();
	cannedvrThreshold2_rpmBins();
	cannedvrThreshold2_values();
	cannedmafDecodingBins();
	cannedmafDecoding();
	cannediatFuelCorr();
	cannedcltTimingBins();
	cannedcltTimingExtra();
	cannedcltFuelCorrBins();
	cannedcltFuelCorr();
	cannedcrankingFuelBins();
	cannedcrankingFuelCoef();
	cannedcrankingFuelCoefE100();
	cannedcltRevLimitRpmBins();
	cannedetbBiasBins();
	cannedetbBiasValues();
	cannedcrankingCycleCoef();
	cannedcltIdleCorrBins();
	cannedcltIdleCorr();
	cannedcltCrankingCorrBins();
	cannedcltCrankingCorr();
	cannedcltIdleRpmBins();
	cannedcltIdleRpm();
	cannedfuelLevelBins();
	cannedhpfpLobeProfileQuantityBins();
	cannedhpfpLobeProfileAngle();
	cannedhpfpDeadtimeVoltsBins();
	cannedwwTauCltValues();
	cannedwwMapBins();
	cannedwwTauMapValues();
	cannedwwBetaCltValues();
	cannedwwBetaMapValues();
	cannedignBlends1_blendBins();
	cannedignBlends1_blendValues();
	cannedignBlends2_blendBins();
	cannedignBlends2_blendValues();
	cannedignBlends3_blendBins();
	cannedignBlends3_blendValues();
	cannedignBlends4_blendBins();
	cannedignBlends4_blendValues();
	cannedveBlends1_blendBins();
	cannedveBlends1_blendValues();
	cannedveBlends2_blendBins();
	cannedveBlends2_blendValues();
	cannedveBlends3_blendBins();
	cannedveBlends3_blendValues();
	cannedveBlends4_blendBins();
	cannedveBlends4_blendValues();
	cannedtpsTpsAccelFromRpmBins();
	cannedtpsTpsAccelToRpmBins();
	cannedtpsTpsAccelTable();
	cannedboostRpmBins();
	cannedboostTpsBins();
	cannedboostTableOpenLoop();
	cannedboostTableClosedLoop();
	cannedscriptTable1RpmBins();
	cannedscriptTable1LoadBins();
	cannedscriptTable1();
	cannedscriptTable4RpmBins();
	cannedscriptTable4();
	cannedignitionRpmBins();
	cannedignitionLoadBins();
	cannedignitionTable();
	cannedignBlends1_rpmBins();
	cannedignBlends1_loadBins();
	cannedignBlends2_rpmBins();
	cannedignBlends2_loadBins();
	cannedignBlends3_rpmBins();
	cannedignBlends3_loadBins();
	cannedignBlends4_rpmBins();
	cannedignBlends4_loadBins();
	cannedveBlends1_rpmBins();
	cannedveBlends1_loadBins();
	cannedveBlends2_rpmBins();
	cannedveBlends2_loadBins();
	cannedveBlends3_rpmBins();
	cannedveBlends3_loadBins();
	cannedveBlends4_rpmBins();
	cannedveBlends4_loadBins();
	cannedignitionIatCorrLoadBins();
	cannedignitionIatCorrTable();
	cannedveRpmBins();
	cannedveLoadBins();
	cannedveTable();
	cannedfuelTrimRpmBins();
	cannedfuelTrimLoadBins();
	cannedignTrimRpmBins();
	cannedignTrimLoadBins();
	cannedmapEstimateRpmBins();
	cannedmapEstimateTpsBins();
	cannedmapEstimateTable();
	cannedinjectionPhase();
	cannedpedalToTpsRpmBins();
	cannedpedalToTpsPedalBins();
	cannedpedalToTpsTable();
	cannedmaxKnockRetardRpmBins();
	cannedmaxKnockRetardLoadBins();
	cannedmaxKnockRetardTable();
	cannedlambdaLoadBins();
	cannedlambdaTable();
	cannediacPidMultRpmBins();
	cannediacPidMultLoadBins();
	cannediacPidMultTable();
	cannedgppwm1_loadBins();
	cannedgppwm1_table();
	cannedgppwm2_rpmBins();
	cannedgppwm2_table();
	cannedgppwm3_table();
	cannedtcuSolenoidTable();
	cannedhpfpTargetRpmBins();
	cannedhpfpTargetLoadBins();
	cannedhpfpCompensationRpmBins();
	cannedhpfpCompensationLoadBins();
	cannedpostCrankingFactor();
```
