/*
 * @file test_issue_898.cpp
 *
 * @date Aug 8, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

static void boardConfigurationForIssue898(engine_configuration_s *engineConfiguration) {
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
}

TEST(issues, issue898) {
	EngineTestHelper eth(TEST_ISSUE_898, &boardConfigurationForIssue898);

	ASSERT_EQ(TRUE, engine->triggerCentral.triggerShape.shapeDefinitionError) << "MRE_MIATA_NA6 shapeDefinitionError";

	ASSERT_EQ( 2,  eth.recentWarnings()->getCount()) << "warningCounter#testFuelSchedulerBug299smallAndMedium";
	ASSERT_EQ(CUSTOM_ERR_BOTH_FRONTS_REQUIRED, eth.recentWarnings()->get(0));
	ASSERT_EQ(CUSTOM_ERR_TRIGGER_SYNC, eth.recentWarnings()->get(1));
}
