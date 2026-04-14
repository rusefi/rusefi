/**
 * @file test_all_triggers.cpp
 */
#include "pch.h"
#include "trigger_meta_generated.h"
#include "auto_generated_sync_edge.h"

// uncomment to test starting from specific trigger
//#define TEST_FROM_TRIGGER_ID ((int)trigger_type_e::TT_MAZDA_MIATA_NA)
// uncomment to test only newest trigger
//#define TEST_FROM_TRIGGER_ID ((int)trigger_type_e::TT_UNUSED - 1)
#define TEST_FROM_TRIGGER_ID 1

#define TEST_TO_TRIGGER_ID trigger_type_e::TT_UNUSED
// uncomment to test only one trigger
//#define TEST_TO_TRIGGER_ID (TEST_FROM_TRIGGER_ID + 1)

class AllTriggersFixture : public ::testing::TestWithParam<int> {
public:
	static FILE* fp;

	static void SetUpTestSuite() {
	}

	static void TearDownTestSuite() {
	}
};

FILE* AllTriggersFixture::fp = nullptr;

INSTANTIATE_TEST_SUITE_P(
	Triggers,
	AllTriggersFixture,
	// Test all triggers from the first valid trigger thru the last
	// (Skip index 0, that's custom toothed wheel which is covered by others)
	::testing::Range((int)TEST_FROM_TRIGGER_ID, (int)TEST_TO_TRIGGER_ID)
);

extern bool printTriggerDebug;
extern bool printTriggerTrace;

TEST_P(AllTriggersFixture, TestTrigger) {
	// handy debugging options
	//printTriggerDebug = true;
	//printTriggerTrace = true;

	trigger_type_e tt = (trigger_type_e)GetParam();

#if EFI_UNIT_TEST
	extern TriggerDecoderBase initState;
#endif // EFI_UNIT_TEST

	persistent_config_s pc{};
	Engine e;
	Engine* engine = &e;
	EngineTestHelperBase base(engine, &pc.engineConfiguration, &pc);

#if EFI_UNIT_TEST
	for (size_t i = 0; i < efi::size(initState.gapRatio); i++) {
		initState.gapRatio[i] = NAN;
	}
#endif // EFI_UNIT_TEST

	engineConfiguration->trigger.type = tt;
	setCamOperationMode();

	initDataStructures();

	TriggerWaveform *shape = &engine->triggerCentral.triggerShape;
	TriggerFormDetails *triggerFormDetails = &engine->triggerCentral.triggerFormDetails;
	try {
		engine->updateTriggerConfiguration();
	} catch (...) {
		GTEST_FAIL() << "Trigger type " << (int)tt << " threw exception during initialization.";
	}

	if (shape->shapeDefinitionError) {
		// Some trigger types are not yet implemented - skip them
		GTEST_FAIL() << "Trigger type " << (int)tt << " has shapeDefinitionError.";
	}

	int zeroCount = 0;

	for (size_t i = 0; i < shape->getLength(); i++) {
		int triggerDefinitionCoordinate = (shape->getTriggerWaveformSynchPointIndex() + i) % shape->getSize();
		if (triggerFormDetails->eventAngles[i] == 0.0f && shape->triggerSignalStates[triggerDefinitionCoordinate] == TriggerValue::FALL){
			zeroCount++;
		}
	}

	ASSERT_LE(zeroCount, 1);
}
