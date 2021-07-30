// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on gen_config.sh integration/rusefi_config.txt Fri Jul 30 21:18:00 UTC 2021
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
#define accelerometerSpiDevice_offset 2712
#define acFanPin_offset 762
#define acFanPinMode_offset 763
#define acIdleExtraMin_offset 761
#define acIdleExtraOffset_offset 711
#define acIdleRpmBump_offset 1496
#define acRelayPin_offset 830
#define acRelayPinMode_offset 831
#define acSwitch_offset 1469
#define acSwitchMode_offset 4516
#define adc_channel_e_auto_enum "EFI_ADC_NONE", "EFI_ADC_0", "EFI_ADC_1", "EFI_ADC_2", "EFI_ADC_3", "EFI_ADC_4", "EFI_ADC_5", "EFI_ADC_6", "EFI_ADC_7", "EFI_ADC_8", "EFI_ADC_9", "EFI_ADC_10", "EFI_ADC_11", "EFI_ADC_12", "EFI_ADC_13", "EFI_ADC_14", "EFI_ADC_15", "EFI_ADC_LAST_CHANNEL", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "EFI_ADC_ERROR"
#define adc_channel_e_enum "Disabled", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PB0", "PB1", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5"
#define adc_channel_mode_e_auto_enum "ADC_OFF", "ADC_SLOW", "ADC_FAST"
#define ADC_CHANNEL_NONE 0
#define adcVcc_offset 548
#define afr_alignmentFill_afr_offset 561
#define afr_hwChannel_offset 560
#define afr_offset 560
#define afr_override_e_auto_enum "AFR_None", "AFR_MAP", "AFR_Tps", "AFR_AccPedal", "AFR_CylFilling"
#define afr_override_e_enum "None", "MAP", "TPS", "Acc Pedal", "Cyl Filling %"
#define afr_sensor_s_size 20
#define afr_type_offset 692
#define afr_v1_offset 564
#define afr_v2_offset 572
#define afr_value1_offset 568
#define afr_value2_offset 576
#define afrOverrideMode_offset 2111
#define afterCrankingIACtaperDuration_offset 2036
#define AFTERSTART_DECAY_CURVE_SIZE 8
#define AFTERSTART_ENRICH_CURVE_SIZE 8
#define AFTERSTART_HOLD_CURVE_SIZE 8
#define afterstartCoolantBins_offset 6120
#define afterstartDecayTime_offset 6216
#define afterstartEnrich_offset 6184
#define afterstartHoldTime_offset 6152
#define air_pressure_sensor_config_s_size 16
#define air_pressure_sensor_type_e_auto_enum "MT_CUSTOM", "MT_DENSO183", "MT_MPX4250", "MT_HONDA3BAR", "MT_DODGE_NEON_2003", "MT_SUBY_DENSO", "MT_GM_3_BAR", "MT_MPX4100", "MT_TOYOTA_89420_02010", "MT_MPX4250A", "MT_BOSCH_2_5", "MT_MAZDA_1_BAR", "MT_GM_2_BAR", "MT_GM_1_BAR"
#define alignEngineSnifferAtTDC_offset 744
#define alignmentFill_at_2227_offset 2227
#define alignmentFill_at_2253_offset 2253
#define alignmentFill_at_4539_offset 4539
#define alternator_antiwindupFreq_offset 1764
#define alternator_derivativeFilterLoss_offset 1760
#define alternatorControl_dFactor_offset 1724
#define alternatorControl_iFactor_offset 1720
#define alternatorControl_maxValue_offset 1734
#define alternatorControl_minValue_offset 1732
#define alternatorControl_offset 1716
#define alternatorControl_offset_offset 1728
#define alternatorControl_periodMs_offset 1730
#define alternatorControl_pFactor_offset 1716
#define alternatorControlPin_offset 665
#define alternatorControlPinMode_offset 666
#define alternatorOffAboveTps_offset 2028
#define alternatorPwmFrequency_offset 2256
#define ambiguousOperationMode_offset 488
#define analogInputDividerCoefficient_offset 460
#define antiLagActivatePin_offset 990
#define antiLagActivationMode_e_auto_enum "SWITCH_INPUT_ANTILAG", "ALWAYS_ON_ANTILAG"
#define antiLagActivationMode_e_enum "Always Active", "Switch Input"
#define antiLagActivationMode_offset 1024
#define antiLagEnabled_offset 76
#define antiLagRpmTreshold_offset 824
#define applyNonlinearBelowPulse_offset 2508
#define AUX_DIGITAL_VALVE_COUNT 2
#define auxFastSensor1_adcChannel_offset 680
#define auxPid1_dFactor_offset 2620
#define auxPid1_iFactor_offset 2616
#define auxPid1_maxValue_offset 2630
#define auxPid1_minValue_offset 2628
#define auxPid1_offset 2612
#define auxPid1_offset_offset 2624
#define auxPid1_periodMs_offset 2626
#define auxPid1_pFactor_offset 2612
#define auxPid2_dFactor_offset 2640
#define auxPid2_iFactor_offset 2636
#define auxPid2_maxValue_offset 2650
#define auxPid2_minValue_offset 2648
#define auxPid2_offset 2632
#define auxPid2_offset_offset 2644
#define auxPid2_periodMs_offset 2646
#define auxPid2_pFactor_offset 2632
#define auxPidFrequency1_offset 2248
#define auxPidFrequency2_offset 2250
#define auxPidPins1_offset 2220
#define auxPidPins2_offset 2221
#define auxPidPins3_offset 2222
#define auxPidPins4_offset 2223
#define auxSerialRxPin_offset 2042
#define auxSerialSpeed_offset 2096
#define auxSerialTxPin_offset 2040
#define auxTempSensor1_adcChannel_offset 2472
#define auxTempSensor1_alignmentFill_at_29_offset 2473
#define auxTempSensor1_bias_resistor_offset 2468
#define auxTempSensor1_config_offset 2444
#define auxTempSensor1_offset 2444
#define auxTempSensor1_resistance_1_offset 2456
#define auxTempSensor1_resistance_2_offset 2460
#define auxTempSensor1_resistance_3_offset 2464
#define auxTempSensor1_tempC_1_offset 2444
#define auxTempSensor1_tempC_2_offset 2448
#define auxTempSensor1_tempC_3_offset 2452
#define auxTempSensor2_adcChannel_offset 2504
#define auxTempSensor2_alignmentFill_at_29_offset 2505
#define auxTempSensor2_bias_resistor_offset 2500
#define auxTempSensor2_config_offset 2476
#define auxTempSensor2_offset 2476
#define auxTempSensor2_resistance_1_offset 2488
#define auxTempSensor2_resistance_2_offset 2492
#define auxTempSensor2_resistance_3_offset 2496
#define auxTempSensor2_tempC_1_offset 2476
#define auxTempSensor2_tempC_2_offset 2480
#define auxTempSensor2_tempC_3_offset 2484
#define auxValves1_offset 1816
#define auxValves2_offset 1817
#define BANKS_COUNT 2
#define BARO_CORR_SIZE 4
#define baroCorrPressureBins_offset 1544
#define baroCorrRpmBins_offset 1560
#define baroCorrTable_offset 1576
#define baroSensor_alignmentFill_offset 597
#define baroSensor_highValue_offset 588
#define baroSensor_hwChannel_offset 596
#define baroSensor_lowValue_offset 584
#define baroSensor_offset 584
#define baroSensor_type_offset 592
#define binarySerialRxPin_offset 1815
#define binarySerialTxPin_offset 1814
#define BLOCKING_FACTOR 256
#define BOOST_LOAD_COUNT 8
#define BOOST_RPM_COUNT 8
#define boostControlPin_offset 988
#define boostControlPinMode_offset 989
#define boostControlSafeDutyCycle_offset 542
#define boostCutPressure_offset 2132
#define boostPid_dFactor_offset 1000
#define boostPid_iFactor_offset 996
#define boostPid_maxValue_offset 1010
#define boostPid_minValue_offset 1008
#define boostPid_offset 992
#define boostPid_offset_offset 1004
#define boostPid_periodMs_offset 1006
#define boostPid_pFactor_offset 992
#define boostPwmFrequency_offset 1016
#define boostRpmBins_offset 6320
#define boostTableClosedLoop_offset 6328
#define boostTableOpenLoop_offset 6248
#define boostTpsBins_offset 6392
#define boostType_e_auto_enum "OPEN_LOOP", "CLOSED_LOOP"
#define boostType_offset 1012
#define brain_input_pin_e_enum "NONE", "INVALID", "INVALID", "PA1", "PA2", "PA3", "INVALID", "PA5", "PA6", "PA7", "PA8", "PA9", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "PA15", "INVALID", "INVALID", "INVALID", "PB3", "PB4", "PB5", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "PC6", "PC7", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "PE5", "PE6", "INVALID", "INVALID", "PE9", "INVALID", "PE11", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID"
#define brain_pin_e_auto_enum "GPIO_UNASSIGNED", "GPIO_INVALID", "GPIOA_0", "GPIOA_1", "GPIOA_2", "GPIOA_3", "GPIOA_4", "GPIOA_5", "GPIOA_6", "GPIOA_7", "GPIOA_8", "GPIOA_9", "GPIOA_10", "GPIOA_11", "GPIOA_12", "GPIOA_13", "GPIOA_14", "GPIOA_15", "GPIOB_0", "GPIOB_1", "GPIOB_2", "GPIOB_3", "GPIOB_4", "GPIOB_5", "GPIOB_6", "GPIOB_7", "GPIOB_8", "GPIOB_9", "GPIOB_10", "GPIOB_11", "GPIOB_12", "GPIOB_13", "GPIOB_14", "GPIOB_15", "GPIOC_0", "GPIOC_1", "GPIOC_2", "GPIOC_3", "GPIOC_4", "GPIOC_5", "GPIOC_6", "GPIOC_7", "GPIOC_8", "GPIOC_9", "GPIOC_10", "GPIOC_11", "GPIOC_12", "GPIOC_13", "GPIOC_14", "GPIOC_15", "GPIOD_0", "GPIOD_1", "GPIOD_2", "GPIOD_3", "GPIOD_4", "GPIOD_5", "GPIOD_6", "GPIOD_7", "GPIOD_8", "GPIOD_9", "GPIOD_10", "GPIOD_11", "GPIOD_12", "GPIOD_13", "GPIOD_14", "GPIOD_15", "GPIOE_0", "GPIOE_1", "GPIOE_2", "GPIOE_3", "GPIOE_4", "GPIOE_5", "GPIOE_6", "GPIOE_7", "GPIOE_8", "GPIOE_9", "GPIOE_10", "GPIOE_11", "GPIOE_12", "GPIOE_13", "GPIOE_14", "GPIOE_15", "GPIOF_0", "GPIOF_1", "GPIOF_2", "GPIOF_3", "GPIOF_4", "GPIOF_5", "GPIOF_6", "GPIOF_7", "GPIOF_8", "GPIOF_9", "GPIOF_10", "GPIOF_11", "GPIOF_12", "GPIOF_13", "GPIOF_14", "GPIOF_15", "GPIOG_0", "GPIOG_1", "GPIOG_2", "GPIOG_3", "GPIOG_4", "GPIOG_5", "GPIOG_6", "GPIOG_7", "GPIOG_8", "GPIOG_9", "GPIOG_10", "GPIOG_11", "GPIOG_12", "GPIOG_13", "GPIOG_14", "GPIOG_15", "GPIOH_0", "GPIOH_1", "GPIOH_2", "GPIOH_3", "GPIOH_4", "GPIOH_5", "GPIOH_6", "GPIOH_7", "GPIOH_8", "GPIOH_9", "GPIOH_10", "GPIOH_11", "GPIOH_12", "GPIOH_13", "GPIOH_14", "GPIOH_15", "GPIOI_0", "GPIOI_1", "GPIOI_2", "GPIOI_3", "GPIOI_4", "GPIOI_5", "GPIOI_6", "GPIOI_7", "GPIOI_8", "GPIOI_9", "GPIOI_10", "GPIOI_11", "GPIOI_12", "GPIOI_13", "GPIOI_14", "GPIOI_15", "MC33972_PIN_1", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "MC33972_PIN_21", "MC33972_PIN_22", "TLE8888_PIN_1", "TLE8888_PIN_2", "TLE8888_PIN_3", "TLE8888_PIN_4", "TLE8888_PIN_5", "TLE8888_PIN_6", "TLE8888_PIN_7", "TLE8888_PIN_8", "TLE8888_PIN_9", "TLE8888_PIN_10", "TLE8888_PIN_11", "TLE8888_PIN_12", "TLE8888_PIN_13", "TLE8888_PIN_14", "TLE8888_PIN_15", "TLE8888_PIN_16", "TLE8888_PIN_17", "TLE8888_PIN_18", "TLE8888_PIN_19", "TLE8888_PIN_20", "TLE8888_PIN_21", "TLE8888_PIN_22", "TLE8888_PIN_23", "TLE8888_PIN_24", "TLE8888_PIN_25", "TLE8888_PIN_26", "TLE8888_PIN_27", "TLE8888_PIN_28", "TLE8888_PIN_MR", "TLE8888_PIN_KEY", "TLE8888_PIN_WAKE", "TLE6240_PIN_1", "TLE6240_PIN_2", "TLE6240_PIN_3", "TLE6240_PIN_4", "TLE6240_PIN_5", "TLE6240_PIN_6", "TLE6240_PIN_7", "TLE6240_PIN_8", "TLE6240_PIN_9", "TLE6240_PIN_10", "TLE6240_PIN_11", "TLE6240_PIN_12", "TLE6240_PIN_13", "TLE6240_PIN_14", "TLE6240_PIN_15", "TLE6240_PIN_16"
#define brain_pin_e_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PA8", "PA9", "PA10", "PA11", "PA12", "PA13", "PA14", "PA15", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PB8", "PB9", "PB10", "PB11", "PB12", "PB13", "PB14", "PB15", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "PC10", "PC11", "PC12", "PC13", "PC14", "PC15", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PD8", "PD9", "PD10", "PD11", "PD12", "PD13", "PD14", "PD15", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "PE12", "PE13", "PE14", "PE15", "PF0", "PF1", "PF2", "PF3", "PF4", "PF5", "PF6", "PF7", "PF8", "PF9", "PF10", "PF11", "PF12", "PF13", "PF14", "PF15", "PG0", "PG1", "PG2", "PG3", "PG4", "PG5", "PG6", "PG7", "PG8", "PG9", "PG10", "PG11", "PG12", "PG13", "PG14", "PG15", "PH0", "PH1", "PH2", "PH3", "PH4", "PH5", "PH6", "PH7", "PH8", "PH9", "PH10", "PH11", "PH12", "PH13", "PH14", "PH15", "PI0", "PI1", "PI2", "PI3", "PI4", "PI5", "PI6", "PI7", "PI8", "PI9", "PI10", "PI11", "PI12", "PI13", "PI14", "PI15", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID"
#define brakePedalPin_offset 2608
#define brakePedalPinMode_offset 2611
#define byFirmwareVersion_offset 500
#define CAM_INPUTS_COUNT 4
#define CAM_INPUTS_COUNT_padding 0
#define camInputs1_offset 556
#define camInputs2_offset 557
#define camInputs3_offset 558
#define camInputs4_offset 559
#define camInputsDebug1_offset 1492
#define camInputsDebug2_offset 1493
#define camInputsDebug3_offset 1494
#define camInputsDebug4_offset 1495
#define camInputsDebugPadding_offset 1496
#define camInputsPadding_offset 560
#define CAMS_PER_BANK 2
#define CAMS_PER_BANK_padding 0
#define can2BaudRate_offset 2110
#define can2NbcType_offset 2120
#define can2ReadEnabled_offset 2116
#define can2RxPin_offset 2125
#define can2SleepPeriodMs_offset 712
#define can2TxPin_offset 2124
#define can2WriteEnabled_offset 2116
#define can_baudrate_e_auto_enum "B100KBPS", "B250KBPS", "B500KBPS", "B1MBPS"
#define can_baudrate_e_enum "100kbps", "250kbps", "500kbps", "1Mbps"
#define CAN_DEFAULT_BASE 0x200
#define can_nbc_e_auto_enum "CAN_BUS_NBC_NONE", "CAN_BUS_NBC_FIAT", "CAN_BUS_NBC_VAG", "CAN_BUS_MAZDA_RX8", "CAN_BUS_NBC_BMW", "CAN_BUS_W202_C180", "CAN_BUS_BMW_E90", "CAN_BUS_Haltech", "CAN_BUS_MQB", "CAN_BUS_NISSAN_VQ", "CAN_BUS_GENESIS_COUPE"
#define CAN_RX_PREFIX "CAN_rx"
#define can_vss_nbc_e_auto_enum "BMW_e46", "W202"
#define canBaudRate_offset 2108
#define canNbcType_offset 480
#define canReadEnabled_offset 1464
#define canRxPin_offset 709
#define canSleepPeriodMs_offset 484
#define canTxPin_offset 708
#define canVssNbcType_offset 4140
#define canWriteEnabled_offset 1464
#define cdmInputPin_offset 937
#define cj125CsPin_offset 674
#define cj125CsPinMode_offset 2225
#define cj125isLsu49_offset 76
#define cj125isUaDivided_offset 76
#define cj125isUrDivided_offset 76
#define cj125SpiDevice_offset 2224
#define cj125ua_offset 2609
#define cj125ur_offset 2610
#define clt_adcChannel_offset 280
#define clt_alignmentFill_at_29_offset 281
#define clt_bias_resistor_offset 276
#define clt_config_offset 252
#define CLT_CRANKING_CURVE_SIZE 8
#define CLT_CURVE_SIZE 16
#define CLT_LIMITER_CURVE_SIZE 4
#define clt_offset 252
#define clt_resistance_1_offset 264
#define clt_resistance_2_offset 268
#define clt_resistance_3_offset 272
#define clt_tempC_1_offset 252
#define clt_tempC_2_offset 256
#define clt_tempC_3_offset 260
#define CLT_TIMING_CURVE_SIZE 8
#define cltCrankingCorr_offset 6512
#define cltCrankingCorrBins_offset 6480
#define cltFuelCorr_offset 10336
#define cltFuelCorrBins_offset 10272
#define cltIdleCorr_offset 10720
#define cltIdleCorrBins_offset 10656
#define cltIdleRpm_offset 1960
#define cltIdleRpmBins_offset 1896
#define cltRevLimitRpm_offset 3332
#define cltRevLimitRpmBins_offset 3328
#define cltTimingBins_offset 2352
#define cltTimingExtra_offset 2384
#define clutchDownPin_offset 664
#define clutchDownPinInverted_offset 976
#define clutchDownPinMode_offset 667
#define clutchUpPin_offset 969
#define clutchUpPinInverted_offset 976
#define clutchUpPinMode_offset 971
#define CMD_AC_RELAY_BENCH "acrelaybench"
#define CMD_ALTERNATOR_PIN "set_alternator_pin"
#define CMD_BOOST_PIN "set_boost_pin"
#define CMD_BURNCONFIG "burnconfig"
#define CMD_CALIBRATE_PEDAL_DOWN "calibrate_pedal_down"
#define CMD_CALIBRATE_PEDAL_UP "calibrate_pedal_up"
#define CMD_DATE "date"
#define CMD_DISABLE "disable"
#define CMD_ENABLE "enable"
#define CMD_ENGINE_TYPE "engine_type"
#define CMD_ENGINESNIFFERRPMTHRESHOLD "engineSnifferRpmThreshold"
#define CMD_ETB_DUTY "set_etb_duty"
#define CMD_EXTERNAL_STIMULATION "ext_stimulation"
#define CMD_FAN2_BENCH "fan2bench"
#define CMD_FAN_BENCH "fanbench"
#define CMD_FUEL_BENCH "fuelbench"
#define CMD_FUNCTIONAL_TEST_MODE "test_mode"
#define CMD_HPFP_BENCH "hpfpbench"
#define CMD_IDLE_PIN "set_idle_pin"
#define CMD_IGNITION "ignition"
#define CMD_IGNITION_PIN "set_ignition_pin"
#define CMD_INJECTION "injection"
#define CMD_INJECTION_PIN "set_injection_pin"
#define CMD_LOGIC_PIN "set_logic_input_pin"
#define CMD_MIL_BENCH "milbench"
#define CMD_PINS "pins"
#define CMD_PWM "pwm"
#define CMD_REBOOT "reboot"
#define CMD_REBOOT_DFU "reboot_dfu"
#define CMD_RESET_ENGINE_SNIFFER "reset_engine_chart"
#define CMD_RPM "rpm"
#define CMD_SELF_STIMULATION "self_stimulation"
#define CMD_SPARK_BENCH "sparkbench"
#define CMD_STARTER_BENCH "starterbench"
#define CMD_TRIGGER_HW_INPUT "trigger_hw_input"
#define CMD_TRIGGER_PIN "set_trigger_input_pin"
#define CMD_TRIGGER_SIMULATOR_PIN "set_trigger_simulator_pin"
#define CMD_TRIGGERINFO "triggerinfo"
#define CMD_TS_BENCH_AC_COMPRESSOR_RELAY 6
#define CMD_TS_BENCH_CATEGORY 22
#define CMD_TS_BENCH_CHECK_ENGINE_LIGHT 7
#define CMD_TS_BENCH_FAN_RELAY 4
#define CMD_TS_BENCH_FAN_RELAY_2 5
#define CMD_TS_BENCH_FUEL_PUMP 1
#define CMD_TS_BENCH_GPPWM1_VALVE 10
#define CMD_TS_BENCH_GPPWM2_VALVE 11
#define CMD_TS_BENCH_GPPWM3_VALVE 12
#define CMD_TS_BENCH_GPPWM4_VALVE 13
#define CMD_TS_BENCH_HPFP_VALVE 9
#define CMD_TS_BENCH_IDLE_VALVE 8
#define CMD_TS_BENCH_MAIN_RELAY 0
#define CMD_TS_BENCH_STARTER_DISABLE_RELAY 3
#define CMD_TS_BENCH_STARTER_ENABLE_RELAY 2
#define CMD_TS_FSIO_CATEGORY 33
#define CMD_TS_IGNITION_CATEGORY 18
#define CMD_TS_INJECTOR_CATEGORY 19
#define CMD_TS_SOLENOID_CATEGORY 41
#define CMD_TS_X14 20
#define CMD_TS_X17 23
#define CMD_TS_X18 24
#define CMD_TS_X31 49
#define CMD_VSS_PIN "vss_pin"
#define CMD_WRITECONFIG "writeconfig"
#define coastingFuelCutClt_offset 3154
#define coastingFuelCutEnabled_offset 744
#define coastingFuelCutMap_offset 3158
#define coastingFuelCutRpmHigh_offset 3148
#define coastingFuelCutRpmLow_offset 3150
#define coastingFuelCutTps_offset 3152
#define communicationLedPin_offset 1812
#define COMPOSITE_DATA_LENGTH 2500
#define COMPOSITE_DATA_LENGTH_HALF 1250
#define COMPOSITE_PACKET_COUNT 500
#define COMPOSITE_PACKET_SIZE 5
#define compressionRatio_offset 732
#define CONSOLE_DATA_PROTOCOL_TAG " @"
#define consoleUartDevice_offset 943
#define consumeObdSensors_offset 976
#define CRANKING_ADVANCE_CURVE_SIZE 4
#define cranking_alignmentFill_at_6_offset 94
#define cranking_baseFuel_offset 88
#define CRANKING_CLT_IDLE_CURVE_SIZE 8
#define CRANKING_CURVE_SIZE 8
#define cranking_offset 88
#define cranking_parameters_s_size 8
#define cranking_rpm_offset 92
#define crankingAdvance_offset 3124
#define crankingAdvanceBins_offset 3108
#define crankingCycleBins_offset 10624
#define crankingCycleCoef_offset 10592
#define crankingFuelBins_offset 10560
#define crankingFuelCoef_offset 10528
#define crankingIACposition_offset 2228
#define crankingInjectionMode_offset 424
#define crankingTimingAngle_offset 436
#define crankingTpsBins_offset 1672
#define crankingTpsCoef_offset 1640
#define CRITICAL_PREFIX "CRITICAL"
#define cutFuelOnHardLimit_offset 1464
#define cutSparkOnHardLimit_offset 1464
#define cylinderBankSelect10_offset 4025
#define cylinderBankSelect11_offset 4026
#define cylinderBankSelect12_offset 4027
#define cylinderBankSelect1_offset 4016
#define cylinderBankSelect2_offset 4017
#define cylinderBankSelect3_offset 4018
#define cylinderBankSelect4_offset 4019
#define cylinderBankSelect5_offset 4020
#define cylinderBankSelect6_offset 4021
#define cylinderBankSelect7_offset 4022
#define cylinderBankSelect8_offset 4023
#define cylinderBankSelect9_offset 4024
#define cylinderBore_offset 408
#define cylindersCount_offset 400
#define DBG_DBG_LOGIC_ANALYZER 45
#define dc_io_size 4
#define DC_PER_STEPPER 2
#define debug_mode_e_auto_enum "DBG_ALTERNATOR_PID", "DBG_TPS_ACCEL", "DBG_GPPWM", "DBG_IDLE_CONTROL", "DBG_EL_ACCEL", "DBG_TRIGGER_COUNTERS", "DBG_FSIO_ADC", "DBG_VVT_1_PID", "DBG_VVT", "DBG_CRANKING_DETAILS", "DBG_IGNITION_TIMING", "DBG_FUEL_PID_CORRECTION", "DBG_VEHICLE_SPEED_SENSOR", "DBG_SD_CARD", "DBG_SR5_PROTOCOL", "DBG_KNOCK", "DBG_16", "DBG_ELECTRONIC_THROTTLE_PID", "DBG_EXECUTOR", "DBG_BENCH_TEST", "DBG_AUX_VALVES", "DBG_ANALOG_INPUTS", "DBG_INSTANT_RPM", "DBG_FSIO_EXPRESSION_1_7", "DBG_STATUS", "DBG_CJ125", "DBG_CAN", "DBG_MAP", "DBG_METRICS", "DBG_ELECTRONIC_THROTTLE_EXTRA", "DBG_ION", "DBG_TLE8888", "DBG_ANALOG_INPUTS2", "DBG_DWELL_METRIC", "DBG_34", "DBG_ETB_LOGIC", "DBG_BOOST", "DBG_START_STOP", "DBG_LAUNCH", "DBG_ETB_AUTOTUNE", "DBG_COMPOSITE_LOG", "DBG_FSIO_EXPRESSION_8_14", "DBG_FSIO_SPECIAL", "DBG_INJECTOR_COMPENSATION", "DBG_DYNO_VIEW", "DBG_LOGIC_ANALYZER", "DBG_RUSEFI_WIDEBAND", "DBG_TCU", "DBG_LUA", "DBG_VVT_2_PID", "DBG_VVT_3_PID", "DBG_VVT_4_PID", "MODE_52", "MODE_53"
#define debug_mode_e_enum "Alternator PID", "TPS acceleration enrichment", "GPPWM", "Idle Control", "Engine Load accl enrich", "Trigger Counters", "FSIO_ADC", "VVT1 PID", "VVT input", "Cranking", "Timing", "Closed-loop fuel corr PID", "VSS", "SD card", "sr5", "Knock", "mode16", "Electronic Throttle", "Executor", "Bench Test / TS commands", "Aux Valves", "Analog inputs #1", "INSTANT_RPM", "FSIO_EXPRESSION_1_7", "Status", "CJ125", "CAN", "MAP", "Metrics", "ETB#2", "Ion Sense", "TLE8888", "Analog inputs #2", "Dwell Metric", "INVALID", "ETB Logic", "Boost Control", "Start/Stop", "Launch", "ETB Autotune", "FSIO_COMPOSITE_LOG", "FSIO_EXPRESSION_8_14", "FSIO_SPECIAL", "Injector Compensation", "Dyno_View", "Logic_Analyzer", "rusEFI Wideband", "TCU", "Lua", "VVT2 PID", "VVT3 PID", "VVT4 PID", "mode 52", "mode 53"
#define debugMapAveraging_offset 807
#define debugMode_offset 2092
#define debugTriggerSync_offset 676
#define DIGIPOT_COUNT 4
#define digitalPotentiometerChipSelect1_offset 668
#define digitalPotentiometerChipSelect2_offset 669
#define digitalPotentiometerChipSelect3_offset 670
#define digitalPotentiometerChipSelect4_offset 671
#define digitalPotentiometerSpiDevice_offset 677
#define disableEtbWhenEngineStopped_offset 744
#define disableFan1WhenStopped_offset 76
#define disableFan2WhenStopped_offset 76
#define displacement_offset 396
#define display_mode_e_auto_enum "DM_NONE", "DM_HD44780", "DM_HD44780_OVER_PCF8574"
#define displayLogicLevelsInEngineSniffer_offset 76
#define displayMode_offset 492
#define drv8860_cs_offset 1201
#define drv8860_csPinMode_offset 1202
#define drv8860_miso_offset 1203
#define drv8860spiDevice_offset 1200
#define DWELL_CURVE_SIZE 8
#define ego_sensor_e_auto_enum "ES_BPSX_D1", "ES_Innovate_MTX_L", "ES_14Point7_Free", "ES_NarrowBand", "ES_PLX", "ES_Custom", "ES_AEM"
#define ego_sensor_e_enum "BPSX", "Innovate", "14Point7", "Narrow", "PLX", "Custom", "INVALID", "INVALID"
#define egoValueShift_offset 2216
#define EGT_CHANNEL_COUNT 8
#define electronicThrottlePin1Mode_offset 672
#define enableAemXSeries_offset 744
#define enableCanVss_offset 976
#define enabledStep1Limiter_offset 744
#define enableFan1WithAc_offset 76
#define enableFan2WithAc_offset 76
#define enableInnovateLC2_offset 976
#define enableLaunchBoost_offset 976
#define enableLaunchRetard_offset 976
#define enableMapEstimationTableFallback_offset 76
#define enableSoftwareKnock_offset 976
#define enableTrailingSparks_offset 76
#define enableVerboseCan2Tx_offset 2116
#define enableVerboseCanTx_offset 744
#define engine_configuration_s_size 6000
#define engine_load_mode_e_auto_enum "INVALID", "INVALID", "INVALID", "LM_SPEED_DENSITY", "LM_REAL_MAF", "LM_ALPHA_N", "LM_LUA", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "LM_MOCK"
#define engine_load_mode_e_enum "INVALID", "INVALID", "INVALID", "Speed Density", "MAF Air Charge", "Alpha-N", "Lua"
#define ENGINE_MAKE_BMW "BMW"
#define ENGINE_MAKE_GM "GM"
#define ENGINE_MAKE_LADA "Lada"
#define ENGINE_MAKE_MAZDA "Mazda"
#define ENGINE_MAKE_MERCEDES "Mercedes"
#define ENGINE_MAKE_NISSAN "Nissan"
#define ENGINE_MAKE_SUBARU "Subaru"
#define ENGINE_MAKE_TOYOTA "Toyota"
#define ENGINE_MAKE_VAG "VAG"
#define ENGINE_NOISE_CURVE_SIZE 8
#define ENGINE_SNIFFER_UNIT_US 10
#define engine_type_e_auto_enum "DEFAULT_FRANKENSO", "MIATA_PROTEUS_TCU", "DODGE_NEON_1995", "FORD_ASPIRE_1996", "INVALID", "NISSAN_PRIMERA", "HONDA_ACCORD_CD", "FORD_INLINE_6_1995", "GY6_139QMB", "MAZDA_MIATA_NB1", "INVALID", "MRE_MIATA_NB2_MAP", "MRE_MIATA_NA6_VAF", "MRE_MIATA_NB2_ETB", "FORD_ESCORT_GT", "MRE_MIATA_NB2_MAF", "MITSU_4G93", "HONDA_ACCORD_CD_TWO_WIRES", "HONDA_ACCORD_CD_DIP", "MIATA_1990", "MRE_MIATA_94_MAP", "MIATA_1996", "SUBARU_2003_WRX", "MRE_BODY_CONTROL", "BMW_M73_M", "INVALID", "TEST_ENGINE", "TEST_ISSUE_898", "MAZDA_626", "SACHS", "MRE_BOARD_OLD_TEST", "MRE_BOARD_NEW_TEST", "VW_ABA", "HELLEN72_ETB", "HELLEN_NA6", "CAMARO_4", "HELLEN_128_MERCEDES", "MRE_SUBARU_EJ18", "TOYOTA_JZS147", "LADA_KALINA", "BMW_M73_F", "MIATA_NA6_MAP", "PROTEUS_QC_TEST_BOARD", "HONDA_600", "TOYOTA_2JZ_GTE_VVTi", "TEST_ENGINE_VVT", "DODGE_NEON_2003_CRANK", "MAZDA_MIATA_2003", "HONDA_ACCORD_1_24_SHIFTED", "FRANKENSO_QA_ENGINE", "TEST_CIVIC_4_0_BOTH", "TEST_CIVIC_4_0_RISE", "TEST_ISSUE_366_BOTH", "TEST_ISSUE_366_RISE", "MAZDA_MIATA_2003_NA_RAIL", "MAZDA_MIATA_2003_BOARD_TEST", "MAZDA_MIATA_NA8", "MIATA_NA6_VAF", "ETB_BENCH_ENGINE", "TLE8888_BENCH_ENGINE", "MICRO_RUS_EFI", "PROTEUS_DEFAULTS", "VW_B6", "BMW_M73_PROTEUS", "DODGE_RAM", "CITROEN_TU3JP", "MRE_MIATA_NA6_MAP", "PROTEUS_MIATA_NB2", "MRE_M111", "HELLEN_NB2", "SUBARUEG33_DEFAULTS", "HELLEN_121_VAG", "HELLEN_121_NISSAN", "HELLEN_55_BMW", "HELLEN_88_BMW", "HELLEN_134_BMW", "HELLEN_154_VAG", "HELLEN_121_VAG_5_CYL", "HELLEN_121_VAG_V6_CYL", "HELLEN_121_VAG_VR6_CYL", "HELLEN_NA94", "HELLEN_121_VAG_8_CYL", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "MINIMAL_PINS", "PROMETHEUS_DEFAULTS", "SUBARUEJ20G_DEFAULTS", "VAG_18_TURBO", "TEST_33816", "BMW_M73_MRE", "BMW_M73_MRE_SLAVE", "PROTEUS_ANALOG_PWM_TEST", "TEST_ROTARY"
#define engineChartSize_offset 1480
#define engineCode_offset 1128
#define engineConfiguration_offset 0
#define engineLoadAccelEnrichmentMultiplier_offset 2072
#define engineLoadAccelEnrichmentThreshold_offset 2068
#define engineLoadAccelLength_offset 2056
#define engineLoadDecelEnleanmentMultiplier_offset 2064
#define engineLoadDecelEnleanmentThreshold_offset 2060
#define engineMake_offset 1096
#define engineSnifferRpmThreshold_offset 4
#define engineType_offset 0
#define ERROR_BUFFER_SIZE 120
#define ET_BMW_M73_F 40
#define ET_CAMARO 35
#define ET_CITROEN_TU3JP 65
#define ET_DEFAULT_FRANKENSO 0
#define ET_DODGE_NEON_1995 2
#define ET_DODGE_NEON_2003_CRANK 46
#define ET_FORD_ASPIRE 3
#define ET_FORD_ESCORT_GT 14
#define ET_FORD_INLINE_6 7
#define ET_FRANKENSO_MIATA_NA6 41
#define ET_FRANKENSO_MIATA_NA6_VAF 57
#define ET_FRANKENSO_MIATA_NB2 47
#define ET_HELLEN_121_NISSAN 72
#define ET_HELLEN_121_VAG 71
#define ET_HELLEN_121_VAG_5_CYL 77
#define ET_HELLEN_121_VAG_6_CYL 78
#define ET_HELLEN_121_VAG_8_CYL 81
#define ET_HELLEN_121_VAG_V6_CYL 78
#define ET_HELLEN_121_VAG_VR6_CYL 79
#define ET_HELLEN_128_MERCEDES 36
#define ET_HELLEN_134_BMW 75
#define ET_HELLEN_154_VAG 76
#define ET_HELLEN_55_BMW 73
#define ET_HELLEN_88_BMW 74
#define ET_HELLEN_NA6 34
#define ET_HELLEN_NA94 80
#define ET_HELLEN_NB2 69
#define ET_MIATA_TCU_PROTEUS 1
#define ET_MRE_BODY_CONTROL 23
#define ET_MRE_DEFAULTS 60
#define ET_MRE_M111 68
#define ET_MRE_MIATA_94_MAP 20
#define ET_MRE_MIATA_NA6_MAP 66
#define ET_MRE_MIATA_NA6_VAF 12
#define ET_MRE_MIATA_NB2_MAF 15
#define ET_MRE_MIATA_NB2_MAP 11
#define ET_MRE_NEW_TEST_BOARD 31
#define ET_MRE_OLD_TEST_BOARD 30
#define ET_MRE_SUBARU_EJ18 37
#define ET_NISSAN_PRIMERA 5
#define ET_PROTEUS_ANALOG_PWM_TEST 106
#define ET_PROTEUS_BMW_M73 63
#define ET_PROTEUS_MIATA_NB2 67
#define ET_PROTEUS_QC_TEST_BOARD 42
#define ET_SACHS 29
#define ET_TEST_ROTARY 107
#define ET_VW_ABA 32
#define ET_VW_B6 62
#define ETB_BIAS_CURVE_LENGTH 8
#define ETB_COUNT 2
#define etb_dFactor_offset 1744
#define ETB_ETB_None 0
#define etb_function_e_auto_enum "ETB_None", "ETB_Throttle1", "ETB_Throttle2", "ETB_IdleValve", "ETB_Wastegate"
#define ETB_HW_MAX_FREQUENCY 10000
#define etb_iFactor_offset 1740
#define etb_iTermMax_offset 3958
#define etb_iTermMin_offset 3956
#define etb_maxValue_offset 1754
#define etb_minValue_offset 1752
#define etb_offset 1736
#define etb_offset_offset 1748
#define etb_periodMs_offset 1750
#define etb_pFactor_offset 1736
#define etb_use_two_wires_offset 76
#define etbBiasBins_offset 3888
#define etbBiasValues_offset 3920
#define etbDeadband_offset 3960
#define etbFreq_offset 2514
#define etbFunctions1_offset 1198
#define etbFunctions2_offset 1199
#define etbIdleThrottleRange_offset 4012
#define etbIo1_controlPin_offset 982
#define etbIo1_directionPin1_offset 980
#define etbIo1_directionPin2_offset 981
#define etbIo1_disablePin_offset 983
#define etbIo1_offset 980
#define etbIo2_controlPin_offset 986
#define etbIo2_directionPin1_offset 984
#define etbIo2_directionPin2_offset 985
#define etbIo2_disablePin_offset 987
#define etbIo2_offset 984
#define etbNeutralPosition_offset 1471
#define etbRevLimitRange_offset 106
#define etbRevLimitStart_offset 104
#define etbWastegatePid_dFactor_offset 2524
#define etbWastegatePid_iFactor_offset 2520
#define etbWastegatePid_maxValue_offset 2534
#define etbWastegatePid_minValue_offset 2532
#define etbWastegatePid_offset 2516
#define etbWastegatePid_offset_offset 2528
#define etbWastegatePid_periodMs_offset 2530
#define etbWastegatePid_pFactor_offset 2516
#define externalKnockSenseAdc_offset 3103
#define extraInjectionOffset_offset 432
#define failedMapFallback_offset 541
#define fan1ExtraIdle_offset 2252
#define fan2ExtraIdle_offset 2330
#define fan2OffTemperature_offset 2715
#define fan2OnTemperature_offset 2714
#define fan2Pin_offset 2713
#define fan2PinMode_offset 2419
#define fanOffTemperature_offset 472
#define fanOnTemperature_offset 468
#define fanPin_offset 663
#define fanPinMode_offset 662
#define firingOrder_offset 404
#define fixedModeTiming_offset 452
#define fixedTiming_offset 2204
#define FLASH_DATA_VERSION 10003
#define flexSensorPin_offset 3100
#define frequencyReportingMapInputPin_offset 970
#define FSIO_ANALOG_INPUT_COUNT 4
#define FSIO_COMMAND_COUNT 16
#define FSIO_CURVE_16 16
#define FSIO_CURVE_8 8
#define FSIO_METHOD_FSIO_ANALOG_INPUT "fsio_analog_input"
#define FSIO_METHOD_FSIO_DIGITAL_INPUT "fsio_digital_input"
#define FSIO_METHOD_FSIO_SETTING "fsio_setting"
#define FSIO_METHOD_FSIO_TABLE "fsio_table"
#define fsio_setting10_offset 900
#define fsio_setting11_offset 904
#define fsio_setting12_offset 908
#define fsio_setting13_offset 912
#define fsio_setting14_offset 916
#define fsio_setting15_offset 920
#define fsio_setting16_offset 924
#define fsio_setting1_offset 864
#define fsio_setting2_offset 868
#define fsio_setting3_offset 872
#define fsio_setting4_offset 876
#define fsio_setting5_offset 880
#define fsio_setting6_offset 884
#define fsio_setting7_offset 888
#define fsio_setting8_offset 892
#define fsio_setting9_offset 896
#define FSIO_TABLE_8 8
#define fsioAdc1_offset 2200
#define fsioAdc2_offset 2201
#define fsioAdc3_offset 2202
#define fsioAdc4_offset 2203
#define fsioCurve1_offset 2780
#define fsioCurve1Bins_offset 2716
#define fsioCurve2_offset 2908
#define fsioCurve2Bins_offset 2844
#define fsioCurve3_offset 3004
#define fsioCurve3Bins_offset 2972
#define fsioCurve4_offset 3068
#define fsioCurve4Bins_offset 3036
#define fsioDigitalInputs10_offset 961
#define fsioDigitalInputs11_offset 962
#define fsioDigitalInputs12_offset 963
#define fsioDigitalInputs13_offset 964
#define fsioDigitalInputs14_offset 965
#define fsioDigitalInputs15_offset 966
#define fsioDigitalInputs16_offset 967
#define fsioDigitalInputs1_offset 952
#define fsioDigitalInputs2_offset 953
#define fsioDigitalInputs3_offset 954
#define fsioDigitalInputs4_offset 955
#define fsioDigitalInputs5_offset 956
#define fsioDigitalInputs6_offset 957
#define fsioDigitalInputs7_offset 958
#define fsioDigitalInputs8_offset 959
#define fsioDigitalInputs9_offset 960
#define fsioFormulas10_offset 8472
#define fsioFormulas11_offset 8672
#define fsioFormulas12_offset 8872
#define fsioFormulas13_offset 9072
#define fsioFormulas14_offset 9272
#define fsioFormulas15_offset 9472
#define fsioFormulas16_offset 9672
#define fsioFormulas1_offset 6672
#define fsioFormulas2_offset 6872
#define fsioFormulas3_offset 7072
#define fsioFormulas4_offset 7272
#define fsioFormulas5_offset 7472
#define fsioFormulas6_offset 7672
#define fsioFormulas7_offset 7872
#define fsioFormulas8_offset 8072
#define fsioFormulas9_offset 8272
#define fsioFrequency10_offset 850
#define fsioFrequency11_offset 852
#define fsioFrequency12_offset 854
#define fsioFrequency13_offset 856
#define fsioFrequency14_offset 858
#define fsioFrequency15_offset 860
#define fsioFrequency16_offset 862
#define fsioFrequency1_offset 832
#define fsioFrequency2_offset 834
#define fsioFrequency3_offset 836
#define fsioFrequency4_offset 838
#define fsioFrequency5_offset 840
#define fsioFrequency6_offset 842
#define fsioFrequency7_offset 844
#define fsioFrequency8_offset 846
#define fsioFrequency9_offset 848
#define fsioInputModes10_offset 1525
#define fsioInputModes11_offset 1526
#define fsioInputModes12_offset 1527
#define fsioInputModes13_offset 1528
#define fsioInputModes14_offset 1529
#define fsioInputModes15_offset 1530
#define fsioInputModes16_offset 1531
#define fsioInputModes1_offset 1516
#define fsioInputModes2_offset 1517
#define fsioInputModes3_offset 1518
#define fsioInputModes4_offset 1519
#define fsioInputModes5_offset 1520
#define fsioInputModes6_offset 1521
#define fsioInputModes7_offset 1522
#define fsioInputModes8_offset 1523
#define fsioInputModes9_offset 1524
#define fsioOutputPins10_offset 789
#define fsioOutputPins11_offset 790
#define fsioOutputPins12_offset 791
#define fsioOutputPins13_offset 792
#define fsioOutputPins14_offset 793
#define fsioOutputPins15_offset 794
#define fsioOutputPins16_offset 795
#define fsioOutputPins1_offset 780
#define fsioOutputPins2_offset 781
#define fsioOutputPins3_offset 782
#define fsioOutputPins4_offset 783
#define fsioOutputPins5_offset 784
#define fsioOutputPins6_offset 785
#define fsioOutputPins7_offset 786
#define fsioOutputPins8_offset 787
#define fsioOutputPins9_offset 788
#define fsioTable1_offset 19296
#define fsioTable1LoadBins_offset 19552
#define fsioTable1RpmBins_offset 19584
#define fsioTable2_offset 19616
#define fsioTable2LoadBins_offset 19680
#define fsioTable2RpmBins_offset 19712
#define fsioTable3_offset 19744
#define fsioTable3LoadBins_offset 19808
#define fsioTable3RpmBins_offset 19840
#define fsioTable4_offset 19872
#define fsioTable4LoadBins_offset 19936
#define fsioTable4RpmBins_offset 19968
#define FUEL_LEVEL_TABLE_COUNT 8
#define FUEL_LOAD_COUNT 16
#define FUEL_RPM_COUNT 16
#define fuelAlgorithm_offset 420
#define fuelClosedLoopCorrectionEnabled_offset 1464
#define fuelLevelBins_offset 1204
#define fuelLevelSensor_offset 514
#define fuelLevelValues_offset 684
#define fuelPumpPin_offset 658
#define fuelPumpPinMode_offset 659
#define fuelReferencePressure_offset 2420
#define GAP_TRACKING_LENGTH 18
#define gapTrackingLengthOverride_offset 444
#define GAUGE_COIL_DWELL_TIME "dwell: coil charge time"
#define GAUGE_NAME_ACCEL_X "Acceleration: X"
#define GAUGE_NAME_ACCEL_Y "Acceleration: Y"
#define GAUGE_NAME_ACCEL_Z "Acceleration: Z"
#define GAUGE_NAME_AFR "Air/Fuel Ratio"
#define GAUGE_NAME_AFR2 "Air/Fuel Ratio 2"
#define GAUGE_NAME_AIR_FLOW "MAF air flow"
#define GAUGE_NAME_AIR_MASS "air mass"
#define GAUGE_NAME_AUX_TEMP1 "Aux temp 1"
#define GAUGE_NAME_AUX_TEMP2 "Aux temp 2"
#define GAUGE_NAME_BARO_PRESSURE "Barometric pressure"
#define GAUGE_NAME_CLT "CLT"
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
#define GAUGE_NAME_ETB_DUTY "ETB Duty"
#define GAUGE_NAME_ETB_ERROR "ETB position error"
#define GAUGE_NAME_ETB_TARGET "ETB position target"
#define GAUGE_NAME_FLEX "Flex Ethanol %"
#define GAUGE_NAME_FUEL_BARO_CORR "fuel: Barometric pressure correction"
#define GAUGE_NAME_FUEL_BASE "fuel: base mass"
#define GAUGE_NAME_FUEL_CHARGE_TEMP "fuel: Estimated charge temperature"
#define GAUGE_NAME_FUEL_CLT_CORR "fuel: CLT correction"
#define GAUGE_NAME_FUEL_CONSUMPTION "fuel: Total consumed"
#define GAUGE_NAME_FUEL_CRANKING "fuel: cranking"
#define GAUGE_NAME_FUEL_EL_EXTRA "fuel: engine load acceleration extra fuel"
#define GAUGE_NAME_FUEL_FLOW "fuel: Flow rate"
#define GAUGE_NAME_FUEL_IAT_CORR "fuel: IAT correction"
#define GAUGE_NAME_FUEL_INJ_DUTY "fuel: injector duty cycle"
#define GAUGE_NAME_FUEL_LAST_INJECTION "fuel: Last injection"
#define GAUGE_NAME_FUEL_LOAD "fuel: load"
#define GAUGE_NAME_FUEL_PRESSURE_HIGH "Fuel pressure (high)"
#define GAUGE_NAME_FUEL_PRESSURE_HIGH_UNITS "bar"
#define GAUGE_NAME_FUEL_PRESSURE_LOW "Fuel pressure (low)"
#define GAUGE_NAME_FUEL_PRESSURE_LOW_UNITS "kPa"
#define GAUGE_NAME_FUEL_RUNNING "fuel: running"
#define GAUGE_NAME_FUEL_TPS_EXTRA "fuel: TPS acceleration extra fuel ms"
#define GAUGE_NAME_FUEL_TPS_ROC "fuel: TPS change"
#define GAUGE_NAME_FUEL_TRIM "fuel: fuel trim"
#define GAUGE_NAME_FUEL_TRIM_2 "fuel: fuel trim 2"
#define GAUGE_NAME_FUEL_VE "fuel: VE"
#define GAUGE_NAME_FUEL_WALL_AMOUNT "fuel: wall amount"
#define GAUGE_NAME_FUEL_WALL_CORRECTION "fuel: wall corr ms"
#define GAUGE_NAME_IAC "Idle Air Valve"
#define GAUGE_NAME_IAT "IAT"
#define GAUGE_NAME_IGNITION_LOAD "ign: load"
#define GAUGE_NAME_INJECTOR_LAG "fuel: injector lag"
#define GAUGE_NAME_KNOCK_COUNTER "knock: counter"
#define GAUGE_NAME_KNOCK_LEVEL "knock: current level"
#define GAUGE_NAME_LAMBDA "Lambda"
#define GAUGE_NAME_LAMBDA2 "Lambda 2"
#define GAUGE_NAME_MAF "MAF"
#define GAUGE_NAME_MAP "MAP"
#define GAUGE_NAME_OIL_PRESSURE "Oil Pressure"
#define GAUGE_NAME_OIL_PRESSURE_UNITS "kPa"
#define GAUGE_NAME_RAW_FUEL_PRESSURE_HIGH "Fuel pressure raw (high)"
#define GAUGE_NAME_RAW_FUEL_PRESSURE_LOW "Fuel pressure raw (low)"
#define GAUGE_NAME_RPM "RPM"
#define GAUGE_NAME_TARGET_AFR "fuel: target AFR"
#define GAUGE_NAME_TARGET_LAMBDA "fuel: target lambda"
#define GAUGE_NAME_TCHARGE "fuel: SD tCharge"
#define GAUGE_NAME_THROTTLE_PEDAL "Throttle pedal position"
#define GAUGE_NAME_TIME "Time"
#define GAUGE_NAME_TIMING_ADVANCE "timing"
#define GAUGE_NAME_TPS "TPS"
#define GAUGE_NAME_TPS2 "TPS2"
#define GAUGE_NAME_TRG_ERR "trg err"
#define GAUGE_NAME_VBAT "VBatt"
#define GAUGE_NAME_VERSION "firmware"
#define GAUGE_NAME_VVS "Vehicle Speed"
#define GAUGE_NAME_VVT_B1E "VVT: bank 1 exhaust"
#define GAUGE_NAME_VVT_B1I "VVT: bank 1 intake"
#define GAUGE_NAME_VVT_B2E "VVT: bank 2 exhaust"
#define GAUGE_NAME_VVT_B2I "VVT: bank 2 intake"
#define GAUGE_NAME_WARNING_COUNTER "warning: counter"
#define GAUGE_NAME_WARNING_LAST "warning: last"
#define GAUGE_NAME_WG_POSITION "Wastegate position sensor"
#define globalFuelCorrection_offset 544
#define globalTriggerAngleOffset_offset 456
#define gpioPinModes10_offset 773
#define gpioPinModes11_offset 774
#define gpioPinModes12_offset 775
#define gpioPinModes13_offset 776
#define gpioPinModes14_offset 777
#define gpioPinModes15_offset 778
#define gpioPinModes16_offset 779
#define gpioPinModes1_offset 764
#define gpioPinModes2_offset 765
#define gpioPinModes3_offset 766
#define gpioPinModes4_offset 767
#define gpioPinModes5_offset 768
#define gpioPinModes6_offset 769
#define gpioPinModes7_offset 770
#define gpioPinModes8_offset 771
#define gpioPinModes9_offset 772
#define gppwm1_alignmentFill_map_offset 4151
#define gppwm1_dutyIfError_offset 4145
#define gppwm1_loadAxis_offset 4150
#define gppwm1_loadBins_offset 4152
#define gppwm1_offBelowDuty_offset 4149
#define gppwm1_offset 4144
#define gppwm1_onAboveDuty_offset 4148
#define gppwm1_pin_offset 4144
#define gppwm1_pwmFrequency_offset 4146
#define gppwm1_rpmBins_offset 4160
#define gppwm1_table_offset 4168
#define gppwm2_alignmentFill_map_offset 4239
#define gppwm2_dutyIfError_offset 4233
#define gppwm2_loadAxis_offset 4238
#define gppwm2_loadBins_offset 4240
#define gppwm2_offBelowDuty_offset 4237
#define gppwm2_offset 4232
#define gppwm2_onAboveDuty_offset 4236
#define gppwm2_pin_offset 4232
#define gppwm2_pwmFrequency_offset 4234
#define gppwm2_rpmBins_offset 4248
#define gppwm2_table_offset 4256
#define gppwm3_alignmentFill_map_offset 4327
#define gppwm3_dutyIfError_offset 4321
#define gppwm3_loadAxis_offset 4326
#define gppwm3_loadBins_offset 4328
#define gppwm3_offBelowDuty_offset 4325
#define gppwm3_offset 4320
#define gppwm3_onAboveDuty_offset 4324
#define gppwm3_pin_offset 4320
#define gppwm3_pwmFrequency_offset 4322
#define gppwm3_rpmBins_offset 4336
#define gppwm3_table_offset 4344
#define gppwm4_alignmentFill_map_offset 4415
#define gppwm4_dutyIfError_offset 4409
#define gppwm4_loadAxis_offset 4414
#define gppwm4_loadBins_offset 4416
#define gppwm4_offBelowDuty_offset 4413
#define gppwm4_offset 4408
#define gppwm4_onAboveDuty_offset 4412
#define gppwm4_pin_offset 4408
#define gppwm4_pwmFrequency_offset 4410
#define gppwm4_rpmBins_offset 4424
#define gppwm4_table_offset 4432
#define gppwm_channel_e_auto_enum "GPPWM_Tps", "GPPWM_Map", "GPPWM_Clt", "GPPWM_Iat", "GPPWM_FuelLoad", "GPPWM_IgnLoad", "GPPWM_AuxTemp1", "GPPWM_AuxTemp2", "GPPWM_Zero", "GPPWM_AccelPedal"
#define gppwm_channel_e_enum "TPS", "MAP", "CLT", "IAT", "Fuel Load", "Ignition Load", "Aux Temp 1", "Aux Temp 2", "Zero", "Accel Pedal"
#define gppwm_channel_size 88
#define GPPWM_CHANNELS 4
#define GPPWM_LOAD_COUNT 8
#define GPPWM_RPM_COUNT 8
#define gps_rx_pin_offset 656
#define gps_tx_pin_offset 657
#define hardCutRpmRange_offset 1048
#define hasFrequencyReportingMapSensor_offset 1464
#define HD44780_db4_offset 652
#define HD44780_db5_offset 653
#define HD44780_db6_offset 654
#define HD44780_db7_offset 655
#define HD44780_e_offset 651
#define HD44780_rs_offset 650
#define HD44780height_offset 508
#define HD44780width_offset 504
#define highPressureFuel_alignmentFill_offset 3289
#define highPressureFuel_hwChannel_offset 3288
#define highPressureFuel_offset 3288
#define highPressureFuel_v1_offset 3292
#define highPressureFuel_v2_offset 3300
#define highPressureFuel_value1_offset 3296
#define highPressureFuel_value2_offset 3304
#define hip9011CsPin_offset 753
#define hip9011CsPinMode_offset 703
#define hip9011Gain_offset 3952
#define hip9011IntHoldPin_offset 754
#define hip9011IntHoldPinMode_offset 755
#define hip9011PrescalerAndSDO_offset 324
#define hip9011SpiDevice_offset 540
#define hipOutputChannel_offset 1468
#define hpfpValvePin_offset 2130
#define hpfpValvePinMode_offset 2131
#define HW_MAX_ADC_INDEX 17
#define IAC_PID_MULT_SIZE 8
#define iacByTpsTaper_offset 2038
#define iacCoasting_offset 3224
#define iacCoastingBins_offset 3160
#define iacPidMultLoadBins_offset 4124
#define iacPidMultRpmBins_offset 4132
#define iacPidMultTable_offset 4060
#define iat_adcChannel_offset 312
#define iat_alignmentFill_at_29_offset 313
#define iat_bias_resistor_offset 308
#define iat_config_offset 284
#define IAT_CURVE_SIZE 16
#define iat_offset 284
#define iat_resistance_1_offset 296
#define iat_resistance_2_offset 300
#define iat_resistance_3_offset 304
#define iat_tempC_1_offset 284
#define iat_tempC_2_offset 288
#define iat_tempC_3_offset 292
#define iatFuelCorr_offset 10464
#define iatFuelCorrBins_offset 10400
#define IDLE_ADVANCE_CURVE_SIZE 8
#define idle_antiwindupFreq_offset 696
#define idle_derivativeFilterLoss_offset 516
#define idle_hardware_s_size 8
#define idle_mode_e_auto_enum "IM_AUTO", "IM_MANUAL"
#define idle_offset 600
#define idle_solenoidFrequency_offset 600
#define idle_solenoidPin_offset 604
#define idle_solenoidPinMode_offset 607
#define idle_stepperDirectionPin_offset 605
#define idle_stepperStepPin_offset 606
#define IDLE_VE_CURVE_SIZE 8
#define idleAdvance_offset 6576
#define idleAdvanceBins_offset 6544
#define idleIncrementalPidCic_offset 744
#define idleMode_offset 1472
#define idlePidActivationTime_offset 2588
#define idlePidDeactivationTpsThreshold_offset 816
#define idlePidRpmDeadZone_offset 1894
#define idlePidRpmUpperLimit_offset 1484
#define idlePositionMax_offset 722
#define idlePositionMin_offset 720
#define idlePositionSensor_offset 806
#define idleRpmPid2_dFactor_offset 4048
#define idleRpmPid2_iFactor_offset 4044
#define idleRpmPid2_maxValue_offset 4058
#define idleRpmPid2_minValue_offset 4056
#define idleRpmPid2_offset 4040
#define idleRpmPid2_offset_offset 4052
#define idleRpmPid2_periodMs_offset 4054
#define idleRpmPid2_pFactor_offset 4040
#define idleRpmPid_dFactor_offset 1796
#define idleRpmPid_iFactor_offset 1792
#define idlerpmpid_iTermMax_offset 4006
#define idlerpmpid_iTermMin_offset 4002
#define idleRpmPid_maxValue_offset 1806
#define idleRpmPid_minValue_offset 1804
#define idleRpmPid_offset 1788
#define idleRpmPid_offset_offset 1800
#define idleRpmPid_periodMs_offset 1802
#define idleRpmPid_pFactor_offset 1788
#define idleStepperReactionTime_offset 1508
#define idleStepperTotalSteps_offset 1532
#define idleTimingPid_dFactor_offset 3976
#define idleTimingPid_iFactor_offset 3972
#define idleTimingPid_maxValue_offset 3986
#define idleTimingPid_minValue_offset 3984
#define idleTimingPid_offset 3968
#define idleTimingPid_offset_offset 3980
#define idleTimingPid_periodMs_offset 3982
#define idleTimingPid_pFactor_offset 3968
#define idleTimingPidDeadZone_offset 3990
#define idleVe_offset 6640
#define idleVeBins_offset 6608
#define IGN_LOAD_COUNT 16
#define IGN_RPM_COUNT 16
#define ignition_mode_e_auto_enum "IM_ONE_COIL", "IM_INDIVIDUAL_COILS", "IM_WASTED_SPARK", "IM_TWO_COILS"
#define ignitionDwellForCrankingMs_offset 100
#define ignitionIatCorrLoadBins_offset 13856
#define ignitionIatCorrRpmBins_offset 13920
#define ignitionIatCorrTable_offset 12832
#define ignitionLoadBins_offset 17312
#define ignitionMode_offset 440
#define ignitionPinMode_offset 649
#define ignitionPins10_offset 645
#define ignitionPins11_offset 646
#define ignitionPins12_offset 647
#define ignitionPins1_offset 636
#define ignitionPins2_offset 637
#define ignitionPins3_offset 638
#define ignitionPins4_offset 639
#define ignitionPins5_offset 640
#define ignitionPins6_offset 641
#define ignitionPins7_offset 642
#define ignitionPins8_offset 643
#define ignitionPins9_offset 644
#define ignitionRpmBins_offset 17376
#define ignitionTable_offset 16288
#define ignOverrideMode_offset 2128
#define IM_IM_BATCH 2
#define IM_IM_SINGLE_POINT 3
#define IM_IM_WASTED_SPARK 2
#define INDICATOR_NAME_AC_SWITCH "AC switch"
#define INDICATOR_NAME_BRAKE_DOWN "brake: down"
#define INDICATOR_NAME_CLUTCH_DOWN "clutch: down"
#define INDICATOR_NAME_CLUTCH_UP "clutch: up"
#define injection_mode_e_auto_enum "IM_SIMULTANEOUS", "IM_SEQUENTIAL", "IM_BATCH", "IM_SINGLE_POINT"
#define injectionMode_offset 428
#define injectionPhase_offset 13984
#define injectionPinMode_offset 648
#define injectionPins10_offset 633
#define injectionPins11_offset 634
#define injectionPins12_offset 635
#define injectionPins1_offset 624
#define injectionPins2_offset 625
#define injectionPins3_offset 626
#define injectionPins4_offset 627
#define injectionPins5_offset 628
#define injectionPins6_offset 629
#define injectionPins7_offset 630
#define injectionPins8_offset 631
#define injectionPins9_offset 632
#define injector_battLagCorr_offset 44
#define injector_battLagCorrBins_offset 12
#define injector_compensation_mode_e_auto_enum "ICM_None", "ICM_FixedRailPressure", "ICM_SensedRailPressure"
#define injector_flow_offset 8
#define injector_offset 8
#define injector_pressure_type_e_auto_enum "IPT_Low", "IPT_High"
#define injector_s_size 68
#define injectorCompensationMode_offset 2418
#define injectorCorrectionPolynomial1_offset 2652
#define injectorCorrectionPolynomial2_offset 2656
#define injectorCorrectionPolynomial3_offset 2660
#define injectorCorrectionPolynomial4_offset 2664
#define injectorCorrectionPolynomial5_offset 2668
#define injectorCorrectionPolynomial6_offset 2672
#define injectorCorrectionPolynomial7_offset 2676
#define injectorCorrectionPolynomial8_offset 2680
#define InjectorNonlinearMode_auto_enum "INJ_None", "INJ_PolynomialAdder"
#define injectorNonlinearMode_offset 2510
#define injectorPressureType_offset 2129
#define injPhaseLoadBins_offset 15008
#define injPhaseRpmBins_offset 15072
#define invertCamVVTSignal_offset 976
#define invertPrimaryTriggerSignal_offset 1464
#define invertSecondaryTriggerSignal_offset 1464
#define is_enabled_spi_1_offset 744
#define is_enabled_spi_2_offset 744
#define is_enabled_spi_3_offset 744
#define is_enabled_spi_4_offset 744
#define isAlternatorControlEnabled_offset 1464
#define isBoostControlEnabled_offset 1476
#define isCJ125Enabled_offset 744
#define isCJ125Verbose_offset 76
#define isCylinderCleanupEnabled_offset 1476
#define isDoubleSolenoidIdle_offset 76
#define isEngineChartEnabled_offset 1464
#define isEngineControlEnabled_offset 744
#define isFasterEngineSpinUpEnabled_offset 744
#define isForcedInduction_offset 76
#define isHip9011Enabled_offset 744
#define isIgnitionEnabled_offset 1476
#define isInjectionEnabled_offset 1476
#define isManualSpinningMode_offset 1476
#define isMapAveragingEnabled_offset 1476
#define isSdCardEnabled_offset 744
#define issue_294_29_offset 76
#define issue_294_30_offset 76
#define issue_294_31_offset 76
#define isVerboseAlternator_offset 744
#define isVerboseAuxPid1_offset 76
#define isVerboseETB_offset 1464
#define isVerboseIAC_offset 1464
#define isWaveAnalyzerEnabled_offset 1476
#define joystickAPin_offset 939
#define joystickBPin_offset 940
#define joystickCenterPin_offset 938
#define joystickCPin_offset 941
#define joystickDPin_offset 942
#define knockBandCustom_offset 328
#define knockBankCyl10_offset 976
#define knockBankCyl11_offset 976
#define knockBankCyl12_offset 976
#define knockBankCyl1_offset 976
#define knockBankCyl2_offset 976
#define knockBankCyl3_offset 976
#define knockBankCyl4_offset 976
#define knockBankCyl5_offset 976
#define knockBankCyl6_offset 976
#define knockBankCyl7_offset 976
#define knockBankCyl8_offset 976
#define knockBankCyl9_offset 976
#define knockBaseNoise_offset 4523
#define knockDetectionWindowEnd_offset 1504
#define knockDetectionWindowStart_offset 1500
#define knockNoise_offset 1820
#define knockNoiseRpmBins_offset 1852
#define knockVThreshold_offset 1512
#define lambdaLoadBins_offset 18848
#define lambdaRpmBins_offset 18912
#define lambdaTable_offset 18592
#define launchActivateDelay_offset 1060
#define launchActivatePin_offset 991
#define launchActivationMode_e_auto_enum "SWITCH_INPUT_LAUNCH", "CLUTCH_INPUT_LAUNCH", "ALWAYS_ACTIVE_LAUNCH"
#define launchActivationMode_e_enum "Switch Input", "Clutch Input", "Always Active(Disabled By Speed)", "INVALID"
#define launchActivationMode_offset 1020
#define launchAdvanceRpmRange_offset 1052
#define launchBoostDuty_offset 1044
#define launchControlEnabled_offset 76
#define launchDisableBySpeed_offset 976
#define launchFuelAdded_offset 1040
#define launchFuelCutEnable_offset 1464
#define launchRpm_offset 316
#define launchRpmTreshold_offset 1032
#define launchSmoothRetard_offset 1476
#define launchSparkCutEnable_offset 1464
#define launchSpeedTreshold_offset 1028
#define launchTimingRetard_offset 320
#define launchTimingRpmRange_offset 1036
#define launchTpsTreshold_offset 1056
#define LDS_ALTERNATOR_PID_STATE_INDEX 9
#define LDS_CJ125_PID_STATE_INDEX 10
#define LDS_ENGINE_STATE_INDEX 3
#define LDS_ETB_PID_STATE_INDEX 7
#define LDS_FUEL_TRIM_STATE_INDEX 4
#define LDS_IDLE_PID_STATE_INDEX 8
#define LDS_SPEED_DENSITY_STATE_INDEX 2
#define LDS_TPS_TPS_ENEICHMENT_STATE_INDEX 5
#define LDS_TRIGGER_CENTRAL_STATE_INDEX 6
#define LDS_TRIGGER_STATE_STATE_INDEX 11
#define LE_COMMAND_LENGTH 200
#define linear_sensor_s_size 20
#define LIS302DLCsPin_offset 2043
#define LIS302DLCsPinMode_offset 2417
#define LOAD_1_BYTE_PACKING_MULT 2
#define log_format_e_auto_enum "LF_NATIVE", "LM_MLV"
#define logFormat_offset 496
#define LOGIC_ANALYZER_CHANNEL_COUNT 4
#define logicAnalyzerPins1_offset 748
#define logicAnalyzerPins2_offset 749
#define logicAnalyzerPins3_offset 750
#define logicAnalyzerPins4_offset 751
#define lowPressureFuel_alignmentFill_offset 3309
#define lowPressureFuel_hwChannel_offset 3308
#define lowPressureFuel_offset 3308
#define lowPressureFuel_v1_offset 3312
#define lowPressureFuel_v2_offset 3320
#define lowPressureFuel_value1_offset 3316
#define lowPressureFuel_value2_offset 3324
#define lps25BaroSensorScl_offset 1458
#define lps25BaroSensorSda_offset 1459
#define LUA_PWM_COUNT 8
#define LUA_SCRIPT_SIZE 256
#define luaOutputPins1_offset 1220
#define luaOutputPins2_offset 1221
#define luaOutputPins3_offset 1222
#define luaOutputPins4_offset 1223
#define luaOutputPins5_offset 1224
#define luaOutputPins6_offset 1225
#define luaOutputPins7_offset 1226
#define luaOutputPins8_offset 1227
#define luaScript_offset 16032
#define MAF_DECODING_COUNT 256
#define maf_sensor_type_e_auto_enum "CUSTOM", "Bosch0280218037", "Bosch0280218004", "DensoTODO"
#define maf_sensor_type_e_enum "v0", "v1", "v2", "v3"
#define mafAdcChannel_offset 543
#define mafDecoding_offset 10784
#define mafDecodingBins_offset 11808
#define mafSensorType_offset 948
#define MAIN_HELP_URL "http://www.rusefi.com/"
#define mainRelayPin_offset 706
#define mainRelayPinMode_offset 752
#define mainUnusedEnd_offset 4612
#define malfunctionIndicatorPin_offset 660
#define malfunctionIndicatorPinMode_offset 661
#define manIdlePosition_offset 608
#define MAP_ACCEL_TAPER 8
#define MAP_ANGLE_SIZE 8
#define map_offset 108
#define map_samplingAngle_offset 140
#define map_samplingAngleBins_offset 108
#define map_samplingWindow_offset 204
#define map_samplingWindowBins_offset 172
#define map_sensor_alignmentFill_offset 249
#define MAP_sensor_config_s_size 144
#define map_sensor_highValue_offset 240
#define map_sensor_hwChannel_offset 248
#define map_sensor_lowValue_offset 236
#define map_sensor_offset 236
#define map_sensor_type_offset 244
#define MAP_WINDOW_SIZE 8
#define mapAccelTaperBins_offset 2136
#define mapAccelTaperMult_offset 2168
#define mapAveragingSchedulingAtIndex_offset 1540
#define mapErrorDetectionTooHigh_offset 1780
#define mapErrorDetectionTooLow_offset 1776
#define mapEstimateRpmBins_offset 15744
#define mapEstimateTable_offset 15200
#define mapEstimateTpsBins_offset 15712
#define mapFrequency0Kpa_offset 612
#define mapFrequency100Kpa_offset 616
#define mapHighValueVoltage_offset 2212
#define mapLowValueVoltage_offset 2208
#define mapMinBufferLength_offset 812
#define max31855_cs1_offset 796
#define max31855_cs2_offset 797
#define max31855_cs3_offset 798
#define max31855_cs4_offset 799
#define max31855_cs5_offset 800
#define max31855_cs6_offset 801
#define max31855_cs7_offset 802
#define max31855_cs8_offset 803
#define max31855spiDevice_offset 675
#define MAX_CYLINDER_COUNT 12
#define maxAcClt_offset 1711
#define maxAcRpm_offset 1708
#define maxAcTps_offset 1710
#define maxIdleVss_offset 445
#define maxKnockSubDeg_offset 552
#define mc33816_cs_offset 3107
#define mc33816_driven_offset 2607
#define mc33816_flag0_offset 1774
#define mc33816_rstb_offset 2606
#define mc33816spiDevice_offset 4001
#define mc33972_cs_offset 678
#define mc33972_csPinMode_offset 679
#define mc33972spiDevice_offset 4008
#define mc33_hvolt_offset 760
#define mc33_i_boost_offset 4496
#define mc33_i_hold_offset 4500
#define mc33_i_peak_offset 4498
#define mc33_t_bypass_offset 4508
#define mc33_t_hold_off_offset 4510
#define mc33_t_hold_tot_offset 4512
#define mc33_t_max_boost_offset 4502
#define mc33_t_peak_off_offset 4504
#define mc33_t_peak_tot_offset 4506
#define measureMapOnlyInOneCylinder_offset 744
#define minOilPressureAfterStart_offset 446
#define MLQ_FIELD_HEADER_SIZE 55
#define MLQ_HEADER_SIZE 22
#define MOCK_AFR_COMMAND "mock_afr_voltage"
#define MOCK_CLT_COMMAND "mock_clt_voltage"
#define MOCK_IAT_COMMAND "mock_iat_voltage"
#define MOCK_MAF_COMMAND "mock_maf_voltage"
#define MOCK_MAP_COMMAND "mock_map_voltage"
#define MOCK_PPS_POSITION_COMMAND "mock_pps_position"
#define MOCK_PPS_VOLTAGE_COMMAND "mock_pps_voltage"
#define multisparkDwell_offset 1786
#define multisparkEnable_offset 976
#define multisparkMaxExtraSparkCount_offset 975
#define multisparkMaxRpm_offset 972
#define multisparkMaxSparkingAngle_offset 974
#define multisparkSparkDuration_offset 1784
#define NARROW_BAND_WIDE_BAND_CONVERSION_SIZE 8
#define narrowToWideOxygen_offset 2296
#define narrowToWideOxygenBins_offset 2264
#define noAccelAfterHardLimitPeriodSecs_offset 1536
#define o2heaterPin_offset 742
#define o2heaterPinModeTodO_offset 743
#define oilPressure_alignmentFill_offset 2693
#define oilPressure_hwChannel_offset 2692
#define oilPressure_offset 2692
#define oilPressure_v1_offset 2696
#define oilPressure_v2_offset 2704
#define oilPressure_value1_offset 2700
#define oilPressure_value2_offset 2708
#define onOffAlternatorLogic_offset 744
#define operation_mode_e_auto_enum "OM_NONE", "FOUR_STROKE_CRANK_SENSOR", "FOUR_STROKE_CAM_SENSOR", "TWO_STROKE", "FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR", "FOUR_STROKE_THREE_TIMES_CRANK_SENSOR"
#define output_pin_e_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PA8", "PA9", "PA10", "PA11", "PA12", "PA13", "PA14", "PA15", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PB8", "PB9", "PB10", "PB11", "PB12", "PB13", "PB14", "PB15", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "PC10", "PC11", "PC12", "PC13", "PC14", "PC15", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PD8", "PD9", "PD10", "PD11", "PD12", "PD13", "PD14", "PD15", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "PE12", "PE13", "PE14", "PE15", "PF0", "PF1", "PF2", "PF3", "PF4", "PF5", "PF6", "PF7", "PF8", "PF9", "PF10", "PF11", "PF12", "PF13", "PF14", "PF15", "PG0", "PG1", "PG2", "PG3", "PG4", "PG5", "PG6", "PG7", "PG8", "PG9", "PG10", "PG11", "PG12", "PG13", "PG14", "PG15", "PH0", "PH1", "PH2", "PH3", "PH4", "PH5", "PH6", "PH7", "PH8", "PH9", "PH10", "PH11", "PH12", "PH13", "PH14", "PH15", "PI0", "PI1", "PI2", "PI3", "PI4", "PI5", "PI6", "PI7", "PI8", "PI9", "PI10", "PI11", "PI12", "PI13", "PI14", "PI15", "MC33972_1", "MC33972_2", "MC33972_3", "MC33972_4", "MC33972_5", "MC33972_6", "MC33972_7", "MC33972_8", "MC33972_9", "MC33972_10", "MC33972_11", "MC33972_12", "MC33972_13", "MC33972_14", "MC33972_15", "MC33972_16", "MC33972_17", "MC33972_18", "MC33972_19", "MC33972_20", "MC33972_21", "MC33972_22", "TLE8888_1", "TLE8888_2", "TLE8888_3", "TLE8888_4", "TLE8888_5", "TLE8888_6", "TLE8888_7", "TLE8888_8", "TLE8888_9", "TLE8888_10", "TLE8888_11", "TLE8888_12", "TLE8888_13", "TLE8888_14", "TLE8888_15", "TLE8888_16", "TLE8888_17", "TLE8888_18", "TLE8888_19", "TLE8888_20", "TLE8888_21", "TLE8888_22", "TLE8888_23", "TLE8888_24", "TLE8888_25", "TLE8888_26", "TLE8888_27", "TLE8888_28", "INVALID", "INVALID", "INVALID", "TLE6240_1", "TLE6240_2", "TLE6240_3", "TLE6240_4", "TLE6240_5", "TLE6240_6", "TLE6240_7", "TLE6240_8", "TLE6240_9", "TLE6240_10", "TLE6240_11", "TLE6240_12", "TLE6240_13", "TLE6240_14", "TLE6240_15", "TLE6240_16", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID"
#define overrideCrankingIacSetting_offset 1476
#define overrideTriggerGaps_offset 76
#define PACK_ADD_TEMPERATURE 40
#define PACK_MULT_AFR 1000
#define PACK_MULT_AFR_CFG 10
#define PACK_MULT_ANGLE 50
#define PACK_MULT_FUEL_FLOW 200
#define PACK_MULT_FUEL_MASS 100
#define PACK_MULT_HIGH_PRESSURE 10
#define PACK_MULT_LAMBDA 10000
#define PACK_MULT_LAMBDA_CFG 147
#define PACK_MULT_MAP_ESTIMATE 100
#define PACK_MULT_MASS_FLOW 10
#define PACK_MULT_MS 300
#define PACK_MULT_PERCENT 100
#define PACK_MULT_PRESSURE 30
#define PACK_MULT_TEMPERATURE 100
#define PACK_MULT_VOLTAGE 1000
#define pauseEtbControl_offset 744
#define PEDAL_TO_TPS_SIZE 8
#define pedalToTpsPedalBins_offset 6464
#define pedalToTpsRpmBins_offset 6472
#define pedalToTpsTable_offset 6400
#define persistent_config_s_size 20000
#define pid_s_size 20
#define pidExtraForLowRpm_offset 3156
#define pin_input_mode_e_auto_enum "PI_DEFAULT", "PI_PULLUP", "PI_PULLDOWN"
#define pin_input_mode_e_enum "DEFAULT", "PULLUP", "PULLDOWN"
#define pin_mode_e_auto_enum "PO_DEFAULT", "INVALID", "INVALID", "INVALID", "PO_OPENDRAIN", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "PO_PULLUP", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "PO_PULLDOWN"
#define pin_mode_e_enum "default", "INVALID", "INVALID", "INVALID", "opendrain", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "PULLUP", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "PULLDOWN"
#define pin_output_mode_e_auto_enum "OM_DEFAULT", "OM_INVERTED", "OM_OPENDRAIN", "OM_OPENDRAIN_INVERTED"
#define pin_output_mode_e_enum "default", "default inverted", "open collector", "open collector inverted"
#define postCrankingDurationSec_offset 2440
#define postCrankingFactor_offset 2436
#define primeInjFalloffTemperature_offset 1486
#define primingSquirtDurationMs_offset 96
#define PROTOCOL_ANALOG_CHART "analog_chart"
#define PROTOCOL_COIL1_SHORT_NAME "c1"
#define PROTOCOL_CRANK1 "t1"
#define PROTOCOL_CRANK2 "t2"
#define PROTOCOL_CRANK3 "t3"
#define PROTOCOL_ENGINE_SNIFFER "wave_chart"
#define PROTOCOL_ES_DOWN "d"
#define PROTOCOL_ES_UP "u"
#define PROTOCOL_HELLO_PREFIX "***"
#define PROTOCOL_HIP_NAME "HIP"
#define PROTOCOL_HPFP_NAME "hpfp"
#define PROTOCOL_INJ1_SHORT_NAME "i1"
#define PROTOCOL_MSG "msg"
#define PROTOCOL_OUTPIN "outpin"
#define PROTOCOL_SIGNATURE_PREFIX "rusEFI "
#define PROTOCOL_TACH_NAME "tach"
#define PROTOCOL_TEST_RESPONSE_TAG "ts_p_alive"
#define PROTOCOL_VERSION_TAG "rusEfiVersion"
#define PROTOCOL_VVT1_NAME "VVT1"
#define PROTOCOL_VVT2_NAME "VVT2"
#define PROTOCOL_VVT3_NAME "VVT3"
#define PROTOCOL_VVT4_NAME "VVT4"
#define PROTOCOL_WA_CHANNEL_1 "input1"
#define PROTOCOL_WA_CHANNEL_2 "input2"
#define PROTOCOL_WA_CHANNEL_3 "input3"
#define PROTOCOL_WA_CHANNEL_4 "input4"
#define rollingLaunchEnabled_offset 76
#define RPM_1_BYTE_PACKING_MULT 50
#define rpmHardLimit_offset 416
#define runningLedPin_offset 1813
#define sdCardCsPin_offset 707
#define sdCardCsPinMode_offset 2226
#define sdCardPeriodMs_offset 804
#define sdCardSpiDevice_offset 2592
#define secondSolenoidPin_offset 810
#define sensor_chart_e_auto_enum "SC_OFF", "SC_TRIGGER", "SC_MAP", "SC_RPM_ACCEL", "SC_DETAILED_RPM", "SC_AUX_FAST1"
#define sensor_chart_e_enum "none", "trigger", "MAP", "RPM ACCEL", "DETAILED RPM", "Fast Aux1", "INVALID", "INVALID"
#define sensorChartMode_offset 944
#define sensorSnifferRpmThreshold_offset 412
#define SERVO_COUNT 8
#define servoOutputPins1_offset 3140
#define servoOutputPins2_offset 3141
#define servoOutputPins3_offset 3142
#define servoOutputPins4_offset 3143
#define servoOutputPins5_offset 3144
#define servoOutputPins6_offset 3145
#define servoOutputPins7_offset 3146
#define servoOutputPins8_offset 3147
#define show_Frankenso_presets true
#define show_test_presets true
#define showHumanReadableWarning_offset 976
#define showSdCardWarning_offset 76
#define SIGNATURE_BOARD atlas
#define SIGNATURE_DATE 2021.07.30
#define SIGNATURE_HASH 1133107817
#define silentTriggerError_offset 1464
#define slowAdcAlpha_offset 2088
#define sparkDwellRpmBins_offset 332
#define sparkDwellValues_offset 364
#define specs_offset 396
#define specs_s_size 12
#define spi1MisoMode_offset 2598
#define spi1misoPin_offset 929
#define spi1MosiMode_offset 2597
#define spi1mosiPin_offset 928
#define spi1SckMode_offset 2596
#define spi1sckPin_offset 930
#define spi2MisoMode_offset 2601
#define spi2misoPin_offset 932
#define spi2MosiMode_offset 2600
#define spi2mosiPin_offset 931
#define spi2SckMode_offset 2599
#define spi2sckPin_offset 933
#define spi3MisoMode_offset 2604
#define spi3misoPin_offset 935
#define spi3MosiMode_offset 2603
#define spi3mosiPin_offset 934
#define spi3SckMode_offset 2602
#define spi3sckPin_offset 936
#define spi_device_e_auto_enum "SPI_NONE", "SPI_DEVICE_1", "SPI_DEVICE_2", "SPI_DEVICE_3", "SPI_DEVICE_4"
#define spi_pins_size 4
#define startCrankingDuration_offset 826
#define starterControlPin_offset 1772
#define starterControlPinMode_offset 2126
#define starterRelayDisablePin_offset 808
#define starterRelayDisablePinMode_offset 809
#define startOfCrankingPrimingPulse_offset 2032
#define startStopButtonMode_offset 1773
#define startStopButtonPin_offset 811
#define startUpFuelPumpDuration_offset 1892
#define stepperDcIo1_controlPin_offset 1090
#define stepperDcIo1_directionPin1_offset 1088
#define stepperDcIo1_directionPin2_offset 1089
#define stepperDcIo1_disablePin_offset 1091
#define stepperDcIo1_offset 1088
#define stepperDcIo2_controlPin_offset 1094
#define stepperDcIo2_directionPin1_offset 1092
#define stepperDcIo2_directionPin2_offset 1093
#define stepperDcIo2_disablePin_offset 1095
#define stepperDcIo2_offset 1092
#define stepperDirectionPinMode_offset 3102
#define stepperEnablePin_offset 3104
#define stepperEnablePinMode_offset 2605
#define stepperForceParkingEveryRestart_offset 744
#define stepperParkingExtraSteps_offset 818
#define STFT_BANK_COUNT 2
#define stft_cell_cfg_s_size 4
#define STFT_CELL_COUNT 4
#define stft_cellCfgs1_maxAdd_offset 1072
#define stft_cellCfgs1_maxRemove_offset 1073
#define stft_cellCfgs1_offset 1072
#define stft_cellCfgs1_timeConstant_offset 1074
#define stft_cellCfgs2_maxAdd_offset 1076
#define stft_cellCfgs2_maxRemove_offset 1077
#define stft_cellCfgs2_offset 1076
#define stft_cellCfgs2_timeConstant_offset 1078
#define stft_cellCfgs3_maxAdd_offset 1080
#define stft_cellCfgs3_maxRemove_offset 1081
#define stft_cellCfgs3_offset 1080
#define stft_cellCfgs3_timeConstant_offset 1082
#define stft_cellCfgs4_maxAdd_offset 1084
#define stft_cellCfgs4_maxRemove_offset 1085
#define stft_cellCfgs4_offset 1084
#define stft_cellCfgs4_timeConstant_offset 1086
#define stft_deadband_offset 1067
#define stft_maxAfr_offset 1070
#define stft_maxIdleRegionRpm_offset 1064
#define stft_maxOverrunLoad_offset 1065
#define stft_minAfr_offset 1069
#define stft_minClt_offset 1068
#define stft_minPowerLoad_offset 1066
#define stft_offset 1064
#define stft_s_size 24
#define stft_startupDelay_offset 1071
#define stftIgnoreErrorMagnitude_offset 976
#define stoichRatioPrimary_offset 4005
#define stoichRatioSecondary_offset 4009
#define switch_input_pin_e_enum "NONE", "INVALID", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7", "PA8", "PA9", "PA10", "PA11", "PA12", "PA13", "PA14", "PA15", "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PB8", "PB9", "PB10", "PB11", "PB12", "PB13", "PB14", "PB15", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7", "PC8", "PC9", "PC10", "PC11", "PC12", "PC13", "PC14", "PC15", "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PD8", "PD9", "PD10", "PD11", "PD12", "PD13", "PD14", "PD15", "PE0", "PE1", "PE2", "PE3", "PE4", "PE5", "PE6", "PE7", "PE8", "PE9", "PE10", "PE11", "PE12", "PE13", "PE14", "PE15", "PF0", "PF1", "PF2", "PF3", "PF4", "PF5", "PF6", "PF7", "PF8", "PF9", "PF10", "PF11", "PF12", "PF13", "PF14", "PF15", "PG0", "PG1", "PG2", "PG3", "PG4", "PG5", "PG6", "PG7", "PG8", "PG9", "PG10", "PG11", "PG12", "PG13", "PG14", "PG15", "PH0", "PH1", "PH2", "PH3", "PH4", "PH5", "PH6", "PH7", "PH8", "PH9", "PH10", "PH11", "PH12", "PH13", "PH14", "PH15", "PI0", "PI1", "PI2", "PI3", "PI4", "PI5", "PI6", "PI7", "PI8", "PI9", "PI10", "PI11", "PI12", "PI13", "PI14", "PI15", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID", "INVALID"
#define tachOutputPin_offset 704
#define tachOutputPinMode_offset 705
#define tachPulseDuractionMs_offset 1704
#define tachPulseDurationAsDutyCycle_offset 1464
#define tachPulsePerRev_offset 1775
#define targetVBatt_offset 2024
#define tChargeAirCoefMax_offset 3868
#define tChargeAirCoefMin_offset 3864
#define tChargeAirDecrLimit_offset 3880
#define tChargeAirFlowMax_offset 3872
#define tChargeAirIncrLimit_offset 3876
#define tChargeMaxRpmMaxTps_offset 2244
#define tChargeMaxRpmMinTps_offset 2240
#define tChargeMinRpmMaxTps_offset 2236
#define tChargeMinRpmMinTps_offset 2232
#define tChargeMode_e_auto_enum "TCHARGE_MODE_RPM_TPS", "TCHARGE_MODE_AIR_INTERP"
#define tChargeMode_e_enum "RPM+TPS (Default)", "Air Mass Interpolation"
#define tChargeMode_offset 3884
#define TCU_GEAR_COUNT 10
#define tcu_solenoid1_offset 1192
#define tcu_solenoid2_offset 1193
#define tcu_solenoid3_offset 1194
#define tcu_solenoid4_offset 1195
#define tcu_solenoid5_offset 1196
#define tcu_solenoid6_offset 1197
#define TCU_SOLENOID_COUNT 6
#define tcu_solenoid_mode1_offset 4517
#define tcu_solenoid_mode2_offset 4518
#define tcu_solenoid_mode3_offset 4519
#define tcu_solenoid_mode4_offset 4520
#define tcu_solenoid_mode5_offset 4521
#define tcu_solenoid_mode6_offset 4522
#define tcuDownshiftButtonPin_offset 1819
#define tcuDownshiftButtonPinMode_offset 4515
#define tcuEnabled_offset 976
#define tcuSolenoidTable_offset 15136
#define tcuUpshiftButtonPin_offset 1818
#define tcuUpshiftButtonPinMode_offset 4514
#define tempHpfpDuration_offset 726
#define tempHpfpStart_offset 724
#define test557pin_offset 3101
#define thermistor_conf_s_size 28
#define ThermistorConf_size 32
#define throttlePedalPositionAdcChannel_offset 580
#define throttlePedalPositionSecondAdcChannel_offset 683
#define throttlePedalSecondaryUpVoltage_offset 2100
#define throttlePedalSecondaryWOTVoltage_offset 2104
#define throttlePedalUpPin_offset 583
#define throttlePedalUpPinMode_offset 710
#define throttlePedalUpVoltage_offset 1884
#define throttlePedalWOTVoltage_offset 1888
#define timing_mode_e_auto_enum "TM_DYNAMIC", "TM_FIXED"
#define timing_offset_cylinder10_offset 2576
#define timing_offset_cylinder11_offset 2580
#define timing_offset_cylinder12_offset 2584
#define timing_offset_cylinder1_offset 2540
#define timing_offset_cylinder2_offset 2544
#define timing_offset_cylinder3_offset 2548
#define timing_offset_cylinder4_offset 2552
#define timing_offset_cylinder5_offset 2556
#define timing_offset_cylinder6_offset 2560
#define timing_offset_cylinder7_offset 2564
#define timing_offset_cylinder8_offset 2568
#define timing_offset_cylinder9_offset 2572
#define timingAdditive_offset 10072
#define timingMode_offset 448
#define timingMultiplier_offset 9872
#define tle6240_cs_offset 581
#define tle6240_csPinMode_offset 582
#define tle6240spiDevice_offset 4004
#define tle8888_cs_offset 3105
#define tle8888_csPinMode_offset 3106
#define tle8888_mode_e_auto_enum "TL_AUTO", "TL_SEMI_AUTO", "TL_MANUAL", "TL_HALL"
#define tle8888mode_offset 2416
#define tle8888spiDevice_offset 4000
#define TOOTH_DATA_LENGTH 2000
#define TOOTH_PACKET_COUNT 1000
#define TOOTH_PACKET_SIZE 2
#define TOP_DEAD_CENTER_MESSAGE "r"
#define TOTAL_CONFIG_SIZE 20000
#define tps1_1AdcChannel_offset 512
#define tps1_2AdcChannel_offset 681
#define tps1SecondaryMax_offset 822
#define tps1SecondaryMin_offset 820
#define tps2_1AdcChannel_offset 515
#define tps2_2AdcChannel_offset 682
#define tps2Max_offset 1770
#define tps2Min_offset 1768
#define tps2SecondaryMax_offset 1462
#define tps2SecondaryMin_offset 1460
#define TPS_1_BYTE_PACKING_MULT 2
#define TPS_2_BYTE_PACKING_MULT 100
#define TPS_TPS_ACCEL_TABLE 8
#define tpsAccelEnrichmentThreshold_offset 2048
#define tpsAccelFractionDivisor_offset 3996
#define tpsAccelFractionPeriod_offset 3994
#define tpsAccelLength_offset 2044
#define tpsDecelEnleanmentMultiplier_offset 2084
#define tpsDecelEnleanmentThreshold_offset 2080
#define tpsErrorDetectionTooHigh_offset 86
#define tpsErrorDetectionTooLow_offset 84
#define tpsMax_offset 82
#define tpsMin_offset 80
#define tpsTpsAccelFromRpmBins_offset 19232
#define tpsTpsAccelTable_offset 18976
#define tpsTpsAccelToRpmBins_offset 19264
#define trailingCoilPins10_offset 2349
#define trailingCoilPins11_offset 2350
#define trailingCoilPins12_offset 2351
#define trailingCoilPins1_offset 2340
#define trailingCoilPins2_offset 2341
#define trailingCoilPins3_offset 2342
#define trailingCoilPins4_offset 2343
#define trailingCoilPins5_offset 2344
#define trailingCoilPins6_offset 2345
#define trailingCoilPins7_offset 2346
#define trailingCoilPins8_offset 2347
#define trailingCoilPins9_offset 2348
#define trailingSparkAngle_offset 520
#define trigger_config_s_size 16
#define trigger_customSkippedToothCount_offset 536
#define trigger_customTotalToothCount_offset 532
#define TRIGGER_INPUT_PIN_COUNT 3
#define trigger_offset 524
#define TRIGGER_SIMULATOR_PIN_COUNT 3
#define trigger_todoRemoveMeOneDay0_offset 528
#define trigger_todoRemoveMeOneDay1_offset 528
#define trigger_type_e_auto_enum "TT_TOOTHED_WHEEL", "TT_FORD_ASPIRE", "TT_DODGE_NEON_1995", "TT_MAZDA_MIATA_NA", "TT_MAZDA_MIATA_NB1", "TT_GM_7X", "TT_DAIHATSU", "TT_MAZDA_SOHC_4", "TT_TOOTHED_WHEEL_60_2", "TT_TOOTHED_WHEEL_36_1", "TT_HONDA_4_24_1", "TT_MITSUBISHI", "TT_HONDA_4_24", "TT_HONDA_1_4_24", "TT_DODGE_NEON_2003_CAM", "TT_MAZDA_DOHC_1_4", "TT_ONE_PLUS_ONE", "TT_VVT_JZ", "TT_ONE", "TT_DODGE_RAM", "TT_60_2_VW", "TT_HONDA_1_24", "TT_DODGE_STRATUS", "TT_36_2_2_2", "TT_NISSAN_SR20VE", "TT_2JZ_3_34", "TT_ROVER_K", "TT_GM_LS_24", "TT_HONDA_CBR_600", "TT_2JZ_1_12", "TT_CHRYSLER_NGC_36_2_2", "TT_3_1_CAM", "TT_DODGE_NEON_2003_CRANK", "TT_MIATA_VVT", "TT_HONDA_ACCORD_1_24_SHIFTED", "TT_MAZDA_MIATA_VVT_TEST", "TT_SUBARU_7_6", "TT_JEEP_18_2_2_2", "TT_NISSAN_SR20VE_360", "TT_DODGE_NEON_1995_ONLY_CRANK", "TT_JEEP_4_CYL", "TT_FIAT_IAW_P8", "TT_FORD_ST170", "TT_VVT_MIATA_NB2", "TT_RENIX_44_2_2", "TT_RENIX_66_2_2_2", "TT_HONDA_K_12_1", "TT_VVT_BOSCH_QUICK_START", "TT_TOOTHED_WHEEL_36_2", "TT_SUBARU_SVX", "TT_1_16", "TT_SUBARU_7_WITHOUT_6", "TT_52", "TT_TRI_TACH", "TT_GM_60_2_2_2", "TT_SKODA_FAVORIT", "TT_VVT_BARRA_3_PLUS_1", "TT_KAWA_KX450F", "TT_NISSAN_VQ35", "TT_VVT_NISSAN_VQ35", "TT_NISSAN_VQ30", "TT_UNUSED"
#define trigger_type_e_enum "custom toothed wheel", "Ford Aspire", "Dodge Neon 1995", "Miata NA", "Miata NB", "GM_7X", "Cooper R50", "Mazda SOHC 4", "60/2", "36/1", "Honda 4+24+1", "Mitsubishi", "Honda 4+24", "Honda 1+4+24", "Dodge Neon 2003", "Mazda DOHC 1+4", "1+1", "INVALID", "Single Tooth", "Dodge Ram 1+16", "60/2 VW", "Honda 1+24", "Dodge Stratus", "36_2_2_2", "Nissan Primera", "dev 2JZ 3/34 simulator", "Rover K", "GM LS 24", "Honda CBR 600", "2JZ_1_12", "Honda CBR 600 custom", "3/1 skipped", "Dodge Neon 2003 crank", "Miata VVT", "trg34", "trg35", "Subaru 7+6", "Jeep 18-2-2-2", "WIP", "Dodge Neon 1995 crank only", "Jeep XJ 4 cyl", "FiatIAQ_P8", "Mazda Z5", "trg43", "Renix 44-2-2", "Renix 66-2-2-2", "Honda K 12+1", "trg47", "36/2", "Subaru SVX", "1+16", "Subaru 7 without 6", "trg52", "TriTach", "GM 60/2/2/2", "Skoda Favorit", "Barra 3+1 Cam", "Kawa KX450F", "Nissan VQ35", "trg59", "Nissan VQ30", "trg61", "trg62", "INVALID"
#define trigger_type_offset 524
#define trigger_unusedBit_4_10_offset 528
#define trigger_unusedBit_4_11_offset 528
#define trigger_unusedBit_4_12_offset 528
#define trigger_unusedBit_4_13_offset 528
#define trigger_unusedBit_4_14_offset 528
#define trigger_unusedBit_4_15_offset 528
#define trigger_unusedBit_4_16_offset 528
#define trigger_unusedBit_4_17_offset 528
#define trigger_unusedBit_4_18_offset 528
#define trigger_unusedBit_4_19_offset 528
#define trigger_unusedBit_4_20_offset 528
#define trigger_unusedBit_4_21_offset 528
#define trigger_unusedBit_4_22_offset 528
#define trigger_unusedBit_4_23_offset 528
#define trigger_unusedBit_4_24_offset 528
#define trigger_unusedBit_4_25_offset 528
#define trigger_unusedBit_4_26_offset 528
#define trigger_unusedBit_4_27_offset 528
#define trigger_unusedBit_4_28_offset 528
#define trigger_unusedBit_4_29_offset 528
#define trigger_unusedBit_4_30_offset 528
#define trigger_unusedBit_4_31_offset 528
#define trigger_unusedBit_4_3_offset 528
#define trigger_unusedBit_4_4_offset 528
#define trigger_unusedBit_4_5_offset 528
#define trigger_unusedBit_4_6_offset 528
#define trigger_unusedBit_4_7_offset 528
#define trigger_unusedBit_4_8_offset 528
#define trigger_unusedBit_4_9_offset 528
#define trigger_useOnlyFirstChannel_offset 528
#define triggerCompCenterVolt_offset 4036
#define triggerCompHystMax_offset 4038
#define triggerCompHystMin_offset 4037
#define triggerCompSensorSatRpm_offset 4039
#define triggerErrorPin_offset 828
#define triggerErrorPinMode_offset 829
#define triggerGapOverride10_offset 4576
#define triggerGapOverride11_offset 4580
#define triggerGapOverride12_offset 4584
#define triggerGapOverride13_offset 4588
#define triggerGapOverride14_offset 4592
#define triggerGapOverride15_offset 4596
#define triggerGapOverride16_offset 4600
#define triggerGapOverride17_offset 4604
#define triggerGapOverride18_offset 4608
#define triggerGapOverride1_offset 4540
#define triggerGapOverride2_offset 4544
#define triggerGapOverride3_offset 4548
#define triggerGapOverride4_offset 4552
#define triggerGapOverride5_offset 4556
#define triggerGapOverride6_offset 4560
#define triggerGapOverride7_offset 4564
#define triggerGapOverride8_offset 4568
#define triggerGapOverride9_offset 4572
#define triggerInputDebugPins1_offset 1756
#define triggerInputDebugPins2_offset 1757
#define triggerInputDebugPins3_offset 1758
#define triggerInputPins1_offset 700
#define triggerInputPins2_offset 701
#define triggerInputPins3_offset 702
#define TRIGGERS_FILE_NAME "triggers.txt"
#define triggerSimulatorFrequency_offset 620
#define triggerSimulatorPinModes1_offset 739
#define triggerSimulatorPinModes2_offset 740
#define triggerSimulatorPinModes3_offset 741
#define triggerSimulatorPins1_offset 736
#define triggerSimulatorPins2_offset 737
#define triggerSimulatorPins3_offset 738
#define TS_BURN_COMMAND 'B'
#define TS_BURN_COMMAND_char B
#define TS_CHUNK_WRITE_COMMAND 'C'
#define TS_CHUNK_WRITE_COMMAND_char C
#define TS_COMMAND_F 'F'
#define TS_COMMAND_F_char F
#define TS_COMPOSITE_DISABLE 2
#define TS_COMPOSITE_ENABLE 1
#define TS_CRC_CHECK_COMMAND 'k'
#define TS_CRC_CHECK_COMMAND_char k
#define TS_EXECUTE 'E'
#define TS_EXECUTE_char E
#define TS_FILE_VERSION 20200310
#define TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY '8'
#define TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY_char 8
#define TS_GET_CONFIG_ERROR 'e'
#define TS_GET_CONFIG_ERROR_char e
#define TS_GET_FIRMWARE_VERSION 'V'
#define TS_GET_FIRMWARE_VERSION_char V
#define TS_GET_LOGGER_GET_BUFFER 'L'
#define TS_GET_LOGGER_GET_BUFFER_char L
#define TS_GET_STRUCT '9'
#define TS_GET_STRUCT_char 9
#define TS_GET_TEXT 'G'
#define TS_GET_TEXT_char G
#define TS_HELLO_COMMAND 'S'
#define TS_HELLO_COMMAND_char S
#define TS_IO_TEST_COMMAND 'Z'
#define TS_IO_TEST_COMMAND_char Z
#define TS_ONLINE_PROTOCOL 'z'
#define TS_ONLINE_PROTOCOL_char z
#define TS_OUTPUT_COMMAND 'O'
#define TS_OUTPUT_COMMAND_char O
#define TS_OUTPUT_SIZE 340
#define TS_PAGE_COMMAND 'P'
#define TS_PAGE_COMMAND_char P
#define TS_PERF_TRACE_BEGIN '_'
#define TS_PERF_TRACE_BEGIN_char _
#define TS_PERF_TRACE_GET_BUFFER 'b'
#define TS_PERF_TRACE_GET_BUFFER_char b
#define TS_PROTOCOL "001"
#define TS_READ_COMMAND 'R'
#define TS_READ_COMMAND_char R
#define TS_RESPONSE_BURN_OK 4
#define TS_RESPONSE_COMMAND_OK 7
#define TS_RESPONSE_CRC_FAILURE 0x82
#define TS_RESPONSE_FRAMING_ERROR 0x8D
#define TS_RESPONSE_OK 0
#define TS_RESPONSE_OUT_OF_RANGE 0x84
#define TS_RESPONSE_UNDERRUN 0x80
#define TS_RESPONSE_UNRECOGNIZED_COMMAND 0x83
#define TS_SET_LOGGER_SWITCH 'l'
#define TS_SET_LOGGER_SWITCH_char l
#define ts_show_analog_divider true
#define ts_show_auxserial_pins true
#define ts_show_can2 true
#define ts_show_can_pins true
#define ts_show_cj125 true
#define ts_show_egt true
#define ts_show_etb true
#define ts_show_etb_pins true
#define ts_show_full_pinout true
#define ts_show_gps true
#define ts_show_hardware_simulator true
#define ts_show_hip9011 true
#define ts_show_joystick true
#define ts_show_lcd true
#define ts_show_main_relay true
#define ts_show_main_relay_microRusEFI_message false
#define ts_show_sd_card true
#define ts_show_software_knock false
#define ts_show_spi true
#define ts_show_trigger_comparator false
#define ts_show_tunerstudio_port true
#define TS_SIGNATURE "rusEFI 2021.07.30.atlas.1133107817"
#define TS_SINGLE_WRITE_COMMAND 'W'
#define TS_SINGLE_WRITE_COMMAND_char W
#define TS_TEST_COMMAND 't'
#define TS_TEST_COMMAND_char t
#define TT_TT_1_16 50
#define TT_TT_2JZ_1_12 29
#define TT_TT_2JZ_3_34 25
#define TT_TT_36_2_2_2 23
#define TT_TT_3_1_CAM 31
#define TT_TT_52 52
#define TT_TT_60_2_VW 20
#define TT_TT_CHRYSLER_NGC_36_2_2 30
#define TT_TT_DAIHATSU 6
#define TT_TT_FORD_ASPIRE 1
#define TT_TT_FORD_ST170 42
#define TT_TT_GM_60_2_2_2 54
#define TT_TT_GM_7X 5
#define TT_TT_GM_LS_24 27
#define TT_TT_HONDA_1_24 21
#define TT_TT_HONDA_1_4_24 13
#define TT_TT_HONDA_4_24_1 10
#define TT_TT_HONDA_CBR_600 28
#define TT_TT_HONDA_K_12_1 46
#define TT_TT_JEEP_18_2_2_2 37
#define TT_TT_JEEP_4_CYL 40
#define TT_TT_KAWA_KX450F 57
#define TT_TT_MAZDA_DOHC_1_4 15
#define TT_TT_MAZDA_MIATA_NA 3
#define TT_TT_MAZDA_MIATA_NB1 4
#define TT_TT_MAZDA_SOHC_4 7
#define TT_TT_MIATA_VVT 33
#define TT_TT_NISSAN_VQ30 60
#define TT_TT_NISSAN_VQ35 58
#define TT_TT_ONE 18
#define TT_TT_RENIX_44_2_2 44
#define TT_TT_RENIX_66_2_2_2 45
#define TT_TT_ROVER_K 26
#define TT_TT_SKODA_FAVORIT 55
#define TT_TT_SUBARU_7_6 36
#define TT_TT_SUBARU_7_WITHOUT_6 51
#define TT_TT_SUBARU_SVX 49
#define TT_TT_TOOTHED_WHEEL 0
#define TT_TT_TOOTHED_WHEEL_36_1 9
#define TT_TT_TOOTHED_WHEEL_36_2 48
#define TT_TT_TOOTHED_WHEEL_60_2 8
#define TT_TT_TRI_TACH 53
#define TT_TT_VVT_BARRA_3_PLUS_1 56
#define TT_TT_VVT_BOSCH_QUICK_START 47
#define TT_TT_VVT_JZ 17
#define TT_TT_VVT_MIATA_NB2 43
#define TT_TT_VVT_NISSAN_VQ35 59
#define tunerStudioSerialSpeed_offset 728
#define turboSpeedSensorInputPin_offset 1759
#define turboSpeedSensorMultiplier_offset 1488
#define twoWireBatchIgnition_offset 1476
#define twoWireBatchInjection_offset 1476
#define uart_device_e_auto_enum "UART_NONE", "UART_DEVICE_1", "UART_DEVICE_2", "UART_DEVICE_3", "UART_DEVICE_4"
#define uartConsoleSerialSpeed_offset 2076
#define unused1059_offset 3964
#define unused1126_offset 2116
#define unused1127_offset 2116
#define unused1128_offset 2116
#define unused1129_offset 2116
#define unused1130_offset 2116
#define unused1366_offset 2684
#define unused1464b0_offset 1464
#define unused1464b4_offset 1464
#define unused1476b19_offset 1476
#define unused1476b20_offset 1476
#define unused1476b3_offset 1476
#define unused1476b8_offset 1476
#define unused2260_offset 2260
#define unused2432_offset 2432
#define unused244_2_offset 2424
#define unused244_3_offset 2428
#define unused2508_offset 2511
#define unused2536_offset 2536
#define unused3328_offset 3340
#define unused3942_offset 3992
#define unused3988_offset 3988
#define unused4028_offset 4028
#define unused6312_offset 6312
#define unused744b4_offset 744
#define unused76b19_offset 76
#define unused976b11_offset 976
#define unused_1484_bit_24_offset 1476
#define unused_1484_bit_25_offset 1476
#define unused_1484_bit_26_offset 1476
#define unused_1484_bit_27_offset 1476
#define unused_1484_bit_28_offset 1476
#define unused_1484_bit_29_offset 1476
#define unused_1484_bit_30_offset 1476
#define unused_1484_bit_31_offset 1476
#define unusedAtOldBoardConfigurationEnd_offset 1276
#define unusedBit4_1476_offset 1476
#define unusedBit_251_29_offset 976
#define unusedBit_291_30_offset 976
#define unusedBit_291_31_offset 976
#define unusedBit_34_31_offset 76
#define unusedBit_500_10_offset 2116
#define unusedBit_500_11_offset 2116
#define unusedBit_500_12_offset 2116
#define unusedBit_500_13_offset 2116
#define unusedBit_500_14_offset 2116
#define unusedBit_500_15_offset 2116
#define unusedBit_500_16_offset 2116
#define unusedBit_500_17_offset 2116
#define unusedBit_500_18_offset 2116
#define unusedBit_500_19_offset 2116
#define unusedBit_500_20_offset 2116
#define unusedBit_500_21_offset 2116
#define unusedBit_500_22_offset 2116
#define unusedBit_500_23_offset 2116
#define unusedBit_500_24_offset 2116
#define unusedBit_500_25_offset 2116
#define unusedBit_500_26_offset 2116
#define unusedBit_500_27_offset 2116
#define unusedBit_500_28_offset 2116
#define unusedBit_500_29_offset 2116
#define unusedBit_500_30_offset 2116
#define unusedBit_500_31_offset 2116
#define unusedBit_500_8_offset 2116
#define unusedBit_500_9_offset 2116
#define unusedOldBiquad_offset 2331
#define unusedSpiPadding4_offset 2593
#define unusedSpiPadding8_offset 4010
#define unusedVvtOffsetWasHere_offset 2052
#define useAdvanceCorrectionsForCranking_offset 1476
#define useCicPidForIdle_offset 76
#define useETBforIdleControl_offset 744
#define useFixedBaroCorrFromMap_offset 1476
#define useFordRedundantTps_offset 76
#define useFSIO10ForServo3_offset 1464
#define useFSIO11ForServo4_offset 1464
#define useFSIO12ForIdleOffset_offset 1464
#define useFSIO12ForServo5_offset 1464
#define useFSIO13ForIdleMinValue_offset 1464
#define useFSIO15ForIdleRpmAdjustment_offset 1464
#define useFSIO16ForTimingAdjustment_offset 1464
#define useFSIO4ForSeriousEngineWarning_offset 1464
#define useFSIO5ForCriticalIssueEngineStop_offset 1464
#define useFSIO6ForRevLimiter_offset 1464
#define useFSIO8ForServo1_offset 1464
#define useFSIO9ForServo2_offset 1464
#define useFSIOTableForCanSniffingFiltering_offset 76
#define useHbridges_offset 976
#define useIacPidMultTable_offset 1476
#define useIacTableForCoasting_offset 744
#define useIdleTimingPidControl_offset 744
#define useInstantRpmForIdle_offset 76
#define useLcdScreen_offset 744
#define useLinearCltSensor_offset 1464
#define useLinearIatSensor_offset 1464
#define useNoiselessTriggerDecoder_offset 744
#define useOnlyRisingEdgeForTrigger_offset 1476
#define useRunningMathForCranking_offset 76
#define useSeparateAdvanceForCranking_offset 1476
#define useSeparateAdvanceForIdle_offset 1476
#define useSeparateVeForIdle_offset 1476
#define useSerialPort_offset 744
#define useStepperIdle_offset 744
#define useTLE8888_cranking_hack_offset 76
#define useTLE8888_stepper_offset 76
#define useTpicAdvancedMode_offset 744
#define VBAT_INJECTOR_CURVE_SIZE 8
#define vbattAdcChannel_offset 513
#define vbattDividerCoeff_offset 464
#define ve_override_e_auto_enum "VE_None", "VE_MAP", "VE_TPS"
#define ve_override_e_enum "None", "MAP", "TPS"
#define VEHICLE_INFO_SIZE 32
#define vehicleName_offset 1160
#define vehicleSpeedCoef_offset 476
#define vehicleSpeedSensorInputPin_offset 968
#define vehicleWeight_offset 1456
#define veLoadBins_offset 18464
#define veOverrideMode_offset 2109
#define verboseCan2BaseAddress_offset 2112
#define verboseCanBaseAddress_offset 756
#define verboseTLE8888_offset 744
#define verboseTriggerSynchDetails_offset 1476
#define verboseVVTDecoding_offset 976
#define veRpmBins_offset 18528
#define veTable_offset 17440
#define VM_VVT_INACTIVE 0
#define VOLTAGE_1_BYTE_PACKING_DIV 0.02
#define vr_threshold_s_size 16
#define vRefAdcChannel_offset 1470
#define vrThreshold1_offset 1244
#define vrThreshold1_pad_offset 1245
#define vrThreshold1_pin_offset 1244
#define vrThreshold1_rpmBins_offset 1248
#define vrThreshold1_values_offset 1254
#define vrThreshold2_offset 1260
#define vrThreshold2_pad_offset 1261
#define vrThreshold2_pin_offset 1260
#define vrThreshold2_rpmBins_offset 1264
#define vrThreshold2_values_offset 1270
#define vssFilterReciprocal_offset 15196
#define vvt_mode_e_auto_enum "VVT_INACTIVE", "VVT_SECOND_HALF", "VVT_2JZ", "VVT_MIATA_NB2", "VVT_FIRST_HALF", "VVT_BOSCH_QUICK_START", "VVT_4_1", "VVT_FORD_ST170", "VVT_BARRA_3_PLUS_1", "VVT_NISSAN_VQ"
#define vvt_mode_e_enum "Inactive", "Single Tooth Second Half", "2GZ", "Miata NB2", "Single Tooth First Half", "Bosch Quick Start", "4/1", "ST 170", "Ford Barra 3+1", "Nissan VQ", "vvt10", "vvt11"
#define vvtCamSensorUseRise_offset 744
#define vvtMode1_offset 2328
#define vvtMode2_offset 2329
#define vvtModePadding_offset 2330
#define vvtOffsets1_offset 1228
#define vvtOffsets2_offset 1232
#define vvtOffsets3_offset 1236
#define vvtOffsets4_offset 1240
#define vvtOffsetsPadding_offset 1244
#define vvtTable1_offset 15776
#define vvtTable1LoadBins_offset 15840
#define vvtTable1RpmBins_offset 15872
#define vvtTable2_offset 15904
#define vvtTable2LoadBins_offset 15968
#define vvtTable2RpmBins_offset 16000
#define warning_message_offset 6000
#define warningLedPin_offset 2041
#define warningPeriod_offset 1498
#define wastegatePositionMax_offset 718
#define wastegatePositionMin_offset 716
#define wastegatePositionSensor_offset 2127
#define wboHeaterPin_offset 673
#define wwaeBeta_offset 1808
#define wwaeTau_offset 1712
// start of stft_cell_cfg_s
struct stft_cell_cfg_s {
	/**
	%
	 * offset 0
	 */
	int8_t maxAdd = (int8_t)0;
	/**
	%
	 * offset 1
	 */
	int8_t maxRemove = (int8_t)0;
	/**
	sec
	 * offset 2
	 */
	uint16_t timeConstant = (uint16_t)0;
	/** total size 4*/
};

