#include "pch.h"
#include "dynoview.h"

struct DynoViewTestData {
    const float time;
    const int rpm;
    const int tps;
    const float torque;
    const float hp;
};

// time, rpm, tps, torque, hp (got from real car)
std::vector<DynoViewTestData> testData = {
        // run 1
        {1200.07,2283,98,0,0},
        {1200.43,2386,98,16.8726,5.40956},
        {1200.82,2497,98,33.7452,10.8191},
        {1201.15,2601,98,50.6178,16.2287},
        {1201.51,2709,98,69.3862,22.5175},
        {1201.81,2817,98,91.9344,31.2339},
        {1202.19,2948,98,118.506,42.7111},
        {1202.47,3056,98,149.807,57.6313},
        {1202.77,3166,98,155.283,62.1219},
        {1203,3275,98,167.756,69.9024},
        {1203.25,3380,98,174.816,75.5194},
        {1203.46,3488,98,188.899,84.5814},
        {1203.73,3625,98,198.748,92.2807},
        {1203.99,3735,98,204.152,97.6292},
        {1204.24,3851,98,209.562,103.394},
        {1204.52,3962,98,211.882,107.395},
        {1204.73,4077,98,217.039,113.86},
        {1204.99,4194,98,219.407,118.429},
        {1205.16,4295,98,224.985,125.631},
        {1205.35,4397,98,227.317,130.705},
        {1205.57,4497,98,229.777,135.27},
        {1205.78,4620,98,238.062,143.976},
        {1206.01,4729,98,243.519,150.461},
        {1206.18,4837,98,249.851,158.641},
        {1206.38,4942,98,255,165.381},
        {1206.6,5064,98,252.994,168.213},
        {1206.76,5169,98,261.168,178.033},
        {1206.99,5295,98,267.731,186.344},
        {1207.15,5396,98,270.951,193.047},
        {1207.41,5536,98,275.685,200.357},
        {1207.67,5665,98,267.277,198.63},
        {1207.82,5779,98,282.833,215.403},
        {1208.06,5882,98,275.542,213.68},
        {1208.26,6000,98,271.978,215.814},
        {1208.46,6103,98,270.519,218.782},
        {1208.66,6208,98,264.809,218.632},
        {1208.89,6321,98,262.324,220.478},
        {1209.06,6422,98,269.139,230.267},
        {1209.3,6522,98,248.092,216.295},
        {1209.48,6624,98,257.483,228.171},

        // not run
        {1209.48,6624,50,0, 0},
        {1209.68,5624,10,0, 0},
        {1209.88,4624,18,0, 0},

        // run 2
        {1223.23,2859,98,0,0},
        {1223.6,2960,98,18.0707,7.25545},
        {1223.92,3067,98,36.1415,14.5109},
        {1224.23,3187,98,54.2122,21.7663},
        {1224.52,3291,98,72.3119,29.2901},
        {1224.77,3397,98,101.94,43.0101},
        {1225.02,3498,98,135.702,60.1274},
        {1225.25,3601,98,174.089,81.035},
        {1225.48,3709,98,187.111,89.7217},
        {1225.67,3810,98,200.139,99.0021},
        {1225.89,3911,98,205.137,104.346},
        {1226.1,4013,98,213.652,111.558},
        {1226.31,4122,98,220.305,118.264},
        {1226.6,4235,98,219.791,120.732},
        {1226.79,4335,98,225.097,127.019},
        {1227.01,4447,98,228.05,132.089},
        {1227.2,4554,98,230.712,137.465},
        {1227.46,4678,98,232.211,141.742},
        {1227.64,4786,98,240.061,150.525},
        {1227.81,4888,98,245.58,158.046},
        {1228.03,4993,98,251.713,165.144},
        {1228.2,5099,98,258.621,173.842},
        {1228.41,5203,98,258.277,177.209},
        {1228.57,5303,98,262.57,184.384},
        {1228.81,5426,98,265.514,189.995},
        {1229.01,5531,98,260.742,190.49},
        {1229.24,5643,98,253.664,189.145},
        {1229.47,5783,98,262.544,199.953},
        {1229.74,5893,98,249.408,193.751},
        {1229.94,5999,98,252.283,199.944},
        {1230.16,6101,98,242.73,196.479},
        {1230.37,6219,98,246.382,203.567},
        {1230.58,6331,98,248.105,209.074},
        {1230.78,6432,98,251.875,216.15},
        {1230.99,6543,98,246.177,215.323},
        {1231.21,6650,98,251.7,223.536},
        {1231.44,6754,98,247.394,223.332},
        {1231.67,6879,98,254.677,233.926},
        {1231.78,7032,98,302.062,285.134},
        {1232.11,7232,98,307.492,295.345}
};

