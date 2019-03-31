/*
 * avg_values.c
 *
 * @date Jul 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "avg_values.h"
#include <math.h>

void avgFill(AvgTable *table, int count, float value) {
	for (int i = 0; i < AVG_TAB_SIZE; i++) {
		for (int j = 0; j < AVG_TAB_SIZE; j++) {
			table->counts[i][j] = count;
			table->values[i][j] = value;
		}
	}
}

void avgReset(AvgTable *table) {
	avgFill(table, 0, 0);
}

void avgAddValue(AvgTable *table, int rpm, float key, float value) {
  if (rpm >= MAX_RPM || key >= MAX_KEY) {
		return;
        }
	int i = (int)(AVG_TAB_SIZE * rpm / MAX_RPM);
	int j = (int)(AVG_TAB_SIZE * key / MAX_KEY);

	table->values[i][j] += value;
	table->counts[i][j]++;
}

float avgGetValueByIndexes(AvgTable *table, int i, int j) {
	int count = table->counts[i][j];
	if (count == 0) {
		return NAN;
        }
	return table->values[i][j] / count;
}

float avgGetValue(AvgTable *table, int rpm, float key) {
  if (rpm >= MAX_RPM || key >= MAX_KEY) {
		return NAN;
  }
	int i = (int)(AVG_TAB_SIZE * rpm / MAX_RPM);
	int j = (int)(AVG_TAB_SIZE * key / MAX_KEY);
	return avgGetValueByIndexes(table, i, j);
}

int avgGetValuesCount(AvgTable *table, int rpm, float key) {
  if (rpm >= MAX_RPM || key >= MAX_KEY) {
		return 0;
  }
	int i = (int)(AVG_TAB_SIZE * rpm / MAX_RPM);
	int j = (int)(AVG_TAB_SIZE * key / MAX_KEY);

	return table->counts[i][j];
}
