#include "global.h"
#include "rusefi_enums.h"
// was generated automatically by rusEFI tool  from rusefi_enums.h // by enum_to_string.jar tool on Fri Dec 26 02:52:40 UTC 2025
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
case GearControllerMode::Automatic:
  return "Automatic";
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
const char *getMsIoBoxId(MsIoBoxId value){
switch(value) {
case MsIoBoxId::ID200:
  return "ID200";
case MsIoBoxId::ID220:
  return "ID220";
case MsIoBoxId::ID240:
  return "ID240";
case MsIoBoxId::OFF:
  return "OFF";
  }
 return NULL;
}
const char *getMsIoBoxVss(MsIoBoxVss value){
switch(value) {
case MsIoBoxVss::ALL1234:
  return "ALL1234";
case MsIoBoxVss::HALL34:
  return "HALL34";
case MsIoBoxVss::OFF:
  return "OFF";
case MsIoBoxVss::VR12:
  return "VR12";
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
const char *getSentFuelHighPressureType(SentFuelHighPressureType value){
switch(value) {
case SentFuelHighPressureType::GM_TYPE:
  return "GM_TYPE";
case SentFuelHighPressureType::NONE:
  return "NONE";
  }
 return NULL;
}
const char *getSentInput(SentInput value){
switch(value) {
case SentInput::INPUT1:
  return "INPUT1";
case SentInput::INPUT2:
  return "INPUT2";
case SentInput::INPUT3:
  return "INPUT3";
case SentInput::INPUT4:
  return "INPUT4";
case SentInput::INPUT5:
  return "INPUT5";
case SentInput::INPUT6:
  return "INPUT6";
case SentInput::INPUT7:
  return "INPUT7";
case SentInput::NONE:
  return "NONE";
  }
 return NULL;
}
const char *getTransmissionControllerMode(TransmissionControllerMode value){
switch(value) {
case TransmissionControllerMode::Generic4:
  return "Generic4";
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
case TsCalMode::EwgPosMax:
  return "EwgPosMax";
case TsCalMode::EwgPosMin:
  return "EwgPosMin";
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
const char *getAccel_enrichment_mode_e(accel_enrichment_mode_e value){
switch(value) {
case AE_MODE_MS_ADDER:
  return "AE_MODE_MS_ADDER";
case AE_MODE_PERCENT_ADDER:
  return "AE_MODE_PERCENT_ADDER";
case AE_MODE_PREDICTIVE_MAP:
  return "AE_MODE_PREDICTIVE_MAP";
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
case LUA_ANTILAG:
  return "LUA_ANTILAG";
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
case B33KBPS:
  return "B33KBPS";
case B500KBPS:
  return "B500KBPS";
case B50KBPS:
  return "B50KBPS";
case B666KBPS:
  return "B666KBPS";
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
const char *getCan_wbo_aem_id_e(can_wbo_aem_id_e value){
switch(value) {
case WBO_AEM_ID1:
  return "WBO_AEM_ID1";
case WBO_AEM_ID10:
  return "WBO_AEM_ID10";
case WBO_AEM_ID11:
  return "WBO_AEM_ID11";
case WBO_AEM_ID12:
  return "WBO_AEM_ID12";
case WBO_AEM_ID13:
  return "WBO_AEM_ID13";
case WBO_AEM_ID14:
  return "WBO_AEM_ID14";
case WBO_AEM_ID15:
  return "WBO_AEM_ID15";
case WBO_AEM_ID16:
  return "WBO_AEM_ID16";
case WBO_AEM_ID2:
  return "WBO_AEM_ID2";
case WBO_AEM_ID3:
  return "WBO_AEM_ID3";
case WBO_AEM_ID4:
  return "WBO_AEM_ID4";
case WBO_AEM_ID5:
  return "WBO_AEM_ID5";
case WBO_AEM_ID6:
  return "WBO_AEM_ID6";
case WBO_AEM_ID7:
  return "WBO_AEM_ID7";
case WBO_AEM_ID8:
  return "WBO_AEM_ID8";
case WBO_AEM_ID9:
  return "WBO_AEM_ID9";
  }
 return NULL;
}
const char *getCan_wbo_re_hwidx_e(can_wbo_re_hwidx_e value){
switch(value) {
case WBO_RE_HWIDX0:
  return "WBO_RE_HWIDX0";
case WBO_RE_HWIDX1:
  return "WBO_RE_HWIDX1";
case WBO_RE_HWIDX2:
  return "WBO_RE_HWIDX2";
case WBO_RE_HWIDX3:
  return "WBO_RE_HWIDX3";
case WBO_RE_HWIDX4:
  return "WBO_RE_HWIDX4";
case WBO_RE_HWIDX5:
  return "WBO_RE_HWIDX5";
case WBO_RE_HWIDX6:
  return "WBO_RE_HWIDX6";
case WBO_RE_HWIDX7:
  return "WBO_RE_HWIDX7";
  }
 return NULL;
}
const char *getCan_wbo_re_id_e(can_wbo_re_id_e value){
switch(value) {
case WBO_RE_ID1:
  return "WBO_RE_ID1";
case WBO_RE_ID10:
  return "WBO_RE_ID10";
case WBO_RE_ID11:
  return "WBO_RE_ID11";
case WBO_RE_ID12:
  return "WBO_RE_ID12";
case WBO_RE_ID13:
  return "WBO_RE_ID13";
case WBO_RE_ID14:
  return "WBO_RE_ID14";
case WBO_RE_ID15:
  return "WBO_RE_ID15";
case WBO_RE_ID16:
  return "WBO_RE_ID16";
case WBO_RE_ID2:
  return "WBO_RE_ID2";
case WBO_RE_ID3:
  return "WBO_RE_ID3";
case WBO_RE_ID4:
  return "WBO_RE_ID4";
case WBO_RE_ID5:
  return "WBO_RE_ID5";
case WBO_RE_ID6:
  return "WBO_RE_ID6";
case WBO_RE_ID7:
  return "WBO_RE_ID7";
case WBO_RE_ID8:
  return "WBO_RE_ID8";
case WBO_RE_ID9:
  return "WBO_RE_ID9";
  }
 return NULL;
}
const char *getCan_wbo_re_status_e(can_wbo_re_status_e value){
switch(value) {
case WBO_RE_BUSY:
  return "WBO_RE_BUSY";
case WBO_RE_DONE:
  return "WBO_RE_DONE";
case WBO_RE_FAILED:
  return "WBO_RE_FAILED";
case WBO_RE_IDLE:
  return "WBO_RE_IDLE";
  }
 return NULL;
}
const char *getCan_wbo_type_e(can_wbo_type_e value){
switch(value) {
case AEM:
  return "AEM";
case DISABLED:
  return "DISABLED";
case RUSEFI:
  return "RUSEFI";
  }
 return NULL;
}
const char *getCranking_condition_e(cranking_condition_e value){
switch(value) {
case CCNONE:
  return "CCNONE";
case CC_BRAKE:
  return "CC_BRAKE";
case CC_CLUTCH:
  return "CC_CLUTCH";
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
const char *getFt_region_e(ft_region_e value){
switch(value) {
case ftRegionCruise:
  return "ftRegionCruise";
case ftRegionIdle:
  return "ftRegionIdle";
case ftRegionOverrun:
  return "ftRegionOverrun";
case ftRegionPower:
  return "ftRegionPower";
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
case GPPWM_AuxLinear3:
  return "GPPWM_AuxLinear3";
case GPPWM_AuxLinear4:
  return "GPPWM_AuxLinear4";
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
case GPPWM_Egt1:
  return "GPPWM_Egt1";
case GPPWM_Egt2:
  return "GPPWM_Egt2";
case GPPWM_EthanolPercent:
  return "GPPWM_EthanolPercent";
case GPPWM_FuelLoad:
  return "GPPWM_FuelLoad";
case GPPWM_FuelPressure:
  return "GPPWM_FuelPressure";
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
case GPPWM_OilPressure:
  return "GPPWM_OilPressure";
case GPPWM_OilTemp:
  return "GPPWM_OilTemp";
case GPPWM_Rpm:
  return "GPPWM_Rpm";
case GPPWM_ThrottleRatio:
  return "GPPWM_ThrottleRatio";
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
case GPPWM_VehicleSpeed:
  return "GPPWM_VehicleSpeed";
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
case idle_mode_e::IM_AUTO:
  return "IM_AUTO";
case idle_mode_e::IM_MANUAL:
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
case ICM_HPFP_Manual_Compensation:
  return "ICM_HPFP_Manual_Compensation";
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
case LUA_LAUNCH:
  return "LUA_LAUNCH";
case STOP_INPUT_LAUNCH:
  return "STOP_INPUT_LAUNCH";
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
const char *getLua_gauge_e(lua_gauge_e value){
switch(value) {
case LUA_GAUGE_1:
  return "LUA_GAUGE_1";
case LUA_GAUGE_2:
  return "LUA_GAUGE_2";
case LUA_GAUGE_3:
  return "LUA_GAUGE_3";
case LUA_GAUGE_4:
  return "LUA_GAUGE_4";
case LUA_GAUGE_5:
  return "LUA_GAUGE_5";
case LUA_GAUGE_6:
  return "LUA_GAUGE_6";
case LUA_GAUGE_7:
  return "LUA_GAUGE_7";
case LUA_GAUGE_8:
  return "LUA_GAUGE_8";
  }
 return NULL;
}
const char *getLua_gauge_meaning_e(lua_gauge_meaning_e value){
switch(value) {
case LUA_GAUGE_LOWER_BOUND:
  return "LUA_GAUGE_LOWER_BOUND";
case LUA_GAUGE_UPPER_BOUND:
  return "LUA_GAUGE_UPPER_BOUND";
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
const char *getNitrous_arming_method_e(nitrous_arming_method_e value){
switch(value) {
case DIGITAL_SWITCH_INPUT:
  return "DIGITAL_SWITCH_INPUT";
case LUA_GAUGE:
  return "LUA_GAUGE";
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
case PI_INVERTED_DEFAULT:
  return "PI_INVERTED_DEFAULT";
case PI_INVERTED_PULLDOWN:
  return "PI_INVERTED_PULLDOWN";
case PI_INVERTED_PULLUP:
  return "PI_INVERTED_PULLUP";
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
const char *getStft_state_e(stft_state_e value){
switch(value) {
case stftDisabledAfrOurOfRange:
  return "stftDisabledAfrOurOfRange";
case stftDisabledClt:
  return "stftDisabledClt";
case stftDisabledCrankingDelay:
  return "stftDisabledCrankingDelay";
case stftDisabledDFCO:
  return "stftDisabledDFCO";
case stftDisabledFuelCut:
  return "stftDisabledFuelCut";
case stftDisabledRPM:
  return "stftDisabledRPM";
case stftDisabledSettings:
  return "stftDisabledSettings";
case stftDisabledTpsAccel:
  return "stftDisabledTpsAccel";
case stftDisabledTuning:
  return "stftDisabledTuning";
case stftEnabled:
  return "stftEnabled";
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
const char *getTorqueReductionActivationMode_e(torqueReductionActivationMode_e value){
switch(value) {
case LAUNCH_BUTTON:
  return "LAUNCH_BUTTON";
case TORQUE_REDUCTION_BUTTON:
  return "TORQUE_REDUCTION_BUTTON";
case TORQUE_REDUCTION_CLUTCH_DOWN_SWITCH:
  return "TORQUE_REDUCTION_CLUTCH_DOWN_SWITCH";
case TORQUE_REDUCTION_CLUTCH_UP_SWITCH:
  return "TORQUE_REDUCTION_CLUTCH_UP_SWITCH";
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
case VVT_CHRYSLER_PHASER:
  return "VVT_CHRYSLER_PHASER";
case VVT_CUSTOM_1:
  return "VVT_CUSTOM_1";
case VVT_CUSTOM_2:
  return "VVT_CUSTOM_2";
case VVT_CUSTOM_25:
  return "VVT_CUSTOM_25";
case VVT_CUSTOM_26:
  return "VVT_CUSTOM_26";
case VVT_DEV:
  return "VVT_DEV";
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
case VVT_HR12DDR_IN:
  return "VVT_HR12DDR_IN";
case VVT_INACTIVE:
  return "VVT_INACTIVE";
case VVT_MAP_V_TWIN:
  return "VVT_MAP_V_TWIN";
case VVT_MAZDA_L:
  return "VVT_MAZDA_L";
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
case VVT_MITSUBISHI_6G72:
  return "VVT_MITSUBISHI_6G72";
case VVT_NISSAN_MR:
  return "VVT_NISSAN_MR";
case VVT_NISSAN_VQ:
  return "VVT_NISSAN_VQ";
case VVT_SINGLE_TOOTH:
  return "VVT_SINGLE_TOOTH";
case VVT_SUBARU_7TOOTH:
  return "VVT_SUBARU_7TOOTH";
case VVT_TOYOTA_3TOOTH_UZ:
  return "VVT_TOYOTA_3TOOTH_UZ";
case VVT_TOYOTA_3_TOOTH:
  return "VVT_TOYOTA_3_TOOTH";
case VVT_TOYOTA_4_1:
  return "VVT_TOYOTA_4_1";
case VVT_UNUSED_17:
  return "VVT_UNUSED_17";
  }
 return NULL;
}
