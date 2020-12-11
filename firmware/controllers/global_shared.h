/*
 * @file global_shared.h
 *
 * part of global.h which is shared between firmware and simulator
 * See also common_headers.h
 *
 * Dec 25, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

/**
 * The following obscurantism is a hack to reduce stack usage, maybe even a questionable performance
 * optimization.
 * 
 * Of note is that interrupts are NOT serviced on the stack of the thread that was running when the
 * interrupt occurred. The only thing that happens on that thread's stack is that its registers are
 * pushed (by hardware) when an interrupt occurs, just before swapping the stack pointer out for the
 * main stack (currently 0x400=1024 bytes), where the ISR actually runs.
 * see also __main_stack_size__
 * see also __process_stack_size__
 * 
 * see also http://www.chibios.org/dokuwiki/doku.php?id=chibios:kb:stacks
 *
 * In the firmware we are using 'extern *Engine' - in the firmware Engine is a signleton
 *
 * On the other hand, in order to have a meaningful unit test we are passing Engine * engine as a parameter
 */

#include "global.h"
#include "engine_ptr.h"

#define EXTERN_ENGINE_CONFIGURATION \
		extern engine_configuration_s *engineConfiguration; \
		extern persistent_config_container_s persistentState; \
		extern persistent_config_s *config;

/**
 * this macro allows the compiled to figure out the complete static address, that's a performance
 * optimization which is hopefully useful at least for anything trigger-related
 *
 * this is related to the fact that for unit tests we prefer to explicitly pass references in method signature thus code covered by
 * unit tests would need to use by-reference access. These macro allow us to have faster by-address access in real firmware and by-reference
 * access in unit tests
 */
#define CONFIG(x) persistentState.persistentConfiguration.engineConfiguration.x

#ifdef __cplusplus

/**
 * & is reference in C++ (not C)
 * Ref is a pointer that:
 *   you access with dot instead of arrow
 *   Cannot be null
 * This is about EFI_ACTIVE_CONFIGURATION_IN_FLASH
 */
#define EXTERN_CONFIG \
		EXTERN_ENGINE_CONFIGURATION \
		extern engine_configuration_s & activeConfiguration; \

#define EXTERN_ENGINE \
		extern Engine ___engine; \
		extern Engine *engine; \
		EXTERN_CONFIG \
		extern EnginePins enginePins \

#define ENGINE(x) ___engine.x

#define DEFINE_CONFIG_PARAM(x, y)
#define CONFIG_PARAM(x) CONFIG(x)
#define PASS_CONFIG_PARAM(x)

#define EXPECTED_REMAINING_STACK 128

#endif /* __cplusplus */

/*
 * Stack debugging
 */
EXTERNC int getRemainingStack(thread_t *otp);
int CountFreeStackSpace(const void* wabase);
void validateStack(const char*msg, obd_code_e code, int stackUnusedSize);
