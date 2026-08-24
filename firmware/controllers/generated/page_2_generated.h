// this section was generated automatically by rusEFI tool config_definition-all.jar based on gen_config.sh integration/config_page_2.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of page2_s
struct page2_s {
	/**
	 * offset 0
	 */
	uint16_t highSpeedOffsets[TS_SCATTER_OFFSETS_COUNT] = {};
};
static_assert(sizeof(page2_s) == 256);

// end
// this section was generated automatically by rusEFI tool config_definition-all.jar based on gen_config.sh integration/config_page_2.txt
