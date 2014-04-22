/**
 * @file	nissan_primera.h
 *
 * @date Oct 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#include "efifeatures.h"

#ifndef NISSAN_PRIMERA_H_
#define NISSAN_PRIMERA_H_

#if EFI_SUPPORT_NISSAN_PRIMERA

#include "engine_configuration.h"

void setNissanPrimeraEngineConfiguration(engine_configuration_s *engineConfiguration);
void setNissanPrimeraengine_configuration2_s(engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2);

#endif /* EFI_SUPPORT_NISSAN_PRIMERA */

#endif /* NISSAN_PRIMERA_H_ */
