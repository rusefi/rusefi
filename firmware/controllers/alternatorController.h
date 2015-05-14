/**
 * @file    alternatorController.h
 * @brief   alternator controller
 *
 * @date Apr 6, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy, (c) 2012-2015
 *
 */
#ifndef ALTERNATORCONTROLLER_H_
#define ALTERNATORCONTROLLER_H_

#include "main.h"
void initAlternatorCtrl(Logging *sharedLogger);
void setAltPFactor(float p);
void showAltInfo(void);

#endif /* ALTERNATORCONTROLLER_H_ */
