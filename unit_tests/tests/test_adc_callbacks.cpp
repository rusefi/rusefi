#include "pch.h"

#include "adc_callback_utils.h"

TEST(AdcCallbacks, LinearCompletionIsAccepted) {
	bool checked = false;
	EXPECT_TRUE(adcCallbackShouldProcess(false, [&checked] {
		checked = true;
		return false;
	}));
	EXPECT_FALSE(checked);
}

TEST(AdcCallbacks, CircularHalfCompletionIsIgnored) {
	EXPECT_FALSE(adcCallbackShouldProcess(true, [] {
		return false;
	}));
}

TEST(AdcCallbacks, CircularFullCompletionIsAccepted) {
	EXPECT_TRUE(adcCallbackShouldProcess(true, [] {
		return true;
	}));
}