// start of stft_s
struct stft_s {
	/**
	 * Below this RPM, the idle region is active
	RPM
	 * offset 0
	 */
	uint8_t maxIdleRegionRpm = (uint8_t)0;
	/**
	 * Below this engine load, the overrun region is active
	load
	 * offset 1
	 */
	uint8_t maxOverrunLoad = (uint8_t)0;
	/**
	 * Above this engine load, the power region is active
	load
	 * offset 2
	 */
	uint8_t minPowerLoad = (uint8_t)0;
	/**
	 * When close to correct AFR, pause correction. This can improve stability by not changing the adjustment if the error is extremely small, but is not required.
	%
	 * offset 3
	 */
	uint8_t deadband = (uint8_t)0;
	/**
	 * Below this temperature, correction is disabled.
	C
	 * offset 4
	 */
	int8_t minClt = (int8_t)0;
	/**
	 * Below this AFR, correction is paused
	afr
	 * offset 5
	 */
	uint8_t minAfr = (uint8_t)0;
	/**
	 * Above this AFR, correction is paused
	afr
	 * offset 6
	 */
	uint8_t maxAfr = (uint8_t)0;
	/**
	 * Delay after starting the engine before beginning closed loop correction.
	seconds
	 * offset 7
	 */
	uint8_t startupDelay = (uint8_t)0;
	/**
	 * offset 8
	 */
	stft_cell_cfg_s cellCfgs[STFT_CELL_COUNT];
	/** total size 24*/
};

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
	ms
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

