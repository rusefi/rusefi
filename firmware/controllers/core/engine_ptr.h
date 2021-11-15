/**
 * @file engine_ptr.h
 */
#pragma once

#ifndef EFI_UNIT_TEST
#error EFI_UNIT_TEST must be defined!
#endif

#ifndef EFI_SIMULATOR
#error EFI_SIMULATOR must be defined!
#endif

#ifndef EFI_PROD_CODE
#error EFI_PROD_CODE must be defined!
#endif

#ifdef __cplusplus
class Engine;
#endif // def __cplusplus

struct engine_configuration_s;
struct persistent_config_s;

#if EFI_UNIT_TEST

#ifdef __cplusplus
	#define DECLARE_CONFIG_PARAMETER_SIGNATURE engine_configuration_s *engineConfiguration, persistent_config_s *config
	#define DECLARE_CONFIG_PARAMETER_SUFFIX , DECLARE_CONFIG_PARAMETER_SIGNATURE
	#define PASS_CONFIG_PARAMETER_SIGNATURE engineConfiguration, config
	#define PASS_CONFIG_PARAMETER_SUFFIX , PASS_CONFIG_PARAMETER_SIGNATURE

	/**
	 * @see firmware/global.h for explanation
	 */
	#define DECLARE_ENGINE_PARAMETER_SIGNATURE Engine *engine, DECLARE_CONFIG_PARAMETER_SIGNATURE
	#define DECLARE_ENGINE_PARAMETER_SUFFIX , DECLARE_ENGINE_PARAMETER_SIGNATURE
	#define PASS_ENGINE_PARAMETER_SIGNATURE engine, PASS_CONFIG_PARAMETER_SIGNATURE
	#define PASS_ENGINE_PARAMETER_SUFFIX , PASS_ENGINE_PARAMETER_SIGNATURE

	#define EXTERN_ENGINE extern engine_configuration_s & activeConfiguration

	struct EnginePtr {
		Engine* engine = nullptr;
		engine_configuration_s* engineConfiguration = nullptr;
		persistent_config_s* config = nullptr;

		void inject(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
			this->engine = engine;
			this->engineConfiguration = engineConfiguration;
			this->config = config;
		}
	};
#endif // def __cplusplus

	#define DEFINE_CONFIG_PARAM(x, y) , x y
	#define PASS_CONFIG_PARAM(x) , x

	#define EXPAND_Engine \
		    if (engine == nullptr) { firmwareError(OBD_PCM_Processor_Fault, "EXPAND_Engine engine ptr missing"); } \
			engine_configuration_s *engineConfiguration = engine->engineConfiguration; \
			persistent_config_s *config = engine->config; \
			(void)engineConfiguration; \
			(void)config;

	#define CONFIG(x) engineConfiguration->x
	#define ENGINE(x) engine->x
#else // EFI_UNIT_TEST

	// These are the non-unit-test (AKA real firmware) noop versions

#ifdef __cplusplus
	// these macro are used when we should not have visibility to 'engine'
	#define DECLARE_CONFIG_PARAMETER_SIGNATURE void
	#define DECLARE_CONFIG_PARAMETER_SUFFIX
	#define PASS_CONFIG_PARAMETER_SIGNATURE
	#define PASS_CONFIG_PARAMETER_SUFFIX

	// Use this macro to declare a function which only takes magic references
	#define DECLARE_ENGINE_PARAMETER_SIGNATURE void
	// Use this version of the macro as the suffix if method has other parameters
	#define DECLARE_ENGINE_PARAMETER_SUFFIX
	// Pass this if only magic references are needed
	#define PASS_ENGINE_PARAMETER_SIGNATURE
	// Pass this after some other parameters are passed
	#define PASS_ENGINE_PARAMETER_SUFFIX

	#define ENGINE(x) ___engine.x

	struct EnginePtr {
		void inject(DECLARE_ENGINE_PARAMETER_SIGNATURE) { }
	};
#endif // def __cplusplus

	/**
	 * this macro allows the compiled to figure out the complete static address, that's a performance
	 * optimization which is hopefully useful at least for anything trigger-related
	 *
	 * this is related to the fact that for unit tests we prefer to explicitly pass references in method signature thus code covered by
	 * unit tests would need to use by-reference access. These macro allow us to have faster by-address access in real firmware and by-reference
	 * access in unit tests
	 */
	#define CONFIG(x) persistentState.persistentConfiguration.engineConfiguration.x

	#define DEFINE_CONFIG_PARAM(x, y)
	#define CONFIG_PARAM(x) CONFIG(x)
	#define PASS_CONFIG_PARAM(x)

	#define EXPAND_Engine
#endif // EFI_UNIT_TEST
