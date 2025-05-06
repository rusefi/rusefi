/*
 * @file nissan_vq.cpp
 *
 *  Created on: Jul 2, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"

#include "nissan_vq.h"
#include "hellen_meta.h"
#include "defaults.h"

#define NISSAN_VQ_VVT_OFFSET 157

void setHellen121nissanQR() {
	engineConfiguration->trigger.type = trigger_type_e::TT_NISSAN_QR25;

	setInline4();
	engineConfiguration->displacement = 2;
	strcpy(engineConfiguration->engineCode, "QR");

	engineConfiguration->camInputs[1 * CAMS_PER_BANK] = Gpio::Unassigned;

	engineConfiguration->vvtMode[0] = VVT_NISSAN_VQ;

	// we have this here and not in board_configuration.cpp so that unit test would get this value
	engineConfiguration->invertCamVVTSignal = true;

	engineConfiguration->vvtOffsets[0] = 157;

}

static void setNissanVqEngineConfig() {
	setNissanMAF0031();
	engineConfiguration->trigger.type = trigger_type_e::TT_NISSAN_VQ35;

	engineConfiguration->cylindersCount = 6;
	engineConfiguration->firingOrder = FO_1_2_3_4_5_6;
	engineConfiguration->displacement = 4;
	strcpy(engineConfiguration->engineCode, "VQ");

	engineConfiguration->vvtMode[0] = VVT_NISSAN_VQ;

	engineConfiguration->vvtOffsets[0 * CAMS_PER_BANK] = NISSAN_VQ_VVT_OFFSET;
	engineConfiguration->vvtOffsets[1 * CAMS_PER_BANK] = NISSAN_VQ_VVT_OFFSET + NISSAN_VQ_CAM_OFFSET;

	// VVT closed loop
	engineConfiguration->auxPid[0].pFactor = 2;
	engineConfiguration->auxPid[0].iFactor = 0.5;
	engineConfiguration->auxPid[0].dFactor = 0.05;
	engineConfiguration->auxPid[0].offset = 50;
//	engineConfiguration->auxPid[0].minValue = 20;
//	engineConfiguration->auxPid[0].maxValue = 90;

	setTable(config->crankingCycleBaseFuel, 35);
}

void setHellen121nissanVQ() {
    #include "nissan_vq.lua"

    setNissanVqEngineConfig();

	// we have this here and not in board_configuration.cpp so that unit test would get this value
	engineConfiguration->invertCamVVTSignal = true;

#if HW_HELLEN && EFI_PROD_CODE
	engineConfiguration->vvtPins[0 * CAMS_PER_BANK] = H176_LS_7;
	engineConfiguration->vvtPins[1 * CAMS_PER_BANK] = H176_LS_8;
#endif
}

void setProteusNissanVQ() {
    setNissanVqEngineConfig();

    // open question if that's a board setting or engine setting
	engineConfiguration->invertCamVVTSignal = true;
}

void setHellen121nissanAltimaN16() {
	// https://en.wikipedia.org/wiki/Nissan_QG_engine
	// https://en.wikipedia.org/wiki/N-VCT
	setHellen121nissanQR();
	engineConfiguration->displacement = 1.8;
	strcpy(engineConfiguration->engineCode, "N16");

}
