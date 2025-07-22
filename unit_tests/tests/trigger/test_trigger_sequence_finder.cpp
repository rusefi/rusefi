#include "pch.h"
#include "trigger_simulator.h"
#include "mock_trigger_configuration.h"
#include "trigger_gm.h"
#include "trigger_universal.h"

typedef void (*TriggerWaveformFunctionPtr)(TriggerWaveform*);

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
	return form->getSwitchAngle(2 * index + toothOffset);
}

static float ratios[400];
static float ratiosThisTime[400];

static bool tryGapSequence(size_t length, int toothIndex, TriggerWaveform &form,
		trigger_config_s &triggerConfig) {
	int toothCount = form.getSize() / 2;

	for (int gapIndex = 0; gapIndex < length; gapIndex++) {

		int ratioIndex = toothIndex - gapIndex;
		if (ratioIndex < 0)
			ratioIndex += toothCount;
		float ratio = ratios[ratioIndex];
		ratiosThisTime[ratioIndex] = ratio;
//			printf("trying %d ratio %f\n", gapIndex, ratio);
    form.setTriggerSynchronizationGap4(gapIndex, ratio);
	}

	TriggerDecoderBase decoder("test");

	MockTriggerConfiguration triggerConfiguration(triggerConfig);
	triggerConfiguration.TriggerType = triggerConfig; // technical debt :(

	try {
		form.setShapeDefinitionError(false);
		uint32_t res = decoder.findTriggerZeroEventIndex(form,
				triggerConfiguration);

		if (!form.shapeDefinitionError) {
			for (int i = 0; i < length; i++) {
				float ratio = ratiosThisTime[i];
				printf("happy ratio %f @ %d\n", ratio, i);
			}
			printf("************** GREAT length=%d at tooth index %d\n", length,
					toothIndex);
			return true;
		}

	} catch (...) {
		printf("Not good sync tooth index %d\n", toothIndex);
	}

	return false;
}

static size_t findAllSyncSequences(trigger_type_e t, size_t maxLength,
		TriggerWaveformFunctionPtr function) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	size_t happySequenceCounter = 0;

	trigger_config_s triggerConfig;
	triggerConfig.type = t;

	TriggerWaveform form;
	function(&form);

	operation_mode_e om = form.getWheelOperationMode();
	float cycle = 720 / getCrankDivider(om);

	int toothCount = form.getSize() / 2;

	for (int i = 0; i < toothCount; i++) {
//		printf("%d angle %f\n", i, getPos(&form, i));

		float duration0 = wrap(getPos(&form, i) - getPos(&form, i - 1), cycle);
		float duration1 = wrap(getPos(&form, i - 1) - getPos(&form, i - 2),
				cycle);

		float ratio = duration0 / duration1;

		printf("%d ratio %f\n", i, ratio);
		ratios[i] = ratio;
	}

	for (size_t length = 1; length <= maxLength; length++) {
		for (int sourceIndex = 0; sourceIndex < toothCount; sourceIndex++) {
			if (tryGapSequence(length, sourceIndex, form, triggerConfig)) {
				happySequenceCounter++;
			}
		}
	}
//    setVerboseTrigger(true);
	return happySequenceCounter;
}

TEST(trigger, finder) {
	toothOffset = 1; // todo: make this smarter?
	ASSERT_EQ(9, findAllSyncSequences(trigger_type_e::TT_VVT_BOSCH_QUICK_START, 3, [] (TriggerWaveform* form) {
						configureQuickStartSenderWheel(form);
					}));

	toothOffset = 0; // todo: make this smarter?
	ASSERT_EQ(27, findAllSyncSequences(trigger_type_e::TT_GM_24x_3, 3, [] (TriggerWaveform* form) {
						initGmLS24_3deg(form);
					}));

}
