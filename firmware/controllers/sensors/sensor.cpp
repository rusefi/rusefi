#include "pch.h"

#include "loggingcentral.h"

static const char* const s_sensorNames[] = {
	"Invalid",
	"CLT",
	"IAT",
	"RPM",
	"MAP",
	"MAF",

	"Oil Pressure",

	"Fuel Pressure (LP)",
	"Fuel Pressure (HP)",
	"Fuel Pressure (injector)",

	"TPS 1",
	"TPS 1 Primary",
	"TPS 1 Secondary",

	"TPS 2",
	"TPS 2 Primary",
	"TPS 2 Secondary",

	"Acc Pedal",
	"Acc Pedal Primary",
	"Acc Pedal Secondary",

	"Driver Acc Intent",

	"Aux Temp 1",
	"Aux Temp 2",

	"Lambda 1",
	"Lambda 2",

	"Wastegate Position",
	"Idle Valve Position",

	"Flex Fuel",

	"Battery Voltage",

	"Barometric Pressure",

	"Fuel Level %",

	"Aux 1",
	"Aux 2",
	"Aux 3",
	"Aux 4",
};

// This struct represents one sensor in the registry.
// It stores whether the sensor should use a mock value,
// the value to use, and if not a pointer to the sensor that
// can provide a real value.
class SensorRegistryEntry {
public:
	const Sensor* getSensor() {
		return m_sensor;
	}

	void setMockValue(float value, bool mockRedundant) {
		m_mockValue = value;
		m_useMock = true;
		m_mockRedundant = mockRedundant;
	}

	void resetMock() {
		m_useMock = false;
		m_mockValue = 0.0f;
	}

	void reset() {
		m_sensor = nullptr;
		resetMock();
	}

	bool Register(Sensor* sensor) {
		// If there's somebody already here - a consumer tried to double-register a sensor
		if (m_sensor) {
			// This sensor has already been registered. Don't re-register it.
			firmwareError(CUSTOM_OBD_26, "Duplicate registration for sensor \"%s\"", sensor->getSensorName());
			return false;
		} else {
			// Put the sensor in the registry
			m_sensor = sensor;
			return true;
		}
	}

	SensorResult get() const {
		// Check if mock
		if (m_useMock) {
			return m_mockValue;
		}

		// Get the sensor out of the entry
		const Sensor *s = m_sensor;
		if (s) {
			// If this sensor says it doesn't exist, return unexpected
			if (!s->hasSensor()) {
				return unexpected;
			}

			// If we found the sensor, ask it for a result.
			return s->get();
		}

		// We've exhausted all valid ways to return something - sensor not found.
		return unexpected;
	}

	void showInfo(const char* sensorName) const {
		if (m_useMock) {
			efiPrintf("Sensor \"%s\" mocked with value %.2f", sensorName, m_mockValue);
		} else {
			const auto sensor = m_sensor;

			if (sensor) {
				sensor->showInfo(sensorName);
			} else {
				efiPrintf("Sensor \"%s\" is not configured.", sensorName);
			}
		}
	}

	bool hasSensor() const {
		return m_useMock || (m_sensor && m_sensor->hasSensor());
	}

	float getRaw() const {
		const auto sensor = m_sensor;

		if (sensor) {
			return sensor->getRaw();
		}

		// We've exhausted all valid ways to return something - sensor not found.
		return 0;
	}

	bool isRedundant() const {
		const auto sensor = m_sensor;

		if (sensor) {
			return sensor->isRedundant();
		}

		if (m_useMock) {
			return m_mockRedundant;
		}

		return false;
	}

private:
	bool m_useMock = false;
	bool m_mockRedundant = false;
	float m_mockValue;
	Sensor* m_sensor = nullptr;
};

static SensorRegistryEntry s_sensorRegistry[static_cast<size_t>(SensorType::PlaceholderLast)] = {};

static_assert(efi::size(s_sensorNames) == efi::size(s_sensorRegistry));

bool Sensor::Register() {
	return s_sensorRegistry[getIndex()].Register(this);
}

/*static*/ void Sensor::resetRegistry() {
	// Clear all entries
	for (size_t i = 0; i < efi::size(s_sensorRegistry); i++) {
		auto &entry = s_sensorRegistry[i];

		entry.reset();
	}
}

/*static*/ SensorRegistryEntry *Sensor::getEntryForType(SensorType type) {
	size_t index = getIndex(type);
	// Check that we didn't get garbage
	if (index >= getIndex(SensorType::PlaceholderLast)) {
		return nullptr;
	}

	return &s_sensorRegistry[index];
}

/*static*/ const Sensor *Sensor::getSensorOfType(SensorType type) {
	auto entry = getEntryForType(type);
	return entry ? entry->getSensor() : nullptr;
}

/*static*/ SensorResult Sensor::get(SensorType type) {
	const auto entry = getEntryForType(type);

	// Check if this is a valid sensor entry
	if (!entry) {
		return unexpected;
	}

	return entry->get();
}

/*static*/ float Sensor::getRaw(SensorType type) {
	const auto entry = getEntryForType(type);

	return entry ? entry->getRaw() : 0;
}

/*static*/ bool Sensor::isRedundant(SensorType type) {
	const auto entry = getEntryForType(type);

	return entry ? entry->isRedundant() : false;
}

/*static*/ bool Sensor::hasSensor(SensorType type) {
	const auto entry = getEntryForType(type);

	return entry ? entry->hasSensor() : false;
}

/*static*/ void Sensor::setMockValue(SensorType type, float value, bool mockRedundant) {
	auto entry = getEntryForType(type);

	if (entry) {
		entry->setMockValue(value, mockRedundant);
	}
}

/*static*/ void Sensor::setMockValue(int type, float value) {
	// bounds check
	if (type <= 0 || type >= static_cast<int>(SensorType::PlaceholderLast)) {
		return;
	}

	setMockValue(static_cast<SensorType>(type), value);
}

/*static*/ void Sensor::resetMockValue(SensorType type) {
	auto entry = getEntryForType(type);

	if (entry) {
		entry->resetMock();
	}
}

/*static*/ void Sensor::resetAllMocks() {
	// Reset all mocks
	for (size_t i = 0; i < efi::size(s_sensorRegistry); i++) {
		s_sensorRegistry[i].resetMock();
	}
}

/*static*/ const char* Sensor::getSensorName(SensorType type) {
	return s_sensorNames[static_cast<size_t>(type)];
}

// Print information about all sensors
/*static*/ void Sensor::showAllSensorInfo() {
	for (size_t i = 1; i < efi::size(s_sensorRegistry); i++) {
		auto& entry = s_sensorRegistry[i];
		const char* name = s_sensorNames[i];

		entry.showInfo(name);
	}
}

// Print information about a particular sensor
/*static*/ void Sensor::showInfo(SensorType type) {
	auto entry = getEntryForType(type);

	if (entry) {
		entry->showInfo(getSensorName(type));
	}
}
