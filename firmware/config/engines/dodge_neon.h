/**
 * @file	dodge_neon.h
 * @brief	1995 Dodge Neon default engine configuration
 *
 * @date Dec 16, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef DODGE_NEON_H_
#define DODGE_NEON_H_

#if EFI_SUPPORT_DODGE_NEON

#include "engine_configuration.h"

void setDodgeNeon1995EngineConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);

void setDodgeNeonNGCEngineConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);

/**
 * testing or broken CAM sensor option
 */
void setDodgeNeonNGCEngineConfigurationCrankBased(DECLARE_CONFIG_PARAMETER_SIGNATURE);

#endif /* EFI_SUPPORT_DODGE_NEON */

#endif /* DODGE_NEON_H_ */
