#include "pch.h"
//#include "efilib.h"

TEST(util, hash) {
	ASSERT_EQ(djb2lowerCase("Hello1"), 30950378);
	ASSERT_EQ(djb2lowerCase("Hello2"), 30950379);
	ASSERT_EQ(djb2lowerCase("HELLO2"), 30950379);
}
