//
// Created by kifir on 12/15/24.
//

#include "pch.h"

#include "test_lua_script_executor.h"

#include "rusefi_lua.h"

TestLuaScriptExecutor& TestLuaScriptExecutor::getInstance() {
	return instance;
}

void TestLuaScriptExecutor::setClutchDownState(const bool state) {
	char luaScript[256];
	std::snprintf(luaScript, sizeof(luaScript), "setClutchDownState(%s);", state ? "true" : "false");
	EXPECT_NO_THROW(testLuaExecString(luaScript));
}

void TestLuaScriptExecutor::setTorqueReductionState(const bool state) {
	char luaScript[256];
	std::snprintf(luaScript, sizeof(luaScript), "setTorqueReductionState(%s);", state ? "true" : "false");
	EXPECT_NO_THROW(testLuaExecString(luaScript));
}

void TestLuaScriptExecutor::setSparkSkipRatio(const float sparkSkipRatio) {
	char luaScript[256];
	std::snprintf(luaScript, sizeof(luaScript), "setSparkSkipRatio(%f);", sparkSkipRatio);
	EXPECT_NO_THROW(testLuaExecString(luaScript));
}

void TestLuaScriptExecutor::setSparkHardSkipRatio(const float sparkSkipRatio) {
	char luaScript[256];
	std::snprintf(luaScript, sizeof(luaScript), "setSparkHardSkipRatio(%f);", sparkSkipRatio);
	EXPECT_NO_THROW(testLuaExecString(luaScript));
}

void TestLuaScriptExecutor::setFuelAdd(const float fuelAdd) {
	char luaScript[256];
	std::snprintf(luaScript, sizeof(luaScript), "setFuelAdd(%f);", fuelAdd);
	EXPECT_NO_THROW(testLuaExecString(luaScript));
}

TestLuaScriptExecutor TestLuaScriptExecutor::instance;