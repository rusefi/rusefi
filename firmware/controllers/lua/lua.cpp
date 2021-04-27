#include "lua.hpp"
#include "lua_hooks.h"
#include "thread_controller.h"

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

lua_State* setupLuaState() {
	auto *ls = lua_newstate(myAlloc, NULL);

	// TODO handle null ls
	if (!ls) {
		firmwareError(OBD_PCM_Processor_Fault, "Failed to start Lua interpreter");

		return nullptr;
	}

	// load libraries
	luaopen_base(ls);
	luaopen_string(ls);
	luaopen_math(ls);

	// Load rusEFI hooks
	configureRusefiLuaHooks(ls);

	// run a GC cycle
	lua_gc(ls, LUA_GCCOLLECT, 0);

	// set GC settings
	// TODO: why these values
	lua_gc(ls, LUA_GCSETPAUSE, 99);
	lua_gc(ls, LUA_GCSETSTEPMUL, 1000);

	return ls;
}

void stopLua(lua_State* ls) {
	lua_close(ls);
}

void loadScript(lua_State* ls) {
	auto scriptStr = "n=0\nfunction onTick()\nefiprint('hello lua ' ..n)\nn=n+1\nend\n";

	efiPrintf("loading script length: %d", efiStrlen(scriptStr));

	if (0 != luaL_dostring(ls, scriptStr)) {
		// TODO: handle script load fail
		return;
	}

	efiPrintf("script loaded");
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

	loadScript(ls);

	while (!chThdShouldTerminateX()) {
		// run the tick function
		lua_getglobal(ls, "onTick");
		if (lua_isnil(ls, -1)) {
			// TODO: handle missing tick function
			lua_pop(ls, 1);
			lua_settop(ls, 0);
			continue;
		}

		int status = lua_pcall(ls, 0, 0, 0);

		if (0 != status) {
			// error calling function
			// TODO: handle
			// error msg = lua_tostring(ls, -1)
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

#endif // EFI_UNIT_TEST
