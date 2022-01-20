/**
 * @file boards/f429-discovery/mcuconf.h
 *
 * @brief In this header we can override mcuconf.h.
 *
 * @date Jan 08, 2022
 * @author Andrey Gusakov, 2022
 */

#ifndef MCUCONF_F429_H
#define MCUCONF_F429_H

#include "../../../hw_layer/ports/stm32/stm32f4/cfg/mcuconf.h"

/*
 * USB driver system settings.
 */
#undef STM32_USB_USE_OTG1
#define STM32_USB_USE_OTG1                  FALSE

#undef STM32_USB_USE_OTG2
#define STM32_USB_USE_OTG2                  TRUE

#endif /* MCUCONF_F429_H */
