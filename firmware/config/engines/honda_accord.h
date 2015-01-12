/**
 * @file	honda_accord.h
 *
 * @date Jan 12, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef HONDA_ACCORD_H_
#define HONDA_ACCORD_H_

#include "engine_configuration.h"

void setFrankenso_01_LCD(board_configuration_s *boardConfiguration);
void setHondaAccordConfigurationTwoWires(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration);
void setHondaAccordConfigurationThreeWires(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration);
void setHondaAccordConfigurationDip(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration);

#endif /* HONDA_ACCORD_H_ */
