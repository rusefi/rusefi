//
// Created by kifir on 6/18/24.
//

#include "pch.h"

#include "test_base.h"

void TestBase::SetUp() {
    eth = std::make_unique<EngineTestHelper>(engine_type_e::TEST_ENGINE);
}

void TestBase::TearDown() {
    eth.reset();
}

void TestBase::periodicFastCallback() {
    // run the ignition math
    engine->periodicFastCallback();
}