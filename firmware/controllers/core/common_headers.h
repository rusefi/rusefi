/*
 * @file common_headers.h
 *
 * Header file shared between firmware, simulator and unit_tests
 *
 * @date Sep 16, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#ifndef FALSE
#define FALSE (0)
#endif /* FALSE */

#ifndef TRUE
#define TRUE (!(FALSE))
#endif /* TRUE */

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

#include "efifeatures.h"
#include "rusefi_types.h"

#include <math.h>
#include <stdio.h>

#include "auto_generated_enums.h"
#include "auto_generated_commonenum.h"
#include "auto_generated_enginetypes.h"
#include "efilib.h"
#include "efitime.h"

#ifdef __cplusplus
#include "datalogging.h"
#include "cli_registry.h"
#endif /* __cplusplus */
