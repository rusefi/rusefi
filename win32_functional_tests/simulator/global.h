
#include <ch.h>
#include <hal.h>
#include <time.h>
#include <math.h>

#include "rusefi_enums.h"
#include "obd_error_codes.h"
#include "error_handling.h"

#include "boards.h"
#include "eficonsole.h"
#include "efilib.h"
#include "datalogging.h"
#include "efitime.h"

#ifndef GLOBAL_FT_H_
#define GLOBAL_FT_H_

#define EFI_ERROR_CODE 0xffffffff

#define DL_OUTPUT_BUFFER 9000

#define CCM_OPTIONAL

#include "cli_registry.h"
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

//##define TRUE 1
//#define FALSE 0
#endif /* GLOBAL_FT_H_ */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void printToWin32Console(char *p);
int systicks2ms(int systicks);
int getRemainingStack(Thread *otp);

// todo: move somewhere else?
bool_t lockAnyContext(void);
void unlockAnyContext(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */


/**
 * number of SysClock ticks in one ms
 */
#define TICKS_IN_MS  (CH_FREQUENCY / 1000)

#define hal_lld_get_counter_value() 0

// this stuff is about ChibiOS 2.6 > Migration
typedef VirtualTimer virtual_timer_t;
typedef EventListener event_listener_t;
#define THD_WORKING_AREA WORKING_AREA
