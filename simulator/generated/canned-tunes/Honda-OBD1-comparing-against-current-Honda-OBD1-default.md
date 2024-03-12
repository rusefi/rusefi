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
    // default "Ethanol (Flex) %"
    engineConfiguration->ignBlends[1].blendParameter = GPPWM_Zero;
    // default "Lua Gauge 2"
    engineConfiguration->ignBlends[1].yAxisOverride = GPPWM_Zero;
    // default "Ethanol (Flex) %"
    engineConfiguration->ignBlends[2].blendParameter = GPPWM_Zero;
    // default "Lua Gauge 2"
    engineConfiguration->ignBlends[2].yAxisOverride = GPPWM_Zero;
    // default "Ethanol (Flex) %"
    engineConfiguration->ignBlends[3].blendParameter = GPPWM_Zero;
    // default "Lua Gauge 2"
    engineConfiguration->ignBlends[3].yAxisOverride = GPPWM_Zero;
    // default "Ethanol (Flex) %"
    engineConfiguration->veBlends[0].blendParameter = GPPWM_Zero;
    // default "Lua Gauge 2"
    engineConfiguration->veBlends[0].yAxisOverride = GPPWM_Zero;
    // default "Ethanol (Flex) %"
    engineConfiguration->veBlends[1].blendParameter = GPPWM_Zero;
    // default "Lua Gauge 2"
    engineConfiguration->veBlends[1].yAxisOverride = GPPWM_Zero;
    // default "Ethanol (Flex) %"
    engineConfiguration->veBlends[2].blendParameter = GPPWM_Zero;
    // default "Lua Gauge 2"
    engineConfiguration->veBlends[2].yAxisOverride = GPPWM_Zero;
    // default "Ethanol (Flex) %"
    engineConfiguration->veBlends[3].blendParameter = GPPWM_Zero;
    // default "Lua Gauge 2"
    engineConfiguration->veBlends[3].yAxisOverride = GPPWM_Zero;
    // default "Lua Gauge 2"
    engineConfiguration->boostOpenLoopBlends[0].blendParameter = GPPWM_Zero;
    // default "MAP"
    engineConfiguration->boostOpenLoopBlends[0].yAxisOverride = GPPWM_Zero;
    // default 0.0
    engineConfiguration->mapLowValueVoltage = 2.91;
    // default "false"
    engineConfiguration->enableAemXSeries = true;
    // default "false"
    engineConfiguration->enableSoftwareKnock = true;
    // default 500.0
    engineConfiguration->vvtControlMinRpm = 550;
    // default "false"
    engineConfiguration->enableVerboseCanTx = true;
    // default 60.0
    engineConfiguration->gppwm[0].onAboveDuty = 80;
    // default 50.0
    engineConfiguration->gppwm[0].offBelowDuty = 10;


	cannedboostTableOpenLoop();
	cannedscriptTable4();
	cannedignitionTable();
	cannedveTable();
	cannedthrottle2TrimTable();
	cannedlambdaTable();
	cannedtcuSolenoidTable();
```
