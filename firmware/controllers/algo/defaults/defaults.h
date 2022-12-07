/**
 * file defaults.h
 */

void setLeftRightBanksNeedBetterName();
void setDefaultBaseEngine();
void setDefaultFuel();
void setDefaultIgnition();
void setDefaultCranking();

void setPPSInputs(adc_channel_e pps1, adc_channel_e pps2);
void setPPSCalibration(float primaryUp, float primaryDown, float secondaryUp, float secondaryDown);

void setTPS1Inputs(adc_channel_e tps1, adc_channel_e tps2);
void setTPS1Calibration(uint16_t tpsMin, uint16_t tpsMax, uint16_t tps1SecondaryMin, uint16_t tps1SecondaryMax);

void setEtbPID(float p, float i, float d);