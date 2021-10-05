/**
 * @file    sensor.h
 * @brief Base class for sensors.  Inherit this class to implement a new type of sensor.
 *
 * This file defines the basis for all sensor inputs to the ECU, and provides a registry
 * so that consumers may be agnostic to how each sensor may work.
 *
 * HOW TO ADD A NEW SENSOR TYPE:
 *
 * 1. Add an entry to the enum in sensor_type.h.  Be sure to add it ABOVE the placeholder
 *    at the end of the list.
 *
 * 2. In the init/sensor folder, create/modify logic to create an instance of the new sensor,
 *    configure it if necessary, and call its Register() function if it should be enabled.
 *    See init_oil_pressure.cpp for a minimal example.
 *
 * 3. Consume the new sensor with instance(s) of SensorConsumer<SensorType::MyNewSensor>
 *
 * Consumers:
 *
 *   tl;dr: Use a SensorConsumer.  See sensor_consumer.h
 *
 *   All a consumer does is look up whether a particular sensor is present in the table,
 *   and if so, asks it for the current reading.  This could synchronously perform sensor
 *   acquisition and conversion (not recommended), or use a previously stored value (recommended!).
 *   This functionality is implemented in sensor_consumer.h, and sensor.cpp.
 *
 * Providers:
 *   Instantiate a subclass of Sensor, and implement the Get() function.
 *   Call Register() to install the new sensor in the registry, preparing it for use.
 *
 * Mocking:
 *   The sensor table supports mocking each sensors value.  Call Sensor::SetMockValue to
 *   set a mock value for a particular sensor, and Sensor::ResetMockValue or
 *   Sensor::ResetAllMocks to reset one or all stored mock values.
 *
 * Composite Sensors:
 *   Some sensors may be implemented as composite sensors, ie sensors that depend on other
 *   sensors to determine their reading.  For example, the throttle pedal may have a pair of
 *   potentiometers that provide redundancy for the pedal's position.  Each one may be its
 *   own sensor, then with one "master" sensors that combines the results of the others, and
 *   provides validation of whether the readings agree.
 *
 * @date September 12, 2019
 * @author Matthew Kennedy, (c) 2019
 */

#pragma once

#include "sensor_type.h"
#include "expected.h"

#include <cstddef>

using SensorResult = expected<float>;

// Fwd declare - nobody outside of Sensor.cpp needs to see inside this type
class SensorRegistryEntry;

class Sensor {
public:
	// Register this sensor in the sensor registry.
	// Returns true if registration succeeded, or false if
	// another sensor of the same type is already registered.
	bool Register();

	// Print information about this sensor
	virtual void showInfo(const char* sensorName) const = 0;

	// Print information about all sensors
	static void showAllSensorInfo();

	// Print information about a particular sensor
	static void showInfo(SensorType type);

	// Remove all sensors from the sensor registry - tread carefully if you use this outside of a unit test
	static void resetRegistry();

	/*
	 * Static helper for sensor lookup
	 */
	static const Sensor *getSensorOfType(SensorType type);

	/*
	 * Get a reading from the specified sensor.
	 */
	static SensorResult get(SensorType type);

	/*
	 * Get a reading from the specified sensor, or zero if unavailable.
	 */
	static float getOrZero(SensorType type) {
		return Sensor::get(type).value_or(0);
	}

	/*
	 * Get a raw (unconverted) value from the sensor, if available.
	 */
	static float getRaw(SensorType type);

	/*
	 * Get whether a sensor is redundant (a composite of multiple other sensors that can check consistency between them)
	 */
	static bool isRedundant(SensorType type);

	/*
	 * Query whether there is a sensor of a particular type currently registered.
	 */
	static bool hasSensor(SensorType type);

	/*
	 * Mock a value for a particular sensor.
	 */
	static void setMockValue(SensorType type, float value, bool mockRedundant = false);

	/*
	 * Mock a value for a particular sensor.
	 */
	static void setMockValue(int type, float value);

	/*
	 * Reset mock for a particular sensor.
	 */
	static void resetMockValue(SensorType type);

	/*
	 * Reset mocking for all sensors.
	 */
	static void resetAllMocks();

	/*
	 * Inhibit sensor timeouts. Used if you're doing something that will block sensor updates, such as 
	 * erasing flash memory (which stalls the CPU on some MCUs)
	 */
	static void inhibitTimeouts(bool inhibit);

	/*
	 * Get a friendly name for the sensor.
	 * For example, CLT, IAT, Throttle Position 2, etc.
	 */
	const char* getSensorName() { return getSensorName(m_type); }
	static const char* getSensorName(SensorType type);

	// Retrieve the current reading from the sensor.
	//
	// Override this in a particular sensor's implementation.  As reading sensors is in many hot paths,
	// it is unwise to synchronously read the sensor or do anything otherwise costly here.  At the most,
	// this should be field lookup and simple math.
	virtual SensorResult get() const = 0;

	// Retrieve whether the sensor is present.  Some sensors may be registered but not present, i.e. if initialization failed.
	virtual bool hasSensor() const {
		return true;
	}

	/*
	 * Get an unconverted value from the sensor, if available.
	 */
	virtual float getRaw() const {
		return 0;
	}

	/*
	 * Get whether this sensor is redundant (backed by multiple other sensors)
	 */
	virtual bool isRedundant() const {
		// By default sensors are not redundant
		return false;
	}

protected:
	// Protected constructor - only subclasses call this
	explicit Sensor(SensorType type)
		: m_type(type) {}

	static bool s_inhibitSensorTimeouts;

private:
	const SensorType m_type;

	// Get this sensor's index in the list
	constexpr size_t getIndex() {
		return getIndex(m_type);
	}

	// Get the index in the list for a sensor of particular type
	static constexpr size_t getIndex(SensorType type) {
		return static_cast<size_t>(type);
	}

	/*
	 * Static helper for sensor lookup
	 */
	static SensorRegistryEntry *getEntryForType(SensorType type);
};
