// this section was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/rusefi_config.txt Mon Nov 25 01:00:42 EST 2019
// by class com.rusefi.output.CHeaderConsumer
// begin
#ifndef CONFIG_BOARDS_KINETIS_CONFIG_CONTROLLERS_ALGO_ENGINE_CONFIGURATION_GENERATED_STRUCTURES_H
#define CONFIG_BOARDS_KINETIS_CONFIG_CONTROLLERS_ALGO_ENGINE_CONFIGURATION_GENERATED_STRUCTURES_H
#include "rusefi_types.h"
#define absoluteFuelPressure_offset 76
#define absoluteFuelPressure_offset_hex 4c
#define accelerometerSpiDevice_offset 2712
#define accelerometerSpiDevice_offset_hex a98
#define acCutoffHighRpm_offset 1494
#define acCutoffHighRpm_offset_hex 5d6
#define acCutoffLowRpm_offset 1492
#define acCutoffLowRpm_offset_hex 5d4
#define acIdleRpmBump_offset 1496
#define acIdleRpmBump_offset_hex 5d8
#define acRelayPin_offset 830
#define acRelayPin_offset_hex 33e
#define acRelayPinMode_offset 831
#define acRelayPinMode_offset_hex 33f
#define acSwitchAdc_offset 1469
#define acSwitchAdc_offset_hex 5bd
#define activateAuxPid1_offset 76
#define activateAuxPid1_offset_hex 4c
#define activateAuxPid2_offset 76
#define activateAuxPid2_offset_hex 4c
#define activateAuxPid3_offset 76
#define activateAuxPid3_offset_hex 4c
#define activateAuxPid4_offset 76
#define activateAuxPid4_offset_hex 4c
#define adc_channel_e_enum "PA2", "PA3", "INVALID", "PD3", "INVALID", "INVALID", "INVALID", "PB12", "PB13", "INVALID", "PE2", "INVALID", "PC14", "PC15", "PC16", "PC17", "Disabled", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID"
#define adcVcc_offset 548
#define adcVcc_offset_hex 224
#define afr_alignAf_offset 561
#define afr_alignAf_offset_hex 231
#define afr_hwChannel_offset 560
#define afr_hwChannel_offset_hex 230
#define afr_offset 560
#define afr_offset_hex 230
#define afr_type_offset 692
#define afr_type_offset_hex 2b4
#define afr_v1_offset 564
#define afr_v1_offset_hex 234
#define afr_v2_offset 572
#define afr_v2_offset_hex 23c
#define afr_value1_offset 568
#define afr_value1_offset_hex 238
#define afr_value2_offset 576
#define afr_value2_offset_hex 240
#define afrLoadBins_offset 18848
#define afrLoadBins_offset_hex 49a0
#define afrRpmBins_offset 18912
#define afrRpmBins_offset_hex 49e0
#define afrTable_offset 18592
#define afrTable_offset_hex 48a0
#define afterCrankingIACtaperDuration_offset 2036
#define afterCrankingIACtaperDuration_offset_hex 7f4
#define alFIn_offset 4024
#define alFIn_offset_hex fb8
#define alignEngineSnifferAtTDC_offset 744
#define alignEngineSnifferAtTDC_offset_hex 2e8
#define alternator_antiwindupFreq_offset 1764
#define alternator_antiwindupFreq_offset_hex 6e4
#define alternator_derivativeFilterLoss_offset 1760
#define alternator_derivativeFilterLoss_offset_hex 6e0
#define alternatorControl_dFactor_offset 1724
#define alternatorControl_dFactor_offset_hex 6bc
#define alternatorControl_iFactor_offset 1720
#define alternatorControl_iFactor_offset_hex 6b8
#define alternatorControl_maxValue_offset 1734
#define alternatorControl_maxValue_offset_hex 6c6
#define alternatorControl_minValue_offset 1732
#define alternatorControl_minValue_offset_hex 6c4
#define alternatorControl_offset 1716
#define alternatorControl_offset_hex 6b4
#define alternatorControl_offset_offset 1728
#define alternatorControl_offset_offset_hex 6c0
#define alternatorControl_periodMs_offset 1730
#define alternatorControl_periodMs_offset_hex 6c2
#define alternatorControl_pFactor_offset 1716
#define alternatorControl_pFactor_offset_hex 6b4
#define alternatorControlPin_offset 665
#define alternatorControlPin_offset_hex 299
#define alternatorControlPinMode_offset 666
#define alternatorControlPinMode_offset_hex 29a
#define alternatorOffAboveTps_offset 2028
#define alternatorOffAboveTps_offset_hex 7ec
#define alternatorPwmFrequency_offset 2256
#define alternatorPwmFrequency_offset_hex 8d0
#define ambiguousOperationMode_offset 488
#define ambiguousOperationMode_offset_hex 1e8
#define analogInputDividerCoefficient_offset 460
#define analogInputDividerCoefficient_offset_hex 1cc
#define autoTuneCltThreshold_offset 2420
#define autoTuneCltThreshold_offset_hex 974
#define autoTuneTpsQuietPeriod_offset 2428
#define autoTuneTpsQuietPeriod_offset_hex 97c
#define autoTuneTpsRocThreshold_offset 2424
#define autoTuneTpsRocThreshold_offset_hex 978
#define AUX_DIGITAL_VALVE_COUNT 2
#define AUX_PID_COUNT 4
#define auxPid1_dFactor_offset 2620
#define auxPid1_dFactor_offset_hex a3c
#define auxPid1_iFactor_offset 2616
#define auxPid1_iFactor_offset_hex a38
#define auxPid1_maxValue_offset 2630
#define auxPid1_maxValue_offset_hex a46
#define auxPid1_minValue_offset 2628
#define auxPid1_minValue_offset_hex a44
#define auxPid1_offset 2612
#define auxPid1_offset_hex a34
#define auxPid1_offset_offset 2624
#define auxPid1_offset_offset_hex a40
#define auxPid1_periodMs_offset 2626
#define auxPid1_periodMs_offset_hex a42
#define auxPid1_pFactor_offset 2612
#define auxPid1_pFactor_offset_hex a34
#define auxPid2_dFactor_offset 2640
#define auxPid2_dFactor_offset_hex a50
#define auxPid2_iFactor_offset 2636
#define auxPid2_iFactor_offset_hex a4c
#define auxPid2_maxValue_offset 2650
#define auxPid2_maxValue_offset_hex a5a
#define auxPid2_minValue_offset 2648
#define auxPid2_minValue_offset_hex a58
#define auxPid2_offset 2632
#define auxPid2_offset_hex a48
#define auxPid2_offset_offset 2644
#define auxPid2_offset_offset_hex a54
#define auxPid2_periodMs_offset 2646
#define auxPid2_periodMs_offset_hex a56
#define auxPid2_pFactor_offset 2632
#define auxPid2_pFactor_offset_hex a48
#define auxPid3_dFactor_offset 2660
#define auxPid3_dFactor_offset_hex a64
#define auxPid3_iFactor_offset 2656
#define auxPid3_iFactor_offset_hex a60
#define auxPid3_maxValue_offset 2670
#define auxPid3_maxValue_offset_hex a6e
#define auxPid3_minValue_offset 2668
#define auxPid3_minValue_offset_hex a6c
#define auxPid3_offset 2652
#define auxPid3_offset_hex a5c
#define auxPid3_offset_offset 2664
#define auxPid3_offset_offset_hex a68
#define auxPid3_periodMs_offset 2666
#define auxPid3_periodMs_offset_hex a6a
#define auxPid3_pFactor_offset 2652
#define auxPid3_pFactor_offset_hex a5c
#define auxPid4_dFactor_offset 2680
#define auxPid4_dFactor_offset_hex a78
#define auxPid4_iFactor_offset 2676
#define auxPid4_iFactor_offset_hex a74
#define auxPid4_maxValue_offset 2690
#define auxPid4_maxValue_offset_hex a82
#define auxPid4_minValue_offset 2688
#define auxPid4_minValue_offset_hex a80
#define auxPid4_offset 2672
#define auxPid4_offset_hex a70
#define auxPid4_offset_offset 2684
#define auxPid4_offset_offset_hex a7c
#define auxPid4_periodMs_offset 2686
#define auxPid4_periodMs_offset_hex a7e
#define auxPid4_pFactor_offset 2672
#define auxPid4_pFactor_offset_hex a70
#define auxPidFrequency1_offset 2248
#define auxPidFrequency1_offset_hex 8c8
#define auxPidFrequency2_offset 2250
#define auxPidFrequency2_offset_hex 8ca
#define auxPidFrequency3_offset 2252
#define auxPidFrequency3_offset_hex 8cc
#define auxPidFrequency4_offset 2254
#define auxPidFrequency4_offset_hex 8ce
#define auxPidPins1_offset 2220
#define auxPidPins1_offset_hex 8ac
#define auxPidPins2_offset 2221
#define auxPidPins2_offset_hex 8ad
#define auxPidPins3_offset 2222
#define auxPidPins3_offset_hex 8ae
#define auxPidPins4_offset 2223
#define auxPidPins4_offset_hex 8af
#define auxTempSensor1_adcChannel_offset 2472
#define auxTempSensor1_adcChannel_offset_hex 9a8
#define auxTempSensor1_alignmentFill_offset 2473
#define auxTempSensor1_alignmentFill_offset_hex 9a9
#define auxTempSensor1_bias_resistor_offset 2468
#define auxTempSensor1_bias_resistor_offset_hex 9a4
#define auxTempSensor1_config_offset 2444
#define auxTempSensor1_config_offset_hex 98c
#define auxTempSensor1_offset 2444
#define auxTempSensor1_offset_hex 98c
#define auxTempSensor1_resistance_1_offset 2456
#define auxTempSensor1_resistance_1_offset_hex 998
#define auxTempSensor1_resistance_2_offset 2460
#define auxTempSensor1_resistance_2_offset_hex 99c
#define auxTempSensor1_resistance_3_offset 2464
#define auxTempSensor1_resistance_3_offset_hex 9a0
#define auxTempSensor1_tempC_1_offset 2444
#define auxTempSensor1_tempC_1_offset_hex 98c
#define auxTempSensor1_tempC_2_offset 2448
#define auxTempSensor1_tempC_2_offset_hex 990
#define auxTempSensor1_tempC_3_offset 2452
#define auxTempSensor1_tempC_3_offset_hex 994
#define auxTempSensor2_adcChannel_offset 2504
#define auxTempSensor2_adcChannel_offset_hex 9c8
#define auxTempSensor2_alignmentFill_offset 2505
#define auxTempSensor2_alignmentFill_offset_hex 9c9
#define auxTempSensor2_bias_resistor_offset 2500
#define auxTempSensor2_bias_resistor_offset_hex 9c4
#define auxTempSensor2_config_offset 2476
#define auxTempSensor2_config_offset_hex 9ac
#define auxTempSensor2_offset 2476
#define auxTempSensor2_offset_hex 9ac
#define auxTempSensor2_resistance_1_offset 2488
#define auxTempSensor2_resistance_1_offset_hex 9b8
#define auxTempSensor2_resistance_2_offset 2492
#define auxTempSensor2_resistance_2_offset_hex 9bc
#define auxTempSensor2_resistance_3_offset 2496
#define auxTempSensor2_resistance_3_offset_hex 9c0
#define auxTempSensor2_tempC_1_offset 2476
#define auxTempSensor2_tempC_1_offset_hex 9ac
#define auxTempSensor2_tempC_2_offset 2480
#define auxTempSensor2_tempC_2_offset_hex 9b0
#define auxTempSensor2_tempC_3_offset 2484
#define auxTempSensor2_tempC_3_offset_hex 9b4
#define auxValves1_offset 1816
#define auxValves1_offset_hex 718
#define auxValves2_offset 1817
#define auxValves2_offset_hex 719
#define BARO_CORR_SIZE 4
#define baroCorrPressureBins_offset 1544
#define baroCorrPressureBins_offset_hex 608
#define baroCorrRpmBins_offset 1560
#define baroCorrRpmBins_offset_hex 618
#define baroCorrTable_offset 1576
#define baroCorrTable_offset_hex 628
#define baroSensor_align_offset 597
#define baroSensor_align_offset_hex 255
#define baroSensor_highValue_offset 588
#define baroSensor_highValue_offset_hex 24c
#define baroSensor_hwChannel_offset 596
#define baroSensor_hwChannel_offset_hex 254
#define baroSensor_lowValue_offset 584
#define baroSensor_lowValue_offset_hex 248
#define baroSensor_offset 584
#define baroSensor_offset_hex 248
#define baroSensor_type_offset 592
#define baroSensor_type_offset_hex 250
#define bc_offset 600
#define bc_offset_hex 258
#define binarySerialRxPin_offset 1815
#define binarySerialRxPin_offset_hex 717
#define binarySerialTxPin_offset 1814
#define binarySerialTxPin_offset_hex 716
#define biQuad_a0_offset 2332
#define biQuad_a0_offset_hex 91c
#define biQuad_a1_offset 2336
#define biQuad_a1_offset_hex 920
#define biQuad_a2_offset 2340
#define biQuad_a2_offset_hex 924
#define biQuad_b1_offset 2344
#define biQuad_b1_offset_hex 928
#define biQuad_b2_offset 2348
#define biQuad_b2_offset_hex 92c
#define biQuad_offset 2332
#define biQuad_offset_hex 91c
#define BLOCKING_FACTOR 400
#define boostCutPressure_offset 2132
#define boostCutPressure_offset_hex 854
#define brain_input_pin_e_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "N/A", "N/A", "PA10", "PA11", "PA12", "PA13", "N/A", "N/A", "N/A", "N/A", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "N/A", "N/A", "N/A", "N/A", "PB12", "PB13", "N/A", "N/A", "N/A", "N/A", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "N/A", "N/A", "N/A", "N/A", "PC14", "PC15", "PC16", "PC17", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "PD15", "PD16", "N/A", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A"
#define brain_pin_e_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PA8", "PA9", "PA10", "PA11", "PA12", "PA13", "PA14", "PA15", "PA16", "PA17", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PB8", "PB9", "PB10", "PB11", "PB12", "PB13", "PB14", "PB15", "PB16", "PB17", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "PC10", "PC11", "PC12", "PC13", "PC14", "PC15", "PC16", "PC17", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PD8", "PD9", "PD10", "PD11", "PD12", "PD13", "PD14", "PD15", "PD16", "PD17", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "PE12", "PE13", "PE14", "PE15", "PE16", "PE17", "TLE6240_1", "TLE6240_2", "TLE6240_3", "TLE6240_4", "TLE6240_5", "TLE6240_6", "TLE6240_7", "TLE6240_8", "TLE6240_9", "TLE6240_10", "TLE6240_11", "TLE6240_12", "TLE6240_13", "TLE6240_14", "TLE6240_15", "TLE6240_16"
#define brakePedalPin_offset 2608
#define brakePedalPin_offset_hex a30
#define brakePedalPinMode_offset 2611
#define brakePedalPinMode_offset_hex a33
#define byFirmwareVersion_offset 500
#define byFirmwareVersion_offset_hex 1f4
#define CAM_INPUTS_COUNT 4
#define camInputs1_offset 556
#define camInputs1_offset_hex 22c
#define camInputs2_offset 557
#define camInputs2_offset_hex 22d
#define camInputs3_offset 558
#define camInputs3_offset_hex 22e
#define camInputs4_offset 559
#define camInputs4_offset_hex 22f
#define can_device_mode_e_enum "v0", "v1"
#define canDeviceMode_offset 732
#define canDeviceMode_offset_hex 2dc
#define canNbcType_offset 480
#define canNbcType_offset_hex 1e0
#define canReadEnabled_offset 1464
#define canReadEnabled_offset_hex 5b8
#define canRxPin_offset 709
#define canRxPin_offset_hex 2c5
#define canSleepPeriodMs_offset 484
#define canSleepPeriodMs_offset_hex 1e4
#define canTxPin_offset 708
#define canTxPin_offset_hex 2c4
#define canWriteEnabled_offset 1464
#define canWriteEnabled_offset_hex 5b8
#define cdmInputPin_offset 937
#define cdmInputPin_offset_hex 3a9
#define cj125CsPin_offset 674
#define cj125CsPin_offset_hex 2a2
#define cj125CsPinMode_offset 2225
#define cj125CsPinMode_offset_hex 8b1
#define cj125isLsu49_offset 76
#define cj125isLsu49_offset_hex 4c
#define cj125isUaDivided_offset 76
#define cj125isUaDivided_offset_hex 4c
#define cj125isUrDivided_offset 76
#define cj125isUrDivided_offset_hex 4c
#define cj125SpiDevice_offset 2224
#define cj125SpiDevice_offset_hex 8b0
#define cj125ua_offset 2609
#define cj125ua_offset_hex a31
#define cj125ur_offset 2610
#define cj125ur_offset_hex a32
#define clt_adcChannel_offset 280
#define clt_adcChannel_offset_hex 118
#define clt_alignmentFill_offset 281
#define clt_alignmentFill_offset_hex 119
#define clt_bias_resistor_offset 276
#define clt_bias_resistor_offset_hex 114
#define clt_config_offset 252
#define clt_config_offset_hex fc
#define CLT_CRANKING_CURVE_SIZE 8
#define CLT_CURVE_SIZE 16
#define clt_offset 252
#define clt_offset_hex fc
#define clt_resistance_1_offset 264
#define clt_resistance_1_offset_hex 108
#define clt_resistance_2_offset 268
#define clt_resistance_2_offset_hex 10c
#define clt_resistance_3_offset 272
#define clt_resistance_3_offset_hex 110
#define clt_tempC_1_offset 252
#define clt_tempC_1_offset_hex fc
#define clt_tempC_2_offset 256
#define clt_tempC_2_offset_hex 100
#define clt_tempC_3_offset 260
#define clt_tempC_3_offset_hex 104
#define CLT_TIMING_CURVE_SIZE 8
#define cltCrankingCorr_offset 6512
#define cltCrankingCorr_offset_hex 1970
#define cltCrankingCorrBins_offset 6480
#define cltCrankingCorrBins_offset_hex 1950
#define cltFuelCorr_offset 10336
#define cltFuelCorr_offset_hex 2860
#define cltFuelCorrBins_offset 10272
#define cltFuelCorrBins_offset_hex 2820
#define cltIdleCorr_offset 10720
#define cltIdleCorr_offset_hex 29e0
#define cltIdleCorrBins_offset 10656
#define cltIdleCorrBins_offset_hex 29a0
#define cltIdleRpm_offset 1960
#define cltIdleRpm_offset_hex 7a8
#define cltIdleRpmBins_offset 1896
#define cltIdleRpmBins_offset_hex 768
#define cltTimingBins_offset 2352
#define cltTimingBins_offset_hex 930
#define cltTimingExtra_offset 2384
#define cltTimingExtra_offset_hex 950
#define clutchDownPin_offset 664
#define clutchDownPin_offset_hex 298
#define clutchDownPinInverted_offset 976
#define clutchDownPinInverted_offset_hex 3d0
#define clutchDownPinMode_offset 667
#define clutchDownPinMode_offset_hex 29b
#define clutchUpPin_offset 969
#define clutchUpPin_offset_hex 3c9
#define clutchUpPinInverted_offset 976
#define clutchUpPinInverted_offset_hex 3d0
#define clutchUpPinMode_offset 971
#define clutchUpPinMode_offset_hex 3cb
#define CMD_CALIBRATE_PEDAL_DOWN "calibrate_pedal_down"
#define CMD_CALIBRATE_PEDAL_UP "calibrate_pedal_up"
#define CMD_DATE "date"
#define CMD_DISABLE "disable"
#define CMD_ENABLE "enable"
#define CMD_ENGINE_TYPE "engine_type"
#define CMD_ETB_DUTY "set_etb_duty"
#define CMD_REBOOT "reboot"
#define CMD_REBOOT_DFU "reboot_dfu"
#define CMD_TRIGGER_HW_INPUT "trigger_hw_input"
#define CMD_TRIGGERINFO "triggerinfo"
#define CMD_WRITECONFIG "writeconfig"
#define coastingFuelCutClt_offset 3154
#define coastingFuelCutClt_offset_hex c52
#define coastingFuelCutEnabled_offset 744
#define coastingFuelCutEnabled_offset_hex 2e8
#define coastingFuelCutMap_offset 3158
#define coastingFuelCutMap_offset_hex c56
#define coastingFuelCutRpmHigh_offset 3148
#define coastingFuelCutRpmHigh_offset_hex c4c
#define coastingFuelCutRpmLow_offset 3150
#define coastingFuelCutRpmLow_offset_hex c4e
#define coastingFuelCutTps_offset 3152
#define coastingFuelCutTps_offset_hex c50
#define communicationLedPin_offset 1812
#define communicationLedPin_offset_hex 714
#define CONSOLE_DATA_PROTOCOL_TAG " @"
#define consoleLoopPeriodMs_offset 716
#define consoleLoopPeriodMs_offset_hex 2cc
#define consoleSerialRxPin_offset 1819
#define consoleSerialRxPin_offset_hex 71b
#define consoleSerialTxPin_offset 1818
#define consoleSerialTxPin_offset_hex 71a
#define consoleUartDevice_offset 943
#define consoleUartDevice_offset_hex 3af
#define CRANKING_ADVANCE_CURVE_SIZE 4
#define cranking_alignmentFill_offset 94
#define cranking_alignmentFill_offset_hex 5e
#define cranking_baseFuel_offset 88
#define cranking_baseFuel_offset_hex 58
#define CRANKING_CLT_IDLE_CURVE_SIZE 8
#define CRANKING_CURVE_SIZE 8
#define cranking_offset 88
#define cranking_offset_hex 58
#define cranking_rpm_offset 92
#define cranking_rpm_offset_hex 5c
#define crankingAdvance_offset 3124
#define crankingAdvance_offset_hex c34
#define crankingAdvanceBins_offset 3108
#define crankingAdvanceBins_offset_hex c24
#define crankingChargeAngle_offset 104
#define crankingChargeAngle_offset_hex 68
#define crankingCycleBins_offset 10624
#define crankingCycleBins_offset_hex 2980
#define crankingCycleCoef_offset 10592
#define crankingCycleCoef_offset_hex 2960
#define crankingFuelBins_offset 10560
#define crankingFuelBins_offset_hex 2940
#define crankingFuelCoef_offset 10528
#define crankingFuelCoef_offset_hex 2920
#define crankingIACposition_offset 2228
#define crankingIACposition_offset_hex 8b4
#define crankingInjectionMode_offset 424
#define crankingInjectionMode_offset_hex 1a8
#define crankingTimingAngle_offset 436
#define crankingTimingAngle_offset_hex 1b4
#define crankingTpsBins_offset 1672
#define crankingTpsBins_offset_hex 688
#define crankingTpsCoef_offset 1640
#define crankingTpsCoef_offset_hex 668
#define cutFuelOnHardLimit_offset 1464
#define cutFuelOnHardLimit_offset_hex 5b8
#define cutSparkOnHardLimit_offset 1464
#define cutSparkOnHardLimit_offset_hex 5b8
#define cylinderBore_offset 408
#define cylinderBore_offset_hex 198
#define cylindersCount_offset 400
#define cylindersCount_offset_hex 190
#define debug_mode_e_enum "Alternator PID", "TPS acceleration enrichment", "INVALID", "Idle Control", "Engine Load accl enrich", "Trigger Counters", "FSIO_ADC", "AUX_PID_1", "VVT input", "Cranking", "Timing", "Closed-loop fuel corr PID", "VSS", "SD card", "sr5", "Knock", "Trigger Sync", "Electronic Throttle", "Executor", "Bench Test / TS commands", "Aux Valves", "Analog inputs #1", "INSTANT_RPM", "FSIO_EXPRESSION", "Status", "CJ125", "CAN", "MAP", "Metrics", "ETB#2", "Ion Sense", "TLE8888", "Analog inputs #2", "Dwell Metric", "Aux Temperature", "ETB Logic"
#define debugMapAveraging_offset 807
#define debugMapAveraging_offset_hex 327
#define debugMode_offset 2092
#define debugMode_offset_hex 82c
#define debugSetTimer_offset 806
#define debugSetTimer_offset_hex 326
#define debugTimerCallback_offset 711
#define debugTimerCallback_offset_hex 2c7
#define debugTriggerSync_offset 676
#define debugTriggerSync_offset_hex 2a4
#define DIGIPOT_COUNT 4
#define digitalPotentiometerChipSelect1_offset 668
#define digitalPotentiometerChipSelect1_offset_hex 29c
#define digitalPotentiometerChipSelect2_offset 669
#define digitalPotentiometerChipSelect2_offset_hex 29d
#define digitalPotentiometerChipSelect3_offset 670
#define digitalPotentiometerChipSelect3_offset_hex 29e
#define digitalPotentiometerChipSelect4_offset 671
#define digitalPotentiometerChipSelect4_offset_hex 29f
#define digitalPotentiometerSpiDevice_offset 677
#define digitalPotentiometerSpiDevice_offset_hex 2a5
#define directSelfStimulation_offset 76
#define directSelfStimulation_offset_hex 4c
#define displacement_offset 396
#define displacement_offset_hex 18c
#define displayMode_offset 492
#define displayMode_offset_hex 1ec
#define dizzySparkOutputPin_offset 2226
#define dizzySparkOutputPin_offset_hex 8b2
#define dizzySparkOutputPinMode_offset 2227
#define dizzySparkOutputPinMode_offset_hex 8b3
#define DWELL_CURVE_SIZE 8
#define ego_sensor_e_enum "BPSX", "Innovate", "14Point7", "Narrow", "PLX", "Custom"
#define egoValueShift_offset 2216
#define egoValueShift_offset_hex 8a8
#define EGT_CHANNEL_COUNT 8
#define electronicThrottlePin1Mode_offset 672
#define electronicThrottlePin1Mode_offset_hex 2a0
#define enabledStep1Limiter_offset 744
#define enabledStep1Limiter_offset_hex 2e8
#define engine_load_mode_e_enum "MAF", "Alpha-N/TPS", "MAP", "SPEED DENSITY", "MAF Air Charge"
#define ENGINE_NOISE_CURVE_SIZE 8
#define engineChartSize_offset 1480
#define engineChartSize_offset_hex 5c8
#define engineConfiguration_offset 0
#define engineConfiguration_offset_hex 0
#define engineLoadAccelEnrichmentMultiplier_offset 2072
#define engineLoadAccelEnrichmentMultiplier_offset_hex 818
#define engineLoadAccelEnrichmentThreshold_offset 2068
#define engineLoadAccelEnrichmentThreshold_offset_hex 814
#define engineLoadAccelLength_offset 2056
#define engineLoadAccelLength_offset_hex 808
#define engineLoadDecelEnleanmentMultiplier_offset 2064
#define engineLoadDecelEnleanmentMultiplier_offset_hex 810
#define engineLoadDecelEnleanmentThreshold_offset 2060
#define engineLoadDecelEnleanmentThreshold_offset_hex 80c
#define engineSnifferRpmThreshold_offset 4
#define engineSnifferRpmThreshold_offset_hex 4
#define engineType_offset 0
#define engineType_offset_hex 0
#define etb1_controlPin1_offset 682
#define etb1_controlPin1_offset_hex 2aa
#define etb1_controlPinMode_offset 683
#define etb1_controlPinMode_offset_hex 2ab
#define etb1_directionPin1_offset 680
#define etb1_directionPin1_offset_hex 2a8
#define etb1_directionPin2_offset 681
#define etb1_directionPin2_offset_hex 2a9
#define etb1_offset 680
#define etb1_offset_hex 2a8
#define etb1_use_two_wires_offset 76
#define etb1_use_two_wires_offset_hex 4c
#define etb2_controlPin1_offset 3966
#define etb2_controlPin1_offset_hex f7e
#define etb2_controlPinMode_offset 3967
#define etb2_controlPinMode_offset_hex f7f
#define etb2_directionPin1_offset 3964
#define etb2_directionPin1_offset_hex f7c
#define etb2_directionPin2_offset 3965
#define etb2_directionPin2_offset_hex f7d
#define etb2_offset 3964
#define etb2_offset_hex f7c
#define etb2_use_two_wires_offset 76
#define etb2_use_two_wires_offset_hex 4c
#define ETB_BIAS_CURVE_LENGTH 8
#define etb_dFactor_offset 1744
#define etb_dFactor_offset_hex 6d0
#define etb_iFactor_offset 1740
#define etb_iFactor_offset_hex 6cc
#define etb_iTermMax_offset 3958
#define etb_iTermMax_offset_hex f76
#define etb_iTermMin_offset 3956
#define etb_iTermMin_offset_hex f74
#define etb_maxValue_offset 1754
#define etb_maxValue_offset_hex 6da
#define etb_minValue_offset 1752
#define etb_minValue_offset_hex 6d8
#define etb_offset 1736
#define etb_offset_hex 6c8
#define etb_offset_offset 1748
#define etb_offset_offset_hex 6d4
#define etb_periodMs_offset 1750
#define etb_periodMs_offset_hex 6d6
#define etb_pFactor_offset 1736
#define etb_pFactor_offset_hex 6c8
#define etbBiasBins_offset 3888
#define etbBiasBins_offset_hex f30
#define etbBiasValues_offset 3920
#define etbBiasValues_offset_hex f50
#define etbCalibrationOnStart_offset 1476
#define etbCalibrationOnStart_offset_hex 5c4
#define etbDeadband_offset 3960
#define etbDeadband_offset_hex f78
#define etbFreq_offset 2514
#define etbFreq_offset_hex 9d2
#define etbIdleRange_offset 972
#define etbIdleRange_offset_hex 3cc
#define etbIdleThrottleRange_offset 4012
#define etbIdleThrottleRange_offset_hex fac
#define etbNeutralPosition_offset 1471
#define etbNeutralPosition_offset_hex 5bf
#define externalKnockSenseAdc_offset 3103
#define externalKnockSenseAdc_offset_hex c1f
#define extraInjectionOffset_offset 432
#define extraInjectionOffset_offset_hex 1b0
#define fanOffTemperature_offset 472
#define fanOffTemperature_offset_hex 1d8
#define fanOnTemperature_offset 468
#define fanOnTemperature_offset_hex 1d4
#define fanPin_offset 663
#define fanPin_offset_hex 297
#define fanPinMode_offset 662
#define fanPinMode_offset_hex 296
#define firingOrder_offset 404
#define firingOrder_offset_hex 194
#define fixedModeTiming_offset 452
#define fixedModeTiming_offset_hex 1c4
#define fixedTiming_offset 2204
#define fixedTiming_offset_hex 89c
#define frequencyReportingMapInputPin_offset 970
#define frequencyReportingMapInputPin_offset_hex 3ca
#define FSIO_ANALOG_INPUT_COUNT 4
#define FSIO_COMMAND_COUNT 16
#define FSIO_CURVE_16 16
#define FSIO_CURVE_8 8
#define fsio_setting10_offset 900
#define fsio_setting10_offset_hex 384
#define fsio_setting11_offset 904
#define fsio_setting11_offset_hex 388
#define fsio_setting12_offset 908
#define fsio_setting12_offset_hex 38c
#define fsio_setting13_offset 912
#define fsio_setting13_offset_hex 390
#define fsio_setting14_offset 916
#define fsio_setting14_offset_hex 394
#define fsio_setting15_offset 920
#define fsio_setting15_offset_hex 398
#define fsio_setting16_offset 924
#define fsio_setting16_offset_hex 39c
#define fsio_setting1_offset 864
#define fsio_setting1_offset_hex 360
#define fsio_setting2_offset 868
#define fsio_setting2_offset_hex 364
#define fsio_setting3_offset 872
#define fsio_setting3_offset_hex 368
#define fsio_setting4_offset 876
#define fsio_setting4_offset_hex 36c
#define fsio_setting5_offset 880
#define fsio_setting5_offset_hex 370
#define fsio_setting6_offset 884
#define fsio_setting6_offset_hex 374
#define fsio_setting7_offset 888
#define fsio_setting7_offset_hex 378
#define fsio_setting8_offset 892
#define fsio_setting8_offset_hex 37c
#define fsio_setting9_offset 896
#define fsio_setting9_offset_hex 380
#define FSIO_TABLE_8 8
#define fsioAdc1_offset 2200
#define fsioAdc1_offset_hex 898
#define fsioAdc2_offset 2201
#define fsioAdc2_offset_hex 899
#define fsioAdc3_offset 2202
#define fsioAdc3_offset_hex 89a
#define fsioAdc4_offset 2203
#define fsioAdc4_offset_hex 89b
#define fsioCurve1_offset 2780
#define fsioCurve1_offset_hex adc
#define fsioCurve1Bins_offset 2716
#define fsioCurve1Bins_offset_hex a9c
#define fsioCurve2_offset 2908
#define fsioCurve2_offset_hex b5c
#define fsioCurve2Bins_offset 2844
#define fsioCurve2Bins_offset_hex b1c
#define fsioCurve3_offset 3004
#define fsioCurve3_offset_hex bbc
#define fsioCurve3Bins_offset 2972
#define fsioCurve3Bins_offset_hex b9c
#define fsioCurve4_offset 3068
#define fsioCurve4_offset_hex bfc
#define fsioCurve4Bins_offset 3036
#define fsioCurve4Bins_offset_hex bdc
#define fsioDigitalInputs10_offset 961
#define fsioDigitalInputs10_offset_hex 3c1
#define fsioDigitalInputs11_offset 962
#define fsioDigitalInputs11_offset_hex 3c2
#define fsioDigitalInputs12_offset 963
#define fsioDigitalInputs12_offset_hex 3c3
#define fsioDigitalInputs13_offset 964
#define fsioDigitalInputs13_offset_hex 3c4
#define fsioDigitalInputs14_offset 965
#define fsioDigitalInputs14_offset_hex 3c5
#define fsioDigitalInputs15_offset 966
#define fsioDigitalInputs15_offset_hex 3c6
#define fsioDigitalInputs16_offset 967
#define fsioDigitalInputs16_offset_hex 3c7
#define fsioDigitalInputs1_offset 952
#define fsioDigitalInputs1_offset_hex 3b8
#define fsioDigitalInputs2_offset 953
#define fsioDigitalInputs2_offset_hex 3b9
#define fsioDigitalInputs3_offset 954
#define fsioDigitalInputs3_offset_hex 3ba
#define fsioDigitalInputs4_offset 955
#define fsioDigitalInputs4_offset_hex 3bb
#define fsioDigitalInputs5_offset 956
#define fsioDigitalInputs5_offset_hex 3bc
#define fsioDigitalInputs6_offset 957
#define fsioDigitalInputs6_offset_hex 3bd
#define fsioDigitalInputs7_offset 958
#define fsioDigitalInputs7_offset_hex 3be
#define fsioDigitalInputs8_offset 959
#define fsioDigitalInputs8_offset_hex 3bf
#define fsioDigitalInputs9_offset 960
#define fsioDigitalInputs9_offset_hex 3c0
#define fsioFormulas10_offset 8472
#define fsioFormulas10_offset_hex 2118
#define fsioFormulas11_offset 8672
#define fsioFormulas11_offset_hex 21e0
#define fsioFormulas12_offset 8872
#define fsioFormulas12_offset_hex 22a8
#define fsioFormulas13_offset 9072
#define fsioFormulas13_offset_hex 2370
#define fsioFormulas14_offset 9272
#define fsioFormulas14_offset_hex 2438
#define fsioFormulas15_offset 9472
#define fsioFormulas15_offset_hex 2500
#define fsioFormulas16_offset 9672
#define fsioFormulas16_offset_hex 25c8
#define fsioFormulas1_offset 6672
#define fsioFormulas1_offset_hex 1a10
#define fsioFormulas2_offset 6872
#define fsioFormulas2_offset_hex 1ad8
#define fsioFormulas3_offset 7072
#define fsioFormulas3_offset_hex 1ba0
#define fsioFormulas4_offset 7272
#define fsioFormulas4_offset_hex 1c68
#define fsioFormulas5_offset 7472
#define fsioFormulas5_offset_hex 1d30
#define fsioFormulas6_offset 7672
#define fsioFormulas6_offset_hex 1df8
#define fsioFormulas7_offset 7872
#define fsioFormulas7_offset_hex 1ec0
#define fsioFormulas8_offset 8072
#define fsioFormulas8_offset_hex 1f88
#define fsioFormulas9_offset 8272
#define fsioFormulas9_offset_hex 2050
#define fsioFrequency10_offset 850
#define fsioFrequency10_offset_hex 352
#define fsioFrequency11_offset 852
#define fsioFrequency11_offset_hex 354
#define fsioFrequency12_offset 854
#define fsioFrequency12_offset_hex 356
#define fsioFrequency13_offset 856
#define fsioFrequency13_offset_hex 358
#define fsioFrequency14_offset 858
#define fsioFrequency14_offset_hex 35a
#define fsioFrequency15_offset 860
#define fsioFrequency15_offset_hex 35c
#define fsioFrequency16_offset 862
#define fsioFrequency16_offset_hex 35e
#define fsioFrequency1_offset 832
#define fsioFrequency1_offset_hex 340
#define fsioFrequency2_offset 834
#define fsioFrequency2_offset_hex 342
#define fsioFrequency3_offset 836
#define fsioFrequency3_offset_hex 344
#define fsioFrequency4_offset 838
#define fsioFrequency4_offset_hex 346
#define fsioFrequency5_offset 840
#define fsioFrequency5_offset_hex 348
#define fsioFrequency6_offset 842
#define fsioFrequency6_offset_hex 34a
#define fsioFrequency7_offset 844
#define fsioFrequency7_offset_hex 34c
#define fsioFrequency8_offset 846
#define fsioFrequency8_offset_hex 34e
#define fsioFrequency9_offset 848
#define fsioFrequency9_offset_hex 350
#define fsioInputModes10_offset 1525
#define fsioInputModes10_offset_hex 5f5
#define fsioInputModes11_offset 1526
#define fsioInputModes11_offset_hex 5f6
#define fsioInputModes12_offset 1527
#define fsioInputModes12_offset_hex 5f7
#define fsioInputModes13_offset 1528
#define fsioInputModes13_offset_hex 5f8
#define fsioInputModes14_offset 1529
#define fsioInputModes14_offset_hex 5f9
#define fsioInputModes15_offset 1530
#define fsioInputModes15_offset_hex 5fa
#define fsioInputModes16_offset 1531
#define fsioInputModes16_offset_hex 5fb
#define fsioInputModes1_offset 1516
#define fsioInputModes1_offset_hex 5ec
#define fsioInputModes2_offset 1517
#define fsioInputModes2_offset_hex 5ed
#define fsioInputModes3_offset 1518
#define fsioInputModes3_offset_hex 5ee
#define fsioInputModes4_offset 1519
#define fsioInputModes4_offset_hex 5ef
#define fsioInputModes5_offset 1520
#define fsioInputModes5_offset_hex 5f0
#define fsioInputModes6_offset 1521
#define fsioInputModes6_offset_hex 5f1
#define fsioInputModes7_offset 1522
#define fsioInputModes7_offset_hex 5f2
#define fsioInputModes8_offset 1523
#define fsioInputModes8_offset_hex 5f3
#define fsioInputModes9_offset 1524
#define fsioInputModes9_offset_hex 5f4
#define fsioOutputPins10_offset 789
#define fsioOutputPins10_offset_hex 315
#define fsioOutputPins11_offset 790
#define fsioOutputPins11_offset_hex 316
#define fsioOutputPins12_offset 791
#define fsioOutputPins12_offset_hex 317
#define fsioOutputPins13_offset 792
#define fsioOutputPins13_offset_hex 318
#define fsioOutputPins14_offset 793
#define fsioOutputPins14_offset_hex 319
#define fsioOutputPins15_offset 794
#define fsioOutputPins15_offset_hex 31a
#define fsioOutputPins16_offset 795
#define fsioOutputPins16_offset_hex 31b
#define fsioOutputPins1_offset 780
#define fsioOutputPins1_offset_hex 30c
#define fsioOutputPins2_offset 781
#define fsioOutputPins2_offset_hex 30d
#define fsioOutputPins3_offset 782
#define fsioOutputPins3_offset_hex 30e
#define fsioOutputPins4_offset 783
#define fsioOutputPins4_offset_hex 30f
#define fsioOutputPins5_offset 784
#define fsioOutputPins5_offset_hex 310
#define fsioOutputPins6_offset 785
#define fsioOutputPins6_offset_hex 311
#define fsioOutputPins7_offset 786
#define fsioOutputPins7_offset_hex 312
#define fsioOutputPins8_offset 787
#define fsioOutputPins8_offset_hex 313
#define fsioOutputPins9_offset 788
#define fsioOutputPins9_offset_hex 314
#define fsioTable1_offset 19296
#define fsioTable1_offset_hex 4b60
#define fsioTable1LoadBins_offset 19552
#define fsioTable1LoadBins_offset_hex 4c60
#define fsioTable1RpmBins_offset 19584
#define fsioTable1RpmBins_offset_hex 4c80
#define fsioTable2_offset 19616
#define fsioTable2_offset_hex 4ca0
#define fsioTable2LoadBins_offset 19680
#define fsioTable2LoadBins_offset_hex 4ce0
#define fsioTable2RpmBins_offset 19712
#define fsioTable2RpmBins_offset_hex 4d00
#define fsioTable3_offset 19744
#define fsioTable3_offset_hex 4d20
#define fsioTable3LoadBins_offset 19808
#define fsioTable3LoadBins_offset_hex 4d60
#define fsioTable3RpmBins_offset 19840
#define fsioTable3RpmBins_offset_hex 4d80
#define fsioTable4_offset 19872
#define fsioTable4_offset_hex 4da0
#define fsioTable4LoadBins_offset 19936
#define fsioTable4LoadBins_offset_hex 4de0
#define fsioTable4RpmBins_offset 19968
#define fsioTable4RpmBins_offset_hex 4e00
#define FUEL_LOAD_COUNT 16
#define FUEL_RPM_COUNT 16
#define fuelAlgorithm_offset 420
#define fuelAlgorithm_offset_hex 1a4
#define fuelClosedLoopAfrHighThreshold_offset 2536
#define fuelClosedLoopAfrHighThreshold_offset_hex 9e8
#define fuelClosedLoopAfrLowThreshold_offset 696
#define fuelClosedLoopAfrLowThreshold_offset_hex 2b8
#define fuelClosedLoopCltThreshold_offset 2508
#define fuelClosedLoopCltThreshold_offset_hex 9cc
#define fuelClosedLoopCorrectionEnabled_offset 1464
#define fuelClosedLoopCorrectionEnabled_offset_hex 5b8
#define fuelClosedLoopPid_dFactor_offset 2524
#define fuelClosedLoopPid_dFactor_offset_hex 9dc
#define fuelClosedLoopPid_iFactor_offset 2520
#define fuelClosedLoopPid_iFactor_offset_hex 9d8
#define fuelClosedLoopPid_maxValue_offset 2534
#define fuelClosedLoopPid_maxValue_offset_hex 9e6
#define fuelClosedLoopPid_minValue_offset 2532
#define fuelClosedLoopPid_minValue_offset_hex 9e4
#define fuelClosedLoopPid_offset 2516
#define fuelClosedLoopPid_offset_hex 9d4
#define fuelClosedLoopPid_offset_offset 2528
#define fuelClosedLoopPid_offset_offset_hex 9e0
#define fuelClosedLoopPid_periodMs_offset 2530
#define fuelClosedLoopPid_periodMs_offset_hex 9e2
#define fuelClosedLoopPid_pFactor_offset 2516
#define fuelClosedLoopPid_pFactor_offset_hex 9d4
#define fuelClosedLoopRpmThreshold_offset 2512
#define fuelClosedLoopRpmThreshold_offset_hex 9d0
#define fuelClosedLoopTpsThreshold_offset 2510
#define fuelClosedLoopTpsThreshold_offset_hex 9ce
#define fuelLevelEmptyTankVoltage_offset 684
#define fuelLevelEmptyTankVoltage_offset_hex 2ac
#define fuelLevelFullTankVoltage_offset 688
#define fuelLevelFullTankVoltage_offset_hex 2b0
#define fuelLevelSensor_offset 514
#define fuelLevelSensor_offset_hex 202
#define fuelLoadBins_offset 16160
#define fuelLoadBins_offset_hex 3f20
#define fuelPumpPin_offset 658
#define fuelPumpPin_offset_hex 292
#define fuelPumpPinMode_offset 659
#define fuelPumpPinMode_offset_hex 293
#define fuelRailPressure_offset 1756
#define fuelRailPressure_offset_hex 6dc
#define fuelRpmBins_offset 16224
#define fuelRpmBins_offset_hex 3f60
#define fuelTable_offset 15136
#define fuelTable_offset_hex 3b20
#define GAUGE_COIL_DWELL_TIME "dwell: coil charge time"
#define GAUGE_NAME_ACCEL_X "Acceleration: X"
#define GAUGE_NAME_ACCEL_Y "Acceleration: Y"
#define GAUGE_NAME_ACCEL_Z "Acceleration: Z"
#define GAUGE_NAME_AFR "Air/Fuel Ratio"
#define GAUGE_NAME_AIR_FLOW "air flow"
#define GAUGE_NAME_AIR_MASS "air mass"
#define GAUGE_NAME_CPU_TEMP "CPU Temperature"
#define GAUGE_NAME_DEBUG_F1 "debug f1"
#define GAUGE_NAME_DEBUG_F2 "debug f2: iTerm"
#define GAUGE_NAME_DEBUG_F3 "debug f3: prevError"
#define GAUGE_NAME_DEBUG_F4 "debug f4: iParam"
#define GAUGE_NAME_DEBUG_F5 "debug f5: dParam"
#define GAUGE_NAME_DEBUG_F6 "debug f6: dTerm"
#define GAUGE_NAME_DEBUG_F7 "debug f7"
#define GAUGE_NAME_DEBUG_I1 "debug i1: pParam"
#define GAUGE_NAME_DEBUG_I2 "debug i2: offset"
#define GAUGE_NAME_DEBUG_I3 "debug i3"
#define GAUGE_NAME_DEBUG_I4 "debug i4"
#define GAUGE_NAME_DEBUG_I5 "debug i5"
#define GAUGE_NAME_DWELL_DUTY "dwell: coil duty cycle"
#define GAUGE_NAME_ECU_TEMPERATURE "ECU temperature"
#define GAUGE_NAME_ENGINE_LOAD "Engine Load"
#define GAUGE_NAME_ETB_DUTY "ETB duty cycle"
#define GAUGE_NAME_ETB_ERROR "ETB position error"
#define GAUGE_NAME_ETB_TARGET "ETB position target"
#define GAUGE_NAME_FUEL_BARO_CORR "fuel: Barometric pressure correction"
#define GAUGE_NAME_FUEL_BASE "fuel: base"
#define GAUGE_NAME_FUEL_CHARGE_TEMP "fuel: Estimated charge temperature"
#define GAUGE_NAME_FUEL_CLT_CORR "fuel: CLT correction"
#define GAUGE_NAME_FUEL_CRANKING "fuel: cranking"
#define GAUGE_NAME_FUEL_EL_EXTRA "fuel: engine load acceleration extra fuel"
#define GAUGE_NAME_FUEL_IAT_CORR "fuel: IAT correction"
#define GAUGE_NAME_FUEL_INJ_DUTY "fuel: injector duty cycle"
#define GAUGE_NAME_FUEL_LAST_INJECTION "fuel: Last injection"
#define GAUGE_NAME_FUEL_PID_CORR "fuel: Short-term closed loop correction"
#define GAUGE_NAME_FUEL_RUNNING "fuel: running"
#define GAUGE_NAME_FUEL_TPS_EXTRA "fuel: TPS acceleration extra fuel ms"
#define GAUGE_NAME_FUEL_TPS_ROC "fuel: TPS change"
#define GAUGE_NAME_FUEL_VE "fuel: VE"
#define GAUGE_NAME_FUEL_WALL_AMOUNT "fuel: wall amount"
#define GAUGE_NAME_FUEL_WALL_CORRECTION "fuel: wall corr ms"
#define GAUGE_NAME_IAC "Idle Air Valve"
#define GAUGE_NAME_INJECTOR_LAG "fuel: injector lag"
#define GAUGE_NAME_KNOCK_COUNTER "knock: counter"
#define GAUGE_NAME_KNOCK_LEVEL "knock: current level"
#define GAUGE_NAME_TARGET_AFR "fuel: target AFR"
#define GAUGE_NAME_TCHARGE "fuel: SD tCharge"
#define GAUGE_NAME_THROTTLE_PEDAL "Throttle pedal position"
#define GAUGE_NAME_TIMING_ADVANCE "timing"
#define GAUGE_NAME_VBAT "VBatt"
#define GAUGE_NAME_VERSION "firmware"
#define GAUGE_NAME_VVS "Vehicle Speed"
#define GAUGE_NAME_VVT "VVT position"
#define GAUGE_NAME_WARNING_COUNTER "warning: counter"
#define GAUGE_NAME_WARNING_LAST "warning: last"
#define generalPeriodicThreadPeriodMs_offset 724
#define generalPeriodicThreadPeriodMs_offset_hex 2d4
#define globalFuelCorrection_offset 544
#define globalFuelCorrection_offset_hex 220
#define globalTriggerAngleOffset_offset 456
#define globalTriggerAngleOffset_offset_hex 1c8
#define gpioPinModes10_offset 773
#define gpioPinModes10_offset_hex 305
#define gpioPinModes11_offset 774
#define gpioPinModes11_offset_hex 306
#define gpioPinModes12_offset 775
#define gpioPinModes12_offset_hex 307
#define gpioPinModes13_offset 776
#define gpioPinModes13_offset_hex 308
#define gpioPinModes14_offset 777
#define gpioPinModes14_offset_hex 309
#define gpioPinModes15_offset 778
#define gpioPinModes15_offset_hex 30a
#define gpioPinModes16_offset 779
#define gpioPinModes16_offset_hex 30b
#define gpioPinModes1_offset 764
#define gpioPinModes1_offset_hex 2fc
#define gpioPinModes2_offset 765
#define gpioPinModes2_offset_hex 2fd
#define gpioPinModes3_offset 766
#define gpioPinModes3_offset_hex 2fe
#define gpioPinModes4_offset 767
#define gpioPinModes4_offset_hex 2ff
#define gpioPinModes5_offset 768
#define gpioPinModes5_offset_hex 300
#define gpioPinModes6_offset 769
#define gpioPinModes6_offset_hex 301
#define gpioPinModes7_offset 770
#define gpioPinModes7_offset_hex 302
#define gpioPinModes8_offset 771
#define gpioPinModes8_offset_hex 303
#define gpioPinModes9_offset 772
#define gpioPinModes9_offset_hex 304
#define gps_rx_pin_offset 656
#define gps_rx_pin_offset_hex 290
#define gps_tx_pin_offset 657
#define gps_tx_pin_offset_hex 291
#define hasFrequencyReportingMapSensor_offset 1464
#define hasFrequencyReportingMapSensor_offset_hex 5b8
#define HD44780_db4_offset 652
#define HD44780_db4_offset_hex 28c
#define HD44780_db5_offset 653
#define HD44780_db5_offset_hex 28d
#define HD44780_db6_offset 654
#define HD44780_db6_offset_hex 28e
#define HD44780_db7_offset 655
#define HD44780_db7_offset_hex 28f
#define HD44780_e_offset 651
#define HD44780_e_offset_hex 28b
#define HD44780_rs_offset 650
#define HD44780_rs_offset_hex 28a
#define HD44780height_offset 508
#define HD44780height_offset_hex 1fc
#define HD44780width_offset 504
#define HD44780width_offset_hex 1f8
#define high_fuel_pressure_sensor_1_offset 541
#define high_fuel_pressure_sensor_1_offset_hex 21d
#define high_fuel_pressure_sensor_2_offset 542
#define high_fuel_pressure_sensor_2_offset_hex 21e
#define hip9011CsPin_offset 753
#define hip9011CsPin_offset_hex 2f1
#define hip9011CsPinMode_offset 703
#define hip9011CsPinMode_offset_hex 2bf
#define hip9011Gain_offset 3952
#define hip9011Gain_offset_hex f70
#define hip9011IntHoldPin_offset 754
#define hip9011IntHoldPin_offset_hex 2f2
#define hip9011IntHoldPinMode_offset 755
#define hip9011IntHoldPinMode_offset_hex 2f3
#define hip9011PrescalerAndSDO_offset 324
#define hip9011PrescalerAndSDO_offset_hex 144
#define hip9011SpiDevice_offset 540
#define hip9011SpiDevice_offset_hex 21c
#define hipOutputChannel_offset 1468
#define hipOutputChannel_offset_hex 5bc
#define HW_MAX_ADC_INDEX 17
#define IAC_PID_MULT_SIZE 8
#define iacByTpsTaper_offset 2038
#define iacByTpsTaper_offset_hex 7f6
#define iacCoasting_offset 3224
#define iacCoasting_offset_hex c98
#define iacCoastingBins_offset 3160
#define iacCoastingBins_offset_hex c58
#define iacPidMultLoadBins_offset 4124
#define iacPidMultLoadBins_offset_hex 101c
#define iacPidMultRpmBins_offset 4132
#define iacPidMultRpmBins_offset_hex 1024
#define iacPidMultTable_offset 4060
#define iacPidMultTable_offset_hex fdc
#define iat_adcChannel_offset 312
#define iat_adcChannel_offset_hex 138
#define iat_alignmentFill_offset 313
#define iat_alignmentFill_offset_hex 139
#define iat_bias_resistor_offset 308
#define iat_bias_resistor_offset_hex 134
#define iat_config_offset 284
#define iat_config_offset_hex 11c
#define IAT_CURVE_SIZE 16
#define iat_offset 284
#define iat_offset_hex 11c
#define iat_resistance_1_offset 296
#define iat_resistance_1_offset_hex 128
#define iat_resistance_2_offset 300
#define iat_resistance_2_offset_hex 12c
#define iat_resistance_3_offset 304
#define iat_resistance_3_offset_hex 130
#define iat_tempC_1_offset 284
#define iat_tempC_1_offset_hex 11c
#define iat_tempC_2_offset 288
#define iat_tempC_2_offset_hex 120
#define iat_tempC_3_offset 292
#define iat_tempC_3_offset_hex 124
#define iatFuelCorr_offset 10464
#define iatFuelCorr_offset_hex 28e0
#define iatFuelCorrBins_offset 10400
#define iatFuelCorrBins_offset_hex 28a0
#define IDLE_ADVANCE_CURVE_SIZE 8
#define idle_offset 600
#define idle_offset_hex 258
#define idle_solenoidFrequency_offset 600
#define idle_solenoidFrequency_offset_hex 258
#define idle_solenoidPin_offset 604
#define idle_solenoidPin_offset_hex 25c
#define idle_solenoidPinMode_offset 607
#define idle_solenoidPinMode_offset_hex 25f
#define idle_stepperDirectionPin_offset 605
#define idle_stepperDirectionPin_offset_hex 25d
#define idle_stepperStepPin_offset 606
#define idle_stepperStepPin_offset_hex 25e
#define IDLE_VE_CURVE_SIZE 8
#define idleAdvance_offset 6576
#define idleAdvance_offset_hex 19b0
#define idleAdvanceBins_offset 6544
#define idleAdvanceBins_offset_hex 1990
#define idleIncrementalPidCic_offset 744
#define idleIncrementalPidCic_offset_hex 2e8
#define idleMode_offset 1472
#define idleMode_offset_hex 5c0
#define idlePidActivationTime_offset 2588
#define idlePidActivationTime_offset_hex a1c
#define idlePidDeactivationTpsThreshold_offset 816
#define idlePidDeactivationTpsThreshold_offset_hex 330
#define idlePidFalloffDeltaRpm_offset 3992
#define idlePidFalloffDeltaRpm_offset_hex f98
#define idlePidRpmDeadZone_offset 1894
#define idlePidRpmDeadZone_offset_hex 766
#define idlePidRpmUpperLimit_offset 1484
#define idlePidRpmUpperLimit_offset_hex 5cc
#define idleRpmPid2_dFactor_offset 4048
#define idleRpmPid2_dFactor_offset_hex fd0
#define idleRpmPid2_iFactor_offset 4044
#define idleRpmPid2_iFactor_offset_hex fcc
#define idleRpmPid2_maxValue_offset 4058
#define idleRpmPid2_maxValue_offset_hex fda
#define idleRpmPid2_minValue_offset 4056
#define idleRpmPid2_minValue_offset_hex fd8
#define idleRpmPid2_offset 4040
#define idleRpmPid2_offset_hex fc8
#define idleRpmPid2_offset_offset 4052
#define idleRpmPid2_offset_offset_hex fd4
#define idleRpmPid2_periodMs_offset 4054
#define idleRpmPid2_periodMs_offset_hex fd6
#define idleRpmPid2_pFactor_offset 4040
#define idleRpmPid2_pFactor_offset_hex fc8
#define idleRpmPid_dFactor_offset 1796
#define idleRpmPid_dFactor_offset_hex 704
#define idleRpmPid_iFactor_offset 1792
#define idleRpmPid_iFactor_offset_hex 700
#define idlerpmpid_iTermMax_offset 4006
#define idlerpmpid_iTermMax_offset_hex fa6
#define idlerpmpid_iTermMin_offset 4002
#define idlerpmpid_iTermMin_offset_hex fa2
#define idleRpmPid_maxValue_offset 1806
#define idleRpmPid_maxValue_offset_hex 70e
#define idleRpmPid_minValue_offset 1804
#define idleRpmPid_minValue_offset_hex 70c
#define idleRpmPid_offset 1788
#define idleRpmPid_offset_hex 6fc
#define idleRpmPid_offset_offset 1800
#define idleRpmPid_offset_offset_hex 708
#define idleRpmPid_periodMs_offset 1802
#define idleRpmPid_periodMs_offset_hex 70a
#define idleRpmPid_pFactor_offset 1788
#define idleRpmPid_pFactor_offset_hex 6fc
#define idleStepperReactionTime_offset 1508
#define idleStepperReactionTime_offset_hex 5e4
#define idleStepperTotalSteps_offset 1532
#define idleStepperTotalSteps_offset_hex 5fc
#define idleThreadPeriodMs_offset 712
#define idleThreadPeriodMs_offset_hex 2c8
#define idleTimingPid_dFactor_offset 3976
#define idleTimingPid_dFactor_offset_hex f88
#define idleTimingPid_iFactor_offset 3972
#define idleTimingPid_iFactor_offset_hex f84
#define idleTimingPid_maxValue_offset 3986
#define idleTimingPid_maxValue_offset_hex f92
#define idleTimingPid_minValue_offset 3984
#define idleTimingPid_minValue_offset_hex f90
#define idleTimingPid_offset 3968
#define idleTimingPid_offset_hex f80
#define idleTimingPid_offset_offset 3980
#define idleTimingPid_offset_offset_hex f8c
#define idleTimingPid_periodMs_offset 3982
#define idleTimingPid_periodMs_offset_hex f8e
#define idleTimingPid_pFactor_offset 3968
#define idleTimingPid_pFactor_offset_hex f80
#define idleTimingPidDeadZone_offset 3990
#define idleTimingPidDeadZone_offset_hex f96
#define idleTimingPidWorkZone_offset 3988
#define idleTimingPidWorkZone_offset_hex f94
#define idleVe_offset 6640
#define idleVe_offset_hex 19f0
#define idleVeBins_offset 6608
#define idleVeBins_offset_hex 19d0
#define IGN_LOAD_COUNT 16
#define IGN_RPM_COUNT 16
#define IGN_TPS_COUNT 16
#define IGNITION_PIN_COUNT 12
#define ignitionDwellForCrankingMs_offset 100
#define ignitionDwellForCrankingMs_offset_hex 64
#define ignitionIatCorrLoadBins_offset 13856
#define ignitionIatCorrLoadBins_offset_hex 3620
#define ignitionIatCorrRpmBins_offset 13920
#define ignitionIatCorrRpmBins_offset_hex 3660
#define ignitionIatCorrTable_offset 12832
#define ignitionIatCorrTable_offset_hex 3220
#define ignitionLoadBins_offset 17312
#define ignitionLoadBins_offset_hex 43a0
#define ignitionMode_offset 440
#define ignitionMode_offset_hex 1b8
#define ignitionOffset_offset 444
#define ignitionOffset_offset_hex 1bc
#define ignitionPin10logic 
#define ignitionPin11logic 
#define ignitionPin12logic 
#define ignitionPin2logic || ((firingOrder == 2) || (firingOrder == 7) || (firingOrder == 9) || (firingOrder == 11) || (firingOrder == 12))
#define ignitionPin3logic || ((firingOrder == 1) || (firingOrder == 3) || (firingOrder == 4) || (firingOrder == 5) || (firingOrder == 9))
#define ignitionPin4logic || ((firingOrder == 5) || (firingOrder == 7) || (firingOrder == 12))
#define ignitionPin5logic || ((firingOrder == 4) || (firingOrder == 12))
#define ignitionPin6logic 
#define ignitionPin7logic || ((firingOrder == 11))
#define ignitionPin8logic || ((firingOrder == 5) || (firingOrder == 11))
#define ignitionPin9logic 
#define ignitionPinMode_offset 649
#define ignitionPinMode_offset_hex 289
#define ignitionPins10_offset 645
#define ignitionPins10_offset_hex 285
#define ignitionPins11_offset 646
#define ignitionPins11_offset_hex 286
#define ignitionPins12_offset 647
#define ignitionPins12_offset_hex 287
#define ignitionPins1_offset 636
#define ignitionPins1_offset_hex 27c
#define ignitionPins2_offset 637
#define ignitionPins2_offset_hex 27d
#define ignitionPins3_offset 638
#define ignitionPins3_offset_hex 27e
#define ignitionPins4_offset 639
#define ignitionPins4_offset_hex 27f
#define ignitionPins5_offset 640
#define ignitionPins5_offset_hex 280
#define ignitionPins6_offset 641
#define ignitionPins6_offset_hex 281
#define ignitionPins7_offset 642
#define ignitionPins7_offset_hex 282
#define ignitionPins8_offset 643
#define ignitionPins8_offset_hex 283
#define ignitionPins9_offset 644
#define ignitionPins9_offset_hex 284
#define ignitionRpmBins_offset 17376
#define ignitionRpmBins_offset_hex 43e0
#define ignitionTable_offset 16288
#define ignitionTable_offset_hex 3fa0
#define ignitionTpsBins_offset 3800
#define ignitionTpsBins_offset_hex ed8
#define ignitionTpsTable_offset 3288
#define ignitionTpsTable_offset_hex cd8
#define ignMathCalculateAtIndex_offset 1488
#define ignMathCalculateAtIndex_offset_hex 5d0
#define INDICATOR_NAME_AC_SWITCH "AC switch"
#define INDICATOR_NAME_BRAKE_DOWN "brake: down"
#define INDICATOR_NAME_CLUTCH_DOWN "clutch: down"
#define INDICATOR_NAME_CLUTCH_UP "clutch: up"
#define INJECTION_PIN_COUNT 12
#define injectionMode_offset 428
#define injectionMode_offset_hex 1ac
#define injectionPhase_offset 13984
#define injectionPhase_offset_hex 36a0
#define injectionPinMode_offset 648
#define injectionPinMode_offset_hex 288
#define injectionPins10_offset 633
#define injectionPins10_offset_hex 279
#define injectionPins11_offset 634
#define injectionPins11_offset_hex 27a
#define injectionPins12_offset 635
#define injectionPins12_offset_hex 27b
#define injectionPins1_offset 624
#define injectionPins1_offset_hex 270
#define injectionPins2_offset 625
#define injectionPins2_offset_hex 271
#define injectionPins3_offset 626
#define injectionPins3_offset_hex 272
#define injectionPins4_offset 627
#define injectionPins4_offset_hex 273
#define injectionPins5_offset 628
#define injectionPins5_offset_hex 274
#define injectionPins6_offset 629
#define injectionPins6_offset_hex 275
#define injectionPins7_offset 630
#define injectionPins7_offset_hex 276
#define injectionPins8_offset 631
#define injectionPins8_offset_hex 277
#define injectionPins9_offset 632
#define injectionPins9_offset_hex 278
#define injector_battLagCorr_offset 44
#define injector_battLagCorr_offset_hex 2c
#define injector_battLagCorrBins_offset 12
#define injector_battLagCorrBins_offset_hex c
#define injector_flow_offset 8
#define injector_flow_offset_hex 8
#define injector_offset 8
#define injector_offset_hex 8
#define injPhaseLoadBins_offset 15008
#define injPhaseLoadBins_offset_hex 3aa0
#define injPhaseRpmBins_offset 15072
#define injPhaseRpmBins_offset_hex 3ae0
#define invertPrimaryTriggerSignal_offset 1464
#define invertPrimaryTriggerSignal_offset_hex 5b8
#define invertSecondaryTriggerSignal_offset 1464
#define invertSecondaryTriggerSignal_offset_hex 5b8
#define is_enabled_spi_1_offset 744
#define is_enabled_spi_1_offset_hex 2e8
#define is_enabled_spi_2_offset 744
#define is_enabled_spi_2_offset_hex 2e8
#define is_enabled_spi_3_offset 744
#define is_enabled_spi_3_offset_hex 2e8
#define is_enabled_spi_4_offset 744
#define is_enabled_spi_4_offset_hex 2e8
#define isAlternatorControlEnabled_offset 1464
#define isAlternatorControlEnabled_offset_hex 5b8
#define isCJ125Enabled_offset 744
#define isCJ125Enabled_offset_hex 2e8
#define isCylinderCleanupEnabled_offset 1476
#define isCylinderCleanupEnabled_offset_hex 5c4
#define isEngineChartEnabled_offset 1464
#define isEngineChartEnabled_offset_hex 5b8
#define isEngineControlEnabled_offset 744
#define isEngineControlEnabled_offset_hex 2e8
#define isFastAdcEnabled_offset 744
#define isFastAdcEnabled_offset_hex 2e8
#define isFasterEngineSpinUpEnabled_offset 744
#define isFasterEngineSpinUpEnabled_offset_hex 2e8
#define isHip9011Enabled_offset 744
#define isHip9011Enabled_offset_hex 2e8
#define isIgnitionEnabled_offset 1476
#define isIgnitionEnabled_offset_hex 5c4
#define isInjectionEnabled_offset 1476
#define isInjectionEnabled_offset_hex 5c4
#define isManualSpinningMode_offset 1476
#define isManualSpinningMode_offset_hex 5c4
#define isMapAveragingEnabled_offset 1476
#define isMapAveragingEnabled_offset_hex 5c4
#define isSdCardEnabled_offset 744
#define isSdCardEnabled_offset_hex 2e8
#define issue_294_21_offset 76
#define issue_294_21_offset_hex 4c
#define issue_294_22_offset 76
#define issue_294_22_offset_hex 4c
#define issue_294_23_offset 76
#define issue_294_23_offset_hex 4c
#define issue_294_24_offset 76
#define issue_294_24_offset_hex 4c
#define issue_294_25_offset 76
#define issue_294_25_offset_hex 4c
#define issue_294_26_offset 76
#define issue_294_26_offset_hex 4c
#define issue_294_27_offset 76
#define issue_294_27_offset_hex 4c
#define issue_294_28_offset 76
#define issue_294_28_offset_hex 4c
#define issue_294_29_offset 76
#define issue_294_29_offset_hex 4c
#define issue_294_30_offset 76
#define issue_294_30_offset_hex 4c
#define issue_294_31_offset 76
#define issue_294_31_offset_hex 4c
#define isTunerStudioEnabled_offset 1476
#define isTunerStudioEnabled_offset_hex 5c4
#define isVerboseAlternator_offset 744
#define isVerboseAlternator_offset_hex 2e8
#define isVerboseAuxPid1_offset 76
#define isVerboseAuxPid1_offset_hex 4c
#define isVerboseAuxPid2_offset 76
#define isVerboseAuxPid2_offset_hex 4c
#define isVerboseAuxPid3_offset 76
#define isVerboseAuxPid3_offset_hex 4c
#define isVerboseAuxPid4_offset 76
#define isVerboseAuxPid4_offset_hex 4c
#define isVerboseETB_offset 1464
#define isVerboseETB_offset_hex 5b8
#define isVerboseIAC_offset 1464
#define isVerboseIAC_offset_hex 5b8
#define isWaveAnalyzerEnabled_offset 1476
#define isWaveAnalyzerEnabled_offset_hex 5c4
#define joystickAPin_offset 939
#define joystickAPin_offset_hex 3ab
#define joystickBPin_offset 940
#define joystickBPin_offset_hex 3ac
#define joystickCenterPin_offset 938
#define joystickCenterPin_offset_hex 3aa
#define joystickCPin_offset 941
#define joystickCPin_offset_hex 3ad
#define joystickDPin_offset 942
#define joystickDPin_offset_hex 3ae
#define knockBandCustom_offset 328
#define knockBandCustom_offset_hex 148
#define knockDetectionWindowEnd_offset 1504
#define knockDetectionWindowEnd_offset_hex 5e0
#define knockDetectionWindowStart_offset 1500
#define knockDetectionWindowStart_offset_hex 5dc
#define knockNoise_offset 1820
#define knockNoise_offset_hex 71c
#define knockNoiseRpmBins_offset 1852
#define knockNoiseRpmBins_offset_hex 73c
#define knockVThreshold_offset 1512
#define knockVThreshold_offset_hex 5e8
#define lcdThreadPeriodMs_offset 720
#define lcdThreadPeriodMs_offset_hex 2d0
#define LDS_ALTERNATOR_PID_STATE_INDEX 9
#define LDS_CJ125_PID_STATE_INDEX 10
#define LDS_CLT_STATE_INDEX 0
#define LDS_ENGINE_STATE_INDEX 3
#define LDS_ETB_PID_STATE_INDEX 7
#define LDS_FUEL_TRIM_STATE_INDEX 4
#define LDS_IAT_STATE_INDEX 1
#define LDS_IDLE_PID_STATE_INDEX 8
#define LDS_SPEED_DENSITY_STATE_INDEX 2
#define LDS_TPS_TPS_ENEICHMENT_STATE_INDEX 5
#define LDS_TRIGGER_CENTRAL_STATE_INDEX 6
#define LDS_TRIGGER_STATE_STATE_INDEX 11
#define LE_COMMAND_LENGTH 200
#define LIS302DLCsPin_offset 2043
#define LIS302DLCsPin_offset_hex 7fb
#define logFormat_offset 496
#define logFormat_offset_hex 1f0
#define LOGIC_ANALYZER_CHANNEL_COUNT 4
#define logicAnalyzerMode_offset 756
#define logicAnalyzerMode_offset_hex 2f4
#define logicAnalyzerPins1_offset 748
#define logicAnalyzerPins1_offset_hex 2ec
#define logicAnalyzerPins2_offset 749
#define logicAnalyzerPins2_offset_hex 2ed
#define logicAnalyzerPins3_offset 750
#define logicAnalyzerPins3_offset_hex 2ee
#define logicAnalyzerPins4_offset 751
#define logicAnalyzerPins4_offset_hex 2ef
#define MAF_DECODING_COUNT 256
#define maf_sensor_type_e_enum "v0", "v1", "v2", "v3"
#define mafAdcChannel_offset 543
#define mafAdcChannel_offset_hex 21f
#define mafDecoding_offset 10784
#define mafDecoding_offset_hex 2a20
#define mafDecodingBins_offset 11808
#define mafDecodingBins_offset_hex 2e20
#define mafSensorType_offset 948
#define mafSensorType_offset_hex 3b4
#define mainRelayPin_offset 706
#define mainRelayPin_offset_hex 2c2
#define mainRelayPinMode_offset 752
#define mainRelayPinMode_offset_hex 2f0
#define mainUnusedEnd_offset 4140
#define mainUnusedEnd_offset_hex 102c
#define malfunctionIndicatorPin_offset 660
#define malfunctionIndicatorPin_offset_hex 294
#define malfunctionIndicatorPinMode_offset 661
#define malfunctionIndicatorPinMode_offset_hex 295
#define manIdlePosition_offset 608
#define manIdlePosition_offset_hex 260
#define MAP_ACCEL_TAPER 8
#define MAP_ANGLE_SIZE 8
#define map_offset 108
#define map_offset_hex 6c
#define map_samplingAngle_offset 140
#define map_samplingAngle_offset_hex 8c
#define map_samplingAngleBins_offset 108
#define map_samplingAngleBins_offset_hex 6c
#define map_samplingWindow_offset 204
#define map_samplingWindow_offset_hex cc
#define map_samplingWindowBins_offset 172
#define map_samplingWindowBins_offset_hex ac
#define map_sensor_align_offset 249
#define map_sensor_align_offset_hex f9
#define map_sensor_highValue_offset 240
#define map_sensor_highValue_offset_hex f0
#define map_sensor_hwChannel_offset 248
#define map_sensor_hwChannel_offset_hex f8
#define map_sensor_lowValue_offset 236
#define map_sensor_lowValue_offset_hex ec
#define map_sensor_offset 236
#define map_sensor_offset_hex ec
#define map_sensor_type_offset 244
#define map_sensor_type_offset_hex f4
#define MAP_WINDOW_SIZE 8
#define mapAccelTaperBins_offset 2136
#define mapAccelTaperBins_offset_hex 858
#define mapAccelTaperMult_offset 2168
#define mapAccelTaperMult_offset_hex 878
#define mapAveragingSchedulingAtIndex_offset 1540
#define mapAveragingSchedulingAtIndex_offset_hex 604
#define mapErrorDetectionTooHigh_offset 1780
#define mapErrorDetectionTooHigh_offset_hex 6f4
#define mapErrorDetectionTooLow_offset 1776
#define mapErrorDetectionTooLow_offset_hex 6f0
#define mapFrequency0Kpa_offset 612
#define mapFrequency0Kpa_offset_hex 264
#define mapFrequency100Kpa_offset 616
#define mapFrequency100Kpa_offset_hex 268
#define mapHighValueVoltage_offset 2212
#define mapHighValueVoltage_offset_hex 8a4
#define mapLowValueVoltage_offset 2208
#define mapLowValueVoltage_offset_hex 8a0
#define mapMinBufferLength_offset 812
#define mapMinBufferLength_offset_hex 32c
#define mass_storage_e_enum "Auto", "Always", "Never"
#define max31855_cs1_offset 796
#define max31855_cs1_offset_hex 31c
#define max31855_cs2_offset 797
#define max31855_cs2_offset_hex 31d
#define max31855_cs3_offset 798
#define max31855_cs3_offset_hex 31e
#define max31855_cs4_offset 799
#define max31855_cs4_offset_hex 31f
#define max31855_cs5_offset 800
#define max31855_cs5_offset_hex 320
#define max31855_cs6_offset 801
#define max31855_cs6_offset_hex 321
#define max31855_cs7_offset 802
#define max31855_cs7_offset_hex 322
#define max31855_cs8_offset 803
#define max31855_cs8_offset_hex 323
#define max31855spiDevice_offset 675
#define max31855spiDevice_offset_hex 2a3
#define maxKnockSubDeg_offset 552
#define maxKnockSubDeg_offset_hex 228
#define mc33816_cs_offset 3107
#define mc33816_cs_offset_hex c23
#define mc33816_driven_offset 2607
#define mc33816_driven_offset_hex a2f
#define mc33816_rstb_offset 2606
#define mc33816_rstb_offset_hex a2e
#define mc33816spiDevice_offset 4001
#define mc33816spiDevice_offset_hex fa1
#define mc33972_cs_offset 678
#define mc33972_cs_offset_hex 2a6
#define mc33972_csPinMode_offset 679
#define mc33972_csPinMode_offset_hex 2a7
#define mc33972spiDevice_offset 4008
#define mc33972spiDevice_offset_hex fa8
#define measureMapOnlyInOneCylinder_offset 744
#define measureMapOnlyInOneCylinder_offset_hex 2e8
#define miataNb2VVTRatioFrom_offset 820
#define miataNb2VVTRatioFrom_offset_hex 334
#define miataNb2VVTRatioTo_offset 824
#define miataNb2VVTRatioTo_offset_hex 338
#define MOCK_AFR_COMMAND "mock_afr_voltage"
#define MOCK_CLT_COMMAND "mock_clt_voltage"
#define MOCK_IAT_COMMAND "mock_iat_voltage"
#define MOCK_MAF_COMMAND "mock_maf_voltage"
#define MOCK_MAP_COMMAND "mock_map_voltage"
#define MOCK_PPS_POSITION_COMMAND "mock_pps_position"
#define MOCK_PPS_VOLTAGE_COMMAND "mock_pps_voltage"
#define MOCK_TPS_COMMAND "mock_tps_voltage"
#define NARROW_BAND_WIDE_BAND_CONVERSION_SIZE 8
#define narrowToWideOxygen_offset 2296
#define narrowToWideOxygen_offset_hex 8f8
#define narrowToWideOxygenBins_offset 2264
#define narrowToWideOxygenBins_offset_hex 8d8
#define nbVvtIndex_offset 2416
#define nbVvtIndex_offset_hex 970
#define needSecondTriggerInputDeprecated_offset 1476
#define needSecondTriggerInputDeprecated_offset_hex 5c4
#define noAccelAfterHardLimitPeriodSecs_offset 1536
#define noAccelAfterHardLimitPeriodSecs_offset_hex 600
#define o2heaterPin_offset 742
#define o2heaterPin_offset_hex 2e6
#define o2heaterPinModeTodO_offset 743
#define o2heaterPinModeTodO_offset_hex 2e7
#define oilPressure_align_offset 2693
#define oilPressure_align_offset_hex a85
#define oilPressure_hwChannel_offset 2692
#define oilPressure_hwChannel_offset_hex a84
#define oilPressure_offset 2692
#define oilPressure_offset_hex a84
#define oilPressure_v1_offset 2696
#define oilPressure_v1_offset_hex a88
#define oilPressure_v2_offset 2704
#define oilPressure_v2_offset_hex a90
#define oilPressure_value1_offset 2700
#define oilPressure_value1_offset_hex a8c
#define oilPressure_value2_offset 2708
#define oilPressure_value2_offset_hex a94
#define onOffAlternatorLogic_offset 744
#define onOffAlternatorLogic_offset_hex 2e8
#define output_pin_e_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "N/A", "N/A", "PA10", "PA11", "PA12", "PA13", "N/A", "N/A", "N/A", "N/A", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "N/A", "N/A", "N/A", "N/A", "PB12", "PB13", "N/A", "N/A", "N/A", "N/A", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "N/A", "N/A", "N/A", "N/A", "PC14", "PC15", "PC16", "PC17", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "PD15", "PD16", "N/A", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "TLE6240_1", "TLE6240_2", "TLE6240_3", "TLE6240_4", "TLE6240_5", "TLE6240_6", "TLE6240_7", "TLE6240_8", "TLE6240_9", "TLE6240_10", "TLE6240_11", "TLE6240_12", "TLE6240_13", "TLE6240_14", "TLE6240_15", "TLE6240_16"
#define overrideCrankingIacSetting_offset 1476
#define overrideCrankingIacSetting_offset_hex 5c4
#define overrideCrankingIgnition_offset 516
#define overrideCrankingIgnition_offset_hex 204
#define pauseEtbControl_offset 744
#define pauseEtbControl_offset_hex 2e8
#define PEDAL_TO_TPS_SIZE 8
#define pedalToTpsPedalBins_offset 6464
#define pedalToTpsPedalBins_offset_hex 1940
#define pedalToTpsRpmBins_offset 6472
#define pedalToTpsRpmBins_offset_hex 1948
#define pedalToTpsTable_offset 6400
#define pedalToTpsTable_offset_hex 1900
#define pidExtraForLowRpm_offset 3156
#define pidExtraForLowRpm_offset_hex c54
#define pin_mode_e_enum "default", "INVALID", "INVALID", "INVALID", "opendrain", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "PULLUP", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "PULLDOWN"
#define pin_output_mode_e_enum "default", "default inverted", "open collector", "open collector inverted"
#define postCrankingDurationSec_offset 2440
#define postCrankingDurationSec_offset_hex 988
#define postCrankingFactor_offset 2436
#define postCrankingFactor_offset_hex 984
#define postCrankingTargetClt_offset 2432
#define postCrankingTargetClt_offset_hex 980
#define primeInjFalloffTemperature_offset 1486
#define primeInjFalloffTemperature_offset_hex 5ce
#define primingSquirtDurationMs_offset 96
#define primingSquirtDurationMs_offset_hex 60
#define PROTOCOL_ANALOG_CHART "analog_chart"
#define PROTOCOL_COIL1_SHORT_NAME "c1"
#define PROTOCOL_CRANK1 "t1"
#define PROTOCOL_CRANK2 "t2"
#define PROTOCOL_CRANK3 "t3"
#define PROTOCOL_DIZZY_NAME "dizzy"
#define PROTOCOL_ENGINE_SNIFFER "wave_chart"
#define PROTOCOL_ES_DOWN "d"
#define PROTOCOL_ES_UP "u"
#define PROTOCOL_HIP_NAME "HIP"
#define PROTOCOL_INJ1_SHORT_NAME "i1"
#define PROTOCOL_OUTPIN "outpin"
#define PROTOCOL_TACH_NAME "tach"
#define PROTOCOL_TEST_RESPONSE_TAG "ts_p_alive"
#define PROTOCOL_VERSION_TAG "rusEfiVersion"
#define PROTOCOL_VVT_NAME "VVT"
#define PROTOCOL_WA_CHANNEL_1 "input1"
#define PROTOCOL_WA_CHANNEL_2 "input2"
#define PROTOCOL_WA_CHANNEL_3 "input3"
#define PROTOCOL_WA_CHANNEL_4 "input4"
#define RPM_1_BYTE_PACKING_MULT 50
#define rpmHardLimit_offset 416
#define rpmHardLimit_offset_hex 1a0
#define runningLedPin_offset 1813
#define runningLedPin_offset_hex 715
#define sdCardCsPin_offset 707
#define sdCardCsPin_offset_hex 2c3
#define sdCardPeriodMs_offset 804
#define sdCardPeriodMs_offset_hex 324
#define sdCardSpiDevice_offset 2592
#define sdCardSpiDevice_offset_hex a20
#define secondTriggerChannelEnabled_offset 1476
#define secondTriggerChannelEnabled_offset_hex 5c4
#define sensor_chart_e_enum "none", "trigger", "MAP", "RPM ACCEL", "DETAILED RPM", "INVALID"
#define sensorChartFrequency_offset 520
#define sensorChartFrequency_offset_hex 208
#define sensorChartMode_offset 944
#define sensorChartMode_offset_hex 3b0
#define sensorSnifferRpmThreshold_offset 412
#define sensorSnifferRpmThreshold_offset_hex 19c
#define SERVO_COUNT 8
#define servoOutputPins1_offset 3140
#define servoOutputPins1_offset_hex c44
#define servoOutputPins2_offset 3141
#define servoOutputPins2_offset_hex c45
#define servoOutputPins3_offset 3142
#define servoOutputPins3_offset_hex c46
#define servoOutputPins4_offset 3143
#define servoOutputPins4_offset_hex c47
#define servoOutputPins5_offset 3144
#define servoOutputPins5_offset_hex c48
#define servoOutputPins6_offset 3145
#define servoOutputPins6_offset_hex c49
#define servoOutputPins7_offset 3146
#define servoOutputPins7_offset_hex c4a
#define servoOutputPins8_offset 3147
#define servoOutputPins8_offset_hex c4b
#define showSdCardWarning_offset 76
#define showSdCardWarning_offset_hex 4c
#define silentTriggerError_offset 1464
#define silentTriggerError_offset_hex 5b8
#define slowAdcAlpha_offset 2088
#define slowAdcAlpha_offset_hex 828
#define sparkDwellRpmBins_offset 332
#define sparkDwellRpmBins_offset_hex 14c
#define sparkDwellValues_offset 364
#define sparkDwellValues_offset_hex 16c
#define specs_offset 396
#define specs_offset_hex 18c
#define spi1MisoMode_offset 2598
#define spi1MisoMode_offset_hex a26
#define spi1misoPin_offset 929
#define spi1misoPin_offset_hex 3a1
#define spi1MosiMode_offset 2597
#define spi1MosiMode_offset_hex a25
#define spi1mosiPin_offset 928
#define spi1mosiPin_offset_hex 3a0
#define spi1SckMode_offset 2596
#define spi1SckMode_offset_hex a24
#define spi1sckPin_offset 930
#define spi1sckPin_offset_hex 3a2
#define spi2MisoMode_offset 2601
#define spi2MisoMode_offset_hex a29
#define spi2misoPin_offset 932
#define spi2misoPin_offset_hex 3a4
#define spi2MosiMode_offset 2600
#define spi2MosiMode_offset_hex a28
#define spi2mosiPin_offset 931
#define spi2mosiPin_offset_hex 3a3
#define spi2SckMode_offset 2599
#define spi2SckMode_offset_hex a27
#define spi2sckPin_offset 933
#define spi2sckPin_offset_hex 3a5
#define spi3MisoMode_offset 2604
#define spi3MisoMode_offset_hex a2c
#define spi3misoPin_offset 935
#define spi3misoPin_offset_hex 3a7
#define spi3MosiMode_offset 2603
#define spi3MosiMode_offset_hex a2b
#define spi3mosiPin_offset 934
#define spi3mosiPin_offset_hex 3a6
#define spi3SckMode_offset 2602
#define spi3SckMode_offset_hex a2a
#define spi3sckPin_offset 936
#define spi3sckPin_offset_hex 3a8
#define starterRelayPin_offset 808
#define starterRelayPin_offset_hex 328
#define starterRelayPinMode_offset 809
#define starterRelayPinMode_offset_hex 329
#define startOfCrankingPrimingPulse_offset 2032
#define startOfCrankingPrimingPulse_offset_hex 7f0
#define startUpFuelPumpDuration_offset 1892
#define startUpFuelPumpDuration_offset_hex 764
#define step1fuelCutEnable_offset 1464
#define step1fuelCutEnable_offset_hex 5b8
#define step1rpm_offset 316
#define step1rpm_offset_hex 13c
#define step1RpmWindow_offset 1784
#define step1RpmWindow_offset_hex 6f8
#define step1SparkCutEnable_offset 1464
#define step1SparkCutEnable_offset_hex 5b8
#define step1timing_offset 320
#define step1timing_offset_hex 140
#define stepperDirectionPinMode_offset 3102
#define stepperDirectionPinMode_offset_hex c1e
#define stepperEnablePin_offset 3104
#define stepperEnablePin_offset_hex c20
#define stepperEnablePinMode_offset 2605
#define stepperEnablePinMode_offset_hex a2d
#define stepperForceParkingEveryRestart_offset 744
#define stepperForceParkingEveryRestart_offset_hex 2e8
#define stepperParkingExtraSteps_offset 818
#define stepperParkingExtraSteps_offset_hex 332
#define storageMode_offset 2260
#define storageMode_offset_hex 8d4
#define switch_input_pin_e_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "N/A", "N/A", "PA10", "PA11", "PA12", "PA13", "N/A", "N/A", "N/A", "N/A", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "N/A", "N/A", "N/A", "N/A", "PB12", "PB13", "N/A", "N/A", "N/A", "N/A", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "N/A", "N/A", "N/A", "N/A", "PC14", "PC15", "PC16", "PC17", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "PD15", "PD16", "N/A", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A"
#define tachOutputPin_offset 704
#define tachOutputPin_offset_hex 2c0
#define tachOutputPinMode_offset 705
#define tachOutputPinMode_offset_hex 2c1
#define tachPulseDuractionMs_offset 1704
#define tachPulseDuractionMs_offset_hex 6a8
#define tachPulseDurationAsDutyCycle_offset 1464
#define tachPulseDurationAsDutyCycle_offset_hex 5b8
#define tachPulseTriggerIndex_offset 1708
#define tachPulseTriggerIndex_offset_hex 6ac
#define targetVBatt_offset 2024
#define targetVBatt_offset_hex 7e8
#define tChargeAirCoefMax_offset 3868
#define tChargeAirCoefMax_offset_hex f1c
#define tChargeAirCoefMin_offset 3864
#define tChargeAirCoefMin_offset_hex f18
#define tChargeAirDecrLimit_offset 3880
#define tChargeAirDecrLimit_offset_hex f28
#define tChargeAirFlowMax_offset 3872
#define tChargeAirFlowMax_offset_hex f20
#define tChargeAirIncrLimit_offset 3876
#define tChargeAirIncrLimit_offset_hex f24
#define tChargeMaxRpmMaxTps_offset 2244
#define tChargeMaxRpmMaxTps_offset_hex 8c4
#define tChargeMaxRpmMinTps_offset 2240
#define tChargeMaxRpmMinTps_offset_hex 8c0
#define tChargeMinRpmMaxTps_offset 2236
#define tChargeMinRpmMaxTps_offset_hex 8bc
#define tChargeMinRpmMinTps_offset 2232
#define tChargeMinRpmMinTps_offset_hex 8b8
#define tChargeMode_e_enum "RPM+TPS (Default)", "Air Mass Interpolation"
#define tChargeMode_offset 3884
#define tChargeMode_offset_hex f2c
#define test557pin_offset 3101
#define test557pin_offset_hex c1d
#define throttlePedalPositionAdcChannel_offset 580
#define throttlePedalPositionAdcChannel_offset_hex 244
#define throttlePedalUpPin_offset 583
#define throttlePedalUpPin_offset_hex 247
#define throttlePedalUpPinMode_offset 710
#define throttlePedalUpPinMode_offset_hex 2c6
#define throttlePedalUpVoltage_offset 1884
#define throttlePedalUpVoltage_offset_hex 75c
#define throttlePedalWOTVoltage_offset 1888
#define throttlePedalWOTVoltage_offset_hex 760
#define timing_offset_cylinder10_offset 2576
#define timing_offset_cylinder10_offset_hex a10
#define timing_offset_cylinder11_offset 2580
#define timing_offset_cylinder11_offset_hex a14
#define timing_offset_cylinder12_offset 2584
#define timing_offset_cylinder12_offset_hex a18
#define timing_offset_cylinder1_offset 2540
#define timing_offset_cylinder1_offset_hex 9ec
#define timing_offset_cylinder2_offset 2544
#define timing_offset_cylinder2_offset_hex 9f0
#define timing_offset_cylinder3_offset 2548
#define timing_offset_cylinder3_offset_hex 9f4
#define timing_offset_cylinder4_offset 2552
#define timing_offset_cylinder4_offset_hex 9f8
#define timing_offset_cylinder5_offset 2556
#define timing_offset_cylinder5_offset_hex 9fc
#define timing_offset_cylinder6_offset 2560
#define timing_offset_cylinder6_offset_hex a00
#define timing_offset_cylinder7_offset 2564
#define timing_offset_cylinder7_offset_hex a04
#define timing_offset_cylinder8_offset 2568
#define timing_offset_cylinder8_offset_hex a08
#define timing_offset_cylinder9_offset 2572
#define timing_offset_cylinder9_offset_hex a0c
#define timingAdditive_offset 10072
#define timingAdditive_offset_hex 2758
#define timingMode_offset 448
#define timingMode_offset_hex 1c0
#define timingMultiplier_offset 9872
#define timingMultiplier_offset_hex 2690
#define tle6240_cs_offset 581
#define tle6240_cs_offset_hex 245
#define tle6240_csPinMode_offset 582
#define tle6240_csPinMode_offset_hex 246
#define tle6240spiDevice_offset 4004
#define tle6240spiDevice_offset_hex fa4
#define tle8888_cs_offset 3105
#define tle8888_cs_offset_hex c21
#define tle8888_csPinMode_offset 3106
#define tle8888_csPinMode_offset_hex c22
#define tle8888spiDevice_offset 4000
#define tle8888spiDevice_offset_hex fa0
#define TOP_DEAD_CENTER_MESSAGE "r"
#define TOTAL_CONFIG_SIZE 20000
#define TOTAL_CONFIG_SIZE_hex 4e20
#define tps1_1AdcChannel_offset 512
#define tps1_1AdcChannel_offset_hex 200
#define tps2_1AdcChannel_offset 515
#define tps2_1AdcChannel_offset_hex 203
#define TPS_TPS_ACCEL_TABLE 8
#define tpsAccelEnrichmentThreshold_offset 2048
#define tpsAccelEnrichmentThreshold_offset_hex 800
#define tpsAccelFractionDivisor_offset 3996
#define tpsAccelFractionDivisor_offset_hex f9c
#define tpsAccelFractionPeriod_offset 3994
#define tpsAccelFractionPeriod_offset_hex f9a
#define tpsAccelLength_offset 2044
#define tpsAccelLength_offset_hex 7fc
#define tpsDecelEnleanmentMultiplier_offset 2084
#define tpsDecelEnleanmentMultiplier_offset_hex 824
#define tpsDecelEnleanmentThreshold_offset 2080
#define tpsDecelEnleanmentThreshold_offset_hex 820
#define tpsErrorDetectionTooHigh_offset 86
#define tpsErrorDetectionTooHigh_offset_hex 56
#define tpsErrorDetectionTooLow_offset 84
#define tpsErrorDetectionTooLow_offset_hex 54
#define tpsMax_offset 82
#define tpsMax_offset_hex 52
#define tpsMin_offset 80
#define tpsMin_offset_hex 50
#define tpsTpsAccelFromRpmBins_offset 19232
#define tpsTpsAccelFromRpmBins_offset_hex 4b20
#define tpsTpsAccelTable_offset 18976
#define tpsTpsAccelTable_offset_hex 4a20
#define tpsTpsAccelToRpmBins_offset 19264
#define tpsTpsAccelToRpmBins_offset_hex 4b40
#define trigger_customSkippedToothCount_offset 536
#define trigger_customSkippedToothCount_offset_hex 218
#define trigger_customTotalToothCount_offset 532
#define trigger_customTotalToothCount_offset_hex 214
#define TRIGGER_INPUT_PIN_COUNT 3
#define trigger_offset 524
#define trigger_offset_hex 20c
#define TRIGGER_SIMULATOR_PIN_COUNT 3
#define trigger_type_e_enum "custom toothed wheel", "Ford Aspire", "Dodge Neon 1995", "Miata NA", "Miata NB", "GM_7X", "Cooper R50", "Mazda SOHC 4", "60/2", "36/1", "Honda 4+24+1", "Mitsubishi", "Honda 4+24", "Honda 1+4+24", "Dodge Neon 2003", "Mazda DOHC 1+4", "1+1", "1+60/2", "Single Tooth", "Dodge Ram 1+16", "60/2 VW", "Honda 1+24", "Dodge Stratus", "36_2_2_2", "Nissan Primera", "2JZ", "Rover K", "GM LS 24", "Honda CBR 600", "2JZ_1_12", "Honda CBR 600 custom", "3/1 skipped" , "Dodge Neon 2003 crank", "Miata VVT", "trg34", "trg35", "Subaru 7+6", "Jeep 18-2-2-2", "WIP", "Dodge Neon 1995 crank only", "Jeep XJ 4 cyl", "FiatIAQ_P8", "Mazda Z5", "trg43", "trg44", "trg45", "INVALID"
#define trigger_type_offset 524
#define trigger_type_offset_hex 20c
#define trigger_unusedCustomIsSynchronizationNeeded_offset 528
#define trigger_unusedCustomIsSynchronizationNeeded_offset_hex 210
#define trigger_unusedCustomNeedSecondTriggerInput_offset 528
#define trigger_unusedCustomNeedSecondTriggerInput_offset_hex 210
#define trigger_useOnlyFirstChannel_offset 528
#define trigger_useOnlyFirstChannel_offset_hex 210
#define triggerErrorPin_offset 828
#define triggerErrorPin_offset_hex 33c
#define triggerErrorPinMode_offset 829
#define triggerErrorPinMode_offset_hex 33d
#define triggerInputPins1_offset 700
#define triggerInputPins1_offset_hex 2bc
#define triggerInputPins2_offset 701
#define triggerInputPins2_offset_hex 2bd
#define triggerInputPins3_offset 702
#define triggerInputPins3_offset_hex 2be
#define triggerSimulatorFrequency_offset 620
#define triggerSimulatorFrequency_offset_hex 26c
#define triggerSimulatorPinModes1_offset 739
#define triggerSimulatorPinModes1_offset_hex 2e3
#define triggerSimulatorPinModes2_offset 740
#define triggerSimulatorPinModes2_offset_hex 2e4
#define triggerSimulatorPinModes3_offset 741
#define triggerSimulatorPinModes3_offset_hex 2e5
#define triggerSimulatorPins1_offset 736
#define triggerSimulatorPins1_offset_hex 2e0
#define triggerSimulatorPins2_offset 737
#define triggerSimulatorPins2_offset_hex 2e1
#define triggerSimulatorPins3_offset 738
#define triggerSimulatorPins3_offset_hex 2e2
#define TS_FILE_VERSION 20190701
#define TS_OUTPUT_SIZE 356
#define ts_show_cj125 true
#define ts_show_egt true
#define ts_show_etb true
#define ts_show_etb_pins true
#define ts_show_full_pinout true
#define ts_show_gps true
#define ts_show_hip9011 true
#define ts_show_joystick true
#define ts_show_lcd true
#define TS_SIGNATURE "rusEFI v1.07"
#define tunerStudioSerialSpeed_offset 728
#define tunerStudioSerialSpeed_offset_hex 2d8
#define twoWireBatchIgnition_offset 1476
#define twoWireBatchIgnition_offset_hex 5c4
#define twoWireBatchInjection_offset 1476
#define twoWireBatchInjection_offset_hex 5c4
#define uartConsoleSerialSpeed_offset 2076
#define uartConsoleSerialSpeed_offset_hex 81c
#define unrealisticRpmThreashold_offset 760
#define unrealisticRpmThreashold_offset_hex 2f8
#define unused1234234_offset 2042
#define unused1234234_offset_hex 7fa
#define unused_1484_bit_21_offset 1476
#define unused_1484_bit_21_offset_hex 5c4
#define unused_1484_bit_22_offset 1476
#define unused_1484_bit_22_offset_hex 5c4
#define unused_1484_bit_23_offset 1476
#define unused_1484_bit_23_offset_hex 5c4
#define unused_1484_bit_24_offset 1476
#define unused_1484_bit_24_offset_hex 5c4
#define unused_1484_bit_25_offset 1476
#define unused_1484_bit_25_offset_hex 5c4
#define unused_1484_bit_26_offset 1476
#define unused_1484_bit_26_offset_hex 5c4
#define unused_1484_bit_27_offset 1476
#define unused_1484_bit_27_offset_hex 5c4
#define unused_1484_bit_28_offset 1476
#define unused_1484_bit_28_offset_hex 5c4
#define unused_1484_bit_29_offset 1476
#define unused_1484_bit_29_offset_hex 5c4
#define unused_1484_bit_30_offset 1476
#define unused_1484_bit_30_offset_hex 5c4
#define unused_1484_bit_31_offset 1476
#define unused_1484_bit_31_offset_hex 5c4
#define unused_board_984_31_offset 744
#define unused_board_984_31_offset_hex 2e8
#define unused_former_warmup_target_afr_offset 2096
#define unused_former_warmup_target_afr_offset_hex 830
#define unusedAnotherOne_offset 744
#define unusedAnotherOne_offset_hex 2e8
#define unusedAtBoardConfigurationEnd_offset 980
#define unusedAtBoardConfigurationEnd_offset_hex 3d4
#define unusedErrorPin_offset 2040
#define unusedErrorPin_offset_hex 7f8
#define unusedFlexFuelSensor_offset 3100
#define unusedFlexFuelSensor_offset_hex c1c
#define unusedFormerWarmupAfrPid_offset 1768
#define unusedFormerWarmupAfrPid_offset_hex 6e8
#define unusedOldWarmupAfr_offset 744
#define unusedOldWarmupAfr_offset_hex 2e8
#define unusedSpiPadding3_offset 4036
#define unusedSpiPadding3_offset_hex fc4
#define unusedSpiPadding4_offset 2593
#define unusedSpiPadding4_offset_hex a21
#define unusedSpiPadding5_offset 2713
#define unusedSpiPadding5_offset_hex a99
#define unusedSpiPadding7_offset 4005
#define unusedSpiPadding7_offset_hex fa5
#define unusedSpiPadding8_offset 4009
#define unusedSpiPadding8_offset_hex fa9
#define unuseduartPadding1_offset 810
#define unuseduartPadding1_offset_hex 32a
#define unusuedsw_offset 4020
#define unusuedsw_offset_hex fb4
#define unusuedvref_offset 4016
#define unusuedvref_offset_hex fb0
#define useAdvanceCorrectionsForCranking_offset 1476
#define useAdvanceCorrectionsForCranking_offset_hex 5c4
#define useBiQuadAnalogFiltering_offset 76
#define useBiQuadAnalogFiltering_offset_hex 4c
#define useConstantDwellDuringCranking_offset 1464
#define useConstantDwellDuringCranking_offset_hex 5b8
#define useETBforIdleControl_offset 744
#define useETBforIdleControl_offset_hex 2e8
#define useFixedBaroCorrFromMap_offset 1476
#define useFixedBaroCorrFromMap_offset_hex 5c4
#define useFSIO10ForServo3_offset 1464
#define useFSIO10ForServo3_offset_hex 5b8
#define useFSIO11ForServo4_offset 1464
#define useFSIO11ForServo4_offset_hex 5b8
#define useFSIO12ForIdleOffset_offset 1464
#define useFSIO12ForIdleOffset_offset_hex 5b8
#define useFSIO12ForServo5_offset 1464
#define useFSIO12ForServo5_offset_hex 5b8
#define useFSIO13ForIdleMinValue_offset 1464
#define useFSIO13ForIdleMinValue_offset_hex 5b8
#define useFSIO15ForIdleRpmAdjustment_offset 1464
#define useFSIO15ForIdleRpmAdjustment_offset_hex 5b8
#define useFSIO16ForTimingAdjustment_offset 1464
#define useFSIO16ForTimingAdjustment_offset_hex 5b8
#define useFSIO4ForSeriousEngineWarning_offset 1464
#define useFSIO4ForSeriousEngineWarning_offset_hex 5b8
#define useFSIO5ForCriticalIssueEngineStop_offset 1464
#define useFSIO5ForCriticalIssueEngineStop_offset_hex 5b8
#define useFSIO6ForRevLimiter_offset 1464
#define useFSIO6ForRevLimiter_offset_hex 5b8
#define useFSIO8ForServo1_offset 1464
#define useFSIO8ForServo1_offset_hex 5b8
#define useFSIO9ForServo2_offset 1464
#define useFSIO9ForServo2_offset_hex 5b8
#define useIacTableForCoasting_offset 744
#define useIacTableForCoasting_offset_hex 2e8
#define useIdleTimingPidControl_offset 744
#define useIdleTimingPidControl_offset_hex 2e8
#define useInstantRpmForIdle_offset 76
#define useInstantRpmForIdle_offset_hex 4c
#define useLcdScreen_offset 744
#define useLcdScreen_offset_hex 2e8
#define useLinearCltSensor_offset 1464
#define useLinearCltSensor_offset_hex 5b8
#define useLinearIatSensor_offset 1464
#define useLinearIatSensor_offset_hex 5b8
#define useNoiselessTriggerDecoder_offset 744
#define useNoiselessTriggerDecoder_offset_hex 2e8
#define useOnlyRisingEdgeForTrigger_offset 1476
#define useOnlyRisingEdgeForTrigger_offset_hex 5c4
#define useSeparateAdvanceForCranking_offset 1476
#define useSeparateAdvanceForCranking_offset_hex 5c4
#define useSeparateAdvanceForIdle_offset 1476
#define useSeparateAdvanceForIdle_offset_hex 5c4
#define useSeparateVeForIdle_offset 1476
#define useSeparateVeForIdle_offset_hex 5c4
#define useSerialPort_offset 744
#define useSerialPort_offset_hex 2e8
#define useStepperIdle_offset 744
#define useStepperIdle_offset_hex 2e8
#define useTLE8888_cranking_hack_offset 76
#define useTLE8888_cranking_hack_offset_hex 4c
#define useTLE8888_hall_mode_offset 76
#define useTLE8888_hall_mode_offset_hex 4c
#define useTpicAdvancedMode_offset 744
#define useTpicAdvancedMode_offset_hex 2e8
#define useTPSAdvanceTable_offset 1476
#define useTPSAdvanceTable_offset_hex 5c4
#define useTPSBasedVeTable_offset 744
#define useTPSBasedVeTable_offset_hex 2e8
#define VBAT_INJECTOR_CURVE_SIZE 8
#define vbattAdcChannel_offset 513
#define vbattAdcChannel_offset_hex 201
#define vbattDividerCoeff_offset 464
#define vbattDividerCoeff_offset_hex 1d0
#define vehicleSpeedCoef_offset 476
#define vehicleSpeedCoef_offset_hex 1dc
#define vehicleSpeedSensorInputPin_offset 968
#define vehicleSpeedSensorInputPin_offset_hex 3c8
#define veLoadBins_offset 18464
#define veLoadBins_offset_hex 4820
#define verboseTriggerSynchDetails_offset 1476
#define verboseTriggerSynchDetails_offset_hex 5c4
#define veRpmBins_offset 18528
#define veRpmBins_offset_hex 4860
#define veTable_offset 17440
#define veTable_offset_hex 4420
#define vRefAdcChannel_offset 1470
#define vRefAdcChannel_offset_hex 5be
#define vvt_mode_e_enum "First half", "Second half", "2GZ", "Miata NB2", "mode4", "mode5", "mode6", "mode7"
#define vvtCamSensorUseRise_offset 744
#define vvtCamSensorUseRise_offset_hex 2e8
#define vvtDisplayInverted_offset 1464
#define vvtDisplayInverted_offset_hex 5b8
#define vvtMode_offset 2328
#define vvtMode_offset_hex 918
#define vvtOffset_offset 2052
#define vvtOffset_offset_hex 804
#define warningLedPin_offset 2041
#define warningLedPin_offset_hex 7f9
#define warningPeriod_offset 1498
#define warningPeriod_offset_hex 5da
#define wboHeaterPin_offset 673
#define wboHeaterPin_offset_hex 2a1
#define wwaeBeta_offset 1808
#define wwaeBeta_offset_hex 710
#define wwaeTau_offset 1712
#define wwaeTau_offset_hex 6b0
// start of pid_s
struct pid_s {
	/**
	 * offset 0
	 */
	float pFactor = (float)0;
	/**
	 * offset 4
	 */
	float iFactor = (float)0;
	/**
	 * offset 8
	 */
	float dFactor = (float)0;
	/**
	 * Linear addition to PID logic
	 * offset 12
	 */
	int16_t offset = (int16_t)0;
	/**
	 * PID dTime
	 * offset 14
	 */
	int16_t periodMs = (int16_t)0;
	/**
	 * Output min value
	 * offset 16
	 */
	int16_t minValue = (int16_t)0;
	/**
	 * Output max value
	 * offset 18
	 */
	int16_t maxValue = (int16_t)0;
	/** total size 20*/
};

