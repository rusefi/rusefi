/**
 * @file	electronic_throttle.h
 *
 * @date Dec 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef ELECTRONIC_THROTTLE_H_
#define ELECTRONIC_THROTTLE_H_

void initElectronicThrottle(void);
void setDefaultEtbParameters(void);
void setEtbPFactor(float value);
void setEtbIFactor(float value);
void stopETBPins(void);
void startETBPins(void);

#endif /* ELECTRONIC_THROTTLE_H_ */
