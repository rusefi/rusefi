#include "pch.h"
#include "rusefi_lua.h"
#include "idle_thread.h"
#include "lua_hooks.h"
#include "lua_hooks_registry.h"

#include <cstdlib>

namespace {
int luaRegistryTestHook42(lua_State* state) {
	lua_pushinteger(state, 42);
	return 1;
}

int luaRegistryTestHook84(lua_State* state) {
	lua_pushinteger(state, 84);
	return 1;
}

int luaForceGarbageCollection(lua_State* state) {
	lua_gc(state, LUA_GCCOLLECT, 0);
	return 0;
}

struct LuaAllocationControl {
	size_t allowedAllocations = 0;
	size_t deniedAllocations = 0;
	bool armed = false;
};

void* luaFaultAllocator(void* userData, void* pointer, size_t oldSize, size_t newSize) {
	auto& control = *static_cast<LuaAllocationControl*>(userData);
	if (newSize == 0) {
		std::free(pointer);
		return nullptr;
	}

	// Lua requires that shrinking an existing block never fail. Inject only
	// allocation/growth failures, matching the production allocator contract.
	if (pointer && newSize <= oldSize) {
		void* resized = std::realloc(pointer, newSize);
		return resized ? resized : pointer;
	}

	if (control.armed) {
		if (control.allowedAllocations == 0) {
			control.deniedAllocations++;
			return nullptr;
		}
		control.allowedAllocations--;
	}

	return std::realloc(pointer, newSize);
}

int luaResolveOomNative(lua_State* state) {
	lua_pushglobaltable(state);
	lua_pushvalue(state, lua_upvalueindex(1));
	lua_gettable(state, -2);
	lua_remove(state, -2);
	return 1;
}

int luaAssignOomNative(lua_State* state) {
	lua_pushglobaltable(state);
	lua_pushvalue(state, lua_upvalueindex(1));
	lua_pushinteger(state, 99);
	lua_settable(state, -3);
	return 0;
}

int luaOomNativeHook(lua_State* state) {
	lua_pushinteger(state, 42);
	return 1;
}

void enumerateLuaOomNativeHook(LuaHookRegistry& hooks) {
	hooks.add("oomNative", luaOomNativeHook);
}

void enumerateLuaRegistryTestHooks42(LuaHookRegistry& hooks) {
	// Lookup checks this list without adding every function name to Lua.
	hooks.add("lazyA", luaRegistryTestHook42);
	hooks.add("lazyAlias", luaRegistryTestHook42);
	hooks.add("lazyBefore", luaRegistryTestHook42);
}

void enumerateLuaRegistryTestHooks84(LuaHookRegistry& hooks) {
	hooks.add("lazyA", luaRegistryTestHook84);
}
}

