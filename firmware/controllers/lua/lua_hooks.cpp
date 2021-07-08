#include "lua.hpp"
#include "lua_hooks.h"

#include "engine.h"
#include "loggingcentral.h"
#include "sensor.h"
#include "adc_inputs.h"
#include "efilib.h"
#include "tunerstudio_outputs.h"
#include "fuel_math.h"
#include "airmass.h"
#include "lua_airmass.h"
#include "pwm_generator_logic.h"

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

static LuaAirmass luaAirmass;

AirmassModelBase& getLuaAirmassModel() {
	return luaAirmass;
}

#if !EFI_UNIT_TEST
static SimplePwm pwms[LUA_PWM_COUNT];
static OutputPin pins[LUA_PWM_COUNT];

struct P {
	SimplePwm& pwm;
	lua_Integer idx;
};

static P luaL_checkPwmIndex(lua_State* l, int pos) {
	auto channel = luaL_checkinteger(l, pos);

	// Ensure channel is valid
	if (channel < 0 || channel >= FSIO_COMMAND_COUNT) {
		luaL_error(l, "setPwmDuty invalid channel %d", channel);
	}

	return { pwms[channel], channel };
}

static int lua_startPwm(lua_State* l) {
	auto p = luaL_checkPwmIndex(l, 1);
	auto freq = luaL_checknumber(l, 2);
	auto duty = luaL_checknumber(l, 2);

	// clamp to 1..1000 hz
	freq = clampF(1, freq, 1000);

	startSimplePwmExt(
		&p.pwm, "lua", &engine->executor,
		CONFIG(luaOutputPins[p.idx]), &pins[p.idx],
		freq, duty
	);

	return 0;
}

void luaDeInitPins() {
	// Simply de-init all pins - when the script runs again, they will be re-init'd
	for (size_t i = 0; i < efi::size(pins); i++) {
		pins[i].deInit();
	}
}

static int lua_setPwmDuty(lua_State* l) {
	auto p = luaL_checkPwmIndex(l, 1);
	auto duty = luaL_checknumber(l, 2);

	// clamp to 0..1
	duty = clampF(0, duty, 1);

	p.pwm.setSimplePwmDutyCycle(duty);

	return 0;
}

static int lua_setPwmFreq(lua_State* l) {
	auto p = luaL_checkPwmIndex(l, 1);
	auto freq = luaL_checknumber(l, 2);

	// clamp to 1..1000 hz
	freq = clampF(1, freq, 1000);

	p.pwm.setFrequency(freq);

	return 0;
}

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

static auto lua_getAirmassResolveMode(lua_State* l) {
	if (lua_gettop(l) == 0) {
		// zero args, return configured mode
		return CONFIG(fuelAlgorithm);
	} else {
		return static_cast<engine_load_mode_e>(luaL_checkinteger(l, 1));
	}
}

static int lua_getAirmass(lua_State* l) {
	auto airmassMode = lua_getAirmassResolveMode(l);
	auto airmass = getAirmassModel(airmassMode);

	if (!airmass) {
		return luaL_error(l, "null airmass");
	}

	auto rpm = Sensor::get(SensorType::Rpm).value_or(0);
	auto result = airmass->getAirmass(rpm).CylinderAirmass;

	lua_pushnumber(l, result);
	return 1;
}

static int lua_setAirmass(lua_State* l) {
	float airmass = luaL_checknumber(l, 1);
	float engineLoadPercent = luaL_checknumber(l, 2);

	airmass = clampF(0, airmass, 10);
	engineLoadPercent = clampF(0, engineLoadPercent, 1000);

	luaAirmass.setAirmass({airmass, engineLoadPercent});

	return 0;
}

static int lua_stopEngine(lua_State*) {
	doScheduleStopEngine();

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
	lua_register(l, "startPwm", lua_startPwm);
	lua_register(l, "setPwmDuty", lua_setPwmDuty);
	lua_register(l, "setPwmFreq", lua_setPwmFreq);

	lua_register(l, "getFan", lua_fan);
	lua_register(l, "getDigital", lua_getDigital);
	lua_register(l, "setDebug", lua_setDebug);
	lua_register(l, "getAirmass", lua_getAirmass);
	lua_register(l, "setAirmass", lua_setAirmass);

	lua_register(l, "stopEngine", lua_stopEngine);
#endif
}
