/**
 * @file malfunction_indicator.h
 * @brief We can blink out OBD-II error codes using Malfunction Indicator Light (MIL)
 *
 *
 * @date Dec 20, 2013
 * @author Konstantin Nikonenko
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#if EFI_MALFUNCTION_INDICATOR

bool isMilEnabled();
void initMalfunctionIndicator(void);

#endif /* EFI_MALFUNCTION_INDICATOR */