typedef struct pid_s pid_s;

// start of cranking_parameters_s
struct cranking_parameters_s {
	/**
	 * Base duration of the fuel injection during cranking, this is modified by the multipliers for CLT, IAT, TPS ect, to give the final cranking pulse width.
	 * offset 0
	 */
	float baseFuel = (float)0;
	/**
	 * This sets the RPM limit below which the ECU will use cranking fuel and ignition logic, typically this is around 350-450rpm. 
	 * set cranking_rpm X
	 * offset 4
	 */
	int16_t rpm = (int16_t)0;
	/**
	 * need 4 byte alignment
	 * offset 6
	 */
	uint8_t alignmentFill[2];
	/** total size 8*/
};

typedef struct cranking_parameters_s cranking_parameters_s;

// start of spi_pins
struct spi_pins {
	/**
	 * offset 0
	 */
	brain_pin_e mosiPin;
	/**
	 * offset 1
	 */
	brain_pin_e misoPin;
	/**
	 * offset 2
	 */
	brain_pin_e sckPin;
	/**
	 * need 4 byte alignment
	 * offset 3
	 */
	uint8_t alignmentFill = (uint8_t)0;
	/** total size 4*/
};

typedef struct spi_pins spi_pins;

// start of air_pressure_sensor_config_s
struct air_pressure_sensor_config_s {
	/**
	 * kPa value at low volts
	 * offset 0
	 */
	float lowValue = (float)0;
	/**
	 * kPa value at high volts
	 * offset 4
	 */
	float highValue = (float)0;
	/**
	 * offset 8
	 */
	air_pressure_sensor_type_e type;
	/**
	 * offset 12
	 */
	adc_channel_e hwChannel;
	/**
	 * offset 13
	 */
	uint8_t align[3];
	/** total size 16*/
};

