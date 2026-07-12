/*
 * @file hardware.h
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "spi.h"

// 'startHardware' is invoked both on boot and configuration change
void startHardware();
void stopHardware();

#if HAL_USE_ADC
// This callback is called by the ADC driver when a new fast ADC sample is ready
void onFastAdcComplete(adcsample_t*);
#endif /* HAL_USE_ADC */

void applyNewHardwareSettings();

// Initialize hardware that doesn't require configuration to be loaded
void initHardwareNoConfig();

// Initialize hardware with configuration loaded
void initHardware();

// todo: can we do simpler here? move conditional compilation into debounce.h?
#if EFI_PROD_CODE
#include "debounce.h"
#else
class ButtonDebounce;
#endif /* EFI_PROD_CODE */

void setBor(int borValue);
