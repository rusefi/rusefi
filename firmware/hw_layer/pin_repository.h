/**
 * @file    pin_repository.h
 * @brief   I/O pin registry header
 *
 *
 *
 * @date Jan 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef PIN_REPOSITORY_H_
#define PIN_REPOSITORY_H_

#include "ch.h"
#include "hal.h"
#include "io_pins.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void initPinRepository(void);
char *hwPortname(brain_pin_e brainPin);
brain_pin_e parseBrainPin(const char *str);
void mySetPadMode(const char *msg, ioportid_t port, ioportmask_t pin, iomode_t mode);
void mySetPadMode2(const char *msg, brain_pin_e pin, iomode_t mode);
char *portname(GPIO_TypeDef* GPIOx);
// does not exactly belong here, but that works better for tests
void outputPinRegister(const char *msg, io_pin_e ioPin, GPIO_TypeDef *port, uint32_t pin);

ioportmask_t getHwPin(brain_pin_e brainPin);
GPIO_TypeDef * getHwPort(brain_pin_e brainPin);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* PIN_REPOSITORY_H_ */
