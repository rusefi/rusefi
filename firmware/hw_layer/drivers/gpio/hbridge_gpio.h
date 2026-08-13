/*
 * hbridge_gpio.h
 *
 * ETB-style H-bridge exposed as general-purpose on/off output pins
 * (Gpio::HBRIDGE_1_OUT / Gpio::HBRIDGE_2_OUT), see https://github.com/rusefi/rusefi/issues/9673
 */

#pragma once

#include <cstddef>

// Registers the gpiochip when any etbFunctions[] slot selects DC_Gpio.
// Must be called before gpiochips_init().
void initHbridgeGpio();

// The DC motor hardware starts later than the gpiochip accepts writes (an OutputPin may be
// initialized, written, and its logic value cached before doInitElectronicThrottle() runs).
// Call this right after the DC hardware for the given slot is started+enabled to push the
// last requested pin state into the now-live motor.
void hbridgeGpioReplayPinState(size_t dcIndex);
