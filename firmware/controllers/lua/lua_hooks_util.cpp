/*
 * lua_hooks_util.cpp
 *
 *  Created on: Nov 2, 2021
 *      Author: rusefi
 */

#include "pch.h"
#include "lua_hooks_util.h"
#include "script_impl.h"

static int lua_efi_print(lua_State* l) {
	auto msg = luaL_checkstring(l, 1);

	// we have somewhat similar debug code at serial_can.cpp
#if EFI_UNIT_TEST
	printf("[LUA] %s\n", msg);
#endif

	efiPrintf("LUA: %s", msg);

	return 0;
}

static int lua_interpolate(lua_State* l) {
	auto x1 = luaL_checknumber(l, 1);
	auto y1 = luaL_checknumber(l, 2);
	auto x2 = luaL_checknumber(l, 3);
	auto y2 = luaL_checknumber(l, 4);
	auto x = luaL_checknumber(l, 5);

	auto result = interpolateMsg("lua", x1, y1, x2, y2, x);

	lua_pushnumber(l, result);
	return 1;
}


void configureRusefiLuaUtilHooks(lua_State* lState) {
	lua_register(lState, "print", lua_efi_print);
	lua_register(lState, "interpolate", lua_interpolate);

	lua_register(lState, "findCurveIndex", [](lua_State* l) {
		auto name = luaL_checklstring(l, 1, nullptr);
		auto result = getCurveIndexByName(name);
		if (!result) {
			lua_pushnil(l);
		} else {
			// TS counts curve from 1 so convert indexing here
			lua_pushnumber(l, result.Value + HUMAN_OFFSET);
		}
		return 1;
	});

	lua_register(lState, "findTableIndex",
			[](lua_State* l) {
			auto name = luaL_checklstring(l, 1, nullptr);
			auto index = getTableIndexByName(name);
			if (!index) {
				lua_pushnil(l);
			} else {
				// TS counts curve from 1 so convert indexing here
				lua_pushnumber(l, index.Value + HUMAN_OFFSET);
			}
			return 1;
	});

	lua_register(lState, "findSetting",
			[](lua_State* l) {
			auto name = luaL_checklstring(l, 1, nullptr);
			auto defaultValue = luaL_checknumber(l, 2);

			auto index = getSettingIndexByName(name);
			if (!index) {
				lua_pushnumber(l, defaultValue);
			} else {
				// TS counts curve from 1 so convert indexing here
				lua_pushnumber(l, engineConfiguration->scriptSetting[index.Value]);
			}
			return 1;
	});

#ifndef STARTUP_STANDBY_PROHIBITED_PERIOD_SEC
#define STARTUP_STANDBY_PROHIBITED_PERIOD_SEC 3
#endif

#if defined(STM32F4) || defined(STM32F7)
	lua_register(lState, "mcu_standby", [](lua_State*) {
	  if (getTimeNowS() < STARTUP_STANDBY_PROHIBITED_PERIOD_SEC) {
	    criticalError("mcu_standby invoked right on start");
	    return 0;
	  }
	    onBoardStandBy();
		stm32_standby();
		return 0;
	});
#endif

/*
 * todo: shall we? same for milliseconds?
	lua_register(l, "getNowSeconds", [](lua_State* l) -> int {
		int result = getTimeNowS();
		lua_pushnumber(l, result);
		return 1;
	});
*/
}

void * hackEngineConfigurationPointer(void *ptr) {
	// we know that 'engineConfiguration' was null at the time of initialization in unit tests
#if EFI_UNIT_TEST
			intptr_t offset = (intptr_t)ptr;
			void * valuePtr = (void *)((char *)engineConfiguration + offset);
			return valuePtr;
#else
			return ptr;
#endif
}
