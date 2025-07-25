//
// Created by kifir on 12/15/24.
//

#pragma once

class TestLuaScriptExecutor {
public:
	static TestLuaScriptExecutor& getInstance();

	void setClutchDownState(bool state);
	void setClutchUpState(bool state);
	void setTorqueReductionState(bool state);
	void setSparkSkipRatio(float sparkSkipRatio);
	void setSparkHardSkipRatio(float sparkSkipRatio);
	void setFuelAdd(float fuelAdd);
private:
	template<typename... Args> void executeFormattedLuaScript(const char* luaScriptFormatString, Args... args);
	void executeLuaScript(const char* luaScript);

	static TestLuaScriptExecutor instance;
};

template<typename... Args>
void TestLuaScriptExecutor::executeFormattedLuaScript(const char* const luaScriptFormatString, Args... args) {
	char luaScript[256];
	const size_t luaScriptLength = std::snprintf(luaScript, sizeof(luaScript), luaScriptFormatString, args...);
	ASSERT_TRUE(0 <= luaScriptLength) << "Encoding error" << std::endl << luaScriptFormatString;
	ASSERT_TRUE(luaScriptLength < sizeof(luaScript)) << "Insufficient buffer" << std::endl <<  luaScriptFormatString;
	executeLuaScript(luaScript);
}
