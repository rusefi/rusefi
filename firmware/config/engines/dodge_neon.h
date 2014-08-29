/**
 * @file	dodge_neon.h
 * @brief	1995 Dodge Neon default engine configuration
 *
 * @date Dec 16, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef DODGE_NEON_H_
#define DODGE_NEON_H_

#if EFI_SUPPORT_DODGE_NEON

#include "engine_configuration.h"

void setDodgeNeonEngineConfiguration(engine_configuration_s *engineConfiguration,
		board_configuration_s *boardConfiguration);

#endif /* EFI_SUPPORT_DODGE_NEON */

#endif /* DODGE_NEON_H_ */
