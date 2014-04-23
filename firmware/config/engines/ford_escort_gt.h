/**
 * @file	ford_escort_gt.h
 * @brief	1993 Ford Escort GT engine configuration. The US Escort.
 *
 * @date Oct 31, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 *
 * http://rusefi.com/forum/viewtopic.php?t=537
 * Injectors: Denso 195500-2180, 230-265cc (?), tan, 13.9 ohms
 */

#ifndef FORD_ESCORT_GT_H_
#define FORD_ESCORT_GT_H_

#include "engine_configuration.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void setFordEscortGt(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORD_ESCORT_GT_H_ */
