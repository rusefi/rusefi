#pragma once

namespace protected_resource_detail {

template <typename Mutex, typename Wait, typename Result>
bool waitForUsers(Mutex& mutex, Wait wait, Result success, Result timeout) {
	const Result waitResult = wait();
	if (waitResult == timeout) {
		// ChibiOS condition waits do not reacquire the mutex on timeout.
		mutex.lock();
	}

	return waitResult == success;
}

} // namespace protected_resource_detail
