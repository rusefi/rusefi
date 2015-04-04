/**
 * @file    pin_repository.h
 * @brief   I/O pin registry header
 *
 *
 *
 * @date Jan 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef PIN_REPOSITORY_H_
#define PIN_REPOSITORY_H_

#include "ch.h"
#include "hal.h"
#include "io_pins.h"

#ifdef __cplusplus
#include "efiGpio.h"
// does not exactly belong here, but that works better for tests
void outputPinRegister(const char *msg, OutputPin *output, GPIO_TypeDef *port, uint32_t pin);
#endif /* __cplusplus */

#define PORT_SIZE 16

void initPinRepository(void);
brain_pin_e parseBrainPin(const char *str);
void mySetPadMode(const char *msg, ioportid_t port, ioportmask_t pin, iomode_t mode);

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

const char *hwPortname(brain_pin_e brainPin);
const char * getPinFunction(brain_input_pin_e brainPin);
void mySetPadMode2(const char *msg, brain_pin_e pin, iomode_t mode);
const char *portname(GPIO_TypeDef* GPIOx);

#ifdef __cplusplus
}
#endif /* __cplusplus */

iomode_t getInputMode(pin_input_mode_e mode);
void efiIcuStart(ICUDriver *icup, const ICUConfig *config);
ioportmask_t getHwPin(brain_pin_e brainPin);
GPIO_TypeDef * getHwPort(brain_pin_e brainPin);

#endif /* PIN_REPOSITORY_H_ */
