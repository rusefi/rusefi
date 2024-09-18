# PB-comparing-against-global-defaults

// canned tune https://rusefi.com/online/view.php?msq=1626

```
    // default "End of injection"
    engineConfiguration->injectionTimingMode = Center;
    // default "Single Coil"
    engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
    // default 2.0
    engineConfiguration->displacement = 1.6;
    // default 0.0
    engineConfiguration->globalTriggerAngleOffset = 115;
    // default "Inactive"
    engineConfiguration->vvtMode[1] = VVT_SINGLE_TOOTH;
    // default 450.0
    engineConfiguration->vvtOffsets[0] = 203;
    // default 0.0
    engineConfiguration->vvtOffsets[1] = 20;
    // default "false"
    engineConfiguration->isPhaseSyncRequiredForIgnition = true;
    // default 200.0
    engineConfiguration->injector.flow = 1000;
    // default "Low"
    engineConfiguration->injectorPressureType = IPT_High;
    // default "None"
    engineConfiguration->injectorCompensationMode = ICM_SensedRailPressure;
    // default 300.0
    engineConfiguration->fuelReferencePressure = 10000;
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
    // default 4.0
    engineConfiguration->benchTestOnTime = 1.5;
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
    engineConfiguration->throttlePedalSecondaryUpVoltage = 0.74;
    // default 0.0
    engineConfiguration->throttlePedalSecondaryWOTVoltage = 2.28;
    // default "Normal"
    engineConfiguration->clutchDownPinInverted = false;
    // default "Normal"
    engineConfiguration->clutchUpPinInverted = false;
    // default "Normal"
    engineConfiguration->flexSensorInverted = false;
    // default 0.0
    engineConfiguration->highPressureFuel.v1 = 0.5;
    // default 0.0
    engineConfiguration->highPressureFuel.v2 = 4.5;
    // default 0.0
    engineConfiguration->highPressureFuel.value2 = 20000;
    // default 0.0
    engineConfiguration->mapLowValueVoltage = 0.79;
    // default 5.0
    engineConfiguration->mapHighValueVoltage = 4;
    // default "false"
    engineConfiguration->enableAemXSeries = true;
    // default 15.0
    engineConfiguration->etbIdleThrottleRange = 6;
    // default 0.05
    engineConfiguration->idleRpmPid.pFactor = 0.03;
    // default 0.0
    engineConfiguration->idle_derivativeFilterLoss = 0.1;
    // default 0.0
    engineConfiguration->idle_antiwindupFreq = 0.1;
    // default 0.0
    engineConfiguration->idleRpmPid.dFactor = 5.0E-4;
    // default 50.0
    engineConfiguration->idlePidRpmDeadZone = 40;
    // default "false"
    engineConfiguration->useSeparateAdvanceForIdle = true;
    // default "Open Loop"
    engineConfiguration->idleMode = IM_AUTO;
    // default "false"
    engineConfiguration->useIdleTimingPidControl = true;
    // default 0.0
    engineConfiguration->idleTimingPid.dFactor = 5.0E-4;
    // default 0.0
    engineConfiguration->idleTimingSoftEntryTime = 1;
    // default 88.0
    engineConfiguration->fanOffTemperature = 86;
    // default "false"
    engineConfiguration->enableFan1WithAc = true;
    // default "false"
    engineConfiguration->disableFan1WhenStopped = true;
    // default 91.0
    engineConfiguration->fan2OffTemperature = 90;
    // default "false"
    engineConfiguration->enableFan2WithAc = true;
    // default "false"
    engineConfiguration->disableFan2WhenStopped = true;
    // default 4.0
    engineConfiguration->startUpFuelPumpDuration = 2;
    // default "false"
    engineConfiguration->enableSoftwareKnock = true;
    // default 87.5
    engineConfiguration->cylinderBore = 77;
    // default "false"
    engineConfiguration->fuelClosedLoopCorrectionEnabled = true;
    // default 60.0
    engineConfiguration->stft.startupDelay = 30;
    // default 60.0
    engineConfiguration->stft.minClt = 40;
    // default 12.0
    engineConfiguration->stft.minAfr = 10;
    // default "advance"
    engineConfiguration->invertVvtControlExhaust = retard;
    // default 0.0
    engineConfiguration->auxPid[0].dFactor = 5.0E-4;
    // default 0.0
    engineConfiguration->auxPid[1].offset = 33;
    // default 0.0
    engineConfiguration->auxPid[1].pFactor = 1;
    // default 0.0
    engineConfiguration->auxPid[1].iFactor = 0.005;
    // default 0.0
    engineConfiguration->auxPid[1].dFactor = 5.0E-4;
    // default 0.0
    engineConfiguration->totalGearsCount = 6;
    // default 0.0
    engineConfiguration->gearRatio[0] = 8.1;
    // default 0.0
    engineConfiguration->gearRatio[1] = 4.3;
    // default 0.0
    engineConfiguration->gearRatio[2] = 2.9;
    // default 0.0
    engineConfiguration->gearRatio[3] = 2.3;
    // default 0.0
    engineConfiguration->gearRatio[4] = 1.88;
    // default 0.0
    engineConfiguration->gearRatio[5] = 1.57;
    // default "Executor"
    engineConfiguration->debugMode = DBG_27;
    // default "Simultaneous"
    engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;
    // default 200.0
    engineConfiguration->afterCrankingIACtaperDuration = 100;
    // default 0.3
    engineConfiguration->wwaeTau = 0;
    // default 0.3
    engineConfiguration->wwaeBeta = 0;
    // default 1.0
    engineConfiguration->etb.pFactor = 8.8944;
    // default 10.0
    engineConfiguration->etb.iFactor = 70.2307;
    // default 0.05
    engineConfiguration->etb.dFactor = 0.1855;
    // default "false"
    engineConfiguration->disableEtbWhenEngineStopped = true;
    // default 1.0
    engineConfiguration->etbJamTimeout = 0;
    // default "Launch Button"
    engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;
    // default "false"
    engineConfiguration->rethrowHardFault = true;
    // default 3.0
    engineConfiguration->hpfpCamLobes = 4;
    // default "NONE"
    engineConfiguration->hpfpCam = HPFP_CAM_EX1;
    // default 10.0
    engineConfiguration->hpfpPeakPos = -30;
    // default 5.0
    engineConfiguration->mc33_hpfp_i_peak = 10;
    // default 3.0
    engineConfiguration->mc33_hpfp_i_hold = 4;


	pbcannedknockBaseNoise();
	pbcannedinjector_battLagCorr();
	pbcannedcltTimingBins();
	pbcannedcltTimingExtra();
	pbcannedcltIdleCorrBins();
	pbcannedcltIdleCorr();
	pbcannedcltIdleRpm();
	pbcannedidleAdvanceBins();
	pbcannedidleAdvance();
	pbcannedtractionControlSlipBins();
	pbcannedvvtTable1();
	pbcannedvvtTable2();
	pbcannedscriptTable4();
	pbcannedignitionRpmBins();
	pbcannedignitionTable();
	pbcannedveTable();
	pbcannedinjectionPhase();
	pbcannedmaxKnockRetardRpmBins();
	pbcannedmaxKnockRetardLoadBins();
	pbcannedmaxKnockRetardTable();
	pbcannedlambdaTable();
	pbcannedtcuSolenoidTable();
	pbcannedhpfpTargetLoadBins();
```
