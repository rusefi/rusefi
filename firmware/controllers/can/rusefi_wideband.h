// file rusefi_wideband.h

#pragma once
#include "can.h"

// Send info to the wideband controller like battery voltage, heater enable bit, etc.
void sendWidebandInfo();

// Handles ack and pong responses from the wideband bootloader
void handleWidebandCan(const CANRxFrame &frame);

// WARNING:
// Two following functions can block thread execution while waiting for ACK from WBO
// Do not call from critical tasks!
// Usually called from bench test task
// Also do not call from ISR context

// Set the CAN index offset of any attached wideband controller
void setWidebandOffset(uint8_t index);

#if EFI_WIDEBAND_FIRMWARE_UPDATE
// Update the firmware on any connected wideband controller
void updateWidebandFirmware();
#endif //EFI_WIDEBAND_FIRMWARE_UPDATE
