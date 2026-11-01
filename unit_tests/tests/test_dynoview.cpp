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
        {1199.91f, 2237, 98, 0, 0},
        {1200.33f, 2358, 98, 132.507f, 43.8788f},
        {1200.71f, 2468, 98, 132.507f, 43.8788f},
        {1201.06f, 2575, 98, 101.319f, 34.0879f},
        {1201.42f, 2677, 98, 101.319f, 34.0879f},
        {1201.77f, 2803, 98, 91.7098f, 31.4668f},
        {1202.06f, 2900, 98, 91.7098f, 31.4668f},
        {1202.39f, 3019, 98, 89.463f, 31.3981f},
        {1202.66f, 3133, 98, 89.463f, 31.3981f},
        {1202.92f, 3237, 98, 91.1714f, 32.8149f},
        {1203.18f, 3349, 98, 91.1714f, 32.8149f},
        {1203.42f, 3468, 98, 101.615f, 38.0108f},
        {1203.7f, 3586, 98, 113.536f, 43.9937f},
        {1203.89f, 3689, 98, 133.16f, 54.975f},
        {1204.12f, 3784, 98, 154.744f, 66.1321f},
        {1204.34f, 3872, 98, 176.571f, 77.7658f},
        {1204.55f, 4004, 98, 199.558f, 90.4851f},
        {1204.79f, 4107, 98, 215.496f, 100.216f},
        {1204.99f, 4194, 98, 229.02f, 109.666f},
        {1205.21f, 4317, 98, 235.284f, 115.874f},
        {1205.41f, 4414, 98, 231.857f, 118.075f},
        {1205.62f, 4527, 98, 233.976f, 122.622f},
        {1205.83f, 4637, 98, 235.271f, 126.771f},
        {1206.03f, 4753, 98, 235.78f, 130.722f},
        {1206.23f, 4858, 98, 242.037f, 137.697f},
        {1206.4f, 4963, 98, 249.119f, 145.879f},
        {1206.6f, 5064, 98, 252.107f, 151.227f},
        {1206.78f, 5150, 98, 255.635f, 157.219f},
        {1206.97f, 5265, 98, 259.892f, 163.614f},
        {1207.17f, 5395, 98, 262.793f, 169.127f},
        {1207.35f, 5479, 98, 267.64f, 176.252f},
        {1207.54f, 5593, 98, 269.124f, 181.115f},
        {1207.75f, 5702, 98, 261.354f, 179.872f},
        {1207.96f, 5839, 98, 260.866f, 183.25f},
        {1208.16f, 5945, 98, 259.311f, 186.145f},
        {1208.36f, 6039, 98, 258.369f, 189.427f},
        {1208.56f, 6145, 98, 258.941f, 193.729f},
        {1208.75f, 6243, 98, 258.176f, 197.368f},
        {1208.96f, 6357, 98, 256.443f, 199.949f},
        {1209.15f, 6451, 98, 260, 206.637f},
        {1209.35f, 6582, 98, 262.935f, 212.898f},
        {1209.53f, 6681, 98, 267.457f, 220.788f},

        // not tun
        {1209.48f, 6624, 29, 0, 0},
        {1209.68f, 5624, 10, 0, 0},
        {1209.88f, 4624, 18, 0, 0},

        // run 2
        {1222.98f, 2807, 98, 0, 0},
        {1223.4f, 2927, 98, 132.644f, 54.5234f},
        {1223.76f, 3014, 98, 132.644f, 54.5234f},
        {1224.11f, 3140, 98, 100.35f, 41.7268f},
        {1224.41f, 3241, 98, 100.35f, 41.7268f},
        {1224.7f, 3363, 98, 96.5131f, 40.8638f},
        {1224.95f, 3477, 98, 96.5131f, 40.8638f},
        {1225.19f, 3574, 98, 99.2021f, 42.8801f},
        {1225.42f, 3685, 98, 99.2021f, 42.8801f},
        {1225.64f, 3789, 98, 102.104f, 45.0223f},
        {1225.86f, 3894, 98, 102.104f, 45.0223f},
        {1226.07f, 3984, 98, 114.109f, 51.8489f},
        {1226.28f, 4077, 98, 131.139f, 61.396f},
        {1226.48f, 4217, 98, 148.014f, 72.3723f},
        {1226.71f, 4289, 98, 168.489f, 84.4988f},
        {1226.93f, 4399, 98, 187.246f, 96.2993f},
        {1227.17f, 4535, 98, 201.557f, 106.14f},
        {1227.36f, 4630, 98, 222.198f, 119.975f},
        {1227.56f, 4721, 98, 231.989f, 128.398f},
        {1227.76f, 4844, 98, 234.202f, 133.203f},
        {1227.98f, 4974, 98, 231.457f, 135.188f},
        {1228.17f, 5085, 98, 239.914f, 143.776f},
        {1228.36f, 5162, 98, 246.712f, 151.547f},
        {1228.54f, 5294, 98, 257.228f, 161.743f},
        {1228.75f, 5354, 98, 254.613f, 163.828f},
        {1229.01f, 5531, 98, 249.888f, 164.202f},
        {1229.18f, 5605, 98, 256.615f, 172.838f},
        {1229.35f, 5678, 98, 264.986f, 182.496f},
        {1229.53f, 5790, 98, 266.519f, 187.736f},
        {1229.74f, 5893, 98, 262.636f, 188.805f},
        {1229.96f, 5996, 98, 254.576f, 186.738f},
        {1230.18f, 6112, 98, 252.638f, 188.773f},
        {1230.37f, 6219, 98, 261.059f, 198.656f},
        {1230.56f, 6304, 98, 255.356f, 198.295f},
        {1230.75f, 6424, 98, 252.785f, 200.4f},
        {1230.97f, 6507, 98, 252.785f, 200.4f},
        {1231.19f, 6636, 98, 244.623f, 197.995f},
        {1231.4f, 6737, 98, 245.47f, 202.721f},
        {1231.64f, 6828, 98, 244.215f, 205.326f},
        {1231.88f, 7011, 98, 244.972f, 209.81f},
        {1232.12f, 7252, 98, 243.574f, 213.126f},
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
    config->dynoCarGearRatio = 1.160f;
    config->dynoCarGearFinalDrive = 5.358f;

    config->dynoCarCarMassKg = 1413;
    config->dynoCarCargoMassKg = 180;
    config->dynoCarCoeffOfDrag = 0.29f;
    config->dynoCarFrontalAreaM2 = 1.8580608f;

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
    config->dynoCarGearRatio = 1.160f;
    config->dynoCarGearFinalDrive = 5.358f;

    config->dynoCarCarMassKg = 1413;
    config->dynoCarCargoMassKg = 180;
    config->dynoCarCoeffOfDrag = 0.29f;
    config->dynoCarFrontalAreaM2 = 1.8580608f;

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


