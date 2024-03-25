/*
 * @file common_headers.h
 *
 * Header file shared between firmware, simulator and unit_tests
 *
 * @date Sep 16, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <rusefi/true_false.h>

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

#include "efifeatures.h"
#include "rusefi_types.h"
#include "efi_quote.h"

#include <math.h>
#include <stdio.h>

#include "auto_generated_enums.h"
#include "auto_generated_commonenum.h"
#include "auto_generated_enginetypes.h"
#include "efilib.h"
#include "efitime.h"

// FIRMWARE_ID is only used to display custom nicer board names to the user in console window and error message
#ifndef FIRMWARE_ID
#define FIRMWARE_ID QUOTE(SHORT_BOARD_NAME)
#endif

#ifdef __cplusplus
#include "datalogging.h"
#include "cli_registry.h"
#endif /* __cplusplus */

#define EXPECTED_REMAINING_STACK 128

// see also validateStack
#define hasLotsOfRemainingStack() (getCurrentRemainingStack() > EXPECTED_REMAINING_STACK)

// this macro helps locate all board API methods
#define PUBLIC_API_WEAK __attribute__((weak))
