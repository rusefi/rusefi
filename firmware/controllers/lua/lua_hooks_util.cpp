/*
 * lua_hooks_util.cpp
 *
 *  Created on: Nov 2, 2021
 *      Author: rusefi
 */

#include "pch.h"
#include "lua_hooks_util.h"

static int lua_efi_print(lua_State* l) {
	auto msg = luaL_checkstring(l, 1);

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


void configureRusefiLuaUtilHooks(lua_State* l) {
	lua_register(l, "print", lua_efi_print);
	lua_register(l, "interpolate", lua_interpolate);
/*
 * todo: shall we? same for milliseconds?
	lua_register(l, "getNowSeconds", [](lua_State* l) -> int {
		int result = getTimeNowSeconds();
		lua_pushnumber(l, result);
		return 1;
	});
*/
}


