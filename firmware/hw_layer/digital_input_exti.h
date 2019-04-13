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
void enableExti(brain_pin_e pin, uint32_t mode, palcallback_t cb);
#endif /* HAL_USE_PAL */

#endif /* HW_LAYER_DIGITAL_INPUT_EXTI_H_ */
