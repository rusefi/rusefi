#include "global.h"
#include "sync_edge.h"
// was generated automatically by rusEFI tool  from sync_edge.h // by enum2string.jar tool on Sun Sep 25 22:53:38 UTC 2022
// see also gen_config_and_enums.bat



const char *getSyncEdge(SyncEdge value){
switch(value) {
case SyncEdge::Both:
  return "Both";
case SyncEdge::Fall:
  return "Fall";
case SyncEdge::Rise:
  return "Rise";
case SyncEdge::RiseOnly:
  return "RiseOnly";
  }
 return NULL;
}
