#include "global.h"
#include "can_category.h"
// was generated automatically by rusEFI tool  from can_category.h // by enum_to_string.jar tool on Sat Apr 20 12:21:13 UTC 2024
// see also gen_config_and_enums.bat



const char *getCanCategory(CanCategory value){
switch(value) {
case CanCategory::BENCH_TEST:
  return "BENCH_TEST";
case CanCategory::DOWNSTREAM_FLASHING:
  return "DOWNSTREAM_FLASHING";
case CanCategory::LUA:
  return "LUA";
case CanCategory::MEGASQUIRT:
  return "MEGASQUIRT";
case CanCategory::NBC:
  return "NBC";
case CanCategory::OBD:
  return "OBD";
case CanCategory::SERIAL:
  return "SERIAL";
case CanCategory::VERBOSE:
  return "VERBOSE";
case CanCategory::WBO_SERVICE:
  return "WBO_SERVICE";
  }
 return NULL;
}
