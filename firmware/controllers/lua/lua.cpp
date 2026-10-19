/**
 * @file lua.cpp
 * @brief Lua scripting engine integration.
 *
 * Owns the embedded Lua virtual machine lifecycle: allocation, loading user scripts,
 * the periodic Lua thread that runs onTick/onCanRx callbacks, and reset/reload
 * handling. User-facing rusEFI APIs are registered separately in lua_hooks.cpp.
 */

#include "pch.h"

#include "rusefi_lua.h"
#include "thread_controller.h"

#if EFI_LUA

#include "lua.hpp"
#include "lua_heap.h"
#include "lua_hooks.h"
#include "can_filter.h"
#include "lua_config_page.h"
#include "bench_test.h"
#include <cstdlib>

#if LUA_I2C_DAC
#include "mcp4728.h"
#endif

#define TAG "LUA "

static bool withErrorLoading = false;
static int luaTickPeriodUs;

#if EFI_CAN_SUPPORT
static int recentRxCount = 0;
static int totalRxCount = 0;
static int rxTime;
#endif // EFI_CAN_SUPPORT

static int lua_setTickRate(lua_State* l) {
	float userFreq = luaL_checknumber(l, 1);

	// For instance BMW does 100 CAN messages per second on some IDs, let's allow at least twice that speed
	// Limit to 1..200 hz
	float freq = clampF(1, userFreq, 2000);
	if (freq != userFreq) {
		efiPrintf(TAG "clamping tickrate %f", freq);
	}

	if (freq > 150 && !engineConfiguration->luaCanRxWorkaround) {
		efiPrintf(TAG "luaCanRxWorkaround recommended at high tick rate!");
	}

	luaTickPeriodUs = 1000000.0f / freq;
	return 0;
}

