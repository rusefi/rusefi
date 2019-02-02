/*
 * @file config_engine_specs.h
 *
 * @date Jan 28, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#ifndef CONTROLLERS_MATH_CONFIG_ENGINE_SPECS_H_
#define CONTROLLERS_MATH_CONFIG_ENGINE_SPECS_H_

#include "global.h"

/**
 * All these scary macro are about three goals:
 * 1) performance optimization: real ECU hardware firmware should access configuration via direct global variable access without any pointers
 * 2) design enforcement: unit tests should access configuration via pointers, NOT via global variables
 * 3) design enforcement: user code should take special considerations in order to access specific group of configuration and not have access to complete configuration by default
 */

#if EFI_UNIT_TEST
#define DECLARE_GLOBAL_SIGNATURE Engine *engine, engine_configuration_s *___engineConfiguration, persistent_config_s *config, board_configuration_s *boardConfiguration
#define DECLARE_GLOBAL_SUFFIX , DECLARE_GLOBAL_SIGNATURE
#define PASS_GLOBAL_SIGNATURE engine, ___engineConfiguration, config, boardConfiguration
#define PASS_GLOBAL_SUFFIX , PASS_GLOBAL_SIGNATURE
#define CONFIG_ACCESS_FOR_CONFIG_HEADER_ONLY(x) ___engineConfiguration->x
#else /* EFI_UNIT_TEST */
#define DECLARE_GLOBAL_SIGNATURE void
// Use this version of the macro as the suffix if method has other parameters
#define DECLARE_GLOBAL_SUFFIX
// Pass this if only magic references are needed
#define PASS_GLOBAL_SIGNATURE
// Pass this after some other parameters are passed
#define PASS_GLOBAL_SUFFIX
#define CONFIG_ACCESS_FOR_CONFIG_HEADER_ONLY(x) persistentState.persistentConfiguration.engineConfiguration.x
#endif /* EFI_UNIT_TEST */

#define get_operationMode CONFIG_ACCESS_FOR_CONFIG_HEADER_ONLY(operationMode)
#define get_specs_displacement CONFIG_ACCESS_FOR_CONFIG_HEADER_ONLY(specs.displacement)
#define get_specs_cylindersCount CONFIG_ACCESS_FOR_CONFIG_HEADER_ONLY(specs.cylindersCount)
#define get_injector_flow CONFIG_ACCESS_FOR_CONFIG_HEADER_ONLY(injector.flow)

#endif /* CONTROLLERS_MATH_CONFIG_ENGINE_SPECS_H_ */
