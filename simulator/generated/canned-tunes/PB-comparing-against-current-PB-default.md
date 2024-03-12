# PB-comparing-against-current-PB-default

// canned tune https://rusefi.com/online/view.php?msq=1620

```
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
    // default 0.34
    engineConfiguration->throttlePedalSecondaryUpVoltage = 0.74;
    // default 1.86
    engineConfiguration->throttlePedalSecondaryWOTVoltage = 2.28;
    // default "Launch Button"
    engineConfiguration->launchActivationMode = CLUTCH_INPUT_LAUNCH;


	pbcannedboostTableOpenLoop();
	pbcannedscriptTable4();
	pbcannedthrottle2TrimTable();
	pbcannedlambdaTable();
	pbcannedtcuSolenoidTable();
```
