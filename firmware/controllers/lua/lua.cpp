#include "pch.h"

#include "rusefi_lua.h"
#include "thread_controller.h"

#if EFI_LUA

#include "lua.hpp"
#include "lua_hooks.h"

#define TAG "LUA "

#if EFI_PROD_CODE || EFI_SIMULATOR

#ifndef LUA_USER_HEAP
#define LUA_USER_HEAP 1
#endif // LUA_USER_HEAP

#ifndef LUA_SYSTEM_HEAP
#define LUA_SYSTEM_HEAP 1
#endif // LUA_SYSTEM_HEAP

static char luaUserHeap[LUA_USER_HEAP];
static char luaSystemHeap[LUA_SYSTEM_HEAP];

class Heap {
public:
	memory_heap_t m_heap;

	size_t m_memoryUsed = 0;
	const size_t m_size;

	void* alloc(size_t n) {
		return chHeapAlloc(&m_heap, n);
	}

	void free(void* obj) {
		chHeapFree(obj);
	}

public:
	template<size_t TSize>
	Heap(char (&buffer)[TSize])
		: m_size(TSize)
	{
		chHeapObjectInit(&m_heap, buffer, TSize);
	}

	void* realloc(void* ptr, size_t osize, size_t nsize) {
		if (nsize == 0) {
			// requested size is zero, free if necessary and return nullptr
			if (ptr) {
				free(ptr);
				m_memoryUsed -= osize;
			}

			return nullptr;
		}

		void *new_mem = alloc(nsize);
		m_memoryUsed += nsize;

		if (!ptr) {
			// No old pointer passed in, simply return allocated block
			return new_mem;
		}

		// An old pointer was passed in, copy the old data in, then free
		if (new_mem != nullptr) {
			memcpy(new_mem, ptr, chHeapGetSize(ptr) > nsize ? nsize : chHeapGetSize(ptr));
			free(ptr);
			m_memoryUsed -= osize;
		}

		return new_mem;
	}

	size_t size() const {
		return m_size;
	}

	size_t used() const {
		return m_memoryUsed;
	}
};

static Heap heaps[] = { luaUserHeap, luaSystemHeap };

template <int HeapIdx>
static void* myAlloc(void* /*ud*/, void* ptr, size_t osize, size_t nsize) {
	static_assert(HeapIdx < efi::size(heaps));

	if (CONFIG(debugMode) == DBG_LUA) {
		switch (HeapIdx) {
			case 0: tsOutputChannels.debugIntField1 = heaps[HeapIdx].used(); break;
			case 1: tsOutputChannels.debugIntField2 = heaps[HeapIdx].used(); break;
		}
	}

	return heaps[HeapIdx].realloc(ptr, osize, nsize);
}
#else // not EFI_PROD_CODE
// Non-MCU code can use plain realloc function instead of custom implementation
template <int /*ignored*/>
static void* myAlloc(void* /*ud*/, void* ptr, size_t /*osize*/, size_t nsize) {
	if (!nsize) {
		free(ptr);
		return nullptr;
	}

	if (!ptr) {
		return malloc(nsize);
	}

	return realloc(ptr, nsize);
}
#endif // EFI_PROD_CODE

static int luaTickPeriodMs;

static int lua_setTickRate(lua_State* l) {
	float freq = luaL_checknumber(l, 1);

	// Limit to 1..100 hz
	freq = clampF(1, freq, 100);

	luaTickPeriodMs = 1000.0f / freq;
	return 0;
}

static void loadLibraries(LuaHandle& ls) {
	constexpr luaL_Reg libs[] = {
		// TODO: do we even need the base lib?
		//{ LUA_GNAME, luaopen_base },
		{ LUA_MATHLIBNAME, luaopen_math },
	};

	for (size_t i = 0; i < efi::size(libs); i++) {
		luaL_requiref(ls, libs[i].name, libs[i].func, 1);
		lua_pop(ls, 1);
	}
}

static LuaHandle setupLuaState(lua_Alloc alloc) {
	LuaHandle ls = lua_newstate(alloc, NULL);

	if (!ls) {
		firmwareError(OBD_PCM_Processor_Fault, "Failed to start Lua interpreter");

		return nullptr;
	}

	// Load Lua's own libraries
	loadLibraries(ls);

	// Load rusEFI hooks
	lua_register(ls, "setTickRate", lua_setTickRate);
	configureRusefiLuaHooks(ls);

	// run a GC cycle
	lua_gc(ls, LUA_GCCOLLECT, 0);

	// set GC settings
	// see https://www.lua.org/manual/5.4/manual.html#2.5.1
	lua_gc(ls, LUA_GCINC, 50, 1000, 9);

	return ls;
}

