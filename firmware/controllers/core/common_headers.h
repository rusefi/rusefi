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
#include "cli_registry.h"
#endif /* __cplusplus */

/**
 * reference to configuration parameter
 */
#define DISPLAY_CONFIG(x) x
/**
 * The main annotation of live data - reference to dynamic state variable
 * See also 'TS_GET_STRUCT'
 */
#define DISPLAY_FIELD(x) x
/**
 * Advanced tag to access fields within data structures
 * "running.fuel" variable would be referenced as DISPLAY_PREFIX(running).DISPLAY_FIELD(fuel)
 */
#define DISPLAY_PREFIX(x) x

/**
 * we use this 'DISPLAY' macro if value is not used by C++ code in current context and we want to hide it from compiler
 */
#define DISPLAY(x)

#define DISPLAY_TAG(x)
/**
 * Just a human-readable comment. '_' underscore symbols become spaces. EOL is handled as new line.
 */
#define DISPLAY_TEXT(x)
#define DISPLAY_STATE(x)
/**
 * Reference to one of 'output' channels/standard gauges.
 */
#define DISPLAY_SENSOR(x) {}
#define DISPLAY_IF(x) x

#define DECLARE_ENGINE_PTR                              \
	Engine *engine = NULL;                              \
	engine_configuration_s *engineConfiguration = NULL; \
	persistent_config_s *config = NULL;                 \
	board_configuration_s *boardConfiguration = NULL;


#define INJECT_ENGINE_REFERENCE(x)              \
	x.engine = engine;                           \
	x.engineConfiguration = engineConfiguration; \
	x.config = config;                           \
	x.boardConfiguration = boardConfiguration;

#endif /* CONTROLLERS_CORE_COMMON_HEADERS_H_ */
