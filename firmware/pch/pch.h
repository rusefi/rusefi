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
#include "engine_ptr.h"
#include "global.h"
#include "efi_gpio.h"
#include "engine.h"
#include "engine_configuration.h"
#include "engine_controller.h"
#include "tunerstudio_outputs.h"
#include "engine_math.h"
#include "pwm_generator_logic.h"
#include "allsensors.h"
#include "sensor.h"
#include "error_handling.h"
#include "settings.h"

