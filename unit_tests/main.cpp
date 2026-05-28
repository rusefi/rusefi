/**
 * @file main.cpp
 * @file Unit tests (and some integration tests to be fair) of rusEFI
 *
 * @author Andrey Belomutskiy, (c) 2012-2021
 */


#include "pch.h"
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "can_msg_tx.h"
#include "unit_test_logger.h"

#include <chrono>
#include <string>
#include <unordered_set>

bool hasInitGtest = false;

#if EFI_SIMULATOR || EFI_UNIT_TEST
// Clears the shared txCanBuffer before every test so that frames left over
// from a previous test cannot pollute subsequent tests. See discussion in
// LuaHooks.CanTx* vs CanObd2.handleGetDataRequest_SUPPORTED_PIDS_01_20.
class TxCanBufferCleaner : public ::testing::EmptyTestEventListener {
	void OnTestStart(const ::testing::TestInfo& /*test_info*/) override {
		txCanBuffer.clear();
	}
};

// Tests known to produce log artifacts larger than LOG_FILE_SIZE_LIMIT (16 MB)
// when unit-test logging is enabled. For these we skip log creation so the
// LogsTooLargeException doesn't fail them. Keep in sync with the failure list
// observed under setUnitTestCreateLogs(true).
static const std::unordered_set<std::string>& getLogDisabledTests() {
	static const std::unordered_set<std::string> s = {
		"Toyota3ToothCam.RealEngineRunning",
		"arctic.realStartFromFile",
		"realBQS.realHarleyCranking",
		"realBQS.readAsPrimarySensor",
		"realBQS.readAsCam",
		"realChryslerPhaser.phaser_cam_1",
		"realChryslerPhaser.phaser_cam_2",
		"cranking.realCrankingFromFile",
		"cranking.naCrankFromFile",
		"realCrankingVQ40.normalCrankingSyncCam1",
		"realCrankingVQ40.normalCrankingSyncCam2",
		"realCas24Plus1.spinningOnBench",
		"real4b11.running",
		"real4b11.runningDoubledEdge",
		"real4g93.cranking",
		"real4g93.crankingOn11",
		"real4g93.crankingCamOnly",
		"real6g72.sync_3000gt_cranking_rusefi",
		"real6g72.sync_3000gt_cranking_rusefi_2",
		"real6g72.sync_3000gt_crank_cam_cranking",
		"real6g72.sync_3000gt_crank_cam_cranking_2",
		"real6g72.sync_3000gt_crank_cam_cranking_idle",
		"real6g75.withoutSparkPlugs",
		"real6g75.realWithSparkPlugs",
		"fordCoyote.intakeCam",
		"fordCoyote.exhaustCam",
		"fordCoyote.exhaustCamInverted",
		"crankingVW.vwRealCrankingFromFile",
		"crankingVW.crankingTwiceWithGap",
		"realCrankingNB2.normalCranking",
		"realCrankingNB2.crankingMissingInjector",
		"realNeon.srt4_looks_like_cam",
		"realNeon.srt4_crank",
		"crankingGm24x_5.gmRealCrankingFromFile",
		"nissan.realFromFile",
		"nissan.realNoSparkPlugsFromFile",
		"nissan.realFromFile4seconds",
		"nissan.realFromFileVVTIN",
		"realk24.crankingNoPlugs1",
		"realk24.crankingNoPlugs2",
		"realk20.cranking",
		"realJeepEva.cranking",
		"real.SubaruEj20gcranking_only_cam7",
		"real.SubaruEj20gDefaultCranking",
		"real.SubaruEj20gCrankingWot",
		"real.SubaruEj20gDefaultCranking_only_crank",
		"real.SubaruEj20gDefaultCrankingSeparateTrigger",
		"real.SubaruEj20gDefaultRev",
		"RealNoisyTrigger.AvoidOverdwell1NoInstant",
		"RealNoisyTrigger.AvoidOverdwell1WithInstant",
		"RealNoisyTrigger.AvoidOverdwell2NoInstant",
		"RealNoisyTrigger.AvoidOverdwell2WithInstant",
		"RealNoisyTrigger.AvoidOverdwell3NoInstant",
		"RealNoisyTrigger.AvoidOverdwell3WithInstant",
		"harley.hdCrankingWithCam1",
		"harley.hdCrankingWithCam3",
		"harley.hdCrankingWithCam4",
		"harley.hdCrankingWithCamAnother",
		"harley.hdCrankingWithCamAnother2",
		"harley.hdCrankingWithCamAnother3",
	};
	return s;
}

