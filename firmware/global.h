/*
 * @file global.h
 *
 * Global utility header file for firmware
 *
 * Simulator and unit tests have their own version of this header
 *
 * While this header contains 'EXTERN_ENGINE' and 'DECLARE_ENGINE_PARAMETER_SIGNATURE' magic,
 * this header is not allowed to actually include higher-level engine related headers
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

#include "common_headers.h"
#include "controllers/global_shared.h"

#include "io_pins.h"

#ifdef __cplusplus
#include "cli_registry.h"

#include "eficonsole.h"
#endif /* __cplusplus */

#include "chprintf.h"

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
#if defined(STM32F7XX)
#undef EFI_USE_CCM
// todo: DTCM == CCM on STM32F7?
//#define CCM_RAM ".ram3"
#else /* defined(STM32F4XX) */
#define CCM_RAM ".ram4"
#endif /* defined(STM32F4XX) */

#if EFI_USE_CCM
#if defined __GNUC__
#define CCM_OPTIONAL __attribute__((section(CCM_RAM)))
#else // non-gcc
#define CCM_OPTIONAL @ CCM_RAM
#endif
#else /* !EFI_USE_CCM */
#define CCM_OPTIONAL
#endif /* EFI_USE_CCM */

#define EXTERN_ENGINE \
		extern Engine _engine; \
		COMMON_EXTERN_ENGINE

#define ENGINE(x) _engine.x

/**
 * low-level function is used here to reduce stack usage
 */
#define ON_FATAL_ERROR() \
		palWritePad(LED_ERROR_PORT, LED_ERROR_PIN, 1); \
		turnAllPinsOff(); \
		enginePins.communicationLedPin.setValue(1);

/*
 * Stack debugging
 */
int getRemainingStack(thread_t *otp);

#ifdef __cplusplus
}
#endif /* __cplusplus */


// 168 ticks in microsecond
#define US_TO_NT_MULTIPLIER (CORE_CLOCK / 1000000)

/**
 * converts efitimeus_t to efitick_t
 */
#define US2NT(us) (((efitime_t)(us))*US_TO_NT_MULTIPLIER)

/**
 * converts efitick_t to efitimeus_t
 */
#define NT2US(nt) ((nt) / US_TO_NT_MULTIPLIER)

#define Delay(ms) chThdSleepMilliseconds(ms)
#ifdef __cplusplus
extern "C"
{
#endif

bool lockAnyContext(void);
void unlockAnyContext(void);

#ifdef __cplusplus
}
#endif

#endif /* GLOBAL_H_ */

