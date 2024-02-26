#include "global.h"
#include "rusefi_enums.h"
// was generated automatically by rusEFI tool  from rusefi_enums.h // by enum_to_string.jar tool on Mon Feb 26 19:54:17 UTC 2024
// see also gen_config_and_enums.bat



const char *getCanGpioType(CanGpioType value){
switch(value) {
case CanGpioType::DRT:
  return "DRT";
case CanGpioType::MS:
  return "MS";
case CanGpioType::NONE:
  return "NONE";
  }
 return NULL;
}
const char *getGearControllerMode(GearControllerMode value){
switch(value) {
case GearControllerMode::ButtonShift:
  return "ButtonShift";
case GearControllerMode::Generic:
  return "Generic";
case GearControllerMode::None:
  return "None";
  }
 return NULL;
}
const char *getInjectionTimingMode(InjectionTimingMode value){
switch(value) {
case InjectionTimingMode::Center:
  return "Center";
case InjectionTimingMode::End:
  return "End";
case InjectionTimingMode::Start:
  return "Start";
  }
 return NULL;
}
const char *getInjectorNonlinearMode(InjectorNonlinearMode value){
switch(value) {
case INJ_FordModel:
  return "INJ_FordModel";
case INJ_None:
  return "INJ_None";
case INJ_PolynomialAdder:
  return "INJ_PolynomialAdder";
  }
 return NULL;
}
const char *getSelectedGear(SelectedGear value){
switch(value) {
case SelectedGear::Drive:
  return "Drive";
case SelectedGear::Invalid:
  return "Invalid";
case SelectedGear::Low:
  return "Low";
case SelectedGear::Manual:
  return "Manual";
case SelectedGear::Manual1:
  return "Manual1";
case SelectedGear::Manual2:
  return "Manual2";
case SelectedGear::Manual3:
  return "Manual3";
case SelectedGear::ManualMinus:
  return "ManualMinus";
case SelectedGear::ManualPlus:
  return "ManualPlus";
case SelectedGear::Neutral:
  return "Neutral";
case SelectedGear::Park:
  return "Park";
case SelectedGear::Reverse:
  return "Reverse";
  }
 return NULL;
}
const char *getSentEtbType(SentEtbType value){
switch(value) {
case SentEtbType::CUSTOM:
  return "CUSTOM";
case SentEtbType::FORD_TYPE_1:
  return "FORD_TYPE_1";
case SentEtbType::GM_TYPE_1:
  return "GM_TYPE_1";
case SentEtbType::NONE:
  return "NONE";
  }
 return NULL;
}
const char *getTransmissionControllerMode(TransmissionControllerMode value){
switch(value) {
case TransmissionControllerMode::Gm4l6x:
  return "Gm4l6x";
case TransmissionControllerMode::None:
  return "None";
case TransmissionControllerMode::SimpleTransmissionController:
  return "SimpleTransmissionController";
  }
 return NULL;
}
const char *getTriggerWheel(TriggerWheel value){
switch(value) {
case TriggerWheel::T_PRIMARY:
  return "T_PRIMARY";
case TriggerWheel::T_SECONDARY:
  return "T_SECONDARY";
  }
 return NULL;
}
const char *getTsCalMode(TsCalMode value){
switch(value) {
case TsCalMode::EtbKd:
  return "EtbKd";
case TsCalMode::EtbKi:
  return "EtbKi";
case TsCalMode::EtbKp:
  return "EtbKp";
case TsCalMode::None:
  return "None";
case TsCalMode::PedalMax:
  return "PedalMax";
case TsCalMode::PedalMin:
  return "PedalMin";
case TsCalMode::Tps1Max:
  return "Tps1Max";
case TsCalMode::Tps1Min:
  return "Tps1Min";
case TsCalMode::Tps1SecondaryMax:
  return "Tps1SecondaryMax";
case TsCalMode::Tps1SecondaryMin:
  return "Tps1SecondaryMin";
case TsCalMode::Tps2Max:
  return "Tps2Max";
case TsCalMode::Tps2Min:
  return "Tps2Min";
case TsCalMode::Tps2SecondaryMax:
  return "Tps2SecondaryMax";
case TsCalMode::Tps2SecondaryMin:
  return "Tps2SecondaryMin";
  }
 return NULL;
}
const char *getUiMode(UiMode value){
switch(value) {
case UiMode::FULL:
  return "FULL";
case UiMode::INSTALLATION:
  return "INSTALLATION";
case UiMode::TUNING:
  return "TUNING";
  }
 return NULL;
}
const char *getAir_pressure_sensor_type_e(air_pressure_sensor_type_e value){
switch(value) {
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
case MT_MPXH6300:
  return "MT_MPXH6300";
case MT_MPXH6400:
  return "MT_MPXH6400";
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
case SWITCH_INPUT_ANTILAG:
  return "SWITCH_INPUT_ANTILAG";
  }
 return NULL;
}
const char *getBoostType_e(boostType_e value){
switch(value) {
case CLOSED_LOOP:
  return "CLOSED_LOOP";
case OPEN_LOOP:
  return "OPEN_LOOP";
  }
 return NULL;
}
const char *getBrain_pin_diag_e(brain_pin_diag_e value){
switch(value) {
case PIN_DRIVER_OFF:
  return "PIN_DRIVER_OFF";
case PIN_DRIVER_OVERTEMP:
  return "PIN_DRIVER_OVERTEMP";
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
case PIN_UNKNOWN:
  return "PIN_UNKNOWN";
  }
 return NULL;
}
const char *getCan_baudrate_e(can_baudrate_e value){
switch(value) {
case B100KBPS:
  return "B100KBPS";
case B125KBPS:
  return "B125KBPS";
case B1MBPS:
  return "B1MBPS";
case B250KBPS:
  return "B250KBPS";
case B500KBPS:
  return "B500KBPS";
case B50KBPS:
  return "B50KBPS";
case B83KBPS:
  return "B83KBPS";
  }
 return NULL;
}
const char *getCan_nbc_e(can_nbc_e value){
switch(value) {
case CAN_AIM_DASH:
  return "CAN_AIM_DASH";
case CAN_BUS_BMW_E46:
  return "CAN_BUS_BMW_E46";
case CAN_BUS_BMW_E90:
  return "CAN_BUS_BMW_E90";
case CAN_BUS_GENESIS_COUPE:
  return "CAN_BUS_GENESIS_COUPE";
case CAN_BUS_HONDA_K:
  return "CAN_BUS_HONDA_K";
case CAN_BUS_Haltech:
  return "CAN_BUS_Haltech";
case CAN_BUS_MAZDA_RX8:
  return "CAN_BUS_MAZDA_RX8";
case CAN_BUS_MQB:
  return "CAN_BUS_MQB";
case CAN_BUS_MS_SIMPLE_BROADCAST:
  return "CAN_BUS_MS_SIMPLE_BROADCAST";
case CAN_BUS_NBC_FIAT:
  return "CAN_BUS_NBC_FIAT";
case CAN_BUS_NBC_NONE:
  return "CAN_BUS_NBC_NONE";
case CAN_BUS_NBC_VAG:
  return "CAN_BUS_NBC_VAG";
case CAN_BUS_NISSAN_VQ:
  return "CAN_BUS_NISSAN_VQ";
case CAN_BUS_W202_C180:
  return "CAN_BUS_W202_C180";
  }
 return NULL;
}
const char *getCan_vss_nbc_e(can_vss_nbc_e value){
switch(value) {
case BMW_e46:
  return "BMW_e46";
case BMW_e90:
  return "BMW_e90";
case HONDA_CIVIC9:
  return "HONDA_CIVIC9";
case HYUNDAI_PB:
  return "HYUNDAI_PB";
case NISSAN_350:
  return "NISSAN_350";
case W202:
  return "W202";
  }
 return NULL;
}
const char *getDc_function_e(dc_function_e value){
switch(value) {
case DC_IdleValve:
  return "DC_IdleValve";
case DC_None:
  return "DC_None";
case DC_Throttle1:
  return "DC_Throttle1";
case DC_Throttle2:
  return "DC_Throttle2";
case DC_Wastegate:
  return "DC_Wastegate";
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
case ES_PLX:
  return "ES_PLX";
  }
 return NULL;
}
const char *getEngine_load_mode_e(engine_load_mode_e value){
switch(value) {
case LM_ALPHA_N:
  return "LM_ALPHA_N";
case LM_LUA:
  return "LM_LUA";
case LM_MOCK:
  return "LM_MOCK";
case LM_REAL_MAF:
  return "LM_REAL_MAF";
case LM_SPEED_DENSITY:
  return "LM_SPEED_DENSITY";
  }
 return NULL;
}
const char *getFuel_pressure_sensor_mode_e(fuel_pressure_sensor_mode_e value){
switch(value) {
case FPM_Absolute:
  return "FPM_Absolute";
case FPM_Differential:
  return "FPM_Differential";
case FPM_Gauge:
  return "FPM_Gauge";
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
case GPPWM_AuxLinear1:
  return "GPPWM_AuxLinear1";
case GPPWM_AuxLinear2:
  return "GPPWM_AuxLinear2";
case GPPWM_AuxTemp1:
  return "GPPWM_AuxTemp1";
case GPPWM_AuxTemp2:
  return "GPPWM_AuxTemp2";
case GPPWM_BaroPressure:
  return "GPPWM_BaroPressure";
case GPPWM_Clt:
  return "GPPWM_Clt";
case GPPWM_DetectedGear:
  return "GPPWM_DetectedGear";
case GPPWM_EthanolPercent:
  return "GPPWM_EthanolPercent";
case GPPWM_FuelLoad:
  return "GPPWM_FuelLoad";
case GPPWM_GppwmOutput1:
  return "GPPWM_GppwmOutput1";
case GPPWM_GppwmOutput2:
  return "GPPWM_GppwmOutput2";
case GPPWM_GppwmOutput3:
  return "GPPWM_GppwmOutput3";
case GPPWM_GppwmOutput4:
  return "GPPWM_GppwmOutput4";
case GPPWM_Iat:
  return "GPPWM_Iat";
case GPPWM_IgnLoad:
  return "GPPWM_IgnLoad";
case GPPWM_LuaGauge1:
  return "GPPWM_LuaGauge1";
case GPPWM_LuaGauge2:
  return "GPPWM_LuaGauge2";
case GPPWM_Map:
  return "GPPWM_Map";
case GPPWM_Rpm:
  return "GPPWM_Rpm";
case GPPWM_Tps:
  return "GPPWM_Tps";
case GPPWM_VVT_1E:
  return "GPPWM_VVT_1E";
case GPPWM_VVT_1I:
  return "GPPWM_VVT_1I";
case GPPWM_VVT_2E:
  return "GPPWM_VVT_2E";
case GPPWM_VVT_2I:
  return "GPPWM_VVT_2I";
case GPPWM_Vbatt:
  return "GPPWM_Vbatt";
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
const char *getHpfp_cam_e(hpfp_cam_e value){
switch(value) {
case HPFP_CAM_EX1:
  return "HPFP_CAM_EX1";
case HPFP_CAM_EX2:
  return "HPFP_CAM_EX2";
case HPFP_CAM_IN1:
  return "HPFP_CAM_IN1";
case HPFP_CAM_IN2:
  return "HPFP_CAM_IN2";
case HPFP_CAM_NONE:
  return "HPFP_CAM_NONE";
  }
 return NULL;
}
const char *getIdle_mode_e(idle_mode_e value){
switch(value) {
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
const char *getImu_type_e(imu_type_e value){
switch(value) {
case IMU_MM5_10:
  return "IMU_MM5_10";
case IMU_NONE:
  return "IMU_NONE";
case IMU_TYPE_3:
  return "IMU_TYPE_3";
case IMU_TYPE_4:
  return "IMU_TYPE_4";
case IMU_TYPE_MB_A0065422618:
  return "IMU_TYPE_MB_A0065422618";
case IMU_VAG:
  return "IMU_VAG";
  }
 return NULL;
}
const char *getInjection_mode_e(injection_mode_e value){
switch(value) {
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
case SWITCH_INPUT_LAUNCH:
  return "SWITCH_INPUT_LAUNCH";
  }
 return NULL;
}
const char *getLoad_override_e(load_override_e value){
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
  }
 return NULL;
}
const char *getMc33810maxDwellTimer_e(mc33810maxDwellTimer_e value){
switch(value) {
case DWELL_16MS:
  return "DWELL_16MS";
case DWELL_2MS:
  return "DWELL_2MS";
case DWELL_32MS:
  return "DWELL_32MS";
case DWELL_4MS:
  return "DWELL_4MS";
case DWELL_64MS:
  return "DWELL_64MS";
case DWELL_8MS:
  return "DWELL_8MS";
  }
 return NULL;
}
const char *getOperation_mode_e(operation_mode_e value){
switch(value) {
case FOUR_STROKE_CAM_SENSOR:
  return "FOUR_STROKE_CAM_SENSOR";
case FOUR_STROKE_CRANK_SENSOR:
  return "FOUR_STROKE_CRANK_SENSOR";
case FOUR_STROKE_SIX_TIMES_CRANK_SENSOR:
  return "FOUR_STROKE_SIX_TIMES_CRANK_SENSOR";
case FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR:
  return "FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR";
case FOUR_STROKE_THREE_TIMES_CRANK_SENSOR:
  return "FOUR_STROKE_THREE_TIMES_CRANK_SENSOR";
case FOUR_STROKE_TWELVE_TIMES_CRANK_SENSOR:
  return "FOUR_STROKE_TWELVE_TIMES_CRANK_SENSOR";
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
case SC_AUX_FAST1:
  return "SC_AUX_FAST1";
case SC_DETAILED_RPM:
  return "SC_DETAILED_RPM";
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
case SPI_DEVICE_5:
  return "SPI_DEVICE_5";
case SPI_DEVICE_6:
  return "SPI_DEVICE_6";
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
const char *getStepper_num_micro_steps_e(stepper_num_micro_steps_e value){
switch(value) {
case STEPPER_EIGHTH:
  return "STEPPER_EIGHTH";
case STEPPER_FOURTH:
  return "STEPPER_FOURTH";
case STEPPER_FULL:
  return "STEPPER_FULL";
case STEPPER_HALF:
  return "STEPPER_HALF";
  }
 return NULL;
}
const char *getTChargeMode_e(tChargeMode_e value){
switch(value) {
case TCHARGE_MODE_AIR_INTERP:
  return "TCHARGE_MODE_AIR_INTERP";
case TCHARGE_MODE_AIR_INTERP_TABLE:
  return "TCHARGE_MODE_AIR_INTERP_TABLE";
case TCHARGE_MODE_RPM_TPS:
  return "TCHARGE_MODE_RPM_TPS";
  }
 return NULL;
}
const char *getTiming_mode_e(timing_mode_e value){
switch(value) {
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
case VVT_BARRA_3_PLUS_1:
  return "VVT_BARRA_3_PLUS_1";
case VVT_BOSCH_QUICK_START:
  return "VVT_BOSCH_QUICK_START";
case VVT_FORD_COYOTE:
  return "VVT_FORD_COYOTE";
case VVT_FORD_ST170:
  return "VVT_FORD_ST170";
case VVT_HONDA_CBR_600:
  return "VVT_HONDA_CBR_600";
case VVT_HONDA_K_EXHAUST:
  return "VVT_HONDA_K_EXHAUST";
case VVT_HONDA_K_INTAKE:
  return "VVT_HONDA_K_INTAKE";
case VVT_INACTIVE:
  return "VVT_INACTIVE";
case VVT_MAP_V_TWIN:
  return "VVT_MAP_V_TWIN";
case VVT_MAZDA_SKYACTIV:
  return "VVT_MAZDA_SKYACTIV";
case VVT_MIATA_NB:
  return "VVT_MIATA_NB";
case VVT_MITSUBISHI_3A92:
  return "VVT_MITSUBISHI_3A92";
case VVT_MITSUBISHI_4G63:
  return "VVT_MITSUBISHI_4G63";
case VVT_MITSUBISHI_4G69:
  return "VVT_MITSUBISHI_4G69";
case VVT_MITSUBISHI_4G9x:
  return "VVT_MITSUBISHI_4G9x";
case VVT_MITSUBISHI_6G72:
  return "VVT_MITSUBISHI_6G72";
case VVT_MITSUBISHI_6G75:
  return "VVT_MITSUBISHI_6G75";
case VVT_NISSAN_MR:
  return "VVT_NISSAN_MR";
case VVT_NISSAN_VQ:
  return "VVT_NISSAN_VQ";
case VVT_SINGLE_TOOTH:
  return "VVT_SINGLE_TOOTH";
case VVT_TOYOTA_3_TOOTH:
  return "VVT_TOYOTA_3_TOOTH";
case VVT_TOYOTA_4_1:
  return "VVT_TOYOTA_4_1";
  }
 return NULL;
}
