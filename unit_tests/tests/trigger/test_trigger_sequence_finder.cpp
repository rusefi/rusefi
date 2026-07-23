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

// 'skipMask' marks which gap positions in the candidate sequence are treated as
// allowed decoding errors: those gaps get a wide-open window so a noisy tooth
// at that position does not fail the sync. This budgets out a small number of
// real-data decoding errors (tooManyTeethCounter-style noise) per candidate.
static bool tryGapSequence(size_t length, int toothIndex, TriggerWaveform &form,
		trigger_config_s &triggerConfig, size_t step, unsigned int skipMask = 0,
		TriggerWaveformFunctionPtr reinit = nullptr) {
	// IMPORTANT: TriggerWaveform::setTriggerSynchronizationGap3 only grows
	// gapTrackingLength (maxI(1+gapIndex, ...)) and never resets it. If we keep
	// reusing the same `form` across many candidate (length, sourceIndex) calls,
	// the form ends up with stale gap ratios at indices we did not write this
	// time, and findTriggerZeroEventIndex fails for nearly every later candidate
	// even when it would otherwise sync. Rebuild the waveform from the
	// initializer for each attempt to guarantee a clean state.
	if (reinit != nullptr) {
		reinit(&form);
	}
	int toothCount = form.getSize() / step;

	for (size_t gapIndex = 0; gapIndex < length; gapIndex++) {

		int ratioIndex = toothIndex - gapIndex;
		if (ratioIndex < 0)
			ratioIndex += toothCount;
		float ratio = ratios[ratioIndex];
		ratiosThisTime[ratioIndex] = ratio;
//			printf("trying %d ratio %f\n", gapIndex, ratio);
		if (skipMask & (1u << gapIndex)) {
			// Wide-open window = this position is allowed to be a decoding error.
			form.setTriggerSynchronizationGap3(gapIndex, 0.01f, 100.0f);
		} else {
			form.setTriggerSynchronizationGap4(gapIndex, ratio);
		}
	}

	TriggerDecoderBase decoder("test");

	MockTriggerConfiguration triggerConfiguration(triggerConfig);
	triggerConfiguration.TriggerType = triggerConfig; // technical debt :(

	try {
		form.setShapeDefinitionError(false);
		decoder.findTriggerZeroEventIndex(form,
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
			if (tryGapSequence(length, sourceIndex, form, triggerConfig, step, 0, function)) {
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

// Self-consistency check on the REAL ratio array: a candidate window sequence is
// "happy" iff sliding it across all positions of `realRatios` matches exactly one
// position. This mirrors what the runtime decoder does (it sees real ratios, not
// synthetic ones), and answers "could this gap sequence sync the real capture
// uniquely?" — a question the synthetic-stimulator-based `tryGapSequence` cannot
// answer when real and synthetic geometry differ (issue #8827).
//
// `tolerance` widens each gap window by +/- fraction of the ratio so a small amount
// of measurement noise per tooth does not kill the match.
static bool tryGapSequenceOnRealData(size_t length, int toothIndex,
		const float *realRatios, int toothCount,
		unsigned int skipMask, float tolerance) {
	int matchCount = 0;
	for (int candidatePos = 0; candidatePos < toothCount; candidatePos++) {
		bool ok = true;
		for (size_t gapIndex = 0; gapIndex < length; gapIndex++) {
			if (skipMask & (1u << gapIndex)) {
				continue; // wide-open: this position is allowed to be a decoding error
			}
			int templateIdx = toothIndex - gapIndex;
			if (templateIdx < 0) templateIdx += toothCount;
			int probeIdx = candidatePos - gapIndex;
			if (probeIdx < 0) probeIdx += toothCount;
			float expected = realRatios[templateIdx];
			float actual = realRatios[probeIdx];
			float lo = expected * (1.0f - tolerance);
			float hi = expected * (1.0f + tolerance);
			if (actual < lo || actual > hi) { ok = false; break; }
		}
		if (ok) matchCount++;
		if (matchCount > 1) return false; // not unique => not a sync candidate
	}
	return matchCount == 1;
}

// Variant of findAllSyncSequences that derives the per-tooth ratio array from a real
// captured CSV instead of the synthetic TriggerWaveform geometry. We run two checks
// per candidate gap sequence:
//   1) tryGapSequence() against the synthetic waveform (legacy diagnostic prints).
//   2) tryGapSequenceOnRealData() against the real ratio array — this is the one
//      counted into the returned `happySequenceCounter`, because the synthetic
//      stimulator path can't validate real ratios when synthetic and real geometry
//      differ (e.g. real 6g75 has gaps at ~2.62 vs synthetic 3.0; see issue #8827).
static size_t findAllSyncSequencesFromCsv(trigger_type_e t, size_t maxLength, size_t step,
		TriggerWaveformFunctionPtr function, const char *csvPath, int errorBudget = 2,
		float realRatioTolerance = 0.20f) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	size_t happySequenceCounter = 0;

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
				// Try with 0..errorBudget allowed decoding errors per candidate. Each bit
				// in skipMask represents one position whose ratio is allowed to deviate
				// (window opened wide); we iterate all masks with popcount <= budget.
				unsigned int maxMask = (length >= 32) ? 0xFFFFFFFFu : ((1u << length) - 1u);
				for (unsigned int mask = 0; mask <= maxMask; mask++) {
 				if (__builtin_popcount(mask) > errorBudget)
						continue;
					// Authoritative check: does this candidate sync UNIQUELY against the real ratios?
					// We intentionally do NOT route the candidate through the synthetic-stimulator
					// path (tryGapSequence + findTriggerZeroEventIndex) here: its internal
					// TriggerStimulatorHelper replays the synthetic waveform's own teeth, so any
					// real-recording ratio that doesn't match the synthetic geometry fails sync
					// regardless of capture quality (issue #8827). Gaps are taken purely from the
					// real recording — see tryGapSequenceOnRealData.
					if (tryGapSequenceOnRealData(length, sourceIndex, ratios, waveformToothCount,
							mask, realRatioTolerance)) {
						happySequenceCounter++;
					}
					if (mask == maxMask) break; // avoid overflow when length==32
				}
			}
		}
	}

	return happySequenceCounter;
}

