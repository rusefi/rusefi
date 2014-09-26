/*
 * @file gpio_helper.h
 * @brief   General I/O helper
 *
 * @date Aug 25, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef GPIO_HELPER_H_
#define GPIO_HELPER_H_

#include "efiGpio.h"

#define INITIAL_PIN_STATE -1

void initOutputPin(const char *msg, OutputPin *outputPin, GPIO_TypeDef *port, uint32_t pinNumber);
void initOutputPinExt(const char *msg, OutputPin *outputPin, GPIO_TypeDef *port, uint32_t pinNumber, iomode_t mode);
void setPinValue(OutputPin * outputPin, int electricalValue, int logicValue);

#endif /* GPIO_HELPER_H_ */
