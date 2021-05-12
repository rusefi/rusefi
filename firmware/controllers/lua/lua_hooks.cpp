#include "lua.hpp"
#include "lua_hooks.h"

#include "engine.h"
#include "loggingcentral.h"
#include "sensor.h"
#include "adc_inputs.h"
#include "efilib.h"
#include "tunerstudio_outputs.h"

// Some functions lean on existing FSIO implementation
#include "fsio_impl.h"

EXTERN_ENGINE;

static int lua_efi_print(lua_State* l) {
	auto msg = luaL_checkstring(l, 1);

	efiPrintf("LUA: %s", msg);

	return 0;
}

static int lua_getSensor(lua_State* l) {
	auto sensorIndex = luaL_checkinteger(l, 1);

	auto result = Sensor::get(static_cast<SensorType>(sensorIndex));

	if (result) {
		// return value if valid
		lua_pushnumber(l, result.Value);
	} else {
		// return nil if invalid
		lua_pushnil(l);
	}

	return 1;
}

static int lua_getSensorRaw(lua_State* l) {
	auto sensorIndex = luaL_checkinteger(l, 1);

	lua_pushnumber(l, Sensor::getRaw(static_cast<SensorType>(sensorIndex)));
	return 1;
}

static int lua_hasSensor(lua_State* l) {
	auto sensorIndex = luaL_checkinteger(l, 1);

	lua_pushboolean(l, Sensor::hasSensor(static_cast<SensorType>(sensorIndex)));
	return 1;
}

static int lua_table3d(lua_State* l) {
	auto tableIdx = luaL_checkinteger(l, 1);
	auto x = luaL_checknumber(l, 2);
	auto y = luaL_checknumber(l, 3);

	// index table, compute table lookup
	auto result = getFSIOTable(tableIdx)->getValue(x, y);

	lua_pushnumber(l, result);
	return 1;
}

#if !EFI_UNIT_TEST
static int lua_fan(lua_State* l) {
	lua_pushboolean(l, enginePins.fanRelay.getLogicValue());
	return 1;
}

static int lua_getDigital(lua_State* l) {
	auto idx = luaL_checkinteger(l, 1);

	bool state = false;

	switch (idx) {
		case 0: state = engine->clutchDownState; break;
		case 1: state = engine->clutchUpState; break;
		case 2: state = engine->brakePedalState; break;
		case 3: state = engine->acSwitchState; break;
		default:
			// Return nil to indicate invalid parameter
			lua_pushnil(l);
			return 1;
	}

	lua_pushboolean(l, state);
	return 1;
}

static int lua_setDebug(lua_State* l) {
	// wrong debug mode, ignore
	if (CONFIG(debugMode) != DBG_LUA) {
		return 0;
	}

	auto idx = luaL_checkinteger(l, 1);
	auto val = luaL_checknumber(l, 2);

	// invalid index, ignore
	if (idx < 1 || idx > 7) {
		return 0;
	}

	auto firstDebugField = &tsOutputChannels.debugFloatField1;
	firstDebugField[idx - 1] = val;

	return 0;
}
#endif // EFI_UNIT_TEST

void configureRusefiLuaHooks(lua_State* l) {
	lua_register(l, "print", lua_efi_print);
	lua_register(l, "getSensor", lua_getSensor);
	lua_register(l, "getSensorRaw", lua_getSensorRaw);
	lua_register(l, "hasSensor", lua_hasSensor);
	lua_register(l, "table3d", lua_table3d);

#if !EFI_UNIT_TEST
	lua_register(l, "getFan", lua_fan);
	lua_register(l, "getDigital", lua_getDigital);
	lua_register(l, "setDebug", lua_setDebug);
#endif
}
