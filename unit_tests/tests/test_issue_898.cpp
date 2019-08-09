/*
 * test_issue_898.cpp
 *
 *  Created on: Aug 8, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "engine_test_helper.h"
extern WarningCodeState unitTestWarningCodeState;

static void boardConfigurationForIssue898(engine_configuration_s *engineConfiguration) {
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
}

TEST(issues, issue898) {
	EngineTestHelper eth(ISSUE_898, &boardConfigurationForIssue898);
	EXPAND_EngineTestHelper;

	ASSERT_EQ(TRUE, engine->triggerCentral.triggerShape.shapeDefinitionError) << "MRE_MIATA_NA6 shapeDefinitionError";

	ASSERT_EQ( 2,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#testFuelSchedulerBug299smallAndMedium";
	ASSERT_EQ(CUSTOM_ERR_BOTH_FRONTS_REQUIRED, unitTestWarningCodeState.recentWarnings.get(0));
	ASSERT_EQ(CUSTOM_ERR_TRIGGER_SYNC, unitTestWarningCodeState.recentWarnings.get(1));
}
