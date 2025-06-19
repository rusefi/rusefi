// this section was generated automatically by rusEFI tool config_definition-all.jar based on gen_config.sh integration/config_page_1.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of page1_s
struct page1_s {
	/**
	 * offset 0
	 */
	uint16_t highSpeedOffsets[TS_SCATTER_OFFSETS_COUNT] = {};
};
static_assert(sizeof(page1_s) == 256);

// end
// this section was generated automatically by rusEFI tool config_definition-all.jar based on gen_config.sh integration/config_page_1.txt
