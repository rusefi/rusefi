/*
 * @file persistent_configuration.h
 *
 * @date Feb 27, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "globalaccess.h"
#include "crc.h"
#include "engine_configuration_generated_structures.h"

typedef struct {
	uint32_t version;
	size_t size;
	persistent_config_s persistentConfiguration;
	crc_t value;
} persistent_config_container_s;
