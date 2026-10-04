#pragma once

#include <cstddef>
#include <cstdint>

// A low-priority linear DMA acquisition sharing a converter with a real-time
// consumer. All access must be serialized by the port's interrupt/system lock.
// DMA writes to a separate buffer: only complete batches enter the cache.
template <size_t Channels, size_t Depth>
class AdcSharedSampler {
public:
	bool tryStart(bool driverReady, int64_t now) {
		if (!driverReady || m_active) {
			return false;
		}
		m_started = now;
		m_active = true;
		return true;
	}

	bool active() const { return m_active; }

	bool timedOut(int64_t now, int64_t timeout) const {
		return m_active && now - m_started >= timeout;
	}

	// Preemption retains the last complete batch only for its original lifetime.
	void cancel() { m_active = false; }

	void fail() {
		m_active = false;
		m_valid = false;
	}

	void complete(const volatile uint16_t* samples, int64_t now) {
		if (!m_active) {
			return;
		}
		for (size_t channel = 0; channel < Channels; channel++) {
			uint32_t sum = 0;
			for (size_t row = 0; row < Depth; row++) {
				sum += samples[row * Channels + channel];
			}
			m_samples[channel] = sum / Depth;
		}
		m_completed = now;
		m_valid = true;
		m_active = false;
	}

	int read(size_t channel, int64_t now, int64_t maxAge) const {
		if (channel >= Channels || !m_valid || now < m_completed || now - m_completed >= maxAge) {
			return -1;
		}
		return m_samples[channel];
	}

private:
	static_assert(Channels > 0 && Depth > 0 && Depth <= 65535);
	uint16_t m_samples[Channels] = {};
	int64_t m_started = 0;
	int64_t m_completed = 0;
	bool m_active = false;
	bool m_valid = false;
};
