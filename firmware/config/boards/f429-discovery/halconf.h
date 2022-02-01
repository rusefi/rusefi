/**
 * @file boards/f429-discovery/halconf.h
 *
 * @brief In this header we can override halconf.h.
 *
 * @date Jan 08, 2022
 * @author Andrey Gusakov, 2022
 */

#ifndef _HALCONF_F429_H_
#define _HALCONF_F429_H_

/* this line exists just to include mcuconf.h from THIS directory */
#include "mcuconf.h"

/* you can override some halconf defaults here */

#include "../../../hw_layer/ports/stm32/stm32f4/cfg/halconf.h"

#include "halconf_community.h"

#endif /* _HALCONF_F429_ */
