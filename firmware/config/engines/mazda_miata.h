/**
 * @file	mazda_miata.h
 * @brief	Mazda Miata NA, also 1993 Ford Escort GT engine configuration. The US Escort.
 *
 * @date Oct 31, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 *
 * http://rusefi.com/forum/viewtopic.php?t=537
 * Injectors: Denso 195500-2180, 230-265cc (?), tan, 13.9 ohms
 */

#ifndef MAZDA_MIATA_H_
#define MAZDA_MIATA_H_

#include "engine_configuration.h"

void setFordEscortGt(engine_configuration_s *engineConfiguration);
void setMiata1990(engine_configuration_s *engineConfiguration);
void setMiata1994_d(engine_configuration_s *engineConfiguration);
void setMiata1994_s(engine_configuration_s *engineConfiguration);
void setMiata1996(engine_configuration_s *engineConfiguration);

#endif /* MAZDA_MIATA_H_ */
