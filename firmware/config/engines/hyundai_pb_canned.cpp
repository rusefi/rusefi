#include "hyundai_pb_canned_tables.cpp"

void cannedPbTables() {
#if (IGN_LOAD_COUNT == 16) && (IGN_RPM_COUNT == 16) && (FUEL_LOAD_COUNT == 16) && (FUEL_RPM_COUNT == 16)
	pbcannedvvtTable1();
	pbcannedvvtTable2();
	pbcannedignitionTable();
	pbcannedveTable();
	pbcannedinjectionPhase();
	pbcannedlambdaTable();
#endif
}
