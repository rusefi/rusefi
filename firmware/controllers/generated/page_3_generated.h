// this section was generated automatically by rusEFI tool config_definition-all.jar based on gen_config.sh integration/config_page_3.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of page3_s
struct page3_s {
	/**
	 * units: %
	 * offset 0
	 */
	float ltft_table_bank1[VE_LOAD_COUNT][VE_RPM_COUNT] = {};
	/**
	 * units: %
	 * offset 1024
	 */
	float ltft_table_bank2[VE_LOAD_COUNT][VE_RPM_COUNT] = {};
};
static_assert(sizeof(page3_s) == 2048);

// end
// this section was generated automatically by rusEFI tool config_definition-all.jar based on gen_config.sh integration/config_page_3.txt
