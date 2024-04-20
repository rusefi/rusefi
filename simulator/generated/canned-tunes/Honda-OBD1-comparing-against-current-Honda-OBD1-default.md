# Honda-OBD1-comparing-against-current-Honda-OBD1-default

// canned tune https://rusefi.com/online/view.php?msq=1621

```
    // default 1.645
    engineConfiguration->displacement = 1.493;
    // default "12crank/24cam"
    engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
    // default 0.0
    engineConfiguration->trigger.customTotalToothCount = 24;
    // default "On crankshaft"
    engineConfiguration->skippedWheelOnCam = On camshaft;
    // default 450.0
    engineConfiguration->vvtOffsets[0] = 0;
    // default 248.0
    engineConfiguration->injector.flow = 240;
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
    // default "Normal"
    engineConfiguration->clutchDownPinInverted = false;
    // default "Normal"
    engineConfiguration->clutchUpPinInverted = false;
    // default "Normal"
    engineConfiguration->flexSensorInverted = false;
    // default 0.0
    engineConfiguration->mapLowValueVoltage = 2.91;
    // default "false"
    engineConfiguration->enableAemXSeries = true;
    // default 92.0
    engineConfiguration->fanOnTemperature = 95;
    // default 88.0
    engineConfiguration->fanOffTemperature = 91;
    // default "false"
    engineConfiguration->enableSoftwareKnock = true;
    // default 500.0
    engineConfiguration->vvtControlMinRpm = 550;
    // default "false"
    engineConfiguration->enableVerboseCanTx = true;
    // default 200.0
    engineConfiguration->rpmSoftLimitWindowSize = 0;
    // default 4.0
    engineConfiguration->rpmSoftLimitTimingRetard = 0;
    // default 1.0
    engineConfiguration->tpsAccelFractionDivisor = 0;
    // default 60.0
    engineConfiguration->gppwm[0].onAboveDuty = 80;
    // default 50.0
    engineConfiguration->gppwm[0].offBelowDuty = 10;


	cannedinjector_battLagCorrBins();
	cannedinjector_battLagCorr();
	cannedmafDecodingBins();
	cannedcltIdleCorr();
	cannedboostTableOpenLoop();
	cannedscriptTable4();
	cannedignitionTable();
	cannedignBlends1_rpmBins();
	cannedignBlends2_rpmBins();
	cannedignBlends3_rpmBins();
	cannedignBlends4_rpmBins();
	cannedveBlends1_rpmBins();
	cannedveBlends2_rpmBins();
	cannedveBlends3_rpmBins();
	cannedveBlends4_rpmBins();
	cannedveTable();
	cannedlambdaTable();
	cannedgppwm1_rpmBins();
	cannedgppwm1_table();
	cannedtcuSolenoidTable();
```
