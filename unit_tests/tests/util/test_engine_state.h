//
// Created by kifir on 11/4/24.
//

#pragma once

class TestEngineState {
public:
    static TestEngineState& getInstance();

    float getInjectionStage2Fraction() const;
    floatms_t getInjectionDuration() const;
    floatms_t getInjectionDurationStage2() const;
private:
    TestEngineState();
    static TestEngineState instance;
};
