/**
 * @file	can_sensor.h
 *
 * @date March 31, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#pragma once

#include "stored_value_sensor.h"
#include "scaled_channel.h"
#include "hal.h"
#include "can_msg_tx.h"
#include "obd2.h"

/**
 * Sensor which reads it's value from CAN
 */
class CanSensorBase : public StoredValueSensor {
public:
	CanSensorBase(uint32_t eid, SensorType type, efitick_t timeout)
		: StoredValueSensor(type, timeout)
		, m_eid(eid)
	{
	}

	virtual CanSensorBase* request() {
		return m_next;
	}

	void showInfo(Logging* logger, const char* sensorName) const override;

	CanSensorBase* processFrame(const CANRxFrame& frame, efitick_t nowNt) {
		if (frame.EID == m_eid) {
			decodeFrame(frame, nowNt);
		}

		return m_next;
	}

	uint32_t getEid() {
		return m_eid;
	}

	void setNext(CanSensorBase* next) {
		m_next = next;
	}

protected:
	virtual void decodeFrame(const CANRxFrame& frame, efitick_t nowNt) = 0;
	CanSensorBase* m_next = nullptr;

private:
	const uint32_t m_eid;
};

template <typename TStorage, int TScale>
class CanSensor : public CanSensorBase {
public:
	CanSensor(uint32_t eid, uint8_t offset, SensorType type, efitick_t timeout)
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

template <typename TStorage, int TScale>
class ObdCanSensor: public CanSensorBase {
public:
	ObdCanSensor(uint32_t eid, int PID, SensorType type, efitick_t timeout) :
			CanSensorBase(eid, type, timeout) {
		this->PID = PID;
	}

	void decodeFrame(const CANRxFrame& frame, efitick_t nowNt) override {
		if (frame.data8[2] != PID) {
			return;
		}
		// Compute the location of our data within the frame
		const uint8_t* dataLocation = &frame.data8[0];

		// Reinterpret as a scaled_channel - it already has the logic for decoding a scaled integer to a float
		const auto scaler = reinterpret_cast<const scaled_channel<TStorage, TScale>*>(dataLocation);

		// Actually do the conversion
		float value = *scaler;
		setValidValue(value, nowNt);
	}

	CanSensorBase* request() override {
		{
			CanTxMessage msg(OBD_TEST_REQUEST);
			msg[0] = _OBD_2;
			msg[1] = OBD_CURRENT_DATA;
			msg[2] = getEid();

		}
		return m_next;
	}

	int PID;
};

