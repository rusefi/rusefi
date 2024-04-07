# PB-comparing-against-current-PB-default

// canned tune https://rusefi.com/online/view.php?msq=1626

```
    // default 227.0
    engineConfiguration->vvtOffsets[0] = 203;
    // default 50.0
    engineConfiguration->vvtOffsets[1] = 20;
    // default 1214.0
    engineConfiguration->injector.flow = 1000;
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
    // default 0.34
    engineConfiguration->throttlePedalSecondaryUpVoltage = 0.74;
    // default 1.86
    engineConfiguration->throttlePedalSecondaryWOTVoltage = 2.28;
    // default "Normal"
    engineConfiguration->clutchDownPinInverted = false;
    // default "Normal"
    engineConfiguration->clutchUpPinInverted = false;
    // default "Normal"
    engineConfiguration->flexSensorInverted = false;
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
    // default 0.0
    engineConfiguration->knockBandCustom = 14.8;
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
    // default 1500.0
    engineConfiguration->vvtControlMinRpm = 550;
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
    // default 5.0
    engineConfiguration->totalGearsCount = 6;
    // default 12.0
    engineConfiguration->gearRatio[0] = 8.1;
    // default 6.0
    engineConfiguration->gearRatio[1] = 4.3;
    // default 4.4
    engineConfiguration->gearRatio[2] = 2.9;
    // default 2.4
    engineConfiguration->gearRatio[3] = 2.3;
    // default 1.0
    engineConfiguration->gearRatio[4] = 1.88;
    // default 0.0
    engineConfiguration->gearRatio[5] = 1.57;
    // default 200.0
    engineConfiguration->afterCrankingIACtaperDuration = 100;
    // default "Launch Button"
    engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;
    // default "NONE"
    engineConfiguration->hpfpCam = HPFP_CAM_EX1;
    // default 10.0
    engineConfiguration->hpfpPeakPos = -30;


	pbcannedvvtTable2();
	pbcannedscriptTable4();
	pbcannedignitionTable();
	pbcannedveTable();
	pbcannedmaxKnockRetardTable();
	pbcannedlambdaTable();
	pbcannedtcuSolenoidTable();
```
