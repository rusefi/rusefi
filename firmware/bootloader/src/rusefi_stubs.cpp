#include "global.h"
#include "hardware.h"
#include "efiGpio.h"

/*
 * We need only a small portion of code from rusEFI codebase in the bootloader.
 * Mostly it's tunerstudio_io.cpp. And other files like efiGpio.cpp etc. needed only to make it work.
 * And stubs needed just to settle down compiler errors.
 * The whole idea of bootloader is to make it as small as possible. And reasonably independent.
*/

int maxNesting = 0;

void chDbgPanic3(const char */*msg*/, const char * /*file*/, int /*line*/) {
}

void print(const char */*format*/, ...) {
}

void scheduleMsg(Logging */*logging*/, const char */*fmt*/, ...) {
}

void firmwareError(obd_code_e /*code*/, const char */*fmt*/, ...) {
}

Logging::Logging(char const */*name*/, char */*buffer*/, int /*bufferSize*/) {
}

LoggingWithStorage::LoggingWithStorage(const char *name) : Logging(name, DEFAULT_BUFFER, sizeof(DEFAULT_BUFFER))   {
}

