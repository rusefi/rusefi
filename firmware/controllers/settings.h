/**
 * @file settings.h
 * @brief This file is about configuring engine via the human-readable protocol
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef INJECTOR_CONTROL_H_
#define INJECTOR_CONTROL_H_

#include "engine.h"

void initSettings(const engine_configuration_s *engineConfiguration);
void printSpiState(Logging *logger, board_configuration_s *boardConfiguration);
void printConfiguration(const engine_configuration_s *engineConfiguration);
void stopEngine(void);
void setEngineType(int value);
const char* getConfigurationName(engine_type_e engineType);

#endif /* INJECTOR_CONTROL_H_ */
