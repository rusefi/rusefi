/**
 * file defaults.h
 */

void setLeftRightBanksNeedBetterName();
void setDefaultBaseEngine();
void setDefaultFuel();
void setDefaultIgnition();
void setDefaultCranking();
void setGDIFueling();
void setGdiWallWetting();
void setInline4();

#define DEFAULT_CRANKING_DWELL_MS 6
#define DEFAULT_CRANKING_ANGLE 6

void setPPSInputs(adc_channel_e pps1, adc_channel_e pps2);
void setPPSCalibration(float primaryUp, float primaryDown, float secondaryUp, float secondaryDown);

void setTPS1Inputs(adc_channel_e tps1, adc_channel_e tps2);
// legacy since channel TPS still exists
void setTPS1Calibration(uint16_t tpsMin, uint16_t tpsMax);
// Most ETB require redundant dual channel TPS
void setTPS1Calibration(uint16_t tpsMin, uint16_t tpsMax, uint16_t tps1SecondaryMin, uint16_t tps1SecondaryMax);

void setCustomMap(float lowValue, float mapLowValueVoltage, float highValue, float mapHighValueVoltage);

void setEtbPID(float p, float i, float d);

// split threshold
#ifndef MAX_TPS_PPS_DISCREPANCY
#define MAX_TPS_PPS_DISCREPANCY 5.0f
#endif
