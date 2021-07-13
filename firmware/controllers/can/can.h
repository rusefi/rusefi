/**
 * @file	can.h
 *
 * @date Mar 19, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#pragma once

#include "hal.h"

#include "periodic_thread_controller.h"

#define CAN_PEDAL_TPS_OFFSET 2
#define CAN_SENSOR_1_OFFSET 3

#define CAN_TIMEOUT MS2NT(100)

//can tx periodic task cycle time in frequency, 200hz -> 5ms period
#define CAN_CYCLE_FREQ		(200.0f)
//can tx periodic task cycle time in ms
#define CAN_CYCLE_PERIOD    (CH_CFG_ST_FREQUENCY / CAN_CYCLE_FREQ)

enum class CanInterval : uint16_t {
	None    = 0,
	_5ms    = 1 << 0,
	_10ms   = 1 << 1,
	_20ms   = 1 << 2,
	_50ms   = 1 << 3,
	_100ms  = 1 << 4,
	_200ms  = 1 << 5,
	_250ms  = 1 << 6,
	_500ms  = 1 << 7,
	_1000ms = 1 << 8,
	_MAX_Cycle = _1000ms,
};

class CanListener;
class CanSensorBase;

#if EFI_CAN_SUPPORT
void processCanRxMessage(const CANRxFrame& msg, efitick_t nowNt);
#endif // EFI_CAN_SUPPORT

void registerCanListener(CanListener& listener);
void registerCanSensor(CanSensorBase& sensor);

class CanWrite final : public PeriodicController<512> {
public:
	CanWrite();
	void PeriodicTask(efitime_t nowNt) override;
};

// allow using shorthand CI
using CI = CanInterval;

// logical and/or operators so we can use our enum like an int
constexpr CI operator |(CI lhs, CI rhs) {
    using T = std::underlying_type_t<CI>;
    return static_cast<CI>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

constexpr CI operator &(CI lhs, CI rhs) {
    using T = std::underlying_type_t<CI>;
    return static_cast<CI>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

constexpr CI& operator |=(CI& lhs, CI rhs) {
    lhs = lhs | rhs;
    return lhs;
}

class CanCycle {
public:
	explicit CanCycle(uint32_t cycleCounter200hz)
		: m_cycleFlags(computeFlags(cycleCounter200hz))
	{
	}

	bool isInterval(CanInterval interval) {
		return CanInterval::None != (m_cycleFlags & interval);
	}

private:
	static CanInterval computeFlags(uint32_t cycleCount);

	const CanInterval m_cycleFlags;
};

// We need these helpers because the frame layout is different on STM32H7
#ifdef STM32H7XX
#define CAN_SID(f) ((f).std.SID)
#define CAN_EID(f) ((f).ext.EID)
#else
#define CAN_SID(f) ((f).SID)
#define CAN_EID(f) ((f).EID)
#endif
