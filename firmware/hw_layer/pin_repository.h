/**
 * @file    pin_repository.h
 * @brief   I/O pin registry header
 *
 *
 *
 * @date Jan 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef PIN_REPOSITORY_H_
#define PIN_REPOSITORY_H_

#include "global.h"
#include "io_pins.h"

#ifdef __cplusplus
#include "efi_gpio.h"

class PinRepository {
	public:
	PinRepository();

};

#endif /* __cplusplus */

#define PORT_SIZE 16

void initPinRepository(void);
EXTERNC bool brain_pin_is_onchip(brain_pin_e brainPin);
EXTERNC bool markUsed(ioportid_t port, ioportmask_t pin, const char *msg);
EXTERNC bool brain_pin_markUsed(brain_pin_e brainPin, const char *msg);
EXTERNC void markUnused(ioportid_t port, ioportmask_t pin);
EXTERNC void brain_pin_markUnused(brain_pin_e brainPin);
const char * getPinFunction(brain_input_pin_e brainPin);

#endif /* PIN_REPOSITORY_H_ */
