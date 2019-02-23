/*
 * @file global.h
 *
 * Global header file for unit tests
 *
 * @date Nov 28, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdio.h>
#include <time.h>

#include "common_headers.h"

#include "boards.h"

//#define chThdGetSelfX() 0
//#define getRemainingStack(x) (999999)
#define getCurrentRemainingStack() (999999)

// this is needed by all DECLARE_ENGINE_PARAMETER_* usages
#include "engine_configuration_generated_structures.h"

#ifdef __cplusplus
// this is needed by all DECLARE_ENGINE_PARAMETER_* usages
class Engine;
#endif /* __cplusplus */


#ifdef __cplusplus
// todo: include it right here? #include "unit_test_framework.h"
extern "C"
{
#endif /* __cplusplus */

#define CH_FREQUENCY 1000

typedef int bool_t;
typedef uint32_t systime_t;

void chDbgAssert(int c, char *msg, void *arg);

void print(const char *fmt, ...);

#define TICKS_IN_MS 100

#define chDbgCheck(x, y) chDbgAssert(x, y, NULL)

#ifdef __cplusplus
}
#endif /* __cplusplus */


#define US_TO_NT_MULTIPLIER 100
#define VCS_VERSION "321"
#define RUS_EFI_VERSION_TAG "rusEfiVersion"

#define ALWAYS_INLINE INLINE

#define US2NT(x) (US_TO_NT_MULTIPLIER * (x))

#define NT2US(x) ((x) / US_TO_NT_MULTIPLIER)

#define INLINE inline

#define EFI_ERROR_CODE 0xffffffff

#define CCM_OPTIONAL

#define EXTERN_ENGINE extern EnginePins enginePins
#define EXTERN_CONFIG


#define DEFINE_CONFIG_PARAM(x, y) , x y
#define PASS_CONFIG_PARAM(x) , x

/**
 * this macro is a work-around for uint tests to get all needed pointers only
 * if engine is in scope
 */
#define EXPAND_Engine \
	    engine_configuration_s *engineConfiguration = engine->engineConfigurationPtr; \
		persistent_config_s *config = engine->config; \
		board_configuration_s *boardConfiguration = &engineConfiguration->bc;

/**
 * this macro provides references to engine from EngineTestHelper
 */
#define EXPAND_EngineTestHelper \
	    Engine *engine = &eth.engine; \
		EXPAND_Engine

#define WITH_ENGINE_TEST_HELPER(x) EngineTestHelper eth(x); \
		EXPAND_EngineTestHelper;

#define CONFIG_PARAM(x) (x)

#endif /* GLOBAL_H_ */
