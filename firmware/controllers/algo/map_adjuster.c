/*
 * map_adjuster.c
 *
 * @date Jul 23, 2013
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

#include <stddef.h>
#include "map_adjuster.h"
#include "avg_values.h"

#define TARGET_MIN_AFR 11
#define TARGET_MAX_AFR 16

#define HOW_MANY_MEASURMENTS_ARE_NEEDED 5
#define MULT_STEP_DOWN 0.92

#if 0

static AvgTable adjustments;

static AvgTable afrs;

void initMapAdjuster(void) {
	avgReset(&afrs);

	avgFill(&adjustments, 1, 1);
}

static int adjustCell(int i, int j, void (*callback)(int, float, float)) {
	int count = afrs.counts[i][j];
	if (count < HOW_MANY_MEASURMENTS_ARE_NEEDED)
		return 0;

	float value = avgGetValueByIndexes(&afrs, i, j);
	afrs.counts[i][j] = 0;
	afrs.values[i][j] = 0;

	if (value < TARGET_MIN_AFR) {
		float currentMult = adjustments.values[i][j];
//		printf("adj %d %d. cur=%.2f\r\n", i, j, currentMult);
		float newValue = maxF(0.1, MULT_STEP_DOWN * currentMult);
		adjustments.values[i][j] = newValue;
//		printf("adj %d %d. new=%.2f\r\n", i, j, adjustments.values[i][j]);
		if (callback != NULL)
			callback(MAX_RPM * i / AVG_TAB_SIZE, 1.0 * MAX_KEY * j / AVG_TAB_SIZE, newValue);
		return 1;
	}
	return 0;
}

int runMapAdjustments(void (*callback)(int, float, float)) {
	int total = 0;
	for (int i = 0; i < AVG_TAB_SIZE; i++) {
		for (int j = 0; j < AVG_TAB_SIZE; j++) {
			total += adjustCell(i, j, callback);
		}
	}
	return total;
}

void addAfr(int rpm, float key, float afr) {
	avgAddValue(&afrs, rpm, key, afr);
}

float getMultiplier(int rpm, float key) {
	return avgGetValue(&adjustments, rpm, key);
}

#endif /* 0 */
