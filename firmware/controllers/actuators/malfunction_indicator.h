/**
 * @file malfunction_indicator.h
 * @brief We can blink out OBD-II error codes using Malfunction Indicator Light (MIL)
 *
 *
 * @date Dec 20, 2013
 * @author Konstantin Nikonenko
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef MALFUNCTION_INDICATOR_H_
#define MALFUNCTION_INDICATOR_H_

#include "global.h"
#include "engine.h"

#if EFI_MALFUNCTION_INDICATOR || defined(__DOXYGEN__)

bool isMilEnabled();
void initMalfunctionIndicator(void);

#endif /* EFI_MALFUNCTION_INDICATOR */

#endif /* MALFUNCTION_INDICATOR_H_ */
