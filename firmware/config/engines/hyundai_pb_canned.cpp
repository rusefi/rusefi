#include "hyundai_pb_canned_tables.cpp"

void cannedPbTables() {
#if (IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT) && (IGN_RPM_COUNT == DEFAULT_IGN_RPM_COUNT) && (FUEL_LOAD_COUNT == DEFAULT_FUEL_LOAD_COUNT) && (FUEL_RPM_COUNT == DEFAULT_FUEL_LOAD_COUNT)
	cannedvvtTable1();
	cannedvvtTable2();
	cannedignitionTable();
	cannedveTable();
	cannedinjectionPhase();
	cannedlambdaTable();
#endif
}
