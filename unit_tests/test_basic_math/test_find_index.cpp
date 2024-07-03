/*
 * test_find_index.cpp
 *
 *  Created on: Oct 30, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "efi_interpolation.h"

TEST(misc, testSetTableValue) {
	printf("*************************************************** testSetTableValue\r\n");

	persistent_config_s cfg;

	for (int i = 0; i < CLT_CURVE_SIZE; i++) {
		cfg.cltFuelCorrBins[i] = -40 + i * 10;
		cfg.cltFuelCorr[i] = 1;
	}

	ASSERT_EQ(1, cfg.cltFuelCorr[0]);

	setCurveValue(cfg.cltFuelCorrBins, cfg.cltFuelCorr, CLT_CURVE_SIZE, -40, 1.5);
	ASSERT_FLOAT_EQ(1.5, cfg.cltFuelCorr[0]);

	setCurveValue(cfg.cltFuelCorrBins, cfg.cltFuelCorr, CLT_CURVE_SIZE, -50, 1.4);
	ASSERT_FLOAT_EQ(1.4, cfg.cltFuelCorr[0]);

}
