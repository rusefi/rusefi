/*
 * @file gpio_helper.h
 * @brief   General I/O helper
 *
 * @date Aug 25, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef GPIO_HELPER_H_
#define GPIO_HELPER_H_

#include "efiGpio.h"

void initOutputPin(const char *msg, OutputPin *outputPin, ioportid_t port, uint32_t pinNumber);
void initOutputPinExt(const char *msg, OutputPin *outputPin, ioportid_t port, uint32_t pinNumber, iomode_t mode);

#endif /* GPIO_HELPER_H_ */
