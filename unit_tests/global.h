/*
 * @file	global.h
 *
 * Global header file for unit tests
 *
 * @date Nov 28, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#define DEFAULT_ENGINE_TYPE CUSTOM_ENGINE

#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "efitime.h"
#include "auto_generated_enums.h"

#define US_TO_NT_MULTIPLIER 100

#define ALWAYS_INLINE INLINE

#define US2NT(x) (US_TO_NT_MULTIPLIER * (x))

#define NT2US(x) ((x) / US_TO_NT_MULTIPLIER)

#define INLINE inline

typedef int bool_t;

#define EFI_ERROR_CODE 0xffffffff
#define TRUE 1
#define FALSE 0

#define CCM_OPTIONAL

#define EXTERN_ENGINE extern EnginePins enginePins

#ifdef __cplusplus
class Engine;
#endif

/**
 * @see firmware/global.h for explanation
 */
#define DECLARE_ENGINE_PARAMETER_SIGNATURE Engine *engine, engine_configuration_s *engineConfiguration, persistent_config_s *config, board_configuration_s *boardConfiguration
#define DECLARE_ENGINE_PARAMETER_SUFFIX , Engine *engine, engine_configuration_s *engineConfiguration, persistent_config_s *config, board_configuration_s *boardConfiguration
#define PASS_ENGINE_PARAMETER_SIGNATURE engine, engineConfiguration, config, boardConfiguration
#define PASS_ENGINE_PARAMETER_SUFFIX , engine, engineConfiguration, config, boardConfiguration


/**
 * this macro is a work-around for uint tests to get all needed pointers only
 * if engine is in scope
 */
#define EXPAND_Engine \
	    engine_configuration_s *engineConfiguration = engine->engineConfiguration; \
		persistent_config_s *config = engine->config; \
		board_configuration_s *boardConfiguration = &engineConfiguration->bc;


/**
 * this macro provides references to engine from EngineTestHelper
 */
#define EXPAND_EngineTestHelper \
	    Engine *engine = &eth.engine; \
		EXPAND_Engine

#define CONFIG(x) engineConfiguration->x
#define ENGINE(x) engine->x
#define TRIGGER_SHAPE(x) engine->triggerCentral.triggerShape.x

#endif /* GLOBAL_H_ */
