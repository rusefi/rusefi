/*
 * boost_controller.h
 *
 *  Created on: 14. aug. 2019
 *      Author: Ola
 */

#ifndef CONTROLLERS_ACTUATORS_BOOST_CONTROLLER_H_
#define CONTROLLERS_ACTUATORS_BOOST_CONTROLLER_H_


#include "engine.h"
void initBoostCtrl(Logging *sharedLogger);
void setBstPFactor(float p);
void setBstIFactor(float p);
void setBstDFactor(float p);
void setDefaultBoostParameters(void);

void onConfigurationChangeBoostCallback(engine_configuration_s *previousConfiguration);



#endif /* CONTROLLERS_ACTUATORS_BOOST_CONTROLLER_H_ */
