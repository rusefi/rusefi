/*
 * @file persistent_configuration.h
 *
 * @date Feb 27, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "generated_lookup_engine_configuration.h"

#include "rusefi/crc.h"

#ifdef EFI_PERSISTENT_CUSTOM_FIELD_HEADER
#include EFI_PERSISTENT_CUSTOM_FIELD_HEADER
#endif

// todo: ChibiOS has pretty similar functionality for persistent container, we probably should reuse it one day?
struct persistent_config_container_s {
	int version{};
	int size{};
	persistent_config_s persistentConfiguration{};
	uint32_t crc{};
#ifdef EFI_PERSISTENT_CUSTOM_FIELD_HEADER
	// after 'crc' so that the offsets of everything above do not depend on the board option
	persistent_custom_field_s customField{};
#endif

	uint32_t getCrc() {
		return crc32(&persistentConfiguration, sizeof(persistent_config_s));
	}
};