typedef struct air_pressure_sensor_config_s air_pressure_sensor_config_s;

/**
 * @brief MAP averaging configuration

*/
// start of MAP_sensor_config_s
struct MAP_sensor_config_s {
	/**
	 * offset 0
	 */
	float samplingAngleBins[MAP_ANGLE_SIZE];
	/**
	 * @brief MAP averaging sampling start angle, by RPM
	 * offset 32
	 */
	float samplingAngle[MAP_ANGLE_SIZE];
	/**
	 * offset 64
	 */
	float samplingWindowBins[MAP_WINDOW_SIZE];
	/**
	 * @brief MAP averaging angle duration, by RPM
	 * offset 96
	 */
	float samplingWindow[MAP_WINDOW_SIZE];
	/**
	 * offset 128
	 */
	air_pressure_sensor_config_s sensor;
	/** total size 144*/
};

typedef struct MAP_sensor_config_s MAP_sensor_config_s;

/**
 * @brief Thermistor known values

*/
// start of thermistor_conf_s
struct thermistor_conf_s {
	/**
	 * these values are in Celcius
	 * offset 0
	 */
	float tempC_1 = (float)0;
	/**
	 * offset 4
	 */
	float tempC_2 = (float)0;
	/**
	 * offset 8
	 */
	float tempC_3 = (float)0;
	/**
	 * offset 12
	 */
	float resistance_1 = (float)0;
	/**
	 * offset 16
	 */
	float resistance_2 = (float)0;
	/**
	 * offset 20
	 */
	float resistance_3 = (float)0;
	/**
	 * Pull-up resistor value on your board
	 * offset 24
	 */
	float bias_resistor = (float)0;
	/** total size 28*/
};

