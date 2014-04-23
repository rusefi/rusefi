/**
 * @file	ford_fiesta.h
 *
 * @date Nov 22, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef FORD_FIESTA_H_
#define FORD_FIESTA_H_
#if EFI_SUPPORT_FORD_FIESTA || defined(__DOXYGEN__)

#include "engine_configuration.h"

void setFordFiestaDefaultEngineConfiguration(engine_configuration_s *engineConfiguration);

#endif /* EFI_SUPPORT_FORD_FIESTA */
#endif /* FORD_FIESTA_H_ */
