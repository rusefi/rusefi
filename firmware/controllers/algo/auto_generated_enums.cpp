// auto-generated from../../firmware/\controllers/algo/io_pins.h
// by enum2string.jar tool

#include "main.h"

// auto-generated from../../firmware/\controllers/algo/rusefi_enums.h
// by enum2string.jar tool

#include "main.h"
#include "rusefi_enums.h"

const char *getVvt_mode_e(vvt_mode_e value) {
	switch(value) {
	case MIATA_NB2:
		return "MIATA_NB2";

	default: return "no_test";
	}
	return NULL;
}


const char *getTiming_mode_e(timing_mode_e value) {
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

const char *getEngine_type_e(engine_type_e value){
switch(value) {
case MIATA_NA_1_6:
	return "MIATA_NA_1_6";
case ZIL_130:
	return "ZIL_130";
case HONDA_600:
	return "Honda 600";
case ACURA_RSX:
	return "ACURA_RSX";
case AUDI_AAN:
  return "AUDI_AAN";
case SACHS:
  return "SACHS";
case DAIHATSU:
	return "DAIHATSU";
case SUZUKI_VITARA:
	return "SUZUKI_VITARA";
case CAMARO_4:
	return "CAMARO_4";
case CHEVY_C20_1973:
	return "CHEVY C20";
case TOYOTA_JZS147:
	return "TOYOTA_JZS147";
case TOYOTA_2JZ_GTE_VVTi:
	return "2JZ_GTE_VVTi";
case GEO_STORM:
	return "GEO_STORM";
case LADA_KALINA:
	return "LADA_KALINA";
case GM_2_2:
  return "GM_2_2";
case DODGE_RAM:
  return "DODGE_RAM";
case BMW_E34:
  return "BMW_E34";
case CITROEN_TU3JP:
  return "CITROEN_TU3JP";
case CUSTOM_ENGINE:
  return "CUSTOM_ENGINE";
case DODGE_NEON_1995:
  return "DODGE_NEON_1995";
case DODGE_NEON_2003_CAM:
  return "NEON_2003_CAM";
case DODGE_NEON_2003_CRANK:
  return "NEON_2003_CRANK";
case FORD_ASPIRE_1996:
  return "FORD_ASPIRE_1996";
case FORD_ESCORT_GT:
  return "FORD_ESCORT_GT";
case FORD_FIESTA:
  return "FORD_FIESTA";
case FORD_INLINE_6_1995:
  return "FORD_INLINE_6_1995";
case Force_4b_engine_type:
  return "Force_4b_engine_type";
case GY6_139QMB:
  return "GY6_139QMB";
case HONDA_ACCORD_CD:
  return "HONDA_ACCORD_CD";
case HONDA_ACCORD_CD_DIP:
  return "HONDA_ACCORD_CD_DIP";
case HONDA_ACCORD_CD_TWO_WIRES:
  return "HONDA_ACCORD_CD_TWO_WIRES";
case HONDA_ACCORD_1_24_SHIFTED:
  return "HONDA_ACCORD_1_24_SHIFTED";
case MAZDA_323:
  return "MAZDA_323";
case FRANKENSO_QA_ENGINE:
	return "QA";
case MAZDA_626:
  return "MAZDA_626";
case MAZDA_MIATA_NB1:
  return "MAZDA_MIATA_NB1";
case MIATA_1990:
  return "MIATA_1990";
case MIATA_1994_DEVIATOR:
  return "MIATA_1994_DEVIATOR";
case MIATA_1994_SPAGS:
  return "MIATA_1994_SPAGS";
case MIATA_1996:
  return "MIATA_1996";
case MINI_COOPER_R50:
  return "MINI_COOPER_R50";
case MITSU_4G93:
  return "MITSU_4G93";
case NISSAN_PRIMERA:
  return "NISSAN_PRIMERA";
case ROVER_V8:
  return "ROVER_V8";
case SATURN_ION_2004:
  return "SATURN_ION_2004";
case SUBARU_2003_WRX:
  return "SUBARU_2003_WRX";
case TEST_ENGINE:
  return "TEST_ENGINE";
case MAZDA_MIATA_2003:
	return "miata 2003";
case MAZDA_MIATA_2003_NA_RAIL:
	return "miata 2003 NArail";
case MAZDA_MIATA_2003_BOARD_TEST:
	return "miata 2003 bt";
case PROMETHEUS_DEFAULTS:
	return "PROMETHEUS";
case TEST_ENGINE_VVT:
  return "TEST_ENGINE_VVT";
case VW_ABA:
  return "VW_ABA";
case DODGE_STRATUS:
	return "DODGE_STRATUS";
case TEST_CIVIC_4_0_BOTH:
case TEST_CIVIC_4_0_RISE:
	return "civictest";
case TEST_ISSUE_366_BOTH:
case TEST_ISSUE_366_RISE:
	return "unittest";
  }
 return NULL;
}
const char *getTrigger_type_e(trigger_type_e value){
switch(value) {
case Force_4b_trigger_type:
  return "Force_4b_trigger_type";
case TT_HONDA_CBR_600:
	return "TT_HONDA_CBR_600";
case TT_HONDA_CBR_600_CUSTOM:
	return "TT_HONDA_CBR_600_CUSTOM";
case TT_3_1_CAM:
	return "3_1_CAM";
case TT_MIATA_VVT:
	return "MIATA_VVT";
case TT_DODGE_NEON_1995:
  return "TT_DODGE_NEON_1995";
case TT_GM_LS_24:
	return "TT_GM_LS_24";
case TT_DODGE_NEON_2003_CAM:
  return "TT_NEON_2003_CAM";
case TT_DODGE_NEON_2003_CRANK:
  return "TT_NEON_2003_CRANK";
case TT_FORD_ASPIRE:
  return "TT_FORD_ASPIRE";
case TT_MAZDA_SOHC_4:
  return "TT_MAZDA_SOHC_4";
case TT_MAZDA_DOHC_1_4:
  return "TT_MAZDA_DOHC_1_4";
case TT_ONE_PLUS_ONE:
  return "TT_ONE_PLUS_ONE";
case TT_GM_7X:
  return "TT_GM_7X";
case TT_HONDA_4_24_1:
  return "TT_HONDA_4_24_1";
case TT_HONDA_1_24:
	return "TT_HONDA_1_24";
case TT_HONDA_ACCORD_1_24_SHIFTED:
	return "ACCORD_1_24_SHIFTED";
case TT_DODGE_STRATUS:
	return "TT_DODGE_STRATUS";
case TT_36_2_2_2:
	return "TT_36_2_2_2";
case TT_HONDA_1_4_24:
  return "TT_HONDA_1_4_24";
case TT_HONDA_4_24:
  return "TT_HONDA_4_24";
case TT_MAZDA_MIATA_NA:
  return "TT_MAZDA_MIATA_NA";
case TT_MAZDA_MIATA_NB1:
  return "TT_MAZDA_MIATA_NB1";
case TT_MAZDA_MIATA_VVT_TEST:
	return "TT_MAZDA_MIATA_VVT_TEST";
case TT_SUBARU_7_6:
	return "TT_SUBARU_7_6";
case TT_JEEP_18_2_2_2:
	return "TT_18_2_2_2";
case TT_MINI_COOPER_R50:
  return "TT_MINI_COOPER_R50";
case TT_MITSUBISHI:
  return "TT_MITSUBISHI";
case TT_TOOTHED_WHEEL:
  return "TT_TOOTHED_WHEEL";
case TT_TOOTHED_WHEEL_36_1:
  return "TT_TOOTHED_WHEEL_36_1";
case TT_TOOTHED_WHEEL_60_2:
  return "TT_TOOTHED_WHEEL_60_2";
case TT_ONE_PLUS_TOOTHED_WHEEL_60_2:
	return "TT_ONE_PLUS_TOOTHED_WHEEL_60_2";
case TT_ONE:
	return "TT_ONE";
case TT_60_2_VW:
	return "TT_60_2_VW";
case TT_DODGE_RAM:
	return "TT_DODGE_RAM";
case TT_UNUSED:
	return "TT_UNUSED";
case TT_NISSAN_SR20VE:
	return "TT_NISSAN_SR20VE";
case TT_2JZ_1_12:
	return "TT_2JZ_1_12";
case TT_2JZ_3_34:
	return "TT_2JZ_3_34";
case TT_ROVER_K:
	return "TT_ROVER_K";
case TT_NISSAN_SR20VE_360:
	return "TT_NISSAN_SR20VE_360";
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
case Force_4b_adc_channel_mode:
  return "Force_4b_adc_channel_mode";
  }
 return NULL;
}
const char *getTrigger_value_e(trigger_value_e value){
switch(value) {
case TV_RISE:
  return "TV_RISE";
case TV_FALL:
  return "TV_FALL";
  }
 return NULL;
}
const char *getTrigger_wheel_e(trigger_wheel_e value){
switch(value) {
case T_CHANNEL_3:
  return "T_CHANNEL_3";
case T_PRIMARY:
  return "T_PRIMARY";
case T_SECONDARY:
  return "T_SECONDARY";
case T_NONE:
	return "T_NONE";
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
const char *getEngine_load_mode_e(engine_load_mode_e value){
switch(value) {
case Force_4b_engine_load_mode:
  return "Force_4b_engine_load_mode";
case LM_ALPHA_N:
  return "LM_ALPHA_N";
case LM_PLAIN_MAF:
  return "LM_PLAIN_MAF";
case LM_REAL_MAF:
  return "LM_REAL_MAF";
case LM_MAP:
  return "LM_MAP";
case LM_SPEED_DENSITY:
  return "LM_SPEED_DENSITY";
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
case Force_4b_display_mode:
  return "Force_4b_display_mode";
  }
 return NULL;
}
const char *getLog_format_e(log_format_e value){
switch(value) {
case Force_4b_log_format:
  return "Force_4b_log_format";
case LF_NATIVE:
  return "LF_NATIVE";
case LM_MLV:
  return "LM_MLV";
  }
 return NULL;
}
const char *getIdle_mode_e(idle_mode_e value){
switch(value) {
case Force_4b_idle_mode:
  return "Force_4b_idle_mode";
case IM_AUTO:
  return "IM_AUTO";
case IM_MANUAL:
  return "IM_MANUAL";
  }
 return NULL;
}

const char *getPin_output_mode_e(pin_output_mode_e value){
switch(value) {
case Force_4b_pin_output_mode:
  return "Force_4b_pin_output_mode";
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
const char *getGpio_mode_e(gpio_mode_e value){
switch(value) {
case Force_4b_gpio_mode:
  return "Force_4b_gpio_mode";
  }
 return NULL;
}
const char *getPin_input_mode_e(pin_input_mode_e value){
switch(value) {
case Force_4b_pin_input_mode:
  return "Force_4b_pin_input_mode";
case PI_DEFAULT:
  return "PI_DEFAULT";
case PI_PULLDOWN:
  return "PI_PULLDOWN";
case PI_PULLUP:
  return "PI_PULLUP";
  }
 return NULL;
}
const char *getFiring_order_e(firing_order_e value){
switch(value) {
case FO_1_2_4_5_3:
  return "FO_1_2_4_5_3";
case FO_1_2:
  return "FO_1_THEN_2";
case FO_1_8_4_3_6_5_7_2:
  return "FO_1_8_4_3_6_5_7_2";
case FO_1_2_4_3:
  return "FO_1_THEN_2_THEN_4_THEN3";
case FO_1_3_2_4:
  return "FO_1_THEN_3_THEN_2_THEN4";
case FO_1_3_4_2:
  return "FO_1_THEN_3_THEN_4_THEN2";
case FO_1_5_3_6_2_4:
  return "FO_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4";
case FO_1_4_2_5_3_6:
  return "FO_1_THEN_4_THEN_2_THEN_5_THEN_3_THEN_6";
case FO_1_2_3_4_5_6:
	return "FO_1_THEN_2_THEN_3_THEN_4_THEN_5_THEN_6";
case FO_1:
  return "FO_ONE_CYLINDER";
case FO_1_6_3_2_5_4:
	return "FO_1_6_3_2_5_4";
case FO_1_2_3:
	return "FO_1_2_3";
case FO_1_8_7_2_6_5_4_3:
	return "FO_1_8_7_2_6_5_4_3";
case FO_1_5_4_2_6_3_7_8:
	return "FO_1_5_4_2_6_3_7_8";
case FO_1_10_9_4_3_6_5_8_7_2:
	return "FO_1_10_9_4_3_6_5_8_7_2";
case FO_1_7_5_11_3_9_6_12_2_8_4_10:
	return "FO_1_7_5_11_3_9_6_12_2_8_4_10";
case FO_1_7_4_10_2_8_6_12_3_9_5_11:
	return "FO_1_7_4_10_2_8_6_12_3_9_5_11";
case Force_4b_firing_order:
  return "Force_4b_firing_order";
  }
 return NULL;
}
const char *getOperation_mode_e(operation_mode_e value){
switch(value) {
case FOUR_STROKE_CAM_SENSOR:
  return "FOUR_STROKE_CAM_SENSOR";
case FOUR_STROKE_CRANK_SENSOR:
  return "FOUR_STROKE_CRANK_SENSOR";
case TWO_STROKE:
  return "TWO_STROKE";
case FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR:
  return "FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR";
case Force_4b_operation_mode_e:
  return "Force_4b_operation_mode_e";
case OM_NONE:
  return "OM_NONE";
  }
 return NULL;
}
const char *getIgnition_mode_e(ignition_mode_e value){
switch(value) {
case Force_4b_ignition_mode:
  return "Force_4b_ignition_mode";
case IM_INDIVIDUAL_COILS:
  return "IM_INDIVIDUAL_COILS";
case IM_ONE_COIL:
  return "IM_ONE_COIL";
case IM_WASTED_SPARK:
  return "IM_WASTED_SPARK";
  }
 return NULL;
}
const char *getInjection_mode_e(injection_mode_e value){
switch(value) {
case Force_4b_injection_mode:
  return "Force_4b_injection_mode";
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
const char *getCranking_ignition_mode_e(cranking_ignition_mode_e value){
switch(value) {
case CIM_DEFAULT:
  return "CIM_DEFAULT";
case CIM_FIXED_ANGLE:
  return "CIM_FIXED_ANGLE";
case Force_4b_cranking_ignition_mode:
  return "Force_4b_cranking_ignition_mode";
  }
 return NULL;
}
const char *getSpi_device_e(spi_device_e value){
switch(value) {
case Force_4b_spi_device:
  return "Force_4b_spi_device";
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
case EFI_ADC_2:
  return "EFI_ADC_2";
case EFI_ADC_3:
  return "EFI_ADC_3";
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
case EFI_ADC_NONE:
  return "EFI_ADC_NONE";
case Force_4b_cranking_adc_channel:
  return "Force_4b_cranking_adc_channel";
  }
 return NULL;
}
const char *getEgo_sensor_e(ego_sensor_e value){
switch(value) {
case ES_14Point7_Free:
  return "ES_14Point7_Free";
case ES_BPSX_D1:
  return "ES_BPSX_D1";
case ES_Innovate_MTX_L:
  return "ES_Innovate_MTX_L";
case ES_NarrowBand:
  return "ES_NarrowBand";
case ES_PLX:
	return "ES_PLX";
case ES_Custom:
	return "ES_Custom";
case Force_4b_ego_sensor:
  return "Force_4b_ego_sensor";
  }
 return NULL;
}
const char *getBrain_pin_e(brain_pin_e value){
switch(value) {
case Force_4b_brain_pin_e:
  return "Force_4b_brain_pin_e";
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
case GPIO_INVALID:
  return "GPIO_INVALID";
case GPIO_UNASSIGNED:
  return "GPIO_UNASSIGNED";
  }
 return NULL;
}
const char *getAir_pressure_sensor_type_e(air_pressure_sensor_type_e value){
switch(value) {
case Force_4b_cranking_map_type:
  return "Force_4b_cranking_map_type";
case MT_CUSTOM:
  return "MT_CUSTOM";
case MT_DENSO183:
  return "MT_DENSO183";
case MT_DODGE_NEON_2003:
  return "MT_DODGE_NEON_2003";
case MT_HONDA3BAR:
  return "MT_HONDA3BAR";
case MT_MPX4250:
  return "MT_MPX4250";
case MT_SUBY_DENSO:
  return "MT_SUBY_DENSO";
case MT_GM_3_BAR:
  return "MT_GM_3_BAR";
case MT_MPX4100:
  return "MT_MPX4100";
case MT_TOYOTA_89420_02010:
	return "MT_TOYOTA_89420_02010";
  }
 return NULL;
}
