/**
 * @file    idle_hardware.h
 * @brief   Idle Air Control valve hardware
 *
 * @date November 3, 2020
 * 
 * This is just the hardware interface - deciding where to put the valve happens in idle_thread.cpp
 */

#pragma once

#include <cstdint>

/**
 * #9123 - how long the idle solenoid keeps being driven after the engine stops turning, when
 * keepIdleSolenoidWhenStopped is enabled. Bounded because a solenoid held energized with the
 * engine off drains the battery and can overheat the coil, and there is deliberately no
 * "hold forever" setting. A constant rather than a config field: it protects hardware, it is
 * not a tuning knob, and a field would cost flash on every board including those which will
 * never enable the option.
 */
constexpr uint16_t IDLE_SOLENOID_HOLD_TIMEOUT_SEC = 60;

void initIdleHardware();
bool isIdleHardwareRestartNeeded();
bool isIdleMotorBusy();

#if EFI_UNIT_TEST
float getIdleSolenoidOpenDutyForUnitTest();
float getIdleSolenoidCloseDutyForUnitTest();

class StepperMotorBase;
StepperMotorBase& getIacMotorForUnitTest();
void resetIdleHardwareForUnitTest();
#endif // EFI_UNIT_TEST
