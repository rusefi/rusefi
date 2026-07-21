// Host-side stand-in for chibios_rt::Mailbox so firmware code using the
// free/filled buffer-queue pattern (tooth_logger_buffer.cpp) compiles and runs
// in unit tests.
//
// Semantics follow ChibiOS chmboxes.c:
// - postI()/fetchI() return MSG_RESET while the mailbox is in reset state
// - resetI() empties the queue and enters reset state; resumeX() re-arms it
// - non-blocking postI() returns MSG_TIMEOUT when full, fetchI() when empty
// Unit tests are single-threaded, so the blocking fetch() degenerates to the
// non-blocking variant regardless of the requested timeout.

#pragma once

#include <cstdint>
#include <cstddef>

typedef int32_t msg_t;
typedef uint32_t sysinterval_t;

#define MSG_OK      ((msg_t)0)
#define MSG_TIMEOUT ((msg_t)-1)
#define MSG_RESET   ((msg_t)-2)

#define TIME_IMMEDIATE ((sysinterval_t)0)

namespace chibios_rt {

template <typename T, int N>
class Mailbox {
public:
	msg_t postI(T msg) {
		if (m_inReset) {
			return MSG_RESET;
		}

		if (m_count >= N) {
			return MSG_TIMEOUT;
		}

		m_items[(m_read + m_count) % N] = msg;
		m_count++;
		return MSG_OK;
	}

	msg_t fetchI(T* out) {
		if (m_inReset) {
			return MSG_RESET;
		}

		if (m_count == 0) {
			return MSG_TIMEOUT;
		}

		*out = m_items[m_read];
		m_read = (m_read + 1) % N;
		m_count--;
		return MSG_OK;
	}

	msg_t fetch(T* out, sysinterval_t /*timeout*/) {
		return fetchI(out);
	}

	void resetI() {
		m_inReset = true;
		m_read = 0;
		m_count = 0;
	}

	void resumeX() {
		m_inReset = false;
	}

	size_t getUsedCountI() {
		return m_count;
	}

private:
	T m_items[N];
	size_t m_read = 0;
	size_t m_count = 0;
	bool m_inReset = false;
};

} // namespace chibios_rt
