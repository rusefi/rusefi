/*
 * @file global.h
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <ch.h>
#include <hal.h>
    
#include <string.h>

#ifndef DEFAULT_ENGINE_TYPE
#define DEFAULT_ENGINE_TYPE CUSTOM_ENGINE
#endif

// this is about MISRA not liking 'time.h'. todo: figure out something
#if defined __GNUC__
// GCC
#include <sys/types.h>
#define ALWAYS_INLINE __attribute__((always_inline))
#else
// IAR
typedef unsigned int time_t;
// todo: what's the IAR option?
#define ALWAYS_INLINE INLINE
#endif

#include "efifeatures.h"
#include "rusefi_types.h"
#include "rusefi_enums.h"
#if EFI_PROD_CODE
#include "io_pins.h"
#endif
#include "auto_generated_enums.h"
#include "obd_error_codes.h"
#include "error_handling.h"

/* definition to expand macro then apply to pragma message */
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)

// project-wide default thread stack size
// see also PORT_INT_REQUIRED_STACK
#define UTILITY_THREAD_STACK_SIZE 400

#define EFI_ERROR_CODE 0xffffffff

#if EFI_USE_CCM && defined __GNUC__
#define MAIN_RAM __attribute__((section(".ram0")))
#elif defined __GNUC__
#define MAIN_RAM
#else
#define MAIN_RAM @ ".ram0"
#endif


/**
 * rusEfi is placing some of data structures into CCM memory simply
 * in order to use that memory - no magic about which RAM is faster etc.
 *
 * Please note that DMA does not work with CCM memory
 */
#if EFI_USE_CCM && defined __GNUC__
#define CCM_OPTIONAL __attribute__((section(".ram4")))
#elif defined __GNUC__
#define CCM_OPTIONAL
#else
#define CCM_OPTIONAL @ ".ram4"
#endif

#if EFI_PROD_CODE || defined(__DOXYGEN__)

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

#define EXTERN_ENGINE extern Engine *engine; \
		extern engine_configuration_s *engineConfiguration; \
		extern board_configuration_s *boardConfiguration; \
		extern persistent_config_container_s persistentState; \
		extern Engine _engine; \
		extern persistent_config_s *config; \
		extern engine_configuration_s activeConfiguration; \
		extern EnginePins enginePins

// Use this macro to declare a function which only takes magic references
#define DECLARE_ENGINE_PARAMETER_SIGNATURE void
// Use this version of the macro as the suffix if method has other parameters
#define DECLARE_ENGINE_PARAMETER_SUFFIX
// Pass this if only magic reference are needed
#define PASS_ENGINE_PARAMETER_SIGNATURE
// Pass this after some other parameters are passed
#define PASS_ENGINE_PARAMETER_SUFFIX

/**
 * this macro allows the compiled to figure out the complete static address, that's a performance
 * optimization which is hopefully useful at least for anything trigger-related
 */
#define CONFIG(x) persistentState.persistentConfiguration.engineConfiguration.x
#define ENGINE(x) _engine.x
#define TRIGGER_SHAPE(x) _engine.triggerCentral.triggerShape.x

#else
#define EXTERN_ENGINE
#endif


/**
 * low-level function is used here to reduce stack usage
 */
#define ON_FATAL_ERROR() \
		palWritePad(LED_ERROR_PORT, LED_ERROR_PIN, 1); \
		turnAllPinsOff(); \
		enginePins.communicationPin.setValue(1);

/*
 * Stack debugging
 */
int getRemainingStack(thread_t *otp);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GLOBAL_H_ */
