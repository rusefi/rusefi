/*
 * @file global.h
 *
 * Global header file for win32 or posix simulator
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#define DEFAULT_ENGINE_TYPE FORD_ESCORT_GT

#include <ch.h>
#include <hal.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>

#include "efifeatures.h"

#include "auto_generated_enums.h"
#include "boards.h"
#include "efilib.h"
#include "efitime.h"
#include "error_handling.h"
#include "rusefi_enums.h"
#include "obd_error_codes.h"

#define EFI_UNIT_TEST FALSE

#define hasFatalError() (FALSE)

#define US_TO_NT_MULTIPLIER 100

#define ALWAYS_INLINE

#define US2NT(x) (US_TO_NT_MULTIPLIER * (x))

#define NT2US(x) ((x) / US_TO_NT_MULTIPLIER)

// need to fight 32bit int overflow

#define MY_US2ST(x) ((x) / 10)

#define EFI_ERROR_CODE 0xffffffff

#define DL_OUTPUT_BUFFER 9000

#define CCM_OPTIONAL

#include "rusefi_enums.h"

#define EFI_CUSTOM_PANIC_METHOD 1

// project-wide default thread stack size
#define UTILITY_THREAD_STACK_SIZE 1384

/**
 * @brief   @p Win32TestStream virtual methods table.
 */
struct Win32TestStreamVMT {
	_base_channel_methods
};

typedef struct {
  const struct Win32TestStreamVMT *vmt;
} TestStream;

extern TestStream testStream;

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void printToWin32Console(char *p);
int systicks2ms(int systicks);
int getRemainingStack(thread_t *otp);

// todo: move somewhere else?
bool lockAnyContext(void);
void unlockAnyContext(void);
void applyNewConfiguration(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */


/**
 * number of SysClock ticks in one ms
 */
#define TICKS_IN_MS  (CH_FREQUENCY / 1000)

#define hal_lld_get_counter_value() 0

#define EXTERN_ENGINE extern Engine *engine; \
		extern engine_configuration_s *engineConfiguration; \
		extern board_configuration_s *boardConfiguration; \
		extern persistent_config_s *config; \
		extern persistent_config_container_s persistentState; \
		extern EnginePins enginePins

#define DECLARE_ENGINE_PARAMETER_SIGNATURE void
#define DECLARE_ENGINE_PARAMETER_SUFFIX
#define PASS_ENGINE_PARAMETER_SIGNATURE
#define PASS_ENGINE_PARAMETER_SUFFIX

#define CONFIG(x) persistentState.persistentConfiguration.engineConfiguration.x
#define ENGINE(x) engine->x
#define TRIGGER_SHAPE(x) engine->triggerCentral.triggerShape.x

#endif /* GLOBAL_H_ */