typedef struct thermistor_conf_s thermistor_conf_s;

/**
 * @brief Oil pressure sensor interpolation

*/
// start of oil_pressure_config_s
struct oil_pressure_config_s {
	/**
	 * offset 0
	 */
	adc_channel_e hwChannel;
	/**
	 * offset 1
	 */
	uint8_t align[3];
	/**
	 * offset 4
	 */
	float v1 = (float)0;
	/**
	 * offset 8
	 */
	float value1 = (float)0;
	/**
	 * offset 12
	 */
	float v2 = (float)0;
	/**
	 * offset 16
	 */
	float value2 = (float)0;
	/** total size 20*/
};

typedef struct oil_pressure_config_s oil_pressure_config_s;

/**
 * @brief Thermistor curve parameters

*/
// start of ThermistorConf
struct ThermistorConf {
	/**
	 * offset 0
	 */
	thermistor_conf_s config;
	/**
	 * offset 28
	 */
	adc_channel_e adcChannel;
	/**
	 * need 4 byte alignment
	 * offset 29
	 */
	uint8_t alignmentFill[3];
	/** total size 32*/
};

typedef struct ThermistorConf ThermistorConf;

// start of injector_s
struct injector_s {
	/**
	 * This is your injector flow at the fuel pressure used in the vehicle. cc/min, cubic centimetre per minute
	 * By the way, g/s = 0.125997881 * (lb/hr)
	 * g/s = 0.125997881 * (cc/min)/10.5
	 * g/s = 0.0119997981 * cc/min
	 * offset 0
	 */
	float flow = (float)0;
	/**
	 * set_flat_injector_lag LAG
	 * set_injector_lag VOLTAGE LAG
	 * offset 4
	 */
	float battLagCorrBins[VBAT_INJECTOR_CURVE_SIZE];
	/**
	 * ms delay between injector open and close dead times
	 * offset 36
	 */
	float battLagCorr[VBAT_INJECTOR_CURVE_SIZE];
	/** total size 68*/
};

