#include "pch.h"
#include "rusefi_lua.h"

#include <cstdlib>
#include <limits>
#include <string>

namespace {
// Fail after a chosen number of allocation/growth requests, rather than after a
// byte limit tied to host pointer sizes. Once exhausted, keep denying requests
// so Lua's emergency-GC retry cannot turn the intended OOM into a success.
struct LuaMemoryLimit {
	size_t remainingAllocations;
	size_t used = 0;
	size_t denied = 0;
	size_t byteLimit = std::numeric_limits<size_t>::max();

	static void* allocate(void* userdata, void* pointer, size_t oldSize, size_t newSize) {
		auto& limit = *static_cast<LuaMemoryLimit*>(userdata);
		if (!pointer) {
			oldSize = 0; // Lua passes an object type here for new allocations.
		}
		if (newSize == 0) {
			std::free(pointer);
			limit.used -= oldSize;
			return nullptr;
		}
		if (newSize > oldSize) {
			// Shrinking and freeing remain available for GC and partial-VM
			// teardown. Refusing growth leaves any existing block untouched.
			if (limit.remainingAllocations == 0 || limit.used - oldSize + newSize > limit.byteLimit) {
				limit.denied++;
				return nullptr;
			}
			limit.remainingAllocations--;
		}
		void* result = std::realloc(pointer, newSize);
		if (result) {
			// realloc failure preserves the old block and its accounting.
			limit.used = limit.used - oldSize + newSize;
		}
		return result;
	}
};
}

TEST(LuaBasic, StateAllocationFailure) {
	// No VM exists yet to install our panic handler/protected callback. Verify
	// the null result from lua_newstate still becomes C6730, with no fatal latch.
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	LuaMemoryLimit memory{0};
	auto state = setupLuaStateForTests(LuaMemoryLimit::allocate, &memory);
	EXPECT_EQ(static_cast<lua_State*>(state), nullptr);
	EXPECT_TRUE(hasRecentWarningCode(ObdCode::CUSTOM_LUA_ERROR));
	EXPECT_FALSE(hasFirmwareError());
	EXPECT_GT(memory.denied, 0u);
	EXPECT_EQ(memory.used, 0u);
}

TEST(LuaBasic, SetupAllocationFailure) {
	// [tag:lua_protected_setup] Allow bootstrap, then fail during registration.
	// This exercises initializeLuaState's protection and the outer LuaHandle's
	// cleanup; handling only a null return from lua_newstate would miss it.
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	LuaMemoryLimit memory{100};
	auto state = setupLuaStateForTests(LuaMemoryLimit::allocate, &memory);
	EXPECT_EQ(static_cast<lua_State*>(state), nullptr);
	EXPECT_TRUE(hasRecentWarningCode(ObdCode::CUSTOM_LUA_ERROR));
	EXPECT_FALSE(hasFirmwareError());
	EXPECT_GT(memory.denied, 0u);
	EXPECT_EQ(memory.used, 0u);
}

TEST(LuaBasic, SetupFailureAtEachAllocationAndRetry) {
	// [tag:lua_protected_setup] Measure a successful setup so the failure sweep
	// follows the actual library/class/hook registration for this build.
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	constexpr size_t unlimited = std::numeric_limits<size_t>::max();
	LuaMemoryLimit successful{unlimited};
	{
		auto state = setupLuaStateForTests(LuaMemoryLimit::allocate, &successful);
		ASSERT_NE(static_cast<lua_State*>(state), nullptr);
	}
	ASSERT_EQ(successful.used, 0u);
	size_t allocationCount = unlimited - successful.remainingAllocations;

	// Exercise every allocation budget through bootstrap and registration. Each
	// handle must be destroyed before checking bytes: successful VM teardown
	// also resets luaaa's static class-name pointers for the next iteration.
	for (size_t allowance = 0; allowance < allocationCount; allowance++) {
		SCOPED_TRACE(allowance);
		LuaMemoryLimit memory{allowance};
		{
			auto state = setupLuaStateForTests(LuaMemoryLimit::allocate, &memory);
			EXPECT_EQ(static_cast<lua_State*>(state), nullptr);
		}
		EXPECT_GT(memory.denied, 0u);
		EXPECT_EQ(memory.used, 0u);
		EXPECT_TRUE(hasRecentWarningCode(ObdCode::CUSTOM_LUA_ERROR));
		EXPECT_FALSE(hasFirmwareError());
	}

	// Model a subsequent Lua reset once memory is available. Zero leaked bytes
	// alone is insufficient: a dangling luaaa klassName would reject rebinding
	// in this fresh VM even though the previous state's allocations were freed.
	successful.remainingAllocations = unlimited;
	{
		auto state = setupLuaStateForTests(LuaMemoryLimit::allocate, &successful);
		ASSERT_NE(static_cast<lua_State*>(state), nullptr);
		ASSERT_TRUE(loadLuaScriptForTests(state, "function onTick() end"));
	}
	EXPECT_EQ(successful.used, 0u);
}