// start of cranking_parameters_s
struct cranking_parameters_s {
	/**
	 * Base mass of the per-cylinder fuel injected during cranking. This is then modified by the multipliers for CLT, IAT, TPS ect, to give the final cranking pulse width.
	 * A reasonable starting point is 60mg per liter per cylinder.
	 * ex: 2 liter 4 cyl = 500cc/cyl, so 30mg cranking fuel.
	mg
	 * offset 0
	 */
	float baseFuel = (float)0;
	/**
	 * This sets the RPM limit below which the ECU will use cranking fuel and ignition logic, typically this is around 350-450rpm. 
	 * set cranking_rpm X
	RPM
	 * offset 4
	 */
	int16_t rpm = (int16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 6
	 */
	uint8_t alignmentFill_at_6[2];
	/** total size 8*/
};

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
	units
	 * offset 3
	 */
	uint8_t alignmentFill_at_3[1];
	/** total size 4*/
};

// start of gppwm_channel
struct gppwm_channel {
	/**
	 * Select a pin to use for PWM or on-off output.
	 * offset 0
	 */
	output_pin_e pin;
	/**
	 * If an error (with a sensor, etc) is detected, this value is used instead of reading from the table.
	 * This should be a safe value for whatever hardware is connected to prevent damage.
	%
	 * offset 1
	 */
	uint8_t dutyIfError = (uint8_t)0;
	/**
	 * Select a frequency to run PWM at.
	 * Set this to 0hz to enable on-off mode.
	hz
	 * offset 2
	 */
	uint16_t pwmFrequency = (uint16_t)0;
	/**
	 * In on-off mode, turn the output on when the table value is above this duty.
	%
	 * offset 4
	 */
	uint8_t onAboveDuty = (uint8_t)0;
	/**
	 * In on-off mode, turn the output off when the table value is below this duty.
	%
	 * offset 5
	 */
	uint8_t offBelowDuty = (uint8_t)0;
	/**
	 * Selects the load axis to use for the table.
	 * offset 6
	 */
	gppwm_channel_e loadAxis;
	/**
	unit
	 * offset 7
	 */
	uint8_t alignmentFill_map = (uint8_t)0;
	/**
	load
	 * offset 8
	 */
	uint8_t loadBins[GPPWM_LOAD_COUNT];
	/**
	RPM
	 * offset 16
	 */
	uint8_t rpmBins[GPPWM_RPM_COUNT];
	/**
	 * offset 24
	 */
	gppwm_table_t table;
	/** total size 88*/
};

