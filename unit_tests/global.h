/*
 * @file global.h
 *
 * Global header file for unit tests
 *
 * @date Nov 28, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <stdio.h>
#include <time.h>

#include "common_headers.h"
#include "test_executor.h"

#define EFU_UNIT_TESTS fail("typo please EFU_UNIT_TEST");

typedef uint32_t iomode_t;
typedef uint32_t ioportid_t;
typedef uint32_t ioportmask_t;

#define DL_OUTPUT_BUFFER 200

// just a stub implementation for unit tests
#define getCurrentRemainingStack() (999999)

#define EXPECT_NEAR_M3(x, y) EXPECT_NEAR((x), (y), 1e-3)
#define EXPECT_NEAR_M4(a, b) EXPECT_NEAR(a, b, 1e-4)


#ifdef __cplusplus
#include "mock-threads.h"
// todo: include it right here? #include "unit_test_framework.h"
extern "C"
{
#endif /* __cplusplus */

#define CH_FREQUENCY 1000
#define NO_CACHE

typedef int bool_t;
typedef uint32_t systime_t;

void chDbgAssert(int c, char *msg, void *arg);

#define TICKS_IN_MS 100

#define chDbgCheck(x, y) chDbgAssert(x, y, NULL)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#define VCS_VERSION "321"
#define RUS_EFI_VERSION_TAG "rusEfiVersion"

#define EFI_ERROR_CODE 0xffffffff

#define CCM_OPTIONAL

#define chSysLock() {}
#define chSysUnlock() {}
#define osalThreadDequeueNextI(x, y) {}

#ifdef __cplusplus
namespace chibios_rt {
	// Noop for unit tests - this does real lock in FW/sim
	class CriticalSectionLocker { };
}
#endif

#define UNIT_TEST_BUSY_WAIT_CALLBACK() { advanceTimeUs(1); }

#define chsnprintf snprintf
#define chvsnprintf vsnprintf
