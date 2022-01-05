/**
 * @file boards/f429-discovery/halconf_community.h
 *
 * @brief In this header we can override defaults from halconf_community.h.
 *
 * @date Jan 08, 2022
 * @author Andrey Gusakov, 2022
 */

#ifndef HALCONF_COMMUNITY_F429_H
#define HALCONF_COMMUNITY_F429_H

/* Enable SDRAM support */
#define HAL_USE_FSMC                TRUE
#define HAL_USE_SDRAM               TRUE

/* all other options are default */
#include "../../../hw_layer/ports/stm32/cfg/halconf_community.h"

#endif /* HALCONF_COMMUNITY_F429_H */
