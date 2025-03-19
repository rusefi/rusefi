/*
 * file ign_voltage_gatekeeper.h
 */

#include "ignition_controller.h"

class IgnVoltageGatekeeper {
public:

  bool isInitialized = false;

  bool haveVoltage() {
    bool withVoltage = isIgnVoltage();
    if (!withVoltage) {
      if (isInitialized) {
        efiPrintf("Power loss? Would have to re-init smart chip.");
        isInitialized = false;
      } else {
        efiPrintf("Smart chip needs battery voltage");
      }
    }
    return withVoltage;
  }
};
