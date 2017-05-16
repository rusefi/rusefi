/**
 * @file	mazda_miata.h
 * @brief	Mazda Miata NA, also 1993 Ford Escort GT engine configuration. The US Escort.
 *
 * @date Oct 31, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 *
 * http://rusefi.com/forum/viewtopic.php?t=537
 * Injectors: Denso 195500-2180, 230-265cc (?), tan, 13.9 ohms
 */

#ifndef MAZDA_MIATA_H_
#define MAZDA_MIATA_H_

#include "engine.h"

void common079721_2351(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration);
void setMiata1990(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void setMiata1994_d(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void setMiata1994_s(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void setMiata1996(DECLARE_ENGINE_PARAMETER_SIGNATURE);

#endif /* MAZDA_MIATA_H_ */
