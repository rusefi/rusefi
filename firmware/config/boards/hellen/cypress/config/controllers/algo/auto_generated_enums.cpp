#include "global.h"
#include "rusefi_enums.h"
#include "rusefi_hw_enums.h"
// was generated automatically by rusEFI tool  from rusefi_hw_enums.h // was generated automatically by rusEFI tool  from rusefi_enums.h // by enum2string.jar tool on Sun Jun 13 21:14:31 EDT 2021
// see also gen_config_and_enums.bat



const char *getPidAutoTune_AutoTunerState(PidAutoTune_AutoTunerState value){
switch(value) {
case AUTOTUNER_OFF:
  return "AUTOTUNER_OFF";
case CONVERGED:
  return "CONVERGED";
case FAILED:
  return "FAILED";
case RELAY_STEP_DOWN:
  return "RELAY_STEP_DOWN";
case RELAY_STEP_UP:
  return "RELAY_STEP_UP";
case STEADY_STATE_AFTER_STEP_UP:
  return "STEADY_STATE_AFTER_STEP_UP";
case STEADY_STATE_AT_BASELINE:
  return "STEADY_STATE_AT_BASELINE";
  }
 return NULL;
}
const char *getPidAutoTune_Peak(PidAutoTune_Peak value){
switch(value) {
case MAXIMUM:
  return "MAXIMUM";
case MINIMUM:
  return "MINIMUM";
case NOT_A_PEAK:
  return "NOT_A_PEAK";
  }
 return NULL;
}
const char *getAdc_channel_e(adc_channel_e value){
switch(value) {
case EFI_ADC_0:
  return "EFI_ADC_0";
case EFI_ADC_1:
  return "EFI_ADC_1";
case EFI_ADC_10:
  return "EFI_ADC_10";
case EFI_ADC_11:
  return "EFI_ADC_11";
case EFI_ADC_12:
  return "EFI_ADC_12";
case EFI_ADC_13:
  return "EFI_ADC_13";
case EFI_ADC_14:
  return "EFI_ADC_14";
case EFI_ADC_15:
  return "EFI_ADC_15";
case EFI_ADC_16:
  return "EFI_ADC_16";
case EFI_ADC_17:
  return "EFI_ADC_17";
case EFI_ADC_18:
  return "EFI_ADC_18";
case EFI_ADC_19:
  return "EFI_ADC_19";
case EFI_ADC_2:
  return "EFI_ADC_2";
case EFI_ADC_20:
  return "EFI_ADC_20";
case EFI_ADC_21:
  return "EFI_ADC_21";
case EFI_ADC_22:
  return "EFI_ADC_22";
case EFI_ADC_23:
  return "EFI_ADC_23";
case EFI_ADC_24:
  return "EFI_ADC_24";
case EFI_ADC_25:
  return "EFI_ADC_25";
case EFI_ADC_26:
  return "EFI_ADC_26";
case EFI_ADC_27:
  return "EFI_ADC_27";
case EFI_ADC_28:
  return "EFI_ADC_28";
case EFI_ADC_29:
  return "EFI_ADC_29";
case EFI_ADC_3:
  return "EFI_ADC_3";
case EFI_ADC_30:
  return "EFI_ADC_30";
case EFI_ADC_31:
  return "EFI_ADC_31";
case EFI_ADC_4:
  return "EFI_ADC_4";
case EFI_ADC_5:
  return "EFI_ADC_5";
case EFI_ADC_6:
  return "EFI_ADC_6";
case EFI_ADC_7:
  return "EFI_ADC_7";
case EFI_ADC_8:
  return "EFI_ADC_8";
case EFI_ADC_9:
  return "EFI_ADC_9";
case EFI_ADC_ERROR:
  return "EFI_ADC_ERROR";
case EFI_ADC_LAST_CHANNEL:
  return "EFI_ADC_LAST_CHANNEL";
case EFI_ADC_NONE:
  return "EFI_ADC_NONE";
  }
 return NULL;
}
const char *getAdc_channel_mode_e(adc_channel_mode_e value){
switch(value) {
case ADC_FAST:
  return "ADC_FAST";
case ADC_OFF:
  return "ADC_OFF";
case ADC_SLOW:
  return "ADC_SLOW";
case Force_4_bytes_size_adc_channel_mode:
  return "Force_4_bytes_size_adc_channel_mode";
  }
 return NULL;
}
const char *getAfr_override_e(afr_override_e value){
switch(value) {
case AFR_AccPedal:
  return "AFR_AccPedal";
case AFR_CylFilling:
  return "AFR_CylFilling";
case AFR_MAP:
  return "AFR_MAP";
case AFR_None:
  return "AFR_None";
case AFR_Tps:
  return "AFR_Tps";
  }
 return NULL;
}
const char *getAir_pressure_sensor_type_e(air_pressure_sensor_type_e value){
switch(value) {
case Force_4_bytes_size_cranking_map_type:
  return "Force_4_bytes_size_cranking_map_type";
case MT_BOSCH_2_5:
  return "MT_BOSCH_2_5";
case MT_CUSTOM:
  return "MT_CUSTOM";
case MT_DENSO183:
  return "MT_DENSO183";
case MT_DODGE_NEON_2003:
  return "MT_DODGE_NEON_2003";
case MT_GM_1_BAR:
  return "MT_GM_1_BAR";
case MT_GM_2_BAR:
  return "MT_GM_2_BAR";
case MT_GM_3_BAR:
  return "MT_GM_3_BAR";
case MT_HONDA3BAR:
  return "MT_HONDA3BAR";
case MT_MAZDA_1_BAR:
  return "MT_MAZDA_1_BAR";
case MT_MPX4100:
  return "MT_MPX4100";
case MT_MPX4250:
  return "MT_MPX4250";
case MT_MPX4250A:
  return "MT_MPX4250A";
case MT_SUBY_DENSO:
  return "MT_SUBY_DENSO";
case MT_TOYOTA_89420_02010:
  return "MT_TOYOTA_89420_02010";
  }
 return NULL;
}
const char *getAntiLagActivationMode_e(antiLagActivationMode_e value){
switch(value) {
case ALWAYS_ON_ANTILAG:
  return "ALWAYS_ON_ANTILAG";
case Force_4bytes_size_antiLagActivationMode_e:
  return "Force_4bytes_size_antiLagActivationMode_e";
case SWITCH_INPUT_ANTILAG:
  return "SWITCH_INPUT_ANTILAG";
  }
 return NULL;
}
const char *getBoostType_e(boostType_e value){
switch(value) {
case CLOSED_LOOP:
  return "CLOSED_LOOP";
case Force_4bytes_size_boostType_e:
  return "Force_4bytes_size_boostType_e";
case OPEN_LOOP:
  return "OPEN_LOOP";
  }
 return NULL;
}
const char *getBrain_pin_diag_e(brain_pin_diag_e value){
switch(value) {
case PIN_DRIVER_OVERTEMP:
  return "PIN_DRIVER_OVERTEMP";
case PIN_INVALID:
  return "PIN_INVALID";
case PIN_OK:
  return "PIN_OK";
case PIN_OPEN:
  return "PIN_OPEN";
case PIN_OVERLOAD:
  return "PIN_OVERLOAD";
case PIN_SHORT_TO_BAT:
  return "PIN_SHORT_TO_BAT";
case PIN_SHORT_TO_GND:
  return "PIN_SHORT_TO_GND";
  }
 return NULL;
}
const char *getBrain_pin_e(brain_pin_e value){
switch(value) {
case DRV8860_PIN_1:
  return "DRV8860_PIN_1";
case DRV8860_PIN_10:
  return "DRV8860_PIN_10";
case DRV8860_PIN_11:
  return "DRV8860_PIN_11";
case DRV8860_PIN_12:
  return "DRV8860_PIN_12";
case DRV8860_PIN_13:
  return "DRV8860_PIN_13";
case DRV8860_PIN_14:
  return "DRV8860_PIN_14";
case DRV8860_PIN_15:
  return "DRV8860_PIN_15";
case DRV8860_PIN_16:
  return "DRV8860_PIN_16";
case DRV8860_PIN_2:
  return "DRV8860_PIN_2";
case DRV8860_PIN_3:
  return "DRV8860_PIN_3";
case DRV8860_PIN_4:
  return "DRV8860_PIN_4";
case DRV8860_PIN_5:
  return "DRV8860_PIN_5";
case DRV8860_PIN_6:
  return "DRV8860_PIN_6";
case DRV8860_PIN_7:
  return "DRV8860_PIN_7";
case DRV8860_PIN_8:
  return "DRV8860_PIN_8";
case DRV8860_PIN_9:
  return "DRV8860_PIN_9";
case GPIOA_0:
  return "GPIOA_0";
case GPIOA_1:
  return "GPIOA_1";
case GPIOA_10:
  return "GPIOA_10";
case GPIOA_11:
  return "GPIOA_11";
case GPIOA_12:
  return "GPIOA_12";
case GPIOA_13:
  return "GPIOA_13";
case GPIOA_14:
  return "GPIOA_14";
case GPIOA_15:
  return "GPIOA_15";
case GPIOA_2:
  return "GPIOA_2";
case GPIOA_3:
  return "GPIOA_3";
case GPIOA_4:
  return "GPIOA_4";
case GPIOA_5:
  return "GPIOA_5";
case GPIOA_6:
  return "GPIOA_6";
case GPIOA_7:
  return "GPIOA_7";
case GPIOA_8:
  return "GPIOA_8";
case GPIOA_9:
  return "GPIOA_9";
case GPIOB_0:
  return "GPIOB_0";
case GPIOB_1:
  return "GPIOB_1";
case GPIOB_10:
  return "GPIOB_10";
case GPIOB_11:
  return "GPIOB_11";
case GPIOB_12:
  return "GPIOB_12";
case GPIOB_13:
  return "GPIOB_13";
case GPIOB_14:
  return "GPIOB_14";
case GPIOB_15:
  return "GPIOB_15";
case GPIOB_2:
  return "GPIOB_2";
case GPIOB_3:
  return "GPIOB_3";
case GPIOB_4:
  return "GPIOB_4";
case GPIOB_5:
  return "GPIOB_5";
case GPIOB_6:
  return "GPIOB_6";
case GPIOB_7:
  return "GPIOB_7";
case GPIOB_8:
  return "GPIOB_8";
case GPIOB_9:
  return "GPIOB_9";
case GPIOC_0:
  return "GPIOC_0";
case GPIOC_1:
  return "GPIOC_1";
case GPIOC_10:
  return "GPIOC_10";
case GPIOC_11:
  return "GPIOC_11";
case GPIOC_12:
  return "GPIOC_12";
case GPIOC_13:
  return "GPIOC_13";
case GPIOC_14:
  return "GPIOC_14";
case GPIOC_15:
  return "GPIOC_15";
case GPIOC_2:
  return "GPIOC_2";
case GPIOC_3:
  return "GPIOC_3";
case GPIOC_4:
  return "GPIOC_4";
case GPIOC_5:
  return "GPIOC_5";
case GPIOC_6:
  return "GPIOC_6";
case GPIOC_7:
  return "GPIOC_7";
case GPIOC_8:
  return "GPIOC_8";
case GPIOC_9:
  return "GPIOC_9";
case GPIOD_0:
  return "GPIOD_0";
case GPIOD_1:
  return "GPIOD_1";
case GPIOD_10:
  return "GPIOD_10";
case GPIOD_11:
  return "GPIOD_11";
case GPIOD_12:
  return "GPIOD_12";
case GPIOD_13:
  return "GPIOD_13";
case GPIOD_14:
  return "GPIOD_14";
case GPIOD_15:
  return "GPIOD_15";
case GPIOD_2:
  return "GPIOD_2";
case GPIOD_3:
  return "GPIOD_3";
case GPIOD_4:
  return "GPIOD_4";
case GPIOD_5:
  return "GPIOD_5";
case GPIOD_6:
  return "GPIOD_6";
case GPIOD_7:
  return "GPIOD_7";
case GPIOD_8:
  return "GPIOD_8";
case GPIOD_9:
  return "GPIOD_9";
case GPIOE_0:
  return "GPIOE_0";
case GPIOE_1:
  return "GPIOE_1";
case GPIOE_10:
  return "GPIOE_10";
case GPIOE_11:
  return "GPIOE_11";
case GPIOE_12:
  return "GPIOE_12";
case GPIOE_13:
  return "GPIOE_13";
case GPIOE_14:
  return "GPIOE_14";
case GPIOE_15:
  return "GPIOE_15";
case GPIOE_2:
  return "GPIOE_2";
case GPIOE_3:
  return "GPIOE_3";
case GPIOE_4:
  return "GPIOE_4";
case GPIOE_5:
  return "GPIOE_5";
case GPIOE_6:
  return "GPIOE_6";
case GPIOE_7:
  return "GPIOE_7";
case GPIOE_8:
  return "GPIOE_8";
case GPIOE_9:
  return "GPIOE_9";
case GPIOF_0:
  return "GPIOF_0";
case GPIOF_1:
  return "GPIOF_1";
case GPIOF_10:
  return "GPIOF_10";
case GPIOF_11:
  return "GPIOF_11";
case GPIOF_12:
  return "GPIOF_12";
case GPIOF_13:
  return "GPIOF_13";
case GPIOF_14:
  return "GPIOF_14";
case GPIOF_15:
  return "GPIOF_15";
case GPIOF_2:
  return "GPIOF_2";
case GPIOF_3:
  return "GPIOF_3";
case GPIOF_4:
  return "GPIOF_4";
case GPIOF_5:
  return "GPIOF_5";
case GPIOF_6:
  return "GPIOF_6";
case GPIOF_7:
  return "GPIOF_7";
case GPIOF_8:
  return "GPIOF_8";
case GPIOF_9:
  return "GPIOF_9";
case GPIOG_0:
  return "GPIOG_0";
case GPIOG_1:
  return "GPIOG_1";
case GPIOG_10:
  return "GPIOG_10";
case GPIOG_11:
  return "GPIOG_11";
case GPIOG_12:
  return "GPIOG_12";
case GPIOG_13:
  return "GPIOG_13";
case GPIOG_14:
  return "GPIOG_14";
case GPIOG_15:
  return "GPIOG_15";
case GPIOG_2:
  return "GPIOG_2";
case GPIOG_3:
  return "GPIOG_3";
case GPIOG_4:
  return "GPIOG_4";
case GPIOG_5:
  return "GPIOG_5";
case GPIOG_6:
  return "GPIOG_6";
case GPIOG_7:
  return "GPIOG_7";
case GPIOG_8:
  return "GPIOG_8";
case GPIOG_9:
  return "GPIOG_9";
case GPIOH_0:
  return "GPIOH_0";
case GPIOH_1:
  return "GPIOH_1";
case GPIOH_10:
  return "GPIOH_10";
case GPIOH_11:
  return "GPIOH_11";
case GPIOH_12:
  return "GPIOH_12";
case GPIOH_13:
  return "GPIOH_13";
case GPIOH_14:
  return "GPIOH_14";
case GPIOH_15:
  return "GPIOH_15";
case GPIOH_2:
  return "GPIOH_2";
case GPIOH_3:
  return "GPIOH_3";
case GPIOH_4:
  return "GPIOH_4";
case GPIOH_5:
  return "GPIOH_5";
case GPIOH_6:
  return "GPIOH_6";
case GPIOH_7:
  return "GPIOH_7";
case GPIOH_8:
  return "GPIOH_8";
case GPIOH_9:
  return "GPIOH_9";
case GPIOI_0:
  return "GPIOI_0";
case GPIOI_1:
  return "GPIOI_1";
case GPIOI_10:
  return "GPIOI_10";
case GPIOI_11:
  return "GPIOI_11";
case GPIOI_12:
  return "GPIOI_12";
case GPIOI_13:
  return "GPIOI_13";
case GPIOI_14:
  return "GPIOI_14";
case GPIOI_15:
  return "GPIOI_15";
case GPIOI_2:
  return "GPIOI_2";
case GPIOI_3:
  return "GPIOI_3";
case GPIOI_4:
  return "GPIOI_4";
case GPIOI_5:
  return "GPIOI_5";
case GPIOI_6:
  return "GPIOI_6";
case GPIOI_7:
  return "GPIOI_7";
case GPIOI_8:
  return "GPIOI_8";
case GPIOI_9:
  return "GPIOI_9";
case GPIOJ_0:
  return "GPIOJ_0";
case GPIOJ_1:
  return "GPIOJ_1";
case GPIOJ_10:
  return "GPIOJ_10";
case GPIOJ_11:
  return "GPIOJ_11";
case GPIOJ_12:
  return "GPIOJ_12";
case GPIOJ_13:
  return "GPIOJ_13";
case GPIOJ_14:
  return "GPIOJ_14";
case GPIOJ_15:
  return "GPIOJ_15";
case GPIOJ_2:
  return "GPIOJ_2";
case GPIOJ_3:
  return "GPIOJ_3";
case GPIOJ_4:
  return "GPIOJ_4";
case GPIOJ_5:
  return "GPIOJ_5";
case GPIOJ_6:
  return "GPIOJ_6";
case GPIOJ_7:
  return "GPIOJ_7";
case GPIOJ_8:
  return "GPIOJ_8";
case GPIOJ_9:
  return "GPIOJ_9";
case GPIOK_0:
  return "GPIOK_0";
case GPIOK_1:
  return "GPIOK_1";
case GPIOK_10:
  return "GPIOK_10";
case GPIOK_11:
  return "GPIOK_11";
case GPIOK_12:
  return "GPIOK_12";
case GPIOK_13:
  return "GPIOK_13";
case GPIOK_14:
  return "GPIOK_14";
case GPIOK_15:
  return "GPIOK_15";
case GPIOK_2:
  return "GPIOK_2";
case GPIOK_3:
  return "GPIOK_3";
case GPIOK_4:
  return "GPIOK_4";
case GPIOK_5:
  return "GPIOK_5";
case GPIOK_6:
  return "GPIOK_6";
case GPIOK_7:
  return "GPIOK_7";
case GPIOK_8:
  return "GPIOK_8";
case GPIOK_9:
  return "GPIOK_9";
case GPIO_INVALID:
  return "GPIO_INVALID";
case GPIO_UNASSIGNED:
  return "GPIO_UNASSIGNED";
  }
 return NULL;
}
const char *getCan_baudrate_e(can_baudrate_e value){
switch(value) {
case B100KBPS:
  return "B100KBPS";
case B1MBPS:
  return "B1MBPS";
case B250KBPS:
  return "B250KBPS";
case B500KBPS:
  return "B500KBPS";
  }
 return NULL;
}
const char *getCan_nbc_e(can_nbc_e value){
switch(value) {
case CAN_BUS_BMW_E90:
  return "CAN_BUS_BMW_E90";
case CAN_BUS_Haltech:
  return "CAN_BUS_Haltech";
case CAN_BUS_MAZDA_RX8:
  return "CAN_BUS_MAZDA_RX8";
case CAN_BUS_MQB:
  return "CAN_BUS_MQB";
case CAN_BUS_NBC_BMW:
  return "CAN_BUS_NBC_BMW";
case CAN_BUS_NBC_FIAT:
  return "CAN_BUS_NBC_FIAT";
case CAN_BUS_NBC_NONE:
  return "CAN_BUS_NBC_NONE";
case CAN_BUS_NBC_VAG:
  return "CAN_BUS_NBC_VAG";
case CAN_BUS_W202_C180:
  return "CAN_BUS_W202_C180";
case Internal_ForceMyEnumIntSize_can_nbc:
  return "Internal_ForceMyEnumIntSize_can_nbc";
  }
 return NULL;
}
const char *getCan_vss_nbc_e(can_vss_nbc_e value){
switch(value) {
case BMW_e46:
  return "BMW_e46";
case Force_4_bytes_size_can_vss_nbc_e:
  return "Force_4_bytes_size_can_vss_nbc_e";
case W202:
  return "W202";
  }
 return NULL;
}
const char *getChamber_style_e(chamber_style_e value){
switch(value) {
case CS_CLOSED:
  return "CS_CLOSED";
case CS_OPEN:
  return "CS_OPEN";
case CS_SWIRL_TUMBLE:
  return "CS_SWIRL_TUMBLE";
case Internal_ForceMyEnumIntSize_chamber_stype:
  return "Internal_ForceMyEnumIntSize_chamber_stype";
  }
 return NULL;
}
const char *getCranking_ignition_mode_e(cranking_ignition_mode_e value){
switch(value) {
case CIM_DEFAULT:
  return "CIM_DEFAULT";
case CIM_FIXED_ANGLE:
  return "CIM_FIXED_ANGLE";
case Force_4_bytes_size_cranking_ignition_mode:
  return "Force_4_bytes_size_cranking_ignition_mode";
  }
 return NULL;
}
const char *getDebug_mode_e(debug_mode_e value){
switch(value) {
case DBG_16:
  return "DBG_16";
case DBG_34:
  return "DBG_34";
case DBG_ALTERNATOR_PID:
  return "DBG_ALTERNATOR_PID";
case DBG_ANALOG_INPUTS:
  return "DBG_ANALOG_INPUTS";
case DBG_ANALOG_INPUTS2:
  return "DBG_ANALOG_INPUTS2";
case DBG_AUX_PID_1:
  return "DBG_AUX_PID_1";
case DBG_AUX_VALVES:
  return "DBG_AUX_VALVES";
case DBG_BENCH_TEST:
  return "DBG_BENCH_TEST";
case DBG_BOOST:
  return "DBG_BOOST";
case DBG_CAN:
  return "DBG_CAN";
case DBG_CJ125:
  return "DBG_CJ125";
case DBG_COMPOSITE_LOG:
  return "DBG_COMPOSITE_LOG";
case DBG_CRANKING_DETAILS:
  return "DBG_CRANKING_DETAILS";
case DBG_DWELL_METRIC:
  return "DBG_DWELL_METRIC";
case DBG_DYNO_VIEW:
  return "DBG_DYNO_VIEW";
case DBG_ELECTRONIC_THROTTLE_EXTRA:
  return "DBG_ELECTRONIC_THROTTLE_EXTRA";
case DBG_ELECTRONIC_THROTTLE_PID:
  return "DBG_ELECTRONIC_THROTTLE_PID";
case DBG_EL_ACCEL:
  return "DBG_EL_ACCEL";
case DBG_ETB_AUTOTUNE:
  return "DBG_ETB_AUTOTUNE";
case DBG_ETB_LOGIC:
  return "DBG_ETB_LOGIC";
case DBG_EXECUTOR:
  return "DBG_EXECUTOR";
case DBG_FSIO_ADC:
  return "DBG_FSIO_ADC";
case DBG_FSIO_EXPRESSION_1_7:
  return "DBG_FSIO_EXPRESSION_1_7";
case DBG_FSIO_EXPRESSION_8_14:
  return "DBG_FSIO_EXPRESSION_8_14";
case DBG_FSIO_SPECIAL:
  return "DBG_FSIO_SPECIAL";
case DBG_FUEL_PID_CORRECTION:
  return "DBG_FUEL_PID_CORRECTION";
case DBG_GPPWM:
  return "DBG_GPPWM";
case DBG_IDLE_CONTROL:
  return "DBG_IDLE_CONTROL";
case DBG_IGNITION_TIMING:
  return "DBG_IGNITION_TIMING";
case DBG_INJECTOR_COMPENSATION:
  return "DBG_INJECTOR_COMPENSATION";
case DBG_INSTANT_RPM:
  return "DBG_INSTANT_RPM";
case DBG_ION:
  return "DBG_ION";
case DBG_KNOCK:
  return "DBG_KNOCK";
case DBG_LAUNCH:
  return "DBG_LAUNCH";
case DBG_LOGIC_ANALYZER:
  return "DBG_LOGIC_ANALYZER";
case DBG_LUA:
  return "DBG_LUA";
case DBG_MAP:
  return "DBG_MAP";
case DBG_METRICS:
  return "DBG_METRICS";
case DBG_RUSEFI_WIDEBAND:
  return "DBG_RUSEFI_WIDEBAND";
case DBG_SD_CARD:
  return "DBG_SD_CARD";
case DBG_SR5_PROTOCOL:
  return "DBG_SR5_PROTOCOL";
case DBG_START_STOP:
  return "DBG_START_STOP";
case DBG_STATUS:
  return "DBG_STATUS";
case DBG_TCU:
  return "DBG_TCU";
case DBG_TLE8888:
  return "DBG_TLE8888";
case DBG_TPS_ACCEL:
  return "DBG_TPS_ACCEL";
case DBG_TRIGGER_COUNTERS:
  return "DBG_TRIGGER_COUNTERS";
case DBG_VEHICLE_SPEED_SENSOR:
  return "DBG_VEHICLE_SPEED_SENSOR";
case DBG_VVT:
  return "DBG_VVT";
case Force_4_bytes_size_debug_mode_e:
  return "Force_4_bytes_size_debug_mode_e";
  }
 return NULL;
}
const char *getDisplay_mode_e(display_mode_e value){
switch(value) {
case DM_HD44780:
  return "DM_HD44780";
case DM_HD44780_OVER_PCF8574:
  return "DM_HD44780_OVER_PCF8574";
case DM_NONE:
  return "DM_NONE";
case Force_4_bytes_size_display_mode:
  return "Force_4_bytes_size_display_mode";
  }
 return NULL;
}
const char *getEgo_sensor_e(ego_sensor_e value){
switch(value) {
case ES_14Point7_Free:
  return "ES_14Point7_Free";
case ES_AEM:
  return "ES_AEM";
case ES_BPSX_D1:
  return "ES_BPSX_D1";
case ES_Custom:
  return "ES_Custom";
case ES_Innovate_MTX_L:
  return "ES_Innovate_MTX_L";
case ES_NarrowBand:
  return "ES_NarrowBand";
case ES_PLX:
  return "ES_PLX";
case Force_4_bytes_size_ego_sensor:
  return "Force_4_bytes_size_ego_sensor";
  }
 return NULL;
}
const char *getEngine_load_mode_e(engine_load_mode_e value){
switch(value) {
case Force_4_bytes_size_engine_load_mode:
  return "Force_4_bytes_size_engine_load_mode";
case LM_ALPHA_N:
  return "LM_ALPHA_N";
case LM_MOCK:
  return "LM_MOCK";
case LM_REAL_MAF:
  return "LM_REAL_MAF";
case LM_SPEED_DENSITY:
  return "LM_SPEED_DENSITY";
  }
 return NULL;
}
const char *getEngine_type_e(engine_type_e value){
switch(value) {
case BMW_M73_F:
  return "BMW_M73_F";
case BMW_M73_M:
  return "BMW_M73_M";
case BMW_M73_MRE:
  return "BMW_M73_MRE";
case BMW_M73_MRE_SLAVE:
  return "BMW_M73_MRE_SLAVE";
case BMW_M73_PROTEUS:
  return "BMW_M73_PROTEUS";
case CAMARO_4:
  return "CAMARO_4";
case CITROEN_TU3JP:
  return "CITROEN_TU3JP";
case DEFAULT_FRANKENSO:
  return "DEFAULT_FRANKENSO";
case DODGE_NEON_1995:
  return "DODGE_NEON_1995";
case DODGE_NEON_2003_CRANK:
  return "DODGE_NEON_2003_CRANK";
case DODGE_RAM:
  return "DODGE_RAM";
case ETB_BENCH_ENGINE:
  return "ETB_BENCH_ENGINE";
case FORD_ASPIRE_1996:
  return "FORD_ASPIRE_1996";
case FORD_ESCORT_GT:
  return "FORD_ESCORT_GT";
case FORD_INLINE_6_1995:
  return "FORD_INLINE_6_1995";
case FRANKENSO_QA_ENGINE:
  return "FRANKENSO_QA_ENGINE";
case Force_4_bytes_size_engine_type:
  return "Force_4_bytes_size_engine_type";
case GY6_139QMB:
  return "GY6_139QMB";
case HELLEN72_ETB:
  return "HELLEN72_ETB";
case HELLEN_121_NISSAN:
  return "HELLEN_121_NISSAN";
case HELLEN_121_VAG:
  return "HELLEN_121_VAG";
case HELLEN_121_VAG_5_CYL:
  return "HELLEN_121_VAG_5_CYL";
case HELLEN_121_VAG_6_CYL:
  return "HELLEN_121_VAG_6_CYL";
case HELLEN_121_VAG_8_CYL:
  return "HELLEN_121_VAG_8_CYL";
case HELLEN_128_MERCEDES:
  return "HELLEN_128_MERCEDES";
case HELLEN_134_BMW:
  return "HELLEN_134_BMW";
case HELLEN_154_VAG:
  return "HELLEN_154_VAG";
case HELLEN_55_BMW:
  return "HELLEN_55_BMW";
case HELLEN_88_BMW:
  return "HELLEN_88_BMW";
case HELLEN_NA6:
  return "HELLEN_NA6";
case HELLEN_NA94:
  return "HELLEN_NA94";
case HELLEN_NB2:
  return "HELLEN_NB2";
case HONDA_600:
  return "HONDA_600";
case HONDA_ACCORD_1_24_SHIFTED:
  return "HONDA_ACCORD_1_24_SHIFTED";
case HONDA_ACCORD_CD:
  return "HONDA_ACCORD_CD";
case HONDA_ACCORD_CD_DIP:
  return "HONDA_ACCORD_CD_DIP";
case HONDA_ACCORD_CD_TWO_WIRES:
  return "HONDA_ACCORD_CD_TWO_WIRES";
case LADA_KALINA:
  return "LADA_KALINA";
case MAZDA_626:
  return "MAZDA_626";
case MAZDA_MIATA_2003:
  return "MAZDA_MIATA_2003";
case MAZDA_MIATA_2003_BOARD_TEST:
  return "MAZDA_MIATA_2003_BOARD_TEST";
case MAZDA_MIATA_2003_NA_RAIL:
  return "MAZDA_MIATA_2003_NA_RAIL";
case MAZDA_MIATA_NA8:
  return "MAZDA_MIATA_NA8";
case MAZDA_MIATA_NB1:
  return "MAZDA_MIATA_NB1";
case MIATA_1990:
  return "MIATA_1990";
case MIATA_1996:
  return "MIATA_1996";
case MIATA_NA6_MAP:
  return "MIATA_NA6_MAP";
case MIATA_NA6_VAF:
  return "MIATA_NA6_VAF";
case MIATA_PROTEUS_TCU:
  return "MIATA_PROTEUS_TCU";
case MICRO_RUS_EFI:
  return "MICRO_RUS_EFI";
case MINIMAL_PINS:
  return "MINIMAL_PINS";
case MITSU_4G93:
  return "MITSU_4G93";
case MRE_BOARD_NEW_TEST:
  return "MRE_BOARD_NEW_TEST";
case MRE_BOARD_OLD_TEST:
  return "MRE_BOARD_OLD_TEST";
case MRE_BODY_CONTROL:
  return "MRE_BODY_CONTROL";
case MRE_M111:
  return "MRE_M111";
case MRE_MIATA_94_MAP:
  return "MRE_MIATA_94_MAP";
case MRE_MIATA_NA6_MAP:
  return "MRE_MIATA_NA6_MAP";
case MRE_MIATA_NA6_VAF:
  return "MRE_MIATA_NA6_VAF";
case MRE_MIATA_NB2_ETB:
  return "MRE_MIATA_NB2_ETB";
case MRE_MIATA_NB2_MAF:
  return "MRE_MIATA_NB2_MAF";
case MRE_MIATA_NB2_MAP:
  return "MRE_MIATA_NB2_MAP";
case MRE_SUBARU_EJ18:
  return "MRE_SUBARU_EJ18";
case NISSAN_PRIMERA:
  return "NISSAN_PRIMERA";
case PROMETHEUS_DEFAULTS:
  return "PROMETHEUS_DEFAULTS";
case PROTEUS_ANALOG_PWM_TEST:
  return "PROTEUS_ANALOG_PWM_TEST";
case PROTEUS_DEFAULTS:
  return "PROTEUS_DEFAULTS";
case PROTEUS_MIATA_NB2:
  return "PROTEUS_MIATA_NB2";
case PROTEUS_QC_TEST_BOARD:
  return "PROTEUS_QC_TEST_BOARD";
case SACHS:
  return "SACHS";
case SUBARUEG33_DEFAULTS:
  return "SUBARUEG33_DEFAULTS";
case SUBARUEJ20G_DEFAULTS:
  return "SUBARUEJ20G_DEFAULTS";
case SUBARU_2003_WRX:
  return "SUBARU_2003_WRX";
case TEST_33816:
  return "TEST_33816";
case TEST_CIVIC_4_0_BOTH:
  return "TEST_CIVIC_4_0_BOTH";
case TEST_CIVIC_4_0_RISE:
  return "TEST_CIVIC_4_0_RISE";
case TEST_ENGINE:
  return "TEST_ENGINE";
case TEST_ENGINE_VVT:
  return "TEST_ENGINE_VVT";
case TEST_ISSUE_366_BOTH:
  return "TEST_ISSUE_366_BOTH";
case TEST_ISSUE_366_RISE:
  return "TEST_ISSUE_366_RISE";
case TEST_ISSUE_898:
  return "TEST_ISSUE_898";
case TLE8888_BENCH_ENGINE:
  return "TLE8888_BENCH_ENGINE";
case TOYOTA_2JZ_GTE_VVTi:
  return "TOYOTA_2JZ_GTE_VVTi";
case TOYOTA_JZS147:
  return "TOYOTA_JZS147";
case VAG_18_TURBO:
  return "VAG_18_TURBO";
case VW_ABA:
  return "VW_ABA";
case VW_B6:
  return "VW_B6";
  }
 return NULL;
}
const char *getEtb_function_e(etb_function_e value){
switch(value) {
case ETB_IdleValve:
  return "ETB_IdleValve";
case ETB_None:
  return "ETB_None";
case ETB_Throttle1:
  return "ETB_Throttle1";
case ETB_Throttle2:
  return "ETB_Throttle2";
case ETB_Wastegate:
  return "ETB_Wastegate";
  }
 return NULL;
}
const char *getGear_e(gear_e value){
switch(value) {
case GEAR_1:
  return "GEAR_1";
case GEAR_2:
  return "GEAR_2";
case GEAR_3:
  return "GEAR_3";
case GEAR_4:
  return "GEAR_4";
case NEUTRAL:
  return "NEUTRAL";
case REVERSE:
  return "REVERSE";
  }
 return NULL;
}
const char *getGppwm_channel_e(gppwm_channel_e value){
switch(value) {
case GPPWM_AccelPedal:
  return "GPPWM_AccelPedal";
case GPPWM_AuxTemp1:
  return "GPPWM_AuxTemp1";
case GPPWM_AuxTemp2:
  return "GPPWM_AuxTemp2";
case GPPWM_Clt:
  return "GPPWM_Clt";
case GPPWM_FuelLoad:
  return "GPPWM_FuelLoad";
case GPPWM_Iat:
  return "GPPWM_Iat";
case GPPWM_IgnLoad:
  return "GPPWM_IgnLoad";
case GPPWM_Map:
  return "GPPWM_Map";
case GPPWM_Tps:
  return "GPPWM_Tps";
case GPPWM_Zero:
  return "GPPWM_Zero";
  }
 return NULL;
}
const char *getGppwm_compare_mode_e(gppwm_compare_mode_e value){
switch(value) {
case GPPWM_GreaterThan:
  return "GPPWM_GreaterThan";
case GPPWM_LessThan:
  return "GPPWM_LessThan";
  }
 return NULL;
}
const char *getIdle_mode_e(idle_mode_e value){
switch(value) {
case Force_4_bytes_size_idle_mode:
  return "Force_4_bytes_size_idle_mode";
case IM_AUTO:
  return "IM_AUTO";
case IM_MANUAL:
  return "IM_MANUAL";
  }
 return NULL;
}
const char *getIdle_state_e(idle_state_e value){
switch(value) {
case BLIP:
  return "BLIP";
case Force_4bytes_size_idle_state_e:
  return "Force_4bytes_size_idle_state_e";
case INIT:
  return "INIT";
case PID_UPPER:
  return "PID_UPPER";
case PID_VALUE:
  return "PID_VALUE";
case RPM_DEAD_ZONE:
  return "RPM_DEAD_ZONE";
case TPS_THRESHOLD:
  return "TPS_THRESHOLD";
  }
 return NULL;
}
const char *getIgnition_mode_e(ignition_mode_e value){
switch(value) {
case Force_4_bytes_size_ignition_mode:
  return "Force_4_bytes_size_ignition_mode";
case IM_INDIVIDUAL_COILS:
  return "IM_INDIVIDUAL_COILS";
case IM_ONE_COIL:
  return "IM_ONE_COIL";
case IM_TWO_COILS:
  return "IM_TWO_COILS";
case IM_WASTED_SPARK:
  return "IM_WASTED_SPARK";
  }
 return NULL;
}
const char *getInjection_mode_e(injection_mode_e value){
switch(value) {
case Force_4_bytes_size_injection_mode:
  return "Force_4_bytes_size_injection_mode";
case IM_BATCH:
  return "IM_BATCH";
case IM_SEQUENTIAL:
  return "IM_SEQUENTIAL";
case IM_SIMULTANEOUS:
  return "IM_SIMULTANEOUS";
case IM_SINGLE_POINT:
  return "IM_SINGLE_POINT";
  }
 return NULL;
}
const char *getInjector_compensation_mode_e(injector_compensation_mode_e value){
switch(value) {
case ICM_FixedRailPressure:
  return "ICM_FixedRailPressure";
case ICM_None:
  return "ICM_None";
case ICM_SensedRailPressure:
  return "ICM_SensedRailPressure";
  }
 return NULL;
}
const char *getInjector_pressure_type_e(injector_pressure_type_e value){
switch(value) {
case IPT_High:
  return "IPT_High";
case IPT_Low:
  return "IPT_Low";
  }
 return NULL;
}
const char *getLaunchActivationMode_e(launchActivationMode_e value){
switch(value) {
case ALWAYS_ACTIVE_LAUNCH:
  return "ALWAYS_ACTIVE_LAUNCH";
case CLUTCH_INPUT_LAUNCH:
  return "CLUTCH_INPUT_LAUNCH";
case Force_4bytes_size_launchActivationMode_e:
  return "Force_4bytes_size_launchActivationMode_e";
case SWITCH_INPUT_LAUNCH:
  return "SWITCH_INPUT_LAUNCH";
  }
 return NULL;
}
const char *getLog_format_e(log_format_e value){
switch(value) {
case Force_4_bytes_size_log_format:
  return "Force_4_bytes_size_log_format";
case LF_NATIVE:
  return "LF_NATIVE";
case LM_MLV:
  return "LM_MLV";
  }
 return NULL;
}
const char *getMaf_sensor_type_e(maf_sensor_type_e value){
switch(value) {
case Bosch0280218004:
  return "Bosch0280218004";
case Bosch0280218037:
  return "Bosch0280218037";
case CUSTOM:
  return "CUSTOM";
case DensoTODO:
  return "DensoTODO";
case Internal_ForceMyEnumIntSize_maf_sensor:
  return "Internal_ForceMyEnumIntSize_maf_sensor";
  }
 return NULL;
}
const char *getOperation_mode_e(operation_mode_e value){
switch(value) {
case FOUR_STROKE_CAM_SENSOR:
  return "FOUR_STROKE_CAM_SENSOR";
case FOUR_STROKE_CRANK_SENSOR:
  return "FOUR_STROKE_CRANK_SENSOR";
case FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR:
  return "FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR";
case FOUR_STROKE_THREE_TIMES_CRANK_SENSOR:
  return "FOUR_STROKE_THREE_TIMES_CRANK_SENSOR";
case Force_4_bytes_size_operation_mode_e:
  return "Force_4_bytes_size_operation_mode_e";
case OM_NONE:
  return "OM_NONE";
case TWO_STROKE:
  return "TWO_STROKE";
  }
 return NULL;
}
const char *getPin_input_mode_e(pin_input_mode_e value){
switch(value) {
case PI_DEFAULT:
  return "PI_DEFAULT";
case PI_PULLDOWN:
  return "PI_PULLDOWN";
case PI_PULLUP:
  return "PI_PULLUP";
  }
 return NULL;
}
const char *getPin_mode_e(pin_mode_e value){
switch(value) {
case PO_DEFAULT:
  return "PO_DEFAULT";
case PO_OPENDRAIN:
  return "PO_OPENDRAIN";
case PO_PULLDOWN:
  return "PO_PULLDOWN";
case PO_PULLUP:
  return "PO_PULLUP";
  }
 return NULL;
}
const char *getPin_output_mode_e(pin_output_mode_e value){
switch(value) {
case OM_DEFAULT:
  return "OM_DEFAULT";
case OM_INVERTED:
  return "OM_INVERTED";
case OM_OPENDRAIN:
  return "OM_OPENDRAIN";
case OM_OPENDRAIN_INVERTED:
  return "OM_OPENDRAIN_INVERTED";
  }
 return NULL;
}
const char *getSensor_chart_e(sensor_chart_e value){
switch(value) {
case Internal_ForceMyEnumIntSize_sensor_chart:
  return "Internal_ForceMyEnumIntSize_sensor_chart";
case SC_AUX_FAST1:
  return "SC_AUX_FAST1";
case SC_DETAILED_RPM:
  return "SC_DETAILED_RPM";
case SC_MAP:
  return "SC_MAP";
case SC_OFF:
  return "SC_OFF";
case SC_RPM_ACCEL:
  return "SC_RPM_ACCEL";
case SC_TRIGGER:
  return "SC_TRIGGER";
  }
 return NULL;
}
const char *getSpi_device_e(spi_device_e value){
switch(value) {
case SPI_DEVICE_1:
  return "SPI_DEVICE_1";
case SPI_DEVICE_2:
  return "SPI_DEVICE_2";
case SPI_DEVICE_3:
  return "SPI_DEVICE_3";
case SPI_DEVICE_4:
  return "SPI_DEVICE_4";
case SPI_NONE:
  return "SPI_NONE";
  }
 return NULL;
}
const char *getSpi_speed_e(spi_speed_e value){
switch(value) {
case _150KHz:
  return "_150KHz";
case _1_25MHz:
  return "_1_25MHz";
case _2_5MHz:
  return "_2_5MHz";
case _5MHz:
  return "_5MHz";
  }
 return NULL;
}
const char *getTChargeMode_e(tChargeMode_e value){
switch(value) {
case Force_4bytes_size_tChargeMode_e:
  return "Force_4bytes_size_tChargeMode_e";
case TCHARGE_MODE_AIR_INTERP:
  return "TCHARGE_MODE_AIR_INTERP";
case TCHARGE_MODE_RPM_TPS:
  return "TCHARGE_MODE_RPM_TPS";
  }
 return NULL;
}
const char *getTiming_mode_e(timing_mode_e value){
switch(value) {
case Internal_ForceMyEnumIntSize_timing_mode:
  return "Internal_ForceMyEnumIntSize_timing_mode";
case TM_DYNAMIC:
  return "TM_DYNAMIC";
case TM_FIXED:
  return "TM_FIXED";
  }
 return NULL;
}
const char *getTle8888_mode_e(tle8888_mode_e value){
switch(value) {
case TL_AUTO:
  return "TL_AUTO";
case TL_HALL:
  return "TL_HALL";
case TL_MANUAL:
  return "TL_MANUAL";
case TL_SEMI_AUTO:
  return "TL_SEMI_AUTO";
  }
 return NULL;
}
const char *getTrigger_event_e(trigger_event_e value){
switch(value) {
case SHAFT_3RD_FALLING:
  return "SHAFT_3RD_FALLING";
case SHAFT_3RD_RISING:
  return "SHAFT_3RD_RISING";
case SHAFT_PRIMARY_FALLING:
  return "SHAFT_PRIMARY_FALLING";
case SHAFT_PRIMARY_RISING:
  return "SHAFT_PRIMARY_RISING";
case SHAFT_SECONDARY_FALLING:
  return "SHAFT_SECONDARY_FALLING";
case SHAFT_SECONDARY_RISING:
  return "SHAFT_SECONDARY_RISING";
  }
 return NULL;
}
const char *getTrigger_type_e(trigger_type_e value){
switch(value) {
case Force_4_bytes_size_trigger_type:
  return "Force_4_bytes_size_trigger_type";
case TT_1_16:
  return "TT_1_16";
case TT_2JZ_1_12:
  return "TT_2JZ_1_12";
case TT_2JZ_3_34:
  return "TT_2JZ_3_34";
case TT_36_2_2_2:
  return "TT_36_2_2_2";
case TT_3_1_CAM:
  return "TT_3_1_CAM";
case TT_52:
  return "TT_52";
case TT_60_2_VW:
  return "TT_60_2_VW";
case TT_CHRYSLER_NGC_36_2_2:
  return "TT_CHRYSLER_NGC_36_2_2";
case TT_DAIHATSU:
  return "TT_DAIHATSU";
case TT_DODGE_NEON_1995:
  return "TT_DODGE_NEON_1995";
case TT_DODGE_NEON_1995_ONLY_CRANK:
  return "TT_DODGE_NEON_1995_ONLY_CRANK";
case TT_DODGE_NEON_2003_CAM:
  return "TT_DODGE_NEON_2003_CAM";
case TT_DODGE_NEON_2003_CRANK:
  return "TT_DODGE_NEON_2003_CRANK";
case TT_DODGE_RAM:
  return "TT_DODGE_RAM";
case TT_DODGE_STRATUS:
  return "TT_DODGE_STRATUS";
case TT_FIAT_IAW_P8:
  return "TT_FIAT_IAW_P8";
case TT_FORD_ASPIRE:
  return "TT_FORD_ASPIRE";
case TT_FORD_ST170:
  return "TT_FORD_ST170";
case TT_GM_60_2_2_2:
  return "TT_GM_60_2_2_2";
case TT_GM_7X:
  return "TT_GM_7X";
case TT_GM_LS_24:
  return "TT_GM_LS_24";
case TT_HONDA_1_24:
  return "TT_HONDA_1_24";
case TT_HONDA_1_4_24:
  return "TT_HONDA_1_4_24";
case TT_HONDA_4_24:
  return "TT_HONDA_4_24";
case TT_HONDA_4_24_1:
  return "TT_HONDA_4_24_1";
case TT_HONDA_ACCORD_1_24_SHIFTED:
  return "TT_HONDA_ACCORD_1_24_SHIFTED";
case TT_HONDA_CBR_600:
  return "TT_HONDA_CBR_600";
case TT_HONDA_K_12_1:
  return "TT_HONDA_K_12_1";
case TT_JEEP_18_2_2_2:
  return "TT_JEEP_18_2_2_2";
case TT_JEEP_4_CYL:
  return "TT_JEEP_4_CYL";
case TT_KAWA_KX450F:
  return "TT_KAWA_KX450F";
case TT_MAZDA_DOHC_1_4:
  return "TT_MAZDA_DOHC_1_4";
case TT_MAZDA_MIATA_NA:
  return "TT_MAZDA_MIATA_NA";
case TT_MAZDA_MIATA_NB1:
  return "TT_MAZDA_MIATA_NB1";
case TT_MAZDA_MIATA_VVT_TEST:
  return "TT_MAZDA_MIATA_VVT_TEST";
case TT_MAZDA_SOHC_4:
  return "TT_MAZDA_SOHC_4";
case TT_MIATA_VVT:
  return "TT_MIATA_VVT";
case TT_MITSUBISHI:
  return "TT_MITSUBISHI";
case TT_NISSAN_SR20VE:
  return "TT_NISSAN_SR20VE";
case TT_NISSAN_SR20VE_360:
  return "TT_NISSAN_SR20VE_360";
case TT_ONE:
  return "TT_ONE";
case TT_ONE_PLUS_ONE:
  return "TT_ONE_PLUS_ONE";
case TT_RENIX_44_2_2:
  return "TT_RENIX_44_2_2";
case TT_RENIX_66_2_2_2:
  return "TT_RENIX_66_2_2_2";
case TT_ROVER_K:
  return "TT_ROVER_K";
case TT_SKODA_FAVORIT:
  return "TT_SKODA_FAVORIT";
case TT_SUBARU_7_6:
  return "TT_SUBARU_7_6";
case TT_SUBARU_7_WITHOUT_6:
  return "TT_SUBARU_7_WITHOUT_6";
case TT_SUBARU_SVX:
  return "TT_SUBARU_SVX";
case TT_TOOTHED_WHEEL:
  return "TT_TOOTHED_WHEEL";
case TT_TOOTHED_WHEEL_36_1:
  return "TT_TOOTHED_WHEEL_36_1";
case TT_TOOTHED_WHEEL_36_2:
  return "TT_TOOTHED_WHEEL_36_2";
case TT_TOOTHED_WHEEL_60_2:
  return "TT_TOOTHED_WHEEL_60_2";
case TT_TRI_TACH:
  return "TT_TRI_TACH";
case TT_UNUSED:
  return "TT_UNUSED";
case TT_VVT_BARRA_3_PLUS_1:
  return "TT_VVT_BARRA_3_PLUS_1";
case TT_VVT_BOSCH_QUICK_START:
  return "TT_VVT_BOSCH_QUICK_START";
case TT_VVT_JZ:
  return "TT_VVT_JZ";
case TT_VVT_MIATA_NB2:
  return "TT_VVT_MIATA_NB2";
  }
 return NULL;
}
const char *getTrigger_value_e(trigger_value_e value){
switch(value) {
case TV_FALL:
  return "TV_FALL";
case TV_RISE:
  return "TV_RISE";
  }
 return NULL;
}
const char *getTrigger_wheel_e(trigger_wheel_e value){
switch(value) {
case T_CHANNEL_3:
  return "T_CHANNEL_3";
case T_NONE:
  return "T_NONE";
case T_PRIMARY:
  return "T_PRIMARY";
case T_SECONDARY:
  return "T_SECONDARY";
  }
 return NULL;
}
const char *getUart_device_e(uart_device_e value){
switch(value) {
case UART_DEVICE_1:
  return "UART_DEVICE_1";
case UART_DEVICE_2:
  return "UART_DEVICE_2";
case UART_DEVICE_3:
  return "UART_DEVICE_3";
case UART_DEVICE_4:
  return "UART_DEVICE_4";
case UART_NONE:
  return "UART_NONE";
  }
 return NULL;
}
const char *getVe_override_e(ve_override_e value){
switch(value) {
case VE_MAP:
  return "VE_MAP";
case VE_None:
  return "VE_None";
case VE_TPS:
  return "VE_TPS";
  }
 return NULL;
}
const char *getVvt_mode_e(vvt_mode_e value){
switch(value) {
case VVT_2JZ:
  return "VVT_2JZ";
case VVT_4_1:
  return "VVT_4_1";
case VVT_BARRA_3_PLUS_1:
  return "VVT_BARRA_3_PLUS_1";
case VVT_BOSCH_QUICK_START:
  return "VVT_BOSCH_QUICK_START";
case VVT_FIRST_HALF:
  return "VVT_FIRST_HALF";
case VVT_FORD_ST170:
  return "VVT_FORD_ST170";
case VVT_INACTIVE:
  return "VVT_INACTIVE";
case VVT_MIATA_NB2:
  return "VVT_MIATA_NB2";
case VVT_SECOND_HALF:
  return "VVT_SECOND_HALF";
  }
 return NULL;
}