TEST(LuaHooks, LazyNativeHookRegistryAssignmentAndIdentity) {
	// [tag:lua_lazy_native_hooks] Scripts can replace a native function or set it
	// to nil, before or after their first lookup.
	LuaHandle handle = luaL_newstate();
	lua_State* state = handle;
	ASSERT_NE(nullptr, state);
	// If a board already defined this name, the built-in hook must replace it.
	lua_pushinteger(state, 7);
	lua_setglobal(state, "lazyA");
	lua_pushcfunction(state, luaForceGarbageCollection);
	lua_setglobal(state, "forceLuaGc");
	// Numeric key 17 must stay numeric during global-table enumeration.
	lua_pushglobaltable(state);
	lua_pushinteger(state, 17);
	lua_pushstring(state, "numeric-key");
	lua_rawset(state, -3);
	lua_pop(state, 1);
	configureLuaHookRegistryForTests(state, enumerateLuaRegistryTestHooks42);

	// Both interpreters use the same name for different functions. Setting up one
	// interpreter must not change the other's lookup results.
	LuaHandle secondHandle = luaL_newstate();
	lua_State* secondState = secondHandle;
	ASSERT_NE(nullptr, secondState);
	configureLuaHookRegistryForTests(secondState, enumerateLuaRegistryTestHooks84);

	const char* script = R"(
		lazyBefore = nil
		if lazyBefore ~= nil then return 1 end
		lazyBefore = 17
		if lazyBefore ~= 17 then return 2 end
		lazyBefore = nil
		if lazyBefore ~= nil then return 3 end

		local first = lazyA
		forceLuaGc()
		if first() ~= 42 or first ~= lazyA or first ~= lazyAlias then return 4 end
		lazyA = function() return 99 end
		if lazyA() ~= 99 then return 5 end
		forceLuaGc()
		lazyA = nil
		if lazyA ~= nil then return 6 end

		if unknownLuaGlobal ~= nil then return 7 end
		unknownLuaGlobal = 23
		if unknownLuaGlobal ~= 23 then return 8 end
		return 0
	)";
	ASSERT_EQ(0, luaL_loadstring(state, script));
	ASSERT_EQ(0, lua_pcall(state, 0, 1, 0));
	EXPECT_EQ(0, lua_tointeger(state, -1));

	lua_pushglobaltable(state);
	lua_pushinteger(state, 17);
	lua_rawget(state, -2);
	EXPECT_STREQ("numeric-key", lua_tostring(state, -1));
	lua_pop(state, 2);

	// Key 18 is numeric, not the string "18", and bypasses name lookup.
	lua_pushglobaltable(state);
	lua_pushinteger(state, 18);
	lua_pushinteger(state, 99);
	lua_settable(state, -3);
	lua_pushinteger(state, 18);
	lua_rawget(state, -2);
	EXPECT_EQ(99, lua_tointeger(state, -1));
	lua_pop(state, 2);

	// The NUL-containing key "lazyA\0suffix" must not match the "lazyA" prefix.
	lua_pushglobaltable(state);
	constexpr char nulName[] = "lazyA\0suffix";
	lua_pushlstring(state, nulName, sizeof(nulName) - 1);
	lua_gettable(state, -2);
	EXPECT_TRUE(lua_isnil(state, -1));
	lua_pop(state, 2);

	ASSERT_EQ(0, luaL_loadstring(secondState, "return lazyA()"));
	ASSERT_EQ(0, lua_pcall(secondState, 0, 1, 0));
	EXPECT_EQ(84, lua_tointeger(secondState, -1));
}

TEST(LuaHooks, LazyNativeHookFalseOverrideIsNotNil) {
	LuaHandle handle(luaL_newstate());
	auto* state = static_cast<lua_State*>(handle);
	ASSERT_NE(nullptr, state);
	configureLuaHookRegistryForTests(state, enumerateLuaRegistryTestHooks42);
	const char* script = R"(
		lazyA = false
		if lazyA ~= false then return 1 end
		lazyA = nil
		if lazyA ~= nil then return 2 end
		return 0
	)";
	ASSERT_EQ(LUA_OK, luaL_loadstring(state, script));
	ASSERT_EQ(LUA_OK, lua_pcall(state, 0, 1, 0));
	EXPECT_EQ(0, lua_tointeger(state, -1));
}

TEST(LuaHooks, LazyNativeHookMaterializationOomCharacterization) {
	int injectedOutOfMemoryCount = 0;
	int hiddenNativeCount = 0;

	for (size_t allowance = 0; allowance < 16; allowance++) {
		// Keep the allocator settings alive until LuaHandle closes the interpreter.
		LuaAllocationControl allocation{allowance, 0, false};
		LuaHandle handle(lua_newstate(luaFaultAllocator, &allocation));
		ASSERT_NE(nullptr, static_cast<lua_State*>(handle));
		auto* state = static_cast<lua_State*>(handle);
		configureLuaHookRegistryForTests(state, enumerateLuaOomNativeHook);

		// Prepare the protected lookup and its Lua string key before injecting failure.
		lua_pushstring(state, "oomNative");
		lua_pushcclosure(state, luaResolveOomNative, 1);
		ASSERT_TRUE(lua_checkstack(state, 4));

		allocation.armed = true;
		int status = lua_pcall(state, 0, 1, 0);
		allocation.armed = false;

		EXPECT_TRUE(status == LUA_OK || status == LUA_ERRMEM);
		if (status == LUA_OK) {
			EXPECT_TRUE(lua_isfunction(state, -1));
			lua_pop(state, 1);
			continue;
		}

		if (allocation.deniedAllocations > 0) {
			injectedOutOfMemoryCount++;
		}
		lua_pop(state, 1); // the protected Lua memory error

		// Disable allocation failures before checking whether the hook is still available.
		lua_getglobal(state, "oomNative");
		if (!lua_isfunction(state, -1)) {
			hiddenNativeCount++;
		}
		lua_pop(state, 1);
	}

	EXPECT_GT(injectedOutOfMemoryCount, 0);
	// A failed lookup must leave the function available once memory is available.
	EXPECT_EQ(0, hiddenNativeCount);
}

