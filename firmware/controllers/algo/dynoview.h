/*
 * @file dynoview.h
 *
 * @date Nov 29, 2020
 * @author Alexandru Miculescu, (c) 2012-2020
 */

#pragma once

#include "engine_ptr.h"

class Logging;
void initDynoView(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);
void updateDynoView(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void updateDynoViewCan(DECLARE_ENGINE_PARAMETER_SIGNATURE);

class DynoViewBase {
public:
	DECLARE_ENGINE_PTR;

	// Update the state of the launch control system
	void update();
    void updateAcceleration(efitick_t deltaTime, float deltaSpeed);
    void updateHP();
    float getAcceleration();
    int getEngineForce();
    //in KW
    int getEnginePower();

    int getEngineHP();
    //in NM
    int getEngineTorque();
#if EFI_UNIT_TEST
    void setAcceleration(float a);
#endif
private:
	efitimeus_t timeStamp = 0;
    //km/h unit
    float vss = 0;
    //m/s/s unit
    float acceleration = 0;
    //engine force in N
    int engineForce;
    //engine power in W
    int enginePower;
    //engine powerin HP
    int engineHP;
    //Torque in lb-ft
    int engineTorque;
};