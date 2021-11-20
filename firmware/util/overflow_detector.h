#pragma once

#if EFI_PROD_CODE

// no-op out for real firmware
template <typename T>
struct overflow_detect : public T { };

#else // not EFI_PROD_CODE

#include <sanitizer/asan_interface.h>

template <typename T>
struct overflow_detect : public T {
private:
	uint8_t protector[16];

public:
	overflow_detect() {
		__asan_poison_memory_region(&protector, sizeof(protector));
	}

	~overflow_detect() {
		__asan_unpoison_memory_region(&protector, sizeof(protector));
	}
};

#endif
