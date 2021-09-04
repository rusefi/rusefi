#include "pch.h"

#include "rusefi_lua.h"
#include "thread_controller.h"

#if EFI_LUA

#include "lua.hpp"
#include "lua_hooks.h"

#define TAG "LUA "

#if EFI_PROD_CODE || EFI_SIMULATOR
static char luaUserHeap[10000];
static char luaSystemHeap[10000];

class Heap {
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
	return realloc(ptr, nsize);
}
#endif // EFI_PROD_CODE

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

	// Destruction cleans up lua state
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

static int luaTickPeriodMs;

static int lua_setTickRate(lua_State* l) {
	float freq = luaL_checknumber(l, 1);

	// Limit to 1..100 hz
	freq = clampF(1, freq, 100);

	luaTickPeriodMs = 1000.0f / freq;
	return 0;
}

static LuaHandle setupLuaState(lua_Alloc alloc) {
	LuaHandle ls = lua_newstate(alloc, NULL);

	if (!ls) {
		firmwareError(OBD_PCM_Processor_Fault, "Failed to start Lua interpreter");

		return nullptr;
	}

	// load libraries
	luaopen_base(ls);
	luaopen_math(ls);

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
		// First, check if there is a pending interactive command entered by the user
		doInteractive(ls);

		invokeTick(ls);

		chThdSleepMilliseconds(luaTickPeriodMs);
	}

	// De-init pins, they will reinit next start of the script.
	luaDeInitPins();

	return true;
}

void LuaThread::ThreadTask() {
	while (!chThdShouldTerminateX()) {
		bool wasOk = runOneLua(myAlloc<0>, config->luaScript);

		if (!wasOk) {
			// Something went wrong executing the script, spin
			// until reset invoked (maybe the user fixed the script)
			while (!needsReset) {
				chThdSleepMilliseconds(100);
			}
		}
	}
}

static LuaThread luaThread;

static LuaHandle systemLua;

void initSystemLua() {
	efiAssertVoid(OBD_PCM_Processor_Fault, !systemLua, "system lua already init");

	Timer startTimer;
	startTimer.reset();

	systemLua = setupLuaState(myAlloc<1>);

	efiAssertVoid(OBD_PCM_Processor_Fault, systemLua, "system lua init fail");

	efiAssertVoid(OBD_PCM_Processor_Fault, loadScript(systemLua, "function x() end"), "system lua script load fail");

	auto startTime = startTimer.getElapsedSeconds();
	efiPrintf("System Lua loaded in %.2f ms using %d bytes", startTime * 1'000, heaps[1].used());
}

void startLua() {
	luaThread.Start();

	initSystemLua();

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
}

#else // not EFI_UNIT_TEST

void startLua() {
	// todo
}

#include <stdexcept>
#include <string>

static LuaHandle runScript(const char* script) {
	auto ls = setupLuaState(myAlloc<0>);

	if (!ls) {
		throw new std::logic_error("Call to setupLuaState failed, returned null");
	}

	if (!loadScript(ls, script)) {
		throw new std::logic_error("Call to loadScript failed");
	}

	lua_getglobal(ls, "testFunc");
	if (lua_isnil(ls, -1)) {
		throw new std::logic_error("Failed to find function testFunc");
	}

	int status = lua_pcall(ls, 0, 1, 0);

	if (0 != status) {
		std::string msg = std::string("lua error while running script: ") + lua_tostring(ls, -1);
		throw new std::logic_error(msg);
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
		throw new std::logic_error("Returned value is not a number");
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
		throw new std::logic_error("Returned value is not an integer");
	}

	return lua_tointeger(ls, -1);
}

void testLuaExecString(const char* script) {
	auto ls = setupLuaState(myAlloc<0>);

	if (!ls) {
		throw new std::logic_error("Call to setupLuaState failed, returned null");
	}

	if (!loadScript(ls, script)) {
		throw new std::logic_error("Call to loadScript failed");
	}
}

#endif // EFI_UNIT_TEST

#endif // EFI_LUA
