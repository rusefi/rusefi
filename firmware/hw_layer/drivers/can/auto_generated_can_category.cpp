#include "global.h"
#include "can_category.h"
// was generated automatically by rusEFI tool  from can_category.h // by enum_to_string.jar tool on Fri Nov 14 15:58:03 UTC 2025
// see also gen_config_and_enums.bat



const char *getCanCategory(CanCategory value){
switch(value) {
case CanCategory::BENCH_TEST:
  return "BENCH_TEST";
case CanCategory::CAN_IOBOX:
  return "CAN_IOBOX";
case CanCategory::DOWNSTREAM_FLASHING:
  return "DOWNSTREAM_FLASHING";
case CanCategory::GDI:
  return "GDI";
case CanCategory::LUA:
  return "LUA";
case CanCategory::NBC:
  return "NBC";
case CanCategory::OBD:
  return "OBD";
case CanCategory::SERIAL:
  return "SERIAL";
case CanCategory::TCU:
  return "TCU";
case CanCategory::VERBOSE:
  return "VERBOSE";
case CanCategory::WBO_SERVICE:
  return "WBO_SERVICE";
  }
 return NULL;
}
