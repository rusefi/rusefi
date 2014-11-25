/**
 * @file	advance_map.c
 *
 * @date Mar 27, 2013
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
#include "advance_map.h"
#include "interpolation.h"
// that's for 'max' function
#include "idle_controller.h"

#include "engine_configuration.h"
#include "engine_math.h"

extern engine_configuration_s *engineConfiguration;
//extern engine_configuration2_s *engineConfiguration2;

static Map3D1616 advanceMap;

float getBaseAdvance(int rpm, float engineLoad) {
	if (cisnan(engineLoad)) {
		warning(OBD_PCM_Processor_Fault, "NaN engine load");
		return NAN;
	}
	efiAssert(!cisnan(engineLoad), "invalid el", NAN);
	efiAssert(!cisnan(engineLoad), "invalid rpm", NAN);
	return advanceMap.getValue(engineLoad, engineConfiguration->ignitionLoadBins, (float) rpm,
			engineConfiguration->ignitionRpmBins);
}

float getAdvance(int rpm, float engineLoad DECLARE_ENGINE_PARAMETER_S) {
	float angle;
	if (isCrankingR(rpm)) {
		angle = -engineConfiguration->crankingTimingAngle;
	} else {
		angle = getBaseAdvance(rpm, engineLoad);
	}
	angle -= engineConfiguration->ignitionOffset;
	angle = fixAngle(angle PASS_ENGINE_PARAMETER);
	return angle;
}

void prepareTimingMap(void) {
	advanceMap.init(engineConfiguration->ignitionTable);
}
