// file rusefi_wideband.h

#pragma once
#include "can.h"

// Indicate that an ack response was received from the wideband bootloader
void handleWidebandCan(const CANRxFrame &frame);
// Send info to the wideband controller like battery voltage, heater enable bit, etc.
void sendWidebandInfo();


// WARNING:
// Two following functions can block thread execution while waiting for ACK from WBO
// Do not call from critical tasks!
// Usually called from bench test task
// Also do not call from ISR context

// Update the firmware on any connected wideband controller
void updateWidebandFirmware();
// Set the CAN index offset of any attached wideband controller
void setWidebandOffset(uint8_t index);
