/*
 * @file dynoview.h
 *
 * @date Jan 05, 2025
 * @author Alexey Ershov, (c) 2012-2025
 */

#pragma once

void updateDynoView();
int getDynoviewHP();
int getDynoviewTorque();

struct DynoPoint {
    int rpm;
    float time;
    float tps;

    float engineRps;
    float axleRps;
    float vMs;
    float mph;
    float distanceM;
    float aMs2;
    float forceN;
    float forceDragN;
    float forceTotalN;
    float torqueWheelNm;
    float torqueNm;
    float torqueLbFt;
    float hp;
};

#define window_size 7

class DynoView {
public:

    DynoView();
    void update();
    bool onRpm(int rpm, float time, float tps);

    float currentTorque;
    float currentHP;

private:

    void reset();

    static inline float accumulate_window(int size, const float* data)
    {
        float sum = 0.0;

        for(int i = 0; i < size; ++i) {
            sum += data[size - i - 1];
        }

        return sum  / window_size;
    }

    float airDensityKgM3 = 1.225; // 15C
    uint16_t wheelOverallDiameterMm = 0;

    // SAE corrections
    float saeBaroCorrectionFactor;
    float saeBaroMmhg;
    float saeTempCorrectionFactor;
    float saeVaporPressure;
    float saeCorrectionFactor;

    DynoPoint dynoViewPoint;
    DynoPoint dynoViewPointPrev;

    int count = 0;

    float tail_hp[window_size];
    float tail_torque[window_size];
};