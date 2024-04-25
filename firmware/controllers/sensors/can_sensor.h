/**
 * @file	can_sensor.h
 *
 * @date March 31, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#pragma once

#include "stored_value_sensor.h"
#include "efi_scaled_channel.h"
#include "can_msg_tx.h"
#include "obd2.h"
#include "can.h"
#include "can_listener.h"

/**
 * Sensor which reads it's value from CAN
 */
class CanSensorBase : public StoredValueSensor, public CanListener {
public:
	CanSensorBase(uint32_t eid, SensorType type, efidur_t timeout)
		: StoredValueSensor(type, timeout)
		, CanListener(eid)
	{
	}

	void showInfo(const char* sensorName) const override;
};

template <typename TStorage, int TScale>
class CanSensor : public CanSensorBase {
public:
	CanSensor(uint32_t eid, uint8_t offset, SensorType type, efidur_t timeout)
		: CanSensorBase(eid, type, timeout)
		, m_offset(offset)
	{
	}

	void decodeFrame(const CANRxFrame& frame, efitick_t nowNt) override {
		// Compute the location of our data within the frame
		const uint8_t* dataLocation = &frame.data8[m_offset];

		// Reinterpret as a scaled_channel - it already has the logic for decoding a scaled integer to a float
		const auto scaler = reinterpret_cast<const scaled_channel<TStorage, TScale>*>(dataLocation);

		// Actually do the conversion
		float value = *scaler;
		setValidValue(value, nowNt);
	}

private:
	const uint8_t m_offset;
};
