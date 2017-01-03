/**
 * @file    gpio_helper.c
 * @brief   General I/O helper
 *
 * @date Aug 25, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GPIO_HELPER_C_
#define GPIO_HELPER_C_

#include <hal.h>

#include "pin_repository.h"
#include "gpio_helper.h"

/**
 * @brief Initialize the hardware output pin while also assigning it a logical name
 */
void initOutputPinExt(const char *msg, OutputPin *outputPin, ioportid_t port, uint32_t pinNumber, iomode_t mode) {
	if (outputPin->port != NULL && (outputPin->port != port || outputPin->pin != pinNumber)) {
		/**
		 * here we check if another physical pin is already assigned to this logical output
		 */
// todo: need to clear '&outputs' in io_pins.c
		firmwareError(OBD_PCM_Processor_Fault, "outputPin [%s] already assigned to %x%d", msg, outputPin->port, outputPin->pin);
		return;
	}
	outputPin->currentLogicValue = INITIAL_PIN_STATE;
	outputPin->port = port;
	outputPin->pin = pinNumber;

	mySetPadMode(msg, port, pinNumber, mode);
}

void initOutputPin(const char *msg, OutputPin *outputPin, ioportid_t port, uint32_t pinNumber) {
	initOutputPinExt(msg, outputPin, port, pinNumber, PAL_MODE_OUTPUT_PUSHPULL);
}

#endif /* GPIO_HELPER_C_ */
