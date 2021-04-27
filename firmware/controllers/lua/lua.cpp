#include "ch.h"
#include "lua.hpp"
#include "thread_controller.h"

#define LUA_HEAP_SIZE 20000

static memory_heap_t heap;

static void* myAlloc(void * /*ud*/, void *ptr, size_t /*osize*/, size_t nsize) {
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

struct LuaThread : ThreadController<4096> {
	LuaThread() : ThreadController("lua", PRIO_LUA) { }

	void ThreadTask() override;
};

static int lua_efi_print(lua_State* l) {
	luaL_checkstring(l, 1);

	auto msg = lua_tostring(l, 1);

	efiPrintf("LUA: %s", msg);

	return 0;
}

lua_State* setupLuaState() {
	auto *ls = lua_newstate(myAlloc, NULL);

	// TODO handle null ls
	if (!ls) {
		firmwareError(OBD_PCM_Processor_Fault, "Failed to start Lua interpreter");

		return nullptr;
	}

	lua_register(ls, "print", lua_efi_print);

	// load libraries
	luaopen_base(ls);
	luaopen_string(ls);
	luaopen_math(ls);

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
