#pragma once

// Indicate that an ack response was received from the wideband bootloader
void handleWidebandBootloaderAck();
// Update the firmware on any connected wideband controller
void updateWidebandFirmware();
// Set the CAN index offset of any attached wideband controller
void setWidebandOffset(uint8_t index);
// Send info to the wideband controller like battery voltage, heater enable bit, etc.
void sendWidebandInfo();
