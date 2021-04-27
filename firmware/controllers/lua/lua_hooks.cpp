#include "lua.hpp"
#include "lua_hooks.h"
#include "loggingcentral.h"

static int lua_efi_print(lua_State* l) {
	luaL_checkstring(l, 1);

	auto msg = lua_tostring(l, 1);

	efiPrintf("LUA: %s", msg);

	return 0;
}

void configureRusefiLuaHooks(lua_State* l) {
	lua_register(l, "print", lua_efi_print);
}
