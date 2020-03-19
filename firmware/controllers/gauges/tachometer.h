/*
 * @file tachometer.h
 *
 * @date Aug 18, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#ifndef CONTROLLERS_TACHOMETER_H_
#define CONTROLLERS_TACHOMETER_H_

#if !EFI_UNIT_TEST
#include "engine.h"
#endif

void initTachometer(DECLARE_ENGINE_PARAMETER_SIGNATURE);

#endif /* CONTROLLERS_TACHOMETER_H_ */
