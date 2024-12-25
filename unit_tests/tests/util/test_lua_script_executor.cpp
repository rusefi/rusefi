//
// Created by kifir on 12/15/24.
//

#include "pch.h"

#include "test_lua_script_executor.h"

#include "rusefi_lua.h"

TestLuaScriptExecutor& TestLuaScriptExecutor::getInstance() {
	return instance;
}

namespace {
	constexpr const char* const LUA_TRUE = "true";
	constexpr const char* const LUA_FALSE = "false";

	const char* toLuaBoolean(const bool value) {
		return (value ? LUA_TRUE : LUA_FALSE);
	}
}

void TestLuaScriptExecutor::setClutchDownState(const bool state) {
	executeFormattedLuaScript("setClutchDownState(%s);", toLuaBoolean(state));
}

void TestLuaScriptExecutor::setClutchUpState(const bool state) {
	executeFormattedLuaScript("setClutchUpState(%s);", toLuaBoolean(state));
}

void TestLuaScriptExecutor::setTorqueReductionState(const bool state) {
	executeFormattedLuaScript("setTorqueReductionState(%s);", toLuaBoolean(state));
}

void TestLuaScriptExecutor::setSparkSkipRatio(const float sparkSkipRatio) {
	executeFormattedLuaScript("setSparkSkipRatio(%f);", sparkSkipRatio);
}

void TestLuaScriptExecutor::setSparkHardSkipRatio(const float sparkSkipRatio) {
	executeFormattedLuaScript("setSparkHardSkipRatio(%f);", sparkSkipRatio);
}

void TestLuaScriptExecutor::setFuelAdd(const float fuelAdd) {
	executeFormattedLuaScript("setFuelAdd(%f);", fuelAdd);
}

void TestLuaScriptExecutor::executeLuaScript(const char* luaScript) {
	EXPECT_NO_THROW(testLuaExecString(luaScript));
}

TestLuaScriptExecutor TestLuaScriptExecutor::instance;