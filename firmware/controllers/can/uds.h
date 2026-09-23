#pragma once

#include "can.h"

#if EFI_UDS && (EFI_CAN_SUPPORT || EFI_UNIT_TEST)
// Physical, read-only rusEFI identity. Board-specific diagnostics remain separate.
void udsOnCanPacketRx(const CANRxFrame& frame, size_t busIndex);
#endif
