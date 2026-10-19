/*
 * lua_hooks_util.h
 *
 *  Created on: Nov 2, 2021
 *      Author: rusefi
 */

#pragma once

#include "rusefi_lua.h"
#include "lua_hooks_registry.h"

void configureRusefiLuaUtilHooks(lua_State* l);
void enumerateRusefiLuaUtilHooks(LuaHookRegistry& hooks);
