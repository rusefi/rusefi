#include "pch.h"
#include "trigger_simulator.h"
#include "mock_trigger_configuration.h"
#include "trigger_gm.h"
#include "trigger_universal.h"
#include "trigger_mitsubishi.h"

typedef void (*TriggerWaveformFunctionPtr)(TriggerWaveform*);

static float wrap(float angle, float cycle) {
	if (angle < 0)
		angle += cycle;
	return angle;
}

static int toothOffset = 0;

static float getPos(TriggerWaveform *form, int index, size_t step) {
	if (index < 0) {
		index += form->getSize() / step;
	}
	return form->getSwitchAngle(step * index + toothOffset);
}

static float ratios[400];
static float ratiosThisTime[400];

static bool tryGapSequence(size_t length, int toothIndex, TriggerWaveform &form,
		trigger_config_s &triggerConfig, size_t step) {
	int toothCount = form.getSize() / step;

	for (size_t gapIndex = 0; gapIndex < length; gapIndex++) {

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
			for (int i = 0; i < (int)length; i++) {
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

static size_t findAllSyncSequences(trigger_type_e t, size_t maxLength, size_t step,
		TriggerWaveformFunctionPtr function) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	size_t happySequenceCounter = 0;

	trigger_config_s triggerConfig;
	triggerConfig.type = t;

	TriggerWaveform form;
	function(&form);
	toothOffset = (form.syncEdge == SyncEdge::Rise || form.syncEdge == SyncEdge::Both ? 0 : 1);

	operation_mode_e om = form.getWheelOperationMode();
	float cycle = 720 / getCrankDivider(om);

	int toothCount = form.getSize() / step;

	for (int i = 0; i < toothCount; i++) {
//		printf("%d angle %f\n", i, getPos(&form, i));

		float duration0 = wrap(getPos(&form, i, step) - getPos(&form, i - 1, step), cycle);
		float duration1 = wrap(getPos(&form, i - 1, step) - getPos(&form, i - 2, step),
				cycle);

		float ratio = duration0 / duration1;

		printf("%d ratio %f\n", i, ratio);
		ratios[i] = ratio;
	}

	for (size_t length = 1; length <= maxLength; length++) {
		for (int sourceIndex = 0; sourceIndex < toothCount; sourceIndex++) {
			if (tryGapSequence(length, sourceIndex, form, triggerConfig, step)) {
				happySequenceCounter++;
			}
		}
	}
//    setVerboseTrigger(true);
	return happySequenceCounter;
}

TEST(trigger, finder) {
    // step - 1 - for both, 2 - for rise/fall only

	ASSERT_EQ(9u, findAllSyncSequences(trigger_type_e::TT_VVT_BOSCH_QUICK_START, 3, 2, [] (TriggerWaveform* form) {
						configureQuickStartSenderWheel(form);
					}));

	ASSERT_EQ(27u, findAllSyncSequences(trigger_type_e::TT_GM_24x_3, 3, 2, [] (TriggerWaveform* form) {
						initGmLS24_3deg(form);
					}));

    ASSERT_EQ(44u, findAllSyncSequences(trigger_type_e::TT_VVT_MITSU_6G72, 8, 1, [] (TriggerWaveform* form) {
        initializeVvt6G72(form);
    }));
}
