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

class DynoView {
public:

    void init();
    void update();
    bool onRpm(int rpm, float time, float tps);

    float currentTorque;
    float currentHP;

private:

    void reset();

    static inline void move(uint8_t size, float* data) {
        for(int i = size - 1; i > 0; --i)
        {
            memcpy(&data[i], &data[i - 1], sizeof(float));
        }
    }

    static inline float accumulate_window(uint8_t size, const float* data)
    {
        float sum = 0.0;

        for(int i = 0; i < size; ++i) {
            sum += data[size - i - 1];
        }

        return sum  / (float)size;
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
    int count_rpm = 0;
    int prev_rpm = 0;

    static constexpr int dyno_view_window_size = 7;
    static constexpr int dyno_view_window_size_rpm = 10;
    static constexpr int dyno_view_tps_min_for_run = 30;
    static constexpr int dyno_view_rpm_diff_smooth = 30;
    static constexpr float dyno_view_log_time_smooth_sec = 0.05f;
    static constexpr int dyno_view_tps_diff_to_reset_run = 10;
    static constexpr int dyno_view_rpm_fall_to_reset_run = 60;

    float tail_hp[dyno_view_window_size];
    float tail_torque[dyno_view_window_size];
    float tail_rpm[dyno_view_window_size_rpm];

    bool isInitialized = false;
};
