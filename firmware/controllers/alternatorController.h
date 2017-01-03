/**
 * @file    alternatorController.h
 * @brief   alternator controller
 *
 * @date Apr 6, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy, (c) 2012-2017
 *
 */
#ifndef ALTERNATORCONTROLLER_H_
#define ALTERNATORCONTROLLER_H_

#include "main.h"
void initAlternatorCtrl(Logging *sharedLogger);
void setAltPFactor(float p);
void showAltInfo(void);
void setDefaultAlternatorParameters(void);

void onConfigurationChangeAlternatorCallback(engine_configuration_s *previousConfiguration);

#endif /* ALTERNATORCONTROLLER_H_ */
