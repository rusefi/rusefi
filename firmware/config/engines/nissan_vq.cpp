/*
 * @file nissan_vq.cpp
 *
 *  Created on: Jul 2, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "nissan_vq.h"

void setHellen121nissanQR(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->trigger.type = TT_NISSAN_QR25;

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;
	engineConfiguration->specs.displacement = 2;
	strcpy(CONFIG(engineCode), "QR");

	engineConfiguration->camInputs[1 * CAMS_PER_BANK] = GPIO_UNASSIGNED;

	engineConfiguration->vvtMode[0] = VVT_NISSAN_VQ;

	// we have this here and not in board_configuration.cpp so that unit test would get this value
	engineConfiguration->invertCamVVTSignal = true;

	engineConfiguration->vvtOffsets[0] = NISSAN_VQ_VVT_OFFSET;

}

void setHellen121nissanVQ(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->trigger.type = TT_NISSAN_VQ35;

	engineConfiguration->specs.cylindersCount = 6;
	engineConfiguration->specs.firingOrder = FO_1_2_3_4_5_6;
	engineConfiguration->specs.displacement = 4;
	strcpy(CONFIG(engineCode), "VQ");

	engineConfiguration->vvtMode[0] = VVT_NISSAN_VQ;

	// we have this here and not in board_configuration.cpp so that unit test would get this value
	engineConfiguration->invertCamVVTSignal = true;

	engineConfiguration->vvtOffsets[0] = NISSAN_VQ_VVT_OFFSET;
	engineConfiguration->vvtOffsets[1 * CAMS_PER_BANK] = NISSAN_VQ_VVT_OFFSET - NISSAN_VQ_CAM_OFFSET;
}
