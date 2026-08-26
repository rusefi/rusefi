#pragma once

namespace protected_resource_detail {

template <typename Mutex, typename Wait, typename Result>
bool waitForUsers(Mutex& mutex, Wait wait, Result success, Result timeout) {
	(void)mutex;
	(void)timeout;
	return wait() == success;
}

} // namespace protected_resource_detail
