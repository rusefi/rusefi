#include "pch.h"

/**
 * problem: we have three files with bits and pieces of time API and documentation
 * 1) this implementation class
 * 2) rusEFI header efitime.h
 * 3) libfirmware header rusefi_time_types.h
 */

#if !EFI_UNIT_TEST

#include <rusefi/rusefi_time_wraparound.h>

static WrapAround62 timeNt;

/**
 * 64-bit counter CPU/timer cycles since MCU reset
 */
efitick_t getTimeNowNt() {
	return timeNt.update(getTimeNowLowerNt());
}

/**
 * 64-bit result would not overflow, but that's complex stuff for our 32-bit MCU
 */
efitimeus_t getTimeNowUs() {
	ScopePerf perf(PE::GetTimeNowUs);
	return NT2US(getTimeNowNt());
}

/**
 * 32 bit return type overflows in 23(or46?) days. tag#4554. I think we do not expect rusEFI to run for 23 days straight days any time soon?
 */
efitimems_t getTimeNowMs() {
	return US2MS(getTimeNowUs());
}

/**
 * 32 bit integer number of seconds since ECU boot.
 * 31,710 years - would not overflow during our life span.
 */
efitimesec_t getTimeNowS() {
	return getTimeNowUs() / US_PER_SECOND;
}

#endif /* !EFI_UNIT_TEST */
