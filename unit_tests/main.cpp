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
	hasInitGtest = true;

	testing::InitGoogleTest(&argc, argv);
	// uncomment if you only want to run selected tests
	/**
	 * See TEST_FROM_TRIGGER_ID to limit test just for last trigger
	 */
//	setVerboseTrigger(true);
// --gtest_filter=*TEST_NAME*
	//::testing::GTEST_FLAG(filter) = "*AllTriggersFixture*";
	int result = RUN_ALL_TESTS();
	// windows ERRORLEVEL in Jenkins batch file seems to want negative value to detect failure
	int returnCode = result == 0 ? 0 : -1;
	printf("DONE returning %d\n", returnCode);
	return returnCode;
}
