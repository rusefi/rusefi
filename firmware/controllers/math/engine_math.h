/**
 * @file	engine_math.h
 *
 * @date Jul 13, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef ENGINE_MATH_H_
#define ENGINE_MATH_H_

#include "engine_configuration.h"

#ifdef __cplusplus
#include "ec2.h"
#include "trigger_structure.h"
void findTriggerPosition(engine_configuration_s const *engineConfiguration, trigger_shape_s * s,
		event_trigger_position_s *position, float angleOffset);

int isInjectionEnabled(engine_configuration2_s const *engineConfiguration2);

// 'random' value to be sure we are not treating any non-zero trash as TRUE
#define MAGIC_TRUE_VALUE 153351512

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE>
class Map3D {
public:
	void init(float table[RPM_BIN_SIZE][LOAD_BIN_SIZE]);
	float getValue(float x, float xBin[], float y, float yBin[]);
	void setAll(float value);
private:
	float *pointers[LOAD_BIN_SIZE];
	int initialized;
};

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE>
void Map3D<RPM_BIN_SIZE, LOAD_BIN_SIZE>::init(float table[RPM_BIN_SIZE][LOAD_BIN_SIZE]) {
	for (int k = 0; k < LOAD_BIN_SIZE; k++)
		pointers[k] = table[k];
	initialized = MAGIC_TRUE_VALUE;
}

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE>
float Map3D<RPM_BIN_SIZE, LOAD_BIN_SIZE>::getValue(float x, float xBin[], float y, float yBin[]) {
	efiAssert(initialized == MAGIC_TRUE_VALUE, "fuel map initialized", NAN);
	return interpolate3d(x, xBin, LOAD_BIN_SIZE, y, yBin, RPM_BIN_SIZE, pointers);
}

template<int RPM_BIN_SIZE, int LOAD_BIN_SIZE>
void Map3D<RPM_BIN_SIZE, LOAD_BIN_SIZE>::setAll(float value) {
	for (int l = 0; l < LOAD_BIN_SIZE; l++) {
		for (int r = 0; r < RPM_BIN_SIZE; r++) {
			pointers[l][r] = value;
		}
	}
}

typedef Map3D<16, 16> Map3D1616;

#endif

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

//float getDefaultVE(int rpm);

float getDefaultFuel(int rpm, float map);

float getOneDegreeTimeMs(int rpm);
float getOneDegreeTimeUs(int rpm);
float getCrankshaftRevolutionTimeMs(int rpm);

int isCrankingRT(engine_configuration_s *engineConfiguration, int rpm);
#define isCrankingR(rpm) isCrankingRT(engineConfiguration, rpm)

float fixAngle(float angle);
float getTriggerEventAngle(int triggerEventIndex);

float getEngineLoadT(engine_configuration_s *engineConfiguration);
#define getEngineLoad() getEngineLoadT(engineConfiguration)

float getSparkDwellMsT(engine_configuration_s *engineConfiguration, int rpm);
#define getSparkDwellMs(rpm) getSparkDwellMsT(engineConfiguration, rpm)


int getCylinderId(firing_order_e firingOrder, int index);

void setTableBin(float array[], int size, float l, float r);
void setFuelRpmBin(engine_configuration_s *engineConfiguration, float l, float r);
void setFuelLoadBin(engine_configuration_s *engineConfiguration, float l, float r);
void setTimingRpmBin(engine_configuration_s *engineConfiguration, float l, float r);
void setTimingLoadBin(engine_configuration_s *engineConfiguration, float l, float r);

void setSingleCoilDwell(engine_configuration_s *engineConfiguration);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ENGINE_MATH_H_ */
