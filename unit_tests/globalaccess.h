/*
 * @file globalaccess.h
 *
 * @date Jan 28, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"

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

#define CONFIG(x) engineConfiguration->x
#define ENGINE(x) engine->x
