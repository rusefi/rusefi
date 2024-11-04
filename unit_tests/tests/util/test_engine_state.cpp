//
// Created by kifir on 11/4/24.
//

#include "pch.h"

#include "test_engine_state.h"

TestEngineState& TestEngineState::getInstance() {
    return instance;
}

void TestEngineState::setLuaSoftSparkSkip(const float value) {
    engine->engineState.luaSoftSparkSkip = value;
}

void TestEngineState::setLuaHardSparkSkip(const float value) {
    engine->engineState.luaHardSparkSkip = value;
}

TestEngineState::TestEngineState() {
}

TestEngineState TestEngineState::instance;