// Toggles setUnitTestCreateLogs(false) around tests known to overflow the
// 16 MB per-file log cap (LogsTooLargeException), restoring the prior value
// after each such test finishes.
class LogDisablerForHeavyTests : public ::testing::EmptyTestEventListener {
	bool savedLogs = false;
	bool didDisable = false;
	void OnTestStart(const ::testing::TestInfo& test_info) override {
		std::string fullName = std::string(test_info.test_suite_name()) + "." + test_info.name();
		if (getLogDisabledTests().count(fullName) > 0) {
			savedLogs = getUnitTestCreateLogs();
			if (savedLogs) {
				setUnitTestCreateLogs(false);
				didDisable = true;
			}
		}
	}
	void OnTestEnd(const ::testing::TestInfo& /*test_info*/) override {
		if (didDisable) {
			setUnitTestCreateLogs(savedLogs);
			didDisable = false;
		}
	}
};
#endif

GTEST_API_ int main(int argc, char **argv) {
  if (argc == 2 && strcmp(argv[1], "ltft_sandbox") == 0) {
    void runLtftSandbox();
	// feed real log sensor data into our logic
    runLtftSandbox();
    return 0;
  }

  if (argc == 2 && strcmp(argv[1], "replay_log") == 0) {
    void runLogReplay();
	// feed real log sensor data into our logic
    runLogReplay();
    return 0;
  }

  if (argc == 3 && strcmp(argv[1], "csv2logicdata") == 0) {
    void runCsv2LogicData(const char* csvFileName);
    runCsv2LogicData(argv[2]);
    return 0;
  }

  if (argc == 3 && strcmp(argv[1], "logicdata2csv") == 0) {
    void runLogicData2Csv(const char* logicDataFileName);
    runLogicData2Csv(argv[2]);
    return 0;
  }

  if (argc == 3 && strcmp(argv[1], "msl2csv") == 0) {
    void runMsl2Csv(const char* mslFileName);
    runMsl2Csv(argv[2]);
    return 0;
  }

  setUnitTestCreateLogs(true);

	hasInitGtest = true;

//	cleanTestResultsFolder();

	testing::InitGoogleTest(&argc, argv);
#if EFI_SIMULATOR || EFI_UNIT_TEST
	// Auto-clear shared txCanBuffer before every test to prevent inter-test pollution
	::testing::UnitTest::GetInstance()->listeners().Append(new TxCanBufferCleaner());
	::testing::UnitTest::GetInstance()->listeners().Append(new LogDisablerForHeavyTests());
#endif
	// uncomment if you only want to run selected tests
	/**
	 * See TEST_FROM_TRIGGER_ID to limit test just for last trigger
	 */
// todo: can we check argc or argv and setVerboseTrigger(true) dynamically if invoked with AllTriggersFixture?
// command line AllTriggersFixture #7946
//	setVerboseTrigger(true);
// --gtest_filter=*TEST_NAME*
	//::testing::GTEST_FLAG(filter) = "*AllTriggersFixture*";
	auto testsStart = std::chrono::steady_clock::now();
	int result = RUN_ALL_TESTS();
	auto testsEnd = std::chrono::steady_clock::now();
	double totalSeconds = std::chrono::duration<double>(testsEnd - testsStart).count();
	printf("Total tests execution time: %.3f s\n", totalSeconds);
	// windows ERRORLEVEL in Jenkins batch file seems to want negative value to detect failure
	// TODO: Jenkins is long gone! Can we remove this returnCode hack?
	int returnCode = result == 0 ? 0 : -1;
	printf("DONE returning %d\n", returnCode);

  sayByeBye();
	return returnCode;
}
