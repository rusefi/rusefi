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
    // default 95.0
    engineConfiguration->fanOnTemperature = 80;
    // default 91.0
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
    // default "MAP"
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


	coupleBK2cannedtpsTpsAccelTable();
	coupleBK2cannedboostTableOpenLoop();
	coupleBK2cannedscriptTable4();
	coupleBK2cannedignitionTable();
	coupleBK2cannedveTable();
	coupleBK2cannedpedalToTpsTable();
	coupleBK2cannedlambdaTable();
	coupleBK2cannedtcuSolenoidTable();
	coupleBK2cannedpostCrankingFactor();
```
