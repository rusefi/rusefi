/**
 * @file lua_class_adapters.cpp
 * @brief Reduce helper-class registration overhead in the Lua heap.
 *
 * Lua heap space is scarce on STM32F4. Every enabled helper class is registered
 * before the user script runs, so even unused methods consume startup RAM.
 * luaaa's generic member binding allocates userdata to hold a C++ member pointer
 * and a C closure that captures it: two heap allocations per registered method.
 *
 * [tag:lua_direct_class_methods]
 * These targets are fixed when the firmware is built. Encoding each target as a
 * template argument lets us expose a light Lua C function with no captured state,
 * eliminating both allocations and leaving more heap for scripts and instances.
 * This saves registration memory; it does not depend on how often a method runs.
 *
 * The Lua API stays the same. luaaa still supplies constructors, instance userdata,
 * metatables and GC finalizers; LuaStack preserves type checks, argument conversions
 * and return values. Only the method dispatch uses these direct adapters. See the
 * registration procedure below before adding a method.
 */

#include "pch.h"
#include "rusefi_lua.h"
#include "lua_hooks.h"
#include "lua_biquad.h"
#include "lua_pid.h"
#include "signaldebounce.h"
#include <new>
#include <type_traits>

#define LUAAA_WITHOUT_CPP_STDLIB
#include "luaaa.hpp"
using namespace luaaa;

// To add a class method:
// 1. Keep LuaClass<Type> and .ctor<...>() for userdata, metatables and __gc.
// 2. Register .fun("luaName", directMethod<&Type::method>). For an overloaded
//    method, use a static_cast with the exact signature (see Timer below).
//    This selects .fun's lua_CFunction overload, which needs no closure upvalues.
// 3. Keep the class's build guard. Lua ':' calls put self at stack index 1;
//    arguments start at 2. LuaStack retains existing type checks/conversions.
// 4. Exercise the method, invalid receivers/arguments and interpreter teardown
//    in the Lua tests. Do not capture runtime state or pass a raw member pointer
//    to .fun: that reintroduces the allocations this adapter avoids.
// Constructors and finalizers remain luaaa's responsibility. Their metatables
// must stay attached even though methods no longer need captured userdata.
namespace {

template<auto Method, typename Class, typename Result, typename... Args, size_t... Indices>
int invokeDirectMethod(lua_State* l, indices<Indices...>) {
    auto& object = LuaStack<Class>::get(l, 1);
    if constexpr (std::is_void_v<Result>) {
        (object.*Method)(LuaStack<Args>::get(l, Indices + 2)...);
        return 0;
    } else {
        LuaStackReturn<Result>(l, (object.*Method)(LuaStack<Args>::get(l, Indices + 2)...));
        return 1;
    }
}

template<auto Method, typename Class, typename Result, typename... Args>
int dispatchDirectMethod(lua_State* l, Result(Class::*)(Args...)) {
    return invokeDirectMethod<Method, Class, Result, Args...>(l, typename make_indices<sizeof...(Args)>::type());
}

template<auto Method, typename Class, typename Result, typename... Args>
int dispatchDirectMethod(lua_State* l, Result(Class::*)(Args...) const) {
    return invokeDirectMethod<Method, Class, Result, Args...>(l, typename make_indices<sizeof...(Args)>::type());
}

template<auto Method>
int directMethod(lua_State* l) {
    return dispatchDirectMethod<Method>(l, Method);
}

} // namespace

SensorType findLuaSensorByName(lua_State* l, const char* name) {
	SensorType type = findSensorTypeByName(name);

	if (l && type == SensorType::Invalid) {
		luaL_error(l, "Invalid sensor type: %s", name);
	}

	return type;
}

// TODO: PR this back in to https://github.com/gengyong/luaaa
namespace LUAAA_NS {
    template<typename TCLASS, typename ...ARGS>
    struct PlacementConstructorCaller<TCLASS, lua_State*, ARGS...>
    {
        // Pass the Lua state as a hidden first constructor argument. Only the
        // remaining arguments consume positions on the Lua stack.

        static TCLASS * Invoke(lua_State * state, void * mem)
        {
            return InvokeImpl(state, mem, typename make_indices<sizeof...(ARGS)>::type());
        }

