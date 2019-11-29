/**
 * @file	nissan_primera.h
 *
 * @date Oct 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#pragma once

#if EFI_SUPPORT_NISSAN_PRIMERA

#include "engine_configuration.h"

void setNissanPrimeraEngineConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setNissanPrimeraEngineConfiguration_360(DECLARE_CONFIG_PARAMETER_SIGNATURE);

#endif /* EFI_SUPPORT_NISSAN_PRIMERA */
