/*
 * @file test_engine_cylinder.cpp
 *
 * @date: may 26, 2025
 * @author FDSoftware
 */

#include "pch.h"

TEST(EngineCylinders, updateCylinders){
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    // this will call EngineCylinders::updateCylinders
    commonInitEngineController();
    EXPECT_TRUE(engine->cylinders[0].getIsValid());
    EXPECT_TRUE(engine->cylinders[1].getIsValid());
    EXPECT_FALSE(engine->cylinders[5].getIsValid());

}

TEST(EngineCylinders, getAngleOffset){
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    EngineCylinders::updateCylinders();

    // valid with firing order: 1-3-4-2
    EXPECT_EQ(0, engine->cylinders[0].getAngleOffset());
    EXPECT_EQ(540, engine->cylinders[1].getAngleOffset());
    EXPECT_EQ(180, engine->cylinders[2].getAngleOffset());
    EXPECT_EQ(360, engine->cylinders[3].getAngleOffset());

    // invalid cylinder for current configuration (4cyl)
    EXPECT_EQ(0, engine->cylinders[5].getAngleOffset());
}