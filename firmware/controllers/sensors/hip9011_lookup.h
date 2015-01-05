/**
 * @file	hip9011_lookup.h
 *
 * @date Jan 4, 2015
 * @author Andrey Belomutskiy, (c) 2012-2014
 */
#ifndef CONTROLLERS_SENSORS_HIP9011_LOOKUP_H_
#define CONTROLLERS_SENSORS_HIP9011_LOOKUP_H_

#define INT_LOOKUP_SIZE 32
#define GAIN_LOOKUP_SIZE 64
#define BAND_LOOKUP_SIZE 64

#define PIF 3.14159f
#define DESIRED_OUTPUT_VALUE 5.0f

extern const int integratorValues[INT_LOOKUP_SIZE];

float getRpmByAngleWindowAndTimeUs(int timeUs, float angleWindowWidth);

#endif /* CONTROLLERS_SENSORS_HIP9011_LOOKUP_H_ */
