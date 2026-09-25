/**
 * @file	board_bootloader.cpp
 * @brief	Minimal board hooks for the OpenBLT bootloader build (m74_9)
 *
 * The full board_configuration.cpp is excluded from the bootloader build:
 * it drags in the whole engine runtime (sensors, trigger decoding, MFS
 * storage) which the bootloader does not need. The bootloader only needs:
 *   - boardInit(), called from halInit() via the ChibiOS board mechanism
 *     (GPIOs/clocks are already set up by board.c __early_init);
 *   - the LED pins used by the OpenBLT LED driver.
 *
 * @date Aug 19, 2026
 */

#include "pch.h"

void boardInit() {
	// Nothing to do: board.c (__early_init) already set up GPIOs and clocks,
	// and the drivers initialize their own pins (see CanInit in
	// bootloader/openblt_chibios/openblt_can.cpp).
}

Gpio getCommsLedPin() {
	return Gpio::Unassigned;
}

Gpio getWarningLedPin() {
	return Gpio::Unassigned;
}

Gpio getRunningLedPin() {
	return Gpio::Unassigned;
}
