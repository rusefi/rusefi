#pragma once

#if HAL_USE_CAN
CANDriver* detectCanDevice(brain_pin_e pinRx, brain_pin_e pinTx);
static bool isValidCanTxPin(brain_pin_e) { return true; }
static bool isValidCanRxPin(brain_pin_e) { return true; }
#endif // HAL_USE_CAN

static bool allowFlashWhileRunning() { return true; }
