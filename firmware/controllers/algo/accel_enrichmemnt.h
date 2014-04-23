/*
 * @file    accel_enrichmemnt.h
 * @brief   accel_enrichmemnt calculator
 *
 * @date Apr 21, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy (c) 2012-2014
 */

#ifndef EVENTQUEUE_H_
#define EVENTQUEUE_H_

class AccelEnrichmemnt {
public:
	float getEnrichment(float engineLoad);
private:
	float engineLoadD[5];
};

#endif /* EVENTQUEUE_H_ */
