/**
 * @file	rusefi_outputs.h
 *
 * rusEfi output channels enum is needed for subscription/un-subscription
 *
 * @date Jan 17, 2015
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#ifndef CONTROLLERS_ALGO_RUSEFI_OUTPUTS_H_
#define CONTROLLERS_ALGO_RUSEFI_OUTPUTS_H_

/**
 * this enum is part of the console protocol API, it should be in sync with com/rusefi/OutputChannel.java
 */

/**
 * For example,
 * subscribe 10
 * unsubscribe 9
 */

typedef enum {
	RO_TPS_ADC_VALUE = 0,
	RO_MAF = 1,
	RO_MAP = 2,
	RO_TRG1_DUTY = 3,
	RO_TRG2_DUTY = 4,
	RO_TRG3_DUTY = 5,
	RO_TRG4_DUTY = 6,
	RO_INJECTOR_LAG = 7,
	RO_VBATT = 8,
	RO_TOTAL_REVOLUTION_COUNTER = 9,
	RO_RUNNING_REVOLUTION_COUNTER = 10,
	RO_WAVE_CHART_CURRENT_SIZE = 11,
	RO_RUNNING_TRIGGER_ERROR = 12,
	RO_RUNNING_ORDERING_TRIGGER_ERROR = 13,

	RO_LAST_ELEMENT = 14
} rusefi_output_e;



#endif /* CONTROLLERS_ALGO_RUSEFI_OUTPUTS_H_ */
