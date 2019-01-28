/*
 * global_shared.h
 *
 * part of global.h which is shared between firmware and simulator
 * See also common_headers.h
 *
 * Dec 25, 2018
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#ifndef GLOBAL_SHARED_H_
#define GLOBAL_SHARED_H_

/**
 * The following obscurantism is a hack to reduce stack usage, maybe even a questionable performance
 * optimization.
 *
 * rusEfi main processing happens on IRQ so PORT_INT_REQUIRED_STACK has to be pretty large. Problem
 * is that PORT_INT_REQUIRED_STACK is included within each user thread stack, thus this large stack multiplies
 * and this consumes a lot of valueable RAM. While forcing the compiler to inline helps to some degree,
 * it would be even better not to waste stack on passing the parameter.
 *
 * In the firmware we are using 'extern *Engine' - in the firmware Engine is a signleton
 *
 * On the other hand, in order to have a meaningful unit test we are passing Engine * engine as a parameter
 */

#define EXTERN_CONFIG \
		extern engine_configuration_s *engineConfiguration; \
		extern board_configuration_s *boardConfiguration; \
		extern persistent_config_s *config;

#define EXTERN_ENGINE \
		extern Engine ___engine; \
		extern Engine *engine; \
		extern persistent_config_container_s persistentState; \
		EXTERN_CONFIG \
		extern engine_configuration_s activeConfiguration; \
		extern EnginePins enginePins

// Use this macro to declare a function which only takes magic references
#define DECLARE_ENGINE_PARAMETER_SIGNATURE void
// Use this version of the macro as the suffix if method has other parameters
#define DECLARE_ENGINE_PARAMETER_SUFFIX
// Pass this if only magic references are needed
#define PASS_ENGINE_PARAMETER_SIGNATURE
// Pass this after some other parameters are passed
#define PASS_ENGINE_PARAMETER_SUFFIX

// these macro are used when we should not have visibility to 'engine'
#define DECLARE_CONFIG_PARAMETER_SIGNATURE void
#define DECLARE_CONFIG_PARAMETER_SUFFIX
#define PASS_CONFIG_PARAMETER_SIGNATURE
#define PASS_CONFIG_PARAMETER_SUFFIX

/**
 * this macro allows the compiled to figure out the complete static address, that's a performance
 * optimization which is hopefully useful at least for anything trigger-related
 */
#define CONFIG(x) persistentState.persistentConfiguration.engineConfiguration.x
#define CONFIGB(x) persistentState.persistentConfiguration.engineConfiguration.bc.x
#define ENGINE(x) ___engine.x

#define DEFINE_CONFIG_PARAM(x, y)
#define CONFIG_PARAM(x) CONFIG(x)
#define PASS_CONFIG_PARAM(x)


#endif /* GLOBAL_SHARED_H_ */
