#include "global.h"
#include "sync_edge.h"
// was generated automatically by rusEFI tool  from sync_edge.h // by enum2string.jar tool on Fri Sep 23 12:11:58 PDT 2022
// see also gen_config_and_enums.bat



const char *getSyncEdge(SyncEdge value){
switch(value) {
case SyncEdge::Both:
  return "Both";
case SyncEdge::Fall:
  return "Fall";
case SyncEdge::Rise:
  return "Rise";
  }
 return NULL;
}
