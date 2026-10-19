#pragma once

#include "lua.hpp"

class LuaHookRegistry;
using LuaHookEnumerator = void (*)(LuaHookRegistry&);

class LuaHookRegistry {
public:
	// With only a state, add() registers each function immediately.
	// With a key, add() searches for that name without adding functions to Lua.
	explicit LuaHookRegistry(lua_State* state) : m_state(state) { }
	LuaHookRegistry(lua_State* state, const char* key, size_t length)
		: m_state(state), m_lookupKey(key), m_lookupLength(length), m_lookupMode(true) { }

	void add(const char* name, lua_CFunction function);
	lua_CFunction result() const { return m_result; }
	lua_State* state() const { return m_state; }

private:
	lua_State* m_state;
	const char* m_lookupKey = nullptr;
	size_t m_lookupLength = 0;
	lua_CFunction m_result = nullptr;
	bool m_lookupMode = false;
};

// Look up native Lua functions when scripts first use them. enumerate walks
// the same function list used for normal startup registration.
void configureLuaHookRegistry(lua_State* state, LuaHookEnumerator enumerate);
#if EFI_UNIT_TEST
void configureLuaHookRegistryForTests(lua_State* state, LuaHookEnumerator enumerate);
#endif