typedef struct injector_s injector_s;

// start of bi_quard_s
struct bi_quard_s {
	/**
	 * offset 0
	 */
	float a0 = (float)0;
	/**
	 * offset 4
	 */
	float a1 = (float)0;
	/**
	 * offset 8
	 */
	float a2 = (float)0;
	/**
	 * offset 12
	 */
	float b1 = (float)0;
	/**
	 * offset 16
	 */
	float b2 = (float)0;
	/** total size 20*/
};

typedef struct bi_quard_s bi_quard_s;

// start of specs_s
struct specs_s {
	/**
	 * Engine displacement, in litres
	 * see also cylindersCount
	 * offset 0
	 */
	float displacement = (float)0;
	/**
	 * offset 4
	 */
	cylinders_count_t cylindersCount;
	/**
	 * offset 8
	 */
	firing_order_e firingOrder;
	/** total size 12*/
};

typedef struct specs_s specs_s;

/**
 * @brief Trigger wheel(s) configuration

*/
// start of trigger_config_s
struct trigger_config_s {
	/**
	 * set trigger_type X
	 * offset 0
	 */
	trigger_type_e type;
	/**
	offset 4 bit 0 */
	bool unusedCustomIsSynchronizationNeeded : 1;
	/**
	offset 4 bit 1 */
	bool unusedCustomNeedSecondTriggerInput : 1;
	/**
	 * This option could be used if your second trigger channel is broken
	offset 4 bit 2 */
	bool useOnlyFirstChannel : 1;
	/**
	 * offset 8
	 */
	int customTotalToothCount = (int)0;
	/**
	 * offset 12
	 */
	int customSkippedToothCount = (int)0;
	/** total size 16*/
};

typedef struct trigger_config_s trigger_config_s;

// start of afr_sensor_s
struct afr_sensor_s {
	/**
	 * offset 0
	 */
	adc_channel_e hwChannel;
	/**
	 * offset 1
	 */
	uint8_t alignAf[3];
	/**
	 * offset 4
	 */
	float v1 = (float)0;
	/**
	 * offset 8
	 */
	float value1 = (float)0;
	/**
	 * offset 12
	 */
	float v2 = (float)0;
	/**
	 * offset 16
	 */
	float value2 = (float)0;
	/** total size 20*/
};

typedef struct afr_sensor_s afr_sensor_s;

