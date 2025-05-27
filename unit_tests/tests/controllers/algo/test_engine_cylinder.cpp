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