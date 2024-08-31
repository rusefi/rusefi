#include "pch.h"

using ::testing::_;

static const angle_t timing = 6; // same timing cranking and running modes

TEST(threeCylinders, crankWastedSpark) {
  EngineTestHelper eth(engine_type_e::MITSUBISHI_3A92);
	setTable(config->ignitionTable, timing); // this changes run mode timing
	engineConfiguration->crankingTimingAngle = timing;
	engine->tdcMarkEnabled = false; // reduce event queue noise TODO extract helper method

  engineConfiguration->isInjectionEnabled = false; // reduce event queue / less distraction

  // just simpler to use trivial trigger in tests, does not make a difference for actual scheduling
  eth.setTriggerType(trigger_type_e::TT_HALF_MOON);


  // see 'test_odd_firing_engine.cpp' which is the nicest relevant test at the moment
  // see https://github.com/rusefi/rusefi/issues/4195#issuecomment-2322760547
}