static void loadLibraries(lua_State* ls) {
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

bool luaProtectedCall(lua_State* state, lua_CFunction callback, void* context, const char* description) {
	// [tag:lua_protected_call] A zero-upvalue C function and light userdata do
	// not allocate (the caller supplies stack space). A closure with captured
	// upvalues or full userdata could fail before lua_pcall installs its handler.
	lua_pushcfunction(state, callback);
	lua_pushlightuserdata(state, context);
	// No Lua message handler: producing a traceback can itself need memory.
	if (lua_pcall(state, 1, 0, 0) != LUA_OK) {
		// Converting a numeric error to a string can itself allocate. Only read
		// existing strings here, since we may already be out of memory. Do not
		// use lua_isstring: it also accepts numbers that lua_tostring converts.
		const char* message = lua_type(state, -1) == LUA_TSTRING
			? lua_tostring(state, -1) : "non-string error";
		warning(ObdCode::CUSTOM_LUA_ERROR, "Lua %s: %s", description, message);
		// lua_pcall leaves one error object in place of the function/arguments.
		// Consume it so repeated failures cannot fill the Lua stack.
		lua_pop(state, 1);
		return false;
	}
	return true;
}

static int initializeLuaState(lua_State* ls) {
	// [tag:lua_protected_setup] Even library/hook registration can exhaust the
	// heap. Run the entire setup inside luaProtectedCall, before any user script
	// executes. Borrow the raw state here; setupLuaState owns the LuaHandle in
	// the outer C++ frame that survives Lua's longjmp and performs teardown.
	loadLibraries(ls);
	lua_register(ls, "setTickRate", lua_setTickRate);
	configureRusefiLuaHooks(ls);

	lua_gc(ls, LUA_GCCOLLECT, 0);
	// see https://www.lua.org/manual/5.4/manual.html#2.5.1
	lua_gc(ls, LUA_GCINC, 50, 1000, 9);
	return 0;
}

static LuaHandle setupLuaState(lua_Alloc alloc, void* userdata = nullptr) {
	// lua_newstate protects its own bootstrap and frees a partial state before
	// returning null. Our library/hook registration needs a separate boundary.
	LuaHandle ls = lua_newstate(alloc, userdata);

	if (!ls) {
		warning(ObdCode::CUSTOM_LUA_ERROR, "Failed to start Lua interpreter: not enough memory");

		return nullptr;
	}

	lua_atpanic(ls, [](lua_State* l) {
		warning(ObdCode::CUSTOM_LUA_ERROR, "Lua panic: %s",
			lua_type(l, -1) == LUA_TSTRING ? lua_tostring(l, -1) : "non-string error");

		// Last resort for an API error outside a protected call. Returning from
		// panic calls Lua's abort(); sleeping instead lets other ECU threads run.
		// This path never returns to the loop that observes needsReset, so Lua
		// requires an ECU restart. Expected OOM errors return from protected
		// calls and follow the normal callback/setup recovery paths below.
#if EFI_UNIT_TEST
		// Make a missed protection boundary fail the host test immediately.
		std::abort();
#else
		while (true) {
			chThdSleepMilliseconds(1000);
		}
#endif
		return 0;
	});

	if (!luaProtectedCall(ls, initializeLuaState, nullptr, "initialization failed")) {
		// Normal scope exit closes the partial VM, including class-name
		// finalizers in luaaa.hpp. Returning a half-registered VM is unsafe;
		// runOneLua reports failure and ThreadTask waits for an explicit reset.
		return nullptr;
	}

	return ls;
}

// this function is needed for a correct graph on the stackUsageReport
static int doString(lua_State* ls, const char* script, const char* name = "@script.lua") {
	// [tag:lua_chunk_name] Lua retains the chunk name in every function's debug
	// metadata. Using the script itself duplicates up to LUA_SCRIPT_SIZE bytes
	// in the heap, in addition to the configuration page and compiled code.
	// Loading protects parser allocations; lua_pcall protects chunk execution.
	// Preserve the actual status from either stage: combining them with ||
	// reduces errors to 1 and hides LUA_ERRMEM from both callers below.
	int status = luaL_loadbufferx(ls, script, std::strlen(script), name, "t");
	return status == LUA_OK ? lua_pcall(ls, 0, LUA_MULTRET, 0) : status;
}

static bool loadScript(LuaHandle& ls, const char* scriptStr) {
	efiPrintf(TAG "loading script length: %u...", std::strlen(scriptStr));

	int status = doString(ls, scriptStr);
	if (status != LUA_OK) {
		withErrorLoading = true;
		// Syntax/runtime diagnostics still go to the console. Allocation failure
		// additionally reaches the ECU warning channel, without a fatal latch.
		if (status == LUA_ERRMEM) {
			warning(ObdCode::CUSTOM_LUA_ERROR, "Lua script loading failed: not enough memory");
		}
		efiPrintf(TAG "ERROR loading script: %s", lua_tostring(ls, -1));
		lua_pop(ls, 1);
		return false;
	}

	efiPrintf(TAG "script loaded successfully!");

#if EFI_PROD_CODE
	luaHeapPrintInfo();
#endif // EFI_PROD_CODE

	return true;
}

#if !EFI_UNIT_TEST
static bool interactivePending = false;
static char interactiveCmd[100];

static void doInteractive(LuaHandle& ls) {
	if (!interactivePending) {
		// no cmd pending, return
		return;
	}

	auto status = doString(ls, interactiveCmd, "@interactive.lua");

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
		// Use the same OOM warning as script loading; consuming this command
		// below lets the Lua loop continue and accept another command/reset.
		if (status == LUA_ERRMEM) {
			warning(ObdCode::CUSTOM_LUA_ERROR, "Lua interactive command failed: not enough memory");
		}
		efiPrintf(TAG "interactive error: %s", lua_tostring(ls, -1));
	}

	interactivePending = false;

	lua_settop(ls, 0);
}

static uint32_t maxLuaDuration{};

static int invokeTickCallback(lua_State* ls) {
	// [tag:lua_protected_call] Global lookup can intern a name or invoke the
	// lazy native __index hook. It belongs inside the same boundary as onTick.
	lua_getglobal(ls, "onTick");
	if (lua_isnil(ls, -1)) {
		// TODO: handle missing tick function
		return 0;
	}
	// Errors propagate to the outer lua_pcall, which also covers the lookup.
	lua_call(ls, 0, 0);
	return 0;
}

static void invokeTick(LuaHandle& ls) {
	// Keep the performance scope outside the callback so a Lua longjmp cannot
	// bypass its destructor. Timing includes lookup and failure reporting.
	ScopePerf perf(PE::LuaTickFunction);
#if EFI_PROD_CODE
	uint32_t before = port_rt_get_counter_value();
#endif // EFI_PROD_CODE

	// A failed tick is reported and abandoned; later loop iterations still run
	// and can observe a reset request. Existing script side effects remain.
	luaProtectedCall(ls, invokeTickCallback, nullptr, "onTick failed");

#if EFI_PROD_CODE
	uint32_t duration = port_rt_get_counter_value() - before;
	maxLuaDuration = std::max(maxLuaDuration, duration);
#endif // EFI_PROD_CODE

	lua_settop(ls, 0);
}

struct LuaThread : ThreadController<LUA_THREAD_STACK_SIZE> {
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
	luaTickPeriodUs = MS2US(5);

	if (!loadScript(ls, script)) {
		return false;
	}

