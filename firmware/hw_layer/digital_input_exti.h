/*
 * digital_input_exti.h
 *
 *  Created on: Dec 18, 2018
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#ifndef HW_LAYER_DIGITAL_INPUT_EXTI_H_
#define HW_LAYER_DIGITAL_INPUT_EXTI_H_

#include "global.h"

#if HAL_USE_PAL
int efiExtiEnablePin(const char *msg, brain_pin_e pin, uint32_t mode, palcallback_t cb, void *cb_data);
void efiExtiDisablePin(brain_pin_e brainPin);
#endif /* HAL_USE_PAL */

#endif /* HW_LAYER_DIGITAL_INPUT_EXTI_H_ */
