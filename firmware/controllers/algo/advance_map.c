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

#include "advance_map.h"
#include "interpolation.h"
// that's for 'max' function
#include "idle_controller.h"

#include "engine_configuration.h"
#include "engine_math.h"

extern engine_configuration_s *engineConfiguration;
//extern engine_configuration2_s *engineConfiguration2;

static float *timing_ptrs[AD_LOAD_COUNT];
static int initialized = FALSE;

float getBaseAdvance(int rpm, float engineLoad) {
	chDbgAssert(initialized, "fuel map initialized", NULL);
	efiAssert(!cisnan(engineLoad), "invalid el");
	efiAssert(!cisnan(engineLoad), "invalid rpm");
	return interpolate3d(engineLoad, engineConfiguration->ignitionLoadBins, AD_LOAD_COUNT, rpm,
			engineConfiguration->ignitionRpmBins,
			AD_RPM_COUNT, timing_ptrs);
}

float getAdvance(int rpm, float engineLoad) {
	float angle;
	if (isCrankingR(rpm)) {
		angle = engineConfiguration->crankingTimingAngle;
	} else {
		angle = getBaseAdvance(rpm, engineLoad);
	}
	return fixAngle(angle + engineConfiguration->ignitionOffset);
}

void prepareTimingMap(void) {
	for (int k = 0; k < AD_LOAD_COUNT; k++)
		timing_ptrs[k] = engineConfiguration->ignitionTable[k];
	initialized = TRUE;
}
