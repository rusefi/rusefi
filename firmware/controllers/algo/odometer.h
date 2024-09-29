#pragma once

#if EFI_PROD_CODE && HAL_USE_EEPROM
#ifndef EE_PAGE_SIZE
#define EE_PAGE_SIZE 32
#endif

#ifndef EE_SIZE
#define EE_SIZE 4096
#endif

#ifndef EE_U2CD
#define EE_U2CD I2CD3
#endif

struct odometer_state {
	int totalMeters;
	int crc;
};

#endif
