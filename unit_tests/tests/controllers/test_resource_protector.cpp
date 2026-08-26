#include "pch.h"

#include "resource_protector_detail.h"

namespace {

enum class WaitResult {
	Success,
	Timeout,
};

struct TestMutex {
	void lock() {
		wasLocked = true;
	}

	bool wasLocked = false;
};

TEST(ProtectedResource, ReacquiresMutexAfterWaitTimeout) {
	TestMutex mutex;

	const bool signaled = protected_resource_detail::waitForUsers(
		mutex,
		[]() { return WaitResult::Timeout; },
		WaitResult::Success,
		WaitResult::Timeout);

	EXPECT_FALSE(signaled);
	EXPECT_TRUE(mutex.wasLocked);
}

TEST(ProtectedResource, DoesNotRelockAfterSignal) {
	TestMutex mutex;

	const bool signaled = protected_resource_detail::waitForUsers(
		mutex,
		[]() { return WaitResult::Success; },
		WaitResult::Success,
		WaitResult::Timeout);

	EXPECT_TRUE(signaled);
	EXPECT_FALSE(mutex.wasLocked);
}

} // namespace
