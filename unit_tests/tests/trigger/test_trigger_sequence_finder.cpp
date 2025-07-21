#include "pch.h"
#include "trigger_simulator.h"
#include "mock_trigger_configuration.h"

static float wrap(float angle, float cycle) {
	if (angle < 0)
		angle += cycle;
	return angle;
}

static int toothOffset = 0;

static float getPos(TriggerWaveform *form, int index) {
	if (index < 0) {
		index += form->getSize() / 2;
	}
	return form->getSwitchAngle(2 * index
	+ toothOffset
			);

}

static float ratios[400];

static void runTest(trigger_type_e t) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	trigger_config_s triggerConfig;
	triggerConfig.type = t;

	TriggerWaveform form;
	form.initializeTriggerWaveform(FOUR_STROKE_CAM_SENSOR, triggerConfig, /*isCrank*/
	false);

	operation_mode_e om = form.getWheelOperationMode();
	float cycle = 720 / getCrankDivider(om);

	int toothCount = form.getSize() / 2;

	for (int i = 0; i < toothCount; i++) {
//		printf("%d andle %f\n", i, getPos(&form, i));

		float duration0 = wrap(getPos(&form, i) - getPos(&form, i - 1), cycle);
		float duration1 = wrap(getPos(&form, i - 1) - getPos(&form, i - 2), cycle);

		float ratio = duration0 / duration1;

		printf("%d ratio %f\n", i, ratio);
		ratios[i] = ratio;
	}

	int length = 3;

	for (int sourceIndex = 0; sourceIndex < toothCount; sourceIndex++) {

		for (int gapIndex = 0; gapIndex < length; gapIndex++) {

			int ratioIndex = sourceIndex - gapIndex;
			if (ratioIndex < 0)
				ratioIndex += toothCount;
			float ratio = ratios[ratioIndex];

			printf("trying %d ratio %f\n", gapIndex, ratio);

			form.setTriggerSynchronizationGap4(gapIndex, ratio);

		}

		TriggerDecoderBase decoder("test");

		MockTriggerConfiguration triggerConfiguration(triggerConfig);
		triggerConfiguration.TriggerType = triggerConfig; // technical debt :(

		try {
			form.setShapeDefinitionError(false);
			uint32_t res = decoder.findTriggerZeroEventIndex(form,
					triggerConfiguration);

			if (!form.shapeDefinitionError)
				printf("************** GREAT %d\n", sourceIndex);

		} catch (...) {
			printf("Not good %d\n", sourceIndex);
		}

	}

//    setVerboseTrigger(true);

}

TEST(trigger, finder) {
	toothOffset = 1;
	runTest(trigger_type_e::TT_VVT_BOSCH_QUICK_START);
	toothOffset = 0;
	runTest(trigger_type_e::TT_GM_24x_3);

}
