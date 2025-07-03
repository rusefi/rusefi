/*
 * @file test_example_module.cpp
 *
 * @date: jul 07, 2025
 * @author FDSoftware
 */
#include "pch.h"

TEST(ExampleModule, liveDataExampleField){
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    engine->module<ExampleModule>()->onSlowCallback();
    EXPECT_EQ(engine->module<ExampleModule>()->liveDataExampleField, 8);
};