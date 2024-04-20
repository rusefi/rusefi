# VQ-comparing-against-current-VQ-default

// canned tune https://rusefi.com/online/view.php?msq=1641

```
    // default "Single Coil"
    engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
    // default "false"
    engineConfiguration->twoWireBatchIgnition = true;
    // default "None"
    engineConfiguration->ignOverrideMode = AFR_Tps;
    // default 4.0
    engineConfiguration->displacement = 3.5;
    // default 40.0
    engineConfiguration->camDecoder2jzPrecision = 25;
    // default -203.0
    engineConfiguration->vvtOffsets[2] = -207;
    // default 200.0
    engineConfiguration->injector.flow = 320;
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
    // default "Zero"
    engineConfiguration->ignBlends[0].blendParameter = GPPWM_Clt;
    // default "Zero"
    engineConfiguration->ignBlends[1].blendParameter = GPPWM_AccelPedal;
    // default 0.0
    engineConfiguration->tpsMin = 97;
    // default 1000.0
    engineConfiguration->tpsMax = 852;
    // default 1000.0
    engineConfiguration->tps1SecondaryMin = 900;
    // default 0.0
    engineConfiguration->tps1SecondaryMax = 139;
    // default 1000.0
    engineConfiguration->tps2SecondaryMin = 0;
    // default 0.0
    engineConfiguration->tps2SecondaryMax = 1000;
    // default 0.0
    engineConfiguration->throttlePedalUpVoltage = 0.840429;
    // default 5.0
    engineConfiguration->throttlePedalWOTVoltage = 4.536769;
    // default 5.0
    engineConfiguration->throttlePedalSecondaryUpVoltage = 0.395229;
    // default 0.0
    engineConfiguration->throttlePedalSecondaryWOTVoltage = 2.120979;
    // default 5.0
    engineConfiguration->etbSplit = 10;
    // default 400.0
    engineConfiguration->mc33_t_max_boost = 470;
    // default 10.0
    engineConfiguration->mc33_t_bypass = 15;
    // default "Normal"
    engineConfiguration->clutchDownPinInverted = false;
    // default "Normal"
    engineConfiguration->clutchUpPinInverted = false;
    // default 5.0
    engineConfiguration->mapErrorDetectionTooLow = 0;
    // default "false"
    engineConfiguration->enableAemXSeries = true;
    // default 300.0
    engineConfiguration->idle.solenoidFrequency = 200;
    // default "false"
    engineConfiguration->etb_use_two_wires = true;
    // default 3.0
    engineConfiguration->idleStepperReactionTime = 0;
    // default 200.0
    engineConfiguration->idleStepperTotalSteps = 0;
    // default "true"
    engineConfiguration->stepperForceParkingEveryRestart = false;
    // default 15.0
    engineConfiguration->etbIdleThrottleRange = 10;
    // default -20.0
    engineConfiguration->idleRpmPid.minValue = 0;
    // default 20.0
    engineConfiguration->idleRpmPid.maxValue = 99;
    // default 15.0
    engineConfiguration->acIdleExtraOffset = 10;
    // default 2.0
    engineConfiguration->fan1ExtraIdle = 0;
    // default 2.0
    engineConfiguration->fan2ExtraIdle = 1;
    // default 2.0
    engineConfiguration->iacByTpsTaper = 3;
    // default 5.0
    engineConfiguration->idlePidDeactivationTpsThreshold = 2;
    // default "false"
    engineConfiguration->useSeparateAdvanceForIdle = true;
    // default 0.0
    engineConfiguration->iacByTpsHoldTime = 2;
    // default 0.0
    engineConfiguration->iacByTpsDecayTime = 2;
    // default "false"
    engineConfiguration->useIdleTimingPidControl = true;
    // default 0.0
    engineConfiguration->idleTimingPid.iFactor = 0.005;
    // default 0.0
    engineConfiguration->idleTimingPid.dFactor = 5.0E-4;
    // default 0.0
    engineConfiguration->idleTimingSoftEntryTime = 2;
    // default 92.0
    engineConfiguration->fanOnTemperature = 85;
    // default 88.0
    engineConfiguration->fanOffTemperature = 81;
    // default 95.0
    engineConfiguration->fan2OnTemperature = 92;
    // default 91.0
    engineConfiguration->fan2OffTemperature = 87;
    // default 0.5
    engineConfiguration->acDelay = 0;
    // default 4.0
    engineConfiguration->startUpFuelPumpDuration = 2;
    // default "false"
    engineConfiguration->enableSoftwareKnock = true;
    // default 20.0
    engineConfiguration->knockRetardAggression = 0;
    // default 3.0
    engineConfiguration->knockRetardReapplyRate = 0;
    // default 500.0
    engineConfiguration->vvtControlMinRpm = 400;
    // default "advance"
    engineConfiguration->invertVvtControlExhaust = retard;
    // default 50.0
    engineConfiguration->auxPid[0].offset = 33;
    // default 0.5
    engineConfiguration->auxPid[0].iFactor = 0.02;
    // default 0.05
    engineConfiguration->auxPid[0].dFactor = 5.0E-4;
    // default 0.0
    engineConfiguration->auxPid[1].offset = 33;
    // default 0.0
    engineConfiguration->auxPid[1].pFactor = 2;
    // default 0.0
    engineConfiguration->auxPid[1].iFactor = 0.01;
    // default 0.0
    engineConfiguration->auxPid[1].dFactor = 5.0E-4;
    // default 3.0
    engineConfiguration->vssFilterReciprocal = 0;
    // default 1000.0
    engineConfiguration->driveWheelRevPerKm = 500;
    // default 1.0
    engineConfiguration->finalGearRatio = 0;
    // default "Speed Density"
    engineConfiguration->fuelAlgorithm = LM_ALPHA_N;
    // default "MAP"
    engineConfiguration->debugMode = DBG_22;
    // default "yes"
    engineConfiguration->cutFuelOnHardLimit = no;
    // default 7000.0
    engineConfiguration->rpmHardLimit = 7100;
    // default 300.0
    engineConfiguration->boostCutPressure = 200;
    // default 200.0
    engineConfiguration->rpmSoftLimitWindowSize = 0;
    // default 4.0
    engineConfiguration->rpmSoftLimitTimingRetard = 0;
    // default 250.0
    engineConfiguration->etbRevLimitRange = 0;
    // default "Simultaneous"
    engineConfiguration->crankingInjectionMode = IM_BATCH;
    // default 50.0
    engineConfiguration->crankingIACposition = 100;
    // default 200.0
    engineConfiguration->afterCrankingIACtaperDuration = 100;
    // default "false"
    engineConfiguration->overrideCrankingIacSetting = true;
    // default 0.5
    engineConfiguration->primingDelay = 1;
    // default 550.0
    engineConfiguration->cranking.rpm = 400;
    // default 40.0
    engineConfiguration->tpsAccelEnrichmentThreshold = 200;
    // default 1.0
    engineConfiguration->tpsAccelFractionDivisor = 0;
    // default 0.3
    engineConfiguration->wwaeTau = 0;
    // default 0.3
    engineConfiguration->wwaeBeta = 0;
    // default 2000.0
    engineConfiguration->boostControlMinRpm = 0;
    // default 30.0
    engineConfiguration->boostControlMinTps = 0;
    // default 110.0
    engineConfiguration->boostControlMinMap = 0;
    // default 1.0
    engineConfiguration->etb.pFactor = 5.979724;
    // default 10.0
    engineConfiguration->etb.iFactor = 82.88195;
    // default 0.05
    engineConfiguration->etb.dFactor = 0.0709292;
    // default "false"
    engineConfiguration->disableEtbWhenEngineStopped = true;
    // default 100.0
    engineConfiguration->etbMaximumPosition = 90;
    // default 3000.0
    engineConfiguration->launchRpm = 3800;
    // default "false"
    engineConfiguration->enableLaunchRetard = true;
    // default 0.0
    engineConfiguration->launchTimingRetard = -10;
    // default "false"
    engineConfiguration->launchSparkCutEnable = true;
    // default "Switch Input"
    engineConfiguration->antiLagActivationMode = ALWAYS_ON_ANTILAG;
    // default "false"
    engineConfiguration->coastingFuelCutEnabled = true;
    // default 0.0
    engineConfiguration->dfcoDelay = 2.5;
    // default 0.0
    engineConfiguration->noFuelTrimAfterDfcoTime = 0.5;
    // default "true"
    engineConfiguration->watchOutForLinearTime = false;
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


	cannedprimeBins();
	cannedprimeValues();
	cannedmap_samplingAngleBins();
	cannedmap_samplingAngle();
	cannedmap_samplingWindowBins();
	cannedmap_samplingWindow();
	cannedinjector_battLagCorrBins();
	cannedinjector_battLagCorr();
	cannedvrThreshold1_rpmBins();
	cannedvrThreshold2_rpmBins();
	cannedmafDecodingBins();
	cannedmafDecoding();
	cannedcltFuelCorr();
	cannedcrankingFuelCoef();
	cannedcltRevLimitRpmBins();
	cannedcrankingCycleCoef();
	cannedcltIdleCorr();
	cannedcltCrankingCorr();
	cannedcltIdleRpm();
	cannedidleAdvanceBins();
	cannedidleAdvance();
	cannedfuelLevelBins();
	cannedignBlends1_blendBins();
	cannedignBlends1_blendValues();
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
	cannedtpsTpsAccelTable();
	cannedboostRpmBins();
	cannedboostTableOpenLoop();
	cannedvvtTable1RpmBins();
	cannedvvtTable1();
	cannedscriptTable4RpmBins();
	cannedscriptTable4();
	cannedignitionRpmBins();
	cannedignitionLoadBins();
	cannedignitionTable();
	cannedignBlends1_rpmBins();
	cannedignBlends1_loadBins();
	cannedignBlends1_table();
	cannedignBlends2_rpmBins();
	cannedignBlends2_loadBins();
	cannedignBlends2_table();
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
	cannedlambdaTable();
	cannedtcuSolenoidTable();
	cannedhpfpTargetRpmBins();
	cannedhpfpCompensationRpmBins();
	cannedpostCrankingFactor();
```
