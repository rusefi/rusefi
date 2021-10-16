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
#include "can.h"
#include "can_listener.h"

/**
 * Sensor which reads it's value from CAN
 */
class CanSensorBase : public StoredValueSensor, public CanListener {
public:
	CanSensorBase(uint32_t eid, SensorType type, efitick_t timeout)
		: StoredValueSensor(type, timeout)
		, CanListener(eid)
	{
	}

	void showInfo(const char* sensorName) const override;
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

template <int Size, int Offset>
class ObdCanSensor: public CanSensorBase {
public:
	ObdCanSensor(int PID, float Scale, SensorType type) :
			CanSensorBase(OBD_TEST_RESPONSE, type, /* timeout, never expire */ 0) {
		this->PID = PID;
		this->Scale = Scale;
	}

	void decodeFrame(const CANRxFrame& frame, efitick_t nowNt) override {
		if (frame.data8[2] != PID) {
			return;
		}

		int iValue;
		if (Size == 2) {
			iValue = frame.data8[3] * 256 + frame.data8[4];
		} else {
			iValue = frame.data8[3];
		}

		float fValue = (1.0 * iValue / Scale) - Offset;
		setValidValue(fValue, nowNt);
	}

	CanListener* request() override {
		{
			CanTxMessage msg(OBD_TEST_REQUEST);
			msg[0] = _OBD_2;
			msg[1] = OBD_CURRENT_DATA;
			msg[2] = PID;
		}
		// let's sleep on write update after each OBD request, this would give read thread a chance to read response
		// todo: smarter logic of all this with with semaphore not just sleep
		chThdSleepMilliseconds(300);
		return CanListener::request();
	}

	int PID;
	float Scale;
};
