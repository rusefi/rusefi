/**
 * @file boards/S105/efifeatures.h
 *
 * @brief In this header we can override efifeatures.h.
 *
 * @date Jan 01, 2023
 * @author Andrey Gusakov, 2023
 */

#include "../../stm32f4ems/efifeatures.h"

#ifndef EFIFEATURES_S105_H_
#define EFIFEATURES_S105_H_

/* This board has no USB wired out */
#undef EFI_USB_SERIAL
#define EFI_USB_SERIAL				FALSE

#endif /* EFIFEATURES_S105_H_ */