/**
 * @file	hip9011.h
 * @brief	HIP9011/TPIC8101 driver
 *
 * @date Nov 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#define HIP_THREAD_PERIOD 100

void initHip9011();
void startHip9011_pins();
void stopHip9011_pins();
#if HAL_USE_ADC
void hipAdcCallback(float volts);
#endif /* HAL_USE_ADC */

void hip9011_onFireEvent(uint8_t cylinderNumber, efitick_t nowNt);
