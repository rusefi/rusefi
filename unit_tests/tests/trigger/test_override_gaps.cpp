/*
 * @file test_override_gaps.cpp
 *
 *  Created on: May 25, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"

TEST(subaru, overrideGap) {
	EngineTestHelper eth(FRANKENSO_MIATA_NA6_MAP);

	CONFIG(overrideTriggerGaps) = true;
	CONFIG(gapTrackingLengthOverride) = 2;
	CONFIG(triggerGapOverrideFrom[0]) = 0.75 * 11.3;
	CONFIG(triggerGapOverrideTo[0]) = 1.75 * 11.3;
	CONFIG(triggerGapOverrideFrom[1]) = 0.75;
	CONFIG(triggerGapOverrideTo[1]) = 1.25;

	eth.setTriggerType(TT_SUBARU_7_WITHOUT_6);

	ASSERT_EQ(2, engine->triggerCentral.triggerShape.gapTrackingLength);

	ASSERT_EQ(FALSE, engine->triggerCentral.triggerShape.shapeDefinitionError) << "overrideGap shapeDefinitionError";

}



