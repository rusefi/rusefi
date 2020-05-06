#include "sensor.h"
#include "efilib.h"
#include "loggingcentral.h"

// This struct represents one sensor in the registry.
// It stores whether the sensor should use a mock value,
// the value to use, and if not a pointer to the sensor that
// can provide a real value.
struct SensorRegistryEntry {
	bool useMock;
	float mockValue;
	Sensor *sensor;
};

static SensorRegistryEntry s_sensorRegistry[static_cast<size_t>(SensorType::PlaceholderLast)] = {};

static const char* s_sensorNames[] = {
	"Invalid",
	"CLT",
	"IAT",

	"Oil Pressure",

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
};

static_assert(efi::size(s_sensorNames) == efi::size(s_sensorRegistry));

bool Sensor::Register() {
	// Get a ref to where we should be
	auto &entry = s_sensorRegistry[getIndex()];

	// If there's somebody already here - a consumer tried to double-register a sensor
	if (entry.sensor) {
		// This sensor has already been registered. Don't re-register it.
		return false;
	} else {
		// put ourselves in the registry
		s_sensorRegistry[getIndex()].sensor = this;
		return true;
	}
}

/*static*/ void Sensor::resetRegistry() {
	// Clear all entries
	for (size_t i = 0; i < efi::size(s_sensorRegistry); i++) {
		auto &entry = s_sensorRegistry[i];

		entry.sensor = nullptr;
		entry.useMock = false;
		entry.mockValue = 0.0f;
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
	return entry ? entry->sensor : nullptr;
}

/*static*/ SensorResult Sensor::get(SensorType type) {
	const auto entry = getEntryForType(type);

	// Check if this is a valid sensor entry
	if (!entry) {
		return unexpected;
	}

	// Next check for mock
	if (entry->useMock) {
		return entry->mockValue;
	}

	// Get the sensor out of the entry
	const Sensor *s = entry->sensor;
	if (s) {
		// If we found the sensor, ask it for a result.
		return s->get();
	}

	// We've exhausted all valid ways to return something - sensor not found.
	return unexpected;
}

/*static*/ float Sensor::getRaw(SensorType type) {
	const auto entry = getEntryForType(type);

	// Check if this is a valid sensor entry
	if (!entry) {
		return 0;
	}

	const auto s = entry->sensor;
	if (s) {
		return s->getRaw();
	}

	// We've exhausted all valid ways to return something - sensor not found.
	return 0;
}

/*static*/ bool Sensor::hasSensor(SensorType type) {
	const auto entry = getEntryForType(type);

	if (!entry) {
		return false;
	}

	return entry->useMock || entry->sensor;
}

/*static*/ void Sensor::setMockValue(SensorType type, float value) {
	auto entry = getEntryForType(type);

	if (entry) {
		entry->mockValue = value;
		entry->useMock = true;
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
		entry->useMock = false;
	}
}

/*static*/ void Sensor::resetAllMocks() {
	// Reset all mocks
	for (size_t i = 0; i < efi::size(s_sensorRegistry); i++) {
		auto &entry = s_sensorRegistry[i];

		entry.useMock = false;
	}
}

/*static*/ const char* Sensor::getSensorName(SensorType type) {
	return s_sensorNames[static_cast<size_t>(type)];
}

// Print information about all sensors
/*static*/ void Sensor::showAllSensorInfo(Logging* logger) {
	for (size_t i = 1; i < efi::size(s_sensorRegistry); i++) {
		auto& entry = s_sensorRegistry[i];
		const char* name = s_sensorNames[i];

		if (entry.useMock) {
			scheduleMsg(logger, "Sensor \"%s\" mocked with value %.2f", name, entry.mockValue);
		} else {
			const auto sensor = entry.sensor;

			if (sensor) {
				sensor->showInfo(logger, name);
			} else {
				scheduleMsg(logger, "Sensor \"%s\" is not configured.", name);
			}
		}
	}
}
