#include "pch.h"

#include "lua_hooks_registry.h"

#include <new>

namespace {
constexpr int BindingsUpvalue = 1;
constexpr int EnumeratorUpvalue = 2;
// Use this address to remember "name = nil". Storing nil would remove the cache
// entry and bring back the native function on the next lookup. false is a valid
// script value, so it cannot stand for nil.
const char nilBindingMarker = 0;

bool isLazyHookBuild() {
#if defined(LUA_LAZY_NATIVE_HOOKS) && LUA_LAZY_NATIVE_HOOKS
	return true;
#else
	return false;
#endif
}

LuaHookEnumerator enumeratorFromUpvalue(lua_State* state) {
	auto* enumerator = static_cast<LuaHookEnumerator*>(lua_touserdata(state, lua_upvalueindex(EnumeratorUpvalue)));
	return *enumerator;
}

lua_CFunction findHook(lua_State* state, LuaHookEnumerator enumerate, const char* name, size_t length) {
	LuaHookRegistry registry(state, name, length);
	enumerate(registry);
	return registry.result();
}

bool pushCachedBinding(lua_State* state, int keyIndex) {
	lua_pushvalue(state, keyIndex);
	lua_rawget(state, lua_upvalueindex(BindingsUpvalue));
	if (lua_isnil(state, -1)) {
		lua_pop(state, 1);
		return false;
	}
	if (lua_islightuserdata(state, -1) && lua_touserdata(state, -1) == &nilBindingMarker) {
		lua_pop(state, 1);
		lua_pushnil(state);
	}
	return true;
}

bool getStringKey(lua_State* state, int index, const char*& key, size_t& length) {
	if (lua_type(state, index) != LUA_TSTRING) {
		return false;
	}

	key = lua_tolstring(state, index, &length);
	return key != nullptr;
}

int lazyIndex(lua_State* state) {
	if (pushCachedBinding(state, 2)) {
		return 1;
	}

	const char* name = nullptr;
	size_t length = 0;
	if (!getStringKey(state, 2, name, length) || memchr(name, '\0', length)) {
		lua_pushnil(state);
		return 1;
	}

	lua_CFunction function = findHook(state, enumeratorFromUpvalue(state), name, length);
	if (!function) {
		lua_pushnil(state);
		return 1;
	}

	// [tag:lua_lazy_native_hooks] Store native functions and script-assigned values
	// in one private cache. A single write prevents allocation errors from leaving
	// a hook permanently hidden. Keep these names out of the globals table so every
	// assignment, including nil, goes through __newindex.
	lua_pushvalue(state, 2);
	lua_pushcfunction(state, function);
	lua_rawset(state, lua_upvalueindex(BindingsUpvalue));
	lua_pushcfunction(state, function);
	return 1;
}

int lazyNewIndex(lua_State* state) {
	const char* name = nullptr;
	size_t length = 0;
	if (getStringKey(state, 2, name, length) && !memchr(name, '\0', length) &&
		findHook(state, enumeratorFromUpvalue(state), name, length)) {
		lua_pushvalue(state, 2);
		if (lua_isnil(state, 3)) {
			lua_pushlightuserdata(state, const_cast<char*>(&nilBindingMarker));
		} else {
			lua_pushvalue(state, 3);
		}
		lua_rawset(state, lua_upvalueindex(BindingsUpvalue));
		return 0;
	}

	lua_pushvalue(state, 2);
	lua_pushvalue(state, 3);
	lua_rawset(state, 1);
	return 0;
}

void removeExistingNativeGlobals(lua_State* state, LuaHookEnumerator enumerate) {
	lua_pushglobaltable(state);
	int globals = lua_gettop(state);
	while (true) {
		lua_pushnil(state);
		bool removedOne = false;
		while (lua_next(state, globals) != 0) {
			const char* name = nullptr;
			size_t length = 0;
			if (getStringKey(state, -2, name, length) && !memchr(name, '\0', length) &&
				findHook(state, enumerate, name, length)) {
				// Remove this entry, then restart the table scan.
				lua_pushvalue(state, -2);
				lua_pushnil(state);
				lua_rawset(state, globals);
				lua_pop(state, 2); // value and now-deleted key
				removedOne = true;
				break;
			}
			lua_pop(state, 1);
		}
		if (!removedOne) {
			break;
		}
	}
	lua_pop(state, 1);
}

void configureLuaHookRegistryImpl(lua_State* state, LuaHookEnumerator enumerate) {
	removeExistingNativeGlobals(state, enumerate);

	// Native functions stay in the private cache, so the C API cannot list them
	// by scanning the globals table. ECU scripts have no _G, pairs, or rawget.
	lua_pushglobaltable(state);
	int globals = lua_gettop(state);
	lua_newtable(state);
	int metatable = lua_gettop(state);
	lua_newtable(state); // this interpreter's functions, replacements and nil markers
	int bindings = lua_gettop(state);
	auto* storedEnumerator = static_cast<LuaHookEnumerator*>(lua_newuserdatauv(state, sizeof(LuaHookEnumerator), 0));
	new (storedEnumerator) LuaHookEnumerator(enumerate);
	int enumerator = lua_gettop(state);

	lua_pushvalue(state, bindings);
	lua_pushvalue(state, enumerator);
	lua_pushcclosure(state, lazyIndex, 2);
	lua_setfield(state, metatable, "__index");
	lua_pushvalue(state, bindings);
	lua_pushvalue(state, enumerator);
	lua_pushcclosure(state, lazyNewIndex, 2);
	lua_setfield(state, metatable, "__newindex");
	lua_pop(state, 2); // both Lua callbacks keep references to these objects
	lua_setmetatable(state, globals);
	lua_pop(state, 1);
}
}

void LuaHookRegistry::add(const char* name, lua_CFunction function) {
	if (!m_lookupMode) {
		lua_pushcfunction(m_state, function);
		lua_setglobal(m_state, name);
		return;
	}

	if (std::strlen(name) == m_lookupLength && std::memcmp(name, m_lookupKey, m_lookupLength) == 0) {
		// As with normal registration, the last function added for this name wins.
		m_result = function;
	}
}

void configureLuaHookRegistry(lua_State* state, LuaHookEnumerator enumerate) {
	if (isLazyHookBuild()) {
		configureLuaHookRegistryImpl(state, enumerate);
	}
}

#if EFI_UNIT_TEST
void configureLuaHookRegistryForTests(lua_State* state, LuaHookEnumerator enumerate) {
	configureLuaHookRegistryImpl(state, enumerate);
}
#endif
