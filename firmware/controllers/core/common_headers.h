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

#include <cmath>
#include <stdio.h>

#include "auto_generated_enums.h"
#include "auto_generated_commonenum.h"
#include "auto_generated_enginetypes.h"
#include "auto_generated_engine_type_e.h"
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

#if EFI_PROD_CODE && defined(__cplusplus) && defined(RUSEFI_STACK_USAGE)
namespace stack_usage {
template <typename Controller, auto Entry, int Budget = Controller::stackSize>
[[gnu::used, gnu::section(".rusefi_stack_root")]] inline constexpr char controllerRoot = 0;

template <auto Entry, int Budget>
[[gnu::used, gnu::section(".rusefi_stack_root")]] inline constexpr char explicitRoot = 0;

template <int Marker>
[[gnu::used, gnu::section(".rusefi_stack_root")]] inline constexpr char processRoot = 0;
}

#define RUSEFI_STACK_ROOT(controller, entry) \
	template const char stack_usage::controllerRoot<controller, &controller::entry>
#define RUSEFI_STACK_ROOT_EXPLICIT(entry, budget) \
	template const char stack_usage::explicitRoot<&entry, budget>
#define RUSEFI_STACK_PROCESS_ROOT() \
	template const char stack_usage::processRoot<0>
// Static functions in upstream translation units cannot be named by C++ pointer here.
#define RUSEFI_STACK_FOREIGN_ROOT(name, function, budget) \
	__asm__(".pushsection .rusefi_stack_foreign_root." QUOTE(name) "." QUOTE(budget) ",\"R\",%progbits\n" \
		".asciz \"" function "\"\n" \
		".popsection\n")
#else
#define RUSEFI_STACK_ROOT(controller, entry)
#define RUSEFI_STACK_ROOT_EXPLICIT(entry, budget)
#define RUSEFI_STACK_PROCESS_ROOT()
#define RUSEFI_STACK_FOREIGN_ROOT(name, function, budget)
#endif

#define EFI_CONFIGURATION_STORAGE	(EFI_STORAGE_INT_FLASH == TRUE) || (EFI_STORAGE_MFS == TRUE) || (EFI_STORAGE_SD == TRUE)

#define hasLotsOfRemainingStack() (getCurrentRemainingStack() > EXPECTED_REMAINING_STACK)

// this macro helps locate all board Public API methods
// DEPRECATED that's a bad pattern - weak linking is extremely fragile, we risk changing method signature
// and loosing magic
// open question what's best way?
// this approach is deprecated, we shall migrate all usages to proved board_overrides.h pattern
//
#define PUBLIC_API_WEAK __attribute__((weak))