	while (!needsReset && !chThdShouldTerminateX()) {
		efitick_t beforeNt = getTimeNowNt();
#if EFI_CAN_SUPPORT
		// First, process any pending can RX messages
		totalRxCount += recentRxCount;
		recentRxCount = doLuaCanRx(ls);
		rxTime = getTimeNowNt() - beforeNt;
#endif // EFI_CAN_SUPPORT

		// Next, check if there is a pending interactive command entered by the user
		doInteractive(ls);

		invokeTick(ls);

		engine->outputChannels.luaLastCycleDuration = (getTimeNowNt() - beforeNt);
		engine->outputChannels.luaInvocationCounter++;
		chThdSleep(TIME_US2I(luaTickPeriodUs));

		engine->engineState.luaDigitalState0 = getAuxDigital(0);
		engine->engineState.luaDigitalState1 = getAuxDigital(1);
		engine->engineState.luaDigitalState2 = getAuxDigital(2);
		engine->engineState.luaDigitalState3 = getAuxDigital(3);
	}

	resetLua();

	return true;
}

RUSEFI_STACK_ROOT(LuaThread, ThreadTask);
void LuaThread::ThreadTask() {
	while (!chThdShouldTerminateX()) {
		bool wasOk = runOneLua(luaHeapAlloc, luaConfigPageGetState()->luaScript);

		auto usedAfterRun = luaHeapUsed();
		if (usedAfterRun != 0) {
			if (!withErrorLoading) {
				efiPrintf(TAG "MEMORY LEAK DETECTED: %d bytes used after teardown", usedAfterRun);
			}

			// Lua blew up in some terrible way that left memory allocated, reset the heap
			// so that subsequent runs don't overflow the heap
			luaHeapReset();
		}

		// Reset any lua adjustments the script made
		engine->resetLua();

		if (!wasOk) {
			// Setup or initial script loading failed and the LuaHandle has already
			// closed the VM. Wait for luareset instead of repeatedly allocating
			// and warning with the same memory pressure or invalid script.
			while (!needsReset) {
				chThdSleepMilliseconds(100);
			}
		}
	}
}

static LuaThread luaThread;

void startLua() {
#if LUA_I2C_DAC
	initI2cDacConsole();
#endif

#if EFI_CAN_SUPPORT
	initLuaCanRx();
#endif // EFI_CAN_SUPPORT

	addConsoleActionII("set_lua_setting", [](int index, int value) {
		engineConfiguration->scriptSetting[index] = value;
	});

	luaThread.start();

	addConsoleActionS("lua", [](const char* str){
		if (interactivePending) {
			return;
		}

		strncpy(interactiveCmd, str, sizeof(interactiveCmd) - 1);
		interactiveCmd[sizeof(interactiveCmd) - 1] = '\0';

		interactivePending = true;
	});

	addConsoleAction(CMD_LUA_RESET, [](){
		needsReset = true;
	});

	// these commands facilitate TS Lua Button scripts development; registered here rather than
	// in initBenchTest() so that they exist on boards without EFI_ENGINE_CONTROL
	addConsoleActionI("lua_button", [](int index) {
		if (index < 1 || index > LUA_BUTTON_COUNT) {
			return;
		}
		luaCommandCounters[index - 1]++;
	});
	addConsoleActionFFFF("luabench2", [](float humanIndex, float onTime, float offTimeMs, float count) {
		doRunBenchTestLuaOutput((int)humanIndex, onTime, offTimeMs, (int)count);
	});

	addConsoleAction("luainfo", [](){
		efiPrintf("maxLuaDuration %lu", maxLuaDuration);
		maxLuaDuration = 0;
		efiPrintf("rx total/recent/dropped %d %d %d", totalRxCount,
			recentRxCount, getLuaCanRxDropped());
		efiPrintf("luaCycle %luus including luaRxTime %dus", NT2US(engine->outputChannels.luaLastCycleDuration),
			NT2US(rxTime));

		luaHeapPrintInfo();
	});
}

#else // not EFI_UNIT_TEST

void startLua() { }

LuaHandle setupLuaStateForTests(lua_Alloc alloc, void* userdata) {
	return setupLuaState(alloc, userdata);
}

bool loadLuaScriptForTests(LuaHandle& ls, const char* script) {
	return loadScript(ls, script);
}

#include <stdexcept>
#include <string>

static LuaHandle runScript(const char* script) {
	auto ls = setupLuaState(luaHeapAlloc);

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
	auto ls = setupLuaState(luaHeapAlloc);

	if (!ls) {
		throw std::logic_error("Call to setupLuaState failed, returned null");
	}

	if (!loadScript(ls, script)) {
		throw std::logic_error("Call to loadScript failed");
	}
}

#endif // EFI_UNIT_TEST
#endif // EFI_LUA

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
