# MazdaMiataNB2-comparing-against-current-MazdaMiataNB2-default

// canned tune https://rusefi.com/online/view.php?msq=985

```
    // default 1.0
    engineConfiguration->tChargeAirIncrLimit = 15;
    // default 153.6
    engineConfiguration->tChargeAirFlowMax = 165;
    // default 0.0
    engineConfiguration->fixedTiming = 10;
    // default 1.839
    engineConfiguration->displacement = 1.838997;
    // default 0.0
    engineConfiguration->globalTriggerAngleOffset = 76;
    // default 98.0
    engineConfiguration->vvtOffsets[0] = 76;
    // default 0.0
    engineConfiguration->cylinderBankSelect[0] = 1;
    // default 0.0
    engineConfiguration->cylinderBankSelect[1] = 1;
    // default 0.0
    engineConfiguration->cylinderBankSelect[2] = 1;
    // default 0.0
    engineConfiguration->cylinderBankSelect[3] = 1;
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
    // default 0.0
    engineConfiguration->injectorCorrectionPolynomial[4] = 8.2652E-39;
    // default 4.0
    engineConfiguration->benchTestOnTime = 0;
    // default 110.0
    engineConfiguration->tpsMin = 107;
    // default 650.0
    engineConfiguration->tpsMax = 739;
    // default 1000.0
    engineConfiguration->tps1SecondaryMin = 0;
    // default 0.0
    engineConfiguration->tps1SecondaryMax = 1000;
    // default 1000.0
    engineConfiguration->tps2SecondaryMin = 0;
    // default 0.0
    engineConfiguration->tps2SecondaryMax = 1000;
    // default 5.0
    engineConfiguration->throttlePedalSecondaryUpVoltage = 0;
    // default 0.0
    engineConfiguration->throttlePedalSecondaryWOTVoltage = 5;
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
    engineConfiguration->clutchDownPinInverted = true;
    // default "Normal"
    engineConfiguration->clutchUpPinInverted = false;
    // default 1.0
    engineConfiguration->turboSpeedSensorMultiplier = 0;
    // default 410.0
    engineConfiguration->mapErrorDetectionTooHigh = 250;
    // default "14Point7"
    engineConfiguration->afr_type = ES_Custom;
    // default 0.0
    engineConfiguration->afr.v1 = 1;
    // default 9.996
    engineConfiguration->afr.value1 = 8.099976;
    // default 5.0
    engineConfiguration->afr.v2 = 4;
    // default 19.992
    engineConfiguration->afr.value2 = 17.8;
    // default 0.0
    engineConfiguration->egoValueShift = -0.200001;
    // default 300.0
    engineConfiguration->idle.solenoidFrequency = 500;
    // default 3.0
    engineConfiguration->idleStepperReactionTime = 0;
    // default 200.0
    engineConfiguration->idleStepperTotalSteps = 0;
    // default "true"
    engineConfiguration->stepperForceParkingEveryRestart = false;
    // default 15.0
    engineConfiguration->etbIdleThrottleRange = 5;
    // default 0.3
    engineConfiguration->idleRpmPid.iFactor = 0.03;
    // default 10.0
    engineConfiguration->idleRpmPid.maxValue = 76;
    // default 2.0
    engineConfiguration->fan1ExtraIdle = 0;
    // default 2.0
    engineConfiguration->fan2ExtraIdle = 0;
    // default 6.0
    engineConfiguration->iacByTpsTaper = 5;
    // default 5.0
    engineConfiguration->idlePidDeactivationTpsThreshold = 1;
    // default 100.0
    engineConfiguration->idlePidRpmDeadZone = 65;
    // default "false"
    engineConfiguration->useSeparateAdvanceForIdle = true;
    // default "false"
    engineConfiguration->useSeparateVeForIdle = true;
    // default 0.1
    engineConfiguration->idleTimingPid.pFactor = 0.01;
    // default 10.0
    engineConfiguration->idleTimingPid.maxValue = 12;
    // default 92.0
    engineConfiguration->fanOnTemperature = 97;
    // default 88.0
    engineConfiguration->fanOffTemperature = 94;
    // default 95.0
    engineConfiguration->fan2OnTemperature = 99;
    // default 91.0
    engineConfiguration->fan2OffTemperature = 94;
    // default "false"
    engineConfiguration->enableFan2WithAc = true;
    // default 5000.0
    engineConfiguration->maxAcRpm = 5200;
    // default 75.0
    engineConfiguration->maxAcTps = 70;
    // default "false"
    engineConfiguration->enableSoftwareKnock = true;
    // default 20.0
    engineConfiguration->knockRetardAggression = 0;
    // default 3.0
    engineConfiguration->knockRetardReapplyRate = 0;
    // default 35.0
    engineConfiguration->stft.maxOverrunLoad = 30;
    // default "false"
    engineConfiguration->fuelClosedLoopCorrectionEnabled = true;
    // default 60.0
    engineConfiguration->stft.startupDelay = 25;
    // default 60.0
    engineConfiguration->stft.minClt = 80;
    // default 12.0
    engineConfiguration->stft.minAfr = 10.5;
    // default 17.0
    engineConfiguration->stft.maxAfr = 18;
    // default 0.5
    engineConfiguration->stft.deadband = 0;
    // default 90.0
    engineConfiguration->auxPid[1].maxValue = 0;
    // default 14.0
    engineConfiguration->targetVBatt = 14.19995;
    // default 3.0
    engineConfiguration->vssFilterReciprocal = 5;
    // default 538.0
    engineConfiguration->driveWheelRevPerKm = 570;
    // default 1.117
    engineConfiguration->vssGearRatio = 4.1;
    // default 4.0
    engineConfiguration->vssToothCount = 1;
    // default "BMW_e46"
    engineConfiguration->canVssNbcType = W202;
    // default 3.91
    engineConfiguration->finalGearRatio = 4.1;
    // default 6.0
    engineConfiguration->totalGearsCount = 5;
    // default 3.76
    engineConfiguration->gearRatio[0] = 3.14;
    // default 2.27
    engineConfiguration->gearRatio[1] = 1.89;
    // default 1.65
    engineConfiguration->gearRatio[2] = 1.33;
    // default 1.26
    engineConfiguration->gearRatio[3] = 1;
    // default 1.0
    engineConfiguration->gearRatio[4] = 0.81;
    // default 0.84
    engineConfiguration->gearRatio[5] = 0;
    // default "500kbps"
    engineConfiguration->can2BaudRate = B100KBPS;
    // default "MAP"
    engineConfiguration->debugMode = DBG_TPS_ACCEL;
    // default 300.0
    engineConfiguration->boostCutPressure = 0;
    // default 250.0
    engineConfiguration->etbRevLimitRange = 0;
    // default 60.0
    engineConfiguration->crankingIACposition = 33;
    // default 250.0
    engineConfiguration->afterCrankingIACtaperDuration = 189;
    // default "false"
    engineConfiguration->overrideCrankingIacSetting = true;
    // default 0.5
    engineConfiguration->primingDelay = 0;
    // default "true"
    engineConfiguration->isFasterEngineSpinUpEnabled = false;
    // default 0.0
    engineConfiguration->tpsAccelLookback = 0.2;
    // default 40.0
    engineConfiguration->tpsAccelEnrichmentThreshold = 4;
    // default 1.0
    engineConfiguration->tpsAccelFractionDivisor = 0;
    // default 0.1
    engineConfiguration->wwaeTau = 0.15;
    // default 0.0
    engineConfiguration->scriptSetting[0] = 5000;
    // default 2.0
    engineConfiguration->issFilterReciprocal = 0;
    // default 1.0
    engineConfiguration->tcuInputSpeedSensorTeeth = 0;
    // default 100.0
    engineConfiguration->etbMaximumPosition = 0;
    // default "Launch Button"
    engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;
    // default 30.0
    engineConfiguration->launchSpeedThreshold = 1;
    // default 3000.0
    engineConfiguration->launchRpm = 3500;
    // default "false"
    engineConfiguration->enableLaunchRetard = true;
    // default 0.0
    engineConfiguration->launchTimingRetard = -40;
    // default "false"
    engineConfiguration->launchSmoothRetard = true;
    // default "false"
    engineConfiguration->launchSparkCutEnable = true;
    // default "Switch Input"
    engineConfiguration->antiLagActivationMode = ALWAYS_ON_ANTILAG;
    // default 1500.0
    engineConfiguration->coastingFuelCutRpmHigh = 2250;
    // default 1300.0
    engineConfiguration->coastingFuelCutRpmLow = 1400;
    // default 2.0
    engineConfiguration->coastingFuelCutTps = 1;
    // default "Zero"
    engineConfiguration->gppwm[0].loadAxis = GPPWM_Tps;
    // default "Zero"
    engineConfiguration->gppwm[1].loadAxis = GPPWM_Tps;
    // default "Zero"
    engineConfiguration->gppwm[2].loadAxis = GPPWM_Tps;
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


	nb2cannedknockNoiseRpmBins();
	nb2cannedknockBaseNoise();
	nb2cannedscriptCurve5Bins();
	nb2cannedscriptCurve6Bins();
	nb2cannedsparkDwellRpmBins();
	nb2cannedsparkDwellValues();
	nb2canneddwellVoltageCorrVoltBins();
	nb2canneddwellVoltageCorrValues();
	nb2cannedprimeBins();
	nb2cannedprimeValues();
	nb2cannedmap_samplingAngleBins();
	nb2cannedmap_samplingWindowBins();
	nb2cannedinjector_battLagCorrBins();
	nb2cannedinjector_battLagCorr();
	nb2cannedvrThreshold1_rpmBins();
	nb2cannedvrThreshold1_values();
	nb2cannedvrThreshold2_rpmBins();
	nb2cannedvrThreshold2_values();
	nb2cannedmafDecodingBins();
	nb2cannedcltFuelCorr();
	nb2cannedcrankingFuelCoef();
	nb2cannedcrankingFuelCoefE100();
	nb2cannedcltRevLimitRpmBins();
	nb2cannedcrankingCycleCoef();
	nb2cannedcltIdleCorrBins();
	nb2cannedcltIdleCorr();
	nb2cannedcltCrankingCorr();
	nb2cannedcltIdleRpmBins();
	nb2cannedcltIdleRpm();
	nb2cannedidleAdvanceBins();
	nb2cannedidleAdvance();
	nb2cannedfuelLevelBins();
	nb2cannedhpfpLobeProfileQuantityBins();
	nb2cannedhpfpLobeProfileAngle();
	nb2cannedhpfpDeadtimeVoltsBins();
	nb2cannedtpsTpsAccelTable();
	nb2cannedboostRpmBins();
	nb2cannedboostTpsBins();
	nb2cannedboostTableOpenLoop();
	nb2cannedboostTableClosedLoop();
	nb2cannedvvtTable1RpmBins();
	nb2cannedvvtTable1();
	nb2cannedscriptTable4RpmBins();
	nb2cannedscriptTable4();
	nb2cannedignitionRpmBins();
	nb2cannedignitionLoadBins();
	nb2cannedignitionTable();
	nb2cannedignitionIatCorrLoadBins();
	nb2cannedignitionIatCorrTable();
	nb2cannedveRpmBins();
	nb2cannedveLoadBins();
	nb2cannedveTable();
	nb2cannedidleVeRpmBins();
	nb2cannedidleVeLoadBins();
	nb2cannedidleVeTable();
	nb2cannedfuelTrimRpmBins();
	nb2cannedfuelTrimLoadBins();
	nb2cannedignTrimRpmBins();
	nb2cannedignTrimLoadBins();
	nb2cannedmapEstimateRpmBins();
	nb2cannedmapEstimateTpsBins();
	nb2cannedmapEstimateTable();
	nb2cannedinjectionPhase();
	nb2cannedpedalToTpsRpmBins();
	nb2cannedlambdaRpmBins();
	nb2cannedlambdaLoadBins();
	nb2cannedlambdaTable();
	nb2cannedtcuSolenoidTable();
	nb2cannedhpfpTargetRpmBins();
	nb2cannedhpfpTargetLoadBins();
	nb2cannedhpfpCompensationRpmBins();
	nb2cannedhpfpCompensationLoadBins();
	nb2cannedpostCrankingFactor();
```