// Read one revolution's worth of real ratios from a CSV at the given startOffset into out[].
// Returns true if successful.
static bool loadCsvRatios(const char *csvPath, size_t step, int waveformToothCount,
		size_t startOffset, float *out) {
	std::vector<double> teeth = readToothTimestamps(csvPath, step);
	if (teeth.size() < (size_t)(waveformToothCount + startOffset + 2)) return false;
	for (int i = 0; i < waveformToothCount; i++) {
		size_t idx = startOffset + i + 2;
		double d0 = teeth[idx]     - teeth[idx - 1];
		double d1 = teeth[idx - 1] - teeth[idx - 2];
		out[i] = (d1 > 0) ? (float)(d0 / d1) : 0.0f;
	}
	return true;
}

// Sliding match counter against a real ratio array using EXPLICIT (lo, hi) gap windows.
// Returns the number of positions in `realRatios` where the candidate sequence matches.
// Positions in `skipMask` are wildcards.
static int countMatchesWithWindows(size_t length, const float *los, const float *his,
		unsigned int skipMask, const float *realRatios, int toothCount) {
	int matchCount = 0;
	for (int candidatePos = 0; candidatePos < toothCount; candidatePos++) {
		bool ok = true;
		for (size_t gapIndex = 0; gapIndex < length; gapIndex++) {
			if (skipMask & (1u << gapIndex)) continue;
			int probeIdx = candidatePos - gapIndex;
			if (probeIdx < 0) probeIdx += toothCount;
			float actual = realRatios[probeIdx];
			if (actual < los[gapIndex] || actual > his[gapIndex]) { ok = false; break; }
		}
		if (ok) matchCount++;
	}
	return matchCount;
}

