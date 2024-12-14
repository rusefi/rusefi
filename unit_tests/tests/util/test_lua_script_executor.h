//
// Created by kifir on 12/15/24.
//

#pragma once

class TestLuaScriptExecutor {
public:
	static TestLuaScriptExecutor& getInstance();

	void setTorqueReductionState(bool state);
private:
	static TestLuaScriptExecutor instance;
};
