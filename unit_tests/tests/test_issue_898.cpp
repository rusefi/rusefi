/*
 * test_issue_898.cpp
 *
 *  Created on: Aug 8, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "engine_test_helper.h"

static void boardConfigurationForIssue898(engine_configuration_s *engineConfiguration) {
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
}

TEST(issues, issue898) {
// works without extra board settings
	EngineTestHelper eth(MRE_MIATA_NA6);
// fails like this with self-contradictory trigger definition
//	EngineTestHelper eth(MRE_MIATA_NA6, &boardConfigurationForIssue898);
	EXPAND_EngineTestHelper;


	ASSERT_EQ(0, engine->triggerCentral.triggerShape.shapeDefinitionError) << "MRE_MIATA_NA6 shapeDefinitionError";
}
