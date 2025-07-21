#include "pch.h"
#include "trigger_simulator.h"
#include "mock_trigger_configuration.h"

static float wrap(float angle) {
	if (angle < 0)
		angle += 720;
	return angle;
}

static float getPos(TriggerWaveform *form, int index) {
	if (index < 0) {
		index += form->getSize() / 2;
	}
	return form->getSwitchAngle(2 * index + 1);

}

static float ratios[400];

TEST(trigger, finder) {
    trigger_config_s triggerConfig;
    triggerConfig.type = trigger_type_e::TT_VVT_BOSCH_QUICK_START;

    TriggerWaveform form;
    form.initializeTriggerWaveform(FOUR_STROKE_CAM_SENSOR, triggerConfig, /*isCrank*/false);

    int toothCount = form.getSize() / 2;

    for (int i = 0;i<toothCount;i++) {
    	printf("%d %f\n", i, getPos(&form, i));

    	float duration0 = wrap(getPos(&form, i) - getPos(&form, i - 1));
    	float duration1 = wrap(getPos(&form, i - 1) - getPos(&form, i - 2));

    	float ratio = duration0 / duration1;

    	printf("%d ratio %f\n", i, ratio);
    	ratios[i] = ratio;
    }

    int length = 2;

    for (int sourceIndex =0;sourceIndex<toothCount;sourceIndex++) {

    	for (int gapIndex = 0;gapIndex < length;gapIndex++) {

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
/*
		try {
			expected<uint32_t> res = TriggerStimulatorHelper::findTriggerSyncPoint(form,
					triggerConfiguration,
					decoder);

			printf("GREAT %d\n", sourceIndex);

		} catch (...) {
			printf("Not good %d\n", sourceIndex);
		}
*/
    }




//    setVerboseTrigger(true);

}
