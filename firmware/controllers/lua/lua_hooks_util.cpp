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

void configureRusefiLuaUtilHooks(lua_State* l) {
	lua_register(l, "print", lua_efi_print);
}