// start of air_pressure_sensor_config_s
struct air_pressure_sensor_config_s {
	/**
	 * kPa value at low volts
	kpa
	 * offset 0
	 */
	float lowValue = (float)0;
	/**
	 * kPa value at high volts
	kpa
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
	unit
	 * offset 13
	 */
	uint8_t alignmentFill[3];
	/** total size 16*/
};

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
	deg
	 * offset 32
	 */
	float samplingAngle[MAP_ANGLE_SIZE];
	/**
	 * offset 64
	 */
	float samplingWindowBins[MAP_WINDOW_SIZE];
	/**
	 * @brief MAP averaging angle duration, by RPM
	deg
	 * offset 96
	 */
	float samplingWindow[MAP_WINDOW_SIZE];
	/**
	 * offset 128
	 */
	air_pressure_sensor_config_s sensor;
	/** total size 144*/
};

/**
 * @brief Thermistor known values

*/
// start of thermistor_conf_s
struct thermistor_conf_s {
	/**
	 * these values are in Celcius
	*C
	 * offset 0
	 */
	float tempC_1 = (float)0;
	/**
	*C
	 * offset 4
	 */
	float tempC_2 = (float)0;
	/**
	*C
	 * offset 8
	 */
	float tempC_3 = (float)0;
	/**
	Ohm
	 * offset 12
	 */
	float resistance_1 = (float)0;
	/**
	Ohm
	 * offset 16
	 */
	float resistance_2 = (float)0;
	/**
	Ohm
	 * offset 20
	 */
	float resistance_3 = (float)0;
	/**
	 * Pull-up resistor value on your board
	Ohm
	 * offset 24
	 */
	float bias_resistor = (float)0;
	/** total size 28*/
};

