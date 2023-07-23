/**
 * @file value_lookup_stubs.cpp
 * @brief These stubs are used to reduce the Flash amount used for some configs
 *
 * @date Jul 23, 2023
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "value_lookup.h"

float getOutputValueByName(const char * /*name*/) {
	return EFI_ERROR_CODE;
}

float getConfigValueByName(const char * /*name*/) {
	return EFI_ERROR_CODE;
}

bool setConfigValueByName(const char * /*name*/, float /*value*/) {
	return false;
}