TEST(LuaHooks, LazyNativeHookOverrideOomCharacterization) {
	int injectedOutOfMemoryCount = 0;
	int hiddenOriginalNativeCount = 0;

	for (size_t allowance = 0; allowance < 16; allowance++) {
		LuaAllocationControl allocation{allowance, 0, false};
		LuaHandle handle(lua_newstate(luaFaultAllocator, &allocation));
		ASSERT_NE(nullptr, static_cast<lua_State*>(handle));
		auto* state = static_cast<lua_State*>(handle);
		configureLuaHookRegistryForTests(state, enumerateLuaOomNativeHook);

		lua_pushstring(state, "oomNative");
		lua_pushcclosure(state, luaAssignOomNative, 1);
		ASSERT_TRUE(lua_checkstack(state, 4));

		allocation.armed = true;
		int status = lua_pcall(state, 0, 0, 0);
		allocation.armed = false;
		EXPECT_TRUE(status == LUA_OK || status == LUA_ERRMEM);

		if (status == LUA_ERRMEM) {
			if (allocation.deniedAllocations > 0) {
				injectedOutOfMemoryCount++;
			}
			lua_pop(state, 1); // the protected Lua memory error
			lua_getglobal(state, "oomNative");
			if (!lua_isfunction(state, -1)) {
				hiddenOriginalNativeCount++;
			}
		} else {
			lua_getglobal(state, "oomNative");
			EXPECT_TRUE(lua_isinteger(state, -1));
			EXPECT_EQ(99, lua_tointeger(state, -1));
		}
		lua_pop(state, 1);
	}

	EXPECT_GT(injectedOutOfMemoryCount, 0);
	// A failed replacement must leave the function available once memory is available.
	EXPECT_EQ(0, hiddenOriginalNativeCount);
}

TEST(LuaHooks, ProductionLazyHookVisitorUnderUnitTest) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	Sensor::setMockValue(SensorType::Clt, 25);

	LuaHandle handle(luaL_newstate());
	auto* state = static_cast<lua_State*>(handle);
	ASSERT_NE(nullptr, state);
	configureRusefiLuaHooksLazyForTests(state);

	const char* script = R"(
		local timer = Timer.new()
		timer:reset()
		local pid = Pid.new(0.5, 0, 0, -10, 10)
		local pidValue = pid:get(0, 0)
		local randomValue = random()
		return getSensor("CLT") == 25
			and interpolate(1, 10, 5, 50, 3) == 30
			and randomValue >= 0 and randomValue <= 1
			and timer:getElapsedSeconds() >= 0
			and pidValue ~= nil
	)";
	ASSERT_EQ(0, luaL_loadstring(state, script));
	ASSERT_EQ(0, lua_pcall(state, 0, 1, 0));
	EXPECT_TRUE(lua_toboolean(state, -1));
}

