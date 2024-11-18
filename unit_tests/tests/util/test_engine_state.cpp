//
// Created by kifir on 11/4/24.
//

#include "pch.h"

#include "test_engine_state.h"

TestEngineState& TestEngineState::getInstance() {
    return instance;
}

float TestEngineState::getInjectionStage2Fraction() const {
    return engine->engineState.injectionStage2Fraction;
}

floatms_t TestEngineState::getInjectionDuration() const {
    return engine->engineState.injectionDuration;
}

floatms_t TestEngineState::getInjectionDurationStage2() const {
    return engine->engineState.injectionDurationStage2;
}

void TestEngineState::setLuaSoftSparkSkip(const float value) {
    engine->engineState.luaSoftSparkSkip = value;
}

void TestEngineState::setLuaHardSparkSkip(const float value) {
    engine->engineState.luaHardSparkSkip = value;
}

void TestEngineState::setLuaFuelAdd(const float value) {
    engine->engineState.lua.fuelAdd = value;
}

TestEngineState::TestEngineState() {
}

TestEngineState TestEngineState::instance;