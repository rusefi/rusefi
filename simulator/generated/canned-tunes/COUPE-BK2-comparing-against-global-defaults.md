# COUPE-BK2-comparing-against-global-defaults

// canned tune https://rusefi.com/online/view.php?msq=1507

```
    // default "Single Coil"
    engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
    // default 2.0
    engineConfiguration->displacement = 1.998;
    // default "false"
    engineConfiguration->isForcedInduction = true;
    // default 0.0
    engineConfiguration->globalTriggerAngleOffset = 475;
    // default 450.0
    engineConfiguration->vvtOffsets[0] = -154;
    // default 0.0
    engineConfiguration->vvtOffsets[1] = 335;
    // default 200.0
    engineConfiguration->injector.flow = 629.03;
    // default "None"
    engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;
    // default 300.0
    engineConfiguration->fuelReferencePressure = 586.0544;
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
    engineConfiguration->tpsMin = 98;
    // default 1000.0
    engineConfiguration->tpsMax = 926;
    // default 1000.0
    engineConfiguration->tps1SecondaryMin = 891;
    // default 0.0
    engineConfiguration->tps1SecondaryMax = 69;
    // default 0.0
    engineConfiguration->throttlePedalUpVoltage = 0.73;
    // default 5.0
    engineConfiguration->throttlePedalWOTVoltage = 4;
    // default 5.0
    engineConfiguration->throttlePedalSecondaryUpVoltage = 0.34;
    // default 0.0
    engineConfiguration->throttlePedalSecondaryWOTVoltage = 1.86;
    // default "Normal"
    engineConfiguration->clutchDownPinInverted = false;
    // default "Normal"
    engineConfiguration->clutchUpPinInverted = false;
    // default 300.0
    engineConfiguration->idle.solenoidFrequency = 200;
    // default "false"
    engineConfiguration->stepperDcInvertedPins = true;
    // default "false"
    engineConfiguration->useSeparateAdvanceForIdle = true;
    // default 0.0
    engineConfiguration->iacByTpsHoldTime = 2;
    // default 0.0
    engineConfiguration->iacByTpsDecayTime = 3;
    // default "false"
    engineConfiguration->useIdleTimingPidControl = true;
    // default 0.0
    engineConfiguration->idleTimingPid.dFactor = 5.0E-5;
    // default 92.0
    engineConfiguration->fanOnTemperature = 80;
    // default 88.0
    engineConfiguration->fanOffTemperature = 75;
    // default "false"
    engineConfiguration->disableFan1WhenStopped = true;
    // default 95.0
    engineConfiguration->fan2OnTemperature = 87;
    // default 91.0
    engineConfiguration->fan2OffTemperature = 82;
    // default "false"
    engineConfiguration->disableFan2WhenStopped = true;
    // default "false"
    engineConfiguration->enableSoftwareKnock = true;
    // default 20.0
    engineConfiguration->knockRetardAggression = 0;
    // default 3.0
    engineConfiguration->knockRetardReapplyRate = 0;
    // default 550.0
    engineConfiguration->vvtControlMinRpm = 1500;
    // default "advance"
    engineConfiguration->invertVvtControlExhaust = retard;
    // default 33.0
    engineConfiguration->auxPid[0].offset = 38;
    // default 0.005
    engineConfiguration->auxPid[0].iFactor = 25.4;
    // default 0.0
    engineConfiguration->auxPid[0].dFactor = 0.1;
    // default 0.0
    engineConfiguration->auxPid[1].offset = 38;
    // default 0.0
    engineConfiguration->auxPid[1].pFactor = 2;
    // default 0.0
    engineConfiguration->auxPid[1].iFactor = 25.4;
    // default 0.0
    engineConfiguration->auxPid[1].dFactor = 0.2;
    // default "Executor"
    engineConfiguration->debugMode = DBG_22;
    // default 200.0
    engineConfiguration->rpmSoftLimitWindowSize = 0;
    // default 4.0
    engineConfiguration->rpmSoftLimitTimingRetard = 0;
    // default 250.0
    engineConfiguration->etbRevLimitRange = 0;
    // default 50.0
    engineConfiguration->crankingIACposition = 70;
    // default 200.0
    engineConfiguration->afterCrankingIACtaperDuration = 100;
    // default "false"
    engineConfiguration->overrideCrankingIacSetting = true;
    // default 0.0
    engineConfiguration->tpsAccelLookback = 0.3;
    // default 40.0
    engineConfiguration->tpsAccelEnrichmentThreshold = 12;
    // default 0.0
    engineConfiguration->tpsDecelEnleanmentThreshold = 7;
    // default 0.0
    engineConfiguration->tpsAccelFractionPeriod = 3;
    // default 1.0
    engineConfiguration->tpsAccelFractionDivisor = 3;
    // default 2000.0
    engineConfiguration->boostControlMinRpm = 0;
    // default 30.0
    engineConfiguration->boostControlMinTps = 0;
    // default 110.0
    engineConfiguration->boostControlMinMap = 0;
    // default "Throttle 2"
    engineConfiguration->etbFunctions[1] = DC_Wastegate;
    // default 1.0
    engineConfiguration->etb.pFactor = 8.8944;
    // default 10.0
    engineConfiguration->etb.iFactor = 70.2307;
    // default 0.05
    engineConfiguration->etb.dFactor = 0.1855;
    // default 1.0
    engineConfiguration->etbJamTimeout = 0;


	coupleBK2cannedtpsTspCorrValuesBins();
	coupleBK2cannedtpsTspCorrValues();
	coupleBK2cannedprimeBins();
	coupleBK2cannedprimeValues();
	coupleBK2cannedmap_samplingAngleBins();
	coupleBK2cannedmap_samplingWindowBins();
	coupleBK2cannedvrThreshold1_rpmBins();
	coupleBK2cannedvrThreshold2_rpmBins();
	coupleBK2cannedcltRevLimitRpmBins();
	coupleBK2cannedcltIdleCorrBins();
	coupleBK2cannedcltIdleCorr();
	coupleBK2cannedcltCrankingCorr();
	coupleBK2cannedcltIdleRpm();
	coupleBK2cannedidleAdvanceBins();
	coupleBK2cannedidleAdvance();
	coupleBK2cannedignBlends1_blendBins();
	coupleBK2cannedignBlends1_blendValues();
	coupleBK2cannedignBlends2_blendBins();
	coupleBK2cannedignBlends2_blendValues();
	coupleBK2cannedignBlends3_blendBins();
	coupleBK2cannedignBlends3_blendValues();
	coupleBK2cannedignBlends4_blendBins();
	coupleBK2cannedignBlends4_blendValues();
	coupleBK2cannedveBlends1_blendBins();
	coupleBK2cannedveBlends1_blendValues();
	coupleBK2cannedveBlends2_blendBins();
	coupleBK2cannedveBlends2_blendValues();
	coupleBK2cannedveBlends3_blendBins();
	coupleBK2cannedveBlends3_blendValues();
	coupleBK2cannedveBlends4_blendBins();
	coupleBK2cannedveBlends4_blendValues();
	coupleBK2cannedtpsTpsAccelTable();
	coupleBK2cannedboostRpmBins();
	coupleBK2cannedboostTableOpenLoop();
	coupleBK2cannedscriptTable4RpmBins();
	coupleBK2cannedscriptTable4();
	coupleBK2cannedignitionRpmBins();
	coupleBK2cannedignitionLoadBins();
	coupleBK2cannedignitionTable();
	coupleBK2cannedignBlends1_rpmBins();
	coupleBK2cannedignBlends1_loadBins();
	coupleBK2cannedignBlends2_rpmBins();
	coupleBK2cannedignBlends2_loadBins();
	coupleBK2cannedignBlends3_rpmBins();
	coupleBK2cannedignBlends3_loadBins();
	coupleBK2cannedignBlends4_rpmBins();
	coupleBK2cannedignBlends4_loadBins();
	coupleBK2cannedveBlends1_rpmBins();
	coupleBK2cannedveBlends1_loadBins();
	coupleBK2cannedveBlends2_rpmBins();
	coupleBK2cannedveBlends2_loadBins();
	coupleBK2cannedveBlends3_rpmBins();
	coupleBK2cannedveBlends3_loadBins();
	coupleBK2cannedveBlends4_rpmBins();
	coupleBK2cannedveBlends4_loadBins();
	coupleBK2cannedveRpmBins();
	coupleBK2cannedveLoadBins();
	coupleBK2cannedveTable();
	coupleBK2cannedfuelTrimRpmBins();
	coupleBK2cannedfuelTrimLoadBins();
	coupleBK2cannedignTrimRpmBins();
	coupleBK2cannedignTrimLoadBins();
	coupleBK2cannedpedalToTpsRpmBins();
	coupleBK2cannedpedalToTpsTable();
	coupleBK2cannedlambdaTable();
	coupleBK2cannedtcuSolenoidTable();
	coupleBK2cannedhpfpTargetRpmBins();
	coupleBK2cannedhpfpCompensationRpmBins();
	coupleBK2cannedpostCrankingFactor();
```
