//
// Created by kifir on 11/4/24.
//

#pragma once

class TestEngineState {
public:
    static TestEngineState& getInstance();

    void setLuaSoftSparkSkip(float value);
    void setLuaHardSparkSkip(float value);
private:
    TestEngineState();
    static TestEngineState instance;
};
