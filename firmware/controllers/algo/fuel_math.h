/**
 * @file	fuel_math.h
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#ifndef FUEL_MAP_H_
#define FUEL_MAP_H_

void prepareFuelMap(void);

float getBaseFuel(int rpm, float engineLoad);
float getIatCorrection(float iat);
float getInjectorLag(float vBatt);
float getCltCorrection(float clt);
float getRunningFuel(int rpm, float engineLoad);
float getStartingFuel(float coolantTemperature);
float getFuelMs(int rpm);

#endif /* FUEL_MAP_H_ */
