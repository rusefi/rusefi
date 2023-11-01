#include "pch.h"

#include "hardware.h"

/*
 * We need only a small portion of code from FOME codebase in the bootloader.
 * Mostly it's efi_gpio.cpp, flash_main.cpp, etc. needed only to make it work.
 * And stubs needed just to settle down compiler errors.
 * The whole idea of bootloader is to make it as small as possible and reasonably independent.
*/

void chDbgPanic3(const char* /*msg*/, const char* /*file*/, int /*line*/) {
}

void logHardFault(uint32_t type, uintptr_t faultAddress, struct port_extctx* ctx, uint32_t csfr) { }

// this is supposed to be taken from chconf_common.h but it does not work? I am not sure why :(
// TODO: make this be defined by chconf_common.h?
//#if ! ENABLE_PERF_TRACE
void irqEnterHook() {}
void irqExitHook() {}
void contextSwitchHook() {}
void threadInitHook(void*) {}
void onLockHook() {}
void onUnlockHook() {}
//#endif /* ENABLE_PERF_TRACE */
