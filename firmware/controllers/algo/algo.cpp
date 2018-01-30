/*
 * @file	algo.cpp
 *
 * @date Mar 2, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 *
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

#include "global.h"
#include "algo.h"
#include "advance_map.h"
#include "fuel_math.h"
#include "settings.h"
#include "signal_executor.h"
#include "speed_density.h"
#include "fsio_impl.h"

EXTERN_ENGINE;

void initDataStructures(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	prepareFuelMap(PASS_ENGINE_PARAMETER_SIGNATURE);
	prepareTimingMap(PASS_ENGINE_PARAMETER_SIGNATURE);
	initSpeedDensity(PASS_ENGINE_PARAMETER_SIGNATURE);
	prepareFsio();
}

void initAlgo(Logging *sharedLogger) {
	initInterpolation(sharedLogger);
}
