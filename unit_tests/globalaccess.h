/*
 * @file globalaccess.h
 *
 * @date Jan 28, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#ifndef GLOBALACCESS_H_
#define GLOBALACCESS_H_

#define DECLARE_CONFIG_PARAMETER_SIGNATURE engine_configuration_s *engineConfiguration, persistent_config_s *config, board_configuration_s *boardConfiguration
#define DECLARE_CONFIG_PARAMETER_SUFFIX , DECLARE_CONFIG_PARAMETER_SIGNATURE
#define PASS_CONFIG_PARAMETER_SIGNATURE engineConfiguration, config, boardConfiguration
#define PASS_CONFIG_PARAMETER_SUFFIX , PASS_CONFIG_PARAMETER_SIGNATURE

/**
 * @see firmware/global.h for explanation
 */
#define DECLARE_ENGINE_PARAMETER_SIGNATURE Engine *engine, DECLARE_CONFIG_PARAMETER_SIGNATURE
#define DECLARE_ENGINE_PARAMETER_SUFFIX , DECLARE_ENGINE_PARAMETER_SIGNATURE
#define PASS_ENGINE_PARAMETER_SIGNATURE engine, PASS_CONFIG_PARAMETER_SIGNATURE
#define PASS_ENGINE_PARAMETER_SUFFIX , PASS_ENGINE_PARAMETER_SIGNATURE

#define CONFIG(x) engineConfiguration->x
// todo: fix this! this does not work because of 'prepareVoidConfiguration(&activeConfiguration);'
//#define CONFIGB(x) engine->engineConfigurationPtr->bc.x
#define CONFIGB(x) CONFIG(bc.x)
#define ENGINE(x) engine->x

#endif /* GLOBALACCESS_H_ */
