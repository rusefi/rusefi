/*
 * accelerometer.cpp
 *
 * @date May 19, 2016
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "accelerometer.h"
#include "lis302dl.h"
#include "hardware.h"

EXTERN_ENGINE;

void configureAccelerometerPins(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engineConfiguration->LIS302DLCsPin = GPIOE_3;
// 	boardConfiguration->is_enabled_spi_1 = true; // we have a conflict with PA5 input pin

	// stm32f4discovery defaults
	boardConfiguration->spi1mosiPin = GPIOA_7;
	boardConfiguration->spi1misoPin = GPIOA_6;
	boardConfiguration->spi1sckPin = GPIOA_5;
}


void initAccelerometer(DECLARE_ENGINE_PARAMETER_SIGNATURE) {

}

float getLongitudinalAcceleration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return 0;
}

float getTransverseAcceleration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return 0;
}