    private:
        template<std::size_t ...Ns>
        static TCLASS * InvokeImpl(lua_State * state, void * mem, indices<Ns...>)
        {
            return new(mem) TCLASS(state, LuaStack<ARGS>::get(state, Ns + 1)...);
        }
    };
}

struct LuaSensor final : public StoredValueSensor {
	LuaSensor() : LuaSensor(nullptr, "Invalid") { }

	~LuaSensor() {
		unregister();
	}

	LuaSensor(lua_State* l, const char* name)
		: StoredValueSensor(findLuaSensorByName(l, name), MS2NT(100))
	{
		// do a soft collision check to avoid a fatal error from the hard check in Register()
		if (l && Sensor::hasSensor(type())) {
			luaL_error(l, "Tried to create a Lua sensor of type %s, but one was already registered.", getSensorName());
		} else {
			Register();
			efiPrintf("LUA registered sensor of type %s", getSensorName());
		}
	}

	bool isRedundant() const override {
		return m_isRedundant;
	}

	// Bind the override as a LuaSensor member for receiver type checks.
	void setTimeout(int timeoutMs) override {
	    StoredValueSensor::setTimeout(timeoutMs);
	}

	void setRedundant(bool value) {
		m_isRedundant = value;
	}

	void set(float value) {
		setValidValue(value, getTimeNowNt());
	}

	void invalidate() {
		StoredValueSensor::invalidate();
	}

	void showInfo(const char* sensorName) const override {
		const auto value = get();
		efiPrintf("Sensor \"%s\": Lua sensor: Valid: %s Converted value %.2f", sensorName, boolToString(value.Valid), value.Value);
	}

private:
	bool m_isRedundant = false;
};

void configureRusefiLuaClassHooks(lua_State* lState) {
	LuaClass<Timer> luaTimer(lState, "Timer");
	luaTimer
		.ctor()
		.fun("reset",             directMethod<static_cast<void (Timer::*)()>(&Timer::reset)>)
		.fun("getElapsedSeconds", directMethod<static_cast<float(Timer::*)() const>(&Timer::getElapsedSeconds)>);

	LuaClass<LuaSensor> luaSensor(lState, "Sensor");
	luaSensor
		.ctor<lua_State*, const char*>()
		.fun("set", directMethod<&LuaSensor::set>)
		.fun("setRedundant", directMethod<&LuaSensor::setRedundant>)
		.fun("setTimeout", directMethod<&LuaSensor::setTimeout>)
		.fun("invalidate", directMethod<&LuaSensor::invalidate>);

#ifndef WITH_LUA_PID
#define WITH_LUA_PID TRUE
#endif

#if WITH_LUA_PID
	LuaClass<LuaBiQuad> biquad(lState, "Biquad");
	biquad
		.ctor()
		.fun("filter", directMethod<&LuaBiQuad::filter>)
		.fun("configureLowpass", directMethod<&LuaBiQuad::configureLowpass>);

	LuaClass<LuaPid> luaPid(lState, "Pid");
	luaPid
		.ctor<float, float, float, float, float>()
		.fun("get", directMethod<&LuaPid::get>)
		.fun("setOffset", directMethod<&LuaPid::setOffset>)
		.fun("reset", directMethod<&LuaPid::reset>);

	LuaClass<LuaIndustrialPid> luaIndustrialPid(lState, "IndustrialPid");
	luaIndustrialPid
		.ctor<float, float, float, float, float>()
		.fun("get", directMethod<&LuaIndustrialPid::get>)
		.fun("setOffset", directMethod<&LuaIndustrialPid::setOffset>)
		.fun("setDerivativeFilterLoss", directMethod<&LuaIndustrialPid::setDerivativeFilterLoss>)
		.fun("setAntiwindupFreq", directMethod<&LuaIndustrialPid::setAntiwindupFreq>)
		.fun("reset", directMethod<&LuaIndustrialPid::reset>);

	LuaClass<SignalDebounce> luaDebounce(lState, "SignalDebounce");
	luaDebounce
		.ctor<float>()
		.fun("set", directMethod<&SignalDebounce::set>)
		.fun("get", directMethod<&SignalDebounce::get>);
#endif // WITH_LUA_PID
}
