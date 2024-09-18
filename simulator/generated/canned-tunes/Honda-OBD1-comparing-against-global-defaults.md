# Honda-OBD1-comparing-against-global-defaults

// canned tune https://rusefi.com/online/view.php?msq=1621

```
    // default 2.0
    engineConfiguration->displacement = 1.493;
    // default "60-2"
    engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
    // default 0.0
    engineConfiguration->trigger.customTotalToothCount = 24;
    // default "On crankshaft"
    engineConfiguration->skippedWheelOnCam = On camshaft;
    // default 0.0
    engineConfiguration->globalTriggerAngleOffset = 80;
    // default 450.0
    engineConfiguration->vvtOffsets[0] = 0;
    // default 200.0
    engineConfiguration->injector.flow = 240;
    // default "None"
    engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;
    // default 300.0
    engineConfiguration->fuelReferencePressure = 294;
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
    engineConfiguration->tpsMin = 124;
    // default 1000.0
    engineConfiguration->tpsMax = 919;
    // default "Normal"
    engineConfiguration->clutchDownPinInverted = false;
    // default "Normal"
    engineConfiguration->clutchUpPinInverted = false;
    // default "Normal"
    engineConfiguration->flexSensorInverted = false;
    // default 0.0
    engineConfiguration->mapLowValueVoltage = 2.91;
    // default 410.0
    engineConfiguration->mapErrorDetectionTooHigh = 250;
    // default "false"
    engineConfiguration->enableAemXSeries = true;
    // default 50.0
    engineConfiguration->manIdlePosition = 29;
    // default 2.0
    engineConfiguration->fan1ExtraIdle = 10;
    // default 92.0
    engineConfiguration->fanOnTemperature = 95;
    // default 88.0
    engineConfiguration->fanOffTemperature = 91;
    // default "false"
    engineConfiguration->enableSoftwareKnock = true;
    // default "false"
    engineConfiguration->enableVerboseCanTx = true;
    // default "Executor"
    engineConfiguration->debugMode = DBG_27;
    // default 200.0
    engineConfiguration->rpmSoftLimitWindowSize = 0;
    // default 4.0
    engineConfiguration->rpmSoftLimitTimingRetard = 0;
    // default 1.0
    engineConfiguration->tpsAccelFractionDivisor = 0;
    // default "Throttle 1"
    engineConfiguration->etbFunctions[0] = DC_None;
    // default "Throttle 2"
    engineConfiguration->etbFunctions[1] = DC_None;
    // default 1.0
    engineConfiguration->etbJamTimeout = 0;
    // default 250.0
    engineConfiguration->gppwm[0].pwmFrequency = 0;
    // default 60.0
    engineConfiguration->gppwm[0].onAboveDuty = 80;
    // default 50.0
    engineConfiguration->gppwm[0].offBelowDuty = 10;
    // default "Zero"
    engineConfiguration->gppwm[0].loadAxis = GPPWM_Map;


	cannedinjector_battLagCorrBins();
	cannedinjector_battLagCorr();
	cannedtractionControlSlipBins();
	cannedboostTableOpenLoop();
	cannedscriptTable4();
	cannedignitionTable();
	cannedveTable();
	cannedlambdaTable();
	cannedgppwm1_rpmBins();
	cannedgppwm1_table();
	cannedtcuSolenoidTable();
```
