//
// Created by kifir on 12/15/24.
//

#pragma once

class TestLuaScriptExecutor {
public:
	static TestLuaScriptExecutor& getInstance();

	void setTorqueReductionState(bool state);
	void setSparkSkipRatio(float sparkSkipRatio);
	void setSparkHardSkipRatio(float sparkSkipRatio);
	void setFuelAdd(float fuelAdd);
private:
	static TestLuaScriptExecutor instance;
};
