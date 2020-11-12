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
struct engine_configuration_s;
struct persistent_config_s;

#if EFI_UNIT_TEST

	#define DECLARE_ENGINE_PTR                                 \
		Engine *engine = nullptr;                              \
		engine_configuration_s *engineConfiguration = nullptr; \
		persistent_config_s *config = nullptr;

	#define INJECT_ENGINE_REFERENCE(x)                  \
		(x)->engine = engine;                           \
		(x)->engineConfiguration = engineConfiguration; \
		(x)->config = config;

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

#else // EFI_UNIT_TEST

	// These are the non-unit-test (AKA real firmware) noop versions

	#define DECLARE_ENGINE_PTR

	#define INJECT_ENGINE_REFERENCE(x) {}

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

#endif // EFI_UNIT_TEST

#define EXPAND_Engine \
	    engine_configuration_s *engineConfiguration = engine->engineConfigurationPtr; \
		persistent_config_s *config = engine->config; \
		(void)engineConfiguration; \
		(void)config;

#endif // def __cplusplus
