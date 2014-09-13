/*
 * @file	global.h
 *
 * @date Nov 28, 2013
 * @author pc
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <time.h>
#include <string.h>
#include <stdbool.h>

#include "efitime.h"

#define US_TO_NT_MULTIPLIER 100

#define US2NT(x) (US_TO_NT_MULTIPLIER * (x))

#define NT2US(x) ((x) / US_TO_NT_MULTIPLIER)


typedef int bool_t;

typedef void * Logging;

#define EFI_ERROR_CODE 0xffffffff
#define TRUE 1
#define FALSE 0

#define CCM_OPTIONAL

#endif /* GLOBAL_H_ */
