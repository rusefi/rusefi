/*
 * @file nissan_vq.h
 *
 *  Created on: Jul 2, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#pragma once

#include "engine_configuration.h"

#define NISSAN_VQ_VVT_OFFSET -610
#define NISSAN_VQ_CAM_OFFSET -360

void setHellen121nissanVQ(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setHellen121nissanQR(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setHellen121nissanAltimaN16(DECLARE_CONFIG_PARAMETER_SIGNATURE);