TEST(LuaBasic, ProtectedCallbackAllocationFailure) {
	// [tag:lua_protected_call] Exercise the shared boundary without CAN hardware.
	// Fail in C-side argument preparation, before a script function is called.
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	LuaMemoryLimit memory{std::numeric_limits<size_t>::max()};
	{
		LuaHandle state(lua_newstate(LuaMemoryLimit::allocate, &memory));
		ASSERT_NE(static_cast<lua_State*>(state), nullptr);
		memory.remainingAllocations = 0;
		auto callback = [](lua_State* ls) {
			// Like CAN argument construction, this happens before a Lua callback.
			lua_createtable(ls, 100, 0);
			return 0;
		};
		EXPECT_FALSE(luaProtectedCall(state, callback, nullptr, "callback failed"));
		EXPECT_GT(memory.denied, 0u);
		EXPECT_TRUE(hasRecentWarningCode(ObdCode::CUSTOM_LUA_ERROR));
		EXPECT_FALSE(hasFirmwareError());
		EXPECT_EQ(lua_gettop(state), 0);

		// A caught callback error leaves this VM usable; both paths must consume
		// their temporary stack entries so failures cannot accumulate each tick.
		memory.remainingAllocations = std::numeric_limits<size_t>::max();
		EXPECT_TRUE(luaProtectedCall(state, callback, nullptr, "callback failed"));
		EXPECT_EQ(lua_gettop(state), 0);
	}
	EXPECT_EQ(memory.used, 0u);
}

TEST(LuaBasic, ProtectedNonStringError) {
	// [tag:lua_protected_call] Numeric error objects expose accidental coercion
	// by the reporter: lua_tostring would allocate outside the protected call.
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	LuaMemoryLimit memory{std::numeric_limits<size_t>::max()};
	{
		LuaHandle state(lua_newstate(LuaMemoryLimit::allocate, &memory));
		ASSERT_NE(static_cast<lua_State*>(state), nullptr);
		auto callback = [](lua_State* ls) {
			auto* limit = static_cast<LuaMemoryLimit*>(lua_touserdata(ls, 1));
			// Exhaust memory only after entering the callback so this assertion
			// isolates error reporting from any call-frame setup allocations.
			limit->remainingAllocations = 0;
			lua_pushinteger(ls, 42);
			return lua_error(ls);
		};
		EXPECT_FALSE(luaProtectedCall(state, callback, &memory, "callback failed"));
		EXPECT_EQ(memory.denied, 0u); // Reporting the numeric error must not allocate.
		EXPECT_TRUE(hasRecentWarningCode(ObdCode::CUSTOM_LUA_ERROR));
		EXPECT_FALSE(hasFirmwareError());
		EXPECT_EQ(lua_gettop(state), 0);
	}
	EXPECT_EQ(memory.used, 0u);
}

TEST(LuaBasic, ScriptAllocationFailure) {
	// Setup succeeds; parsing/loading the user script fails independently.
	// This guards preservation of LUA_ERRMEM through doString into loadScript.
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	LuaMemoryLimit memory{std::numeric_limits<size_t>::max()};
	{
		auto state = setupLuaStateForTests(LuaMemoryLimit::allocate, &memory);
		ASSERT_NE(static_cast<lua_State*>(state), nullptr);
		memory.remainingAllocations = 0;
		EXPECT_FALSE(loadLuaScriptForTests(state, "function onTick() end"));
		EXPECT_GT(memory.denied, 0u);
		EXPECT_TRUE(hasRecentWarningCode(ObdCode::CUSTOM_LUA_ERROR));
		EXPECT_FALSE(hasFirmwareError());
		EXPECT_EQ(lua_gettop(state), 0);
	}
	EXPECT_EQ(memory.used, 0u);
}