TEST(LuaHooks, ParkNeutral) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& idle = engine->module<IdleController>().unmock();
	setTable(config->cltIdleCorrTable, 50.0f);
	engineConfiguration->idleParkNeutralOffset = -10;

	// Exercise the BMW/CANTCU encoding: P=-1, N=-3, R=-2, forward=1..8.
	for (int gear : {-1, -3, -2, 1, 8}) {
		Sensor::setMockValue(SensorType::DetectedGear, gear);
		EXPECT_EQ(1, testLuaReturnsNumber(R"(
			function testFunc()
				local gear = getSensor("DetectedGear")
				setParkNeutral(gear == -1 or gear == -3)
				return 1
			end
		)"));
		EXPECT_FLOAT_EQ(gear == -1 || gear == -3 ? 40 : 50,
			idle.getRunningOpenLoop(IIdleController::Phase::Idling, 800, 80, 0));
	}

	// Lua treats numeric zero as truthy: require an actual boolean.
	EXPECT_THROW(testLuaReturnsNumber(R"(
		function testFunc()
			setParkNeutral(0)
			return 1
		end
	)"), std::logic_error);
}


TEST(LuaHooks, SetDashLight) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& out = engine->outputChannels;

	// each index maps to exactly one bit, the others are left alone
	EXPECT_EQ(1, testLuaReturnsNumber(R"(
		function testFunc()
			setDashLight(0, true)
			setDashLight(3, true)
			return 1
		end
	)"));
	EXPECT_TRUE(out.dashLeftTurn);
	EXPECT_FALSE(out.dashRightTurn);
	EXPECT_FALSE(out.dashCheckEngine);
	EXPECT_TRUE(out.dashHighBeams);
	EXPECT_FALSE(out.dashParkingBrake);

	EXPECT_EQ(1, testLuaReturnsNumber(R"(
		function testFunc()
			setDashLight(0, false)
			setDashLight(1, true)
			setDashLight(2, true)
			setDashLight(4, true)
			return 1
		end
	)"));
	EXPECT_FALSE(out.dashLeftTurn);
	EXPECT_TRUE(out.dashRightTurn);
	EXPECT_TRUE(out.dashCheckEngine);
	EXPECT_TRUE(out.dashHighBeams);
	EXPECT_TRUE(out.dashParkingBrake);

	// nothing in the firmware clears the bits behind the script's back
	EXPECT_EQ(1, testLuaReturnsNumber(R"(
		function testFunc()
			return 1
		end
	)"));
	EXPECT_TRUE(out.dashRightTurn);
	EXPECT_TRUE(out.dashParkingBrake);

	// out-of-range index is a Lua error rather than a silent no-op
	EXPECT_THROW(testLuaReturnsNumber(R"(
		function testFunc()
			setDashLight(5, true)
			return 1
		end
	)"), std::logic_error);
	EXPECT_THROW(testLuaReturnsNumber(R"(
		function testFunc()
			setDashLight(-1, true)
			return 1
		end
	)"), std::logic_error);
}

TEST(LuaHooks, TestCrc8) {
	const char* realHDdata = R"(

	function testFunc()
		return crc8_j1850({0x13, 0x57, 0x13, 0x45, 0x00, 0xe8, 0x5c }, 7)
	end

	)";
	EXPECT_EQ(testLuaReturnsNumberOrNil(realHDdata).value_or(0), 0x86);

	const char* crc8scripts = R"(

	function testFunc()
		return crc8_j1850({0x31,0x32,0x32 }, 2)
	end

	)";
	EXPECT_EQ(testLuaReturnsNumberOrNil(crc8scripts).value_or(0), 0x43);
}

TEST(LuaHooks, TestGetCalibration) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	const char* sourceCode = R"(

	function testFunc()
		return getCalibration("cranking.rpm")
	end

	)";
	EXPECT_EQ(testLuaReturnsNumber(sourceCode), 550);
}

TEST(LuaHooks, TestSetCalibration) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
		const char* sourceCode = R"(

		function testFunc()
			setCalibration("cranking.rpm", 900, false)
			return getCalibration("cranking.rpm")
		end

		)";
		EXPECT_EQ(testLuaReturnsNumber(sourceCode), 900);
}


TEST(LuaHooks, TestGetSensorByIndex) {
	const char* getSensorTestByIndex = R"(

	function testFunc()
		return getSensorByIndex(10)
	end

	)";

	// Test failed sensor, returns nil
	Sensor::resetMockValue(static_cast<SensorType>(10));
	EXPECT_EQ(testLuaReturnsNumberOrNil(getSensorTestByIndex), unexpected);

	// Now test with a value, returns value
	Sensor::setMockValue(static_cast<SensorType>(10), 33);
	EXPECT_EQ(testLuaReturnsNumberOrNil(getSensorTestByIndex).value_or(0), 33);
}


TEST(LuaHooks, TestGetSensorByName) {
	const char* getSensorTestByName = R"(

	function testFunc()
		return getSensor("CLT")
	end

	)";

	// Test failed sensor, returns nil
	Sensor::resetMockValue(SensorType::Clt);
	EXPECT_EQ(testLuaReturnsNumberOrNil(getSensorTestByName), unexpected);

	// Now test with a value, returns value
	Sensor::setMockValue(SensorType::Clt, 33);
	EXPECT_EQ(testLuaReturnsNumberOrNil(getSensorTestByName).value_or(0), 33);
}

TEST(LuaHooks, Table3d) {
	const char* tableTest = R"(
	function testFunc()
		return table3d(2, 1000, 40)
	end
	)";

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	setTable(config->scriptTable2, (uint8_t)33);
	EXPECT_EQ(testLuaReturnsNumber(tableTest), 33);

	setTable(config->scriptTable2, (uint8_t)14);
	EXPECT_EQ(testLuaReturnsNumber(tableTest), 14);
}

TEST(LuaHooks, CanTxChannel) {
	// channel 1 valid
	EXPECT_NO_THROW(testLuaExecString("txCan(1, 0, 0, {0})"));

	// channel 0 invalid
	EXPECT_ANY_THROW(testLuaExecString("txCan(0, 0, 0, {0})"));
}

