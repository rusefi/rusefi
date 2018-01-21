/**
 * @file	audi_aan.cpp
 * @brief	Audo AAN default engine configuration
 *
 * @date Nov 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
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
 *
 */

#include "main.h"

#if EFI_ENGINE_AUDI_AAN || defined(__DOXYGEN__)

#include "engine_controller.h"

static void configureShaftPositionEmulatorShapeWhat(PwmConfig *state) {
	/**
	 * One signal per cam shaft revolution
	 */
	int pinStates0[] = { 1, 0 };
	float switchTimes[] = { 0.8, 1 };

	int *pinStates[2] = { pinStates0 };

	weComplexInit("distributor", state, 0, 2, switchTimes, 1, pinStates);
}

void setDefaultEngineConfiguration(EngineConfiguration *engineConfiguration) {
	engineConfiguration2.shaftPositionEventCount = 2;
}

#endif /* EFI_ENGINE_AUDI_AAN */
