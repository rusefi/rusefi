// file rusefi_wideband.h

#pragma once
#include "can.h"

// Return index of CAN bus where WBO(s) live
size_t getWidebandBus();

// Send info to the wideband controller like battery voltage, heater enable bit, etc.
void sendWidebandInfo();

// Handles ack and pong responses from the wideband bootloader
void handleWidebandCan(const size_t busIndex, const CANRxFrame &frame);

// Pings wideband controller, reply includes protocol version and FW build date
void pingWideband(uint8_t hwIndex);

// Set CAN index to given wideband controller, does not wait for ack, does not block calling thread
void setWidebandOffsetNoWait(uint8_t hwIndex, uint8_t index);

// WARNING:
// Two following functions can block thread execution while waiting for ACK from WBO
// Do not call from critical tasks!
// Usually called from bench test task
// Also do not call from ISR context

// Set the CAN index offset of any (hwIndex = 0xff) or specific (hwIndex != 0xff) wideband controller
// Addressing by hwIndex is supported by 2025 WBO FW
void setWidebandOffset(uint8_t hwIndex, uint8_t index);

// Set Sensor type for hwIndex wideband controller
// Supported by late 2025 WBO FW
void setWidebandSensorType(uint8_t hwIndex, uint8_t type);

#if EFI_WIDEBAND_FIRMWARE_UPDATE
// Update the firmware on any (hwIndex = 0xff) or hwIndex defined connected wideband controller
// Support of hwIndex require WBO bootloader 09.2025+
void updateWidebandFirmware(uint8_t hwIndex);
// Same, but loads FW from file
void updateWidebandFirmwareFromFile(uint8_t hwIndex);

#endif //EFI_WIDEBAND_FIRMWARE_UPDATE
