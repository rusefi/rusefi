/*
 * @file dynoview.cpp
 *
 * @date Jan 05, 2025
 * @author Alexey Ershov, (c) 2012-2025
 */

#include "pch.h"

#if EFI_DYNO_VIEW
#include "dynoview.h"
#include "fuel_math.h"

void DynoView::init()
{
    if (isInitialized) {
        return;
    }
    isInitialized = true;

    //config->dynoSaeBaro = Sensor::get(SensorType::BarometricPressure).value_or(config->dynoSaeBaro);

    wheelOverallDiameterMm = (uint16_t)((float)config->dynoCarWheelDiaInch * 25.4f + (float)config->dynoCarWheelTireWidthMm * (float)config->dynoCarWheelAspectRatio * 0.01f * 2.0f);

    saeVaporPressure = 6.1078f * pow(10.0f, (7.5f * (float)config->dynoSaeTemperatureC) / (237.3f + (float)config->dynoSaeTemperatureC)) * .02953f * ((float)config->dynoSaeRelativeHumidity / 100.0f);
    saeBaroMmhg = 29.23f * (config->dynoSaeBaro / 100.0f);
    saeBaroCorrectionFactor = 29.23f / (saeBaroMmhg - saeVaporPressure);
    saeTempCorrectionFactor = pow((((float)config->dynoSaeTemperatureC + C_K_OFFSET) / 298.0f), 0.5f);
    saeCorrectionFactor = 1.176f * (saeBaroCorrectionFactor * saeTempCorrectionFactor) - .176f;

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
    count_rpm = 0;
    currentTorque = 0;
    currentHP = 0;
}

bool DynoView::onRpm(int rpm, float time, float tps)
{
    if(tps < dyno_view_tps_min_for_run || dynoViewPointPrev.tps - tps > dyno_view_tps_diff_to_reset_run) {
        reset();
        return false;
    }

    if (dynoViewPointPrev.rpm > 0 && dynoViewPointPrev.time > 0)
    {
        if(abs(rpm - prev_rpm) < 1) {
            return false;
        }
        prev_rpm = rpm;

        // more smoothly
        if((time - dynoViewPointPrev.time) < dyno_view_log_time_smooth_sec)
        {
            return false;
        }

        int rpmDiffSmooth = abs(rpm - dynoViewPointPrev.rpm);

        if(rpmDiffSmooth < dyno_view_rpm_diff_smooth) {
            return false;
        }

        DynoView::move(dyno_view_window_size_rpm, tail_rpm);
        tail_rpm[0] = rpm;

        count_rpm++;
        int accumulate_window_size = std::min(count_rpm, dyno_view_window_size_rpm);
        dynoViewPoint.rpm = (int)accumulate_window(accumulate_window_size, tail_rpm);

        if(dynoViewPoint.rpm + dyno_view_rpm_fall_to_reset_run < dynoViewPointPrev.rpm)
        {
            reset();
            return false;
        }

        int rpmDiffStep = abs(dynoViewPoint.rpm - dynoViewPointPrev.rpm);

        if (rpmDiffStep < config->dynoRpmStep)
        {
            return false;
        }

    } else {
        dynoViewPoint.rpm = rpm;
    }

    //dynoViewPoint.rpm = rpm;
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
        if(dynoViewPoint.aMs2 < 0) {
            dynoViewPoint.aMs2 = 0;
        }
        dynoViewPoint.forceN = (config->dynoCarCargoMassKg + config->dynoCarCarMassKg) * dynoViewPoint.aMs2;
        dynoViewPoint.forceDragN = 0.5 * airDensityKgM3 * (dynoViewPoint.vMs * dynoViewPoint.vMs) * config->dynoCarFrontalAreaM2 * config->dynoCarCoeffOfDrag;

        dynoViewPoint.forceDragN = dynoViewPoint.forceDragN * saeCorrectionFactor;

        dynoViewPoint.forceTotalN = dynoViewPoint.forceN + dynoViewPoint.forceDragN;
        dynoViewPoint.torqueWheelNm = dynoViewPoint.forceTotalN * ((wheelOverallDiameterMm / 2.0) / 1000.0);
        dynoViewPoint.torqueNm = dynoViewPoint.torqueWheelNm / (config->dynoCarGearPrimaryReduction * config->dynoCarGearRatio * config->dynoCarGearFinalDrive);
        dynoViewPoint.torqueLbFt = dynoViewPoint.torqueNm * 0.737562;
        dynoViewPoint.hp = dynoViewPoint.torqueLbFt * dynoViewPoint.rpm / 5252.0;

        DynoView::move(dyno_view_window_size, tail_hp);
        DynoView::move(dyno_view_window_size, tail_torque);

        tail_torque[0] = dynoViewPoint.torqueNm;
        tail_hp[0] = dynoViewPoint.hp;

        if(count < dyno_view_window_size) {
            ++count;
        }

        int accumulate_window_size = std::min(count, dyno_view_window_size);

        currentTorque = accumulate_window(accumulate_window_size, tail_torque);
        currentHP = accumulate_window(accumulate_window_size, tail_hp);

        dynoViewPointPrev = dynoViewPoint;
        return true;
    }

    dynoViewPointPrev = dynoViewPoint;

    return false;
}

int getDynoviewHP() {
    return (int)engine->dynoInstance.currentHP;
}

int getDynoviewTorque() {
    return (int)engine->dynoInstance.currentTorque;
}

/**
 * Periodic update function called from SlowCallback.
 */
void updateDynoView() {
    engine->dynoInstance.update();
}

#endif /* EFI_DYNO_VIEW */
