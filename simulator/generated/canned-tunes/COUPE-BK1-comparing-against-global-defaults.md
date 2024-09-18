# COUPE-BK1-comparing-against-global-defaults

// canned tune https://rusefi.com/online/view.php?msq=1508

```
    // default "Single Coil"
    engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
    // default 2.0
    engineConfiguration->displacement = 1.998;
    // default 0.0
    engineConfiguration->globalTriggerAngleOffset = 475;
    // default 450.0
    engineConfiguration->vvtOffsets[0] = -155;
    // default 0.0
    engineConfiguration->vvtOffsets[1] = 337;
    // default 200.0
    engineConfiguration->injector.flow = 440;
    // default "None"
    engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;
    // default 300.0
    engineConfiguration->fuelReferencePressure = 379.2116;
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
    engineConfiguration->tpsMin = 111;
    // default 1000.0
    engineConfiguration->tpsMax = 923;
    // default 1000.0
    engineConfiguration->tps1SecondaryMin = 874;
    // default 0.0
    engineConfiguration->tps1SecondaryMax = 63;
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
    // default 15.0
    engineConfiguration->etbIdleThrottleRange = 5;
    // default 15.0
    engineConfiguration->acIdleExtraOffset = 10;
    // default 2.0
    engineConfiguration->fan1ExtraIdle = 0;
    // default 2.0
    engineConfiguration->iacByTpsTaper = 0;
    // default "false"
    engineConfiguration->useSeparateAdvanceForIdle = true;
    // default "false"
    engineConfiguration->useIdleTimingPidControl = true;
    // default 0.0
    engineConfiguration->idleTimingPid.dFactor = 5.0E-4;
    // default 92.0
    engineConfiguration->fanOnTemperature = 86;
    // default 88.0
    engineConfiguration->fanOffTemperature = 80;
    // default "false"
    engineConfiguration->enableFan1WithAc = true;
    // default "false"
    engineConfiguration->disableFan1WhenStopped = true;
    // default 95.0
    engineConfiguration->fan2OnTemperature = 93;
    // default 91.0
    engineConfiguration->fan2OffTemperature = 87;
    // default "false"
    engineConfiguration->enableFan2WithAc = true;
    // default "false"
    engineConfiguration->disableFan2WhenStopped = true;
    // default 0.5
    engineConfiguration->acDelay = 0;
    // default "false"
    engineConfiguration->enableSoftwareKnock = true;
    // default 20.0
    engineConfiguration->knockRetardAggression = 0;
    // default 3.0
    engineConfiguration->knockRetardReapplyRate = 0;
    // default 6000.0
    engineConfiguration->vvtActivationDelayMs = 3000;
    // default 550.0
    engineConfiguration->vvtControlMinRpm = 1500;
    // default "advance"
    engineConfiguration->invertVvtControlExhaust = retard;
    // default 33.0
    engineConfiguration->auxPid[0].offset = 38;
    // default 0.005
    engineConfiguration->auxPid[0].iFactor = 25.4;
    // default 0.0
    engineConfiguration->auxPid[0].dFactor = 0.2;
    // default 0.0
    engineConfiguration->auxPid[1].offset = 38;
    // default 0.0
    engineConfiguration->auxPid[1].pFactor = 2;
    // default 0.0
    engineConfiguration->auxPid[1].iFactor = 25.4;
    // default 0.0
    engineConfiguration->auxPid[1].dFactor = 0.2;
    // default "None"
    engineConfiguration->canNbcType = CAN_BUS_GENESIS_COUPE;
    // default "Executor"
    engineConfiguration->debugMode = DBG_22;
    // default 200.0
    engineConfiguration->rpmSoftLimitWindowSize = 0;
    // default 4.0
    engineConfiguration->rpmSoftLimitTimingRetard = 0;
    // default 250.0
    engineConfiguration->etbRevLimitRange = 0;
    // default 0.5
    engineConfiguration->primingDelay = 0;
    // default 1.0
    engineConfiguration->tpsAccelFractionDivisor = 0;
    // default "false"
    engineConfiguration->isBoostControlEnabled = true;
    // default 2000.0
    engineConfiguration->boostControlMinRpm = 0;
    // default 30.0
    engineConfiguration->boostControlMinTps = 0;
    // default 110.0
    engineConfiguration->boostControlMinMap = 0;
    // default "Throttle 2"
    engineConfiguration->etbFunctions[1] = DC_Wastegate;
    // default 1.0
    engineConfiguration->etb.pFactor = 7.213493;
    // default 10.0
    engineConfiguration->etb.iFactor = 94.99006;
    // default 0.05
    engineConfiguration->etb.dFactor = 0.0892407;
    // default 1.0
    engineConfiguration->etbJamTimeout = 0;


	coupleBK1cannedprimeBins();
	coupleBK1cannedprimeValues();
	coupleBK1cannedmap_samplingAngleBins();
	coupleBK1cannedmap_samplingWindowBins();
	coupleBK1cannedvrThreshold1_rpmBins();
	coupleBK1cannedvrThreshold2_rpmBins();
	coupleBK1cannedcltRevLimitRpmBins();
	coupleBK1cannedcltIdleCorrBins();
	coupleBK1cannedcltIdleCorr();
	coupleBK1cannedcltIdleRpm();
	coupleBK1cannedidleAdvanceBins();
	coupleBK1cannedidleAdvance();
	coupleBK1cannedfuelLevelBins();
	coupleBK1cannedignBlends1_blendBins();
	coupleBK1cannedignBlends1_blendValues();
	coupleBK1cannedignBlends2_blendBins();
	coupleBK1cannedignBlends2_blendValues();
	coupleBK1cannedignBlends3_blendBins();
	coupleBK1cannedignBlends3_blendValues();
	coupleBK1cannedignBlends4_blendBins();
	coupleBK1cannedignBlends4_blendValues();
	coupleBK1cannedveBlends1_blendBins();
	coupleBK1cannedveBlends1_blendValues();
	coupleBK1cannedveBlends2_blendBins();
	coupleBK1cannedveBlends2_blendValues();
	coupleBK1cannedveBlends3_blendBins();
	coupleBK1cannedveBlends3_blendValues();
	coupleBK1cannedveBlends4_blendBins();
	coupleBK1cannedveBlends4_blendValues();
	coupleBK1cannedboostRpmBins();
	coupleBK1cannedboostTableOpenLoop();
	coupleBK1cannedvvtTable1();
	coupleBK1cannedvvtTable2();
	coupleBK1cannedscriptTable4RpmBins();
	coupleBK1cannedscriptTable4();
	coupleBK1cannedignitionRpmBins();
	coupleBK1cannedignitionLoadBins();
	coupleBK1cannedignitionTable();
	coupleBK1cannedignBlends1_rpmBins();
	coupleBK1cannedignBlends1_loadBins();
	coupleBK1cannedignBlends2_rpmBins();
	coupleBK1cannedignBlends2_loadBins();
	coupleBK1cannedignBlends3_rpmBins();
	coupleBK1cannedignBlends3_loadBins();
	coupleBK1cannedignBlends4_rpmBins();
	coupleBK1cannedignBlends4_loadBins();
	coupleBK1cannedveBlends1_rpmBins();
	coupleBK1cannedveBlends1_loadBins();
	coupleBK1cannedveBlends2_rpmBins();
	coupleBK1cannedveBlends2_loadBins();
	coupleBK1cannedveBlends3_rpmBins();
	coupleBK1cannedveBlends3_loadBins();
	coupleBK1cannedveBlends4_rpmBins();
	coupleBK1cannedveBlends4_loadBins();
	coupleBK1cannedveTable();
	coupleBK1cannedfuelTrimRpmBins();
	coupleBK1cannedfuelTrimLoadBins();
	coupleBK1cannedignTrimRpmBins();
	coupleBK1cannedignTrimLoadBins();
	coupleBK1cannedpedalToTpsRpmBins();
	coupleBK1cannedlambdaTable();
	coupleBK1cannedtcuSolenoidTable();
	coupleBK1cannedhpfpTargetRpmBins();
	coupleBK1cannedhpfpCompensationRpmBins();
	coupleBK1cannedpostCrankingFactor();
```
