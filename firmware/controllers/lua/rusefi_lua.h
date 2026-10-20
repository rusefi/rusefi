// This file is not named lua.h as it would collide with firmware/ext/lua/src/lua.h

#pragma once

#include "lua.hpp"

class LuaHandle final {
public:
	LuaHandle() : LuaHandle(nullptr) { }
	LuaHandle(lua_State* ptr) : m_ptr(ptr) { }

	// Don't allow copying!
	LuaHandle(const LuaHandle&) = delete;
	LuaHandle& operator=(const LuaHandle&) = delete;

	// Allow moving!
	LuaHandle(LuaHandle&& rhs) {
		m_ptr = rhs.m_ptr;
		rhs.m_ptr = nullptr;
	}

	// Move assignment operator
	LuaHandle& operator=(LuaHandle&& rhs) {
		m_ptr = rhs.m_ptr;
		rhs.m_ptr = nullptr;

		return *this;
	}

	// Keep this owner outside luaProtectedCall's callback: after a caught setup
	// error, normal C++ scope exit must close the partially initialized VM and
	// run its registered userdata finalizers. longjmp does not unwind C++ owners.
	~LuaHandle() {
		if (m_ptr) {
			efiPrintf("LUA: Tearing down instance...");
			lua_close(m_ptr);
		}
	}

	operator lua_State*() const { return m_ptr; }

private:
	lua_State* m_ptr;
};

void startLua();

// [tag:lua_protected_call] Run one synchronous, non-yielding operation under lua_pcall.
// Protect global lookup and argument construction as well as the script call: all
// can allocate or raise Lua errors before an inner lua_pcall would be reached.
// callback receives the borrowed context as light userdata at stack index 1;
// neither context nor state changes ownership. The caller must have two spare
// Lua stack slots for the allocation-free function/context pushes.
// Returns false after reporting C6730 on any Lua error, true on success. Callback
// results and error objects are discarded, preserving the caller's stack entries.
// This does not roll back script side effects or automatically reset the VM.
// Lua errors use longjmp: keep objects requiring C++ destruction (LuaHandle,
// ScopePerf, locks, etc.) outside callback and any calls it can jump out of.
bool luaProtectedCall(lua_State* state, lua_CFunction callback, void* context, const char* description);

#if EFI_UNIT_TEST
#include <rusefi/expected.h>

// Exercise production setup/loading with deterministic allocator failures. The
// allocator's userdata must outlive the returned handle, including lua_close.
LuaHandle setupLuaStateForTests(lua_Alloc alloc, void* userdata);
bool loadLuaScriptForTests(LuaHandle& ls, const char* script);
expected<float> testLuaReturnsNumberOrNil(const char* script);
float testLuaReturnsNumber(const char* script);
int testLuaReturnsInteger(const char* script);
void testLuaExecString(const char* script);
#endif

#if EFI_CAN_SUPPORT && EFI_LUA

#include "can.h"

// Lua CAN rx feature
void initLuaCanRx();

// Called from the Lua loop to process any pending CAN frames
int doLuaCanRx(LuaHandle& ls);
// Called from the CAN RX thread to queue a frame for Lua consumption
void processLuaCan(const size_t busIndex, const CANRxFrame& frame);
size_t getLuaCanRxDropped();
#endif // EFI_CAN_SUPPORT && EFI_LUA