/**
 * @brief Linear sensor interpolation

*/
// start of linear_sensor_s
struct linear_sensor_s {
	/**
	 * offset 0
	 */
	adc_channel_e hwChannel;
	/**
	unit
	 * offset 1
	 */
	uint8_t alignmentFill[3];
	/**
	volts
	 * offset 4
	 */
	float v1 = (float)0;
	/**
	kPa
	 * offset 8
	 */
	float value1 = (float)0;
	/**
	volts
	 * offset 12
	 */
	float v2 = (float)0;
	/**
	kPa
	 * offset 16
	 */
	float value2 = (float)0;
	/** total size 20*/
};

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
	units
	 * offset 29
	 */
	uint8_t alignmentFill_at_29[3];
	/** total size 32*/
};

// start of injector_s
struct injector_s {
	/**
	 * This is your injector flow at the fuel pressure used in the vehicle. cc/min, cubic centimetre per minute
	 * By the way, g/s = 0.125997881 * (lb/hr)
	 * g/s = 0.125997881 * (cc/min)/10.5
	 * g/s = 0.0119997981 * cc/min
	cm3/min
	 * offset 0
	 */
	float flow = (float)0;
	/**
	 * set_flat_injector_lag LAG
	 * set_injector_lag VOLTAGE LAG
	volts
	 * offset 4
	 */
	float battLagCorrBins[VBAT_INJECTOR_CURVE_SIZE];
	/**
	 * ms delay between injector open and close dead times
	ms
	 * offset 36
	 */
	float battLagCorr[VBAT_INJECTOR_CURVE_SIZE];
	/** total size 68*/
};

// start of specs_s
struct specs_s {
	/**
	 * Engine displacement, in litres
	 * see also cylindersCount
	L
	 * offset 0
	 */
	float displacement = (float)0;
	/**
	 * offset 4
	 */
	uint32_t cylindersCount = (uint32_t)0;
	/**
	 * offset 8
	 */
	firing_order_e firingOrder;
	/** total size 12*/
};

/**
 * @brief Trigger wheel(s) configuration

*/
// start of trigger_config_s
struct trigger_config_s {
	/**
	 * https://github.com/rusefi/rusefi/wiki/All-Supported-Triggers
	 * set trigger_type X
	 * offset 0
	 */
	trigger_type_e type;
	/**
	offset 4 bit 0 */
	bool todoRemoveMeOneDay0 : 1;
	/**
	offset 4 bit 1 */
	bool todoRemoveMeOneDay1 : 1;
	/**
	 * This option could be used if your second trigger channel is broken
	offset 4 bit 2 */
	bool useOnlyFirstChannel : 1;
	/**
	offset 4 bit 3 */
	bool unusedBit_4_3 : 1;
	/**
	offset 4 bit 4 */
	bool unusedBit_4_4 : 1;
	/**
	offset 4 bit 5 */
	bool unusedBit_4_5 : 1;
	/**
	offset 4 bit 6 */
	bool unusedBit_4_6 : 1;
	/**
	offset 4 bit 7 */
	bool unusedBit_4_7 : 1;
	/**
	offset 4 bit 8 */
	bool unusedBit_4_8 : 1;
	/**
	offset 4 bit 9 */
	bool unusedBit_4_9 : 1;
	/**
	offset 4 bit 10 */
	bool unusedBit_4_10 : 1;
	/**
	offset 4 bit 11 */
	bool unusedBit_4_11 : 1;
	/**
	offset 4 bit 12 */
	bool unusedBit_4_12 : 1;
	/**
	offset 4 bit 13 */
	bool unusedBit_4_13 : 1;
	/**
	offset 4 bit 14 */
	bool unusedBit_4_14 : 1;
	/**
	offset 4 bit 15 */
	bool unusedBit_4_15 : 1;
	/**
	offset 4 bit 16 */
	bool unusedBit_4_16 : 1;
	/**
	offset 4 bit 17 */
	bool unusedBit_4_17 : 1;
	/**
	offset 4 bit 18 */
	bool unusedBit_4_18 : 1;
	/**
	offset 4 bit 19 */
	bool unusedBit_4_19 : 1;
	/**
	offset 4 bit 20 */
	bool unusedBit_4_20 : 1;
	/**
	offset 4 bit 21 */
	bool unusedBit_4_21 : 1;
	/**
	offset 4 bit 22 */
	bool unusedBit_4_22 : 1;
	/**
	offset 4 bit 23 */
	bool unusedBit_4_23 : 1;
	/**
	offset 4 bit 24 */
	bool unusedBit_4_24 : 1;
	/**
	offset 4 bit 25 */
	bool unusedBit_4_25 : 1;
	/**
	offset 4 bit 26 */
	bool unusedBit_4_26 : 1;
	/**
	offset 4 bit 27 */
	bool unusedBit_4_27 : 1;
	/**
	offset 4 bit 28 */
	bool unusedBit_4_28 : 1;
	/**
	offset 4 bit 29 */
	bool unusedBit_4_29 : 1;
	/**
	offset 4 bit 30 */
	bool unusedBit_4_30 : 1;
	/**
	offset 4 bit 31 */
	bool unusedBit_4_31 : 1;
	/**
	number
	 * offset 8
	 */
	int customTotalToothCount = (int)0;
	/**
	number
	 * offset 12
	 */
	int customSkippedToothCount = (int)0;
	/** total size 16*/
};

// start of afr_sensor_s
struct afr_sensor_s {
	/**
	 * offset 0
	 */
	adc_channel_e hwChannel;
	/**
	unit
	 * offset 1
	 */
	uint8_t alignmentFill_afr[3];
	/**
	volts
	 * offset 4
	 */
	float v1 = (float)0;
	/**
	AFR
	 * offset 8
	 */
	float value1 = (float)0;
	/**
	volts
	 * offset 12
	 */
	float v2 = (float)0;
	/**
	AFR
	 * offset 16
	 */
	float value2 = (float)0;
	/** total size 20*/
};

// start of idle_hardware_s
struct idle_hardware_s {
	/**
	Hz
	 * offset 0
	 */
	int solenoidFrequency = (int)0;
	/**
	 * offset 4
	 */
	output_pin_e solenoidPin;
	/**
	 * offset 5
	 */
	output_pin_e stepperDirectionPin;
	/**
	 * offset 6
	 */
	output_pin_e stepperStepPin;
	/**
	 * offset 7
	 */
	pin_output_mode_e solenoidPinMode;
	/** total size 8*/
};

// start of dc_io
struct dc_io {
	/**
	 * offset 0
	 */
	brain_pin_e directionPin1;
	/**
	 * offset 1
	 */
	brain_pin_e directionPin2;
	/**
	 * Acts as EN pin in two-wire mode
	 * offset 2
	 */
	brain_pin_e controlPin;
	/**
	 * offset 3
	 */
	brain_pin_e disablePin;
	/** total size 4*/
};

