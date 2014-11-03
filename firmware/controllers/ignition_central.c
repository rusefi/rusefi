/**
 * @file	ignition_central.c
 * @brief	TODO
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
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

#include "main.h"
#include "ignition_central.h"
#include "io_pins.h"
#include "signal_executor.h"
#include "main_trigger_callback.h"
#include "engine_configuration.h"

extern engine_configuration_s *engineConfiguration;
extern board_configuration_s *boardConfiguration;

void initIgnitionCentral(void) {

	for (int i = 0; i < engineConfiguration->cylindersCount; i++) {
		io_pin_e pin = (io_pin_e)((int)SPARKOUT_1_OUTPUT + i);
		outputPinRegisterExt2(getPinName(pin), pin, boardConfiguration->ignitionPins[i], &boardConfiguration->ignitionPinMode);
	}
}
