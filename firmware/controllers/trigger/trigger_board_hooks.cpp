/**
 * @file trigger_board_hooks.cpp
 *
 * ELF weak defaults for trigger board hooks.
 *
 * Windows includes the shared fallback bodies directly in each caller for
 * COFF weak-symbol resolution.
 *
 * IMPORTANT: on ELF, these definitions must NOT live in the same translation unit as
 * their call sites (trigger_central.cpp calls boardTriggerCallback,
 * trigger_decoder.cpp calls triggerGetToothProfileFactor). GCC LTO binds a
 * same-TU call to the weak body (dead-call elimination / const folding,
 * which noinline does not prevent) BEFORE the linker can select a board's
 * strong override - the board hook silently stops working. Keep them here.
 */

#include "pch.h"

#include "trigger_central.h"
#include "trigger_decoder.h"

#if !IS_WINDOWS_COMPILER
#include "trigger_central_hooks.inc"
#include "trigger_decoder_hooks.inc"
#endif
