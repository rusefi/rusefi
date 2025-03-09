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
        {1199.91, 2237, 98, 0, 0},
        {1200.33, 2358, 98, 132.507, 43.8788},
        {1200.71, 2468, 98, 132.507, 43.8788},
        {1201.06, 2575, 98, 101.319, 34.0879},
        {1201.42, 2677, 98, 101.319, 34.0879},
        {1201.77, 2803, 98, 91.7098, 31.4668},
        {1202.06, 2900, 98, 91.7098, 31.4668},
        {1202.39, 3019, 98, 89.463, 31.3981},
        {1202.66, 3133, 98, 89.463, 31.3981},
        {1202.92, 3237, 98, 91.1714, 32.8149},
        {1203.18, 3349, 98, 91.1714, 32.8149},
        {1203.42, 3468, 98, 101.615, 38.0108},
        {1203.7, 3586, 98, 113.536, 43.9937},
        {1203.89, 3689, 98, 133.16, 54.975},
        {1204.12, 3784, 98, 154.744, 66.1321},
        {1204.34, 3872, 98, 176.571, 77.7658},
        {1204.55, 4004, 98, 199.558, 90.4851},
        {1204.79, 4107, 98, 215.496, 100.216},
        {1204.99, 4194, 98, 229.02, 109.666},
        {1205.21, 4317, 98, 235.284, 115.874},
        {1205.41, 4414, 98, 231.857, 118.075},
        {1205.62, 4527, 98, 233.976, 122.622},
        {1205.83, 4637, 98, 235.271, 126.771},
        {1206.03, 4753, 98, 235.78, 130.722},
        {1206.23, 4858, 98, 242.037, 137.697},
        {1206.4, 4963, 98, 249.119, 145.879},
        {1206.6, 5064, 98, 252.107, 151.227},
        {1206.78, 5150, 98, 255.635, 157.219},
        {1206.97, 5265, 98, 259.892, 163.614},
        {1207.17, 5395, 98, 262.793, 169.127},
        {1207.35, 5479, 98, 267.64, 176.252},
        {1207.54, 5593, 98, 269.124, 181.115},
        {1207.75, 5702, 98, 261.354, 179.872},
        {1207.96, 5839, 98, 260.866, 183.25},
        {1208.16, 5945, 98, 259.311, 186.145},
        {1208.36, 6039, 98, 258.369, 189.427},
        {1208.56, 6145, 98, 258.941, 193.729},
        {1208.75, 6243, 98, 258.176, 197.368},
        {1208.96, 6357, 98, 256.443, 199.949},
        {1209.15, 6451, 98, 260, 206.637},
        {1209.35, 6582, 98, 262.935, 212.898},
        {1209.53, 6681, 98, 267.457, 220.788},

        // not tun
        {1209.48, 6624, 29, 0, 0},
        {1209.68, 5624, 10, 0, 0},
        {1209.88, 4624, 18, 0, 0},

        // run 2
        {1222.98, 2807, 98, 0, 0},
        {1223.4, 2927, 98, 132.644, 54.5234},
        {1223.76, 3014, 98, 132.644, 54.5234},
        {1224.11, 3140, 98, 100.35, 41.7268},
        {1224.41, 3241, 98, 100.35, 41.7268},
        {1224.7, 3363, 98, 96.5131, 40.8638},
        {1224.95, 3477, 98, 96.5131, 40.8638},
        {1225.19, 3574, 98, 99.2021, 42.8801},
        {1225.42, 3685, 98, 99.2021, 42.8801},
        {1225.64, 3789, 98, 102.104, 45.0223},
        {1225.86, 3894, 98, 102.104, 45.0223},
        {1226.07, 3984, 98, 114.109, 51.8489},
        {1226.28, 4077, 98, 131.139, 61.396},
        {1226.48, 4217, 98, 148.014, 72.3723},
        {1226.71, 4289, 98, 168.489, 84.4988},
        {1226.93, 4399, 98, 187.246, 96.2993},
        {1227.17, 4535, 98, 201.557, 106.14},
        {1227.36, 4630, 98, 222.198, 119.975},
        {1227.56, 4721, 98, 231.989, 128.398},
        {1227.76, 4844, 98, 234.202, 133.203},
        {1227.98, 4974, 98, 231.457, 135.188},
        {1228.17, 5085, 98, 239.914, 143.776},
        {1228.36, 5162, 98, 246.712, 151.547},
        {1228.54, 5294, 98, 257.228, 161.743},
        {1228.75, 5354, 98, 254.613, 163.828},
        {1229.01, 5531, 98, 249.888, 164.202},
        {1229.18, 5605, 98, 256.615, 172.838},
        {1229.35, 5678, 98, 264.986, 182.496},
        {1229.53, 5790, 98, 266.519, 187.736},
        {1229.74, 5893, 98, 262.636, 188.805},
        {1229.96, 5996, 98, 254.576, 186.738},
        {1230.18, 6112, 98, 252.638, 188.773},
        {1230.37, 6219, 98, 261.059, 198.656},
        {1230.56, 6304, 98, 255.356, 198.295},
        {1230.75, 6424, 98, 252.785, 200.4},
        {1230.97, 6507, 98, 252.785, 200.4},
        {1231.19, 6636, 98, 244.623, 197.995},
        {1231.4, 6737, 98, 245.47, 202.721},
        {1231.64, 6828, 98, 244.215, 205.326},
        {1231.88, 7011, 98, 244.972, 209.81},
        {1232.12, 7252, 98, 243.574, 213.126},
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

        //std::cerr << "time:" << point.time << ", rpm: " << point.rpm << ", hp: " << dut.currentHP << ", Torque: " << dut.currentTorque << std::endl;
        //std::cerr << "{" << point.time << ", " << point.rpm << ", " << point.tps << ", " << dut.currentTorque << ", " << dut.currentHP << "}," << std::endl;

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


