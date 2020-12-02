/*
 * @file dynoview.cpp
 *
 * @date Nov 29, 2020
 * @author Alexandru Miculescu, (c) 2012-2020
 */

#include "engine.h"

#if EFI_DYNO_VIEW
#include "dynoview.h"
#include "vehicle_speed.h"

static Logging *logger;

#if EFI_TUNER_STUDIO
#include "tunerstudio_outputs.h"
extern TunerStudioOutputChannels tsOutputChannels;
#endif /* EFI_TUNER_STUDIO */

EXTERN_ENGINE;

DynoViewBase dynoInstance;

void DynoViewBase::update(vssSrc src){

    efitimeus_t timeNow, deltaTime;
    float speed,deltaSpeed;
    timeNow = getTimeNowUs();
    speed = getVehicleSpeed();
    if (src == ICU) {
        speed = efiRound(speed,1.0);
    } else {
        //use speed with 0.1 precision
        speed = efiRound(speed,0.001);
    }

    if(timeStamp != 0) {

        if ( vss!= speed ) {
            deltaTime = timeNow - timeStamp;
            if (vss > speed) {
                deltaSpeed = (vss - speed)*(-1);
            } else {
                deltaSpeed = speed - vss;
            }

            updateAcceleration(deltaTime, deltaSpeed);
#if EFI_TUNER_STUDIO
	        if (CONFIG(debugMode) == DBG_44) {
		        tsOutputChannels.debugIntField1 = deltaTime;
		        tsOutputChannels.debugFloatField1 = vss;
		        tsOutputChannels.debugFloatField2 = speed;
		        tsOutputChannels.debugFloatField3 = deltaSpeed;
	        }
#endif /* EFI_TUNER_STUDIO */        
            updateHP();

            //save data
            timeStamp = timeNow;
            vss = speed;
        }


    } else {
        //ensure we grab init values
        timeStamp = timeNow;
        vss = speed;
    }



}

/**
 * input units: deltaSpeed in km/h
 *              deltaTime in uS
 */
void DynoViewBase::updateAcceleration(efitimeus_t deltaTime, float deltaSpeed) {
    if (deltaSpeed != 0.0) {
        acceleration = ((deltaSpeed / 3.6) / (deltaTime / 1000000.0));
    } else {
        acceleration = 0;
    }
}

/**
 * E = m*a
 * ex. 900 (kg) * 1.5 (m/s^2) = 1350N
 * P = F*V
 * 1350N * 35(m/s) = 47250Watt (35 m/s is the final velocity)
 * 47250 * (1HP/746W) = 63HP
 * https://www.youtube.com/watch?v=FnN2asvFmIs
 * we do not take resistence into account right now.
 */
void DynoViewBase::updateHP() {

    //these are actually at the wheel
    //we would need final drive to calcualte the correct torque at the wheel
    engineForce = CONFIG(vehicleWeight) * acceleration;
    enginePower = engineForce * (vss / 3.6);
    engineHP = enginePower / 746;
    if (isValidRpm(GET_RPM())) { 
        engineTorque = ((engineHP * 5252) / GET_RPM());  
    }

}

#if EFI_UNIT_TEST
void DynoViewBase::setAcceleration(float a) {
    acceleration = a;
}
#endif

float DynoViewBase::getAcceleration() {
    return acceleration;
}

int DynoViewBase::getEngineForce() {
    return engineForce;
}

int DynoViewBase::getEnginePower() {
    return (enginePower/1000);
}

int DynoViewBase::getEngineHP() {
    return engineHP;
}

int DynoViewBase::getEngineTorque() {
    return (engineTorque/0.73756);
}

float getDynoviewAcceleration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
    return dynoInstance.getAcceleration();
}

int getDynoviewPower(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
    return dynoInstance.getEnginePower();
}

/**
 * Periodic update function called from SlowCallback.
 * Only updates if we have Vss from input pin.
 */
void updateDynoView(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
    if ((CONFIG(vehicleSpeedSensorInputPin) != GPIO_UNASSIGNED) &&
        (!CONFIG(enableCanVss))) {
        dynoInstance.update(ICU);
    }
}

/**
 * This function is called after every CAN msg received, we process it
 * as soon as we can to be more acurate.
 */ 
void updateDynoViewCan(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
    if (!CONFIG(enableCanVss)) {
        return;
    }
    
    dynoInstance.update(CAN);
}

void initDynoView(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
	logger = sharedLogger;
}

#endif /* EFI_DYNO_VIEW */