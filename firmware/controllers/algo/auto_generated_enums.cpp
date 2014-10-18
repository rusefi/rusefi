// auto-generated from../../firmware/controllers/algo/io_pins.h


#include "main.h"
#include "io_pins.h"
const char *getIo_pin_e(io_pin_e value){
switch(value) {
case SPARKOUT_1_OUTPUT:
  return "SPARKOUT_1_OUTPUT";
case LED_HUGE_20:
  return "LED_HUGE_20";
case SPARKOUT_10_OUTPUT:
  return "SPARKOUT_10_OUTPUT";
case LED_EMULATOR:
  return "LED_EMULATOR";
case GPIO_1:
  return "GPIO_1";
case GPIO_0:
  return "GPIO_0";
case LED_WARNING:
  return "LED_WARNING";
case GPIO_3:
  return "GPIO_3";
case GPIO_2:
  return "GPIO_2";
case GPIO_5:
  return "GPIO_5";
case INJECTOR_3_OUTPUT:
  return "INJECTOR_3_OUTPUT";
case GPIO_4:
  return "GPIO_4";
case LED_HUGE_19:
  return "LED_HUGE_19";
case IDLE_VALVE:
  return "IDLE_VALVE";
case GPIO_7:
  return "GPIO_7";
case FAN_RELAY:
  return "FAN_RELAY";
case INJECTOR_8_OUTPUT:
  return "INJECTOR_8_OUTPUT";
case GPIO_6:
  return "GPIO_6";
case GPIO_9:
  return "GPIO_9";
case LED_HUGE_16:
  return "LED_HUGE_16";
case SPARKOUT_9_OUTPUT:
  return "SPARKOUT_9_OUTPUT";
case LED_EXT_1:
  return "LED_EXT_1";
case GPIO_8:
  return "GPIO_8";
case LED_HUGE_15:
  return "LED_HUGE_15";
case LED_HUGE_18:
  return "LED_HUGE_18";
case LED_HUGE_17:
  return "LED_HUGE_17";
case LED_HUGE_12:
  return "LED_HUGE_12";
case LED_HUGE_11:
  return "LED_HUGE_11";
case LED_HUGE_14:
  return "LED_HUGE_14";
case LED_HUGE_13:
  return "LED_HUGE_13";
case LED_HUGE_10:
  return "LED_HUGE_10";
case LED_EXT_2:
  return "LED_EXT_2";
case LED_EXT_3:
  return "LED_EXT_3";
case INJECTOR_5_OUTPUT:
  return "INJECTOR_5_OUTPUT";
case MAIN_RELAY:
  return "MAIN_RELAY";
case LED_CHECK_ENGINE:
  return "LED_CHECK_ENGINE";
case TRIGGER_EMULATOR_SECONDARY:
  return "TRIGGER_EMULATOR_SECONDARY";
case INJECTOR_9_OUTPUT:
  return "INJECTOR_9_OUTPUT";
case LED_DEBUG:
  return "LED_DEBUG";
case IO_INVALID:
  return "IO_INVALID";
case INJECTOR_1_OUTPUT:
  return "INJECTOR_1_OUTPUT";
case INJECTOR_4_OUTPUT:
  return "INJECTOR_4_OUTPUT";
case SPARKOUT_3_OUTPUT:
  return "SPARKOUT_3_OUTPUT";
case O2_HEATER:
  return "O2_HEATER";
case LED_RUNNING:
  return "LED_RUNNING";
case TRIGGER_EMULATOR_3RD:
  return "TRIGGER_EMULATOR_3RD";
case INJECTOR_10_OUTPUT:
  return "INJECTOR_10_OUTPUT";
case INJECTOR_6_OUTPUT:
  return "INJECTOR_6_OUTPUT";
case SPI_CS_1:
  return "SPI_CS_1";
case SPI_CS_3:
  return "SPI_CS_3";
case SPI_CS_2:
  return "SPI_CS_2";
case ELECTRONIC_THROTTLE_CONTROL_2:
  return "ELECTRONIC_THROTTLE_CONTROL_2";
case ELECTRONIC_THROTTLE_CONTROL_3:
  return "ELECTRONIC_THROTTLE_CONTROL_3";
case LED_HUGE_9:
  return "LED_HUGE_9";
case LED_COMMUNICATION_1:
  return "LED_COMMUNICATION_1";
case LED_HUGE_8:
  return "LED_HUGE_8";
case INJECTOR_7_OUTPUT:
  return "INJECTOR_7_OUTPUT";
case ELECTRONIC_THROTTLE_CONTROL_1:
  return "ELECTRONIC_THROTTLE_CONTROL_1";
case LED_HUGE_7:
  return "LED_HUGE_7";
case SPARKOUT_6_OUTPUT:
  return "SPARKOUT_6_OUTPUT";
case LED_HUGE_6:
  return "LED_HUGE_6";
case LED_HUGE_5:
  return "LED_HUGE_5";
case LED_HUGE_4:
  return "LED_HUGE_4";
case LED_HUGE_3:
  return "LED_HUGE_3";
case SPI_CS_SD_MODULE:
  return "SPI_CS_SD_MODULE";
case SPI_CS_4:
  return "SPI_CS_4";
case LED_HUGE_2:
  return "LED_HUGE_2";
case SPARKOUT_4_OUTPUT:
  return "SPARKOUT_4_OUTPUT";
case LED_HUGE_1:
  return "LED_HUGE_1";
case FUEL_PUMP_RELAY:
  return "FUEL_PUMP_RELAY";
case SPARKOUT_7_OUTPUT:
  return "SPARKOUT_7_OUTPUT";
case LED_HUGE_0:
  return "LED_HUGE_0";
case GPIO_15:
  return "GPIO_15";
case TRIGGER_EMULATOR_PRIMARY:
  return "TRIGGER_EMULATOR_PRIMARY";
case SPARKOUT_8_OUTPUT:
  return "SPARKOUT_8_OUTPUT";
case SPARKOUT_11_OUTPUT:
  return "SPARKOUT_11_OUTPUT";
case GPIO_11:
  return "GPIO_11";
case INJECTOR_11_OUTPUT:
  return "INJECTOR_11_OUTPUT";
case GPIO_12:
  return "GPIO_12";
case GPIO_13:
  return "GPIO_13";
case GPIO_14:
  return "GPIO_14";
case LED_ERROR:
  return "LED_ERROR";
case INJECTOR_12_OUTPUT:
  return "INJECTOR_12_OUTPUT";
case INJECTOR_2_OUTPUT:
  return "INJECTOR_2_OUTPUT";
case ALTERNATOR_SWITCH:
  return "ALTERNATOR_SWITCH";
case SPARKOUT_12_OUTPUT:
  return "SPARKOUT_12_OUTPUT";
case SPARKOUT_2_OUTPUT:
  return "SPARKOUT_2_OUTPUT";
case SPARKOUT_5_OUTPUT:
  return "SPARKOUT_5_OUTPUT";
case GPIO_10:
  return "GPIO_10";
  }
 return NULL;
}
// auto-generated from../../firmware/controllers/algo/rusefi_enums.h


