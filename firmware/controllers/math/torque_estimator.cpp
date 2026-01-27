#include "pch.h"

void estimateTorqueTable() {
  for (int rpmIndex = 0;rpmIndex<TORQUE_CURVE_RPM_SIZE;rpmIndex++) {
    for (int loadIndex = 0;loadIndex<TORQUE_CURVE_SIZE;loadIndex++) {
      float rpmValue = config->torqueRpmBins[rpmIndex];

      // let's assume speed density for onw
      float mapValue = config->torqueLoadBins[loadIndex];

      percent_t veValue = interpolate3d(
            			config->idleVeTable,
            			config->idleVeLoadBins, mapValue,
            			config->idleVeRpmBins, rpmValue
            		);
      float estimated = engineConfiguration->referenceTorqueForGenerator
       * veValue / engineConfiguration->referenceVeForGenerator
       * mapValue / engineConfiguration->referenceMapForGenerator;

      config->torqueTable[loadIndex][rpmIndex] = estimated;

    }
  }

}