TEST(LuaHooks, CanTxId) {
	// std ID ok
	EXPECT_NO_THROW(testLuaExecString("txCan(1, 2047, 0, {0})"));
	// std ID too high
	EXPECT_ANY_THROW(testLuaExecString("txCan(1, 2048, 0, {0})"));

	// ext ID ok
	EXPECT_NO_THROW(testLuaExecString("txCan(1, 536870911, 1, {0})"));
	// ext ID too high
	EXPECT_ANY_THROW(testLuaExecString("txCan(1, 536870912, 1, {0})"));
}

TEST(LuaHooks, CanTxDataLength) {
	// valid: DLC = 0
	EXPECT_NO_THROW(testLuaExecString("txCan(1, 0, 0, {})"));
	// valid: DLC = 1
	EXPECT_NO_THROW(testLuaExecString("txCan(1, 0, 0, {0})"));
	// valid: DLC = 8
	EXPECT_NO_THROW(testLuaExecString("txCan(1, 0, 0, {1, 2, 3, 4, 5, 6, 7, 8})"));

	// invalid: DLC = 9
	EXPECT_ANY_THROW(testLuaExecString("txCan(1, 0, 0, {1, 2, 3, 4, 5, 6, 7, 8, 9})"));

	// invalid: is a table, but contains a not-number
	EXPECT_ANY_THROW(testLuaExecString("txCan(1, 0, 0, {1, 2, 3, 'hello', 5, 6})"));

	// invalid: not a table
	EXPECT_ANY_THROW(testLuaExecString("txCan(1, 0, 0, 26)"));
}

TEST(LuaHooks, LuaInterpolate) {
	EXPECT_EQ(testLuaReturnsNumber(R"(
			function testFunc()
				return interpolate(1, 10, 5, 50, 3)
			end
			)"), 30);
}

TEST(LuaHooks, TestLuaTimer) {
	EXPECT_EQ(testLuaReturnsNumber(R"(
			function testFunc()
				local a = Timer.new()
				a:reset()
				return a:getElapsedSeconds()
			end
			)"), 0);
}

// [tag:lua_direct_class_methods] Direct adapters retain luaaa's receiver and
// argument validation, including errors before a native method can be called.
TEST(LuaHooks, ClassMethodsRejectInvalidReceiversAndArguments) {
	EXPECT_ANY_THROW(testLuaExecString(R"(
		local timer = Timer.new()
		timer.reset({})
	)"));
	EXPECT_ANY_THROW(testLuaExecString(R"(
		local timer = Timer.new()
		local pid = Pid.new(0.5, 0, 0, -10, 10)
		timer.getElapsedSeconds(pid)
	)"));
	EXPECT_ANY_THROW(testLuaExecString(R"(
		local pid = Pid.new(0.5, 0, 0, -10, 10)
		pid:get(nil, 7)
	)"));
	EXPECT_ANY_THROW(testLuaExecString(R"(
		local debounce = SignalDebounce.new(0.1)
		debounce:set(1)
	)"));
}

static const char* sensorTest = R"(
function testFunc()
	local sens = Sensor.new("CLT")

	-- Check valid sensor
	sens:set(33)
	if getSensor("CLT") ~= 33 then
		return 1
	end

	-- Check invalidation
	sens:invalidate()
	if getSensor("CLT") then
		return 2
	end

	return 0
end
)";

TEST(LuaHooks, LuaSensor) {
	EXPECT_EQ(testLuaReturnsNumber(sensorTest), 0);

	// Ensure that the sensor got unregistered on teardown of the Lua interpreter
	EXPECT_FALSE(Sensor::hasSensor(SensorType::Clt));
}

static const char* pidTest = R"(
function testFunc()
	local pid = Pid.new(0.5, 0, 0, -10, 10)

	-- delta is -4, output -2
	if pid:get(3, 7) ~= -2 then
		return 1
	end

	-- delta is 6, output 3
	if pid:get(4, -2) ~= 3 then
		return 2
	end

	-- test clamping
	if pid:get(0, 100) ~= -10 then
		return 3
	end

	if pid:get(0, -100) ~= 10 then
		return 4
	end

	return 0
end
)";

TEST(LuaHooks, LuaPid) {
	EXPECT_EQ(testLuaReturnsNumber(pidTest), 0);
}