// start of idle_hardware_s
struct idle_hardware_s {
	/**
	 * offset 0
	 */
	int solenoidFrequency = (int)0;
	/**
	 * offset 4
	 */
	brain_pin_e solenoidPin;
	/**
	 * offset 5
	 */
	brain_pin_e stepperDirectionPin;
	/**
	 * offset 6
	 */
	brain_pin_e stepperStepPin;
	/**
	 * offset 7
	 */
	pin_output_mode_e solenoidPinMode;
	/** total size 8*/
};

typedef struct idle_hardware_s idle_hardware_s;

// start of etb_io
struct etb_io {
	/**
	 * offset 0
	 */
	brain_pin_e directionPin1;
	/**
	 * offset 1
	 */
	brain_pin_e directionPin2;
	/**
	 * offset 2
	 */
	brain_pin_e controlPin1;
	/**
	 * offset 3
	 */
	pin_output_mode_e controlPinMode;
	/** total size 4*/
};

typedef struct etb_io etb_io;

// start of board_configuration_s
struct board_configuration_s {
	/**
	 * offset 0
	 */
	idle_hardware_s idle;
	/**
	 * value between 0 and 100 used in Manual mode
	 * offset 8
	 */
	float manIdlePosition = (float)0;
	/**
	 * offset 12
	 */
	float mapFrequency0Kpa = (float)0;
	/**
	 * offset 16
	 */
	float mapFrequency100Kpa = (float)0;
	/**
	 * Same RPM is used for two ways of producing simulated RPM. See also triggerSimulatorPins (with wires)
	 * See also directSelfStimulation (no wires, bypassing input hardware)
	 * rpm X
	 * offset 20
	 */
	int triggerSimulatorFrequency = (int)0;
	/**
	 * offset 24
	 */
	output_pin_e injectionPins[INJECTION_PIN_COUNT];
	/**
	 * offset 36
	 */
	output_pin_e ignitionPins[IGNITION_PIN_COUNT];
	/**
	 * offset 48
	 */
	pin_output_mode_e injectionPinMode;
	/**
	 * offset 49
	 */
	pin_output_mode_e ignitionPinMode;
	/**
	 * offset 50
	 */
	brain_pin_e HD44780_rs;
	/**
	 * offset 51
	 */
	brain_pin_e HD44780_e;
	/**
	 * offset 52
	 */
	brain_pin_e HD44780_db4;
	/**
	 * offset 53
	 */
	brain_pin_e HD44780_db5;
	/**
	 * offset 54
	 */
	brain_pin_e HD44780_db6;
	/**
	 * offset 55
	 */
	brain_pin_e HD44780_db7;
	/**
	 * offset 56
	 */
	brain_pin_e gps_rx_pin;
	/**
	 * offset 57
	 */
	brain_pin_e gps_tx_pin;
	/**
	 * offset 58
	 */
	output_pin_e fuelPumpPin;
	/**
	 * offset 59
	 */
	pin_output_mode_e fuelPumpPinMode;
	/**
	 * Check engine light, also malfunction indicator light. Always blinks once on boot.
	 * offset 60
	 */
	output_pin_e malfunctionIndicatorPin;
	/**
	 * offset 61
	 */
	pin_output_mode_e malfunctionIndicatorPinMode;
	/**
	 * offset 62
	 */
	pin_output_mode_e fanPinMode;
	/**
	 * offset 63
	 */
	output_pin_e fanPin;
	/**
	 * some cars have a switch to indicate that clutch pedal is all the way down
	 * offset 64
	 */
	switch_input_pin_e clutchDownPin;
	/**
	 * offset 65
	 */
	output_pin_e alternatorControlPin;
	/**
	 * offset 66
	 */
	pin_output_mode_e alternatorControlPinMode;
	/**
	 * offset 67
	 */
	pin_input_mode_e clutchDownPinMode;
	/**
	 * offset 68
	 */
	brain_pin_e digitalPotentiometerChipSelect[DIGIPOT_COUNT];
	/**
	 * offset 72
	 */
	pin_output_mode_e electronicThrottlePin1Mode;
	/**
	 * offset 73
	 */
	brain_pin_e wboHeaterPin;
	/**
	 * offset 74
	 */
	brain_pin_e cj125CsPin;
	/**
	 * offset 75
	 */
	spi_device_e max31855spiDevice;
	/**
	 * offset 76
	 */
	brain_pin_e debugTriggerSync;
	/**
	 * Digital Potentiometer is used by stock ECU stimulation code
	 * offset 77
	 */
	spi_device_e digitalPotentiometerSpiDevice;
	/**
	 * offset 78
	 */
	brain_pin_e mc33972_cs;
	/**
	 * offset 79
	 */
	pin_output_mode_e mc33972_csPinMode;
	/**
	 * offset 80
	 */
	etb_io etb1;
	/**
	 * offset 84
	 */
	float fuelLevelEmptyTankVoltage = (float)0;
	/**
	 * offset 88
	 */
	float fuelLevelFullTankVoltage = (float)0;
	/**
	 * AFR, WBO, EGO - whatever you like to call it
	 * offset 92
	 */
	ego_sensor_e afr_type;
	/**
	 * offset 96
	 */
	float fuelClosedLoopAfrLowThreshold = (float)0;
	/**
	 * offset 100
	 */
	brain_input_pin_e triggerInputPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * offset 103
	 */
	pin_output_mode_e hip9011CsPinMode;
	/**
	 * This implementation produces one pulse per engine cycle. See also dizzySparkOutputPin.
	 * offset 104
	 */
	output_pin_e tachOutputPin;
	/**
	 * offset 105
	 */
	pin_output_mode_e tachOutputPinMode;
	/**
	 * offset 106
	 */
	output_pin_e mainRelayPin;
	/**
	 * offset 107
	 */
	brain_pin_e sdCardCsPin;
	/**
	 * offset 108
	 */
	brain_pin_e canTxPin;
	/**
	 * offset 109
	 */
	brain_pin_e canRxPin;
	/**
	 * offset 110
	 */
	pin_input_mode_e throttlePedalUpPinMode;
	/**
	 * offset 111
	 */
	brain_pin_e debugTimerCallback;
	/**
	 * offset 112
	 */
	int idleThreadPeriodMs = (int)0;
	/**
	 * offset 116
	 */
	int consoleLoopPeriodMs = (int)0;
	/**
	 * offset 120
	 */
	int lcdThreadPeriodMs = (int)0;
	/**
	 * offset 124
	 */
	int generalPeriodicThreadPeriodMs = (int)0;
	/**
	 * offset 128
	 */
	uint32_t tunerStudioSerialSpeed = (uint32_t)0;
	/**
	 * offset 132
	 */
	can_device_mode_e canDeviceMode;
	/**
	 * Each rusEfi piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEfi board.
	 * See also directSelfStimulation which is different.
	 * offset 136
	 */
	brain_pin_e triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * offset 139
	 */
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * Narrow band o2 heater, not used for CJ125. See wboHeaterPin
	 * offset 142
	 */
	output_pin_e o2heaterPin;
	/**
	 * offset 143
	 */
	pin_output_mode_e o2heaterPinModeTodO;
	/**
	offset 144 bit 0 */
	bool is_enabled_spi_1 : 1;
	/**
	offset 144 bit 1 */
	bool is_enabled_spi_2 : 1;
	/**
	offset 144 bit 2 */
	bool is_enabled_spi_3 : 1;
	/**
	offset 144 bit 3 */
	bool isSdCardEnabled : 1;
	/**
	offset 144 bit 4 */
	bool isFastAdcEnabled : 1;
	/**
	offset 144 bit 5 */
	bool isEngineControlEnabled : 1;
	/**
	offset 144 bit 6 */
	bool isHip9011Enabled : 1;
	/**
	offset 144 bit 7 */
	bool isVerboseAlternator : 1;
	/**
	offset 144 bit 8 */
	bool useSerialPort : 1;
	/**
	 * This setting should only be used if you have a stepper motor idle valve and a stepper motor driver installed.
	offset 144 bit 9 */
	bool useStepperIdle : 1;
	/**
	offset 144 bit 10 */
	bool enabledStep1Limiter : 1;
	/**
	offset 144 bit 11 */
	bool useTpicAdvancedMode : 1;
	/**
	offset 144 bit 12 */
	bool useLcdScreen : 1;
	/**
	offset 144 bit 13 */
	bool unusedAnotherOne : 1;
	/**
	offset 144 bit 14 */
	bool unusedOldWarmupAfr : 1;
	/**
	 *  +This will cause the alternator to be operated in a basic on or off mode, this is the simplest alternator control.
	offset 144 bit 15 */
	bool onOffAlternatorLogic : 1;
	/**
	offset 144 bit 16 */
	bool isCJ125Enabled : 1;
	/**
	 * Use rise or fall signal front
	offset 144 bit 17 */
	bool vvtCamSensorUseRise : 1;
	/**
	 * Useful for individual intakes
	offset 144 bit 18 */
	bool measureMapOnlyInOneCylinder : 1;
	/**
	offset 144 bit 19 */
	bool stepperForceParkingEveryRestart : 1;
	/**
	 * Smarter cranking logic.
	 * See also startOfCrankingPrimingPulse
	offset 144 bit 20 */
	bool isFasterEngineSpinUpEnabled : 1;
	/**
	 * This setting disables fuel injection while the engine is in overrun, this is useful as a fuel saving measure and to prevent back firing.
	offset 144 bit 21 */
	bool coastingFuelCutEnabled : 1;
	/**
	 * This setting allows the ECU to open the IAC during overrun conditions to help reduce engine breaking, this can be helpful for large engines in light weight cars.
	offset 144 bit 22 */
	bool useIacTableForCoasting : 1;
	/**
	offset 144 bit 23 */
	bool useNoiselessTriggerDecoder : 1;
	/**
	offset 144 bit 24 */
	bool useIdleTimingPidControl : 1;
	/**
	offset 144 bit 25 */
	bool useTPSBasedVeTable : 1;
	/**
	offset 144 bit 26 */
	bool is_enabled_spi_4 : 1;
	/**
	offset 144 bit 27 */
	bool pauseEtbControl : 1;
	/**
	offset 144 bit 28 */
	bool alignEngineSnifferAtTDC : 1;
	/**
	 * This setting allows the ETB to act as the idle air control valve and move to regulate the airflow at idle.
	offset 144 bit 29 */
	bool useETBforIdleControl : 1;
	/**
	offset 144 bit 30 */
	bool idleIncrementalPidCic : 1;
	/**
	offset 144 bit 31 */
	bool unused_board_984_31 : 1;
	/**
	 * offset 148
	 */
	brain_input_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * offset 152
	 */
	pin_output_mode_e mainRelayPinMode;
	/**
	 * offset 153
	 */
	brain_pin_e hip9011CsPin;
	/**
	 * offset 154
	 */
	brain_pin_e hip9011IntHoldPin;
	/**
	 * offset 155
	 */
	pin_output_mode_e hip9011IntHoldPinMode;
	/**
	 * default or inverted input
	 * offset 156
	 */
	uint8_t logicAnalyzerMode[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * offset 160
	 */
	int unrealisticRpmThreashold = (int)0;
	/**
	 * offset 164
	 */
	pin_output_mode_e gpioPinModes[FSIO_COMMAND_COUNT];
	/**
	 * todo: more comments
	 * offset 180
	 */
	output_pin_e fsioOutputPins[FSIO_COMMAND_COUNT];
	/**
	 * offset 196
	 */
	brain_pin_e max31855_cs[EGT_CHANNEL_COUNT];
	/**
	 * SD card logging period, in milliseconds
	 * offset 204
	 */
	int16_t sdCardPeriodMs = (int16_t)0;
	/**
	 * offset 206
	 */
	brain_pin_e debugSetTimer;
	/**
	 * offset 207
	 */
	brain_pin_e debugMapAveraging;
	/**
	 * offset 208
	 */
	brain_pin_e starterRelayPin;
	/**
	 * offset 209
	 */
	pin_output_mode_e starterRelayPinMode;
	/**
	 * offset 210
	 */
	uint8_t unuseduartPadding1[2];
	/**
	 * offset 212
	 */
	int mapMinBufferLength = (int)0;
	/**
	 * offset 216
	 */
	int16_t idlePidDeactivationTpsThreshold = (int16_t)0;
	/**
	 * offset 218
	 */
	int16_t stepperParkingExtraSteps = (int16_t)0;
	/**
	 * This magic property is specific to Mazda Miata NB2
	 * offset 220
	 */
	float miataNb2VVTRatioFrom = (float)0;
	/**
	 * This magic property is specific to Mazda Miata NB2
	 * offset 224
	 */
	float miataNb2VVTRatioTo = (float)0;
	/**
	 * This pin is used for debugging - snap a logic analyzer on it and see if it's ever high
	 * offset 228
	 */
	brain_pin_e triggerErrorPin;
	/**
	 * offset 229
	 */
	pin_output_mode_e triggerErrorPinMode;
	/**
	 * offset 230
	 */
	output_pin_e acRelayPin;
	/**
	 * offset 231
	 */
	pin_output_mode_e acRelayPinMode;
	/**
	 * offset 232
	 */
	fsio_pwm_freq_t fsioFrequency[FSIO_COMMAND_COUNT];
	/**
	 * offset 264
	 */
	fsio_setting_t fsio_setting[FSIO_COMMAND_COUNT];
	/**
	 * offset 328
	 */
	brain_pin_e spi1mosiPin;
	/**
	 * offset 329
	 */
	brain_pin_e spi1misoPin;
	/**
	 * offset 330
	 */
	brain_pin_e spi1sckPin;
	/**
	 * offset 331
	 */
	brain_pin_e spi2mosiPin;
	/**
	 * offset 332
	 */
	brain_pin_e spi2misoPin;
	/**
	 * offset 333
	 */
	brain_pin_e spi2sckPin;
	/**
	 * offset 334
	 */
	brain_pin_e spi3mosiPin;
	/**
	 * offset 335
	 */
	brain_pin_e spi3misoPin;
	/**
	 * offset 336
	 */
	brain_pin_e spi3sckPin;
	/**
	 * Saab Combustion Detection Module knock signal input pin
	 * also known as Saab Ion Sensing Module
	 * offset 337
	 */
	brain_pin_e cdmInputPin;
	/**
	 * offset 338
	 */
	brain_pin_e joystickCenterPin;
	/**
	 * offset 339
	 */
	brain_pin_e joystickAPin;
	/**
	 * offset 340
	 */
	brain_pin_e joystickBPin;
	/**
	 * offset 341
	 */
	brain_pin_e joystickCPin;
	/**
	 * offset 342
	 */
	brain_pin_e joystickDPin;
	/**
	 * offset 343
	 */
	uart_device_e consoleUartDevice;
	/**
	 * rusEfi console Sensor Sniffer mode
	 * offset 344
	 */
	sensor_chart_e sensorChartMode;
	/**
	 * offset 348
	 */
	maf_sensor_type_e mafSensorType;
	/**
	 * todo:not finished
	 * These input pins allow us to pull toggle buttons state
	 * offset 352
	 */
	brain_pin_e fsioDigitalInputs[FSIO_COMMAND_COUNT];
	/**
	 * offset 368
	 */
	brain_input_pin_e vehicleSpeedSensorInputPin;
	/**
	 * Some vehicles have a switch to indicate that clutch pedal is all the way up
	 * offset 369
	 */
	switch_input_pin_e clutchUpPin;
	/**
	 * offset 370
	 */
	brain_input_pin_e frequencyReportingMapInputPin;
	/**
	 * offset 371
	 */
	pin_input_mode_e clutchUpPinMode;
	/**
	 * offset 372
	 */
	float etbIdleRange = (float)0;
	/**
	offset 376 bit 0 */
	bool clutchUpPinInverted : 1;
	/**
	offset 376 bit 1 */
	bool clutchDownPinInverted : 1;
	/**
	 * offset 380
	 */
	int unusedAtBoardConfigurationEnd[121];
	/** total size 864*/
};

typedef struct board_configuration_s board_configuration_s;

// start of engine_configuration_s
struct engine_configuration_s {
	/**
	 * http://rusefi.com/wiki/index.php?title=Manual:Engine_Type
	 * set engine_type X
	 * offset 0
	 */
	engine_type_e engineType;
	/**
	 * Engine sniffer would be disabled above this rpm
	 * set engineSnifferRpmThreshold X
	 * offset 4
	 */
	int engineSnifferRpmThreshold = (int)0;
	/**
	 * offset 8
	 */
	injector_s injector;
	/**
	 * Should trigger emulator push data right into trigger handling logic, eliminating the need for physical jumper wires?
	 * See also triggerSimulatorPins
	 * PS: Funny name, right? :)
	offset 76 bit 0 */
	bool directSelfStimulation : 1;
	/**
	offset 76 bit 1 */
	bool activateAuxPid1 : 1;
	/**
	offset 76 bit 2 */
	bool isVerboseAuxPid1 : 1;
	/**
	offset 76 bit 3 */
	bool activateAuxPid2 : 1;
	/**
	offset 76 bit 4 */
	bool isVerboseAuxPid2 : 1;
	/**
	offset 76 bit 5 */
	bool activateAuxPid3 : 1;
	/**
	offset 76 bit 6 */
	bool isVerboseAuxPid3 : 1;
	/**
	offset 76 bit 7 */
	bool activateAuxPid4 : 1;
	/**
	offset 76 bit 8 */
	bool isVerboseAuxPid4 : 1;
	/**
	offset 76 bit 9 */
	bool useBiQuadAnalogFiltering : 1;
	/**
	offset 76 bit 10 */
	bool cj125isUaDivided : 1;
	/**
	offset 76 bit 11 */
	bool cj125isLsu49 : 1;
	/**
	offset 76 bit 12 */
	bool etb1_use_two_wires : 1;
	/**
	offset 76 bit 13 */
	bool etb2_use_two_wires : 1;
	/**
	offset 76 bit 14 */
	bool showSdCardWarning : 1;
	/**
	 * looks like 3v range should be enough, divider not needed
	offset 76 bit 15 */
	bool cj125isUrDivided : 1;
	/**
	offset 76 bit 16 */
	bool useTLE8888_hall_mode : 1;
	/**
	offset 76 bit 17 */
	bool useTLE8888_cranking_hack : 1;
	/**
	offset 76 bit 18 */
	bool useInstantRpmForIdle : 1;
	/**
	 * If your fuel regulator does not have vacuum line
	offset 76 bit 19 */
	bool absoluteFuelPressure : 1;
	/**
	offset 76 bit 20 */
	bool issue_294_21 : 1;
	/**
	offset 76 bit 21 */
	bool issue_294_22 : 1;
	/**
	offset 76 bit 22 */
	bool issue_294_23 : 1;
	/**
	offset 76 bit 23 */
	bool issue_294_24 : 1;
	/**
	offset 76 bit 24 */
	bool issue_294_25 : 1;
	/**
	offset 76 bit 25 */
	bool issue_294_26 : 1;
	/**
	offset 76 bit 26 */
	bool issue_294_27 : 1;
	/**
	offset 76 bit 27 */
	bool issue_294_28 : 1;
	/**
	offset 76 bit 28 */
	bool issue_294_29 : 1;
	/**
	offset 76 bit 29 */
	bool issue_294_30 : 1;
	/**
	offset 76 bit 30 */
	bool issue_294_31 : 1;
	/**
	 * Closed throttle. todo: extract these two fields into a structure
	 * todo: we need two sets of TPS parameters - modern ETBs have two sensors
	 * See also tps1_1AdcChannel
	 * set tps_min X
	 * offset 80
	 */
	int16_t tpsMin = (int16_t)0;
	/**
	 * Full throttle. tpsMax value as 10 bit ADC value. Not Voltage!
	 * See also tps1_1AdcChannel
	 * set tps_max X
	 * offset 82
	 */
	int16_t tpsMax = (int16_t)0;
	/**
	 * TPS error detection, what TPS % value is unrealistically low
	 * offset 84
	 */
	int16_t tpsErrorDetectionTooLow = (int16_t)0;
	/**
	 * TPS error detection, what TPS % value is unrealistically high
	 * offset 86
	 */
	int16_t tpsErrorDetectionTooHigh = (int16_t)0;
	/**
	 * offset 88
	 */
	cranking_parameters_s cranking;
	/**
	 * offset 96
	 */
	float primingSquirtDurationMs = (float)0;
	/**
	 * Used if useConstantDwellDuringCranking is TRUE
	 * offset 100
	 */
	float ignitionDwellForCrankingMs = (float)0;
	/**
	 * While cranking (which causes battery voltage to drop) we can calculate dwell time in shaft
	 * degrees, not in absolute time as in running mode.
	 * set cranking_charge_angle X
	 * offset 104
	 */
	float crankingChargeAngle = (float)0;
	/**
	 * @see hasMapSensor
	 * @see isMapAveragingEnabled
	 * offset 108
	 */
	MAP_sensor_config_s map;
	/**
	 * todo: merge with channel settings, use full-scale Thermistor here!
	 * offset 252
	 */
	ThermistorConf clt;
	/**
	 * offset 284
	 */
	ThermistorConf iat;
	/**
	 * A secondary Rev limit engaged by the driver to help launch the vehicle faster
	 * offset 316
	 */
	int step1rpm = (int)0;
	/**
	 * offset 320
	 */
	int step1timing = (int)0;
	/**
	 * value '6' for 8MHz hw osc
	 * read hip9011 datasheet for details
	 * todo split into two bit fields
	 * offset 324
	 */
	int hip9011PrescalerAndSDO = (int)0;
	/**
	 * We calculate knock band based of cylinderBore
	 *  Use this to override - kHz knock band override
	 * offset 328
	 */
	float knockBandCustom = (float)0;
	/**
	 * On single-coil or wasted spark setups you have to lower dwell at high RPM
	 * offset 332
	 */
	float sparkDwellRpmBins[DWELL_CURVE_SIZE];
	/**
	 * offset 364
	 */
	float sparkDwellValues[DWELL_CURVE_SIZE];
	/**
	 * offset 396
	 */
	specs_s specs;
	/**
	 * Cylinder diameter, in mm.
	 * offset 408
	 */
	float cylinderBore = (float)0;
	/**
	 * Disable sensor sniffer above this rpm
	 * offset 412
	 */
	int sensorSnifferRpmThreshold = (int)0;
	/**
	 * set rpm_hard_limit X
	 * offset 416
	 */
	int rpmHardLimit = (int)0;
	/**
	 * This setting controls which fuel quantity control algorithm is used.
	 * See also useTPSAdvanceTable
	 * set algorithm X
	 * offset 420
	 */
	engine_load_mode_e fuelAlgorithm;
	/**
	 * This is the injection strategy during engine start. See Fuel/Injection settings for more detail. It is suggested to use "Simultaneous".
	 * offset 424
	 */
	injection_mode_e crankingInjectionMode;
	/**
	 * This is where the fuel injection type is defined: "Simultaneous" means all injectors will fire together at once. "Sequential" fires the injectors on a per cylinder basis, which requires individually wired injectors. "Batched" will fire the injectors in groups. If your injectors are individually wired you will also need to enable "Two wire batch emulation". 
	 * set injection_mode X
	 * See also twoWireBatchInjection
	 * offset 428
	 */
	injection_mode_e injectionMode;
	/**
	 * this is about deciding when the injector starts it's squirt
	 * See also injectionPhase map
	 * todo: do we need even need this since we have the map anyway?
	 * offset 432
	 */
	angle_t extraInjectionOffset = (angle_t)0;
	/**
	 * Ignition advance angle used during engine cranking, 5-10 degrees will work as a base setting for most engines.
	 * set cranking_timing_angle X
	 * offset 436
	 */
	angle_t crankingTimingAngle = (angle_t)0;
	/**
	 * "One Coil" is for use on distributed ignition system. "Individual Coils" is to be used when you have one coil per cylinder (COP or similar). "Wasted" means one coil is driving two spark plugs in two cylinders, with one of the sparks not doing anything since it's happening on the exhaust cycle
	 * set ignition_mode X
	 * offset 440
	 */
	ignition_mode_e ignitionMode;
	/**
	 * this value could be used to offset the whole ignition timing table by a constant
	 * offset 444
	 */
	angle_t ignitionOffset = (angle_t)0;
	/**
	 * Dynamic uses the timing map to decide the ignition timing, Static timing fixes the timing to the value set below (only use for checking static timing).
	 * offset 448
	 */
	timing_mode_e timingMode;
	/**
	 * This value is the ignition timing used when in 'fixed timing' mode, i.e. constant timing
	 * This mode is useful when adjusting distributor location.
	 * offset 452
	 */
	angle_t fixedModeTiming = (angle_t)0;
	/**
	 * Angle between Top Dead Center (TDC) and the first trigger event.
	 * Knowing this angle allows us to control timing and other angles in reference to TDC.
	 * set global_trigger_offset_angle X
	 * offset 456
	 */
	angle_t globalTriggerAngleOffset = (angle_t)0;
	/**
	 * Coefficient of input voltage dividers on your PCB
	 * offset 460
	 */
	float analogInputDividerCoefficient = (float)0;
	/**
	 * This is the ratio of the resistors for the battery voltage, measure the voltage at the battery and then adjust this number until the gauge matches the reading.
	 * offset 464
	 */
	float vbattDividerCoeff = (float)0;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	 * offset 468
	 */
	float fanOnTemperature = (float)0;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	 * offset 472
	 */
	float fanOffTemperature = (float)0;
	/**
	 * This coefficient translates vehicle speed input frequency (in Hz) into vehicle speed, km/h
	 * offset 476
	 */
	float vehicleSpeedCoef = (float)0;
	/**
	 * set can_mode X
	 * offset 480
	 */
	can_nbc_e canNbcType;
	/**
	 * CANbus thread period, ms
	 * offset 484
	 */
	int canSleepPeriodMs = (int)0;
	/**
	 * 'Some triggers could be mounted differently. Most well-known triggers imply specific sensor setup. 4 stroke with symmetrical crank' is a pretty special case for example on Miata NB2
	 * See engineCycle
	 * set operation_mode X
	 * offset 488
	 */
	operation_mode_e ambiguousOperationMode;
	/**
	 * offset 492
	 */
	display_mode_e displayMode;
	/**
	 * offset 496
	 */
	log_format_e logFormat;
	/**
	 * offset 500
	 */
	int byFirmwareVersion = (int)0;
	/**
	 * offset 504
	 */
	int HD44780width = (int)0;
	/**
	 * offset 508
	 */
	int HD44780height = (int)0;
	/**
	 * First TPS, single channel so far. See also pedalPositionAdcChannel
	 * offset 512
	 */
	adc_channel_e tps1_1AdcChannel;
	/**
	 * This is the processor input pin that the battery voltage circuit is connected to, if you are unsure of what pin to use, check the schematic that corresponds to your PCB.
	 * offset 513
	 */
	adc_channel_e vbattAdcChannel;
	/**
	 * This is the processor pin that your fuel level sensor in connected to. This is a non standard input so will need to be user defined.
	 * offset 514
	 */
	adc_channel_e fuelLevelSensor;
	/**
	 * Second throttle body position sensor, single channel so far
	 * offset 515
	 */
	adc_channel_e tps2_1AdcChannel;
	/**
	 * offset 516
	 */
	int overrideCrankingIgnition = (int)0;
	/**
	 * offset 520
	 */
	int sensorChartFrequency = (int)0;
	/**
	 * offset 524
	 */
	trigger_config_s trigger;
	/**
	 * offset 540
	 */
	spi_device_e hip9011SpiDevice;
	/**
	 * offset 541
	 */
	adc_channel_e high_fuel_pressure_sensor_1;
	/**
	 * offset 542
	 */
	adc_channel_e high_fuel_pressure_sensor_2;
	/**
	 * See hasMafSensor
	 * offset 543
	 */
	adc_channel_e mafAdcChannel;
	/**
	 * set global_fuel_correction X
	 * offset 544
	 */
	float globalFuelCorrection = (float)0;
	/**
	 * offset 548
	 */
	float adcVcc = (float)0;
	/**
	 * maximum total number of degrees to subtract from ignition advance
	 * when knocking
	 * offset 552
	 */
	float maxKnockSubDeg = (float)0;
	/**
	 * Camshaft input could be used either just for engine phase detection if your trigger shape does not include cam sensor as 'primary' channel, or it could be used for Variable Valve timing on one of the camshafts.
	 * TODO #660
	 * offset 556
	 */
	brain_input_pin_e camInputs[CAM_INPUTS_COUNT];
	/**
	 * offset 560
	 */
	afr_sensor_s afr;
	/**
	 * Electronic throttle pedal position input
	 * Single channel so far
	 * See also tps1_1AdcChannel
	 * offset 580
	 */
	adc_channel_e throttlePedalPositionAdcChannel;
	/**
	 * offset 581
	 */
	brain_pin_e tle6240_cs;
	/**
	 * offset 582
	 */
	pin_output_mode_e tle6240_csPinMode;
	/**
	 * Throttle Pedal not pressed switch - used on some older vehicles like early Mazda Miata
	 * offset 583
	 */
	switch_input_pin_e throttlePedalUpPin;
	/**
	 * @see hasBaroSensor
	 * offset 584
	 */
	air_pressure_sensor_config_s baroSensor;
	/**
	 * offset 600
	 */
	board_configuration_s bc;
	/**
	offset 1464 bit 0 */
	bool vvtDisplayInverted : 1;
	/**
	 * Enables lambda sensor closed loop feedback for fuelling.
	offset 1464 bit 1 */
	bool fuelClosedLoopCorrectionEnabled : 1;
	/**
	 * Print details into rusEfi console
	offset 1464 bit 2 */
	bool isVerboseIAC : 1;
	/**
	 * Prints ETB details to rusEFI console
	offset 1464 bit 3 */
	bool isVerboseETB : 1;
	/**
	 * If set to true, will use the specified duration for cranking dwell. If set to false, will use the specified dwell angle. Unless you have a really good reason to, leave this set to true to use duration mode.
	offset 1464 bit 4 */
	bool useConstantDwellDuringCranking : 1;
	/**
	 * This options enables data for 'engine sniffer' tab in console, which comes at some CPU price
	offset 1464 bit 5 */
	bool isEngineChartEnabled : 1;
	/**
	 * Sometimes we have a performance issue while printing error
	offset 1464 bit 6 */
	bool silentTriggerError : 1;
	/**
	offset 1464 bit 7 */
	bool useLinearCltSensor : 1;
	/**
	offset 1464 bit 8 */
	bool canReadEnabled : 1;
	/**
	offset 1464 bit 9 */
	bool canWriteEnabled : 1;
	/**
	offset 1464 bit 10 */
	bool useLinearIatSensor : 1;
	/**
	 * See fsioTimingAdjustment
	offset 1464 bit 11 */
	bool useFSIO16ForTimingAdjustment : 1;
	/**
	offset 1464 bit 12 */
	bool tachPulseDurationAsDutyCycle : 1;
	/**
	 * This enables smart alternator control and activates the extra alternator settings.
	offset 1464 bit 13 */
	bool isAlternatorControlEnabled : 1;
	/**
	 * This setting flips the signal from the primary engine speed sensor.
	offset 1464 bit 14 */
	bool invertPrimaryTriggerSignal : 1;
	/**
	 * This setting flips the signal from the secondary engine speed sensor.
	offset 1464 bit 15 */
	bool invertSecondaryTriggerSignal : 1;
	/**
	offset 1464 bit 16 */
	bool cutFuelOnHardLimit : 1;
	/**
	offset 1464 bit 17 */
	bool cutSparkOnHardLimit : 1;
	/**
	offset 1464 bit 18 */
	bool step1fuelCutEnable : 1;
	/**
	offset 1464 bit 19 */
	bool step1SparkCutEnable : 1;
	/**
	offset 1464 bit 20 */
	bool hasFrequencyReportingMapSensor : 1;
	/**
	offset 1464 bit 21 */
	bool useFSIO8ForServo1 : 1;
	/**
	offset 1464 bit 22 */
	bool useFSIO9ForServo2 : 1;
	/**
	offset 1464 bit 23 */
	bool useFSIO10ForServo3 : 1;
	/**
	offset 1464 bit 24 */
	bool useFSIO11ForServo4 : 1;
	/**
	offset 1464 bit 25 */
	bool useFSIO12ForServo5 : 1;
	/**
	offset 1464 bit 26 */
	bool useFSIO15ForIdleRpmAdjustment : 1;
	/**
	 * Sometimes we just have to shut the engine down. Use carefully!
	offset 1464 bit 27 */
	bool useFSIO5ForCriticalIssueEngineStop : 1;
	/**
	 * Sometimes we have to miss injection on purpose to attract driver's attention
	offset 1464 bit 28 */
	bool useFSIO4ForSeriousEngineWarning : 1;
	/**
	offset 1464 bit 29 */
	bool useFSIO12ForIdleOffset : 1;
	/**
	offset 1464 bit 30 */
	bool useFSIO13ForIdleMinValue : 1;
	/**
	offset 1464 bit 31 */
	bool useFSIO6ForRevLimiter : 1;
	/**
	 * offset 1468
	 */
	adc_channel_e hipOutputChannel;
	/**
	 * A/C button input handled as analogue input
	 * offset 1469
	 */
	adc_channel_e acSwitchAdc;
	/**
	 * offset 1470
	 */
	adc_channel_e vRefAdcChannel;
	/**
	 * Expected neutral position
	 * offset 1471
	 */
	uint8_t etbNeutralPosition = (uint8_t)0;
	/**
	 * See also idleRpmPid
	 * offset 1472
	 */
	idle_mode_e idleMode;
	/**
	 * Enable fuel injection - This is default off for new projects as a safety feature, set to "true" to enable fuel injection and further injector settings.
	offset 1476 bit 0 */
	bool isInjectionEnabled : 1;
	/**
	 * Enable ignition - This is default off for new projects as a safety feature, set to "true" to enable ignition and further ignition settings.
	offset 1476 bit 1 */
	bool isIgnitionEnabled : 1;
	/**
	 * When enabled if TPS is held above 95% no fuel is injected while cranking to clear excess fuel from the cylinders.
	offset 1476 bit 2 */
	bool isCylinderCleanupEnabled : 1;
	/**
	offset 1476 bit 3 */
	bool secondTriggerChannelEnabled : 1;
	/**
	offset 1476 bit 4 */
	bool needSecondTriggerInputDeprecated : 1;
	/**
	offset 1476 bit 5 */
	bool isMapAveragingEnabled : 1;
	/**
	 * This setting overrides the normal multiplication values that have been set for the idle air control valve during cranking. If this setting is enabled the "IAC multiplier" table in the Cranking settings tab needs to be adjusted appropriately or potentially no IAC opening will occur.
	offset 1476 bit 6 */
	bool overrideCrankingIacSetting : 1;
	/**
	 * This activates a separate ignition timing table for idle conditions, this can help idle stability by using ignition retard and advance either side of the desired idle speed. Extra retard at low idle speeds will prevent stalling and extra advance at high idle speeds can help reduce engine power and slow the idle speed.
	offset 1476 bit 7 */
	bool useSeparateAdvanceForIdle : 1;
	/**
	offset 1476 bit 8 */
	bool isTunerStudioEnabled : 1;
	/**
	offset 1476 bit 9 */
	bool isWaveAnalyzerEnabled : 1;
	/**
	 * This activates a separate fuel table for Idle, this allows fine tuning of the idle fuelling.
	offset 1476 bit 10 */
	bool useSeparateVeForIdle : 1;
	/**
	 * enable trigger_details
	offset 1476 bit 11 */
	bool verboseTriggerSynchDetails : 1;
	/**
	 * Usually if we have no trigger events that means engine is stopped
	 * Unless we are troubleshooting and spinning the engine by hand - this case a longer
	 * delay is needed
	offset 1476 bit 12 */
	bool isManualSpinningMode : 1;
	/**
	 * This is needed if your coils are individually wired and you wish to use batch injection.
	 * enable two_wire_batch_injection
	offset 1476 bit 13 */
	bool twoWireBatchInjection : 1;
	/**
	 * VR sensors are only precise on rising front
	 * enable trigger_only_front
	offset 1476 bit 14 */
	bool useOnlyRisingEdgeForTrigger : 1;
	/**
	 * This is needed if your coils are individually wired (COP) and you wish to use batch ignition (wasted spark).
	offset 1476 bit 15 */
	bool twoWireBatchIgnition : 1;
	/**
	offset 1476 bit 16 */
	bool useFixedBaroCorrFromMap : 1;
	/**
	 * This activates a separate advance table for cranking conditions, this allows cranking advance to be RPM dependant.
	offset 1476 bit 17 */
	bool useSeparateAdvanceForCranking : 1;
	/**
	 * This enables the various ignition corrections during cranking (IAT, CLT, FSIO and PID idle).
	offset 1476 bit 18 */
	bool useAdvanceCorrectionsForCranking : 1;
	/**
	 * This flag allows to use TPS for ignition lookup while in Speed Density Fuel Mode
	offset 1476 bit 19 */
	bool useTPSAdvanceTable : 1;
	/**
	offset 1476 bit 20 */
	bool etbCalibrationOnStart : 1;
	/**
	offset 1476 bit 21 */
	bool unused_1484_bit_21 : 1;
	/**
	offset 1476 bit 22 */
	bool unused_1484_bit_22 : 1;
	/**
	offset 1476 bit 23 */
	bool unused_1484_bit_23 : 1;
	/**
	offset 1476 bit 24 */
	bool unused_1484_bit_24 : 1;
	/**
	offset 1476 bit 25 */
	bool unused_1484_bit_25 : 1;
	/**
	offset 1476 bit 26 */
	bool unused_1484_bit_26 : 1;
	/**
	offset 1476 bit 27 */
	bool unused_1484_bit_27 : 1;
	/**
	offset 1476 bit 28 */
	bool unused_1484_bit_28 : 1;
	/**
	offset 1476 bit 29 */
	bool unused_1484_bit_29 : 1;
	/**
	offset 1476 bit 30 */
	bool unused_1484_bit_30 : 1;
	/**
	offset 1476 bit 31 */
	bool unused_1484_bit_31 : 1;
	/**
	 * offset 1480
	 */
	uint32_t engineChartSize = (uint32_t)0;
	/**
	 * Relative to the target idle RPM
	 * offset 1484
	 */
	int16_t idlePidRpmUpperLimit = (int16_t)0;
	/**
	 * This sets the temperature above which no priming pulse is used, The value at -40 is reduced until there is no more priming injection at this temperature.
	 * offset 1486
	 */
	int16_t primeInjFalloffTemperature = (int16_t)0;
	/**
	 * At what trigger index should some ignition-related math be executed? This is a performance trick to reduce load on synchronization trigger callback.
	 * offset 1488
	 */
	int ignMathCalculateAtIndex = (int)0;
	/**
	 * offset 1492
	 */
	int16_t acCutoffLowRpm = (int16_t)0;
	/**
	 * offset 1494
	 */
	int16_t acCutoffHighRpm = (int16_t)0;
	/**
	 * offset 1496
	 */
	int16_t acIdleRpmBump = (int16_t)0;
	/**
	 * set warningPeriod X
	 * offset 1498
	 */
	int16_t warningPeriod = (int16_t)0;
	/**
	 * offset 1500
	 */
	float knockDetectionWindowStart = (float)0;
	/**
	 * offset 1504
	 */
	float knockDetectionWindowEnd = (float)0;
	/**
	 * offset 1508
	 */
	float idleStepperReactionTime = (float)0;
	/**
	 * offset 1512
	 */
	float knockVThreshold = (float)0;
	/**
	 * offset 1516
	 */
	pin_input_mode_e fsioInputModes[FSIO_COMMAND_COUNT];
	/**
	 * offset 1532
	 */
	int idleStepperTotalSteps = (int)0;
	/**
	 * TODO: finish this #413
	 * offset 1536
	 */
	float noAccelAfterHardLimitPeriodSecs = (float)0;
	/**
	 * At what trigger index should some MAP-related math be executed? This is a performance trick to reduce load on synchronization trigger callback.
	 * offset 1540
	 */
	int mapAveragingSchedulingAtIndex = (int)0;
	/**
	 * offset 1544
	 */
	float baroCorrPressureBins[BARO_CORR_SIZE];
	/**
	 * offset 1560
	 */
	float baroCorrRpmBins[BARO_CORR_SIZE];
	/**
	 * offset 1576
	 */
	baro_corr_table_t baroCorrTable;
	/**
	 * Cranking fuel correction coefficient based on TPS
	 * offset 1640
	 */
	float crankingTpsCoef[CRANKING_CURVE_SIZE];
	/**
	 * offset 1672
	 */
	float crankingTpsBins[CRANKING_CURVE_SIZE];
	/**
	 * offset 1704
	 */
	float tachPulseDuractionMs = (float)0;
	/**
	 * Trigger cycle index at which we start tach pulse (performance consideration)
	 * offset 1708
	 */
	int tachPulseTriggerIndex = (int)0;
	/**
	 * Length of time the deposited wall fuel takes to dissipate after the start of acceleration. 
	 * offset 1712
	 */
	float wwaeTau = (float)0;
	/**
	 * offset 1716
	 */
	pid_s alternatorControl;
	/**
	 * offset 1736
	 */
	pid_s etb;
	/**
	 * offset 1756
	 */
	float fuelRailPressure = (float)0;
	/**
	 * offset 1760
	 */
	float alternator_derivativeFilterLoss = (float)0;
	/**
	 * offset 1764
	 */
	float alternator_antiwindupFreq = (float)0;
	/**
	 * offset 1768
	 */
	uint8_t unusedFormerWarmupAfrPid[8];
	/**
	 * kPa value which is too low to be true
	 * offset 1776
	 */
	float mapErrorDetectionTooLow = (float)0;
	/**
	 * kPa value which is too high to be true
	 * offset 1780
	 */
	float mapErrorDetectionTooHigh = (float)0;
	/**
	 * RPMs prior to step1rpm point where ignition advance is retarded
	 * offset 1784
	 */
	int step1RpmWindow = (int)0;
	/**
	 * See cltIdleRpmBins
	 * offset 1788
	 */
	pid_s idleRpmPid;
	/**
	 * 0 = No fuel settling on port walls 1 = All the fuel settling on port walls setting this to 0 disables the wall wetting enrichment. 
	 * offset 1808
	 */
	float wwaeBeta = (float)0;
	/**
	 * blue LED on discovery by default
	 * offset 1812
	 */
	brain_pin_e communicationLedPin;
	/**
	 * green LED on discovery by default
	 * offset 1813
	 */
	brain_pin_e runningLedPin;
	/**
	 * offset 1814
	 */
	brain_pin_e binarySerialTxPin;
	/**
	 * offset 1815
	 */
	brain_pin_e binarySerialRxPin;
	/**
	 * offset 1816
	 */
	brain_pin_e auxValves[AUX_DIGITAL_VALVE_COUNT];
	/**
	 *  todo: finish pin migration from hard-coded to configurable?
	 * offset 1818
	 */
	brain_pin_e consoleSerialTxPin;
	/**
	 * todo: finish pin migration from hard-coded to configurable?
	 * offset 1819
	 */
	brain_pin_e consoleSerialRxPin;
	/**
	 * Knock sensor output knock detection threshold depending on current RPM
	 * offset 1820
	 */
	float knockNoise[ENGINE_NOISE_CURVE_SIZE];
	/**
	 * offset 1852
	 */
	float knockNoiseRpmBins[ENGINE_NOISE_CURVE_SIZE];
	/**
	 * offset 1884
	 */
	float throttlePedalUpVoltage = (float)0;
	/**
	 * Pedal in the floor
	 * offset 1888
	 */
	float throttlePedalWOTVoltage = (float)0;
	/**
	 * on ECU start turn fuel pump on to build fuel pressure
	 * offset 1892
	 */
	int16_t startUpFuelPumpDuration = (int16_t)0;
	/**
	 * If RPM is close enough let's leave IAC alone, and maybe engage timing PID correction
	 * offset 1894
	 */
	int16_t idlePidRpmDeadZone = (int16_t)0;
	/**
	 * CLT-based target RPM for automatic idle controller
	 * offset 1896
	 */
	float cltIdleRpmBins[CLT_CURVE_SIZE];
	/**
	 * See idleRpmPid
	 * offset 1960
	 */
	float cltIdleRpm[CLT_CURVE_SIZE];
	/**
	 * This is the target battery voltage the alternator PID control will attempt to maintain
	 * offset 2024
	 */
	float targetVBatt = (float)0;
	/**
	 * Turns off alternator output above specified TPS, enabling this reduced parasitic drag on the engine at full load.
	 * offset 2028
	 */
	float alternatorOffAboveTps = (float)0;
	/**
	 * Prime pulse for cold engine, duration in ms
	 * Linear interpolation between -40F/-40C and fallout temperature
	 * 
	 * See also isFasterEngineSpinUpEnabled
	 * set cranking_priming_pulse X
	 * offset 2032
	 */
	float startOfCrankingPrimingPulse = (float)0;
	/**
	 * This is the duration in cycles that the IAC will take to reach its normal idle position, it can be used to hold the idle higher for a few seconds after cranking to improve startup.
	 * offset 2036
	 */
	int16_t afterCrankingIACtaperDuration = (int16_t)0;
	/**
	 * Extra IAC, in percent between 0 and 100, tapered between zero and idle deactivation TPS value
	 * offset 2038
	 */
	int16_t iacByTpsTaper = (int16_t)0;
	/**
	 * offset 2040
	 */
	brain_pin_e unusedErrorPin;
	/**
	 * offset 2041
	 */
	brain_pin_e warningLedPin;
	/**
	 * offset 2042
	 */
	brain_pin_e unused1234234;
	/**
	 * offset 2043
	 */
	brain_pin_e LIS302DLCsPin;
	/**
	 * This is the number of engine cycles that the TPS position change can occur over, a longer duration will make the enrichment more active but too long may affect steady state driving, a good default is 30-60 cycles. 
	 * offset 2044
	 */
	int tpsAccelLength = (int)0;
	/**
	 * Maximum change delta of TPS percentage over the 'length'. Actual TPS change has to be above this value in order for TPS/TPS acceleration to kick in.
	 * offset 2048
	 */
	float tpsAccelEnrichmentThreshold = (float)0;
	/**
	 * Angle between cam sensor and VVT zero position
	 * set vvt_offset X
	 * offset 2052
	 */
	float vvtOffset = (float)0;
	/**
	 * offset 2056
	 */
	int engineLoadAccelLength = (int)0;
	/**
	 * offset 2060
	 */
	float engineLoadDecelEnleanmentThreshold = (float)0;
	/**
	 * offset 2064
	 */
	float engineLoadDecelEnleanmentMultiplier = (float)0;
	/**
	 * offset 2068
	 */
	float engineLoadAccelEnrichmentThreshold = (float)0;
	/**
	 * offset 2072
	 */
	float engineLoadAccelEnrichmentMultiplier = (float)0;
	/**
	 * offset 2076
	 */
	uint32_t uartConsoleSerialSpeed = (uint32_t)0;
	/**
	 * offset 2080
	 */
	float tpsDecelEnleanmentThreshold = (float)0;
	/**
	 * offset 2084
	 */
	float tpsDecelEnleanmentMultiplier = (float)0;
	/**
	 * ExpAverage alpha coefficient
	 * offset 2088
	 */
	float slowAdcAlpha = (float)0;
	/**
	 * See http://rusefi.com/s/debugmode
	 * 
	 * set debug_mode X
	 * offset 2092
	 */
	debug_mode_e debugMode;
	/**
	 * offset 2096
	 */
	uint32_t unused_former_warmup_target_afr[9];
	/**
	 * kPa value at which we need to cut fuel and spark, 0 if not enabled
	 * offset 2132
	 */
	float boostCutPressure = (float)0;
	/**
	 * offset 2136
	 */
	float mapAccelTaperBins[MAP_ACCEL_TAPER];
	/**
	 * offset 2168
	 */
	float mapAccelTaperMult[MAP_ACCEL_TAPER];
	/**
	 * todo: rename to fsioAnalogInputs
	 * offset 2200
	 */
	adc_channel_e fsioAdc[FSIO_ANALOG_INPUT_COUNT];
	/**
	 * Fixed timing, useful for TDC testing
	 * offset 2204
	 */
	float fixedTiming = (float)0;
	/**
	 * MAP voltage for low point
	 * offset 2208
	 */
	float mapLowValueVoltage = (float)0;
	/**
	 * MAP voltage for low point
	 * offset 2212
	 */
	float mapHighValueVoltage = (float)0;
	/**
	 * EGO value correction
	 * offset 2216
	 */
	float egoValueShift = (float)0;
	/**
	 * offset 2220
	 */
	output_pin_e auxPidPins[AUX_PID_COUNT];
	/**
	 * offset 2224
	 */
	spi_device_e cj125SpiDevice;
	/**
	 * offset 2225
	 */
	pin_output_mode_e cj125CsPinMode;
	/**
	 * This implementation makes a pulse every time one of the coils is charged, using coil dwell for pulse width. See also tachOutputPin
	 * offset 2226
	 */
	brain_pin_e dizzySparkOutputPin;
	/**
	 * offset 2227
	 */
	pin_output_mode_e dizzySparkOutputPinMode;
	/**
	 * This is the IAC position during cranking, some engines start better if given more air during cranking to improve cylinder filling.
	 * offset 2228
	 */
	int crankingIACposition = (int)0;
	/**
	 * offset 2232
	 */
	float tChargeMinRpmMinTps = (float)0;
	/**
	 * offset 2236
	 */
	float tChargeMinRpmMaxTps = (float)0;
	/**
	 * offset 2240
	 */
	float tChargeMaxRpmMinTps = (float)0;
	/**
	 * offset 2244
	 */
	float tChargeMaxRpmMaxTps = (float)0;
	/**
	 * offset 2248
	 */
	fsio_pwm_freq_t auxPidFrequency[AUX_PID_COUNT];
	/**
	 * offset 2256
	 */
	int alternatorPwmFrequency = (int)0;
	/**
	 * offset 2260
	 */
	mass_storage_e storageMode;
	/**
	 * Narrow Band WBO Approximation
	 * offset 2264
	 */
	float narrowToWideOxygenBins[NARROW_BAND_WIDE_BAND_CONVERSION_SIZE];
	/**
	 * offset 2296
	 */
	float narrowToWideOxygen[NARROW_BAND_WIDE_BAND_CONVERSION_SIZE];
	/**
	 * set vvt_mode X
	 * offset 2328
	 */
	vvt_mode_e vvtMode;
	/**
	 * offset 2332
	 */
	bi_quard_s biQuad;
	/**
	 * CLT-based timing correction
	 * offset 2352
	 */
	float cltTimingBins[CLT_TIMING_CURVE_SIZE];
	/**
	 * offset 2384
	 */
	float cltTimingExtra[CLT_TIMING_CURVE_SIZE];
	/**
	 * offset 2416
	 */
	int nbVvtIndex = (int)0;
	/**
	 * offset 2420
	 */
	float autoTuneCltThreshold = (float)0;
	/**
	 * offset 2424
	 */
	float autoTuneTpsRocThreshold = (float)0;
	/**
	 * offset 2428
	 */
	float autoTuneTpsQuietPeriod = (float)0;
	/**
	 * offset 2432
	 */
	float postCrankingTargetClt = (float)0;
	/**
	 * Fuel multiplier taper, see also postCrankingDurationSec
	 * offset 2436
	 */
	float postCrankingFactor = (float)0;
	/**
	 * See also postCrankingFactor
	 * offset 2440
	 */
	float postCrankingDurationSec = (float)0;
	/**
	 * todo: finish implementation #332
	 * offset 2444
	 */
	ThermistorConf auxTempSensor1;
	/**
	 * todo: finish implementation #332
	 * offset 2476
	 */
	ThermistorConf auxTempSensor2;
	/**
	 * offset 2508
	 */
	int16_t fuelClosedLoopCltThreshold = (int16_t)0;
	/**
	 * offset 2510
	 */
	int16_t fuelClosedLoopTpsThreshold = (int16_t)0;
	/**
	 * offset 2512
	 */
	int16_t fuelClosedLoopRpmThreshold = (int16_t)0;
	/**
	 * offset 2514
	 */
	int16_t etbFreq = (int16_t)0;
	/**
	 * offset 2516
	 */
	pid_s fuelClosedLoopPid;
	/**
	 * offset 2536
	 */
	float fuelClosedLoopAfrHighThreshold = (float)0;
	/**
	 * per-cylinder timing correction
	 * offset 2540
	 */
	cfg_float_t_1f timing_offset_cylinder[IGNITION_PIN_COUNT];
	/**
	 * offset 2588
	 */
	float idlePidActivationTime = (float)0;
	/**
	 * offset 2592
	 */
	spi_device_e sdCardSpiDevice;
	/**
	 * offset 2593
	 */
	uint8_t unusedSpiPadding4[3];
	/**
	 * offset 2596
	 */
	pin_mode_e spi1SckMode;
	/**
	 * offset 2597
	 */
	pin_mode_e spi1MosiMode;
	/**
	 * offset 2598
	 */
	pin_mode_e spi1MisoMode;
	/**
	 * offset 2599
	 */
	pin_mode_e spi2SckMode;
	/**
	 * offset 2600
	 */
	pin_mode_e spi2MosiMode;
	/**
	 * offset 2601
	 */
	pin_mode_e spi2MisoMode;
	/**
	 * offset 2602
	 */
	pin_mode_e spi3SckMode;
	/**
	 * offset 2603
	 */
	pin_mode_e spi3MosiMode;
	/**
	 * offset 2604
	 */
	pin_mode_e spi3MisoMode;
	/**
	 * offset 2605
	 */
	pin_output_mode_e stepperEnablePinMode;
	/**
	 * ResetB
	 * offset 2606
	 */
	brain_pin_e mc33816_rstb;
	/**
	 * offset 2607
	 */
	brain_pin_e mc33816_driven;
	/**
	 * Brake pedal switch
	 * offset 2608
	 */
	switch_input_pin_e brakePedalPin;
	/**
	 * lambda input
	 * offset 2609
	 */
	adc_channel_e cj125ua;
	/**
	 * heater input
	 * offset 2610
	 */
	adc_channel_e cj125ur;
	/**
	 * offset 2611
	 */
	pin_input_mode_e brakePedalPinMode;
	/**
	 * offset 2612
	 */
	pid_s auxPid[AUX_PID_COUNT];
	/**
	 * offset 2692
	 */
	oil_pressure_config_s oilPressure;
	/**
	 * offset 2712
	 */
	spi_device_e accelerometerSpiDevice;
	/**
	 * offset 2713
	 */
	uint8_t unusedSpiPadding5[3];
	/**
	 * offset 2716
	 */
	float fsioCurve1Bins[FSIO_CURVE_16];
	/**
	 * offset 2780
	 */
	float fsioCurve1[FSIO_CURVE_16];
	/**
	 * offset 2844
	 */
	float fsioCurve2Bins[FSIO_CURVE_16];
	/**
	 * offset 2908
	 */
	float fsioCurve2[FSIO_CURVE_16];
	/**
	 * offset 2972
	 */
	float fsioCurve3Bins[FSIO_CURVE_8];
	/**
	 * offset 3004
	 */
	float fsioCurve3[FSIO_CURVE_8];
	/**
	 * offset 3036
	 */
	float fsioCurve4Bins[FSIO_CURVE_8];
	/**
	 * offset 3068
	 */
	float fsioCurve4[FSIO_CURVE_8];
	/**
	 * offset 3100
	 */
	uint8_t unusedFlexFuelSensor = (uint8_t)0;
	/**
	 * offset 3101
	 */
	brain_pin_e test557pin;
	/**
	 * offset 3102
	 */
	pin_output_mode_e stepperDirectionPinMode;
	/**
	 * offset 3103
	 */
	adc_channel_e externalKnockSenseAdc;
	/**
	 * offset 3104
	 */
	brain_pin_e stepperEnablePin;
	/**
	 * offset 3105
	 */
	brain_pin_e tle8888_cs;
	/**
	 * offset 3106
	 */
	pin_output_mode_e tle8888_csPinMode;
	/**
	 * offset 3107
	 */
	brain_pin_e mc33816_cs;
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	 * offset 3108
	 */
	float crankingAdvanceBins[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	 * offset 3124
	 */
	float crankingAdvance[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * todo: more comments
	 * offset 3140
	 */
	brain_pin_e servoOutputPins[SERVO_COUNT];
	/**
	 * This sets the RPM limit above which the fuel cut is deactivated, activating this maintains fuel flow at high RPM to help cool pistons
	 * offset 3148
	 */
	int16_t coastingFuelCutRpmHigh = (int16_t)0;
	/**
	 * This sets the RPM limit below which the fuel cut is deactivated, this prevents jerking or issues transitioning to idle
	 * offset 3150
	 */
	int16_t coastingFuelCutRpmLow = (int16_t)0;
	/**
	 * percent between 0 and 100 below which the fuel cut is deactivated, this helps low speed drivability.
	 * offset 3152
	 */
	int16_t coastingFuelCutTps = (int16_t)0;
	/**
	 * Fuel cutoff is deactivated below this coolant threshold.
	 * offset 3154
	 */
	int16_t coastingFuelCutClt = (int16_t)0;
	/**
	 * Increases PID reaction for RPM<target by adding extra percent to PID-error
	 * offset 3156
	 */
	int16_t pidExtraForLowRpm = (int16_t)0;
	/**
	 * MAP value above which fuel injection is re-enabled.
	 * offset 3158
	 */
	int16_t coastingFuelCutMap = (int16_t)0;
	/**
	 * CLT-based idle position for coasting (used in Auto-PID Idle mode)
	 * offset 3160
	 */
	float iacCoastingBins[CLT_CURVE_SIZE];
	/**
	 *  CLT-based idle position for coasting (used in Auto-PID Idle mode)
	 * offset 3224
	 */
	float iacCoasting[CLT_CURVE_SIZE];
	/**
	 * offset 3288
	 */
	ignition_tps_table_t ignitionTpsTable;
	/**
	 * offset 3800
	 */
	float ignitionTpsBins[IGN_TPS_COUNT];
	/**
	 * offset 3864
	 */
	float tChargeAirCoefMin = (float)0;
	/**
	 * offset 3868
	 */
	float tChargeAirCoefMax = (float)0;
	/**
	 * offset 3872
	 */
	float tChargeAirFlowMax = (float)0;
	/**
	 * offset 3876
	 */
	float tChargeAirIncrLimit = (float)0;
	/**
	 * offset 3880
	 */
	float tChargeAirDecrLimit = (float)0;
	/**
	 * offset 3884
	 */
	tChargeMode_e tChargeMode;
	/**
	 * target TPS value, 0 to 100%
	 * TODO: use int8 data date once we template interpolation method
	 * offset 3888
	 */
	float etbBiasBins[ETB_BIAS_CURVE_LENGTH];
	/**
	 * PWM bias, 0 to 100%
	 * offset 3920
	 */
	float etbBiasValues[ETB_BIAS_CURVE_LENGTH];
	/**
	 * offset 3952
	 */
	float hip9011Gain = (float)0;
	/**
	 * iTerm min value
	 * offset 3956
	 */
	int16_t etb_iTermMin = (int16_t)0;
	/**
	 * iTerm max value
	 * offset 3958
	 */
	int16_t etb_iTermMax = (int16_t)0;
	/**
	 * offset 3960
	 */
	float etbDeadband = (float)0;
	/**
	 * offset 3964
	 */
	etb_io etb2;
	/**
	 * See useIdleTimingPidControl
	 * offset 3968
	 */
	pid_s idleTimingPid;
	/**
	 * When the current RPM is closer than this value to the target, closed-loop idle timing control is enabled.
	 * offset 3988
	 */
	int16_t idleTimingPidWorkZone = (int16_t)0;
	/**
	 * If the RPM closer to target than this value, disable timing correction to prevent oscillation
	 * offset 3990
	 */
	int16_t idleTimingPidDeadZone = (int16_t)0;
	/**
	 * Taper out idle timing control over this range as the engine leaves idle conditions
	 * offset 3992
	 */
	int16_t idlePidFalloffDeltaRpm = (int16_t)0;
	/**
	 * A delay in cycles between fuel-enrich. portions
	 * offset 3994
	 */
	int16_t tpsAccelFractionPeriod = (int16_t)0;
	/**
	 * A fraction divisor: 1 or less = entire portion at once, or split into diminishing fractions
	 * offset 3996
	 */
	float tpsAccelFractionDivisor = (float)0;
	/**
	 * offset 4000
	 */
	spi_device_e tle8888spiDevice;
	/**
	 * offset 4001
	 */
	spi_device_e mc33816spiDevice;
	/**
	 * iTerm min value
	 * offset 4002
	 */
	int16_t idlerpmpid_iTermMin = (int16_t)0;
	/**
	 * offset 4004
	 */
	spi_device_e tle6240spiDevice;
	/**
	 * offset 4005
	 */
	uint8_t unusedSpiPadding7 = (uint8_t)0;
	/**
	 * iTerm max value
	 * offset 4006
	 */
	int16_t idlerpmpid_iTermMax = (int16_t)0;
	/**
	 * offset 4008
	 */
	spi_device_e mc33972spiDevice;
	/**
	 * offset 4009
	 */
	uint8_t unusedSpiPadding8[3];
	/**
	 *  ETB idle authority
	 * offset 4012
	 */
	float etbIdleThrottleRange = (float)0;
	/**
	 * offset 4016
	 */
	uint8_t unusuedvref[4];
	/**
	 * offset 4020
	 */
	uint8_t unusuedsw[4];
	/**
	 * offset 4024
	 */
	int alFIn[3];
	/**
	 * offset 4036
	 */
	uint8_t unusedSpiPadding3[4];
	/**
	 * offset 4040
	 */
	pid_s idleRpmPid2;
	/**
	 * offset 4060
	 */
	iac_pid_mult_t iacPidMultTable;
	/**
	 * offset 4124
	 */
	uint8_t iacPidMultLoadBins[IAC_PID_MULT_SIZE];
	/**
	 * offset 4132
	 */
	uint8_t iacPidMultRpmBins[IAC_PID_MULT_SIZE];
	/**
	 * offset 4140
	 */
	int mainUnusedEnd[565];
	/** total size 6400*/
};

typedef struct engine_configuration_s engine_configuration_s;

// start of persistent_config_s
struct persistent_config_s {
	/**
	 * offset 0
	 */
	engine_configuration_s engineConfiguration;
	/**
	 * offset 6400
	 */
	pedal_to_tps_t pedalToTpsTable;
	/**
	 * offset 6464
	 */
	uint8_t pedalToTpsPedalBins[PEDAL_TO_TPS_SIZE];
	/**
	 * offset 6472
	 */
	uint8_t pedalToTpsRpmBins[PEDAL_TO_TPS_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	 * offset 6480
	 */
	float cltCrankingCorrBins[CLT_CRANKING_CURVE_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	 * offset 6512
	 */
	float cltCrankingCorr[CLT_CRANKING_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	 * offset 6544
	 */
	float idleAdvanceBins[IDLE_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	 * offset 6576
	 */
	float idleAdvance[IDLE_ADVANCE_CURVE_SIZE];
	/**
	 * Optional VE table for Idle (see useSeparateVEForIdle)
	 * offset 6608
	 */
	float idleVeBins[IDLE_VE_CURVE_SIZE];
	/**
	 *  Optional VE table for Idle (see useSeparateVEForIdle)
	 * offset 6640
	 */
	float idleVe[IDLE_VE_CURVE_SIZE];
	/**
	 * offset 6672
	 */
	le_formula_t fsioFormulas[FSIO_COMMAND_COUNT];
	/**
	 * offset 9872
	 */
	le_formula_t timingMultiplier;
	/**
	 * offset 10072
	 */
	le_formula_t timingAdditive;
	/**
	 * offset 10272
	 */
	float cltFuelCorrBins[CLT_CURVE_SIZE];
	/**
	 * offset 10336
	 */
	float cltFuelCorr[CLT_CURVE_SIZE];
	/**
	 * offset 10400
	 */
	float iatFuelCorrBins[IAT_CURVE_SIZE];
	/**
	 * offset 10464
	 */
	float iatFuelCorr[IAT_CURVE_SIZE];
	/**
	 * offset 10528
	 */
	float crankingFuelCoef[CRANKING_CURVE_SIZE];
	/**
	 * offset 10560
	 */
	float crankingFuelBins[CRANKING_CURVE_SIZE];
	/**
	 * offset 10592
	 */
	float crankingCycleCoef[CRANKING_CURVE_SIZE];
	/**
	 * offset 10624
	 */
	float crankingCycleBins[CRANKING_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	 * offset 10656
	 */
	float cltIdleCorrBins[CLT_CURVE_SIZE];
	/**
	 *  CLT-based idle position multiplier for simple manual idle controller
	 * offset 10720
	 */
	float cltIdleCorr[CLT_CURVE_SIZE];
	/**
	 * kg/hour value.
	 * By the way 2.081989116 kg/h = 1 ft3/m
	 * offset 10784
	 */
	float mafDecoding[MAF_DECODING_COUNT];
	/**
	 * offset 11808
	 */
	float mafDecodingBins[MAF_DECODING_COUNT];
	/**
	 * offset 12832
	 */
	angle_table_t ignitionIatCorrTable;
	/**
	 * offset 13856
	 */
	float ignitionIatCorrLoadBins[IGN_LOAD_COUNT];
	/**
	 * offset 13920
	 */
	float ignitionIatCorrRpmBins[IGN_RPM_COUNT];
	/**
	 * offset 13984
	 */
	angle_table_t injectionPhase;
	/**
	 * offset 15008
	 */
	float injPhaseLoadBins[FUEL_LOAD_COUNT];
	/**
	 * offset 15072
	 */
	float injPhaseRpmBins[FUEL_RPM_COUNT];
	/**
	 * offset 15136
	 */
	fuel_table_t fuelTable;
	/**
	 * offset 16160
	 */
	float fuelLoadBins[FUEL_LOAD_COUNT];
	/**
	 * RPM is float and not integer in order to use unified methods for interpolation
	 * offset 16224
	 */
	float fuelRpmBins[FUEL_RPM_COUNT];
	/**
	 * offset 16288
	 */
	ignition_table_t ignitionTable;
	/**
	 * offset 17312
	 */
	float ignitionLoadBins[IGN_LOAD_COUNT];
	/**
	 * offset 17376
	 */
	float ignitionRpmBins[IGN_RPM_COUNT];
	/**
	 * offset 17440
	 */
	ve_table_t veTable;
	/**
	 * offset 18464
	 */
	float veLoadBins[FUEL_LOAD_COUNT];
	/**
	 * offset 18528
	 */
	float veRpmBins[FUEL_RPM_COUNT];
	/**
	 * offset 18592
	 */
	afr_table_t afrTable;
	/**
	 * offset 18848
	 */
	float afrLoadBins[FUEL_LOAD_COUNT];
	/**
	 * offset 18912
	 */
	float afrRpmBins[FUEL_RPM_COUNT];
	/**
	 * offset 18976
	 */
	tps_tps_table_t tpsTpsAccelTable;
	/**
	 * offset 19232
	 */
	float tpsTpsAccelFromRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	 * RPM is float and not integer in order to use unified methods for interpolation
	 * offset 19264
	 */
	float tpsTpsAccelToRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	 * offset 19296
	 */
	fsio_table_8x8_f32t fsioTable1;
	/**
	 * offset 19552
	 */
	float fsioTable1LoadBins[FSIO_TABLE_8];
	/**
	 * RPM is float and not integer in order to use unified methods for interpolation
	 * offset 19584
	 */
	float fsioTable1RpmBins[FSIO_TABLE_8];
	/**
	 * offset 19616
	 */
	fsio_table_8x8_u8t fsioTable2;
	/**
	 * offset 19680
	 */
	float fsioTable2LoadBins[FSIO_TABLE_8];
	/**
	 * RPM is float and not integer in order to use unified methods for interpolation
	 * offset 19712
	 */
	float fsioTable2RpmBins[FSIO_TABLE_8];
	/**
	 * offset 19744
	 */
	fsio_table_8x8_u8t fsioTable3;
	/**
	 * offset 19808
	 */
	float fsioTable3LoadBins[FSIO_TABLE_8];
	/**
	 * RPM is float and not integer in order to use unified methods for interpolation
	 * offset 19840
	 */
	float fsioTable3RpmBins[FSIO_TABLE_8];
	/**
	 * offset 19872
	 */
	fsio_table_8x8_u8t fsioTable4;
	/**
	 * offset 19936
	 */
	float fsioTable4LoadBins[FSIO_TABLE_8];
	/**
	 * RPM is float and not integer in order to use unified methods for interpolation
	 * offset 19968
	 */
	float fsioTable4RpmBins[FSIO_TABLE_8];
	/** total size 20000*/
};

typedef struct persistent_config_s persistent_config_s;

#endif
// end
// this section was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/rusefi_config.txt Mon Nov 25 01:00:42 EST 2019
