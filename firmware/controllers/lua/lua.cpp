
#include "rusefi_lua.h"
#include "thread_controller.h"
#include "perf_trace.h"

#if EFI_LUA

#include "lua.hpp"
#include "lua_hooks.h"

#if EFI_PROD_CODE
#include "ch.h"

#define LUA_HEAP_SIZE 20000

static memory_heap_t heap;

static void* myAlloc(void* /*ud*/, void* ptr, size_t /*osize*/, size_t nsize) {
	if (nsize == 0) {
		// requested size is zero, free if necessary and return nullptr
		if (ptr) {
			chHeapFree(ptr);
		}

		return nullptr;
	}

	void *new_mem = chHeapAlloc(&heap, nsize);

	if (!ptr) {
		// No old pointer passed in, simply return allocated block
		return new_mem;
	}

	// An old pointer was passed in, copy the old data in, then free
	if (new_mem != nullptr) {
		memcpy(new_mem, ptr, chHeapGetSize(ptr) > nsize ? nsize : chHeapGetSize(ptr));
		chHeapFree(ptr);
	}

	return new_mem;
}
#else // not EFI_PROD_CODE
// Non-MCU code can use plain realloc function instead of custom implementation
static void* myAlloc(void* /*ud*/, void* ptr, size_t /*osize*/, size_t nsize) {
	return realloc(ptr, nsize);
}
#endif // EFI_PROD_CODE

static lua_State* setupLuaState() {
	auto *ls = lua_newstate(myAlloc, NULL);

	// TODO handle null ls
	if (!ls) {
		firmwareError(OBD_PCM_Processor_Fault, "Failed to start Lua interpreter");

		return nullptr;
	}

	// load libraries
	luaopen_base(ls);
	luaopen_math(ls);

	// Load rusEFI hooks
	configureRusefiLuaHooks(ls);

	// run a GC cycle
	lua_gc(ls, LUA_GCCOLLECT, 0);

	// set GC settings
	// see https://www.lua.org/manual/5.4/manual.html#2.5.1
	lua_gc(ls, LUA_GCINC, 50, 1000, 9);

	return ls;
}

void stopLua(lua_State* ls) {
	lua_close(ls);
}

static bool loadScript(lua_State* ls, const char* scriptStr) {
	efiPrintf("loading script length: %d", efiStrlen(scriptStr));

	if (0 != luaL_dostring(ls, scriptStr)) {
		efiPrintf("LUA error loading script: %s", lua_tostring(ls, -1));
		lua_pop(ls, 1);
		return false;
	}

	efiPrintf("script loaded");

	return true;
}

#if !EFI_UNIT_TEST
struct LuaThread : ThreadController<4096> {
	LuaThread() : ThreadController("lua", PRIO_LUA) { }

	void ThreadTask() override;
};

void LuaThread::ThreadTask() {
	void* buf = malloc(LUA_HEAP_SIZE);
	chHeapObjectInit(&heap, buf, LUA_HEAP_SIZE);

	auto ls = setupLuaState();

	// couldn't start Lua interpreter, bail out
	if (!ls) {
		return;
	}

	//auto scriptStr = "function onTick()\nlocal rpm = getSensor(3)\nif rpm ~= nil then\nprint('RPM: ' ..rpm)\nend\nend\n";
	auto scriptStr = "n=0\nfunction onTick()\nprint('hello lua ' ..n)\nn=n+1\nend\n";

	loadScript(ls, scriptStr);

	while (!chThdShouldTerminateX()) {
		// run the tick function
		lua_getglobal(ls, "onTick");
		if (lua_isnil(ls, -1)) {
			// TODO: handle missing tick function
			lua_pop(ls, 1);
			lua_settop(ls, 0);
			continue;
		}

		{
			ScopePerf perf(PE::LuaTickFunction);

			int status = lua_pcall(ls, 0, 0, 0);

			if (0 != status) {
				// error calling hook function
				auto errMsg = lua_tostring(ls, -1);
				efiPrintf("lua err %s", errMsg);
				lua_pop(ls, 1);
			}
		}

		lua_settop(ls, 0);
	}
}

static LuaThread luaThread;

void startLua() {
	luaThread.Start();
}

#else // not EFI_UNIT_TEST

void startLua() {
	// todo
}

#include <stdexcept>
#include <string>

static lua_State* runScript(const char* script) {
	auto ls = setupLuaState();

	if (!ls) {
		throw new std::logic_error("Call to setupLuaState failed, returned null");
	}

	if (!loadScript(ls, script)) {
		lua_close(ls);
		throw new std::logic_error("Call to loadScript failed");
	}

	lua_getglobal(ls, "testFunc");
	if (lua_isnil(ls, -1)) {
		lua_close(ls);
		throw new std::logic_error("Failed to find function testFunc");
	}

	int status = lua_pcall(ls, 0, 1, 0);

	if (0 != status) {
		std::string msg = std::string("lua error while running script: ") + lua_tostring(ls, -1);
		lua_close(ls);
		throw new std::logic_error(msg);
	}

	return ls;
}

expected<float> testLuaReturnsNumberOrNil(const char* script) {
	auto ls = runScript(script);

	// check nil return first
	if (lua_isnil(ls, -1)) {
		lua_close(ls);
		return unexpected;
	}

	// If not nil, it should be a number
	if (!lua_isnumber(ls, -1)) {
		lua_close(ls);
		throw new std::logic_error("Returned value is not a number");
	}

	// pop the return value
	float retVal = lua_tonumber(ls, -1);

	lua_close(ls);

	return retVal;
}

float testLuaReturnsNumber(const char* script) {
	auto ls = runScript(script);

	// check the return value
	if (!lua_isnumber(ls, -1)) {
		lua_close(ls);
		throw new std::logic_error("Returned value is not a number");
	}

	// pop the return value
	float retVal = lua_tonumber(ls, -1);

	lua_close(ls);

	return retVal;
}

int testLuaReturnsInteger(const char* script) {
	auto ls = runScript(script);

	// pop the return value;
	if (!lua_isinteger(ls, -1)) {
		lua_close(ls);
		throw new std::logic_error("Returned value is not an integer");
	}

	int retVal = lua_tointeger(ls, -1);

	lua_close(ls);

	return retVal;
}

#endif // EFI_UNIT_TEST

#endif // EFI_LUA
