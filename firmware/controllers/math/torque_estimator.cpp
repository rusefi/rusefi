#include "pch.h"

void estimateTorqueTable() {
  for (int rpmIndex = 0;rpmIndex<TORQUE_CURVE_RPM_SIZE;rpmIndex++) {
    for (int loadIndex = 0;loadIndex<TORQUE_CURVE_SIZE;loadIndex++) {
      float rpmValue = config->torqueRpmBins[rpmIndex];

      // let's assume speed density for now
      // here we assume load is MAP while older parts of codebase have TPS, maybe for no good reason
      // TODO: clean this up, see 'torqueLoadBins' usages
      float mapValue = config->torqueLoadBins[loadIndex];

      percent_t veValue = interpolate3d(
            			config->veTable,
            			config->veLoadBins, mapValue,
            			config->veRpmBins, rpmValue
            		);
      float estimated = engineConfiguration->referenceTorqueForGenerator
       * veValue / engineConfiguration->referenceVeForGenerator
       * mapValue / engineConfiguration->referenceMapForGenerator;

      config->torqueTable[loadIndex][rpmIndex] = estimated;
      if (loadIndex == 0 && rpmIndex == 0) {
        efiPrintf("estimateTorqueTable [%f] %f", veValue, estimated);
      }

    }
  }

}
