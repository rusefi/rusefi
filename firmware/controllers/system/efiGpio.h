/**
 * @file	efiGpio.h
 *
 * @date Sep 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */
#ifndef EFIGPIO_H_
#define EFIGPIO_H_

#include "main.h"

/**
 * @brief   Single output pin reference and state
 */
typedef struct {
#if EFI_PROD_CODE
	GPIO_TypeDef *port;
	int pin;
#endif /* EFI_PROD_CODE */
	/**
	 * we track current pin status so that we do not touch the actual hardware if we want to write new pin bit
	 * which is same as current pin value. This maybe helps in case of status leds, but maybe it's a total over-engineering
	 */
	int currentLogicValue;
} OutputPin;

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

int getLogicPinValue(OutputPin * outputPin);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EFIGPIO_H_ */
