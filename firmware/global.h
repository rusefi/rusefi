/*
 * @file global.h
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <ch.h>
#include <hal.h>
#include <string.h>

// this is about MISRA not liking 'time.h'. todo: figure out something
#if defined __GNUC__
#include <sys/types.h>
#else
typedef unsigned int time_t;
#endif

#include "efifeatures.h"
#include "rusefi_enums.h"
#include "obd_error_codes.h"
#include "error_handling.h"

/* definition to expand macro then apply to pragma message */
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)

// project-wide default thread stack size
// see also PORT_INT_REQUIRED_STACK
#define UTILITY_THREAD_STACK_SIZE 128

#define EFI_ERROR_CODE 0xffffffff

#if EFI_USE_CCM && defined __GNUC__
#define CCM_OPTIONAL __attribute__((section(".ccm")));
#else
#define CCM_OPTIONAL @ ".ccm"
#endif

// this stuff is about ChibiOS 2.6 > Migration
typedef VirtualTimer virtual_timer_t;
typedef EventListener event_listener_t;
typedef Thread thread_t;

#define THD_WORKING_AREA WORKING_AREA

#endif /* GLOBAL_H_ */
