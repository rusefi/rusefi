/**
 * @file	pch.h
 * 
 * This file is rusEFI's precompiled header. Most cpp files should include
 * this file in lieu of any of the files below.
 * 
 * This helps compilation performance by parsing/analyzing these files only
 * once, then re-loading that precompiled data for every file that includes
 * this file.
 */

#include "efifeatures.h"
#include "rusefi_generated.h"
#include "engine_ptr.h"
#include "error_handling.h"
#include "global.h"
#include "efi_gpio.h"
#include "tunerstudio_outputs.h"
#include "engine.h"
#include "engine_configuration.h"
#include "engine_controller.h"
#include "engine_math.h"
#include "pwm_generator_logic.h"
#include "allsensors.h"
#include "sensor.h"
#include "settings.h"
#include "interpolation.h"
#include "perf_trace.h"
#include "hardware.h"

#if EFI_UNIT_TEST
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "mocks.h"
#include "engine_test_helper.h"
#endif
