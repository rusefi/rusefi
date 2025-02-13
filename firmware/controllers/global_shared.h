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
 * Of note is that interrupts are NOT serviced on the stack of the thread that was running when the
 * interrupt occurred. The only thing that happens on that thread's stack is that its registers are
 * pushed (by hardware) when an interrupt occurs, just before swapping the stack pointer out for the
 * main stack (currently 0x400=1024 bytes), where the ISR actually runs.
 * see also __main_stack_size__
 * see also __process_stack_size__
 *
 * see also http://www.chibios.org/dokuwiki/doku.php?id=chibios:kb:stacks
 *
 * In the firmware we are using 'extern *Engine' - in the firmware Engine is a singleton
 *
 * On the other hand, in order to have a meaningful unit test we are passing Engine * engine as a parameter
 */

#include "global.h"

/*
 * Stack debugging
 */
int getRemainingStack(thread_t *otp);
int CountFreeStackSpace(const void* wabase);

#define assertStackVoid(message, code, desiredAvailableStack) { if (getCurrentRemainingStack() < desiredAvailableStack) { firmwareError(code, "stack: %s", message); return; } }
#define assertStack(message, code, desiredAvailableStack, result) { if (getCurrentRemainingStack() < desiredAvailableStack) { firmwareError(code, "stack: %s", message); return result; } }
