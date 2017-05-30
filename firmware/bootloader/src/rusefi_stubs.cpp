#include "main.h"
#include "hardware.h"
#include "efiGpio.h"
#include "global.h"


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

int getRemainingStack(thread_t */*otp*/) {
	return 99999;
}
