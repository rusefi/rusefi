#pragma once


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if EFI_PROD_CODE
#include <hal.h>

void logHardFault(uint32_t type, uintptr_t faultAddress, void* sp, port_extctx* ctx, uint32_t csfr);
#endif // EFI_PROD_CODE

#ifdef __cplusplus
}
#endif /* __cplusplus */