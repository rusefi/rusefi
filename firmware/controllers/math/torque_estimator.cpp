#include "pch.h"
#include "second_tables.h"

void estimateTorqueTable() {
  page4_s* page4 = secondTablesGetState();
  for (int rpmIndex = 0;rpmIndex<TORQUE_CURVE_RPM_SIZE;rpmIndex++) {
    for (int loadIndex = 0;loadIndex<TORQUE_CURVE_SIZE;loadIndex++) {
      float rpmValue = page4->torqueRpmBins[rpmIndex];

      // let's assume speed density for now
      // here we assume load is MAP while older parts of codebase have TPS, maybe for no good reason
      // TODO: clean this up, see 'torqueLoadBins' usages
      float mapValue = page4->torqueLoadBins[loadIndex];

      percent_t veValue = interpolate3d(
            			config->veTable,
            			config->veLoadBins, mapValue,
            			config->veRpmBins, rpmValue
            		);
      float estimated = engineConfiguration->referenceTorqueForGenerator
       * veValue / engineConfiguration->referenceVeForGenerator
       * mapValue / engineConfiguration->referenceMapForGenerator;

      page4->torqueTable[loadIndex][rpmIndex] = estimated;
      if (loadIndex == 0 && rpmIndex == 0) {
        efiPrintf("estimateTorqueTable [%f] %f", veValue, estimated);
      }

    }
  }

}
