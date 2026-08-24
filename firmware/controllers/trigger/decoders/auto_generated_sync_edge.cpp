#include "global.h"
#include "sync_edge.h"
// was generated automatically by rusEFI tool  from sync_edge.h // by enum_to_string.jar tool on Sat Apr 04 11:57:46 UTC 2026
// see also gen_config_and_enums.bat



const char *getSyncEdge(SyncEdge value) {
	switch (value) {
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
