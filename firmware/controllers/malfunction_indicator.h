/**
 * @file malfunction_indicator.h
 * @brief We can blink out OBD-II error codes using Malfunction Indicator Light (MIL)
 *
 *
 * @date Dec 20, 2013
 * @author Konstantin Nikonenko
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef MALFUNCTION_INDICATOR_H_
#define MALFUNCTION_INDICATOR_H_

#include "main.h"

#if EFI_MALFUNCTIONAL_INDICATOR

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void initMalfunctionIndicator(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


#endif /* MALFUNCTION_INDICATOR_H_ */
