/**
 * @file main.cpp
 * @file Unit tests (and some integration tests to be fair) of rusEFI
 *
 * @author Andrey Belomutskiy, (c) 2012-2021
 */


#include "pch.h"
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#include "can_msg_tx.h"
#include "unit_test_logger.h"

static void cleanTestResultsDir() {
	DIR* dir = opendir(TEST_RESULTS_DIR);
	if (dir == nullptr) {
		return;
	}
	struct dirent* entry;
	while ((entry = readdir(dir)) != nullptr) {
		const char* name = entry->d_name;
		size_t len = strlen(name);
		bool isTxt = len >= 4 && strcmp(name + len - 4, ".txt") == 0;
		bool isJson = len >= 5 && strcmp(name + len - 5, ".json") == 0;
		bool isMsl = len >= 4 && strcmp(name + len - 4, ".msl") == 0;
		bool isLogicdata = len >= 10 && strcmp(name + len - 10, ".logicdata") == 0;
		if (isTxt || isJson || isMsl || isLogicdata) {
			char path[512];
			snprintf(path, sizeof(path), "%s/%s", TEST_RESULTS_DIR, name);
			remove(path);
		}
	}
	closedir(dir);
}

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

	hasInitGtest = true;

	cleanTestResultsDir();

	testing::InitGoogleTest(&argc, argv);
#if EFI_SIMULATOR || EFI_UNIT_TEST
	// Auto-clear shared txCanBuffer before every test to prevent inter-test pollution
	::testing::UnitTest::GetInstance()->listeners().Append(new TxCanBufferCleaner());
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
	int result = RUN_ALL_TESTS();
	// windows ERRORLEVEL in Jenkins batch file seems to want negative value to detect failure
	// TODO: Jenkins is long gone! Can we remove this returnCode hack?
	int returnCode = result == 0 ? 0 : -1;
	printf("DONE returning %d\n", returnCode);

  sayByeBye();
	return returnCode;
}