TEST(DynoView, Base) {

    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    DynoView dut;

    config->dynoRpmStep = 100;

    config->dynoSaeTemperatureC = 20;
    config->dynoSaeBaro = STD_ATMOSPHERE;
    config->dynoSaeRelativeHumidity = 80;

    config->dynoCarWheelDiaInch = 18;
    config->dynoCarWheelTireWidthMm = 235;
    config->dynoCarWheelAspectRatio = 40;

    config->dynoCarGearPrimaryReduction = 1;
    config->dynoCarGearRatio = 1.160;
    config->dynoCarGearFinalDrive = 5.358;

    config->dynoCarCarMassKg = 1413;
    config->dynoCarCargoMassKg = 180;
    config->dynoCarCoeffOfDrag = 0.29;
    config->dynoCarFrontalAreaM2 = 1.8580608;

    Sensor::setMockValue(SensorType::Tps1, 0);
    dut.update();

    auto lastTime = testData[0].time - 0.01;
    for (size_t i = 0; i < testData.size(); ++i) {
        auto point = testData[i];

        auto timeDiff = int((point.time - lastTime) * 1000);
        lastTime = point.time;

        eth.moveTimeForwardMs(timeDiff);
        Sensor::setMockValue(SensorType::Rpm,  point.rpm);
        Sensor::setMockValue(SensorType::Tps1, point.tps);
        dut.update();

        // std::cerr << "rpm: " << point.rpm << ", hp: " << dut.currentHP << ", Torque: " << dut.currentTorque << std::endl;

        ASSERT_NEAR(point.hp, dut.currentHP, 10);
        ASSERT_NEAR(point.torque, dut.currentTorque, 10);
    }
}

TEST(DynoView, SlowCallback) {

    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    config->dynoRpmStep = 100;

    config->dynoSaeTemperatureC = 20;
    config->dynoSaeBaro = STD_ATMOSPHERE;
    config->dynoSaeRelativeHumidity = 80;

    config->dynoCarWheelDiaInch = 18;
    config->dynoCarWheelTireWidthMm = 235;
    config->dynoCarWheelAspectRatio = 40;

    config->dynoCarGearPrimaryReduction = 1;
    config->dynoCarGearRatio = 1.160;
    config->dynoCarGearFinalDrive = 5.358;

    config->dynoCarCarMassKg = 1413;
    config->dynoCarCargoMassKg = 180;
    config->dynoCarCoeffOfDrag = 0.29;
    config->dynoCarFrontalAreaM2 = 1.8580608;

    Sensor::setMockValue(SensorType::Tps1, 0);
    engine->periodicSlowCallback();

    auto lastTime = testData[0].time - 0.01;
    for (size_t i = 0; i < testData.size(); ++i) {
        auto point = testData[i];

        auto timeDiff = int((point.time - lastTime) * 1000);
        lastTime = point.time;

        eth.moveTimeForwardMs(timeDiff);
        Sensor::setMockValue(SensorType::Rpm,  point.rpm);
        Sensor::setMockValue(SensorType::Tps1, point.tps);

        engine->periodicSlowCallback();

        auto currentHP = getDynoviewHP();
        auto currentTorque = getDynoviewTorque();
        // std::cerr << "rpm: " << point.rpm << ", hp: " << currentHP << ", Torque: " << currentTorque << std::endl;

        ASSERT_NEAR(point.hp, currentHP, 10);
        ASSERT_NEAR(point.torque, currentTorque, 10);
    }
}


