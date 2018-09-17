/**
 * @file	nissan_primera.h
 *
 * @date Oct 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef NISSAN_PRIMERA_H_
#define NISSAN_PRIMERA_H_

#if EFI_SUPPORT_NISSAN_PRIMERA

#include "engine_configuration.h"

void setNissanPrimeraEngineConfiguration(engine_configuration_s *engineConfiguration);
void setNissanPrimeraEngineConfiguration_360(engine_configuration_s *engineConfiguration);

#endif /* EFI_SUPPORT_NISSAN_PRIMERA */

#endif /* NISSAN_PRIMERA_H_ */
