/*
 * @file maf2map.h
 *
 * @author Andrey Belomutskiy, (c) 2012-2019
 * @date Jan 20, 2018
 */

#ifndef CONTROLLERS_SENSORS_MAF2MAP_H_
#define CONTROLLERS_SENSORS_MAF2MAP_H_

#include "table_helper.h"

#define ASIZE 16

typedef Map3D<ASIZE, ASIZE, float> maf2map_Map3D_t;

void initMaf2Map();
float estimateMapByRpmAndMaf(int rpm, float maf);

#endif /* CONTROLLERS_SENSORS_MAF2MAP_H_ */
