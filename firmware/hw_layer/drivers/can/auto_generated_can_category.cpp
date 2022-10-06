#include "global.h"
#include "can_category.h"
// was generated automatically by rusEFI tool  from can_category.h // by enum2string.jar tool on Fri Sep 30 03:14:34 UTC 2022
// see also gen_config_and_enums.bat



const char *getCanCategory(CanCategory value){
switch(value) {
case CanCategory::BENCH_TEST:
  return "BENCH_TEST";
case CanCategory::DOWNSTREAM_FLASHING:
  return "DOWNSTREAM_FLASHING";
case CanCategory::LUA:
  return "LUA";
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
