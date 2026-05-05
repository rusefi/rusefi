#include "pch.h"
#include "trigger_simulator.h"
#include "mock_trigger_configuration.h"
#include "trigger_gm.h"
#include "trigger_universal.h"
#include "trigger_mitsubishi.h"

#include <vector>
#include <cstdio>
#include <cstring>

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

    // Brute-force candidate sync sequences for the broken TT_36_2_1_1 decoder.
    // See https://github.com/rusefi/rusefi/issues/8827 and unit_tests/tests/trigger/test_real_6g75.cpp
    // The "happy" sequences printed by this call are the candidate gap ratios to plug
    // into setTriggerSynchronizationGap*() in initialize36_2_1_1() in trigger_mitsubishi.cpp.
    findAllSyncSequences(trigger_type_e::TT_36_2_1_1, 4, 2, [] (TriggerWaveform* form) {
        initialize36_2_1_1(form);
    });
}

// Read tooth-edge timestamps from a CsvReader-format trigger capture.
// step == 2: keep only rising edges of channel 0 (one entry per tooth).
// step == 1: keep both edges (entry per edge).
static std::vector<double> readToothTimestamps(const char *fileName, size_t step) {
	std::vector<double> result;
	FILE *fp = fopen(fileName, "r");
	if (fp == nullptr) {
		printf("Cannot open %s\n", fileName);
		return result;
	}
	char buffer[255];
	bool first = true;
	int prevState = -1;
	while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
		if (first) { first = false; continue; } // header
		char *tsTok = strtok(buffer, ",");
		char *chTok = strtok(nullptr, ",");
		if (tsTok == nullptr || chTok == nullptr) continue;
		while (*chTok == ' ') chTok++;
		double ts = std::atof(tsTok);
		int state = (chTok[0] == '1') ? 1 : 0;
		if (prevState == -1) {
			prevState = state;
			continue;
		}
		bool isRise = (prevState == 0 && state == 1);
		bool isEdge = (prevState != state);
		if (step == 2 ? isRise : isEdge) {
			result.push_back(ts);
		}
		prevState = state;
	}
	fclose(fp);
	return result;
}

// Variant of findAllSyncSequences that derives the per-tooth ratio array from a real
// captured CSV instead of the synthetic TriggerWaveform geometry. The brute-force
// validation still runs against the synthetic waveform via tryGapSequence(), so any
// "happy" sequence printed is a candidate set of sync gaps that synchronizes the
// existing decoder against the actual recorded tooth pattern.
static size_t findAllSyncSequencesFromCsv(trigger_type_e t, size_t maxLength, size_t step,
		TriggerWaveformFunctionPtr function, const char *csvPath) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	size_t happySequenceCounter = 0;

	trigger_config_s triggerConfig;
	triggerConfig.type = t;

	TriggerWaveform form;
	function(&form);

	int waveformToothCount = form.getSize() / step;

	std::vector<double> teeth = readToothTimestamps(csvPath, step);
	printf("Loaded %zu tooth edges from %s (step=%zu)\n", teeth.size(), csvPath, step);
	if (teeth.size() < (size_t)(waveformToothCount + 2)) {
		printf("Not enough teeth in CSV to form a cycle\n");
		return 0;
	}

	// Use a single revolution worth of real teeth as the "ratio[]" lookup. We try a
	// few different starting offsets in the CSV in case the first cycle is noisy.
	size_t offsetsToTry = std::min<size_t>(waveformToothCount, teeth.size() - waveformToothCount - 2);

	for (size_t startOffset = 0; startOffset < offsetsToTry; startOffset++) {
		printf("\n=== CSV start offset %zu ===\n", startOffset);
		for (int i = 0; i < waveformToothCount; i++) {
			size_t idx = startOffset + i + 2;
			double d0 = teeth[idx]     - teeth[idx - 1];
			double d1 = teeth[idx - 1] - teeth[idx - 2];
			float ratio = (d1 > 0) ? (float)(d0 / d1) : 0.0f;
			ratios[i] = ratio;
			printf("real %d ratio %f (d0=%.6f d1=%.6f)\n", i, ratio, d0, d1);
		}

		for (size_t length = 1; length <= maxLength; length++) {
			for (int sourceIndex = 0; sourceIndex < waveformToothCount; sourceIndex++) {
				if (tryGapSequence(length, sourceIndex, form, triggerConfig, step)) {
					happySequenceCounter++;
				}
			}
		}
	}

	return happySequenceCounter;
}

TEST(trigger, finderRealData) {
	// Brute-force candidate sync sequences for TT_36_2_1_1 using the real 6g75 cranking
	// capture (https://github.com/rusefi/rusefi/issues/8827). The ratio array is computed
	// from real edge timestamps in the CSV rather than from the idealized waveform, so the
	// "happy" sequences printed here are gap ratios that match what the ECU actually sees.
	findAllSyncSequencesFromCsv(trigger_type_e::TT_36_2_1_1, 4, 2,
			[] (TriggerWaveform* form) { initialize36_2_1_1(form); },
			"tests/trigger/resources/6g75-withsparkplugs-cranking.csv");
}
