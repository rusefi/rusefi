//
// Created by kifir on 12/15/24.
//

#include "pch.h"

#include "test_lua_script_executor.h"

#include "rusefi_lua.h"

TestLuaScriptExecutor& TestLuaScriptExecutor::getInstance() {
	return instance;
}

void TestLuaScriptExecutor::setTorqueReductionState(bool state) {
	char luaScript[256];
	std::snprintf(luaScript, sizeof(luaScript), "setTorqueReductionState(%s);", state ? "true" : "false");
	EXPECT_NO_THROW(testLuaExecString(luaScript));
}

TestLuaScriptExecutor TestLuaScriptExecutor::instance;