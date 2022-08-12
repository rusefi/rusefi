/*
 * @file engine_parts.h
 *
 * @date May 27, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "cyclic_buffer.h"
#include "timer.h"

#define MOCK_ADC_SIZE 26

struct Accelerometer {
	float x = 0; // G value
	float y = 0;
	float z = 0;
	float yaw = 0;
	float roll = 0;
};

struct SensorsState {
	Accelerometer accelerometer;
};

class FuelConsumptionState {
public:
	void consumeFuel(float grams, efitick_t nowNt);

	float getConsumedGrams() const;
	float getConsumptionGramPerSecond() const;

private:
	float m_consumedGrams = 0;
	float m_rate = 0;

	Timer m_timer;
};

class TransmissionState {
public:
	gear_e gearSelectorPosition;
};

struct warning_t {
	Timer LastTriggered;
	obd_code_e Code = OBD_None;

	warning_t() { }

	explicit warning_t(obd_code_e code)
		: Code(code)
	{
	}

	// Equality just checks the code, timer doesn't matter
	bool operator ==(const warning_t& other) const {
		return other.Code == Code;
	}

	// Compare against a plain OBD code
	bool operator ==(const obd_code_e other) const {
		return other == Code;
	}
};

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

typedef static_vector<warning_t, 8> warningBuffer_t;

class WarningCodeState {
public:
	WarningCodeState();
	void addWarningCode(obd_code_e code);
	bool isWarningNow() const;
	bool isWarningNow(obd_code_e code) const;
	void clear();
	int warningCounter;
	int lastErrorCode;

	Timer timeSinceLastWarning;

	// todo: we need a way to post multiple recent warnings into TS
	warningBuffer_t recentWarnings;
};

struct multispark_state
{
	efitick_t delay = 0;
	efitick_t dwell = 0;
	uint8_t count = 0;
};
