/*
 * @file common_headers.h
 *
 * Header file shared between firmware, simulator and unit_tests
 *
 * @date Sep 16, 2018
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#ifndef CONTROLLERS_CORE_COMMON_HEADERS_H_
#define CONTROLLERS_CORE_COMMON_HEADERS_H_

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

#include "efifeatures.h"
#include "rusefi_types.h"

#include <math.h>
#include <stdio.h>

#include "error_handling.h"
#include "auto_generated_enums.h"
#include "efilib.h"
#include "efitime.h"

#ifdef __cplusplus
#include "datalogging.h"
#include "loggingcentral.h"
#endif /* __cplusplus */

// See also 'TS_GET_STRUCT'
#define DISPLAY_CONFIG(x) x
#define DISPLAY_PREFIX(x) x
#define DISPLAY_FIELD(x) x
// we use this 'DISPLAY' macro if value is not used by C++ in current context
#define DISPLAY(x)

#define DISPLAY_TAG(x)
#define DISPLAY_TEXT(x)
#define DISPLAY_STATE(x)
#define DISPLAY_SENSOR(x) {}
#define DISPLAY_IF(x) x

#endif /* CONTROLLERS_CORE_COMMON_HEADERS_H_ */