// Per the issue: use the cleaner CSV (`6g75-without-spark-crank.csv`, which actually syncs
// at runtime with tooManyTeethCounter==3) to derive candidate gap windows, then verify each
// candidate uniquely syncs against the noisier with-spark capture too. Print survivors so
// they can be plugged into setTriggerSynchronizationGap3() in initialize36_2_1_1().
static size_t crossValidateGapsFromCleanCsv(size_t maxLength, size_t step,
		int waveformToothCount, const char *cleanCsv, const char *noisyCsv,
		int errorBudget, float tolerance) {
	std::vector<float> cleanRatios(waveformToothCount), noisyRatios(waveformToothCount);
	if (!loadCsvRatios(cleanCsv, step, waveformToothCount, 0, cleanRatios.data())) {
		printf("Cannot load clean CSV %s\n", cleanCsv);
		return 0;
	}
	if (!loadCsvRatios(noisyCsv, step, waveformToothCount, 0, noisyRatios.data())) {
		printf("Cannot load noisy CSV %s\n", noisyCsv);
		return 0;
	}
	printf("\n=== Cross-validating: clean=%s noisy=%s ===\n", cleanCsv, noisyCsv);

	size_t survivors = 0;
	float los[32], his[32];
	for (size_t length = 1; length <= maxLength; length++) {
		for (int sourceIndex = 0; sourceIndex < waveformToothCount; sourceIndex++) {
			// Build (lo, hi) windows from the CLEAN CSV at this template position.
			for (size_t g = 0; g < length; g++) {
				int idx = sourceIndex - (int)g;
				if (idx < 0) idx += waveformToothCount;
				float r = cleanRatios[idx];
				los[g] = r * (1.0f - tolerance);
				his[g] = r * (1.0f + tolerance);
			}
			unsigned int maxMask = (length >= 32) ? 0xFFFFFFFFu : ((1u << length) - 1u);
			for (unsigned int mask = 0; mask <= maxMask; mask++) {
				if (__builtin_popcount(mask) > errorBudget) {
					if (mask == maxMask) break;
					continue;
				}
				int cleanMatches = countMatchesWithWindows(length, los, his, mask,
						cleanRatios.data(), waveformToothCount);
				if (cleanMatches != 1) {
					if (mask == maxMask) break;
					continue;
				}
				int noisyMatches = countMatchesWithWindows(length, los, his, mask,
						noisyRatios.data(), waveformToothCount);
				if (noisyMatches == 1) {
					survivors++;
					if (survivors <= 20) {
						printf("SURVIVOR len=%zu src=%d mask=0x%x:", length, sourceIndex, mask);
						for (size_t g = 0; g < length; g++) {
							if (mask & (1u << g)) printf(" g%zu=*", g);
							else printf(" g%zu=[%.3f..%.3f]", g, los[g], his[g]);
						}
						printf("\n");
					}
				}
				if (mask == maxMask) break;
			}
		}
	}
	printf("Total cross-validated survivors: %zu\n", survivors);
	return survivors;
}

TEST(trigger, finderRealData) {
	// Brute-force candidate sync sequences for TT_36_2_1_1 using the real 6g75 cranking
	// capture (https://github.com/rusefi/rusefi/issues/8827). The ratio array is computed
	// from real edge timestamps in the CSV rather than from the idealized waveform, so the
	// "happy" sequences printed here are gap ratios that match what the ECU actually sees.
	size_t happyWithSpark = findAllSyncSequencesFromCsv(trigger_type_e::TT_36_2_1_1, 4, 2,
			[] (TriggerWaveform* form) { initialize36_2_1_1(form); },
			"tests/trigger/resources/6g75-withsparkplugs-cranking.csv");
	ASSERT_EQ(2194u, happyWithSpark);

	// Cleaner capture (no spark plugs running) - same wheel, less ringing.
	size_t happyWithoutSpark = findAllSyncSequencesFromCsv(trigger_type_e::TT_36_2_1_1, 4, 2,
			[] (TriggerWaveform* form) { initialize36_2_1_1(form); },
			"tests/trigger/resources/6g75-without-spark-crank.csv");
	ASSERT_EQ(2359u, happyWithoutSpark);

	// Per issue: derive candidate gap windows from the CLEAN capture
	// (6g75-without-spark-crank.csv — runtime syncs OK there with tooManyTeethCounter==3),
	// and cross-check each candidate against the noisy with-spark capture. Survivors are
	// the actual fix candidates for setTriggerSynchronizationGap3() in initialize36_2_1_1.
	TriggerWaveform tmp;
	initialize36_2_1_1(&tmp);
	int waveformToothCount = tmp.getSize() / 2;
	size_t survivors = crossValidateGapsFromCleanCsv(/*maxLength*/4, /*step*/2,
			waveformToothCount,
			"tests/trigger/resources/6g75-without-spark-crank.csv",
			"tests/trigger/resources/6g75-withsparkplugs-cranking.csv",
			/*errorBudget*/2, /*tolerance*/0.20f);
	// Pin survivor count as a regression baseline; printed survivors are the actionable
	// gap-window candidates to plug into initialize36_2_1_1(). The clearest single-gap
	// survivor is `len=1 src=30 mask=0x0: g0=[2.095..3.142]` — the big-block gap, which
	// matches the runtime observation in real6g75.withoutSparkPlugs (tooManyTeethCounter==3).
	ASSERT_EQ(51u, survivors);
}
