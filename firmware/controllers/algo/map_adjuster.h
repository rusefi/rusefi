/**
 * map_adjuster.h
 *
 * Auto-tuning core algorithm
 *
 * @date Jul 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef MAP_ADJUSTER_H_
#define MAP_ADJUSTER_H_

void initMapAdjuster(void);
int runMapAdjustments(void (*callback)(int, float, float));

void addAfr(int rpm, float key, float afr);
float getMultiplier(int rpm, float key);

#endif /* MAP_ADJUSTER_H_ */
