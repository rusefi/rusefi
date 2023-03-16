#include "global.h"
#include "sync_edge.h"
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
