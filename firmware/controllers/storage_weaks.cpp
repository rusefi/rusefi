/**
 * @file storage_weaks.cpp
 *
 * ELF weak defaults for board-provided storage handlers.
 *
 * Windows includes the shared fallback bodies directly in each caller for
 * COFF weak-symbol resolution.
 *
 * IMPORTANT: on ELF, these definitions must NOT live in storage.cpp (the same
 * translation unit as the storage manager dispatch call sites). GCC LTO
 * binds a same-TU call to the weak body and const-folds it ('return true',
 * which noinline does not prevent) BEFORE the linker can select a board's
 * strong override - the board handlers get dead-code-eliminated and the
 * dispatch silently becomes a no-op. Keep the weak defaults here.
 */

#include "pch.h"

#include "storage.h"

#if !IS_WINDOWS_COMPILER
#include "storage_board_hooks.inc"
#endif
