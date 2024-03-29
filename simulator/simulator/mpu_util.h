#pragma once

#if HAL_USE_CAN
CANDriver* detectCanDevice(brain_pin_e pinRx, brain_pin_e pinTx);
inline bool isValidCanTxPin(brain_pin_e) { return true; }
inline bool isValidCanRxPin(brain_pin_e) { return true; }
#endif // HAL_USE_CAN

inline bool allowFlashWhileRunning() { return true; }