static bool loadScript(LuaHandle& ls, const char* scriptStr) {
	efiPrintf(TAG "loading script length: %d...", efiStrlen(scriptStr));

	if (0 != luaL_dostring(ls, scriptStr)) {
		efiPrintf(TAG "ERROR loading script: %s", lua_tostring(ls, -1));
		lua_pop(ls, 1);
		return false;
	}

	efiPrintf(TAG "script loaded successfully!");

	return true;
}

static LuaHandle systemLua;

const char* getSystemLuaScript();

void initSystemLua() {
#if LUA_SYSTEM_HEAP > 1
	efiAssertVoid(OBD_PCM_Processor_Fault, !systemLua, "system lua already init");

	Timer startTimer;
	startTimer.reset();

	systemLua = setupLuaState(myAlloc<1>);

	efiAssertVoid(OBD_PCM_Processor_Fault, systemLua, "system lua init fail");

	if (!loadScript(systemLua, getSystemLuaScript())) {
		firmwareError(OBD_PCM_Processor_Fault, "system lua script load fail");
		systemLua = nullptr;
		return;
	}

	auto startTime = startTimer.getElapsedSeconds();

#if !EFI_UNIT_TEST
	efiPrintf("System Lua loaded in %.2f ms using %d bytes", startTime * 1'000, heaps[1].used());
#endif
#endif
}

#if !EFI_UNIT_TEST
static bool interactivePending = false;
static char interactiveCmd[100];

void doInteractive(LuaHandle& ls) {
	if (!interactivePending) {
		// no cmd pending, return
		return;
	}

	auto status = luaL_dostring(ls, interactiveCmd);

	if (0 == status) {
		// Function call was OK, resolve return value and print it
		if (lua_isinteger(ls, -1)) {
			efiPrintf(TAG "interactive returned integer: %d", lua_tointeger(ls, -1));
		} else if (lua_isnumber(ls, -1)) {
			efiPrintf(TAG "interactive returned number: %f", lua_tonumber(ls, -1));
		} else if (lua_isstring(ls, -1)) {
			efiPrintf(TAG "interactive returned string: '%s'", lua_tostring(ls, -1));
		} else if (lua_isboolean(ls, -1)) {
			efiPrintf(TAG "interactive returned bool: %s", lua_toboolean(ls, -1) ? "true" : "false");
		} else if (lua_isnil(ls, -1)) {
			efiPrintf(TAG "interactive returned nil.");
		} else {
			efiPrintf(TAG "interactive returned nothing.");
		}
	} else {
		// error with interactive command, print it
		efiPrintf(TAG "interactive error: %s", lua_tostring(ls, -1));
	}

	interactivePending = false;

	lua_settop(ls, 0);
}

void invokeTick(LuaHandle& ls) {
	ScopePerf perf(PE::LuaTickFunction);

	// run the tick function
	lua_getglobal(ls, "onTick");
	if (lua_isnil(ls, -1)) {
		// TODO: handle missing tick function
		lua_settop(ls, 0);
		return;
	}

	int status = lua_pcall(ls, 0, 0, 0);

	if (0 != status) {
		// error calling hook function
		auto errMsg = lua_tostring(ls, -1);
		efiPrintf(TAG "error %s", errMsg);
		lua_pop(ls, 1);
	}

	lua_settop(ls, 0);
}

struct LuaThread : ThreadController<4096> {
	LuaThread() : ThreadController("lua", PRIO_LUA) { }

	void ThreadTask() override;
};

static bool needsReset = false;

// Each invocation of runOneLua will:
// - create a new Lua instance
// - read the script from config
// - run the tick function until needsReset is set
// Returns true if it should be re-called immediately,
// or false if there was a problem setting up the interpreter
// or parsing the script.
static bool runOneLua(lua_Alloc alloc, const char* script) {
	needsReset = false;

	auto ls = setupLuaState(alloc);

	// couldn't start Lua interpreter, bail out
	if (!ls) {
		return false;
	}

	// Reset default tick rate
	luaTickPeriodMs = 100;

	if (!loadScript(ls, script)) {
		return false;
	}

	while (!needsReset && !chThdShouldTerminateX()) {
#if EFI_CAN_SUPPORT
		// First, process any pending can RX messages
		doLuaCanRx(ls);
#endif // EFI_CAN_SUPPORT

		// Next, check if there is a pending interactive command entered by the user
		doInteractive(ls);

		invokeTick(ls);

		chThdSleepMilliseconds(luaTickPeriodMs);
	}

#if EFI_CAN_SUPPORT
	resetLuaCanRx();
#endif // EFI_CAN_SUPPORT

	// De-init pins, they will reinit next start of the script.
	luaDeInitPins();

	return true;
}

void LuaThread::ThreadTask() {
	initSystemLua();

	while (!chThdShouldTerminateX()) {
		bool wasOk = runOneLua(myAlloc<0>, config->luaScript);

		// Reset any lua adjustments the script made
		ENGINE(engineState).luaAdjustments = {};

		if (!wasOk) {
			// Something went wrong executing the script, spin
			// until reset invoked (maybe the user fixed the script)
			while (!needsReset) {
				chThdSleepMilliseconds(100);
			}
		}
	}
}

#if LUA_USER_HEAP > 1
static LuaThread luaThread;
#endif

void startLua() {
#if LUA_USER_HEAP > 1
#if EFI_CAN_SUPPORT
	initLuaCanRx();
#endif // EFI_CAN_SUPPORT

	luaThread.Start();

	addConsoleActionS("lua", [](const char* str){
		if (interactivePending) {
			return;
		}

		strncpy(interactiveCmd, str, sizeof(interactiveCmd) - 1);
		interactiveCmd[sizeof(interactiveCmd) - 1] = '\0';

		interactivePending = true;
	});

	addConsoleAction("luareset", [](){
		needsReset = true;
	});

	addConsoleAction("luamemory", [](){
		for (size_t i = 0; i < efi::size(heaps); i++) {
			auto heapSize = heaps[i].size();
			auto memoryUsed = heaps[i].used();
			float pct = 100.0f * memoryUsed / heapSize;
			efiPrintf("Lua memory heap %d: %d / %d bytes = %.1f%%", i, memoryUsed, heapSize, pct);
		}
	});
#endif
}

#else // not EFI_UNIT_TEST

void startLua() {
	initSystemLua();
}

#include <stdexcept>
#include <string>

static LuaHandle runScript(const char* script) {
	auto ls = setupLuaState(myAlloc<0>);

	if (!ls) {
		throw std::logic_error("Call to setupLuaState failed, returned null");
	}

	if (!loadScript(ls, script)) {
		throw std::logic_error("Call to loadScript failed");
	}

	lua_getglobal(ls, "testFunc");
	if (lua_isnil(ls, -1)) {
		throw std::logic_error("Failed to find function testFunc");
	}

	int status = lua_pcall(ls, 0, 1, 0);

	if (0 != status) {
		std::string msg = std::string("lua error while running script: ") + lua_tostring(ls, -1);
		throw std::logic_error(msg);
	}

	return ls;
}

expected<float> testLuaReturnsNumberOrNil(const char* script) {
	auto ls = runScript(script);

	// check nil return first
	if (lua_isnil(ls, -1)) {
		return unexpected;
	}

	// If not nil, it should be a number
	if (!lua_isnumber(ls, -1)) {
		throw std::logic_error("Returned value is not a number");
	}

	// pop the return value
	return lua_tonumber(ls, -1);
}

float testLuaReturnsNumber(const char* script) {
	auto ls = runScript(script);

	// check the return value
	if (!lua_isnumber(ls, -1)) {
		throw new std::logic_error("Returned value is not a number");
	}

	// pop the return value
	return lua_tonumber(ls, -1);
}

int testLuaReturnsInteger(const char* script) {
	auto ls = runScript(script);

	// pop the return value;
	if (!lua_isinteger(ls, -1)) {
		throw std::logic_error("Returned value is not an integer");
	}

	return lua_tointeger(ls, -1);
}

void testLuaExecString(const char* script) {
	auto ls = setupLuaState(myAlloc<0>);

	if (!ls) {
		throw std::logic_error("Call to setupLuaState failed, returned null");
	}

	if (!loadScript(ls, script)) {
		throw std::logic_error("Call to loadScript failed");
	}
}

#endif // EFI_UNIT_TEST

#endif // EFI_LUA
