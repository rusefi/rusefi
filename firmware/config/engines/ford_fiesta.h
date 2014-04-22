/**
 * @file	ford_fiesta.h
 *
 * @date Nov 22, 2013
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#ifndef FORD_FIESTA_H_
#define FORD_FIESTA_H_
#if EFI_SUPPORT_FORD_FIESTA || defined(__DOXYGEN__)

#include "engine_configuration.h"

void setFordFiestaDefaultEngineConfiguration(engine_configuration_s *engineConfiguration);
void setFordFiestaengine_configuration2_s(engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2);

#endif /* EFI_SUPPORT_FORD_FIESTA */
#endif /* FORD_FIESTA_H_ */
