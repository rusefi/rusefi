/*
 * @file nissan_vq.cpp
 *
 *  Created on: Jul 2, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "nissan_vq.h"

void setHellen121nissan(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->trigger.type = TT_NISSAN_VQ;

	engineConfiguration->vvtMode[0] = VVT_NISSAN_VQ;
}
