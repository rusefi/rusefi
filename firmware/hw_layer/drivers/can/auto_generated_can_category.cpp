#include "global.h"
#include "can_category.h"
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
