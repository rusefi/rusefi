/*
 * @file dynoview.cpp
 *
 * @date Jan 05, 2025
 * @author Alexey Ershov, (c) 2012-2025
 */

#include "pch.h"

#if EFI_DYNO_VIEW
#include "dynoview.h"

void DynoView::init()
{
  if (isInitialized) {
    return;
  }
  isInitialized = true;
    wheelOverallDiameterMm = (uint16_t)(config->dynoCarWheelDiaInch * 25.4 + config->dynoCarWheelTireWidthMm * config->dynoCarWheelAspectRatio * 0.01 * 2);

    saeVaporPressure = 6.1078 * pow(10.0, (7.5 * config->dynoSaeTemperatureC) / (237.3 + config->dynoSaeTemperatureC)) * .02953 * (config->dynoSaeRelativeHumidity / 100.0);
    saeBaroMmhg = 29.23 * (config->dynoSaeBaro / 100.0);
    saeBaroCorrectionFactor = 29.23 / (saeBaroMmhg - saeVaporPressure);
    saeTempCorrectionFactor = pow(((config->dynoSaeTemperatureC + 273.0) / 298.0), 0.5);
    saeCorrectionFactor = 1.176 * (saeBaroCorrectionFactor * saeTempCorrectionFactor) - .176;

    reset();
}

void DynoView::update()
{
  init();
    float rpm = Sensor::getOrZero(SensorType::Rpm);
    rpm = efiRound(rpm, 1.0);
    int intRpm = (int)rpm;

    float tps = Sensor::getOrZero(SensorType::Tps1);

    if(intRpm > 0) {
        efitimeus_t timeStamp = getTimeNowUs();
        float timeInSec = (float)(timeStamp) / US_PER_SECOND;

        onRpm(intRpm, timeInSec, tps);
    }
}

void DynoView::reset()
{
    dynoViewPointPrev.rpm = -1;
    dynoViewPointPrev.time = -1;
    dynoViewPointPrev.tps = -1;
    count = 0;
    currentTorque = 0;
    currentHP = 0;
}

bool DynoView::onRpm(int rpm, float time, float tps)
{
    if(tps < dynoViewPointPrev.tps || tps < 80) {
        reset();
        return false;
    }

    if (dynoViewPointPrev.rpm > 0 && dynoViewPointPrev.time > 0)
    {
        if(time < dynoViewPointPrev.time || rpm < dynoViewPointPrev.rpm)
        {
            return false;
        }

        int rpmDiff = rpm - dynoViewPointPrev.rpm;

        if (rpmDiff < config->dynoRpmStep)
        {
            return false;
        }
    }

    dynoViewPoint.rpm = rpm;
    dynoViewPoint.time = time;
    dynoViewPoint.tps = tps;

    dynoViewPoint.engineRps = dynoViewPoint.rpm / 60.0;
    dynoViewPoint.axleRps = dynoViewPoint.engineRps / (config->dynoCarGearPrimaryReduction * config->dynoCarGearRatio * config->dynoCarGearFinalDrive);
    dynoViewPoint.vMs = dynoViewPoint.axleRps * (wheelOverallDiameterMm / 1000.f) * 3.1416;
    dynoViewPoint.mph = dynoViewPoint.vMs * 2.2369363;

    if (dynoViewPointPrev.rpm > 0 && dynoViewPointPrev.time > 0)
    {
        dynoViewPoint.distanceM = ((dynoViewPoint.vMs + dynoViewPointPrev.vMs) / 2.0) * (dynoViewPoint.time - dynoViewPointPrev.time);
        dynoViewPoint.aMs2 = (dynoViewPoint.vMs - dynoViewPointPrev.vMs) / (dynoViewPoint.time - dynoViewPointPrev.time);
        dynoViewPoint.forceN = (config->dynoCarCargoMassKg + config->dynoCarCarMassKg) * dynoViewPoint.aMs2;
        dynoViewPoint.forceDragN = 0.5 * airDensityKgM3 * (dynoViewPoint.vMs * dynoViewPoint.vMs) * config->dynoCarFrontalAreaM2 * config->dynoCarCoeffOfDrag;

        dynoViewPoint.forceDragN = dynoViewPoint.forceDragN * saeCorrectionFactor;

        dynoViewPoint.forceTotalN = dynoViewPoint.forceN + dynoViewPoint.forceDragN;
        dynoViewPoint.torqueWheelNm = dynoViewPoint.forceTotalN * ((wheelOverallDiameterMm / 2.0) / 1000.0);
        dynoViewPoint.torqueNm = dynoViewPoint.torqueWheelNm / (config->dynoCarGearPrimaryReduction * config->dynoCarGearRatio * config->dynoCarGearFinalDrive);
        dynoViewPoint.torqueLbFt = dynoViewPoint.torqueNm * 0.737562;
        dynoViewPoint.hp = dynoViewPoint.torqueLbFt * dynoViewPoint.rpm / 5252.0;

        for(int i = 0; i < window_size-1; ++i)
        {
            memcpy(&tail_hp[i], &tail_hp[i + 1], sizeof(float));
            memcpy(&tail_torque[i], &tail_torque[i + 1], sizeof(float));
        }

        tail_torque[window_size-1] = dynoViewPoint.torqueNm;
        tail_hp[window_size-1] = dynoViewPoint.hp;

        if(count == 0)
        {
            for(int i = 0; i < window_size-1; ++i) {
                memcpy(&tail_hp[i], &tail_hp[window_size-1], sizeof(float));
                memcpy(&tail_torque[i], &tail_torque[window_size-1], sizeof(float));
            }
        }

        int accumulate_window_size = std::min(count, window_size);

        currentTorque = accumulate_window(accumulate_window_size, tail_torque);
        currentHP = accumulate_window(accumulate_window_size, tail_hp);

        if(count < window_size) {
            ++count;
        }

        dynoViewPointPrev = dynoViewPoint;
        return true;
    }

    dynoViewPointPrev = dynoViewPoint;

    return false;
}

int getDynoviewHP() {
    return engine->dynoInstance.currentHP;
}

int getDynoviewTorque() {
    return engine->dynoInstance.currentTorque;
}

/**
 * Periodic update function called from SlowCallback.
 */
void updateDynoView() {
    engine->dynoInstance.update();
}

#endif /* EFI_DYNO_VIEW */