TEST(LuaBasic, ScriptSourceMemory) {
	// [tag:lua_chunk_name] Comments in a near-limit uaEFI script must not
	// become a second persistent copy of its source in the execution heap.
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	LuaMemoryLimit memory{std::numeric_limits<size_t>::max()};
	LuaHandle state(lua_newstate(LuaMemoryLimit::allocate, &memory));
	ASSERT_NE(static_cast<lua_State*>(state), nullptr);
	// Plenty for this tiny program's bytecode, but not another 8 KB source copy.
	memory.byteLimit = memory.used + 4096;
	std::string script = "--" + std::string(7900, 'x') + "\nfunction onTick() end";
	lua_gc(state, LUA_GCCOLLECT, 0);
	int before = lua_gc(state, LUA_GCCOUNT, 0) * 1024 + lua_gc(state, LUA_GCCOUNTB, 0);
	ASSERT_TRUE(loadLuaScriptForTests(state, script.c_str()));
	lua_gc(state, LUA_GCCOLLECT, 0);
	int after = lua_gc(state, LUA_GCCOUNT, 0) * 1024 + lua_gc(state, LUA_GCCOUNTB, 0);
	EXPECT_LT(after - before, 1024);
	lua_getglobal(state, "onTick");
	lua_Debug info{};
	ASSERT_NE(lua_getinfo(state, ">S", &info), 0);
	EXPECT_STREQ(info.source, "@script.lua");
	EXPECT_EQ(info.linedefined, 2);
	EXPECT_EQ(memory.denied, 0u);
}

TEST(LuaBasic, ReturnsNumber) {
	auto script = R"(
						function testFunc()
							return 5.5
						end
				)";

	float result = testLuaReturnsNumber(script);

	EXPECT_FLOAT_EQ(result, 5.5f);
}

TEST(LuaBasic, MathLib) {
	auto script = R"(
		function testFunc()
			return math.min(3, 1, 2)
		end
	)";

	float result = testLuaReturnsNumber(script);

	EXPECT_FLOAT_EQ(result, 1.0f);
}

TEST(LuaBasic, MathLibFloor) {
	auto script = R"(
		function testFunc()
			return math.floor(3.2)
		end
	)";

	EXPECT_FLOAT_EQ(testLuaReturnsNumber(script), 3.0f);
}

TEST(LuaBasic, ReturnsInteger) {
	auto script = R"(
						function testFunc()
							return 33
						end
				)";

	int result = testLuaReturnsInteger(script);

	EXPECT_EQ(result, 33);
}

TEST(LuaBasic, InvalidScript) {
	EXPECT_ANY_THROW(testLuaReturnsInteger("ggg"));
}

TEST(LuaBasic, NoFunction) {
	EXPECT_ANY_THROW(testLuaReturnsInteger(R"(
		function testFuncccccc()
			return 3
		end
	)"));
}

TEST(LuaBasic, WrongReturnTypeExpectIntegerReturnsNothing) {
	EXPECT_ANY_THROW(testLuaReturnsInteger(R"(
		function testFunc()
		end
	)"));
}

TEST(LuaBasic, WrongReturnTypeExpectIntegerReturnsNil) {
	EXPECT_ANY_THROW(testLuaReturnsInteger(R"(
		function testFunc()
			return nil
		end
	)"));
}

TEST(LuaBasic, ExpectNumOrNilReturnsNil) {
	EXPECT_EQ(testLuaReturnsNumberOrNil(R"(
		function testFunc()
			return nil
		end
	)"), unexpected);
}

TEST(LuaBasic, ExpectNumOrNilReturnsNumber) {
	EXPECT_FLOAT_EQ(56.3f, testLuaReturnsNumberOrNil(R"(
		function testFunc()
			return 56.3
		end
	)").value_or(0));
}

TEST(LuaPid, Offset) {
	EXPECT_FLOAT_EQ(18.0f, testLuaReturnsNumberOrNil(R"(
		function testFunc()
pid = Pid.new(2, 0, 0, -100, 100)
pid:setOffset(20)
			return pid:get(20, 21)
		end
	)").value_or(0));
}

TEST(LuaBasic, ExpectNumOrNilReturnsNothing) {
	// Returning nothing is generally functionally equivalent to returning nil
	EXPECT_EQ(testLuaReturnsNumberOrNil(R"(
		function testFunc()
			return
		end
	)"), unexpected);
}

TEST(LuaSensor, Timeout) {
	EXPECT_FLOAT_EQ(0.0f, testLuaReturnsNumberOrNil(R"(
		function testFunc()
			cltSensor = Sensor.new("clt")
			cltSensor:setTimeout(3000)
		end
	)").value_or(0));
}

TEST(SystemLua, ScriptLoads) {
	startLua();
}
