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
	#define EXTERN_ENGINE extern engine_configuration_s & activeConfiguration

	struct EnginePtr {
		void inject() {
		}
	};
#endif // def __cplusplus

	#define EXPAND_Engine 

	#define CONFIG(x) engineConfiguration->x
	#define ENGINE(x) engine->x
#else // EFI_UNIT_TEST

	// These are the non-unit-test (AKA real firmware) noop versions

#ifdef __cplusplus
	#define ENGINE(x) ___engine.x

	struct EnginePtr {
		void inject() { }
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

	#define EXPAND_Engine
#endif // EFI_UNIT_TEST
