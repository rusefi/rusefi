# PB-comparing-against-global-defaults

// canned tune https://rusefi.com/online/view.php?msq=1620

```
    // default "End of injection"
    engineConfiguration->injectionTimingMode = Center;
    // default "Single Coil"
    engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
    // default 2.0
    engineConfiguration->displacement = 1.6;
    // default 0.0
    engineConfiguration->globalTriggerAngleOffset = 90;
    // default "Inactive"
    engineConfiguration->vvtMode[1] = VVT_SINGLE_TOOTH;
    // default 450.0
    engineConfiguration->vvtOffsets[0] = 227;
    // default 0.0
    engineConfiguration->vvtOffsets[1] = 50;
    // default "false"
    engineConfiguration->isPhaseSyncRequiredForIgnition = true;
    // default 200.0
    engineConfiguration->injector.flow = 1214;
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
    // default "false"
    engineConfiguration->enableSoftwareKnock = true;
    // default 550.0
    engineConfiguration->vvtControlMinRpm = 1500;
    // default 0.0
    engineConfiguration->totalGearsCount = 5;
    // default 0.0
    engineConfiguration->gearRatio[0] = 12;
    // default 0.0
    engineConfiguration->gearRatio[1] = 6;
    // default 0.0
    engineConfiguration->gearRatio[2] = 4.4;
    // default 0.0
    engineConfiguration->gearRatio[3] = 2.4;
    // default 0.0
    engineConfiguration->gearRatio[4] = 1;
    // default 200.0
    engineConfiguration->rpmSoftLimitWindowSize = 0;
    // default 4.0
    engineConfiguration->rpmSoftLimitTimingRetard = 0;
    // default "Simultaneous"
    engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;
    // default 1.0
    engineConfiguration->tpsAccelFractionDivisor = 0;
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
    // default "Launch Button"
    engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;
    // default "false"
    engineConfiguration->rethrowHardFault = true;
    // default 3.0
    engineConfiguration->hpfpCamLobes = 4;
    // default 5.0
    engineConfiguration->mc33_hpfp_i_peak = 10;
    // default 3.0
    engineConfiguration->mc33_hpfp_i_hold = 4;


	pbcannedboostTableOpenLoop();
	pbcannedvvtTable1();
	pbcannedvvtTable2();
	pbcannedscriptTable4();
	pbcannedignitionTable();
	pbcannedveTable();
	pbcannedinjectionPhase();
	pbcannedlambdaTable();
	pbcannedtcuSolenoidTable();
```
