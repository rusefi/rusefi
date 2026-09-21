#pragma once


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if EFI_PROD_CODE
#include <hal.h>

// Stamps the crash cookie into backup SRAM FIRST, then snapshots registers/stack from sp - sp may be garbage.
void logHardFault(uint32_t type, uintptr_t faultAddress, void* sp, uint32_t csfr);
#endif // EFI_PROD_CODE

#ifdef __cplusplus
}
#endif /* __cplusplus */
