#include "pch.h"

#include "rusefi_lua.h"
#include "thread_controller.h"

#if EFI_LUA

#include "lua.hpp"
#include "lua_hooks.h"
#include "can_filter.h"

#define TAG "LUA "

#if EFI_PROD_CODE || EFI_SIMULATOR

#ifndef LUA_USER_HEAP
#define LUA_USER_HEAP 1
#endif // LUA_USER_HEAP

static char luaUserHeap[LUA_USER_HEAP]
#ifdef EFI_HAS_EXT_SDRAM
SDRAM_OPTIONAL
#endif
;

class Heap {
public:
	memory_heap_t m_heap;

	size_t m_memoryUsed = 0;
	size_t m_size;

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

	void reinit(char *buffer, size_t m_size) {
		efiAssertVoid(OBD_PCM_Processor_Fault, m_memoryUsed == 0, "Too late to reinit Lua heap");
		chHeapObjectInit(&m_heap, buffer, m_size);
		this->m_size = m_size;
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

static Heap userHeap(luaUserHeap);

static void* myAlloc(void* /*ud*/, void* ptr, size_t osize, size_t nsize) {
	if (engineConfiguration->debugMode == DBG_LUA) {
		engine->outputChannels.debugIntField1 = userHeap.used();
	}

	return userHeap.realloc(ptr, osize, nsize);
}
#else // not EFI_PROD_CODE
// Non-MCU code can use plain realloc function instead of custom implementation
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

static int luaTickPeriodUs;

static int lua_setTickRate(lua_State* l) {
	float freq = luaL_checknumber(l, 1);

	// For instance BMW does 100 CAN messages per second on some IDs, let's allow at least twice that speed
	// Limit to 1..200 hz
	freq = clampF(1, freq, 200);

	luaTickPeriodUs = 1000000.0f / freq;
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

	lua_atpanic(ls, [](lua_State* l) {
		firmwareError(OBD_PCM_Processor_Fault, "Lua panic: %s", lua_tostring(l, -1));

		// hang the lua thread
		while (true) ;

		return 0;
	});

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

static void resetLua() {
	engine->module<AcController>().unmock().isDisabledByLua = false;
#if EFI_CAN_SUPPORT
	resetLuaCanRx();
#endif // EFI_CAN_SUPPORT

	// De-init pins, they will reinit next start of the script.
	luaDeInitPins();
}


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
	luaTickPeriodUs = MS2US(100);

	if (!loadScript(ls, script)) {
		return false;
	}

	while (!needsReset && !chThdShouldTerminateX()) {
		efitick_t beforeNt = getTimeNowNt();
#if EFI_CAN_SUPPORT
		// First, process any pending can RX messages
		doLuaCanRx(ls);
#endif // EFI_CAN_SUPPORT

		// Next, check if there is a pending interactive command entered by the user
		doInteractive(ls);

		invokeTick(ls);

		chThdSleep(TIME_US2I(luaTickPeriodUs));
		engine->outputChannels.luaLastCycleDuration = (getTimeNowNt() - beforeNt);
		engine->outputChannels.luaInvocationCounter++;
	}

	resetLua();

	return true;
}

void LuaThread::ThreadTask() {
	while (!chThdShouldTerminateX()) {
		bool wasOk = runOneLua(myAlloc, config->luaScript);

		// Reset any lua adjustments the script made
		engine->resetLua();

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
#if defined(STM32F4) && !defined(EFI_IS_F42x)
	// we need this on microRusEFI for sure
	// definitely should NOT have this on Proteus
	// on Hellen a bit of open question what's the best track
	// cute hack: let's check at runtime if you are a lucky owner of microRusEFI with extra RAM and use that extra RAM for extra Lua
	if (isStm32F42x()) {
		char *buffer = (char *)0x20020000;
		userHeap.reinit(buffer, 60000);
	}
#endif

#if LUA_USER_HEAP > 1
#if EFI_CAN_SUPPORT
	initLuaCanRx();
#endif // EFI_CAN_SUPPORT

	luaThread.start();

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
		auto heapSize = userHeap.size();
		auto memoryUsed = userHeap.used();
		float pct = 100.0f * memoryUsed / heapSize;
		efiPrintf("Lua memory heap usage: %d / %d bytes = %.1f%%", memoryUsed, heapSize, pct);
	});
#endif
}

#else // not EFI_UNIT_TEST

void startLua() { }

#include <stdexcept>
#include <string>

static LuaHandle runScript(const char* script) {
	auto ls = setupLuaState(myAlloc);

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
	auto ls = setupLuaState(myAlloc);

	if (!ls) {
		throw std::logic_error("Call to setupLuaState failed, returned null");
	}

	if (!loadScript(ls, script)) {
		throw std::logic_error("Call to loadScript failed");
	}
}

#endif // EFI_UNIT_TEST

// This is technically non-compliant, but it's only used for lua float parsing.
// It doesn't properly handle very small and very large numbers, and doesn't
// parse numbers in the format 1.3e5 at all.
extern "C" float strtof_rusefi(const char* str, char** endPtr) {
	bool afterDecimalPoint = false;
	float div = 1; // Divider to place digits after the decimal point

	if (endPtr) {
		*endPtr = const_cast<char*>(str);
	}

	float integerPart = 0;
	float fractionalPart = 0;

	while (*str != '\0') {
		char c = *str;
		int digitVal = c - '0';

		if (c >= '0' && c <= '9') {
			if (!afterDecimalPoint) {
				// Integer part
				integerPart = 10 * integerPart + digitVal;
			} else {
				// Fractional part
				fractionalPart = 10 * fractionalPart + digitVal;
				div *= 10;
			}
		} else if (c == '.') {
			afterDecimalPoint = true;
		} else {
			break;
		}

		str++;

		if (endPtr) {
			*endPtr = const_cast<char*>(str);
		}
	}

	return integerPart + fractionalPart / div;
}

#endif // EFI_LUA
