#pragma once

template <typename T, int TSlots>
struct static_vector {

	void clear() {
		m_size = 0;
	}

	template <typename TSearch>
	T* find(const TSearch& search) const {
		for (size_t i = 0; i < m_size; i++) {
			if (m_storage[i] == search) {
				return const_cast<T*>(&m_storage[i]);
			}
		}

		return nullptr;
	}

	T* add(const T& value) {
		if (m_size >= TSlots) {
			// vector full, discard
			return nullptr;
		}

		auto& location = m_storage[m_size];

		location = value;
		m_size++;

		return &location;
	}

	T& get(size_t i) {
		return m_storage[i];
	}

	size_t getCount() const {
		return m_size;
	}

private:
	size_t m_size = 0;
	T m_storage[TSlots];
};
