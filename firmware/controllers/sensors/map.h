#ifndef MAP_H_
#define MAP_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @return Raw MAP sensor value right now
 */
float getRawMap(void);
float getBaroPressure(void);
/**
 * @return MAP value averaged within a window of measurement
 */
float getMap(void);
float getMapVoltage(void);
float getMapByVoltage(float voltage);
float getMAPValueHonda_Denso183(float volts);
float getMAPValueMPX_4250(float volts);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
