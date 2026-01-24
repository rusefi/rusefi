/**
 * @file main.cpp
 * @file Unit tests (and some integration tests to be fair) of rusEFI
 *
 * @author Andrey Belomutskiy, (c) 2012-2021
 */


#include "pch.h"
#include <stdlib.h>

bool hasInitGtest = false;

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

	testing::InitGoogleTest(&argc, argv);
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
	return returnCode;
}