// start of vr_threshold_s
struct vr_threshold_s {
	/**
	 * offset 0
	 */
	brain_pin_e pin;
	/**
	 * offset 1
	 */
	uint8_t pad[3];
	/**
	rpm
	 * offset 4
	 */
	uint8_t rpmBins[6];
	/**
	volts
	 * offset 10
	 */
	uint8_t values[6];
	/** total size 16*/
};

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
	RPM
	 * offset 4
	 */
	int engineSnifferRpmThreshold = (int)0;
	/**
	 * offset 8
	 */
	injector_s injector;
	/**
	offset 76 bit 0 */
	bool isForcedInduction : 1;
	/**
	 * On Ford vehicles one of the sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.
	offset 76 bit 1 */
	bool useFordRedundantTps : 1;
	/**
	offset 76 bit 2 */
	bool isVerboseAuxPid1 : 1;
	/**
	offset 76 bit 3 */
	bool overrideTriggerGaps : 1;
	/**
	 * Turn on this fan when AC is on.
	offset 76 bit 4 */
	bool enableFan1WithAc : 1;
	/**
	 * Turn on this fan when AC is on.
	offset 76 bit 5 */
	bool enableFan2WithAc : 1;
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 76 bit 6 */
	bool disableFan1WhenStopped : 1;
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 76 bit 7 */
	bool disableFan2WhenStopped : 1;
	/**
	 * Enable secondary spark outputs that fire after the primary (rotaries, twin plug engines).
	offset 76 bit 8 */
	bool enableTrailingSparks : 1;
	/**
	 * enable cj125verbose/disable cj125verbose
	offset 76 bit 9 */
	bool isCJ125Verbose : 1;
	/**
	 * Is your UA CJ125 output wired to MCU via resistor divider? Ua can go over 3.3v but only at lambda >3, i.e very lean AFR above 44.1
	 * When exposed to free air and 17x gain, Ua will be 4.17 volt
	offset 76 bit 10 */
	bool cj125isUaDivided : 1;
	/**
	offset 76 bit 11 */
	bool cj125isLsu49 : 1;
	/**
	 * TLE7209 uses two-wire mode. TLE9201 and VNH2SP30 do NOT use two wire mode.
	offset 76 bit 12 */
	bool etb_use_two_wires : 1;
	/**
	 * Subaru/BMW style where default valve position is somewhere in the middle. First solenoid opens it more while second can close it more than default position.
	offset 76 bit 13 */
	bool isDoubleSolenoidIdle : 1;
	/**
	offset 76 bit 14 */
	bool showSdCardWarning : 1;
	/**
	 * Is your UR CJ125 output wired to MCU via resistor divider?
	 * Looks like 3v range should be enough, divider generally not needed.
	offset 76 bit 15 */
	bool cj125isUrDivided : 1;
	/**
	 * Switch between Industrial and Cic PID implementation
	offset 76 bit 16 */
	bool useCicPidForIdle : 1;
	/**
	offset 76 bit 17 */
	bool useTLE8888_cranking_hack : 1;
	/**
	offset 76 bit 18 */
	bool useInstantRpmForIdle : 1;
	/**
	offset 76 bit 19 */
	bool unused76b19 : 1;
	/**
	offset 76 bit 20 */
	bool launchControlEnabled : 1;
	/**
	offset 76 bit 21 */
	bool rollingLaunchEnabled : 1;
	/**
	offset 76 bit 22 */
	bool antiLagEnabled : 1;
	/**
	 * For cranking either use the specified fixed base fuel mass, or use the normal running math (VE table).
	offset 76 bit 23 */
	bool useRunningMathForCranking : 1;
	/**
	 * Shall we display real life signal or just the part consumed by trigger decoder.
	 * enable logic_level_trigger
	offset 76 bit 24 */
	bool displayLogicLevelsInEngineSniffer : 1;
	/**
	offset 76 bit 25 */
	bool useTLE8888_stepper : 1;
	/**
	 * If enabled, the MAP estimate table will be used if the MAP sensor fails to estimate manifold pressure based on RPM and TPS.
	offset 76 bit 26 */
	bool enableMapEstimationTableFallback : 1;
	/**
	offset 76 bit 27 */
	bool useFSIOTableForCanSniffingFiltering : 1;
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
	offset 76 bit 31 */
	bool unusedBit_34_31 : 1;
	/**
	 * Closed throttle, 1 volt = 200 units.
	 * See also tps1_1AdcChannel
	 * set tps_min X
	ADC
	 * offset 80
	 */
	int16_t tpsMin = (int16_t)0;
	/**
	 * Full throttle.
	 * See also tps1_1AdcChannel
	 * set tps_max X
	ADC
	 * offset 82
	 */
	int16_t tpsMax = (int16_t)0;
	/**
	 * TPS error detection: what throttle % is unrealistically low?
	 * Also used for accelerator pedal error detection if so equiped.
	%
	 * offset 84
	 */
	int16_t tpsErrorDetectionTooLow = (int16_t)0;
	/**
	 * TPS error detection: what throttle % is unrealistically high?
	 * Also used for accelerator pedal error detection if so equiped.
	%
	 * offset 86
	 */
	int16_t tpsErrorDetectionTooHigh = (int16_t)0;
	/**
	 * offset 88
	 */
	cranking_parameters_s cranking;
	/**
	*C
	 * offset 96
	 */
	float primingSquirtDurationMs = (float)0;
	/**
	 * Dwell duration while cranking
	ms
	 * offset 100
	 */
	float ignitionDwellForCrankingMs = (float)0;
	/**
	 * Once engine speed passes this value, start reducing ETB angle.
	rpm
	 * offset 104
	 */
	uint16_t etbRevLimitStart = (uint16_t)0;
	/**
	 * This far above 'Soft limiter start', fully close the throttle. At the bottom of the range, throttle control is normal. At the top of the range, the throttle is fully closed.
	rpm
	 * offset 106
	 */
	uint16_t etbRevLimitRange = (uint16_t)0;
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
	rpm
	 * offset 316
	 */
	int launchRpm = (int)0;
	/**
	deg
	 * offset 320
	 */
	int launchTimingRetard = (int)0;
	/**
	 * value '6' for 8MHz hw osc
	 * read hip9011 datasheet for details
	 * todo split into two bit fields
	integer
	 * offset 324
	 */
	int hip9011PrescalerAndSDO = (int)0;
	/**
	 * We calculate knock band based of cylinderBore
	 *  Use this to override - kHz knock band override
	kHz
	 * offset 328
	 */
	float knockBandCustom = (float)0;
	/**
	 * On Single Coil or Wasted Spark setups you have to lower dwell at high RPM
	RPM
	 * offset 332
	 */
	float sparkDwellRpmBins[DWELL_CURVE_SIZE];
	/**
	ms
	 * offset 364
	 */
	float sparkDwellValues[DWELL_CURVE_SIZE];
	/**
	 * offset 396
	 */
	specs_s specs;
	/**
	 * Cylinder diameter, in mm.
	mm
	 * offset 408
	 */
	float cylinderBore = (float)0;
	/**
	 * Disable sensor sniffer above this rpm
	RPM
	 * offset 412
	 */
	int sensorSnifferRpmThreshold = (int)0;
	/**
	 * set rpm_hard_limit X
	rpm
	 * offset 416
	 */
	int rpmHardLimit = (int)0;
	/**
	 * This setting controls which fuel quantity control algorithm is used.
	 * Alpha-N means drive by TPS
	 * Speed Density requires MAP sensor
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
	deg
	 * offset 432
	 */
	angle_t extraInjectionOffset = (angle_t)0;
	/**
	 * Ignition advance angle used during engine cranking, 5-10 degrees will work as a base setting for most engines.
	 * There is tapering towards running timing advance
	 * set cranking_timing_angle X
	deg
	 * offset 436
	 */
	angle_t crankingTimingAngle = (angle_t)0;
	/**
	 * Single coil = distributor
	 * Individual coils = one coil per cylinder (COP, coil-near-plug), requires sequential mode
	 * Wasted spark = Fires pairs of cylinders together, either one coil per pair of cylinders or one coil per cylinder
	 * Two distributors = A pair of distributors, found on some BMW, Toyota and other engines
	 * set ignition_mode X
	 * offset 440
	 */
	ignition_mode_e ignitionMode;
	/**
	count
	 * offset 444
	 */
	int8_t gapTrackingLengthOverride = (int8_t)0;
	/**
	 * Above this speed, disable closed loop idle control. Set to 0 to disable (allow closed loop idle at any speed).
	kph
	 * offset 445
	 */
	uint8_t maxIdleVss = (uint8_t)0;
	/**
	 * Expected oil pressure after starting the engine. If oil pressure does not reach this level within 5 seconds of engine start, fuel will be cut. Set to 0 to disable and always allow starting.
	kPa
	 * offset 446
	 */
	uint16_t minOilPressureAfterStart = (uint16_t)0;
	/**
	 * Dynamic uses the timing map to decide the ignition timing, Static timing fixes the timing to the value set below (only use for checking static timing with a timing light).
	 * offset 448
	 */
	timing_mode_e timingMode;
	/**
	 * This value is the ignition timing used when in 'fixed timing' mode, i.e. constant timing
	 * This mode is useful when adjusting distributor location.
	RPM
	 * offset 452
	 */
	angle_t fixedModeTiming = (angle_t)0;
	/**
	 * Angle between Top Dead Center (TDC) and the first trigger event.
	 * Positive value in case of synchnization point before TDC and negative in case of synchnization point after TDC
	 * .Knowing this angle allows us to control timing and other angles in reference to TDC.
	 * set global_trigger_offset_angle X
	deg btdc
	 * offset 456
	 */
	angle_t globalTriggerAngleOffset = (angle_t)0;
	/**
	 * Ratio/coefficient of input voltage dividers on your PCB. For example, use '2' if your board divides 5v into 2.5v. Use '1.66' if your board divides 5v into 3v.
	coef
	 * offset 460
	 */
	float analogInputDividerCoefficient = (float)0;
	/**
	 * This is the ratio of the resistors for the battery voltage, measure the voltage at the battery and then adjust this number until the gauge matches the reading.
	coef
	 * offset 464
	 */
	float vbattDividerCoeff = (float)0;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	deg C
	 * offset 468
	 */
	float fanOnTemperature = (float)0;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	deg C
	 * offset 472
	 */
	float fanOffTemperature = (float)0;
	/**
	 * This coefficient translates vehicle speed input frequency (in Hz) into vehicle speed, km/h
	coef
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
	ms
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
	index
	 * offset 500
	 */
	int byFirmwareVersion = (int)0;
	/**
	index
	 * offset 504
	 */
	int HD44780width = (int)0;
	/**
	index
	 * offset 508
	 */
	int HD44780height = (int)0;
	/**
	 * First throttle body, first sensor. See also pedalPositionAdcChannel
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
	 * set_analog_input_pin tps2 X
	 * offset 515
	 */
	adc_channel_e tps2_1AdcChannel;
	/**
	 * 0.1 is a good default value
	x
	 * offset 516
	 */
	float idle_derivativeFilterLoss = (float)0;
	/**
	 * just a temporary solution
	angle
	 * offset 520
	 */
	int trailingSparkAngle = (int)0;
	/**
	 * offset 524
	 */
	trigger_config_s trigger;
	/**
	 * offset 540
	 */
	spi_device_e hip9011SpiDevice;
	/**
	 * This value is only used for speed density fueling calculations.
	kPa
	 * offset 541
	 */
	uint8_t failedMapFallback = (uint8_t)0;
	/**
	 * Duty cycle to use in case of a sensor failure. This duty cycle should produce the minimum possible amount of boost.
	%
	 * offset 542
	 */
	uint8_t boostControlSafeDutyCycle = (uint8_t)0;
	/**
	 * offset 543
	 */
	adc_channel_e mafAdcChannel;
	/**
	 * set global_fuel_correction X
	coef
	 * offset 544
	 */
	float globalFuelCorrection = (float)0;
	/**
	volts
	 * offset 548
	 */
	float adcVcc = (float)0;
	/**
	 * maximum total number of degrees to subtract from ignition advance
	 * when knocking
	Deg
	 * offset 552
	 */
	float maxKnockSubDeg = (float)0;
	/**
	 * Camshaft input could be used either just for engine phase detection if your trigger shape does not include cam sensor as 'primary' channel, or it could be used for Variable Valve timing on one of the camshafts.
	 * offset 556
	 */
	brain_input_pin_e camInputs[CAM_INPUTS_COUNT];
	/**
	 * offset 560
	 */
	uint8_t camInputsPadding[CAM_INPUTS_COUNT_padding];
	/**
	 * offset 560
	 */
	afr_sensor_s afr;
	/**
	 * Electronic throttle pedal position first channel
	 * See throttlePedalPositionSecondAdcChannel for second channel
	 * See also tps1_1AdcChannel
	 * set_analog_input_pin pps X
	 * See throttlePedalUpVoltage and throttlePedalWOTVoltage
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
	idle_hardware_s idle;
	/**
	 * value between 0 and 100 used in Manual mode
	%
	 * offset 608
	 */
	float manIdlePosition = (float)0;
	/**
	Hz
	 * offset 612
	 */
	float mapFrequency0Kpa = (float)0;
	/**
	Hz
	 * offset 616
	 */
	float mapFrequency100Kpa = (float)0;
	/**
	 * Same RPM is used for two ways of producing simulated RPM. See also triggerSimulatorPins (with wires)
	 * See also directSelfStimulation (no wires, bypassing input hardware)
	 * rpm X
	 * TODO: rename to triggerSimulatorRpm
	Rpm
	 * offset 620
	 */
	int triggerSimulatorFrequency = (int)0;
	/**
	 * offset 624
	 */
	output_pin_e injectionPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 636
	 */
	output_pin_e ignitionPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 648
	 */
	pin_output_mode_e injectionPinMode;
	/**
	 * offset 649
	 */
	pin_output_mode_e ignitionPinMode;
	/**
	 * offset 650
	 */
	brain_pin_e HD44780_rs;
	/**
	 * offset 651
	 */
	brain_pin_e HD44780_e;
	/**
	 * offset 652
	 */
	brain_pin_e HD44780_db4;
	/**
	 * offset 653
	 */
	brain_pin_e HD44780_db5;
	/**
	 * offset 654
	 */
	brain_pin_e HD44780_db6;
	/**
	 * offset 655
	 */
	brain_pin_e HD44780_db7;
	/**
	 * offset 656
	 */
	brain_pin_e gps_rx_pin;
	/**
	 * offset 657
	 */
	brain_pin_e gps_tx_pin;
	/**
	 * offset 658
	 */
	output_pin_e fuelPumpPin;
	/**
	 * offset 659
	 */
	pin_output_mode_e fuelPumpPinMode;
	/**
	 * Check engine light, also malfunction indicator light. Always blinks once on boot.
	 * offset 660
	 */
	output_pin_e malfunctionIndicatorPin;
	/**
	 * offset 661
	 */
	pin_output_mode_e malfunctionIndicatorPinMode;
	/**
	 * offset 662
	 */
	pin_output_mode_e fanPinMode;
	/**
	 * offset 663
	 */
	output_pin_e fanPin;
	/**
	 * Some cars have a switch to indicate that clutch pedal is all the way down
	 * offset 664
	 */
	switch_input_pin_e clutchDownPin;
	/**
	 * offset 665
	 */
	output_pin_e alternatorControlPin;
	/**
	 * offset 666
	 */
	pin_output_mode_e alternatorControlPinMode;
	/**
	 * offset 667
	 */
	pin_input_mode_e clutchDownPinMode;
	/**
	 * offset 668
	 */
	brain_pin_e digitalPotentiometerChipSelect[DIGIPOT_COUNT];
	/**
	 * offset 672
	 */
	pin_output_mode_e electronicThrottlePin1Mode;
	/**
	 * set_cj125_heater_pin XXX
	 * offset 673
	 */
	brain_pin_e wboHeaterPin;
	/**
	 * set_cj125_cs_pin XXX
	 * offset 674
	 */
	brain_pin_e cj125CsPin;
	/**
	 * offset 675
	 */
	spi_device_e max31855spiDevice;
	/**
	 * offset 676
	 */
	brain_pin_e debugTriggerSync;
	/**
	 * Digital Potentiometer is used by stock ECU stimulation code
	 * offset 677
	 */
	spi_device_e digitalPotentiometerSpiDevice;
	/**
	 * offset 678
	 */
	brain_pin_e mc33972_cs;
	/**
	 * offset 679
	 */
	pin_output_mode_e mc33972_csPinMode;
	/**
	 * Useful in Research&Development phase
	 * offset 680
	 */
	adc_channel_e auxFastSensor1_adcChannel;
	/**
	 * First throttle body, second sensor.
	 * offset 681
	 */
	adc_channel_e tps1_2AdcChannel;
	/**
	 * Second throttle body, second sensor.
	 * offset 682
	 */
	adc_channel_e tps2_2AdcChannel;
	/**
	 * Electronic throttle pedal position input
	 * Second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalSecondaryUpVoltage and throttlePedalSecondaryWOTVoltage
	 * offset 683
	 */
	adc_channel_e throttlePedalPositionSecondAdcChannel;
	/**
	%
	 * offset 684
	 */
	uint8_t fuelLevelValues[FUEL_LEVEL_TABLE_COUNT];
	/**
	 * AFR, WBO, EGO - whatever you like to call it
	 * offset 692
	 */
	ego_sensor_e afr_type;
	/**
	 * 0.1 is a good default value
	x
	 * offset 696
	 */
	float idle_antiwindupFreq = (float)0;
	/**
	 * offset 700
	 */
	brain_input_pin_e triggerInputPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * offset 703
	 */
	pin_output_mode_e hip9011CsPinMode;
	/**
	 * offset 704
	 */
	output_pin_e tachOutputPin;
	/**
	 * offset 705
	 */
	pin_output_mode_e tachOutputPinMode;
	/**
	 * offset 706
	 */
	output_pin_e mainRelayPin;
	/**
	 * offset 707
	 */
	brain_pin_e sdCardCsPin;
	/**
	 * set_can_tx_pin X
	 * offset 708
	 */
	brain_pin_e canTxPin;
	/**
	 * set_can_rx_pin X
	 * offset 709
	 */
	brain_pin_e canRxPin;
	/**
	 * offset 710
	 */
	pin_input_mode_e throttlePedalUpPinMode;
	/**
	 * Additional idle % while A/C is active
	%
	 * offset 711
	 */
	uint8_t acIdleExtraOffset = (uint8_t)0;
	/**
	 * CANbus thread period, ms
	ms
	 * offset 712
	 */
	int can2SleepPeriodMs = (int)0;
	/**
	 * Voltage when the wastegate is closed.
	 * You probably don't have one of these!
	mv
	 * offset 716
	 */
	uint16_t wastegatePositionMin = (uint16_t)0;
	/**
	 * Voltage when the wastegate is fully open.
	 * You probably don't have one of these!
	 * 1 volt = 1000 units
	mv
	 * offset 718
	 */
	uint16_t wastegatePositionMax = (uint16_t)0;
	/**
	 * Voltage when the idle valve is closed.
	 * You probably don't have one of these!
	mv
	 * offset 720
	 */
	uint16_t idlePositionMin = (uint16_t)0;
	/**
	 * Voltage when the idle valve is open.
	 * You probably don't have one of these!
	 * 1 volt = 1000 units
	mv
	 * offset 722
	 */
	uint16_t idlePositionMax = (uint16_t)0;
	/**
	units
	 * offset 724
	 */
	uint16_t tempHpfpStart = (uint16_t)0;
	/**
	units
	 * offset 726
	 */
	uint16_t tempHpfpDuration = (uint16_t)0;
	/**
	 * Secondary TTL channel baud rate
	BPs
	 * offset 728
	 */
	uint32_t tunerStudioSerialSpeed = (uint32_t)0;
	/**
	 * Just for reference really, not taken into account by any logic at this point
	CR
	 * offset 732
	 */
	float compressionRatio = (float)0;
	/**
	 * Each rusEfi piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEfi board.
	 * See also directSelfStimulation which is different.
	 * offset 736
	 */
	brain_pin_e triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * offset 739
	 */
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * Narrow band o2 heater, not used for CJ125. 'ON' if engine is running, 'OFF' if stopped or cranking. See wboHeaterPin
	 * offset 742
	 */
	output_pin_e o2heaterPin;
	/**
	 * offset 743
	 */
	pin_output_mode_e o2heaterPinModeTodO;
	/**
	offset 744 bit 0 */
	bool is_enabled_spi_1 : 1;
	/**
	offset 744 bit 1 */
	bool is_enabled_spi_2 : 1;
	/**
	offset 744 bit 2 */
	bool is_enabled_spi_3 : 1;
	/**
	 * enable sd/disable sd
	offset 744 bit 3 */
	bool isSdCardEnabled : 1;
	/**
	offset 744 bit 4 */
	bool unused744b4 : 1;
	/**
	offset 744 bit 5 */
	bool isEngineControlEnabled : 1;
	/**
	offset 744 bit 6 */
	bool isHip9011Enabled : 1;
	/**
	offset 744 bit 7 */
	bool isVerboseAlternator : 1;
	/**
	offset 744 bit 8 */
	bool useSerialPort : 1;
	/**
	 * This setting should only be used if you have a stepper motor idle valve and a stepper motor driver installed.
	offset 744 bit 9 */
	bool useStepperIdle : 1;
	/**
	offset 744 bit 10 */
	bool enabledStep1Limiter : 1;
	/**
	offset 744 bit 11 */
	bool useTpicAdvancedMode : 1;
	/**
	offset 744 bit 12 */
	bool useLcdScreen : 1;
	/**
	offset 744 bit 13 */
	bool verboseTLE8888 : 1;
	/**
	 * CAN broadcast using custom rusEFI protocol
	 * enable can_broadcast/disable can_broadcast
	offset 744 bit 14 */
	bool enableVerboseCanTx : 1;
	/**
	 * This will cause the alternator to be operated in a basic on or off mode, this is the simplest alternator control.
	offset 744 bit 15 */
	bool onOffAlternatorLogic : 1;
	/**
	 * enable cj125/disable cj125
	offset 744 bit 16 */
	bool isCJ125Enabled : 1;
	/**
	 * Use rise or fall signal front
	 * get vvtCamSensorUseRise
	offset 744 bit 17 */
	bool vvtCamSensorUseRise : 1;
	/**
	 * Useful for individual intakes
	offset 744 bit 18 */
	bool measureMapOnlyInOneCylinder : 1;
	/**
	offset 744 bit 19 */
	bool stepperForceParkingEveryRestart : 1;
	/**
	 * If enabled, try to fire the engine before a full engine cycle has been completed using RPM estimated from the last 90 degrees of engine rotation. As soon as the trigger syncs plus 90 degrees rotation, fuel and ignition events will occur. If disabled, worst case may require up to 4 full crank rotations before any events are scheduled.
	offset 744 bit 20 */
	bool isFasterEngineSpinUpEnabled : 1;
	/**
	 * This setting disables fuel injection while the engine is in overrun, this is useful as a fuel saving measure and to prevent back firing.
	offset 744 bit 21 */
	bool coastingFuelCutEnabled : 1;
	/**
	 * This setting allows the ECU to open the IAC during overrun conditions to help reduce engine breaking, this can be helpful for large engines in light weight cars. Used in Auto-PID Idle mode.
	offset 744 bit 22 */
	bool useIacTableForCoasting : 1;
	/**
	offset 744 bit 23 */
	bool useNoiselessTriggerDecoder : 1;
	/**
	offset 744 bit 24 */
	bool useIdleTimingPidControl : 1;
	/**
	 * Allows disabling the ETB when the engine is stopped. You may not like the power draw or PWM noise from the motor, so this lets you turn it off until it's necessary.
	offset 744 bit 25 */
	bool disableEtbWhenEngineStopped : 1;
	/**
	offset 744 bit 26 */
	bool is_enabled_spi_4 : 1;
	/**
	 * Disable the electronic throttle motor and DC idle motor for testing.
	 * This mode is for testing ETB/DC idle position sensors, etc without actually driving the throttle.
	offset 744 bit 27 */
	bool pauseEtbControl : 1;
	/**
	offset 744 bit 28 */
	bool alignEngineSnifferAtTDC : 1;
	/**
	 * This setting allows the ETB to act as the idle air control valve and move to regulate the airflow at idle.
	offset 744 bit 29 */
	bool useETBforIdleControl : 1;
	/**
	offset 744 bit 30 */
	bool idleIncrementalPidCic : 1;
	/**
	 * AEM X-Series or rusEFI Wideband
	offset 744 bit 31 */
	bool enableAemXSeries : 1;
	/**
	 * offset 748
	 */
	brain_input_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * offset 752
	 */
	pin_output_mode_e mainRelayPinMode;
	/**
	 * offset 753
	 */
	brain_pin_e hip9011CsPin;
	/**
	 * offset 754
	 */
	brain_pin_e hip9011IntHoldPin;
	/**
	 * offset 755
	 */
	pin_output_mode_e hip9011IntHoldPinMode;
	/**
	 * offset 756
	 */
	uint32_t verboseCanBaseAddress = (uint32_t)0;
	/**
	v
	 * offset 760
	 */
	uint8_t mc33_hvolt = (uint8_t)0;
	/**
	 * Additional idle PID minValue while A/C is active
	Percent
	 * offset 761
	 */
	uint8_t acIdleExtraMin = (uint8_t)0;
	/**
	 * Optional Radiator Fan used with A/C
	 * offset 762
	 */
	output_pin_e acFanPin;
	/**
	 * offset 763
	 */
	pin_output_mode_e acFanPinMode;
	/**
	 * offset 764
	 */
	pin_output_mode_e gpioPinModes[FSIO_COMMAND_COUNT];
	/**
	 * todo: more comments
	 * offset 780
	 */
	output_pin_e fsioOutputPins[FSIO_COMMAND_COUNT];
	/**
	 * offset 796
	 */
	brain_pin_e max31855_cs[EGT_CHANNEL_COUNT];
	/**
	 * SD card logging period, in milliseconds
	ms
	 * offset 804
	 */
	int16_t sdCardPeriodMs = (int16_t)0;
	/**
	 * offset 806
	 */
	adc_channel_e idlePositionSensor;
	/**
	 * offset 807
	 */
	brain_pin_e debugMapAveraging;
	/**
	 * offset 808
	 */
	output_pin_e starterRelayDisablePin;
	/**
	 * On some vehicles we can disable starter once engine is already running
	 * offset 809
	 */
	pin_output_mode_e starterRelayDisablePinMode;
	/**
	 * Some Subaru and some Mazda use double-solenoid idle air valve
	 * offset 810
	 */
	output_pin_e secondSolenoidPin;
	/**
	 * See also starterControlPin
	 * offset 811
	 */
	switch_input_pin_e startStopButtonPin;
	/**
	count
	 * offset 812
	 */
	int mapMinBufferLength = (int)0;
	/**
	 * Below this throttle position, the engine is considered idling.
	%
	 * offset 816
	 */
	int16_t idlePidDeactivationTpsThreshold = (int16_t)0;
	/**
	%
	 * offset 818
	 */
	int16_t stepperParkingExtraSteps = (int16_t)0;
	/**
	ADC
	 * offset 820
	 */
	uint16_t tps1SecondaryMin = (uint16_t)0;
	/**
	ADC
	 * offset 822
	 */
	uint16_t tps1SecondaryMax = (uint16_t)0;
	/**
	rpm
	 * offset 824
	 */
	int16_t antiLagRpmTreshold = (int16_t)0;
	/**
	 * Maximum time to crank starter when start/stop button is pressed
	Seconds
	 * offset 826
	 */
	uint16_t startCrankingDuration = (uint16_t)0;
	/**
	 * This pin is used for debugging - snap a logic analyzer on it and see if it's ever high
	 * offset 828
	 */
	brain_pin_e triggerErrorPin;
	/**
	 * offset 829
	 */
	pin_output_mode_e triggerErrorPinMode;
	/**
	 * offset 830
	 */
	output_pin_e acRelayPin;
	/**
	 * offset 831
	 */
	pin_output_mode_e acRelayPinMode;
	/**
	 * offset 832
	 */
	fsio_pwm_freq_t fsioFrequency[FSIO_COMMAND_COUNT];
	/**
	 * offset 864
	 */
	fsio_setting_t fsio_setting[FSIO_COMMAND_COUNT];
	/**
	 * offset 928
	 */
	brain_pin_e spi1mosiPin;
	/**
	 * offset 929
	 */
	brain_pin_e spi1misoPin;
	/**
	 * offset 930
	 */
	brain_pin_e spi1sckPin;
	/**
	 * offset 931
	 */
	brain_pin_e spi2mosiPin;
	/**
	 * offset 932
	 */
	brain_pin_e spi2misoPin;
	/**
	 * offset 933
	 */
	brain_pin_e spi2sckPin;
	/**
	 * offset 934
	 */
	brain_pin_e spi3mosiPin;
	/**
	 * offset 935
	 */
	brain_pin_e spi3misoPin;
	/**
	 * offset 936
	 */
	brain_pin_e spi3sckPin;
	/**
	 * Saab Combustion Detection Module knock signal input pin
	 * also known as Saab Ion Sensing Module
	 * offset 937
	 */
	brain_pin_e cdmInputPin;
	/**
	 * offset 938
	 */
	brain_pin_e joystickCenterPin;
	/**
	 * offset 939
	 */
	brain_pin_e joystickAPin;
	/**
	 * offset 940
	 */
	brain_pin_e joystickBPin;
	/**
	 * offset 941
	 */
	brain_pin_e joystickCPin;
	/**
	 * offset 942
	 */
	brain_pin_e joystickDPin;
	/**
	 * offset 943
	 */
	uart_device_e consoleUartDevice;
	/**
	 * rusEfi console Sensor Sniffer mode
	 * offset 944
	 */
	sensor_chart_e sensorChartMode;
	/**
	 * offset 948
	 */
	maf_sensor_type_e mafSensorType;
	/**
	 * todo:not finished
	 * These input pins allow us to pull toggle buttons state
	 * offset 952
	 */
	brain_pin_e fsioDigitalInputs[FSIO_COMMAND_COUNT];
	/**
	 * offset 968
	 */
	brain_input_pin_e vehicleSpeedSensorInputPin;
	/**
	 * Some vehicles have a switch to indicate that clutch pedal is all the way up
	 * offset 969
	 */
	switch_input_pin_e clutchUpPin;
	/**
	 * offset 970
	 */
	brain_input_pin_e frequencyReportingMapInputPin;
	/**
	 * offset 971
	 */
	pin_input_mode_e clutchUpPinMode;
	/**
	 * Disable multispark above this engine speed.
	rpm
	 * offset 972
	 */
	uint16_t multisparkMaxRpm = (uint16_t)0;
	/**
	 * This parameter sets the latest that the last multispark can occur after the main ignition event. For example, if the ignition timing is 30 degrees BTDC, and this parameter is set to 45, no multispark will ever be fired after 15 degrees ATDC.
	deg
	 * offset 974
	 */
	uint8_t multisparkMaxSparkingAngle = (uint8_t)0;
	/**
	 * Configures the maximum number of extra sparks to fire (does not include main spark)
	count
	 * offset 975
	 */
	uint8_t multisparkMaxExtraSparkCount = (uint8_t)0;
	/**
	offset 976 bit 0 */
	bool clutchUpPinInverted : 1;
	/**
	offset 976 bit 1 */
	bool clutchDownPinInverted : 1;
	/**
	 * If enabled we use two H-bridges to drive stepper idle air valve
	offset 976 bit 2 */
	bool useHbridges : 1;
	/**
	offset 976 bit 3 */
	bool multisparkEnable : 1;
	/**
	offset 976 bit 4 */
	bool enableLaunchRetard : 1;
	/**
	offset 976 bit 5 */
	bool enableLaunchBoost : 1;
	/**
	offset 976 bit 6 */
	bool launchDisableBySpeed : 1;
	/**
	offset 976 bit 7 */
	bool enableCanVss : 1;
	/**
	offset 976 bit 8 */
	bool enableInnovateLC2 : 1;
	/**
	offset 976 bit 9 */
	bool showHumanReadableWarning : 1;
	/**
	 * If enabled, adjust at a constant rate instead of a rate proportional to the current lambda error. This mode may be easier to tune, and more tolerant of sensor noise. Use of this mode is required if you have a narrowband O2 sensor.
	offset 976 bit 10 */
	bool stftIgnoreErrorMagnitude : 1;
	/**
	offset 976 bit 11 */
	bool unused976b11 : 1;
	/**
	offset 976 bit 12 */
	bool enableSoftwareKnock : 1;
	/**
	 * enable vvt_details
	offset 976 bit 13 */
	bool verboseVVTDecoding : 1;
	/**
	 * get invertCamVVTSignal
	offset 976 bit 14 */
	bool invertCamVVTSignal : 1;
	/**
	 * This property is useful if using rusEFI as TCM or BCM only
	 * enable consumeObdSensors
	offset 976 bit 15 */
	bool consumeObdSensors : 1;
	/**
	offset 976 bit 16 */
	bool knockBankCyl1 : 1;
	/**
	offset 976 bit 17 */
	bool knockBankCyl2 : 1;
	/**
	offset 976 bit 18 */
	bool knockBankCyl3 : 1;
	/**
	offset 976 bit 19 */
	bool knockBankCyl4 : 1;
	/**
	offset 976 bit 20 */
	bool knockBankCyl5 : 1;
	/**
	offset 976 bit 21 */
	bool knockBankCyl6 : 1;
	/**
	offset 976 bit 22 */
	bool knockBankCyl7 : 1;
	/**
	offset 976 bit 23 */
	bool knockBankCyl8 : 1;
	/**
	offset 976 bit 24 */
	bool knockBankCyl9 : 1;
	/**
	offset 976 bit 25 */
	bool knockBankCyl10 : 1;
	/**
	offset 976 bit 26 */
	bool knockBankCyl11 : 1;
	/**
	offset 976 bit 27 */
	bool knockBankCyl12 : 1;
	/**
	offset 976 bit 28 */
	bool tcuEnabled : 1;
	/**
	offset 976 bit 29 */
	bool unusedBit_251_29 : 1;
	/**
	offset 976 bit 30 */
	bool unusedBit_291_30 : 1;
	/**
	offset 976 bit 31 */
	bool unusedBit_291_31 : 1;
	/**
	 * offset 980
	 */
	dc_io etbIo[ETB_COUNT];
	/**
	 * Wastegate control Solenoid
	 * offset 988
	 */
	output_pin_e boostControlPin;
	/**
	 * offset 989
	 */
	pin_output_mode_e boostControlPinMode;
	/**
	 * offset 990
	 */
	switch_input_pin_e antiLagActivatePin;
	/**
	 * offset 991
	 */
	switch_input_pin_e launchActivatePin;
	/**
	 * offset 992
	 */
	pid_s boostPid;
	/**
	 * offset 1012
	 */
	boostType_e boostType;
	/**
	Hz
	 * offset 1016
	 */
	int boostPwmFrequency = (int)0;
	/**
	 * offset 1020
	 */
	launchActivationMode_e launchActivationMode;
	/**
	 * offset 1024
	 */
	antiLagActivationMode_e antiLagActivationMode;
	/**
	 * Disabled above this speed
	Kph
	 * offset 1028
	 */
	int launchSpeedTreshold = (int)0;
	/**
	 * Disabled below this rpm
	RPM
	 * offset 1032
	 */
	int launchRpmTreshold = (int)0;
	/**
	 * Range from Launch Rpm for Timing Retard to activate
	RPM
	 * offset 1036
	 */
	int launchTimingRpmRange = (int)0;
	/**
	 * Extra Fuel Added
	%
	 * offset 1040
	 */
	int launchFuelAdded = (int)0;
	/**
	 * Duty Cycle for the Boost Solenoid
	%
	 * offset 1044
	 */
	int launchBoostDuty = (int)0;
	/**
	 * RPM Range for Hard Cut
	rpm
	 * offset 1048
	 */
	int hardCutRpmRange = (int)0;
	/**
	rpm
	 * offset 1052
	 */
	int launchAdvanceRpmRange = (int)0;
	/**
	rpm
	 * offset 1056
	 */
	int launchTpsTreshold = (int)0;
	/**
	rpm
	 * offset 1060
	 */
	float launchActivateDelay = (float)0;
	/**
	 * offset 1064
	 */
	stft_s stft;
	/**
	 * offset 1088
	 */
	dc_io stepperDcIo[DC_PER_STEPPER];
	/**
	 * For example, BMW, GM or Chevrolet
	 * REQUIRED for rusEFI Online
	 * offset 1096
	 */
	vehicle_info_t engineMake;
	/**
	 * For example, LS1 or NB2
	 * REQUIRED for rusEFI Online
	 * offset 1128
	 */
	vehicle_info_t engineCode;
	/**
	 * For example, Hunchback or Orange Miata
	 * Vehicle name has to be unique between your vehicles.
	 * REQUIRED for rusEFI Online
	 * offset 1160
	 */
	vehicle_info_t vehicleName;
	/**
	 * offset 1192
	 */
	output_pin_e tcu_solenoid[TCU_SOLENOID_COUNT];
	/**
	 * offset 1198
	 */
	etb_function_e etbFunctions[ETB_COUNT];
	/**
	 * offset 1200
	 */
	spi_device_e drv8860spiDevice;
	/**
	 * offset 1201
	 */
	brain_pin_e drv8860_cs;
	/**
	 * offset 1202
	 */
	pin_output_mode_e drv8860_csPinMode;
	/**
	 * offset 1203
	 */
	brain_pin_e drv8860_miso;
	/**
	volt
	 * offset 1204
	 */
	uint16_t fuelLevelBins[FUEL_LEVEL_TABLE_COUNT];
	/**
	 * offset 1220
	 */
	output_pin_e luaOutputPins[LUA_PWM_COUNT];
	/**
	 * Angle between cam sensor and VVT zero position
	 * set vvt_offset X
	value
	 * offset 1228
	 */
	float vvtOffsets[CAM_INPUTS_COUNT];
	/**
	 * offset 1244
	 */
	float vvtOffsetsPadding[CAM_INPUTS_COUNT_padding];
	/**
	 * offset 1244
	 */
	vr_threshold_s vrThreshold[2];
	/**
	units
	 * offset 1276
	 */
	int unusedAtOldBoardConfigurationEnd[45];
	/**
	kg
	 * offset 1456
	 */
	uint16_t vehicleWeight = (uint16_t)0;
	/**
	 * offset 1458
	 */
	brain_pin_e lps25BaroSensorScl;
	/**
	 * offset 1459
	 */
	brain_pin_e lps25BaroSensorSda;
	/**
	ADC
	 * offset 1460
	 */
	uint16_t tps2SecondaryMin = (uint16_t)0;
	/**
	ADC
	 * offset 1462
	 */
	uint16_t tps2SecondaryMax = (uint16_t)0;
	/**
	offset 1464 bit 0 */
	bool unused1464b0 : 1;
	/**
	 * Enables lambda sensor closed loop feedback for fuelling.
	offset 1464 bit 1 */
	bool fuelClosedLoopCorrectionEnabled : 1;
	/**
	 * Print details into rusEfi console
	 * enable verbose_idle
	offset 1464 bit 2 */
	bool isVerboseIAC : 1;
	/**
	 * Prints ETB details to rusEFI console
	offset 1464 bit 3 */
	bool isVerboseETB : 1;
	/**
	offset 1464 bit 4 */
	bool unused1464b4 : 1;
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
	 * enable can_read/disable can_read
	offset 1464 bit 8 */
	bool canReadEnabled : 1;
	/**
	 * enable can_write/disable can_write
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
	 * Treat milliseconds value as duty cycle value, i.e. 0.5ms would become 50%
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
	bool launchFuelCutEnable : 1;
	/**
	 * This is the Cut Mode normally used
	offset 1464 bit 19 */
	bool launchSparkCutEnable : 1;
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
	 * A/C button input
	 * offset 1469
	 */
	switch_input_pin_e acSwitch;
	/**
	 * offset 1470
	 */
	adc_channel_e vRefAdcChannel;
	/**
	 * Expected neutral position
	%
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
	bool unused1476b3 : 1;
	/**
	offset 1476 bit 4 */
	bool unusedBit4_1476 : 1;
	/**
	offset 1476 bit 5 */
	bool isMapAveragingEnabled : 1;
	/**
	 * If enabled, use separate temperature multiplier table for cranking idle position.
	 * If disabled, use normal running multiplier table applied to the cranking base position.
	offset 1476 bit 6 */
	bool overrideCrankingIacSetting : 1;
	/**
	 * This activates a separate ignition timing table for idle conditions, this can help idle stability by using ignition retard and advance either side of the desired idle speed. Extra retard at low idle speeds will prevent stalling and extra advance at high idle speeds can help reduce engine power and slow the idle speed.
	offset 1476 bit 7 */
	bool useSeparateAdvanceForIdle : 1;
	/**
	offset 1476 bit 8 */
	bool unused1476b8 : 1;
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
	 * This is needed if your coils are individually wired (COP) and you wish to use batch ignition (Wasted Spark).
	offset 1476 bit 15 */
	bool twoWireBatchIgnition : 1;
	/**
	offset 1476 bit 16 */
	bool useFixedBaroCorrFromMap : 1;
	/**
	 * In Constant mode, timing is automatically tapered to running as RPM increases.
	 * In Table mode, the "Cranking ignition advance" table is used directly.
	offset 1476 bit 17 */
	bool useSeparateAdvanceForCranking : 1;
	/**
	 * This enables the various ignition corrections during cranking (IAT, CLT, FSIO and PID idle).
	 * You probably don't need this.
	offset 1476 bit 18 */
	bool useAdvanceCorrectionsForCranking : 1;
	/**
	offset 1476 bit 19 */
	bool unused1476b19 : 1;
	/**
	offset 1476 bit 20 */
	bool unused1476b20 : 1;
	/**
	 * This flag allows to use a special 'PID Multiplier' table (0.0-1.0) to compensate for nonlinear nature of IAC-RPM controller
	offset 1476 bit 21 */
	bool useIacPidMultTable : 1;
	/**
	offset 1476 bit 22 */
	bool isBoostControlEnabled : 1;
	/**
	 * Interpolates the Ignition Retard from 0 to 100% within the RPM Range
	offset 1476 bit 23 */
	bool launchSmoothRetard : 1;
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
	count
	 * offset 1480
	 */
	uint32_t engineChartSize = (uint32_t)0;
	/**
	 * How far above idle speed do we consider idling?
	 * For example, if target = 800, this param = 200, then anything below 1000 RPM is considered idle.
	RPM
	 * offset 1484
	 */
	int16_t idlePidRpmUpperLimit = (int16_t)0;
	/**
	 * This sets the temperature above which no priming pulse is used, The value at -40 is reduced until there is no more priming injection at this temperature.
	*C
	 * offset 1486
	 */
	int16_t primeInjFalloffTemperature = (int16_t)0;
	/**
	mult
	 * offset 1488
	 */
	float turboSpeedSensorMultiplier = (float)0;
	/**
	 * offset 1492
	 */
	brain_pin_e camInputsDebug[CAM_INPUTS_COUNT];
	/**
	 * offset 1496
	 */
	uint8_t camInputsDebugPadding[CAM_INPUTS_COUNT_padding];
	/**
	 * Extra idle target speed when A/C is enabled. Some cars need the extra speed to keep the AC efficient while idling.
	RPM
	 * offset 1496
	 */
	int16_t acIdleRpmBump = (int16_t)0;
	/**
	 * set warningPeriod X
	seconds
	 * offset 1498
	 */
	int16_t warningPeriod = (int16_t)0;
	/**
	angle
	 * offset 1500
	 */
	float knockDetectionWindowStart = (float)0;
	/**
	angle
	 * offset 1504
	 */
	float knockDetectionWindowEnd = (float)0;
	/**
	ms
	 * offset 1508
	 */
	float idleStepperReactionTime = (float)0;
	/**
	V
	 * offset 1512
	 */
	float knockVThreshold = (float)0;
	/**
	 * offset 1516
	 */
	pin_input_mode_e fsioInputModes[FSIO_COMMAND_COUNT];
	/**
	count
	 * offset 1532
	 */
	int idleStepperTotalSteps = (int)0;
	/**
	 * TODO: finish this #413
	sec
	 * offset 1536
	 */
	float noAccelAfterHardLimitPeriodSecs = (float)0;
	/**
	 * At what trigger index should some MAP-related math be executed? This is a performance trick to reduce load on synchronization trigger callback.
	index
	 * offset 1540
	 */
	int mapAveragingSchedulingAtIndex = (int)0;
	/**
	kPa
	 * offset 1544
	 */
	float baroCorrPressureBins[BARO_CORR_SIZE];
	/**
	RPM
	 * offset 1560
	 */
	float baroCorrRpmBins[BARO_CORR_SIZE];
	/**
	 * offset 1576
	 */
	baro_corr_table_t baroCorrTable;
	/**
	 * Cranking fuel correction coefficient based on TPS
	Ratio
	 * offset 1640
	 */
	float crankingTpsCoef[CRANKING_CURVE_SIZE];
	/**
	%
	 * offset 1672
	 */
	float crankingTpsBins[CRANKING_CURVE_SIZE];
	/**
	 * Duration in ms or duty cycle depending on selected mode
	 * offset 1704
	 */
	float tachPulseDuractionMs = (float)0;
	/**
	 * Above this RPM, disable AC. Set to 0 to disable check.
	rpm
	 * offset 1708
	 */
	uint16_t maxAcRpm = (uint16_t)0;
	/**
	 * Above this TPS, disable AC. Set to 0 to disable check.
	%
	 * offset 1710
	 */
	uint8_t maxAcTps = (uint8_t)0;
	/**
	 * Above this CLT, disable AC to prevent overheating the engine. Set to 0 to disable check.
	deg C
	 * offset 1711
	 */
	uint8_t maxAcClt = (uint8_t)0;
	/**
	 * Length of time the deposited wall fuel takes to dissipate after the start of acceleration. 
	Seconds
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
	brain_pin_e triggerInputDebugPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * offset 1759
	 */
	brain_input_pin_e turboSpeedSensorInputPin;
	/**
	x
	 * offset 1760
	 */
	float alternator_derivativeFilterLoss = (float)0;
	/**
	x
	 * offset 1764
	 */
	float alternator_antiwindupFreq = (float)0;
	/**
	 * Closed throttle#2. todo: extract these two fields into a structure
	 * See also tps2_1AdcChannel
	 * set tps2_min X
	ADC
	 * offset 1768
	 */
	int16_t tps2Min = (int16_t)0;
	/**
	 * Full throttle#2. tpsMax value as 10 bit ADC value. Not Voltage!
	 * See also tps1_1AdcChannel
	 * set tps2_max X
	ADC
	 * offset 1770
	 */
	int16_t tps2Max = (int16_t)0;
	/**
	 * See also startStopButtonPin
	 * offset 1772
	 */
	output_pin_e starterControlPin;
	/**
	 * offset 1773
	 */
	pin_input_mode_e startStopButtonMode;
	/**
	 * offset 1774
	 */
	brain_pin_e mc33816_flag0;
	/**
	Pulse
	 * offset 1775
	 */
	uint8_t tachPulsePerRev = (uint8_t)0;
	/**
	 * kPa value which is too low to be true
	kPa
	 * offset 1776
	 */
	float mapErrorDetectionTooLow = (float)0;
	/**
	 * kPa value which is too high to be true
	kPa
	 * offset 1780
	 */
	float mapErrorDetectionTooHigh = (float)0;
	/**
	 * How long to wait for the spark to fire before recharging the coil for another spark.
	ms
	 * offset 1784
	 */
	uint16_t multisparkSparkDuration = (uint16_t)0;
	/**
	 * This sets the dwell time for subsequent sparks. The main spark's dwell is set by the dwell table.
	ms
	 * offset 1786
	 */
	uint16_t multisparkDwell = (uint16_t)0;
	/**
	 * See cltIdleRpmBins
	 * offset 1788
	 */
	pid_s idleRpmPid;
	/**
	 * 0 = No fuel settling on port walls 1 = All the fuel settling on port walls setting this to 0 disables the wall wetting enrichment. 
	Fraction
	 * offset 1808
	 */
	float wwaeBeta = (float)0;
	/**
	 * blue LED on many rusEFI boards.
	 * Blue Communication LED which is expected to blink at 50% duty cycle during normal board operation.
	 * If USB communication cable is connected Blue LED starts to blink faster.
	 * offset 1812
	 */
	brain_pin_e communicationLedPin;
	/**
	 * Green LED on many rusEFI boards.
	 * Off if engine is stopped, blinks if engine is cranking, solid if engine is running.
	 * offset 1813
	 */
	brain_pin_e runningLedPin;
	/**
	 * See also EFI_CONSOLE_RX_BRAIN_PIN
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
	 * offset 1818
	 */
	switch_input_pin_e tcuUpshiftButtonPin;
	/**
	 * offset 1819
	 */
	switch_input_pin_e tcuDownshiftButtonPin;
	/**
	 * Knock sensor output knock detection threshold depending on current RPM
	v
	 * offset 1820
	 */
	float knockNoise[ENGINE_NOISE_CURVE_SIZE];
	/**
	RPM
	 * offset 1852
	 */
	float knockNoiseRpmBins[ENGINE_NOISE_CURVE_SIZE];
	/**
	voltage
	 * offset 1884
	 */
	float throttlePedalUpVoltage = (float)0;
	/**
	 * Pedal in the floor
	voltage
	 * offset 1888
	 */
	float throttlePedalWOTVoltage = (float)0;
	/**
	 * on ECU start turn fuel pump on to build fuel pressure
	seconds
	 * offset 1892
	 */
	int16_t startUpFuelPumpDuration = (int16_t)0;
	/**
	 * If the RPM closer to target than this value, disable closed loop idle correction to prevent oscillation
	RPM
	 * offset 1894
	 */
	int16_t idlePidRpmDeadZone = (int16_t)0;
	/**
	 * CLT-based target RPM for automatic idle controller
	C
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
	Volts
	 * offset 2024
	 */
	float targetVBatt = (float)0;
	/**
	 * Turns off alternator output above specified TPS, enabling this reduced parasitic drag on the engine at full load.
	%
	 * offset 2028
	 */
	float alternatorOffAboveTps = (float)0;
	/**
	 * Prime pulse for cold engine, duration in ms
	 * Linear interpolation between -40F/-40C and fallout temperature
	 * 
	 * See also isFasterEngineSpinUpEnabled
	 * set cranking_priming_pulse X
	ms
	 * offset 2032
	 */
	float startOfCrankingPrimingPulse = (float)0;
	/**
	 * This is the duration in cycles that the IAC will take to reach its normal idle position, it can be used to hold the idle higher for a few seconds after cranking to improve startup.
	cycles
	 * offset 2036
	 */
	int16_t afterCrankingIACtaperDuration = (int16_t)0;
	/**
	 * Extra IAC, in percent between 0 and 100, tapered between zero and idle deactivation TPS value
	percent
	 * offset 2038
	 */
	int16_t iacByTpsTaper = (int16_t)0;
	/**
	 * set_aux_tx_pin X
	 * offset 2040
	 */
	brain_pin_e auxSerialTxPin;
	/**
	 * offset 2041
	 */
	brain_pin_e warningLedPin;
	/**
	 * set_aux_rx_pin X
	 * offset 2042
	 */
	brain_pin_e auxSerialRxPin;
	/**
	 * offset 2043
	 */
	brain_pin_e LIS302DLCsPin;
	/**
	 * This is the number of engine cycles that the TPS position change can occur over, a longer duration will make the enrichment more active but too long may affect steady state driving, a good default is 30-60 cycles. 
	cycles
	 * offset 2044
	 */
	int tpsAccelLength = (int)0;
	/**
	 * Maximum change delta of TPS percentage over the 'length'. Actual TPS change has to be above this value in order for TPS/TPS acceleration to kick in.
	roc
	 * offset 2048
	 */
	float tpsAccelEnrichmentThreshold = (float)0;
	/**
	v
	 * offset 2052
	 */
	float unusedVvtOffsetWasHere = (float)0;
	/**
	cycles
	 * offset 2056
	 */
	int engineLoadAccelLength = (int)0;
	/**
	roc
	 * offset 2060
	 */
	float engineLoadDecelEnleanmentThreshold = (float)0;
	/**
	coeff
	 * offset 2064
	 */
	float engineLoadDecelEnleanmentMultiplier = (float)0;
	/**
	roc
	 * offset 2068
	 */
	float engineLoadAccelEnrichmentThreshold = (float)0;
	/**
	coeff
	 * offset 2072
	 */
	float engineLoadAccelEnrichmentMultiplier = (float)0;
	/**
	 * Band rate for primary TTL
	BPs
	 * offset 2076
	 */
	uint32_t uartConsoleSerialSpeed = (uint32_t)0;
	/**
	roc
	 * offset 2080
	 */
	float tpsDecelEnleanmentThreshold = (float)0;
	/**
	coeff
	 * offset 2084
	 */
	float tpsDecelEnleanmentMultiplier = (float)0;
	/**
	 * ExpAverage alpha coefficient
	coeff
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
	BPs
	 * offset 2096
	 */
	uint32_t auxSerialSpeed = (uint32_t)0;
	/**
	voltage
	 * offset 2100
	 */
	float throttlePedalSecondaryUpVoltage = (float)0;
	/**
	 * Pedal in the floor
	voltage
	 * offset 2104
	 */
	float throttlePedalSecondaryWOTVoltage = (float)0;
	/**
	 *  set can_baudrate
	 * offset 2108
	 */
	can_baudrate_e canBaudRate;
	/**
	 * Override the Y axis (load) value used for the VE table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 2109
	 */
	ve_override_e veOverrideMode;
	/**
	 * offset 2110
	 */
	can_baudrate_e can2BaudRate;
	/**
	 * Override the Y axis (load) value used for the AFR table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 2111
	 */
	afr_override_e afrOverrideMode;
	/**
	 * offset 2112
	 */
	uint32_t verboseCan2BaseAddress = (uint32_t)0;
	/**
	 * CAN broadcast using custom rusEFI protocol
	 * enable can_broadcast/disable can_broadcast
	offset 2116 bit 0 */
	bool enableVerboseCan2Tx : 1;
	/**
	 * enable can_read/disable can_read
	offset 2116 bit 1 */
	bool can2ReadEnabled : 1;
	/**
	 * enable can_write/disable can_write
	offset 2116 bit 2 */
	bool can2WriteEnabled : 1;
	/**
	offset 2116 bit 3 */
	bool unused1126 : 1;
	/**
	offset 2116 bit 4 */
	bool unused1127 : 1;
	/**
	offset 2116 bit 5 */
	bool unused1128 : 1;
	/**
	offset 2116 bit 6 */
	bool unused1129 : 1;
	/**
	offset 2116 bit 7 */
	bool unused1130 : 1;
	/**
	offset 2116 bit 8 */
	bool unusedBit_500_8 : 1;
	/**
	offset 2116 bit 9 */
	bool unusedBit_500_9 : 1;
	/**
	offset 2116 bit 10 */
	bool unusedBit_500_10 : 1;
	/**
	offset 2116 bit 11 */
	bool unusedBit_500_11 : 1;
	/**
	offset 2116 bit 12 */
	bool unusedBit_500_12 : 1;
	/**
	offset 2116 bit 13 */
	bool unusedBit_500_13 : 1;
	/**
	offset 2116 bit 14 */
	bool unusedBit_500_14 : 1;
	/**
	offset 2116 bit 15 */
	bool unusedBit_500_15 : 1;
	/**
	offset 2116 bit 16 */
	bool unusedBit_500_16 : 1;
	/**
	offset 2116 bit 17 */
	bool unusedBit_500_17 : 1;
	/**
	offset 2116 bit 18 */
	bool unusedBit_500_18 : 1;
	/**
	offset 2116 bit 19 */
	bool unusedBit_500_19 : 1;
	/**
	offset 2116 bit 20 */
	bool unusedBit_500_20 : 1;
	/**
	offset 2116 bit 21 */
	bool unusedBit_500_21 : 1;
	/**
	offset 2116 bit 22 */
	bool unusedBit_500_22 : 1;
	/**
	offset 2116 bit 23 */
	bool unusedBit_500_23 : 1;
	/**
	offset 2116 bit 24 */
	bool unusedBit_500_24 : 1;
	/**
	offset 2116 bit 25 */
	bool unusedBit_500_25 : 1;
	/**
	offset 2116 bit 26 */
	bool unusedBit_500_26 : 1;
	/**
	offset 2116 bit 27 */
	bool unusedBit_500_27 : 1;
	/**
	offset 2116 bit 28 */
	bool unusedBit_500_28 : 1;
	/**
	offset 2116 bit 29 */
	bool unusedBit_500_29 : 1;
	/**
	offset 2116 bit 30 */
	bool unusedBit_500_30 : 1;
	/**
	offset 2116 bit 31 */
	bool unusedBit_500_31 : 1;
	/**
	 * set can_mode X
	 * offset 2120
	 */
	can_nbc_e can2NbcType;
	/**
	 * set_can2_tx_pin X
	 * offset 2124
	 */
	brain_pin_e can2TxPin;
	/**
	 * set_can2_rx_pin X
	 * offset 2125
	 */
	brain_pin_e can2RxPin;
	/**
	 * offset 2126
	 */
	pin_output_mode_e starterControlPinMode;
	/**
	 * offset 2127
	 */
	adc_channel_e wastegatePositionSensor;
	/**
	 * Override the Y axis (load) value used for the ignition table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 2128
	 */
	afr_override_e ignOverrideMode;
	/**
	 * Select which fuel pressure sensor measures the pressure of the fuel at your injectors.
	 * offset 2129
	 */
	injector_pressure_type_e injectorPressureType;
	/**
	 * offset 2130
	 */
	output_pin_e hpfpValvePin;
	/**
	 * offset 2131
	 */
	pin_output_mode_e hpfpValvePinMode;
	/**
	 * MAP value above which fuel is cut in case of overboost.
	 * Set to 0 to disable overboost cut.
	kPa (absolute)
	 * offset 2132
	 */
	float boostCutPressure = (float)0;
	/**
	counter
	 * offset 2136
	 */
	float mapAccelTaperBins[MAP_ACCEL_TAPER];
	/**
	mult
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
	deg
	 * offset 2204
	 */
	float fixedTiming = (float)0;
	/**
	 * MAP voltage for low point
	v
	 * offset 2208
	 */
	float mapLowValueVoltage = (float)0;
	/**
	 * MAP voltage for low point
	v
	 * offset 2212
	 */
	float mapHighValueVoltage = (float)0;
	/**
	 * EGO value correction
	value
	 * offset 2216
	 */
	float egoValueShift = (float)0;
	/**
	 * offset 2220
	 */
	output_pin_e auxPidPins[CAM_INPUTS_COUNT];
	/**
	 * offset 2224
	 */
	spi_device_e cj125SpiDevice;
	/**
	 * offset 2225
	 */
	pin_output_mode_e cj125CsPinMode;
	/**
	 * offset 2226
	 */
	pin_output_mode_e sdCardCsPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2227
	 */
	uint8_t alignmentFill_at_2227[1];
	/**
	 * This is the IAC position during cranking, some engines start better if given more air during cranking to improve cylinder filling.
	percent
	 * offset 2228
	 */
	int crankingIACposition = (int)0;
	/**
	mult
	 * offset 2232
	 */
	float tChargeMinRpmMinTps = (float)0;
	/**
	mult
	 * offset 2236
	 */
	float tChargeMinRpmMaxTps = (float)0;
	/**
	mult
	 * offset 2240
	 */
	float tChargeMaxRpmMinTps = (float)0;
	/**
	mult
	 * offset 2244
	 */
	float tChargeMaxRpmMaxTps = (float)0;
	/**
	 * offset 2248
	 */
	fsio_pwm_freq_t auxPidFrequency[CAMS_PER_BANK];
	/**
	 * Additional idle % when fan #1 is active
	%
	 * offset 2252
	 */
	uint8_t fan1ExtraIdle = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 2253
	 */
	uint8_t alignmentFill_at_2253[3];
	/**
	Hz
	 * offset 2256
	 */
	int alternatorPwmFrequency = (int)0;
	/**
	units
	 * offset 2260
	 */
	int unused2260 = (int)0;
	/**
	 * Narrow Band WBO Approximation
	V
	 * offset 2264
	 */
	float narrowToWideOxygenBins[NARROW_BAND_WIDE_BAND_CONVERSION_SIZE];
	/**
	ratio
	 * offset 2296
	 */
	float narrowToWideOxygen[NARROW_BAND_WIDE_BAND_CONVERSION_SIZE];
	/**
	 * set vvt_mode X
	 * offset 2328
	 */
	vvt_mode_e vvtMode[CAMS_PER_BANK];
	/**
	 * offset 2330
	 */
	uint8_t vvtModePadding[CAMS_PER_BANK_padding];
	/**
	 * Additional idle % when fan #2 is active
	%
	 * offset 2330
	 */
	uint8_t fan2ExtraIdle = (uint8_t)0;
	/**
	units
	 * offset 2331
	 */
	uint8_t unusedOldBiquad[9];
	/**
	 * offset 2340
	 */
	output_pin_e trailingCoilPins[MAX_CYLINDER_COUNT];
	/**
	 * CLT-based timing correction
	C
	 * offset 2352
	 */
	float cltTimingBins[CLT_TIMING_CURVE_SIZE];
	/**
	degree
	 * offset 2384
	 */
	float cltTimingExtra[CLT_TIMING_CURVE_SIZE];
	/**
	 * offset 2416
	 */
	tle8888_mode_e tle8888mode;
	/**
	 * offset 2417
	 */
	pin_output_mode_e LIS302DLCsPinMode;
	/**
	 * None = I have a MAP-referenced fuel pressure regulator
	 * Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	 * Sensed rail pressure = I have a fuel pressure sensor
	 * offset 2418
	 */
	injector_compensation_mode_e injectorCompensationMode;
	/**
	 * offset 2419
	 */
	pin_output_mode_e fan2PinMode;
	/**
	 * This is the pressure at which your injector flow is known.
	 * For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa here.
	kPa
	 * offset 2420
	 */
	float fuelReferencePressure = (float)0;
	/**
	units
	 * offset 2424
	 */
	float unused244_2 = (float)0;
	/**
	units
	 * offset 2428
	 */
	float unused244_3 = (float)0;
	/**
	units
	 * offset 2432
	 */
	float unused2432 = (float)0;
	/**
	 * Fuel multiplier (enrichment) immediately after engine start
	mult
	 * offset 2436
	 */
	float postCrankingFactor = (float)0;
	/**
	 * Time over which to taper out after start enrichment
	seconds
	 * offset 2440
	 */
	float postCrankingDurationSec = (float)0;
	/**
	 * offset 2444
	 */
	ThermistorConf auxTempSensor1;
	/**
	 * offset 2476
	 */
	ThermistorConf auxTempSensor2;
	/**
	 * Apply nonlinearity correction below a pulse of this duration. Pulses longer than this duration will receive no adjustment.
	ms
	 * offset 2508
	 */
	uint16_t applyNonlinearBelowPulse = (uint16_t)0;
	/**
	 * offset 2510
	 */
	InjectorNonlinearMode injectorNonlinearMode;
	/**
	units
	 * offset 2511
	 */
	uint8_t unused2508[3];
	/**
	Hz
	 * offset 2514
	 */
	int16_t etbFreq = (int16_t)0;
	/**
	 * offset 2516
	 */
	pid_s etbWastegatePid;
	/**
	units
	 * offset 2536
	 */
	uint8_t unused2536[4];
	/**
	 * per-cylinder timing correction
	deg
	 * offset 2540
	 */
	angle_t timing_offset_cylinder[MAX_CYLINDER_COUNT];
	/**
	seconds
	 * offset 2588
	 */
	float idlePidActivationTime = (float)0;
	/**
	 * offset 2592
	 */
	spi_device_e sdCardSpiDevice;
	/**
	units
	 * offset 2593
	 */
	uint8_t unusedSpiPadding4[3];
	/**
	 * offset 2596
	 */
	pin_mode_e spi1SckMode;
	/**
	 * Modes count be used for 3v<>5v integration using pull-ups/pull-downs etc.
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
	pid_s auxPid[CAMS_PER_BANK];
	/**
	 * offset 2652
	 */
	float injectorCorrectionPolynomial[8];
	/**
	units
	 * offset 2684
	 */
	uint8_t unused1366[8];
	/**
	 * offset 2692
	 */
	linear_sensor_s oilPressure;
	/**
	 * offset 2712
	 */
	spi_device_e accelerometerSpiDevice;
	/**
	 * offset 2713
	 */
	output_pin_e fan2Pin;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	deg C
	 * offset 2714
	 */
	uint8_t fan2OnTemperature = (uint8_t)0;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	deg C
	 * offset 2715
	 */
	uint8_t fan2OffTemperature = (uint8_t)0;
	/**
	x
	 * offset 2716
	 */
	float fsioCurve1Bins[FSIO_CURVE_16];
	/**
	y
	 * offset 2780
	 */
	float fsioCurve1[FSIO_CURVE_16];
	/**
	x
	 * offset 2844
	 */
	float fsioCurve2Bins[FSIO_CURVE_16];
	/**
	y
	 * offset 2908
	 */
	float fsioCurve2[FSIO_CURVE_16];
	/**
	x
	 * offset 2972
	 */
	float fsioCurve3Bins[FSIO_CURVE_8];
	/**
	y
	 * offset 3004
	 */
	float fsioCurve3[FSIO_CURVE_8];
	/**
	x
	 * offset 3036
	 */
	float fsioCurve4Bins[FSIO_CURVE_8];
	/**
	y
	 * offset 3068
	 */
	float fsioCurve4[FSIO_CURVE_8];
	/**
	 * Continental/GM flex fuel sensor, 50-150hz type
	 * offset 3100
	 */
	brain_input_pin_e flexSensorPin;
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
	output_pin_e stepperEnablePin;
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
	RPM
	 * offset 3108
	 */
	float crankingAdvanceBins[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	deg
	 * offset 3124
	 */
	float crankingAdvance[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * todo: more comments
	 * offset 3140
	 */
	brain_pin_e servoOutputPins[SERVO_COUNT];
	/**
	 * This sets the RPM above which fuel cut is active.
	rpm
	 * offset 3148
	 */
	int16_t coastingFuelCutRpmHigh = (int16_t)0;
	/**
	 * This sets the RPM below which fuel cut is deactivated, this prevents jerking or issues transitioning to idle
	rpm
	 * offset 3150
	 */
	int16_t coastingFuelCutRpmLow = (int16_t)0;
	/**
	 * Throttle position below which fuel cut is active.
	%
	 * offset 3152
	 */
	int16_t coastingFuelCutTps = (int16_t)0;
	/**
	 * Fuel cutoff is disabled when the engine is cold.
	C
	 * offset 3154
	 */
	int16_t coastingFuelCutClt = (int16_t)0;
	/**
	 * Increases PID reaction for RPM<target by adding extra percent to PID-error
	%
	 * offset 3156
	 */
	int16_t pidExtraForLowRpm = (int16_t)0;
	/**
	 * MAP value above which fuel injection is re-enabled.
	kPa
	 * offset 3158
	 */
	int16_t coastingFuelCutMap = (int16_t)0;
	/**
	 * CLT-based idle position for coasting (used in Auto-PID Idle mode)
	C
	 * offset 3160
	 */
	float iacCoastingBins[CLT_CURVE_SIZE];
	/**
	 *  CLT-based idle position for coasting (used in Auto-PID Idle mode)
	%
	 * offset 3224
	 */
	float iacCoasting[CLT_CURVE_SIZE];
	/**
	 * offset 3288
	 */
	linear_sensor_s highPressureFuel;
	/**
	 * offset 3308
	 */
	linear_sensor_s lowPressureFuel;
	/**
	 * CLT-based target RPM for hard limit depending on CLT like on Lexus LFA
	C
	 * offset 3328
	 */
	int8_t cltRevLimitRpmBins[CLT_LIMITER_CURVE_SIZE];
	/**
	 * See idleRpmPid
	 * offset 3332
	 */
	uint16_t cltRevLimitRpm[CLT_LIMITER_CURVE_SIZE];
	/**
	units
	 * offset 3340
	 */
	uint8_t unused3328[524];
	/**
	Min tCharge Coeff.
	 * offset 3864
	 */
	float tChargeAirCoefMin = (float)0;
	/**
	Max tCharge Coeff.
	 * offset 3868
	 */
	float tChargeAirCoefMax = (float)0;
	/**
	Max Air Flow (kg/h)
	 * offset 3872
	 */
	float tChargeAirFlowMax = (float)0;
	/**
	tChange Increment Limit (deg/sec)
	 * offset 3876
	 */
	float tChargeAirIncrLimit = (float)0;
	/**
	tChange Decrement Limit (deg/sec)
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
	target TPS position
	 * offset 3888
	 */
	float etbBiasBins[ETB_BIAS_CURVE_LENGTH];
	/**
	 * PWM bias, 0 to 100%
	ETB duty cycle bias
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
	units
	 * offset 3964
	 */
	uint8_t unused1059[4];
	/**
	 * See useIdleTimingPidControl
	 * offset 3968
	 */
	pid_s idleTimingPid;
	/**
	units
	 * offset 3988
	 */
	uint8_t unused3988[2];
	/**
	 * If the RPM closer to target than this value, disable timing correction to prevent oscillation
	RPM
	 * offset 3990
	 */
	int16_t idleTimingPidDeadZone = (int16_t)0;
	/**
	units
	 * offset 3992
	 */
	uint8_t unused3942[2];
	/**
	 * A delay in cycles between fuel-enrich. portions
	cycles
	 * offset 3994
	 */
	int16_t tpsAccelFractionPeriod = (int16_t)0;
	/**
	 * A fraction divisor: 1 or less = entire portion at once, or split into diminishing fractions
	coef
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
	 * Stoichiometric ratio for your primary fuel. When Flex Fuel is enabled, this value is used when the Flex Fuel sensor indicates E0.
	 * E0 = 14.7
	 * E10 = 14.1
	 * E85 = 9.9
	 * E100 = 9.0
	:1
	 * offset 4005
	 */
	uint8_t stoichRatioPrimary = (uint8_t)0;
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
	 * Stoichiometric ratio for your secondary fuel. This value is used when the Flex Fuel sensor indicates E100, typically 9.0
	:1
	 * offset 4009
	 */
	uint8_t stoichRatioSecondary = (uint8_t)0;
	/**
	units
	 * offset 4010
	 */
	uint8_t unusedSpiPadding8[2];
	/**
	 * This sets the range of the idle control on the ETB. At 100% idle position, the value specified here sets the base ETB position.
	%
	 * offset 4012
	 */
	float etbIdleThrottleRange = (float)0;
	/**
	 * Select which fuel correction bank this cylinder belongs to. Group cylinders that share the same O2 sensor
	 * offset 4016
	 */
	uint8_t cylinderBankSelect[MAX_CYLINDER_COUNT];
	/**
	units
	 * offset 4028
	 */
	int unused4028[2];
	/**
	 * Trigger comparator center point voltage
	V
	 * offset 4036
	 */
	uint8_t triggerCompCenterVolt = (uint8_t)0;
	/**
	 * Trigger comparator hysteresis voltage (Min)
	V
	 * offset 4037
	 */
	uint8_t triggerCompHystMin = (uint8_t)0;
	/**
	 * Trigger comparator hysteresis voltage (Max)
	V
	 * offset 4038
	 */
	uint8_t triggerCompHystMax = (uint8_t)0;
	/**
	 * VR-sensor saturation RPM
	RPM
	 * offset 4039
	 */
	uint8_t triggerCompSensorSatRpm = (uint8_t)0;
	/**
	 * offset 4040
	 */
	pid_s idleRpmPid2;
	/**
	 * offset 4060
	 */
	iac_pid_mult_t iacPidMultTable;
	/**
	Load
	 * offset 4124
	 */
	uint8_t iacPidMultLoadBins[IAC_PID_MULT_SIZE];
	/**
	RPM
	 * offset 4132
	 */
	uint8_t iacPidMultRpmBins[IAC_PID_MULT_SIZE];
	/**
	 * set can_vss X
	 * offset 4140
	 */
	can_vss_nbc_e canVssNbcType;
	/**
	 * offset 4144
	 */
	gppwm_channel gppwm[GPPWM_CHANNELS];
	/**
	mA
	 * offset 4496
	 */
	uint16_t mc33_i_boost = (uint16_t)0;
	/**
	mA
	 * offset 4498
	 */
	uint16_t mc33_i_peak = (uint16_t)0;
	/**
	mA
	 * offset 4500
	 */
	uint16_t mc33_i_hold = (uint16_t)0;
	/**
	us
	 * offset 4502
	 */
	uint16_t mc33_t_max_boost = (uint16_t)0;
	/**
	us
	 * offset 4504
	 */
	uint16_t mc33_t_peak_off = (uint16_t)0;
	/**
	us
	 * offset 4506
	 */
	uint16_t mc33_t_peak_tot = (uint16_t)0;
	/**
	us
	 * offset 4508
	 */
	uint16_t mc33_t_bypass = (uint16_t)0;
	/**
	us
	 * offset 4510
	 */
	uint16_t mc33_t_hold_off = (uint16_t)0;
	/**
	us
	 * offset 4512
	 */
	uint16_t mc33_t_hold_tot = (uint16_t)0;
	/**
	 * offset 4514
	 */
	pin_input_mode_e tcuUpshiftButtonPinMode;
	/**
	 * offset 4515
	 */
	pin_input_mode_e tcuDownshiftButtonPinMode;
	/**
	 * offset 4516
	 */
	pin_input_mode_e acSwitchMode;
	/**
	 * offset 4517
	 */
	pin_output_mode_e tcu_solenoid_mode[TCU_SOLENOID_COUNT];
	/**
	dB
	 * offset 4523
	 */
	int8_t knockBaseNoise[IGN_RPM_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 4539
	 */
	uint8_t alignmentFill_at_4539[1];
	/**
	ratio
	 * offset 4540
	 */
	float triggerGapOverride[GAP_TRACKING_LENGTH];
	/**
	units
	 * offset 4612
	 */
	int mainUnusedEnd[347];
	/** total size 6000*/
};

// start of persistent_config_s
struct persistent_config_s {
	/**
	 * offset 0
	 */
	engine_configuration_s engineConfiguration;
	/**
	 * offset 6000
	 */
	error_message_t warning_message;
	/**
	C
	 * offset 6120
	 */
	float afterstartCoolantBins[AFTERSTART_HOLD_CURVE_SIZE];
	/**
	Seconds
	 * offset 6152
	 */
	float afterstartHoldTime[AFTERSTART_HOLD_CURVE_SIZE];
	/**
	%
	 * offset 6184
	 */
	float afterstartEnrich[AFTERSTART_ENRICH_CURVE_SIZE];
	/**
	Seconds
	 * offset 6216
	 */
	float afterstartDecayTime[AFTERSTART_DECAY_CURVE_SIZE];
	/**
	 * offset 6248
	 */
	boost_table_t boostTableOpenLoop;
	/**
	units
	 * offset 6312
	 */
	uint8_t unused6312[8];
	/**
	RPM
	 * offset 6320
	 */
	uint8_t boostRpmBins[BOOST_RPM_COUNT];
	/**
	 * offset 6328
	 */
	boost_target_table_t boostTableClosedLoop;
	/**
	%
	 * offset 6392
	 */
	uint8_t boostTpsBins[BOOST_LOAD_COUNT];
	/**
	 * offset 6400
	 */
	pedal_to_tps_t pedalToTpsTable;
	/**
	%
	 * offset 6464
	 */
	uint8_t pedalToTpsPedalBins[PEDAL_TO_TPS_SIZE];
	/**
	RPM
	 * offset 6472
	 */
	uint8_t pedalToTpsRpmBins[PEDAL_TO_TPS_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	C
	 * offset 6480
	 */
	float cltCrankingCorrBins[CLT_CRANKING_CURVE_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	%
	 * offset 6512
	 */
	float cltCrankingCorr[CLT_CRANKING_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	RPM
	 * offset 6544
	 */
	float idleAdvanceBins[IDLE_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	deg
	 * offset 6576
	 */
	float idleAdvance[IDLE_ADVANCE_CURVE_SIZE];
	/**
	 * Optional VE table for Idle (see useSeparateVEForIdle)
	RPM
	 * offset 6608
	 */
	float idleVeBins[IDLE_VE_CURVE_SIZE];
	/**
	 *  Optional VE table for Idle (see useSeparateVEForIdle)
	%
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
	C
	 * offset 10272
	 */
	float cltFuelCorrBins[CLT_CURVE_SIZE];
	/**
	ratio
	 * offset 10336
	 */
	float cltFuelCorr[CLT_CURVE_SIZE];
	/**
	C
	 * offset 10400
	 */
	float iatFuelCorrBins[IAT_CURVE_SIZE];
	/**
	ratio
	 * offset 10464
	 */
	float iatFuelCorr[IAT_CURVE_SIZE];
	/**
	ratio
	 * offset 10528
	 */
	float crankingFuelCoef[CRANKING_CURVE_SIZE];
	/**
	C
	 * offset 10560
	 */
	float crankingFuelBins[CRANKING_CURVE_SIZE];
	/**
	ratio
	 * offset 10592
	 */
	float crankingCycleCoef[CRANKING_CURVE_SIZE];
	/**
	counter
	 * offset 10624
	 */
	float crankingCycleBins[CRANKING_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	C
	 * offset 10656
	 */
	float cltIdleCorrBins[CLT_CURVE_SIZE];
	/**
	 *  CLT-based idle position multiplier for simple manual idle controller
	ratio
	 * offset 10720
	 */
	float cltIdleCorr[CLT_CURVE_SIZE];
	/**
	 * Also known as MAF transfer function.
	 * kg/hour value.
	 * By the way 2.081989116 kg/h = 1 ft3/m
	kg/hour
	 * offset 10784
	 */
	float mafDecoding[MAF_DECODING_COUNT];
	/**
	V
	 * offset 11808
	 */
	float mafDecodingBins[MAF_DECODING_COUNT];
	/**
	 * offset 12832
	 */
	angle_table_t ignitionIatCorrTable;
	/**
	Temperature
	 * offset 13856
	 */
	float ignitionIatCorrLoadBins[IGN_LOAD_COUNT];
	/**
	RPM
	 * offset 13920
	 */
	float ignitionIatCorrRpmBins[IGN_RPM_COUNT];
	/**
	 * offset 13984
	 */
	angle_table_t injectionPhase;
	/**
	Load
	 * offset 15008
	 */
	float injPhaseLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 15072
	 */
	float injPhaseRpmBins[FUEL_RPM_COUNT];
	/**
	 * offset 15136
	 */
	tcubinary_table_t tcuSolenoidTable;
	/**
	 * Good example: number of tooth on wheel, For Can 10 is a good number.
	Hz
	 * offset 15196
	 */
	float vssFilterReciprocal = (float)0;
	/**
	 * offset 15200
	 */
	map_estimate_table_t mapEstimateTable;
	/**
	% TPS
	 * offset 15712
	 */
	uint16_t mapEstimateTpsBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 15744
	 */
	uint16_t mapEstimateRpmBins[FUEL_RPM_COUNT];
	/**
	 * offset 15776
	 */
	fsio_table_8x8_u8t vvtTable1;
	/**
	L
	 * offset 15840
	 */
	float vvtTable1LoadBins[FSIO_TABLE_8];
	/**
	 * RPM is float and not integer in order to use unified methods for interpolation
	RPM
	 * offset 15872
	 */
	float vvtTable1RpmBins[FSIO_TABLE_8];
	/**
	 * offset 15904
	 */
	fsio_table_8x8_u8t vvtTable2;
	/**
	L
	 * offset 15968
	 */
	float vvtTable2LoadBins[FSIO_TABLE_8];
	/**
	 * RPM is float and not integer in order to use unified methods for interpolation
	RPM
	 * offset 16000
	 */
	float vvtTable2RpmBins[FSIO_TABLE_8];
	/**
	 * offset 16032
	 */
	lua_script_t luaScript;
	/**
	 * offset 16288
	 */
	ignition_table_t ignitionTable;
	/**
	Load
	 * offset 17312
	 */
	float ignitionLoadBins[IGN_LOAD_COUNT];
	/**
	RPM
	 * offset 17376
	 */
	float ignitionRpmBins[IGN_RPM_COUNT];
	/**
	 * offset 17440
	 */
	ve_table_t veTable;
	/**
	kPa
	 * offset 18464
	 */
	float veLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 18528
	 */
	float veRpmBins[FUEL_RPM_COUNT];
	/**
	 * offset 18592
	 */
	lambda_table_t lambdaTable;
	/**
	 * offset 18848
	 */
	float lambdaLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 18912
	 */
	float lambdaRpmBins[FUEL_RPM_COUNT];
	/**
	 * offset 18976
	 */
	tps_tps_table_t tpsTpsAccelTable;
	/**
	from
	 * offset 19232
	 */
	float tpsTpsAccelFromRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	 * RPM is float and not integer in order to use unified methods for interpolation
	to
	 * offset 19264
	 */
	float tpsTpsAccelToRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	 * offset 19296
	 */
	fsio_table_8x8_f32t fsioTable1;
	/**
	L
	 * offset 19552
	 */
	float fsioTable1LoadBins[FSIO_TABLE_8];
	/**
	 * RPM is float and not integer in order to use unified methods for interpolation
	RPM
	 * offset 19584
	 */
	float fsioTable1RpmBins[FSIO_TABLE_8];
	/**
	 * offset 19616
	 */
	fsio_table_8x8_u8t fsioTable2;
	/**
	L
	 * offset 19680
	 */
	float fsioTable2LoadBins[FSIO_TABLE_8];
	/**
	 * RPM is float and not integer in order to use unified methods for interpolation
	RPM
	 * offset 19712
	 */
	float fsioTable2RpmBins[FSIO_TABLE_8];
	/**
	 * offset 19744
	 */
	fsio_table_8x8_u8t fsioTable3;
	/**
	L
	 * offset 19808
	 */
	float fsioTable3LoadBins[FSIO_TABLE_8];
	/**
	 * RPM is float and not integer in order to use unified methods for interpolation
	RPM
	 * offset 19840
	 */
	float fsioTable3RpmBins[FSIO_TABLE_8];
	/**
	 * offset 19872
	 */
	fsio_table_8x8_u8t fsioTable4;
	/**
	L
	 * offset 19936
	 */
	float fsioTable4LoadBins[FSIO_TABLE_8];
	/**
	 * RPM is float and not integer in order to use unified methods for interpolation
	RPM
	 * offset 19968
	 */
	float fsioTable4RpmBins[FSIO_TABLE_8];
	/** total size 20000*/
};

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on gen_config.sh integration/rusefi_config.txt Fri Jul 30 21:18:00 UTC 2021