#include "main.h"
#include "rusefi_enums.h"
const char *getEngine_type_e(engine_type_e value){
switch(value) {
case SATURN_ION_2004:
  return "SATURN_ION_2004";
case CITROEN_TU3JP:
  return "CITROEN_TU3JP";
case FORD_FIESTA:
  return "FORD_FIESTA";
case ET_UNUSED:
  return "ET_UNUSED";
case SUBARU_2003_WRX:
  return "SUBARU_2003_WRX";
case HONDA_ACCORD_CD_DIP:
  return "HONDA_ACCORD_CD_DIP";
case NISSAN_PRIMERA:
  return "NISSAN_PRIMERA";
case FORD_ESCORT_GT:
  return "FORD_ESCORT_GT";
case MAZDA_323:
  return "MAZDA_323";
case Force_4b_engine_type:
  return "Force_4b_engine_type";
case MITSU_4G93:
  return "MITSU_4G93";
case DODGE_NEON_2003:
  return "DODGE_NEON_2003";
case HONDA_ACCORD_CD:
  return "HONDA_ACCORD_CD";
case DODGE_NEON_1995:
  return "DODGE_NEON_1995";
case HONDA_ACCORD_CD_TWO_WIRES:
  return "HONDA_ACCORD_CD_TWO_WIRES";
case FORD_ASPIRE_1996:
  return "FORD_ASPIRE_1996";
case MINI_COOPER_R50:
  return "MINI_COOPER_R50";
case AUDI_AAN:
  return "AUDI_AAN";
case MIATA_1996:
  return "MIATA_1996";
case MIATA_1994:
  return "MIATA_1994";
case FORD_INLINE_6_1995:
  return "FORD_INLINE_6_1995";
case MIATA_1990:
  return "MIATA_1990";
case ROVER_V8:
  return "ROVER_V8";
case GY6_139QMB:
  return "GY6_139QMB";
case MAZDA_MIATA_NB:
  return "MAZDA_MIATA_NB";
  }
 return NULL;
}
const char *getTrigger_type_e(trigger_type_e value){
switch(value) {
case TT_HONDA_ACCORD_CD:
  return "TT_HONDA_ACCORD_CD";
case TT_MINI_COOPER_R50:
  return "TT_MINI_COOPER_R50";
case TT_FORD_ASPIRE:
  return "TT_FORD_ASPIRE";
case TT_TOOTHED_WHEEL:
  return "TT_TOOTHED_WHEEL";
case TT_TOOTHED_WHEEL_60_2:
  return "TT_TOOTHED_WHEEL_60_2";
case TT_HONDA_ACCORD_CD_DIP:
  return "TT_HONDA_ACCORD_CD_DIP";
case TT_GM_7X:
  return "TT_GM_7X";
case TT_TOOTHED_WHEEL_36_1:
  return "TT_TOOTHED_WHEEL_36_1";
case TT_MITSU:
  return "TT_MITSU";
case TT_HONDA_ACCORD_CD_TWO_WIRES:
  return "TT_HONDA_ACCORD_CD_TWO_WIRES";
case TT_MAZDA_MIATA_NA:
  return "TT_MAZDA_MIATA_NA";
case TT_FORD_ESCORT_GT:
  return "TT_FORD_ESCORT_GT";
case TT_MAZDA_MIATA_NB:
  return "TT_MAZDA_MIATA_NB";
case TT_DODGE_NEON_1995:
  return "TT_DODGE_NEON_1995";
case TT_DODGE_NEON_2003:
  return "TT_DODGE_NEON_2003";
case Force_4b_trigger_type:
  return "Force_4b_trigger_type";
  }
 return NULL;
}
const char *getAdc_channel_mode_e(adc_channel_mode_e value){
switch(value) {
case Force_4b_adc_channel_mode:
  return "Force_4b_adc_channel_mode";
case ADC_FAST:
  return "ADC_FAST";
case ADC_OFF:
  return "ADC_OFF";
case ADC_SLOW:
  return "ADC_SLOW";
  }
 return NULL;
}
const char *getTrigger_value_e(trigger_value_e value){
switch(value) {
case TV_LOW:
  return "TV_LOW";
case TV_HIGH:
  return "TV_HIGH";
  }
 return NULL;
}
const char *getTrigger_wheel_e(trigger_wheel_e value){
switch(value) {
case T_SECONDARY:
  return "T_SECONDARY";
case T_CHANNEL_3:
  return "T_CHANNEL_3";
case T_PRIMARY:
  return "T_PRIMARY";
  }
 return NULL;
}
const char *getTrigger_event_e(trigger_event_e value){
switch(value) {
case SHAFT_3RD_UP:
  return "SHAFT_3RD_UP";
case SHAFT_SECONDARY_DOWN:
  return "SHAFT_SECONDARY_DOWN";
case SHAFT_SECONDARY_UP:
  return "SHAFT_SECONDARY_UP";
case SHAFT_PRIMARY_UP:
  return "SHAFT_PRIMARY_UP";
case SHAFT_PRIMARY_DOWN:
  return "SHAFT_PRIMARY_DOWN";
case SHAFT_3RD_DOWN:
  return "SHAFT_3RD_DOWN";
  }
 return NULL;
}
const char *getEngine_load_mode_e(engine_load_mode_e value){
switch(value) {
case Force_4b_engine_load_mode:
  return "Force_4b_engine_load_mode";
case LM_MAF:
  return "LM_MAF";
case LM_ALPHA_N:
  return "LM_ALPHA_N";
case LM_MAP:
  return "LM_MAP";
case LM_SPEED_DENSITY:
  return "LM_SPEED_DENSITY";
  }
 return NULL;
}
const char *getDisplay_mode_e(display_mode_e value){
switch(value) {
case DM_HD44780_OVER_PCF8574:
  return "DM_HD44780_OVER_PCF8574";
case Force_4b_display_mode:
  return "Force_4b_display_mode";
case DM_NONE:
  return "DM_NONE";
case DM_HD44780:
  return "DM_HD44780";
  }
 return NULL;
}
const char *getLog_format_e(log_format_e value){
switch(value) {
case LF_NATIVE:
  return "LF_NATIVE";
case Force_4b_log_format:
  return "Force_4b_log_format";
case LM_MLV:
  return "LM_MLV";
  }
 return NULL;
}
const char *getIdle_mode_e(idle_mode_e value){
switch(value) {
case IM_MANUAL:
  return "IM_MANUAL";
case Force_4b_idle_mode:
  return "Force_4b_idle_mode";
case IM_AUTO:
  return "IM_AUTO";
  }
 return NULL;
}
const char *getPin_output_mode_e(pin_output_mode_e value){
switch(value) {
case OM_OPENDRAIN:
  return "OM_OPENDRAIN";
case OM_OPENDRAIN_INVERTED:
  return "OM_OPENDRAIN_INVERTED";
case OM_DEFAULT:
  return "OM_DEFAULT";
case Force_4b_pin_output_mode:
  return "Force_4b_pin_output_mode";
case OM_INVERTED:
  return "OM_INVERTED";
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
  }
 return NULL;
}
const char *getFiring_order_e(firing_order_e value){
switch(value) {
case FO_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4:
  return "FO_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4";
case FO_1_THEN_3_THEN_4_THEN2:
  return "FO_1_THEN_3_THEN_4_THEN2";
case FO_1_THEN_3_THEN_2_THEN4:
  return "FO_1_THEN_3_THEN_2_THEN4";
case FO_ONE_CYLINDER:
  return "FO_ONE_CYLINDER";
case FO_1_8_4_3_6_5_7_2:
  return "FO_1_8_4_3_6_5_7_2";
case Force_4b_firing_order:
  return "Force_4b_firing_order";
case FO_1_THEN_2_THEN_4_THEN3:
  return "FO_1_THEN_2_THEN_4_THEN3";
  }
 return NULL;
}
const char *getOperation_mode_e(operation_mode_e value){
switch(value) {
case FOUR_STROKE_CAM_SENSOR:
  return "FOUR_STROKE_CAM_SENSOR";
case OM_NONE:
  return "OM_NONE";
case FOUR_STROKE_CRANK_SENSOR:
  return "FOUR_STROKE_CRANK_SENSOR";
case Force_4b_operation_mode_e:
  return "Force_4b_operation_mode_e";
  }
 return NULL;
}
const char *getIgnition_mode_e(ignition_mode_e value){
switch(value) {
case IM_INDIVIDUAL_COILS:
  return "IM_INDIVIDUAL_COILS";
case Force_4b_ignition_mode:
  return "Force_4b_ignition_mode";
case IM_ONE_COIL:
  return "IM_ONE_COIL";
case IM_WASTED_SPARK:
  return "IM_WASTED_SPARK";
  }
 return NULL;
}
const char *getInjection_mode_e(injection_mode_e value){
switch(value) {
case IM_BATCH:
  return "IM_BATCH";
case Force_4b_injection_mode:
  return "Force_4b_injection_mode";
case IM_SEQUENTIAL:
  return "IM_SEQUENTIAL";
case IM_SIMULTANEOUS:
  return "IM_SIMULTANEOUS";
  }
 return NULL;
}
const char *getCranking_ignition_mode_e(cranking_ignition_mode_e value){
switch(value) {
case CIM_FIXED_ANGLE:
  return "CIM_FIXED_ANGLE";
case CIM_DEFAULT:
  return "CIM_DEFAULT";
case Force_4b_cranking_ignition_mode:
  return "Force_4b_cranking_ignition_mode";
  }
 return NULL;
}
const char *getSpi_device_e(spi_device_e value){
switch(value) {
case SPI_NONE:
  return "SPI_NONE";
case Force_4b_spi_device:
  return "Force_4b_spi_device";
case SPI_DEVICE_3:
  return "SPI_DEVICE_3";
case SPI_DEVICE_2:
  return "SPI_DEVICE_2";
case SPI_DEVICE_4:
  return "SPI_DEVICE_4";
case SPI_DEVICE_1:
  return "SPI_DEVICE_1";
  }
 return NULL;
}
const char *getInternal_error_e(internal_error_e value){
switch(value) {
case Force_4b_cranking_internal_error:
  return "Force_4b_cranking_internal_error";
case IE_UNEXPECTED_FIRING_ORDER:
  return "IE_UNEXPECTED_FIRING_ORDER";
case IE_NO_ERROR:
  return "IE_NO_ERROR";
  }
 return NULL;
}
const char *getAdc_channel_e(adc_channel_e value){
switch(value) {
case EFI_ADC_14:
  return "EFI_ADC_14";
case EFI_ADC_13:
  return "EFI_ADC_13";
case EFI_ADC_15:
  return "EFI_ADC_15";
case EFI_ADC_ERROR:
  return "EFI_ADC_ERROR";
case EFI_ADC_10:
  return "EFI_ADC_10";
case EFI_ADC_12:
  return "EFI_ADC_12";
case EFI_ADC_11:
  return "EFI_ADC_11";
case Force_4b_cranking_adc_channel:
  return "Force_4b_cranking_adc_channel";
case EFI_ADC_7:
  return "EFI_ADC_7";
case EFI_ADC_6:
  return "EFI_ADC_6";
case EFI_ADC_9:
  return "EFI_ADC_9";
case EFI_ADC_8:
  return "EFI_ADC_8";
case EFI_ADC_1:
  return "EFI_ADC_1";
case EFI_ADC_0:
  return "EFI_ADC_0";
case EFI_ADC_3:
  return "EFI_ADC_3";
case EFI_ADC_2:
  return "EFI_ADC_2";
case EFI_ADC_5:
  return "EFI_ADC_5";
case EFI_ADC_4:
  return "EFI_ADC_4";
  }
 return NULL;
}
const char *getEgo_sensor_e(ego_sensor_e value){
switch(value) {
case Force_4b_ego_sensor:
  return "Force_4b_ego_sensor";
case ES_BPSX_D1:
  return "ES_BPSX_D1";
case ES_Innovate_MTX_L:
  return "ES_Innovate_MTX_L";
  }
 return NULL;
}
const char *getBrain_pin_e(brain_pin_e value){
switch(value) {
case GPIOE_6:
  return "GPIOE_6";
case GPIOC_1:
  return "GPIOC_1";
case GPIOB_10:
  return "GPIOB_10";
case GPIOB_6:
  return "GPIOB_6";
case GPIOE_5:
  return "GPIOE_5";
case GPIOC_2:
  return "GPIOC_2";
case GPIOB_7:
  return "GPIOB_7";
case GPIOE_8:
  return "GPIOE_8";
case GPIOB_4:
  return "GPIOB_4";
case GPIOE_7:
  return "GPIOE_7";
case GPIOC_0:
  return "GPIOC_0";
case GPIOB_5:
  return "GPIOB_5";
case GPIOE_2:
  return "GPIOE_2";
case GPIOB_14:
  return "GPIOB_14";
case GPIOE_1:
  return "GPIOE_1";
case GPIOB_13:
  return "GPIOB_13";
case GPIOE_4:
  return "GPIOE_4";
case GPIOD_1:
  return "GPIOD_1";
case GPIOB_12:
  return "GPIOB_12";
case GPIOB_8:
  return "GPIOB_8";
case GPIOE_3:
  return "GPIOE_3";
case GPIOD_0:
  return "GPIOD_0";
case GPIOB_11:
  return "GPIOB_11";
case GPIOB_9:
  return "GPIOB_9";
case GPIOD_3:
  return "GPIOD_3";
case GPIOC_9:
  return "GPIOC_9";
case GPIOD_2:
  return "GPIOD_2";
case GPIOE_0:
  return "GPIOE_0";
case GPIOD_5:
  return "GPIOD_5";
case GPIOC_7:
  return "GPIOC_7";
case GPIO_INVALID:
  return "GPIO_INVALID";
case GPIOD_4:
  return "GPIOD_4";
case GPIOC_8:
  return "GPIOC_8";
case GPIOD_7:
  return "GPIOD_7";
case GPIOC_5:
  return "GPIOC_5";
case GPIOB_2:
  return "GPIOB_2";
case GPIOD_6:
  return "GPIOD_6";
case GPIOC_6:
  return "GPIOC_6";
case GPIOB_3:
  return "GPIOB_3";
case GPIOD_9:
  return "GPIOD_9";
case GPIOC_3:
  return "GPIOC_3";
case GPIOB_0:
  return "GPIOB_0";
case GPIOD_8:
  return "GPIOD_8";
case GPIOC_4:
  return "GPIOC_4";
case GPIOB_1:
  return "GPIOB_1";
case GPIOB_15:
  return "GPIOB_15";
case GPIOE_9:
  return "GPIOE_9";
case GPIOA_10:
  return "GPIOA_10";
case GPIOA_11:
  return "GPIOA_11";
case GPIOA_12:
  return "GPIOA_12";
case GPIOA_13:
  return "GPIOA_13";
case GPIOA_15:
  return "GPIOA_15";
case GPIOA_14:
  return "GPIOA_14";
case GPIOD_11:
  return "GPIOD_11";
case GPIOD_12:
  return "GPIOD_12";
case GPIOD_10:
  return "GPIOD_10";
case GPIOD_15:
  return "GPIOD_15";
case GPIOD_13:
  return "GPIOD_13";
case GPIOD_14:
  return "GPIOD_14";
case GPIOE_14:
  return "GPIOE_14";
case GPIOE_15:
  return "GPIOE_15";
case GPIOE_10:
  return "GPIOE_10";
case GPIOE_11:
  return "GPIOE_11";
case GPIOE_12:
  return "GPIOE_12";
case GPIOE_13:
  return "GPIOE_13";
case GPIOC_14:
  return "GPIOC_14";
case GPIOC_15:
  return "GPIOC_15";
case GPIOC_12:
  return "GPIOC_12";
case GPIOC_13:
  return "GPIOC_13";
case GPIOC_10:
  return "GPIOC_10";
case GPIOC_11:
  return "GPIOC_11";
case Force_4b_cranking_brain_pin:
  return "Force_4b_cranking_brain_pin";
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
case GPIOA_0:
  return "GPIOA_0";
case GPIO_NONE:
  return "GPIO_NONE";
case GPIOA_1:
  return "GPIOA_1";
case GPIOA_2:
  return "GPIOA_2";
case GPIOA_3:
  return "GPIOA_3";
case GPIOA_4:
  return "GPIOA_4";
  }
 return NULL;
}
const char *getAir_pressure_sensor_type_e(air_pressure_sensor_type_e value){
switch(value) {
case MT_MPX4250:
  return "MT_MPX4250";
case MT_DENSO183:
  return "MT_DENSO183";
case MT_HONDA3BAR:
  return "MT_HONDA3BAR";
case Force_4b_cranking_map_type:
  return "Force_4b_cranking_map_type";
case MT_CUSTOM:
  return "MT_CUSTOM";
  }
 return NULL;
}
