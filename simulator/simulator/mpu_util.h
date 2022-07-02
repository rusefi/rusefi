#pragma once

CANDriver* detectCanDevice(brain_pin_e pinRx, brain_pin_e pinTx);
bool isValidCanTxPin(brain_pin_e) { return true; }
bool isValidCanRxPin(brain_pin_e) { return true; }
