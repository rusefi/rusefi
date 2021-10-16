/*
 * @file test_override_gaps.cpp
 *
 *  Created on: May 25, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"

TEST(subaru, overrideGap) {
	WITH_ENGINE_TEST_HELPER(MIATA_NA6_MAP);

	CONFIG(overrideTriggerGaps) = true;
	CONFIG(gapTrackingLengthOverride) = 2;
	CONFIG(triggerGapOverride[0]) = 11.3;
	CONFIG(triggerGapOverride[1]) = 1;

	eth.setTriggerType(TT_SUBARU_7_WITHOUT_6 PASS_ENGINE_PARAMETER_SUFFIX);

	ASSERT_EQ(2, engine->triggerCentral.triggerShape.gapTrackingLength);

	ASSERT_EQ(FALSE, engine->triggerCentral.triggerShape.shapeDefinitionError) << "overrideGap shapeDefinitionError";

}



